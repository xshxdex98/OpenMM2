# Inferred layouts

`data/inferred_layouts.json`, produced by `tools/infer_layouts.py`. Regenerate with:

```
python tools/infer_layouts.py
```

## Why this file exists

A linker map lists functions and statics but never member offsets, so instance layout is the one
thing the map cannot give us — and headers need it twice, for `check_size(Class, 0xNNN)` and for
member declarations at the right byte offsets.

`tools/layouts.py` solves that for the 187 classes the community IDB has a struct for.
`data/mm2types.json` (MM2Hook's `sizeof`) covers 265, overlapping. Between them, 339 of 537
classes have something. The remaining classes had nothing at all.

This file is a third source for those: the decompiled bodies. Every time a class's own method
reads `*(float *)((char *)this + 0x24)`, that is one observed member at offset 0x24, four bytes
wide, probably a float.

## What is actually being read

`MM2_RE_KIT/MM2_PSEUDOCODE/<Class>/<Class>.c`, one file per class. A function is used only when
it is `__thiscall` (or its first parameter is literally named `this`) and the demangled name says
it belongs to this class. Static and `__stdcall` helpers are skipped, because their first
parameter is not the object.

Three access forms are collected, and each member records which ones produced it:

| form | example | how the offset is obtained |
|---|---|---|
| `deref` | `*(float *)((char *)this + 0x24)`, `*(_DWORD *)(a1 + 36)`, `*((_BYTE *)this + 5)` | the literal byte offset, or the constant scaled by the *cast's* pointee width |
| `index` | `a1[9]` where `a1` is `_DWORD *` | the index scaled by the pointee width — **9 is not offset 9, it is offset 36** |
| `field` | `this->Manager` | resolved through the *declared type of `this`* in `layouts.json` |

Widths map to primitives the way `tools/layouts.py` does it — 1 → `u8`, 2 → `u16`, 4 → `f32` when
the cast is `float` and `u32` otherwise, 8 → `f64`/`u64` — because width is what keeps a layout
intact. A member whose name is only recoverable as an offset is written `field_<HEXOFFSET>`, the
same convention `layouts.py` normalises to.

The `field` form contributes almost nothing in practice (17 of 3961 members come from it alone,
0.4%). For a class the IDB has no struct for, IDA usually typed `this` as that same unknown
class, so there is nothing to resolve against. That is convenient: it means this file is very
nearly independent of `layouts.json`, which is what makes the validation below meaningful.

### The scaling trap, and a real bug it caused

Pointer arithmetic is where this goes wrong quietly. `this + 24` is offset 24 when `this` is
`int`, but offset 96 when `this` is `_DWORD *`. Both spellings appear, sometimes in adjacent
methods of the same class, so the declared type of the first parameter has to be consulted every
time.

The first version of this tool also scaled by *struct* widths, and validation caught it: seven
classes came out with a lower bound of exactly twice their real size. The cause was
`this[3].float10` in a method where IDA had typed `this` as `lvlSky *` — scaled by `sizeof(lvlSky)`
that is offset 0x88, in a class only 0x28 bytes long. IDA had mistyped the variable and the code
was really doing byte arithmetic on a larger object. `tools/infer_layouts.py:elem_step` now
refuses struct strides outright. Hex-Rays uses `->` or `(char *)this + N` when it genuinely
believes a struct type, and those are read elsewhere, so the refusal costs almost no coverage and
removed the single largest source of fabricated offsets: past-the-end offsets fell from 43 to 33,
and classes whose lower bound exceeded their real size fell from 13 to 5.

## The hard limits

**A lower bound is not a size.** `max(offset + width)` over the offsets that happen to be *read*
says the object is at least that big. It says nothing about:

- trailing members nothing in the class's own methods touches — a buffer only `memset` clears, a
  field only a friend class or the remaining assembly reads;
- tail padding, which the compiler adds and no code ever mentions;
- members between two observed offsets. A gap in the offsets is a gap in the *evidence*, not in
  the struct.

**Offsets can belong to a different object.** A base-class method is compiled once and run on
derived instances, so `mmGame::Init` reading offset 0x7670 is reading a field of `mmGameMulti`
(0xb1fc) through an `mmGame*`. Nothing in the pseudocode distinguishes that from a real member.
Two partial mitigations are recorded per class rather than silently applied:

- `outlier_offsets` — offsets sitting past a hole wider than 0x400 in the offset histogram, which
  is what a jump into a much larger derived object looks like. This catches `mmGame`'s 0x7408+
  cluster. It does *not* catch `phInertialCS`, whose overrun into (presumably) `dgPhysEntity` is a
  contiguous run of floats from 0x11C to 0x14C with no hole to find.
- `size_lower_bound_confident` — offsets seen at two or more independent call sites. Across the
  gradeable classes, requiring two sites cut past-the-end offsets from 4.1% to 2.2% of offsets, at
  the cost of roughly half the coverage.

**Unions and sub-field reads look like conflicts.** Where the same offset is read at two widths,
both are recorded and the class carries a `conflicts` entry; nothing picks a winner. 17 classes
have at least one. `overlaps` flags the related case of a member's extent running into the next
observed offset (5 classes).

## Do not fill `check_size` from this

`tools/genheaders.py` deliberately emits

```cpp
// check_size(Foo, 0x0); // TODO: no layout in the IDB type library
```

rather than a guess, and this file is not the thing that lets you fill it in.

`check_size` is a `static_assert` that pins the C++ type to an exact byte count. Fill it from a
lower bound and one of two things happens. If the assert is satisfied — because the members were
trimmed to match — every `new Foo` allocates a short object, and the original code, still reading
the real offsets from `game.asm`, writes past the end of it. That is heap corruption whose crash
lands somewhere unrelated, and there is no test that catches it. If the assert fails, you have
spent a day on a contradiction that was never real, because the number was never a size.

The concrete shape of the risk: of the 124 classes here that can be graded against a real size,
the lower bound *equals* the true size for 67 and falls short for 47. Roughly a third would have
been pinned too small, silently.

Every entry carries `"size": null` and a `size_note` saying so, the three floors are named
`size_lower_bound*` rather than `size`, and the whole map is nested under a `"layouts"` key so it
cannot be handed to a `layouts.json` consumer by accident. Where a real size does exist elsewhere,
the entry says so in `size_known_elsewhere` — use that one.

What this file *is* good for: knowing which offsets are live and how wide they are, which is
enough to declare members and to read the remaining assembly, and enough to tell a promising class
from a hopeless one before spending a day on it.

## How well it works

### Against `layouts.json` (74 classes with a real IDB struct and enough members to grade)

The `field` form is excluded here — it resolves through `layouts.json` and would be grading
itself. Raw byte-offset forms only.

| | offsets | share |
|---|--:|--:|
| landed exactly on a known member offset | 523 | 65.0% |
| landed inside a known member's extent | 190 | 23.6% |
| in range, no known member there | 58 | 7.2% |
| **past the end of the known struct** | **33** | **4.1%** |

88.7% land on or inside a real member. The "inside" bucket is mostly the inference being *more*
precise than the IDB: `aiPath`'s only non-exact offset is 0x28, which falls inside the IDB's
`field_24`, an eight-byte unnamed gap. The inference found a live 4-byte read in the middle of it.

Lower bounds exceed the real size for 5 of 74 classes; the stingy `confident` bound for 3.

The four named check classes:

| class | known size | inferred offsets | exact | inside | past end | lower bound |
|---|--:|--:|--:|--:|--:|--:|
| `aiPath` | 0x164 | 49 | 48 | 1 | 0 | 0x140 ✓ |
| `asNode` | 0x18 | 0 | — | — | — | 0x0 |
| `mmGame` | 0x278 | 28 | 4 | 13 | 11 | 0x7684 ✗ |
| `vehWheel` | 0x26C | 13 | 13 | 0 | 0 | 0x26C ✓ |

`aiPath` and `vehWheel` are clean: every inferred offset is a real one and the bound holds.
`asNode` produces nothing — its methods are written entirely as `this->parent`, which the honest
check excludes, so it is a coverage miss, not a wrong answer.

`mmGame` is a genuine failure and the reason the derived-object limit above is stated so plainly.
Its lower bound of 0x7684 is 44× the real 0x278. Eleven offsets are past the end, all of them
reads into `mmGameMulti` through an `mmGame*`. Outlier quarantine drops it to 0x4B8, which is
still wrong. **If you were going to trust one number in this file, `mmGame` is the counterexample.**

### Against `data/mm2types.json` (133 classes, fully independent)

MM2Hook's `sizeof` table is never read by the inference, and it covers 133 classes that the IDB
type dump has no struct for at all — so this grades exactly the population the file is *for*,
not a proxy. Nine classes are excluded because MM2Hook declares them empty and reports 1 or 2
bytes, which is a placeholder rather than a measurement.

| | classes |
|---|--:|
| graded | 124 |
| lower bound within the real size | 114 (92%) |
| lower bound *equals* the real size | 67 (54%) |
| lower bound exceeds the real size | 10 |

Median `lower_bound / real_size` is 1.00.

The ten that exceed are listed in `_meta.mm2types_cross_check.exceeded`, and they are not all
inference errors — some are the grader being short. `camTrackCS` is the clearest: MM2Hook says
0x110, this inference says 0x298, and `mmPlayer` holds three `camTrackCS` instances at 0xE60,
0x10F8 and 0x1390 — a stride of exactly 0x298. MM2Hook only declared part of the class. Same story
for `AudManagerBase` (MM2Hook 0x18, which is just its `asNode` base; the IDB's derived `AudManager`
is 0x54, and the inference says at least 0x28). `mmGameSingle` is the opposite case and a real
failure, in the `mmGame` family again.

## Coverage: who got something and who did not

350 classes have no struct in `layouts.json`. **304 gained members**, 3961 in total, median 8 per
class, mean 13.

| members recovered | classes |
|---|--:|
| 50+ | 8 |
| 21–50 | 48 |
| 11–20 | 70 |
| 6–10 | 77 |
| 3–5 | 59 |
| 1–2 | 42 |
| nothing | 42 |
| no pseudocode file | 4 |

### Useful coverage

The best results are the big stateful gameplay and camera classes, which is convenient, because
they are also the ones blocking the most code. `mmDashView` recovered 150 members and its lower
bound of 0x6DC is *exactly* MM2Hook's size — that class is essentially solved. `mmPlayer` (105
members, 0x23A4 against a real 0x23CC), `camTrackCS` (93), `vehTrailer` (59), `aiVehicleSpline`
(56), `mmHUD` (53), `aiSubway` (52), `aiPedestrian` (51), `RadialGauge` (47), `asViewCS` (43),
`aiCableCar` (41), `mmIcons` (41), `audManager` (39). Several more hit their real size on the
nose: `mmIcons` 0xBC, `camPovCS` 0x148, `mmHudMap` 0xC8, `camAppCS` 0x108, `mmWaypoints` 0x90,
`phBoundTerrain` 0xCC, `RadialGauge` 0x114.

171 of the 304 have no size from any other source, so this is the only layout information that
exists for them.

### Almost nothing

42 classes produced no member at all:

> `AgeDevice`, `AudMidi`, `AudStream`, `EffectBase`, `Matrix66`, `PUReplaySave`, `Quaternion`,
> `VolumeDW`, `asCullable`, `audCD`, `audMemMgr`, `audMemObj`, `audMixer`, `cityPropulator`,
> `datArgParser`, `datAssetManager`, `datOutput`, `datReplay`, `datStack`, `datTimeManager`,
> `dgUnhitMtxBangerInstance`, `gfxPipeline`, `ioEventQueue`, `ioInput`, `ioKeyboard`, `ioMouse`,
> `lvlAiMap`, `lvlLandmark`, `lvlLevelBound`, `lvlProgress`, `mmAccelCompute`, `mmGameHUD`,
> `mmGameMusicData`, `mmSingleRaceMusicData`, `mmSingleRoamMusicData`, `mmSlidingGauge`,
> `phBoundCollision`, `phCollision`, `phCollisionPrim`, `phConvexPoly`, `phIntersection`,
> `ptxGlassBirthRules`

A further 42 produced one or two members, which is not a layout — among them `Base`,
`ControlBase`, `aiGoal`, `dgBoundBox`, `dgBoundSphere`, `dgBoundGeometry`, `dgBoundHotdog`,
`mmDamage`, `mmJoyMan`, `winDispatchable`, and most of the `PU*`/`Dialog_*` menu classes.

Four have no pseudocode file at all: `BMLabel`, `IDirectMusicGetLoader`, `dgLinkData`,
`phPhysicsManager`.

These fail for structural reasons, not fixable ones. Pure-interface and near-empty classes
(`asCullable`, `Base`, `EffectBase`, `phCollision`) have no state to observe. Classes whose
methods are one-line forwarders (`ioMouse`, `ioKeyboard`, `datOutput`) touch statics, not members.
`Quaternion` and `Matrix66` are value types whose operations were inlined into their callers, so
the out-of-line bodies that survive touch nothing. For all of these, the pseudocode simply does
not contain the information, and a different input — the remaining `game.asm`, or the callers
rather than the class itself — is where the next attempt should look.

## Schema

```
{
  "_meta": { counts, member_forms, sanity_check, global_validation, mm2types_cross_check, ... },
  "layouts": {
    "<Class>": {
      "INFERRED": true,
      "size": null,                          // always. see above.
      "size_lower_bound":            0x28,   // every observed offset
      "size_lower_bound_dense":      0x28,   // minus offsets past a >0x400 hole
      "size_lower_bound_confident":  0x28,   // minus offsets seen at only one call site
      "outlier_offsets": [],
      "size_known_elsewhere": { "source": "data/mm2types.json", "size_hex": "0x18" } | null,
      "floor_from_base":      { "base": "asNode", "size_hex": "0x18" } | null,
      "ceiling_from_derived": { "derived": "AudManager", "size_hex": "0x54" } | null,
      "this_types": { "AudManager*": 9, "void*": 3, "_DWORD*": 3, "int": 2 },
      "methods_in_map": 25, "methods_in_file": 19, "methods_with_this_access": 13,
      "members": [
        { "offset": 24, "offset_hex": "0x18", "type": "u32", "name": "Manager",
          "count": 0, "width": 4,
          "sites": 12,               // distinct methods that read this offset - the confidence
          "hits": 17,                // total occurrences
          "forms": ["deref", "field", "index"],
          "conflict": false,         // another width was seen at this same offset
          "suspect_outlier": false }
      ],
      "conflicts": [ ... ], "overlaps": [ ... ]
    }
  }
}
```

`floor_from_base` and `ceiling_from_derived` come from `data/hierarchy.json` and are independent
of the pseudocode: a class is at least as big as its base, and no bigger than any class that
derives from it. Treat the ceiling as advisory — it is only as good as the derived class's own
size, and one pair (`mmPopup` at 0x1 under `asNode` at 0x18) already contradicts itself.

`this_types` is worth reading before trusting a class. A class whose methods IDA typed
consistently — `aiPath *this` throughout — is far safer than one split across `int`, `_DWORD *`,
`char *` and a derived class, which is where the scaling and derived-object problems live.
