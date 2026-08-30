# Layouts from constructors

`data/layouts_from_ctors.json`, produced by `tools/ctor_layouts.py`. Regenerate with:

```
py tools/ctor_layouts.py
```

## Why this file exists

347 classes — **805.4 KB, 47% of all machine code** — have no member offsets, and a class cannot
be ported without them. The assembly still in `game.asm` reads those fields at fixed byte offsets,
so the offsets have to be declared before a single line of the class can be reimplemented. A size
on its own is not enough; `docs/PROGRESS.md` counts a class as *ready* only when the members are
known.

`docs/inferred_layouts.md` attacked this from every member access in the class's decompiled
methods. This attacks it from **constructors only** — a narrower signal, and a much better one.

## Why constructors

A constructor writes the object's members, usually in offset order. Every store reveals three
things at once: the offset, the access width (hence the type), and the initial value. It also
**bounds** the object — a constructor that initialises up to 0x40 and stops is real evidence about
where the class's own fields end, which accesses scattered through the rest of the class are not.

More importantly, a constructor is **structurally immune to the failure that sank the previous
attempt**. A base-class *method* is compiled once and runs on derived instances, so `mmGame::Init`
reading offset 0x7670 may be reading an `mmGameMulti` field through an `mmGame*`; nothing in the
pseudocode distinguishes that from a real member, and it is why `inferred_layouts` put `mmGame`'s
lower bound at 0x7684 against a supposed real size of 0x278. A base-class *constructor* only ever
runs on the base sub-object. The derived constructor calls it and then writes its own fields
itself. **Every offset here belongs to the class that wrote it.**

The base boundary is not inferred either. It is read directly: the `call Base::Base` at `this+0`
that opens the function names the base, and `data/symbols.json` names the callee.

## What is actually being read

`midtown2.exe`, disassembled with capstone — not the RE kit's pseudocode. Three reasons:

- all **665 constructors** are in the binary; the kit decompiled 444 of them;
- the access width is the instruction's operand size, not Hex-Rays' guess at a cast;
- the pointer-scaling trap that fabricated offsets last time (`this + 24` is offset 24 or offset
  96 depending on the declared type of `this`) **cannot arise**: machine code has no declared
  types. `this` is tracked explicitly instead.

A `__thiscall` constructor receives `this` in `ecx`. MSVC then uses `ecx` directly, copies it to a
callee-saved register, or spills it to a frame slot. The scanner tracks that one value through
`mov`, `lea`, `pop` and frame slots, invalidates it across every register a `call` clobbers
(`eax`, `ecx`, `edx`), and records **only** memory writes whose base register currently holds it.
Indexed writes (`[eax+ecx*4]`) are skipped outright, so a loop over an array contributes nothing
rather than contributing a wrong offset.

Two further signals come out of the same pass:

- **`lea ecx,[this+N]; call Foo::Foo`** — an embedded `Foo` at offset N. 300 of these, and they
  are what make the big composite classes tractable: `mmPlayer` holds fifteen named sub-objects,
  `aiVehiclePhysics` holds a `vehCar` at 0x10 and an `aiStuck` at 0x280.
- **Destructors.** What a destructor frees is what holds a pointer. 185 members are typed `void*`
  because `??1<Class>` loads them into `ecx` and calls `operator delete` or a destructor through
  them; 56 more offsets are known *only* from the destructor.

Every symbol is resolved **by address** against `data/symbols.json`, from the retail linker map.
The RE kit mislabels 19 of these constructors — `??0aiVehicleData@@QAE@XZ` is filed under
`asNode` — and the map wins.

> **Symbols ending `_SEH` are not constructors.** They are unwind funclets that share the
> constructor's name, and `this` is not in `ecx` in them. Scanning them produced roughly a hundred
> fabricated offsets, including a 0x7214 for the 0x8C-byte `ControlCustom`. They are skipped.

## How well it works

### The control group

188 classes have a real IDB struct. Grading against all of them would be grading against noise:
eight `Dialog_*` classes share one identical one-member entry of size 0xBC, which is IDA's
placeholder rather than a measurement. The control group is therefore restricted to classes whose
IDB struct has at least four named members, whose members reach at least halfway into the declared
size, and whose size is not contradicted by MM2Hook. That leaves **97 classes with a constructor**.

Then the reference is **held out**: `tools/ctor_layouts.py` is re-run with the IDB struct hidden
for those 97 classes, leaving only MM2Hook's independently-maintained `sizeof` — which is exactly
the evidence available for the 347 classes this file is actually for. The generated layout is then
compared against the struct it never saw.

| | offsets | share |
|---|--:|--:|
| landed exactly on a known member offset | 912 | 78.0% |
| landed inside a member the reference declares as an aggregate | 224 | 19.2% |
| landed inside a member the reference declares as a **scalar** | 7 | 0.6% |
| in range, no member there | 0 | 0.0% |
| **past the end of the reference struct** | 26 | 2.2% |

**97.2% of recovered offsets land on or inside a real member. 2.8% do not.**

The 19.2% "inside an aggregate" bucket is not error. It is the constructor writing sub-fields of
an embedded struct the reference declares as one member — `MenuManager::Scale` is a `Vector4` at
0xC4 and the constructor stores four floats at 0xC4, 0xC8, 0xCC, 0xD0. That is the extractor being
*more* precise than the reference, and it is why the "inside a scalar" case is broken out
separately: that one is a genuine contradiction, and there are seven of them. Three are
`asBirthRule::Color`, an `i32` at 0x8C written a byte at a time at 0x8D/0x8E/0x8F — again the
extractor being right and the reference being coarse.

### The grades separate the failures cleanly

| grade | offsets | exact | inside aggregate | contradicts a scalar | **past the end** |
|---|--:|--:|--:|--:|--:|
| CONFIRMED | 478 | 76.6% | 22.8% | 0.6% | **0.0%** |
| PROBABLE | 589 | 81.3% | 16.1% | 0.5% | **2.0%** |
| WEAK | 102 | 65.7% | 19.6% | 1.0% | **13.7%** |

This is the number that matters. **No CONFIRMED offset in the blinded control group lands past the
end of the object.** Unblinded — where the IDB struct is one of the corroborating sources —
neither CONFIRMED nor PROBABLE produces a single past-the-end offset, and all 26 fall in four
classes, every one graded WEAK:

| class | reference | constructor extent | why |
|---|--:|--:|---|
| `mmGame` | 0x278 | 0x7688 | the reference is a stub, see below |
| `NetSelectMenu` | 0x200 | 0x268 | the reference ends in `i8 unk2[228]`, a trailing blob |
| `RaceMenuBase` | 0x124 | 0x134 | reference two members short |
| `gizFerryMgr` | 0x2C | 0x31 | the constructor stores `200.0f` at 0x2C and a byte at 0x30 |

### Sizes: 35 emitted, 0 wrong

In the blinded run the generator emitted a size for 35 of the 97 control classes, and **none
disagreed with the held-out struct**. Be clear about what that does and does not prove. A
CONFIRMED size is not a prediction; it is a *coincidence test*. The gate fires only when the
constructor's own extent, computed from machine code, aligns exactly to a size that came from
somewhere else entirely. When two unrelated sources land on the same byte count, that is strong —
but the number itself is the other source's.

The genuinely predictive size signals are the two that need no reference at all, and they were
validated separately:

- **Array stride.** A class that constructs three or more instances of `T` at a constant stride
  pins `sizeof(T)` to that stride. Across the whole binary this rule fires 17 times where the size
  is known independently, and agrees **15 times**. The two disagreements are `camTrackCS` and
  `mmTimer`, and in both the reference is a base-only stub (below) — `mmPlayer` holds three
  `camTrackCS` at 0xE60, 0x10F8 and 0x1390, a stride of exactly 0x298, against MM2Hook's 0x110.
  Two instances is *not* enough (9 of the two-instance cases are wrong, because anything may sit
  between them), so the rule requires three.
- **Derived-class ceiling.** A derived class's constructor starts writing its own fields where the
  base ends. Graded on the 16 control classes where it fires: 12 exact, 3 loose but safe, **1
  wrong** — `DirSnd`, where a derived constructor writes into a base field. Because it has a
  demonstrated failure mode it is only ever recorded as corroborating evidence, never used to set
  a size.

### Reference stubs, and why they are called out rather than obeyed

Both reference sets contain placeholder declarations that look like measurements. Two shapes:

- **One-member IDB entries.** Eight `Dialog_*` classes, plus `Vehicle`, `ControlCustom`,
  `VehicleSelectBase`, `CrashCourse`, `mmRecord`, `RaceMenu` and others, carry a single unnamed
  member and a size. These are excluded from the control group.
- **Base-only declarations.** `mmTimer` is 0x18 in MM2Hook, which is exactly `sizeof(asNode)`; so
  are `AudManager`, `AudManagerBase` and `asNetObject`. `camTrackCS` is 0x110, exactly
  `sizeof(camCarCS)`. The body was never declared. The generator treats a reference as base-only
  **only when the constructor's own extent exceeds it**, downgrades that from a contradiction to a
  note, and then refuses to emit a corroborated size from it.

`mmGame` is the clearest case, and worth stating plainly because `docs/inferred_layouts.md` names
it as the counterexample that should not be trusted. Its constructor calls
`mmPlayerConfig::mmPlayerConfig` at `this+0x410`, and `mmPlayerConfig` is 0x716C bytes, so `mmGame`
cannot be smaller than 0x757C. It then writes at 0x7588 and 0x7670–0x7684. `mmGameMulti` is 0xB1FC.
**Both references say 0x278, and both are the same partial declaration.** A constructor cannot be
running on a derived object, so the explanation that applied to `mmGame::Init` does not apply here.
`mmGame` is still graded WEAK — the entry records the contradiction rather than resolving it — but
the 0x278 should not be trusted either.

## What was recovered

Against the 347 classes `docs/PROGRESS.md` counts as blocked (805.4 KB):

| grade | classes | code | share of blocked code |
|---|--:|--:|--:|
| CONFIRMED | 54 | 210.7 KB | 26.2% |
| PROBABLE | 111 | 325.4 KB | 40.4% |
| WEAK | 95 | 143.2 KB | 17.8% |
| nothing recovered | 87 | 126.1 KB | 15.7% |

2,183 observed members across those 260 classes, from 277 constructors and 237 destructors.
Counting the classes whose IDB struct is a stub as well, the file covers 286 classes; the totals
in `_meta` are for all 415 classes with a usable constructor, control group included
(449 constructors scanned in total).

### CONFIRMED

`Unobserved` is the share of the object no constructor or destructor touched. It is the honest
measure of how finished a layout is, and it is why a CONFIRMED grade is a statement about
*correctness*, not *completeness* — see the next section.

| Class | Code | Size | Observed members | Unobserved | Base | Size corroborated by |
|---|--:|--:|--:|--:|---|---|
| `mmSingleStunt` | 20.2 KB | `0x7970` | 14 | 100% | `mmGameSingle` | MM2Hook sizeof |
| `phBoundTerrain` | 16.4 KB | `0xCC` | 16 | 70% | `phBoundGeometry` | MM2Hook sizeof |
| `mmMultiCR` | 15.8 KB | `0xB2A8` | 21 | 100% | `mmGameMulti` | MM2Hook sizeof |
| `phBoundHotdog` | 14.3 KB | `0x84` | 5 | 52% | `phBound` | MM2Hook sizeof |
| `aiVehicleSpline` | 10.6 KB | `0x18C` | 14 | 21% | `aiVehicle` | MM2Hook sizeof |
| `camTrackCS` | 9.5 KB | `0x298` | 88 | 25% | `camCarCS` | array stride in `mmPlayer` |
| `aiSubway` | 8.9 KB | `0x14C` | 14 | 45% | `aiVehicle` | MM2Hook sizeof |
| `mmWaypoints` | 8.5 KB | `0x90` | 23 | 42% | `asNode` | MM2Hook sizeof |
| `aiCableCar` | 8.0 KB | `0x184` | 12 | 16% | `aiVehicle` | MM2Hook sizeof |
| `dgPhysManager` | 7.5 KB | `0x12B0` | 6 | 99% | - | MM2Hook sizeof |
| `mmMultiRace` | 7.1 KB | `0xB208` | 6 | 100% | `mmGameMulti` | MM2Hook sizeof |
| `mmMultiCircuit` | 7.0 KB | `0xB208` | 5 | 100% | `mmGameMulti` | MM2Hook sizeof |
| `DMusicObject` | 5.0 KB | `0x38` | 14 | 5% | - | MM2Hook sizeof |
| `mmDashView` | 4.6 KB | `0x6DC` | 36 | 25% | `asLinearCS` | MM2Hook sizeof |
| `phBoundSphere` | 4.4 KB | `0x80` | 11 | 31% | `phBound` | MM2Hook sizeof |
| `mmHudMap` | 4.3 KB | `0xC8` | 33 | 38% | `asNode` | MM2Hook sizeof |
| `aiRaceData` | 3.9 KB | `0xC8` | 16 | 74% | - | MM2Hook sizeof |
| `vehTrailer` | 3.9 KB | `0x1038` | 26 | 12% | - | MM2Hook sizeof |
| `Aud3DObject` | 3.1 KB | `0x60` | 25 | 2% | - | MM2Hook sizeof |
| `camAppCS` | 3.0 KB | `0x108` | 28 | 58% | `camBaseCS` | MM2Hook sizeof |
| `aiVehicleActive` | 3.0 KB | `0xAFC` | 13 | 6% | - | MM2Hook sizeof |
| `mmIcons` | 2.8 KB | `0xBC` | 38 | 19% | `asNode` | MM2Hook sizeof |
| `vehSurfaceAudioData` | 2.5 KB | `0x38` | 12 | 14% | - | MM2Hook sizeof |
| `aiVehicleAmbient` | 2.5 KB | `0x1A0` | 5 | 95% | `aiVehicleSpline` | MM2Hook sizeof |
| `aiGoalAvoidPlayer` | 2.5 KB | `0x20` | 4 | 50% | - | MM2Hook sizeof |
| `dgUnhitBangerInstance` | 2.4 KB | `0x18` | 3 | 71% | `lvlInstance` | MM2Hook sizeof |
| `mmWaypointObject` | 2.3 KB | `0x5C` | 16 | 30% | `asNode` | MM2Hook sizeof |
| `vehDrivetrain` | 2.2 KB | `0x4C` | 8 | 58% | `asNode` | MM2Hook sizeof |
| `aiVehicleManager` | 2.1 KB | `0x177A4` | 5 | 100% | `asNode` | MM2Hook sizeof |
| `ltLensFlare` | 1.6 KB | `0x20` | 8 | 0% | - | MM2Hook sizeof |
| `phJoint` | 1.6 KB | `0x6C` | 16 | 0% | - | MM2Hook sizeof |
| `MMDMusicManager` | 1.6 KB | `0x58` | 8 | 74% | `DMusicManager` | MM2Hook sizeof |
| `aiCityData` | 1.6 KB | `0x34` | 13 | 10% | - | MM2Hook sizeof |
| `phSleep` | 1.3 KB | `0x50` | 4 | 84% | - | MM2Hook sizeof |
| `vehAero` | 1.1 KB | `0x4C` | 14 | 26% | `asNode` | MM2Hook sizeof |
| `mmExternalView` | 1.1 KB | `0x104` | 15 | 12% | `asNode` | MM2Hook sizeof |
| `dgBangerActiveManager` | 1.1 KB | `0x639C` | 3 | 100% | `asNode` | MM2Hook sizeof |
| `lvlMaterial` | 1.1 KB | `0x4C` | 11 | 47% | `phMaterial` | MM2Hook sizeof |
| `vehGyro` | 1.0 KB | `0x30` | 8 | 33% | `asNode` | MM2Hook sizeof |
| `asMeshSetForm` | 1.0 KB | `0x60` | 7 | 25% | `asNode` | MM2Hook sizeof |
| `vehAxle` | 1.0 KB | `0x9C` | 12 | 13% | `asNode` | MM2Hook sizeof |
| `camViewCS` | 981 B | `0x4C` | 11 | 39% | `asNode` | MM2Hook sizeof |
| `mmCityList` | 896 B | `0x10` | 4 | 0% | - | MM2Hook sizeof |
| `camPovCS` | 848 B | `0x148` | 33 | 60% | `camCarCS` | MM2Hook sizeof |
| `dgBangerManager` | 709 B | `0x24` | 4 | 56% | `asNode` | MM2Hook sizeof |
| `mmTimer` | 637 B | `0x30` | 7 | 42% | `asNode` | array stride in `mmHUD` |
| `camBaseCS` | 608 B | `0x90` | 10 | 12% | `asNode` | MM2Hook sizeof |
| `aiGoalCollision` | 544 B | `0x10` | 4 | 0% | - | MM2Hook sizeof |
| `vehSuspension` | 544 B | `0x88` | 6 | 18% | `asNode` | MM2Hook sizeof |
| `asLinearCS` | 384 B | `0x80` | 6 | 12% | `asNode` | MM2Hook sizeof |
| `dgHitBangerInstance` | 352 B | `0x58` | 4 | 38% | `lvlInstance` | MM2Hook sizeof |
| `mmWaypointInstance` | 274 B | `0x5C` | 8 | 24% | `lvlInstance` | MM2Hook sizeof |
| `mmJaxis` | 256 B | `0x1C` | 5 | 29% | - | array stride in `mmJoystick` |
| `vehBreakable` | 128 B | `0x4C` | 8 | 4% | - | MM2Hook sizeof |

### PROBABLE, the twelve largest

`size` is `null` for all of these. `size_lower_bound` is a floor and nothing more.

| Class | Code | Lower bound | Observed members | Base | Independent size, if any |
|---|--:|--:|--:|---|--:|
| `aiVehiclePhysics` | 58.7 KB | `0x898` | 6 | `aiVehicle` | `0x9770` |
| `aiPedestrian` | 23.7 KB | `0xC4` | 4 | - | - |
| `mmPlayer` | 13.3 KB | `0x23A4` | 94 | `asNode` | `0x23CC` |
| `aiGoalRandomDrive` | 13.2 KB | `0x10` | 4 | - | `0x14` |
| `mmSingleCircuit` | 6.8 KB | `0x771C` | 7 | `mmGameSingle` | `0x7720` |
| `mmSingleBlitz` | 6.3 KB | `0x770C` | 13 | `mmGameSingle` | `0x7710` |
| `AudioOptions` | 6.3 KB | `0x7374` | 20 | `OptionsBase` | - |
| `mmHUD` | 6.0 KB | `0xB98` | 32 | `asNode` | `0xBBC` |
| `crLegData` | 6.0 KB | `0x30` | 12 | - | - |
| `vehSurfaceAudio` | 5.0 KB | `0x80` | 31 | - | `0x84` |
| `mmPlayerData` | 4.9 KB | `0x294` | 9 | `mmInfoBase` | base-only stub |
| `aiVehicleInstance` | 4.9 KB | `0x30` | 10 | `lvlInstance` | `0x3C` |

## What this method cannot see

**A CONFIRMED grade is about correctness, not completeness.** It says every recovered offset is
real and the object's total size is corroborated. It does not say the layout is finished. The
`Unobserved` column above is the point: `mmSingleStunt` is CONFIRMED at 0x7970 and **100% of it is
unobserved** — its fourteen members all sit in the last 0x30 bytes, and everything below is an
`mmGameSingle` base that is itself unresolved. Across the 54 CONFIRMED blocked classes the median
unobserved share is 32%, and only 35 of them (98 KB of code) are under 50%.

This is safe rather than dangerous because of how gaps are declared. Every entry **tiles `[0,
size)` exactly**: unobserved runs become `u8 field_<hex>[n]` blocks bounded by the two observed
offsets around them, so every known offset lands where it belongs and nothing is invented. All 415
entries pass that check. But a class that is 90% gap is not ready to port — it is ready to
*continue*, with the offsets that matter already pinned.

**A constructor only writes what it initialises.** Members set exclusively by `Init`, `Reset`, a
loader, or the remaining assembly are invisible here. `aiVehiclePhysics` is the extreme case: 58.7
KB of code, a 0xA0-byte constructor, and six recovered members. `docs/inferred_layouts.md` covers
exactly the accesses this file misses, and the two are complementary rather than competing — for
any class, read both.

**Loops are seen once.** The scanner disassembles linearly, so a loop that constructs an array
contributes only its first element. `aiVehiclePhysics` shows two `aiRouteNode` constructions 0x5A0
apart where `sizeof(aiRouteNode)` is 0x24: those are two arrays of 40, and only the first element
of each is recorded. The floor is correct and the extent understates.

**Indexed writes are dropped on purpose.** `mov [eax+ecx*4], edx` has no constant offset, so
nothing is recorded. Refusing to guess costs coverage and buys the 0% hole rate in the table above.

**87 blocked classes produced nothing at all** (126.1 KB). 78 of them have **no constructor symbol
in the linker map** — the compiler inlined a trivial one, or the object is built by a factory.
Nine have a constructor that writes nothing to `this`: `DMusicManager`, `aiPoliceForce`,
`aiRouteNode`, `aiRouteRacer`, `cityPropulator`, `mcHookman`, `pedActiveData`, `phIntersection`,
`phIntersectionPoint`. The largest misses are `gfxPipeline` (21.1 KB), `phCollision` (7.0 KB),
`lvlAiMap` (5.8 KB) and `audManager` (5.0 KB), all constructor-less. For these the next attempt has
to look at the *callers* — who allocates the object and what it writes into it — not at the class.

Note the one case that looks like a miss and is not: `aiPoliceOfficer`'s entire constructor is a
single `call aiVehiclePhysics::aiVehiclePhysics` at `this+4`. That yields no member writes but does
establish that `aiPoliceOfficer` **contains** an `aiVehiclePhysics` at offset 4 rather than
deriving from it, which is more than any offset would have told you.

**Types are width-derived, and width is not type.** A 4-byte store is `i32` unless the value looks
like a float or an FPU store wrote it, in which case it is `f32`. Pointers are only identified
where the destructor frees them. `u32` versus `i32` versus a pointer is a guess; the *width* is
not, and width is what keeps the layout intact.

**Do not fill `check_size` from `size_lower_bound`.** The reasoning in
`docs/inferred_layouts.md` applies unchanged: a floor pinned as a size either allocates a short
object that the original code writes past — heap corruption with no test that catches it — or
fails an assert that was never a real contradiction. `size` is `null` unless an independent source
corroborated the extent; use that field, and only that field.

## Schema

Same shape as `data/layouts.json` — a flat map of class name to entry — plus a leading `_meta` key.
`size` is `null` unless corroborated, so a `layouts.json` consumer handed this file fails loudly
rather than silently pinning a bound.

```
"<Class>": {
  "kind": "struct",
  "size": 200 | null,               // only when an independent source corroborates the extent
  "size_source": "MM2Hook sizeof" | "IDB + MM2Hook agree" | "array stride in <Class>" | null,
  "size_lower_bound": 200,          // a FLOOR. never a size.
  "confidence": "CONFIRMED" | "PROBABLE" | "WEAK",
  "constructors": [ { "mangled": "??0mmHudMap@@QAE@XZ", "rva": "0x42EB60", "code_size": 272 } ],
  "destructors":  [ { "mangled": "??1mmHudMap@@UAE@XZ", "rva": "0x42EC70", "code_size": 16 } ],
  "base": "asNode",                 // from the call at this+0, not inferred
  "base_size": 24,
  "polymorphic": true,              // the constructor stores a vftable at offset 0
  "reference": { "idb": null, "idb_is_stub": false, "mm2hook": 200,
                 "reference_is_base_only": false },
  "contradicts_reference": false,
  "class_code_bytes": 4391,
  "evidence": [ ... ],              // in English, including how the size was corroborated
  "overlapping_writes": [ ... ],    // unions and byte-wise stores into wider fields
  "members": [
    { "offset": 28, "offset_hex": "0x1C", "type": "i32", "name": "field_1C", "count": 0,
      "width": 4, "init": "0x1", "src": "ctor", "ctor_sites": 1 },
    { "offset": 32, "offset_hex": "0x20", "type": "u8",  "name": "field_20", "count": 12,
      "width": 1, "unknown": true, "from_base": false }
  ]
}
```

**The grade describes the member evidence, not the size.** `size` is set independently, whenever
the constructor's extent was corroborated, so a WEAK entry can still carry a trustworthy size —
17 do, all tiny classes where the constructor found only two members but the extent matched two
agreeing references exactly. A *contradicted* size is never emitted: those entries have `size`
null and `contradicts_reference` true.

`unknown` marks a gap: bytes nothing touched, bounded by the observed offsets on either side and
declared as `u8 field_X[n]` so the following offsets stay put. `from_base` marks an offset below
`base_size`. `src` is `ctor`, `dtor`, `ctor+dtor`, or `constructed in place` for a sub-object
identified by a `lea ecx,[this+N]; call Foo::Foo`. `freed_by_destructor` means the destructor
passes the field to `operator delete`, so it holds a pointer. `width_conflict` records an offset
written at two different widths.
