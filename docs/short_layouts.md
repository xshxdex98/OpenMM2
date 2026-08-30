# Class sizes from `operator new`, and the 28 layouts that disagree with them

Two classes were found by hand to be short of what `BeginPhase` allocates. This is the sweep of the
whole binary for the same class of error, in both directions.

## Applied, and what now enforces it

**All 28 are now in `data/layouts.json`, and `tools/verify_sizes.py` reports PASS.**
It is the gate: it re-runs this sweep against the binary on every build (`tools/build.py` step 2)
and names every remaining disagreement. Since the list has reached zero, the advisory in
`build.py` step 2 can now become a `sys.exit` like every other verifier.

The first three were applied earlier; the remaining 25 are in
[The 25, recovered](#the-25-recovered) at the end of this document.

One correction to what the table below claims was done: the `eqEventHandler` **rename had not in
fact been made**. `layouts.json` still carried `u32 field_0` at offset 0, so the emitted header
declared a second vtable pointer, `sizeof` came out 0x164 against a `check_size` of 0x160, and the
header could not compile. That is now done, and `code/midtown2/eventq7/eqeventhandler.h` was
hand-edited to match what `genheaders.py` will emit - one deleted line.

| class | change |
|---|---|
| `eqEventHandler` | `size` 0x148 -> 0x160; appended `u8 field_148[0x18]`; **renamed `field_0` -> `vTable`, `u32` -> `void**`** |
| `mmInput` | `size` 0x1DC -> 0x248; appended the twelve members below; filled the `asNode` base member's missing `width` with 0x18 |
| `gfxTextureMovie` | `size` 0x28 -> 0x1C; dropped `field_1C`, `field_20`, `field_24` |

### Correction: why `check_size(eqEventHandler, 0x148)` fails today

Not because of the missing 0x18. The table below classes it as "LIVE, wrong", meaning it passes
against a wrong number, and that is not what happens. `field_0` at offset 0 **is** the vftable
pointer - the constructor at 0x004A16C0 opens `mov dword ptr [edx], 0x005B2960` - and the class has
a virtual destructor, so C++ emits that pointer implicitly. Declaring `u32 field_0` as well gives
the object two of them: `sizeof` came out 0x14C against a `check_size` of 0x148.

That matters for the fix. Correcting only the size would have moved the failure to 0x164 vs 0x160
rather than resolving it. `genheaders.py` already knows how to skip an explicit vptr member -
`emit_members` drops `members[0]` when the class is polymorphic and the member is named one of
`vtable` / `vfptr` / `__vftable` / `vtbl` / `vptr` - so the member was renamed to the spelling
`asNode` and 111 other classes already use, and the header now emits members from 0x004 with the
compiler supplying the pointer at 0x000. 0x004 .. 0x160 is 0x15C bytes, plus the implicit vptr,
which is 0x160.

`mmInput` and `gfxTextureMovie` needed no such treatment: `mmInput` derives from `asNode`, whose
0x18 the generator already skips, and `gfxTextureMovie` has no virtual methods at all.

## Why the allocation literal is the strongest size evidence we have

```asm
push 0x160                  ; <- the 1999 compiler wrote this number
call ??2@YAPAXI@Z           ; operator new(size_t)
mov  ecx, eax
call ??0eqEventHandler@@QAE@XZ
```

The immediate pushed to `operator new` **is** `sizeof(T)`. No human typed it, no analyser inferred
it, and nothing sits between it and the truth. Everything else we have about layout — the IDA type
library, MM2Hook's table, constructor writes, member-access floors — is downstream of somebody's
judgement. This is not.

It only says the total size. It says nothing about what is inside, which is why most of what
follows is a size correction and not a layout.

## The sweep

`midtown2.exe`, sha256 `992c53c9250cf822b44bf4a4013bd5805229bbbeb7b478a4c2556531bc5340f3`,
disassembled with capstone 5.0.7 from the RVAs in `data/symbols.json`. Every call to
`??2@YAPAXI@Z` (0x00577360) and `??2lvlInstance@@SAPAXI@Z` (0x00463110, a bump allocator that still
takes the real `sizeof` from its caller) — **1279 sites**. Sample literals were re-read out of
`C:/mm2ghidra/out/game.asm.pristine` and matched byte for byte.

| outcome | sites |
|---|---|
| the size argument is not a literal (array, computed, `n * sizeof`) | 658 |
| clean `push <imm>` → `new` → ctor called with the fresh pointer in `ecx` | **484** |
| no constructor call reachable from the allocation | 122 |
| array `new` — `operator new(n * sizeof)` followed by a constructor loop | 5 |
| an inlined derived constructor — see below | 10 |

Those 484 sites cover **246 distinct classes**.

### Two rejections that matter, because both produce plausible wrong numbers

**Inlined derived constructors.** When MSVC inlines a derived class's constructor, the only `call`
left is the *base* constructor, and the allocation size belongs to the derived class. It is visible
because the inlined body stores a second, different vftable to `[this+0]` right after the base
constructor returns:

```asm
push 0x44
call ??2lvlInstance@@SAPAXI@Z
mov  ecx, edi
call ??0dgUnhitBangerInstance@@QAE@XZ   ; this ctor stores vftable 0x005B56D4
mov  dword ptr [edi], 0x005B5658        ; ...and then a different one. Not dgUnhitBangerInstance.
```

Taken naively this "proves" `dgUnhitBangerInstance` is 0x44, against 0x18 in two independent
sources. It proves nothing of the kind. 10 sites were dropped this way, including three that would
have made `lvlInstance` 0x3C/0x40/0x58 instead of its correct 0x14.

**Array `new`.** `dgRagdollData`'s constructor does `operator new(0xC0)` and then constructs four
`dgLinkData` in a loop. 0xC0 is `4 * 0x30`, and 0x30 is exactly what `layouts.json` already says.
Detected by a backward branch that re-enters at or before the constructor call.

### How well it agrees where it can be checked

| cross-check | agree | disagree |
|---|---|---|
| `data/layouts.json` (IDA type library), 124 classes overlap | 96 | **28** |
| `data/mm2types.json` (MM2Hook `sizeof`), 105 classes overlap | 95 | 10 |

96/124 and 95/105 is the calibration: the method is not producing noise, and the disagreements are
worth taking seriously.

---

## The 28 disagreements

`short by` / `over by` is relative to what the game actually allocates.

### Too small — `layouts.json` is under the real `sizeof` (23)

| class | real `sizeof` | `layouts.json` | short by | `check_size` today | allocation site |
|---|---|---|---|---|---|
| `ControlCustom` | 0x7218 | 0x8C | 0x718C | commented | 0x00408054 `mmInterface::mmInterface` |
| `CrashCourse` | 0x850 | 0x8C | 0x7C4 | commented | 0x00407F64 `mmInterface::mmInterface` |
| `Dialog_Eject` | 0x2F8 | 0xBC | 0x23C | commented | 0x004089A0 `mmInterface::mmInterface` |
| `Vehicle` | 0x178 | 0x8C | 0xEC | commented | 0x00407EA9 `mmInterface::mmInterface` |
| `phInertialCS` | 0x1B4 | 0x118 | 0x9C | **LIVE, wrong** | 0x005A7D23 `dgRagdoll::dgRagdoll` |
| `Dialog_Serial` | 0x144 | 0xBC | 0x88 | commented | 0x004081E5 `mmInterface::mmInterface` |
| `mmInput` | 0x248 | 0x1DC | 0x6C | **LIVE, wrong** | 0x00401D74 `BeginPhase` |
| `NetSelectMenu` | 0x268 | 0x200 | 0x68 | **LIVE, wrong** | 0x00407F38 `mmInterface::mmInterface` |
| `UICompositeScroll` | 0xD4 | 0x6C | 0x68 | commented | 0x004E15B9 `UIMenu::AddCompScroll` |
| `mmPopup` | 0x60 | 0x1 | 0x5F | commented | 0x00412DD2 `mmGame::Init` |
| `Dialog_ReplayEdit` | 0x114 | 0xBC | 0x58 | commented | 0x004088FD `mmInterface::mmInterface` |
| `MainMenu` | 0x100 | 0xB8 | 0x48 | **LIVE, wrong** | 0x00407E4F `mmInterface::mmInterface` |
| `Dialog_DriverRec` | 0xF0 | 0xBC | 0x34 | commented | 0x00408324 `mmInterface::mmInterface` |
| `Dialog_HallOfFame` | 0xF0 | 0xBC | 0x34 | commented | 0x00408382 `mmInterface::mmInterface` |
| `Dialog_TCPIP` | 0xEC | 0xBC | 0x30 | commented | 0x00408129 `mmInterface::mmInterface` |
| `eqEventHandler` | 0x160 | 0x148 | 0x18 | **LIVE, wrong** | 0x00401CD4 `BeginPhase` |
| `vehCarAudioContainer` | 0x18 | 0x1 | 0x17 | commented | 0x0042C233 `vehCar::InitAudio` |
| `Dialog_City2` | 0xD0 | 0xBC | 0x14 | commented | 0x0040895B `mmInterface::mmInterface` |
| `RaceMenu` | 0x138 | 0x124 | 0x14 | commented | 0x00407F0C `mmInterface::mmInterface` |
| `mmVehList` | 0x10 | 0x1 | 0xF | commented | 0x00402A2B `mmGameManager::mmGameManager` |
| `mmMultiRoam` | 0xB208 | 0xB1FC | 0xC | commented | 0x00402B57 `mmGameManager::mmGameManager` |
| `mmPlayerConfig` | 0x7178 | 0x716C | 0xC | **LIVE, wrong** | 0x0050C8A8 `PUAudioOptions::PUAudioOptions` |
| `gizFerryMgr` | 0x34 | 0x2C | 0x8 | **LIVE, wrong** | 0x004158DB `init_gizmo_mgr` |

### Too large — `layouts.json` is over the real `sizeof` (5)

| class | real `sizeof` | `layouts.json` | over by | `check_size` today | allocation site |
|---|---|---|---|---|---|
| `mmInterface` | 0x7750 | 0x78C0 | 0x170 | **LIVE, wrong** | 0x004017EF `MainPhase` |
| `HostRaceMenu` | 0x1B8 | 0x270 | 0xB8 | **LIVE, wrong** | 0x004080C6 `mmInterface::mmInterface` |
| `NetArena` | 0x144 | 0x1CC | 0x88 | **LIVE, wrong** | 0x004080F2 `mmInterface::mmInterface` |
| `Dialog_Replay` | 0x168 | 0x1B0 | 0x48 | **LIVE, wrong** | 0x004085E6 `mmInterface::mmInterface` |
| `gfxTextureMovie` | 0x1C | 0x28 | 0xC | **LIVE, wrong** | 0x004B49DF `gfxGetTextureMovie` |

### Independent corroboration: what each class's own methods touch

For every one of the 28, every `this`-relative access made by the class's own methods was collected
(tracking `this` from `ecx` through register copies, `[ebp+X]` spill slots, `lea this+N` and `rep
stosd` runs). The result is a *floor*, never a ceiling — but it is an entirely separate measurement
from the allocation literal.

| class | real `sizeof` | `layouts.json` | end of the highest field its own methods touch |
|---|---|---|---|
| `ControlCustom` | 0x7218 | 0x8C | 0x7218 |
| `CrashCourse` | 0x850 | 0x8C | 0x84D |
| `Dialog_City2` | 0xD0 | 0xBC | 0xD0 |
| `Dialog_DriverRec` | 0xF0 | 0xBC | 0xE4 |
| `Dialog_Eject` | 0x2F8 | 0xBC | 0x2EC |
| `Dialog_HallOfFame` | 0xF0 | 0xBC | 0xF0 |
| `Dialog_ReplayEdit` | 0x114 | 0xBC | 0x114 |
| `Dialog_Serial` | 0x144 | 0xBC | 0x11D |
| `Dialog_TCPIP` | 0xEC | 0xBC | 0xEC |
| `MainMenu` | 0x100 | 0xB8 | 0x100 |
| `NetSelectMenu` | 0x268 | 0x200 | 0x268 |
| `RaceMenu` | 0x138 | 0x124 | 0x138 |
| `UICompositeScroll` | 0xD4 | 0x6C | 0xD4 |
| `Vehicle` | 0x178 | 0x8C | 0x178 |
| `gizFerryMgr` | 0x34 | 0x2C | 0x31 |
| `mmInput` | 0x248 | 0x1DC | 0x244 |
| `mmMultiRoam` | 0xB208 | 0xB1FC | 0xB208 |
| `mmPlayerConfig` | 0x7178 | 0x716C | 0x7171 |
| `mmPopup` | 0x60 | 0x1 | 0x60 |
| `mmVehList` | 0x10 | 0x1 | 0x10 |
| `phInertialCS` | 0x1B4 | 0x118 | 0x1B4 |
| `vehCarAudioContainer` | 0x18 | 0x1 | 0x18 |
| `eqEventHandler` | 0x160 | 0x148 | 0x148 — *the only one that does not corroborate* |
| `Dialog_Replay` | 0x168 | 0x1B0 | 0x168 |
| `HostRaceMenu` | 0x1B8 | 0x270 | 0x1B8 |
| `NetArena` | 0x144 | 0x1CC | 0x144 |
| `gfxTextureMovie` | 0x1C | 0x28 | 0x1C |
| `mmInterface` | 0x7750 | 0x78C0 | 0x774C |

Read the two columns together. **22 of the 23 short classes reach past what `layouts.json` says the
object is** — the game is already writing outside the struct we declare. And **no class, in either
group, ever touches a byte at or past its allocation size**: every one of the five over-sized
layouts stops dead at the smaller number. Two independent measurements, one conclusion.

`eqEventHandler` is the single class where only the allocation literal speaks. Its case is below.

## Where the wrong numbers came from — one systematic defect, not 28 accidents

Sort the short classes by what `layouts.json` claims and the cause is obvious:

| derived class | `layouts.json` says | which is exactly `sizeof` of |
|---|---|---|
| `ControlCustom`, `CrashCourse`, `Vehicle` | 0x8C | `OptionsBase` / `UIMenu` / `VehicleSelectBase`, all 0x8C |
| `Dialog_City2`, `Dialog_DriverRec`, `Dialog_Eject`, `Dialog_HallOfFame`, `Dialog_ReplayEdit`, `Dialog_Serial`, `Dialog_TCPIP` | 0xBC | `PUMenuBase`, 0xBC |
| `UICompositeScroll` | 0x6C | `uiWidget`, 0x6C |
| `RaceMenu` | 0x124 | `RaceMenuBase`, 0x124 |
| `MainMenu` | 0xB8 | `UIMenu` 0x8C, plus two fields it did recover |

**The IDA type library gave these derived UI classes their base class's layout.** Every one of them
has a single anonymous member at offset 0 with no type and no name — the base subobject — and
nothing after it. That is why `genheaders.py` emits a commented-out `check_size` for most of them:
`emit_members` drops members with no name or type, the list comes out empty, and the guard is
disabled. The wrong size then survives in the comment, where a human will read it and believe it.

The two over-sized ones with a suspicious number are the same defect running the other way:
`NetArena` is given 0x1CC, which is `aiMap`'s size, and `HostRaceMenu` is given 0x270, which is
`gfxInterface`'s. `gfxTextureMovie` is different and worth its own note — see below.

## What is actually broken today, and what is only latent

The distinction matters because it decides what is urgent.

**Nothing miscompiles and nothing corrupts memory right now.** Every allocation in the shipped
binary uses the literal, and none of these classes has been ported, so no C++ `new` runs for any of
them.

**The 12 live `check_size` assertions that pass while certifying a wrong number** are the real
problem, because they look like verification and are not. `check_size` compares the emitted header
against `layouts.json`; both sides come from the *same* record, so it can only catch a header that
has drifted from its own source. It cannot see that the source is wrong. `check_size(mmInput,
0x1DC)` has been passing all along against an object that is 0x248 bytes.

**The latent failure** is the one the task named: the first ported `new eqEventHandler` under-
allocates by 0x18 bytes and the still-assembly `eqEventHandler` code writes past the block. Nothing
reports it, because the bytes it lands on belong to whatever the allocator handed out next.

`gfxTextureMovie` is the one case where a live `check_size` is wrong in the *dangerous* direction
in reverse: `layouts.json` gives it 13 members ending at 0x28, but the object is 0x1C. Its
constructor at 0x004B4BA0 writes 0x00, 0x04, 0x08, 0x0C, 0x10 (byte), 0x11 (byte), 0x14 and 0x18,
and stops — ending exactly at 0x1C. The trailing `field_1C`, `field_20`, `field_24` in
`code/midtown2/gfx/gfxtexturemovie.h` do not exist. Anyone reading that header would think they do.

## On `check_size` being `<=`

It is not. `code/midtown2/core/hooking.h:42`:

```cpp
#define check_size(TYPE, SIZE) \
    static_assert(sizeof(TYPE) == (SIZE), ...)
```

It is `==`, and that is the right choice. `<=` would accept an over-sized layout in silence, which
is precisely the `gfxTextureMovie` case — a header carrying three fields that are not in the object,
passing its own guard. `==` catches drift in both directions.

But it is worth being clear about what `==` does and does not buy, because it is easy to over-read.
The assertion relates the *emitted header* to the *number recorded next to it*, and both are
generated from one `layouts.json` entry. It is a consistency check on our own bookkeeping, not a
check against `midtown2.exe`. Every one of these 28 classes has a self-consistent, passing (or
commented) `check_size` and a wrong size.

The gap is worth closing mechanically rather than by hand: this sweep is deterministic, it runs on
a file the repo already pins, and it produces a size for 246 classes. A `tools/verify_sizes.py`
that walks the `new` sites and diffs them against `layouts.json` would turn all 28 of these into
build-time output instead of something someone has to go looking for.

---

## Recovered: `eqEventHandler`, 0x148 → 0x160

```asm
00401CCF  push 0x160
00401CD4  call ??2@YAPAXI@Z
00401CDC  mov  [ebp+8], eax
00401CDF  test eax, eax
00401CE8  je   0x401CF3
00401CEA  mov  ecx, eax
00401CEC  call ??0eqEventHandler@@QAE@XZ
```

In `game.asm.pristine`, inside `?BeginPhase@@YAX_N@Z`:

```asm
db 068h, 060h, 001h, 000h, 000h
db 0E8h
dd ??2@YAPAXI@Z - ($ + 4)
```

Everything below 0x148 is confirmed, not merely inherited. The constructor at 0x004A16C0 zeroes
`Clients[8]` at 0x010 (`rep stosd`, ecx=8) and 0x100 bytes at 0x048 (`rep stosd`, ecx=0x40), which
ends at exactly 0x148 — the same boundary the type library gives, reached independently.

**0x148–0x160 could not be named.** All 12 of the class's own methods were scanned, including the
0x5D0-byte `Update` (whose `symbols.json` size of 50 is wrong — the switch arms at 0x004A17C2 are
labelled as separate `IO_EVENT_*` data symbols but are the same function). Nothing reaches 0x148.
There are 16 references to the `SuperQ` singleton at
0x00661788: two stores (`BeginPhase`, `EndPhase`) and fourteen loads, and every one of the
fourteen is a `mov ecx, [SuperQ]` immediately followed by a method call. No caller touches the tail
either.

So the correction is a `u8 field_148[0x18]` filler. That is deliberately an admitted gap: the object
comes out the right size, and nothing pretends to know what is in it. There is one unproven
observation worth recording and not acting on — 0x18 is `sizeof(ioEvent)`, and `Update` builds an
`ioEvent` as a *local* at `[ebp-0x18]`; a member that a later revision moved onto the stack would
look exactly like this. That is a story, not evidence.

```
+0x000 .. +0x148   unchanged, from the IDA type library
+0x148  u8 field_148[0x18]
= 0x160
```

## Recovered: `mmInput`, 0x1DC → 0x248

```asm
00401D6F  push 0x248
00401D74  call ??2@YAPAXI@Z
00401D8A  mov  ecx, eax
00401D8C  call ??0mmInput@@QAE@XZ
```

`db 068h, 048h, 002h, 000h, 000h` in `game.asm.pristine`, again in `BeginPhase`.

Unlike `eqEventHandler`, this one is almost fully recoverable: the class's own 69 methods reach
+0x244 and stop, which lands on 0x248 for a dword. Ten of the twelve new members have real
evidence.

| offset | type | how it is known |
|---|---|---|
| +0x1DC | `i32` | `mmInput::Update` 0x0052C9D1, copied from `[0x006A38DC]` |
| +0x1E0 | `i32` | `mmInput::Update` 0x0052C9DD, copied from `[0x006A38D8]` |
| +0x1E4 | `f32` | `mmInput::Update` 0x0052C9EF, `fstp` |
| +0x1E8 | `f32` | `mmInput::Update` 0x0052CA01, `fstp` |
| +0x1EC | `f32` | `mmInput::Update` 0x0052CA13, `fstp` of `[0x005B0364] / field_1E4` |
| +0x1F0 | `f32` | `mmInput::Update` 0x0052CA25, `fstp` of `[0x005B0364] / field_1E8` |
| +0x1F4 | `i32` | `mmInput::BuildCaptureIO` sets it to 4 / 5 / 3 at 0x0052C622 / 0x0052C62E / 0x0052C6F9, then passes it as `SanityCheck`'s 2nd argument at 0x0052C715 |
| +0x1F8 | `i32` | same function, 0x0052C6E5 / 0x0052C709; `SanityCheck`'s 3rd argument at 0x0052C70F |
| +0x1FC | `u32 field_1FC` | **never observed.** A gap, so the members either side keep their real offsets |
| +0x200 | `i8 KeyboardBuffer[0x40]` | `GetBufferedKeyboardData` 0x0052CCC5: `lea eax,[esi+0x200]; push 0x40; call ioKeyboard::GetBufferedInput`. The 0x40 is the buffer's own length. Read a byte at a time at 0x0052CD0D: `movsx ecx, byte ptr [ecx+edx+0x200]` |
| +0x240 | `i32 KeyboardBufferCount` | 0x0052CCD4 stores `GetBufferedInput`'s return; `GetNextKeyboardEvent` decrements it at 0x0052CCEE and uses it to index `KeyboardBuffer`; `Flush` zeroes it at 0x0052CBDC |
| +0x244 | `u32 field_244` | **never observed.** The last four bytes of the object |

`field_1FC` and `field_244` were checked against every caller reaching `mmInput` through the
singleton at 0x006B1CF0 as well as against the class's own methods. Nothing touches either.

The `KeyboardBuffer` / `KeyboardBufferCount` pair is the only place a real 1999 name could be
argued for, and even those are descriptive rather than recovered — `ioKeyboard::GetBufferedInput`
names the operation, not the field. They are honest as `field_200` / `field_240` if that is
preferred; the offsets and widths are what matter.

---

## What needs a hand edit, and why nothing here can be merged

`data/layouts_short_fix.json` carries all 28 corrections. Run through the four refusals in
`tools/merge_layouts.py`:

1. **Never overwrite a class the IDA type library already has.** All 28 are in `layouts.json`.
   Every entry is refused by rule 1, and refusing is right — a sweep does not get to silently
   overwrite the most-verified source in the project.
2. `CONFIRMED` only — met for `eqEventHandler` and `mmInput`, not claimed for the other 26.
3. A real size, corroborated — met for all 28 (allocation literal + method-access floor).
4. Members must tile 0 → size — met for `eqEventHandler` and `mmInput` only.

So `merge_layouts.py --write` will print 28 rejections and change nothing. **Every correction below
needs a deliberate edit to `data/layouts.json`.**

**Two can be applied in full**, because a complete tiling member list exists:

- `eqEventHandler` — set `size` to `0x160`, append `field_148` `u8[0x18]`.
- `mmInput` — set `size` to `0x248`, append the twelve members in the table above.

`layouts_short_fix.json` holds both entries in `layouts.json`'s own schema, already tiling, so they
can be lifted across as-is. (`mmInput`'s `asNode` base member at offset 0 has no `width` in the IDA
dump, which is what stops it tiling; `layouts_short_fix.json` fills in 0x18, which is `asNode`'s
size in `layouts.json` and in `mm2types.json` both.)

**Twenty-six are size-only.** Their real `sizeof` is known and their members are not. Per
`docs/layout_sources.md` the size must not be written into `layouts.json` without a member list —
a class with a corrected size and a member list that stops short is exactly the failure mode that
document exists to prevent, and it would be worse than the status quo because `genheaders.py` would
then emit a *live* `check_size` for it. Their `size` is `null` in `layouts_short_fix.json`, with the
real number recorded beside it as `real_size_from_operator_new`.

The useful thing to do with those 26 now is to correct the comment, not the size: the seven
`// check_size(Dialog_*, 0xBC)` lines and their siblings are all quoting a base class's size, and a
one-line note saying so costs nothing and stops the next person trusting the number.

**Refinement, and a recommendation not yet applied.** That reasoning is right for the nine classes
whose members ARE emitted, and the failure it predicts is real but LOUD rather than silent: a
corrected `size` with a member list that stops short makes `genheaders.py` emit a live
`check_size` that fails to compile, which blocks every translation unit that includes the header.
That is why `Dialog_Replay`, `HostRaceMenu`, `MainMenu`, `NetArena`, `NetSelectMenu`, `gizFerryMgr`,
`mmInterface`, `mmPlayerConfig` and `phInertialCS` must keep their wrong sizes until their tails are
worked out the way `mmInput`'s were.

It does not hold for the other sixteen. `ControlCustom`, `CrashCourse`, the seven `Dialog_*`,
`RaceMenu`, `UICompositeScroll`, `Vehicle`, `mmMultiRoam`, `mmPopup`, `mmVehList` and
`vehCarAudioContainer` have a single anonymous member at offset 0 with no name and no type, so
`emit_members` drops it, the member list comes out EMPTY, and `genheaders.py` takes the
`// check_size(T, 0xN); // size known, members are not` branch. For those sixteen the `size` field
is not compiled into anything - it only fills in that comment. Writing the real number there
changes a comment from a lie to the truth and nothing else.

Checked before recommending it: none of the sixteen is named as a base by `hierarchy.json` or
`mm2types.json`, so no derived class's `skip_to` moves; and every reference to one of them from
another class's member list is a POINTER (`RaceMenu*` in `mmInterface`, `UICompositeScroll*` in
`NetArena`, `mmPopup*` in `mmGame`), which is four bytes whatever the pointee's size. So the edit
is inert everywhere except the comment. It is left unapplied here only because it could not be
compiled from where this was written; it is the right next move.

The five over-sized ones are a different job. `gfxTextureMovie` can be fixed properly today — drop
`field_1C`, `field_20`, `field_24` and set the size to 0x1C; the constructor proves the object ends
there. The other four (`mmInterface`, `HostRaceMenu`, `NetArena`, `Dialog_Replay`) have long member
lists whose tails are fictional, and working out which members to drop needs the same treatment
`mmInput` got here.

---

## Appendix A — `mm2types.json` sizes contradicted by the binary

`genheaders.py` falls back to MM2Hook's size when `layouts.json` has none, and emits it as
`// check_size(T, N); // size known, members are not`. These ten are wrong there. Where
`layouts.json` also has an entry it agrees with the binary and MM2Hook is the outlier, which is a
useful ordering to know.

| class | real `sizeof` | `layouts.json` | `mm2types.json` |
|---|---|---|---|
| `AudManager` | 0x54 | 0x54 | 0x18 |
| `crAnimation` | 0x20 | 0x20 | 0x8 |
| `vehCarModel` | 0xCC | 0xCC | 0x154 |
| `vehCarSim` | 0x1560 | 0x1560 | 0x1590 |
| `mmPopup` | 0x60 | 0x1 | 0x7C |
| `AudSoundBase` | 0x40 (28 sites) | — | 0x58 |
| `mmPlayer` | 0x23A4 (10 sites) | — | 0x23CC |
| `mmCDPlayer` | 0x138 | — | 0x154 |
| `vehSiren` | 0x44 | — | 0x164 |
| `vehSplash` | 0x328 | — | 0x18 |

`AudSoundBase` is the one to weigh carefully: 28 independent allocation sites all push 0x40, and the
class's own 44 methods reach exactly 0x40. MM2Hook's 0x58 is not supported by anything in the
binary.

## Appendix B — 122 classes with a real `sizeof` and no `layouts.json` entry

Recorded because it is free. A size alone still must not be merged (rule 3, and
`docs/layout_sources.md`), but it is a hard number for any of these that gets ported, and it
resolves the ten Appendix A conflicts.

| class | `sizeof` | `mm2types.json` | sites |
|---|---|---|---|
| `AboutMenu` | 0xAC | - | 1 |
| `Aud3DAmbientObject` | 0x80 | - | 1 |
| `Aud3DObjectManager` | 0x198 | - | 1 |
| `Aud3DSampleWrapper` | 0x14 | - | 2 |
| `AudCreature` | 0xC | - | 3 |
| `AudCreatureAvoid` | 0x3C | - | 3 |
| `AudCreatureImpact` | 0x24 | - | 3 |
| `AudImpact` | 0x20 | 0x1 | 3 |
| `AudImpactData` | 0x10 | - | 2 |
| `AudSoundBase` | 0x40 | 0x58 | 28 |
| `AudSpeechData` | 0x1C | - | 1 |
| `AudStream` | 0x44 | - | 1 |
| `AudStreamingMusic` | 0x7C | - | 1 |
| `AudioOptions` | 0x7378 | - | 1 |
| `CDMan` | 0x30 | - | 1 |
| `CFileStream` | 0x18 | - | 1 |
| `CLoader` | 0x234 | - | 1 |
| `CMemStream` | 0x28 | - | 1 |
| `ControlSetup` | 0x7250 | - | 1 |
| `CrashCourseIntro` | 0x8C | - | 1 |
| `DMusicWaveBuffer` | 0x10 | - | 1 |
| `Dialog_ControlAssign` | 0xBC | - | 1 |
| `Dialog_Message` | 0xCC | - | 12 |
| `Dialog_NewPlayer` | 0x104 | - | 1 |
| `Dialog_Password` | 0xE8 | - | 1 |
| `Dialog_RaceEnvironment` | 0xEC | - | 1 |
| `EchoEffect` | 0x2C | - | 2 |
| `GraphicsOptions` | 0x7228 | - | 1 |
| `OptionsMenu` | 0x98 | - | 1 |
| `PUAudioOptions` | 0xBC | - | 1 |
| `PUChat` | 0x11C | - | 1 |
| `PUControl` | 0xD4 | - | 1 |
| `PUDebug` | 0x11C | - | 1 |
| `PUExit` | 0xBC | - | 1 |
| `PUGraphics` | 0xD4 | - | 1 |
| `PUMain` | 0xC8 | - | 1 |
| `PUOptions` | 0xBC | - | 1 |
| `PUQuit` | 0xC4 | - | 1 |
| `PUReplay` | 0xC0 | - | 1 |
| `PUReplaySave` | 0xBC | - | 1 |
| `PURoster` | 0x2F4 | - | 1 |
| `TextDropWidget` | 0x30 | - | 1 |
| `UIBMLabel` | 0x98 | - | 1 |
| `UIControlWidget` | 0xA8 | - | 1 |
| `UIMexButton` | 0xBC | - | 1 |
| `UISlider` | 0xC8 | - | 1 |
| `UITextDropdown` | 0x100 | - | 1 |
| `UITextRoller` | 0x100 | - | 1 |
| `UITextScroll` | 0xD4 | - | 1 |
| `UIToggleButton` | 0xAC | - | 1 |
| `UIToggleButton2` | 0xB0 | - | 1 |
| `UIVScrollBar` | 0xFC | - | 1 |
| `VehShowcase` | 0x94 | - | 1 |
| `aiEngineAudio` | 0x1C | - | 2 |
| `aiGoalRandomDrive` | 0x14 | 0x14 | 1 |
| `aiGoalRegainRail` | 0x24 | 0x24 | 1 |
| `aiPedestrianInstance` | 0x2C | 0x2C | 1 |
| `aiPoliceForce` | 0x3C | 0x3C | 1 |
| `aiVehicleInstance` | 0x3C | 0x3C | 1 |
| `asViewCS` | 0x11C | - | 1 |
| `camTransitionCS` | 0x128 | - | 1 |
| `crBodyData` | 0x20 | - | 1 |
| `crHeadData` | 0x10 | - | 1 |
| `crIKGoal` | 0x28 | - | 2 |
| `crKinematics` | 0x20 | - | 1 |
| `crSpineData` | 0x14 | - | 1 |
| `dgBoundBox` | 0x318 | 0x318 | 2 |
| `dgBoundGeometry` | 0x80 | 0x80 | 1 |
| `dgBoundHotdog` | 0x88 | 0x88 | 1 |
| `dgBoundSphere` | 0x84 | 0x84 | 1 |
| `dgGlassInstance` | 0x5C | - | 1 |
| `dgPathSet` | 0x310 | 0x310 | 5 |
| `dgRagdoll` | 0x18 | 0x1 | 1 |
| `dgRagdollData` | 0x20 | - | 1 |
| `fxShardManager` | 0x4C | 0x4C | 1 |
| `gizParkedCarMgr` | 0x24 | - | 1 |
| `gizPathspline` | 0x48 | - | 3 |
| `gizTrainMgr` | 0x24 | - | 1 |
| `lvlFixedMatrix` | 0x44 | 0x44 | 1 |
| `lvlMultiRoomInstance` | 0x1C | - | 1 |
| `mmBillInstance` | 0x5C | - | 1 |
| `mmCCSpeech` | 0xA0 | - | 1 |
| `mmCDPlayer` | 0x138 | 0x154 | 1 |
| `mmCNRSpeech` | 0x7C | 0x1 | 1 |
| `mmCRHUD` | 0x17C | - | 1 |
| `mmCarRoadFF` | 0x3C | - | 1 |
| `mmCircuitHUD` | 0x6C | - | 1 |
| `mmCompDRecord` | 0x9C | - | 2 |
| `mmCompRaceRecord` | 0xA0 | - | 2 |
| `mmDropDown` | 0xE8 | - | 1 |
| `mmGameManager` | 0x1B8 | 0x1B8 | 1 |
| `mmJoyMan` | 0x48 | - | 1 |
| `mmMirror` | 0x64 | - | 1 |
| `mmMultiBlitz` | 0xB218 | 0xB218 | 1 |
| `mmNumber` | 0x74 | - | 9 |
| `mmPlayer` | 0x23A4 | 0x23CC | 10 |
| `mmPositions` | 0xC | - | 1 |
| `mmPowerupInstance` | 0x5C | - | 1 |
| `mmRaceData` | 0x4B4 | - | 3 |
| `mmRaceSpeech` | 0xD8 | 0x1 | 1 |
| `mmRainAudio` | 0x1C | - | 1 |
| `mmSingleBlitz` | 0x7710 | 0x7710 | 1 |
| `mmSingleCircuit` | 0x7720 | 0x7720 | 1 |
| `mmSingleRace` | 0x76F0 | 0x76F0 | 1 |
| `mmSingleRoam` | 0x76F0 | 0x76F0 | 2 |
| `mmSpeechContainer` | 0xC | - | 1 |
| `mmToggle` | 0x30 | - | 2 |
| `mmToggle2` | 0x38 | - | 2 |
| `mmVehInfo` | 0x124 | 0x124 | 1 |
| `mmWPHUD` | 0x68 | - | 2 |
| `phBoundTerrainLocal` | 0xCC | 0xCC | 1 |
| `phContactMgr` | 0x34 | - | 1 |
| `vehCarAudio` | 0x130 | 0x130 | 2 |
| `vehEngineSampleWrapper` | 0x50 | - | 1 |
| `vehHornAudio` | 0x34 | - | 2 |
| `vehHornAudioTiming` | 0x14 | - | 2 |
| `vehNitroCarAudio` | 0x13C | 0x13C | 2 |
| `vehPoliceCarAudio` | 0x248 | 0x248 | 2 |
| `vehSemiCarAudio` | 0x148 | 0x148 | 2 |
| `vehSiren` | 0x44 | 0x164 | 1 |
| `vehSplash` | 0x328 | 0x18 | 1 |
| `vehStuck` | 0x54 | 0x54 | 1 |


---

## The 25, recovered

All 25 are now written into `data/layouts.json` and `verify_sizes.py` passes. They fall into three
groups.

### Group 1 - size only, sixteen classes (the edit is inert outside a comment)

`ControlCustom` 0x7218, `CrashCourse` 0x850, `Dialog_City2` 0xD0, `Dialog_DriverRec` 0xF0,
`Dialog_Eject` 0x2F8, `Dialog_HallOfFame` 0xF0, `Dialog_ReplayEdit` 0x114, `Dialog_Serial` 0x144,
`Dialog_TCPIP` 0xEC, `RaceMenu` 0x138, `UICompositeScroll` 0xD4, `Vehicle` 0x178,
`mmMultiRoam` 0xB208, `mmPopup` 0x60, `mmVehList` 0x10, `vehCarAudioContainer` 0x18.

This is the move the previous pass recommended and could not compile from where it was written.
Each of the sixteen has an EMPTY emitted member list - a single anonymous member at offset 0, or no
members at all - so `emit_members` drops everything and `genheaders.py` takes the
`// check_size(T, 0xN); // size known, members are not` branch.

Re-checked before applying rather than taken on trust: every class in `layouts.json` was packed the
way `genheaders.py` packs it, before and after the edit. The emitted `sizeof` of all sixteen is
UNCHANGED, and no other class in the file moved. None of the sixteen is a base in
`hierarchy.json`, and every reference to one from another class's member list is a pointer.

### Group 2 - members recovered, five classes

| class | change | evidence |
|---|---|---|
| `gizFerryMgr` | 0x2C -> 0x34 | The ctor at 0x00579550 ends `mov [esi+0x2C], 0x43480000` (200.0f) then `mov byte [esi+0x30], al`. `Update` at 0x005797E2 tests that byte. So `f32 field_2C` + `u8 field_30`, and 0x31 aligned to 4 is 0x34. `layouts_from_ctors.json` independently records "the constructor reaches 0x31 but IDB type library says 0x2C". |
| `mmPlayerConfig` | 0x716C -> 0x7178 | `DefaultViewSettings` at 0x00526350 writes nine separate bytes, 0x7168..0x7170, and a sweep of every instruction in the image with a displacement in 0x7160..0x7180 finds nothing else. Corroborated from a second direction: `mmInterface` holds a `mmPlayerConfig` BY VALUE at 0x1C8 and its next member is `mmMiscData` at 0x7340 - and 0x7340 - 0x1C8 is exactly 0x7178. Five individual `i8` at 0x716C..0x7170 keep the observed offsets; `u8 field_7171[7]` is the admitted tail. |
| `MainMenu` | 0xB8 -> 0x100 | From the ctor at 0x00505C30 and `InitDriver`: `string` at 0xB8 and at 0xC0 (both `call ??0string@@QAE@XZ`, 8 bytes each), `mmTextNode*` at 0xC8 (`new 0x54` -> `??0mmTextNode@@QAE@XZ` -> `AddChild`), a char buffer at 0xCC (`sprintf(this+0xCC, " ")` at 0x00506100), `UITextDropdown*` at 0xF4 (`AddTextDropdown`'s return, stored at 0x005061A8), a player count at 0xF8 (`AddPlayer` / `RemovePlayer` / `RemoveAllPlayers`), and 0xFC, initialised to -1 and written by `FocusDescription`. 0xFC + 4 = 0x100. |
| `phInertialCS` | 0x118 -> 0x1B4 | Three further sources agree with the allocation literal. `InitArray` at 0x00475CB0 walks the array with `add eax, 0x1B4` - the stride IS `sizeof`. `ZeroForces` at 0x00475F8A calls `Matrix34::Zero` on this+0x120, this+0x150 and this+0x180 - three 0x30 matrices ending at 0x1B0 - and then writes 0x1B0. And four other `layouts.json` classes hold a `phInertialCS` by value with exactly 0x1B4 of room: `MM2::vehCarSim` 0x18 -> 0x1CC, `MM2::dgBangerActive` 0xB8 -> 0x26C, `MM2::vehTrailer` 0x210 -> 0x3C4, `MM2::aiVehicleActive` 0x2D8 -> 0x48C. The members are `u8 field_118` (a flag `Zero` clears), `u32 field_11C`, three `Matrix34`, `f32 field_1B0`. |
| `NetSelectMenu` | 0x200 -> 0x268 | Rebuilt from the ctor at 0x00504130 plus `BuildComs` / `SetComs`. Three `datCallback` at 0xDC / 0xE8 / 0xF4 and two `string` at 0x104 / 0x10C are constructor CALLS, not inference. Four char buffers are `sprintf` / `strcpy` destinations: 0x8C (the net name - `SetNetname` at 0x00504B10 writes it), 0xB4 and 0x134 (both initialised from the literal `"       "`), and 0x210 / 0x238, which `BuildComs` at 0x00504C93 formats INTO from 0xB4 and 0x134 and which `SetComs` hands to `SetIPAddress` and `SetPhoneNumber`. 0x1F8 is where `GetCommPack` takes its `NETCOMMPACK*`. The last dword pair 0x260 / 0x264 is written by the ctor and read by `PreSetup`. Two offsets, 0x170 and 0x1A4, are never observed and are kept as `u32` gaps so their neighbours keep their real offsets. |

`aiVehicleActive` had to move with `phInertialCS`: its `field_2D8` was declared 280 wide with a
`u8 field_3F0[156]` filler behind it, and 280 + 156 = 0x1B4. The member took its real width and the
filler came out. Its packed `sizeof` is unchanged.

### Group 3 - over-sized, four classes: the tail is fictional and comes off

`Dialog_Replay` 0x1B0 -> 0x168 (drop `field_168[72]`), `NetArena` 0x1CC -> 0x144 (drop `unk2`
through `unk7`), `HostRaceMenu` 0x270 -> 0x1B8 (drop `field_1B8[180]` and `unk2`), and
`mmInterface` 0x78C0 -> 0x7750.

For the first three the truncation is exact. The highest byte any of the class's own methods
touches is 0x140, 0x164 and 0x1B4 respectively - each the last dword of the real object - and
dropping every member at or past the real size leaves a list that packs to exactly that size with
no surviving member moving.

`mmInterface` needed a substitution as well as a truncation. Its `LocString field_76C0` is 0x200
bytes and would end at 0x78C0, which is where IDA's wrong size came from. The object ends at
0x7750, and the constructor says why: after the two `mmMiscData` it constructs a `string` at 0x7740
and a `string` at 0x7748, and 0x7748 + 8 = 0x7750. 0x76C0 is a char buffer - `InitPlayerInfo` at
0x00409484 copies `AngelReadString(0x43)` into it - so it becomes `i8 field_76C0[128]`, the space
between it and the first `string`. 0x76AC is a real dword, the protocol, written 0/1/2 by
`SetProtocol`, `SetHostProtocol`, `JoinSession` and four others. 0x76A8 and 0x76B0..0x76C0 are
never observed and are kept as gaps.

### One thing found on the way that is not a size

`HostRaceMenu`'s constructor calls `??0RaceMenuBase@@QAE@H@Z` at 0x005006C5, so its base is
`RaceMenuBase` (0x124), not the `UIMenu` (0x8C) that `hierarchy.json` infers from the vftable.
`layouts.json`'s member list for it starting at 0x124 is the same fact from the other side. Rather
than hand-edit a generated `hierarchy.json`, the 0x98 between the two was declared as
`u8 field_8C[0x98]`: the offsets and the size come out right either way, but if `base_of` is ever
corrected to `RaceMenuBase`, that filler must come out with it.
