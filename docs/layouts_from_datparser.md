# Member layouts from `datParser`

Midtown Madness 1 enumerated its fields with `DeclareFields`, and Open1560 got most of its
layouts for free from it. Midtown Madness 2 was believed to have dropped that: searching
`midtown2.map` for `DeclareFields` or `MetaClass` returns nothing.

The mechanism survived under a different name. `datParser` registers a field by pairing a
**name string** with a **pointer to that field**:

```
?AddRecord@datParser@@AAEAAVdatParserRecord@@HPBDPAXGPAVdatCallback@@@Z    0x4A79D0
?AddValue@datParser@@QAEXPBDPAMPAVdatCallback@@@Z                          0x406850   float*
?AddValue@datParser@@QAEXPBDPAHPAVdatCallback@@@Z                          0x45FC00   int*
?AddParser@datParser@@QAEPAV1@PBDPAVdatCallback@@@Z                        0x4A7980
```

When the pointer argument is `this + N`, the original 1999 code has stated a field's **name and
its byte offset outright**. `AddRecord`'s first argument states the **type** as well. This is the
strongest layout evidence available in the binary - it is not inference, it is the program
declaring its own layout so that `.CAR`, `.INI` and tune files can be read into it.

Output: `data/layouts_from_datparser.json`.

## The type enum, read out of the consumer

The record type is an integer at every call site. Its meaning is not guessed - it comes from
`datParser::Read` (`0x4A7CA0`), which is the code that consumes the record and therefore fixes
both the type and the **width**:

| Type | Meaning | Bytes | arts type |
|--:|---|--:|---|
| 0 | string, stride from `ValueSize` | var | `char[]` |
| 1 | bool - `*Data++ = GetInt() != 0` | **1** | `u8` |
| 2 | char | 1 | `i8` |
| 3 | short | 2 | `i16` |
| 4 | int | 4 | `i32` |
| 5 | float | 4 | `f32` |
| 6 | `Vector2` | 8 | `Vector2` |
| 7 | `Vector3` | 12 | `Vector3` |
| 8 | `Vector4` | 16 | `Vector4` |
| 9 | sub-parser (a nested object) | - | - |

Type 1 is the trap. It reads as `bool` but `datParser::Read` writes exactly **one** byte
(`*Data++`), so it is `u8`, never `b32`. Widening it would move every field after it by three
bytes. Four fields in this data set are type 1.

## Method

Two extractions were run **independently** and then compared.

**A - from the pseudocode.** Every `MM2_RE_KIT/MM2_PSEUDOCODE/<Class>/<Class>.c` and
`_FREE_FUNCTIONS.c` was split into functions by their `ea=` marker, every call to
`datParser::Add*` was parsed, and the pointer expression was resolved to a byte offset. Resolving
it needs the *declared* type of `this` in that function's signature, because Hex-Rays scales
pointer arithmetic: `this + 2243` is byte 2243 when `this` is `char *` and byte 8972 when it is
`float *`. `&this->gap1D0[116]` was resolved through the IDB type dump
(`_ALL_TYPES.h`): `0x1D0 + 116`.

**B - from the machine code.** A small x86 interpreter walks each function's bytes, tracks
`this` through `lea`, `add r32, imm`, `mov r32, r32` and `mov [ebp+X], r32`, and reads the five
pushed arguments at every `call` whose target is one of the four addresses above. This never sees
a decompiler's opinion at all.

**Every source function was identified by ADDRESS against `data/symbols.json`**, never by the
kit's label. That is not a formality - see the mislabels below.

Result: **318 of 318** comparable sites agree exactly between A and B. **Zero** disagreements.

Where they differ is where each one is blind, and both blind spots turned out to matter:

- B found registrations A had lost. In `camBaseCS::FileIO`, Hex-Rays rendered the pointer
  argument as `LogFileStream::Create`; the bytes say `push 5DC420h`, an absolute address.
- A found the *meaning* of two pointers B could only give as numbers, by naming
  `&MMSTATE.HudZoomIn`.

## Control group

Seven of the covered classes already have a real member layout in `data/layouts.json`, from the
community IDB - an entirely separate source. Running the extraction over them:

| | |
|---|--:|
| Members recovered in those 7 classes | 96 |
| Landed **exactly** on a known member offset | **87 (90.6%)** |
| Landed inside an unnamed filler (`gap`/`field_`), refining it | 9 |
| Landed inside a *named, typed* member - i.e. contradicted it | **0** |
| Landed past the IDB `sizeof` | **0** |
| Of the 87 exact hits, name identical to the IDB's | 86 |

Broken out per class:

| Class | Exact hits |
|---|--:|
| `asBirthRule` | 30 / 30 |
| `vehWheel` | 19 / 19 |
| `cityTimeWeatherLighting` | 10 / 10 |
| `vehTransmission` | 10 / 10 |
| `ltLight` | 9 / 9 |
| `vehCarDamage` | 9 / 9 |
| `vehCarSim` | 0 / 9 |

**The six classes whose IDB layout is field-level score 87/87 - 100%.** The nine misses are all
`vehCarSim`, where the IDB has no field-level layout at all: it is one 5006-byte
`gap1D0`. All nine land inside it, so they refine the known layout rather than contradict it.
Counting `vehCarSim` as nine misses is the pessimistic reading; the honest headline is
**90.6% exact, 0% contradicted**.

The extraction was also compared with `data/layouts_from_serialization.json`, a sibling recovery
built by a different route: **200 shared members, 0 disagreements**, and 17 members here that it
does not have.

## Contradictions found

Reported rather than smoothed over.

1. **`asBirthRule` +0x7C.** The IDB names this member `SpewRateLimit`. The string the binary
   registers is `"SpewTimeLimit"`. The registered string is the literal from the 1999 build and
   wins; the IDB name is a human's guess. This is the only name disagreement in 87 exact hits.

2. **`camTrackCS`: MM2Hook's `sizeof` is too small.** MM2Hook says 0x110 (272). The
   registrations run to 0x17C, implying a floor of 0x180 (384). `camTrackCS` derives from
   `camAppCS` (0x108), so fields from 0x10C up are exactly where its own members belong; the
   MM2Hook entry is a stub. `size` is therefore left **unknown** for this class rather than
   pinned to a value that would truncate it.

3. **`vehCarSim`: the two size sources disagree.** The IDB says 0x1560 (5472); MM2Hook says
   0x1590 (5520). The registrations reach 0x155C+4 = 0x1560, so both are consistent as floors and
   neither is refuted. `size` records MM2Hook's, the larger, with the disagreement in
   `conflicts`. Do **not** feed 0x1560 to `check_size` on the strength of this file alone.

4. **`vehSuspension::Copy` (0x4D9920) is a `FileIO`.** The linker map names this address
   `public: void __thiscall vehSuspension::Copy(class vehSuspension const &)`. Its 48 bytes are:

   ```
   55 8B EC              push ebp; mov ebp,esp
   6A 00                 push 0                    ; callback
   81 C1 84 00 00 00     add  ecx, 84h             ; this + 0x84
   6A 01                 push 1                    ; count
   51                    push ecx
   8B 4D 08              mov  ecx, [ebp+8]         ; the datParser
   68 E4 7A 5D 00        push offset "Mode"
   6A 04                 push 4                    ; type 4 = int
   E8 ..                 call datParser::AddRecord
   5D C2 04 00           pop ebp; ret 4
   ```

   That is a `FileIO(datParser&)` body, not a copy constructor - it never reads the source object.
   `vehSuspension` has no `FileIO` symbol in the map at all. The neighbours either side
   (`vehSuspension::Update`, `vehSuspension::'scalar deleting destructor'`) do match their names,
   so this is not a general off-by-one. The field attribution is safe regardless - `this` is a
   `vehSuspension` either way - and `0x84 + 4 = 0x88` is exactly MM2Hook's `sizeof(vehSuspension)`,
   so `Mode` is the class's last member.

## Mislabels the address check caught

| Address | Kit's label | The map's name | What it is |
|---|---|---|---|
| `0x5A88E0` | `MM2::aiPoliceOfficer::getComponentType` | `dgLinkData::FileIO` | 10 fields; trusting the label would have written a complete struct into the wrong class |
| `0x4094C0` | `mmInterface::~mmInterface` | same | the pseudocode shows `MM2::datParser::AddRecord(&NETMGR)`; **no such call exists in the bytes**. A junk label on the callee. |
| `0x43B140` | `mmGameMulti::QuitNetwork` | same | same phantom `AddRecord(&NETMGR)`; not in the bytes |

Two of the three are false *registrations*, not just false names. Extraction A alone would have
invented two classes' worth of nothing; extraction B rejected them.

## Pointers that are not members

Two registrations point at a fixed address rather than `this + N`. These are **config globals**,
recorded in `config_globals` and deliberately kept out of every class's `members`:

| Registered by | Name | Type | Retail address |
|---|---|---|---|
| `mmHudMap::FileIO` | `ZoomIn` | `u8` | `0x6B1987` = `MMSTATE + 0x377` |
| `camBaseCS::FileIO` | `CameraFar` | `f32` | `0x5D4420` (holds `600.0`) |

Both were confirmed twice. `MMSTATE` is at `0x6B1610` in the map and the IDB puts
`mmStatePack::HudZoomIn` at `+0x377`; `0x6B1610 + 0x377 = 0x6B1987`, exactly the address the
bytes push. And `camBaseCS`'s `CameraFar` being a global rather than a member is what makes
MM2Hook's `sizeof(camBaseCS) = 0x90` consistent: `CameraNear` at `0x8C` really is the last field.
Had `CameraFar` been recorded as a member at the "obvious" `0x90`, every derived camera class
would have been shifted by four bytes.

> The reference addresses come from a build whose `.data` sits `0x8000` above retail and whose
> `.rdata` sits `0x4000` above; `.text` is unshifted. The shift was measured, not assumed -
> `datBaseTokenizer::CommentChar` reads `59` (`';'`) and `datTimeManager::Seconds` reads
> `1/60` only at `+0x8000`. Addresses in the JSON are retail.

## Sub-objects

`AddParser(name, cb)` followed by a virtual dispatch on `this + N` registers a **nested object**.
The offset is recovered from the `ecx` of that dispatch and belongs to the **outer** class:

| Class | Sub-object | Offset | Inferred type | Why |
|---|---|--:|---|---|
| `vehCarSim` | `Engine` | `0x25C` | `vehEngine` | `0x2E0 - 0x25C = 132 = sizeof(vehEngine)` |
| `vehCarSim` | `Trans` | `0x2E0` | `vehTransmission` | `0x3D4 - 0x2E0 = 244 = sizeof` |
| `vehCarSim` | `Drivetrain` | `0x3D4` | `vehDrivetrain` | `0x420 - 0x3D4 = 76 = sizeof` |
| `vehCarSim` | `Freetrain` | `0x420` | `vehDrivetrain` | `0x4B8 - 0x420 = 152 = 2 x 76` |
| `vehCarSim` | `WheelFront` | `0x4B8` | `vehWheel[2]` | `0x990 - 0x4B8 = 1240 = 2 x 620` |
| `vehCarSim` | `WheelBack` | `0x990` | `vehWheel[2]` | `0xE68 - 0x990 = 1240 = 2 x 620` |
| `vehCarSim` | `AxleFront` | `0xE68` | `vehAxle` | `0xF04 - 0xE68 = 156 = sizeof` |
| `vehCarSim` | `AxleBack` | `0xF04` | `vehAxle` | same stride |
| `vehCarSim` | `Aero` | `0x14F0` | `vehAero` | `0x14F0 + 76 = 0x153C`, the next registered field |
| `vehTrailer` | `Drivetrain` | `0x344` | `vehDrivetrain` | |
| `vehTrailer` | `WheelFront` | `0x474` | `vehWheel[2]` | `0x94C - 0x474 = 1240` |
| `vehTrailer` | `WheelBack` | `0x94C` | `vehWheel[2]` | |

The chain is self-checking: **every** consecutive pair in `vehCarSim` differs by exactly the
independently-known `sizeof` of the class in between. That "WheelFront" and "WheelBack" are each
a *pair* of wheels also falls straight out of the arithmetic - a car has four.

The **offsets** are confirmed from the bytes. The **types** are marked `probable`, because the
dispatch is virtual and cannot be resolved statically; the arithmetic is what supports them.

`dgRagdollData::FileIO` (`0x5A8170`) is the one registration that pins nothing directly - it
`sprintf`s `"Link%d"` and hands each element of a heap array its own sub-parser. But the loop
around it is worth as much:

```
83 3F 00     cmp  dword ptr [this], 0    ->  i32 NumLinks   @ 0x00
8B 4F 04     mov  ecx, [this+4]          ->  dgLinkData*    @ 0x04
83 C3 30     add  ebx, 30h               ->  element stride  0x30
```

That stride is **exactly** `dgLinkData`'s registration floor, so `sizeof(dgLinkData) = 0x30` with
no tail padding - a size for a class neither MM2Hook nor the IDB has, arrived at independently.
`dgLinkData` comes out **completely recovered**: ten named fields filling all 0x30 bytes.

## What was recovered

**319 registration sites** in **31 functions**, covering **31 classes** and **306 named fields**,
plus 13 sub-object offsets and 2 config globals.

The map holds 35 `FileIO(datParser&)` symbols. Five of them - `asNode`, `asFileIO`, `mmInfoBase`,
`camCarCS`, `camViewCS` - are 16-byte stubs that register nothing. The 31st registering function
is not a `FileIO` at all: it is `vehSuspension::Copy` (see above).

| Class | Fields | Sub | Offset range | Floor | `size` | from | Code | New |
|---|--:|--:|---|--:|--:|---|--:|---|
| `mmPlayer` | 34 | 0 | 0x2260-0x2334 | 0x2338 | 0x23CC | MM2Hook | 13.3 KB | YES |
| `dgTrailerJoint` | 14 | 0 | 0x8-0xA8 | 0xAC | 0x11C | MM2Hook | 11.6 KB | YES |
| `camTrackCS` | 26 | 0 | 0x10C-0x17C | 0x180 | unknown | - | 9.5 KB | YES |
| `vehWheel` | 19 | 0 | 0x54-0x258 | 0x25C | 0x26C | MM2Hook | 8.6 KB | no |
| `vehCarSim` | 9 | 9 | 0x204-0x155C | 0x1560 | 0x1590 | MM2Hook | 6.4 KB | no |
| `vehCarDamage` | 9 | 0 | 0x24-0x388 | 0x38C | 0x3A4 | MM2Hook | 5.1 KB | no |
| `mmDashView` | 18 | 0 | 0x130-0x698 | 0x6A4 | 0x6DC | MM2Hook | 4.6 KB | YES |
| `ltLight` | 9 | 0 | 0x0-0x44 | 0x48 | 0x4C | MM2Hook | 4.6 KB | no |
| `mmHudMap` | 12 | 0 | 0x4C-0x8C | 0x94 | 0xC8 | MM2Hook | 4.3 KB | YES |
| `vehTrailer` | 4 | 3 | 0x310-0x338 | 0x94C | 0x1038 | MM2Hook | 3.9 KB | YES |
| `asBirthRule` | 30 | 0 | 0x18-0xAC | 0xB0 | 0xB4 | MM2Hook | 3.8 KB | no |
| `camAppCS` | 14 | 0 | 0x94-0xD4 | 0xD8 | 0x108 | MM2Hook | 3.0 KB | YES |
| `vehDrivetrain` | 3 | 0 | 0x40-0x48 | 0x4C | 0x4C | MM2Hook | 2.2 KB | YES |
| `vehTransmission` | 10 | 0 | 0x2C-0xF0 | 0xF4 | 0xF4 | MM2Hook | 2.0 KB | no |
| `vehEngine` | 6 | 0 | 0x18-0x34 | 0x38 | 0x84 | MM2Hook | 1.8 KB | YES |
| `vehStuck` | 6 | 0 | 0x2C-0x48 | 0x4C | 0x54 | MM2Hook | 1.6 KB | YES |
| `camPolarCS` | 6 | 0 | 0x110-0x124 | 0x128 | unknown | - | 1.2 KB | YES |
| `vehAero` | 5 | 0 | 0x20-0x48 | 0x4C | 0x4C | MM2Hook | 1.1 KB | YES |
| `vehGyro` | 5 | 0 | 0x1C-0x2C | 0x30 | 0x30 | MM2Hook | 1.0 KB | YES |
| `vehAxle` | 2 | 0 | 0x94-0x98 | 0x9C | 0x9C | MM2Hook | 1.0 KB | YES |
| `mmMirror` | 6 | 0 | 0x3C-0x60 | 0x64 | unknown | - | 928 B | YES |
| `camPovCS` | 4 | 0 | 0x114-0x140 | 0x144 | 0x148 | MM2Hook | 848 B | YES |
| `camPostCS` | 5 | 0 | 0x110-0x120 | 0x124 | unknown | - | 800 B | YES |
| `cityTimeWeatherLighting` | 10 | 0 | 0x0-0x48 | 0x4C | 0x4C | MM2Hook | 672 B | no |
| `camBaseCS` | 4 | 0 | 0x80-0x8C | 0x90 | 0x90 | MM2Hook | 608 B | YES |
| `vehSuspension` | 1 | 0 | 0x84-0x84 | 0x88 | 0x88 | MM2Hook | 544 B | YES |
| `aiVehicleData` | 13 | 0 | 0x18-0xA8 | 0xAC | 0xBC | MM2Hook | 512 B | YES |
| `camPreCS` | 4 | 0 | 0x110-0x124 | 0x128 | unknown | - | 496 B | YES |
| `dgLinkData` | 10 | 0 | 0x0-0x24 | 0x30 | 0x30 | ragdoll stride | 320 B | YES |
| `dgRagdollData` | 2 | 1 | 0x0-0x4 | 0x8 | unknown | - | 288 B | YES |
| `ptxGlassBirthRules` | 6 | 0 | 0x18-0x30 | 0x34 | unknown | - | 240 B | YES |

`Floor` is a **lower bound**, never a size. `size` is filled in only when an independent
`sizeof` - MM2Hook's, the IDB's, or in `dgLinkData`'s case an array stride - is at or above that
floor. Nothing here should reach `check_size` on the strength of a floor.

Every class's first registered field sits at or past the end of its base
(`camAppCS` 0x94 past `camBaseCS` 0x90, `camTrackCS` 0x10C past `camAppCS` 0x108,
`ptxGlassBirthRules` 0x18 past `asNode` 0x18, and so on) - so no field here is misattributed
to a derived class when it belongs to a base.

**24 of the 31 classes had no member layout at all before this.** They hold **67,376 bytes of
machine code, 4.40% of the binary**; the subset that also has a corroborated `sizeof`, and can
therefore be declared completely today with `u8 field_XX` fillers between the named members and
`check_size` pinned, is **53,619 bytes, 3.50%**.

## What this method cannot see

The honest limit, and it is a hard one: **only 31 functions in the entire binary register
anything.** `datParser` is a *tunables* system, not a reflection system. It sees the fields the
designers wanted to edit in `.CAR`, `.INI` and tune files, and nothing else.

- **It sees a slice, not a class.** `mmPlayer` is 0x23CC bytes and `datParser` names 34 fields,
  all of them in the input-tuning block from `0x2260` up. The first 0x2260 bytes are untouched.
  A partial layout does not make a class portable on its own.
- **No runtime state.** Pointers, matrices, link lists, counters, caches, anything the game
  computes rather than loads is invisible.
- **No array extents.** Every one of the 297 `AddRecord` calls passes `count = 1`. Nothing here
  recovers `float Foo[4]`.
- **No strings.** Type 0 never occurs, so no `char[N]` lengths.
- **A floor is not a size.** Registration says nothing about trailing members or tail padding.
  Only `dgLinkData` got a real size out of this method, and only because a stride pinned it.
- **Sub-object types are inferred, offsets are not.** The virtual dispatch cannot be resolved
  statically.
- **The other ~500 classes have no `FileIO` at all.** For them this lever does not exist.

What it *is* good for is the part that no other method gets right: **306 real 1999 field names**,
with exact offsets and exact widths, in classes where the alternative was `field_2C`.

## Reproducing

The extractor is not checked into `tools/` (that directory was owned by another agent while this
ran). The method is fully described above; the two passes are:

1. Parse `MM2_PSEUDOCODE/*/*.c` into functions by their `ea=` marker; for each `datParser::Add*`
   call, resolve the pointer argument against the first parameter's declared type and, when it is
   a struct pointer, against `_ALL_TYPES.h`.
2. Decode `.text` from `0x401000` for each function named in `data/symbols.json`, tracking `this`
   through `lea` / `add r32,imm` / `mov r32,r32` / `mov [ebp+X],r32`, and read the pushed
   arguments at each `call` to `0x4A79D0`, `0x406850`, `0x45FC00`, `0x4A7980`.

Pass 2 needs a byte image with `.text` at retail addresses; `build/OpenMM2.exe` is one. Note the
`.rdata`/`.data` shift described above when following a pointer out of `.text`.
