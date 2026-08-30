# Where OpenMM2 is

## It runs

OpenMM2 boots and reaches `GameLoop` — the same point retail reaches — and stays there. A 90-second
run produces 66,068 lines of debug output identical in shape to retail's, with **zero access
violations**. It advertises **2560x1440**, confirmed in the running game's own log:

```
Max display resolution restricted to 2560 x 1440
```

The whole startup matches retail line for line: Angel banner, adapter detect (RX 9060 XT), intro
movie, all six AR archives, `BeginPhase`, loading screen, audio device enumeration, GameInput,
profile load, lobby, `GameLoop`.

## What was actually wrong

Five defects stood between "assembles and links" and "runs". Every one of them passed every
byte-level verifier at the time, which is the thing worth remembering.

| Defect | Symptom | Why nothing caught it |
|---|---|---|
| 307 DirectInput GUID pointers never relocated | crash on first mouse click | an untouched pointer is *identical* to retail |
| `"tga"` relocated as a pointer | "not properly installed - insert the original CD" | a corrupted string differs by exactly the section delta, same as a correct relocation |
| A live instruction relocated as a pointer | illegal instruction in the JPEG decoder | same as above |
| Half of `vehLoadNameList` left unrelocated | crash loading the first vehicle | same as above, in reverse - the guard that fixed the string broke the pointer array |
| The build restored a stale snapshot over each fresh export | every fix silently absent from the binary | the old assembly is internally consistent, so all gates pass |

The last one is the one to remember: **a verifier that compares the build against retail cannot
catch a build that used the wrong input.**

## Gates now in the build

Thirteen steps, all passing. Three are new, and each exists because of a specific failure above:

- `verify_dataformats.py` — every DirectInput GUID pointer must be relocated. Reads `dwNumObjs` and
  `rgodf` out of the structure rather than inferring anything.
- `verify_strings.py` — every string in retail must be present, unchanged, in ours. Never reasons
  about deltas, which is precisely why it can see what the others cannot.
- `patches.py` verification — a deliberate change must state its address, and that address is
  checked against the retail binary before anything is written.

`verify_text.py` now also understands deliberate patches and the difference between calling a
ported function and taking its address.

## Numbers

- 160 functions ported (1.02%), all correctly mangled and verified
- 243 of 535 classes have a recovered member layout (65.5%)
- 0 bytes of `.text`, `.rdata` or `.data` unaccounted for

## What is not done

- **OpenGL is scaffolded, not live.** `code/midtown2/agigl/` is guarded behind `ARTS_AGIGL`, which
  `tools/build.py` does not define, so none of it has ever been compiled; nothing is registered, and
  the game still runs its original D3D7 path through dgVoodoo. `docs/opengl_plan.md` has the route
  and **`docs/opengl_activation.md` has the switch**: `-gl`, off by default, branching at runtime
  inside three COM-free functions (`InitDirectDraw`, `gfxPipeline::BeginGfx`,
  `gfxPipeline::EndGfx`) so the DirectDraw bring-up is never stripped. `data/ported_agigl.json` is
  deliberately empty. Two things have to be settled before the first build: DPI awareness (a
  non-DPI-aware process is handed virtualised sizes, which decides whether 1440p works at all), and
  the `__stdcall` import-decoration mismatch that currently stops anything in `agigl/` from linking.
- **UI scaling is landed but unrun.** All four fixes from `docs/ui_scaling.md` are written: the CSV
  loaders, font sizing, the design-space funnel, and - as of round two - bitmap resampling at load
  in `gfxBitmap::Create(gfxImage*, bool)` plus the oversize source rect in
  `gfxPipeline::CopyBitmap` that was leaving 1440p with no menu background at all. None of it has
  been compiled or run. `docs/ui_scaling_progress.md` has the ledger and the order in which things
  would fail.
- **Multiplayer** is deliberately untouched.


# Session two

## 1440p: what is actually true

The cap patch alone was not enough, and the reason is worth writing down because it took three
wrong theories to find.

- The game **advertises** 2560x1440 (the patch at `0x004AC2EE`/`0x004AC2F3`).
- It could not **see** 1440p, because dgVoodoo enumerates the mode list and its config said
  `Resolution = h:1920, v:1080` with no extra modes. Adding `ExtraEnumeratedResolutions = 2560x1440`
  to `dgVoodoo.conf` makes the game enumerate it, confirmed in its own log: `ResCallback: 2560 x 1440`,
  with no "rejected due to" line. The original `dgVoodoo.conf` is backed up beside it as
  `dgVoodoo.conf.before-openmm2`.
- It still would not **select** it, because the choice is a stored index in `gfxconf.dat`.
  `tools/setres.py` sets it; both adapters now point at the 1440p entry.
- The **frontend is hardcoded to 640x480** in `BeginPhase` - `gfxPipeline::SetRes(640, 480, 16, 16, 0)`
  on the splash path, with the selected resolution used only on the other branch. So measuring the
  menu window and finding 640x480 is vanilla behaviour, not a fault. 1440p applies to gameplay.

DPI awareness is also fixed, embedded at link time (`-MANIFEST:EMBED`): this display runs at
2560x1440 and hands a non-aware process 1920x1080, which is exactly where enumeration stopped. A
sidecar `.manifest` does not work - Windows only reads those with a machine-wide registry opt-in.

## A new class of silent breakage

`tools/verify_extents.py`, now gate 5, runs *before* stripping.

A function is stripped by its recorded extent, and that extent is the distance to the next known
symbol. `?Extend@Vector3@@QAEXABV1@M@Z` is recorded as 272 bytes, but a **static initialiser starts
50 bytes into it** and the CRT's table points straight at it. Porting the function deleted the
initialiser, so `_initterm` called into the middle of the reimplementation and the game died before
its second line of output.

Every existing gate passed while that was true, for a reason that generalises: **bytes inside a
ported region are expected to differ, so a verifier cannot see something deleted there.** The new
gate asks a different question - does any dword in `.rdata`/`.data` point *strictly inside* a ported
function? - which covers initialiser tables, jump tables and vftable slots alike.

Two related fixes came out of the same crash:
- `tools/asm.py` now preserves a stripped function's internal labels at their original offsets, so
  a switch table emitted outside the function still resolves to the addresses it always did.
- `-Zc:sizedDealloc-`: a virtual destructor generates a call to `operator delete(void*, size_t)`,
  which the 1999 binary does not have.

## Where OpenGL stands

Scaffolded, designed, and **deliberately not enabled**. `docs/opengl_activation.md` has the reasoning
and the exact safe-first set. Two findings shaped it:

- **Only three functions can carry the runtime `-gl` branch** - `InitDirectDraw`,
  `gfxPipeline::BeginGfx`, `gfxPipeline::EndGfx` - because a function carrying the branch must have
  no COM in its original body, or the fallback arm would have to be a hand retyping of DirectDraw
  against vtables this build has no headers for. Those three are also the only callers of the
  Begin/End pair in the whole image, so three is enough.
- **`gfxPipeline::Draw` has zero callers.** Every real draw goes through `gfxPacket::Draw` calling
  `lpD3DDev` directly, so the GL layer needs `gfxPacket`'s system-memory FVF path, not a vertex
  buffer object.

The frame loop is a one-way door: `BeginFrame`, `EndFrame`, `Clear` and friends all dereference
DirectDraw state, so there is no fallback arm to write and registering any of them replaces the
frame loop for every run.

# 1440p, measured

`GetClientRect` on the running game returns **2560 x 1440**. It reaches `GameLoop`, zero access
violations, and stays there until killed.

Four things had to be true, and each was found only after the previous one turned out not to be
enough:

1. The game must **advertise** it - `resolution-cap-width`/`-height` at `0x004AC2EE`/`0x004AC2F3`.
2. It must **see** it - dgVoodoo owns the mode list and had `Resolution = h:1920, v:1080`.
   `ExtraEnumeratedResolutions = 2560x1440` in `dgVoodoo.conf` fixed that (original backed up as
   `dgVoodoo.conf.before-openmm2`).
3. It must **select** it - a stored index in `gfxconf.dat`, set by `tools/setres.py`.
4. The frontend must **use** it - `Main` calls `gfxPipeline::SetRes(640, 480, 16, 16, 0)` with
   literal arguments, and that is what sizes the window.

Point 4 cost the most, and the way it went wrong is the lesson. The pseudocode shows the hardcoded
640x480 inside `BeginPhase`, so that is where I patched first - and the patch verified into the
binary, byte for byte, and changed nothing at all. `BeginPhase` does contain such a pair, but they
are register defaults for a call whose splash path never runs. A caller scan over the *disassembly*
found there are exactly two callers of `SetRes` in the whole image, and the one that matters is in
`Main`.

**Verifying that a patch landed is not the same as verifying that it did anything.** The bytes were
provably correct and provably irrelevant.

## Patches are now located by address, not by byte pattern

`tools/patches.py` used to find its target by searching for a unique byte sequence. That mechanism
kept failing on its own terms: `push 480` is five ordinary bytes and occurs six times, and the
obvious fix - adding leading context - dies immediately, because the instruction before this one is
a `call`, which ExportAsm emits as a symbolic `dd` rather than bytes. There is nothing contiguous
left to match against.

ExportAsm emits `.text` in strict address order with no ALIGN, and every directive's length is
known, so the address of every emitted byte is computable. A patch now states its address, and it is
checked twice: the retail binary must hold the expected bytes there, and the assembly must agree.
That is strictly stronger than any pattern, and it cannot be defeated by a common instruction.

## What 1440p does not yet mean

The interface is authored for 640x480. The scaling work in `docs/ui_scaling_progress.md` - the CSV
widget loaders, font sizing, and bitmap resampling at load - is what keeps it usable at this size,
and it is landed but not visually verified. Rendering is still DirectDraw through dgVoodoo;
OpenGL wave one is enabled and verified inert, and the frame loop is untouched.

## `gfxPacket` is recovered

The follow-on to "`gfxPipeline::Draw` has zero callers". `gfxPacket` — the class every real draw in
retail actually goes through — now has a full member layout read off instruction bytes, in
`docs/gfx_layouts.md` §16–§22, with `gfxReskin` (0x10) emitted in `data/layouts_gfx_round3.json` and
merging cleanly. The retail draw surface above `lpD3DDev` is exactly five functions:
`gfxPacket::Draw(uint)`, `gfxPacket::Draw(const Matrix44*, uint)`, `DrawList`, `OrthoMap`, and the
`gfxRenderState::DoFlush` that precedes them.

Three things a GL layer has to know before it writes a line:

- **`gfxPacket::0x16` is an index count, not a triangle count.** `layouts.json` and MM2Hook both say
  `TriCount` and are both wrong. Four independent proofs in §16.1; `modStatic::GetTriCount` divides
  it by three. Sizing an index buffer as `3 * [0x16]` over-allocates threefold.
- **`gfxMaterial` is 0x48, not the 0x44 in `layouts.json`** — now proven a fourth way, two-sided,
  by `modShader::Load`'s `sub esp, 0x48` frame holding one material (§20). `AddStaticMaterial`
  strides a pool of these, so the wrong size is live corruption. The hand edit is still outstanding.
- **The device vtable slots are pinned** (§18): `DrawIndexedPrimitive` at `+0x68`,
  `DrawIndexedPrimitiveStrided` at `+0x78`, `DrawIndexedPrimitiveVB` at `+0x80`, `SetMaterial` at
  `+0x40`. Getting `GetClipStatus` wrong in the DX7 order shifts three of those by four.

# Session three

## The pointer/text rule, settled by making the conflict explicit

A static audit found 21 pointers that a relocation had skipped, in four real tables - MSVC's
`_rterrs`, the `.com/.exe/.bat/.cmd` extension list, zlib's `z_errmsg`, and `DefaultPrinter`'s
message-prefix array. Two of them point into zeroed BSS, so the wrong pointers behave correctly and
no amount of running the game would ever have shown them.

The cause was three assumptions in `pointerTableRun`, each of which had looked reasonable:

1. **stride 4** - `_rterrs` has stride 8.
2. **ascending** - the extension list descends.
3. **monotone** - `z_errmsg` and the message arrays point at strings that were not emitted in
   address order, so the chain breaks at exactly one link and takes that entry with it.

What survives is *nearness*: a table's entries address the same neighbourhood at a fixed stride,
whatever order they ended up in. Four bytes of ASCII that happen to look like an address do not,
because nothing constrains the bytes beside them to point anywhere nearby.

That alone was not enough, and the failure is worth recording because it is the same collision as
before from the other side: pure nearness rewrote four real strings as pointers. The text rule and
the table rule genuinely conflict, and each has been wrong in its own direction - the text rule once
left half of `vehLoadNameList`, a 128-entry array of `char*`, as raw bytes.

**Length settles it.** A real table is long - `vehLoadNameList` is 128 entries, the CRT arrays ten or
more - while an accidental run through a string table is four or five and then falls apart. So four
entries suffice where nothing suggests text, and eight are required where something does. Findings
went 21 → 4, with `verify_strings` and `verify_dataformats` both still passing. The remaining four
are first entries whose chain breaks at one link; `tools/verify_relocated.py` reports them and is
deliberately not wired into the build yet.

## `asm.py` strips only what is registered

A correctly-guarded, deliberately-inactive `ARTS_EXPORT` inside a `#ifdef` that nothing defines
stopped every build in the tree: the declaration scan is textual and does not understand the
preprocessor. Requiring registration as well makes that harmless by construction rather than by
prohibition - an unregistered declaration simply does not strip, so there is nothing to fail. The
mismatch is still reported, just not fatally.

## Matrix33 / Matrix66: written, verified, and quarantined anyway

28 functions, with genuinely good layout work behind them - `Matrix33` = 0x24 and `Matrix66` = 0x90,
each from four independent witnesses. Both layouts are kept.

The **ports** crash the game, measured by bisect: without them 65,795 lines and zero access
violations; with them 61,218 lines and one, in `mmInterface::PlayerFillStats` with a misaligned
frame pointer. Neither `verify_extents` nor `verify_arity` objects to any of the 28, so it is not
the usual arity mismatch. `wip/README.md` records the evidence and the two leads worth taking first.

Keeping a working game is worth more than 28 functions.

# Session four

## Round 4 of the de-ossification port: 27 functions, none of them large

`data/ported_round4.json`. Three classes, chosen for being explainable rather than for being big -
the Matrix33/Matrix66 round proves that 28 functions which pass every static check can still take
the game down.

| file | functions | class |
|---|--:|---|
| `code/midtown2/mmai/aiintersection.cpp` | 18 of 29 | `aiIntersection` |
| `code/midtown2/mod/modstatic.cpp` | 2 of 10 | `modStatic` |
| `code/midtown2/mmgame/mmjaxis.cpp` | 7 of 7 | `mmJaxis`, complete |

`verify_extents.py` checks 214 ported functions and passes; `verify_arity.py` is unchanged at
6,315 agree / 12 mismatch, and none of the 27 is in the mismatch list **or** the skipped list.
Every extent was also cross-checked against the RE kit's `_INDEX.csv` for an interior function
start (the Trap 7 check) and all are clean. **Nothing here has been compiled or run.**

Seven behaviours were transcribed rather than corrected, and they are the point of the round:
`Road`/`Path` bound the index with `jg` and so accept `index == RoadCount`; the constructor leaves
six members uninitialised; `SetFourWay` clears the banger list; `NumOneWays` has no null test;
`modStatic::GetTriCount` is `(n * 21846) >> 16`, which is `n / 3` only up to 32767;
`mmJaxis::NormalizePOV` compares sixteen bits against `0xFFFF`; and `mmJaxis::Capture` returns -1
for a NaN reading, because `test ah, 1` is C0 alone and C0 is set by *unordered* as well as by
*less than*. That last one is the same instruction pattern as the `Matrix33::IsZero` change
`wip/README.md` names as the prime suspect for the quarantined round's crash, so it is written
`!(moved >= CaptureLo)` and not `moved < CaptureLo`.

## The vtable declaration order is a calling problem, not just a defining problem

The reason three more `aiIntersection` functions are not in that list, and it generalises far
beyond this round.

Trap 1 said a wrong virtual declaration order matters when you *define* a constructor or
destructor, because that is what emits `??_7C@@6B@`. It also matters when you merely *call* a
virtual: the slot index in `call [[this] + 4*slot]` is computed from the header's declaration
order. `py tools/asm_vtables.py --check asNode` reports asNode's eleven virtuals in alphabetical
order rather than the binary's, and a twelfth that occupies no slot at all - so any virtual call
written against asNode or any of its descendants dispatches to the wrong function, silently, in a
translation unit that defines no vtable and produces no duplicate symbol.

`aiIntersection::Reset` tail-jumps slot 3 of `aiTrafficLightSet`, `~aiIntersection` calls slot 0,
and `StopSignOkayToGo` calls slots 11 and 18 of `aiVehicle`. All three stay `ARTS_IMPORT`.
`aiTrafficLightSet` itself passes the checker, which is why the **non-virtual**
`aiTrafficLightSet::SetFourWay` is called normally. Trap 1 now carries this as point 5.

## Two smaller findings worth keeping

- **`gfx/gfxpacket.h` does not compile.** Line 38 declares a parameter as `unsigned const i16*`,
  and `unsigned` cannot be applied to a typedef-name; `genheaders.py` emits that spelling for
  every `unsigned short const *` in the map. Nothing in the tree has ever included it -
  `agigl/glpacket.h` mirrors the struct instead, and `mod/modstatic.cpp` now does the same for the
  two fields it needs, with `static_assert`s on the offsets. This is a class of latent breakage
  that will bite the first port to include a generated header with such a parameter.
- **`mmJaxis` was retyped, not relaid-out** (`data/layouts_round4.json`, merged into
  `data/layouts.json`). `ctor_layouts.py` had the offsets and the `0x1C` size right but called the
  first four members `i32`, because a constructor storing a dword of zero cannot say whether the
  dword is an integer or a float, and it left `0x10`/`0x14` as one opaque eight-byte blob because
  no constructor writes them at all. Every function in the class reads those offsets with x87
  instructions. Offsets and size are unchanged, so nothing in the binary moves.

# Session five: five stale operands, and a real gameplay milestone

## The operand fix

`tools/verify_operands.py` (new gate) disassembles all 9,306 sized functions and reads only the
disp/imm fields at the offsets capstone reports, rather than sliding a window. That reduced 6,218
raw window hits to **5 true findings**, all writes, all in the CRT's MBCS setup:

```
or  byte ptr [eax + 0x6b4e81], 8       __setmbcp
or  byte ptr [eax + 0x6b4e81], 0x20    _setSBUpLow
and byte ptr [eax + 0x6b4d80], 0       _setSBUpLow
or  byte ptr [eax + 0x6b4e81], 0x10    _setSBUpLow
or  byte ptr [eax + 0x6b4e81], 0x20    _setSBUpLow
```

`0x6B4D80` is `_mbctype` and `0x6B4E81` is `_mbcasemap`. **21 of 25 references to that table were
relocated and these were not**, so every read landed correctly and only the writes went astray -
which is exactly why nothing caught it. They scribble into `Stream::sm_Buffers`, the twelve 4 KB
file-I/O buffers, and `or ..., 0x20` turns a `0x00` into a space: it destroys NUL terminators.

The cause was mine. When a byte-window sweep rewrote a live instruction in `skip_variable` and broke
the JPEG decoder, I constrained the sweep to trailing operands - `pos+4 == len` or `pos+8 == len`.
`or byte ptr [eax + disp32], imm8` is `80 88 <disp32> <imm8>`: seven bytes with the address at
offset 2, so neither reached it. The fix allows `pos+5 == len` as well, and deliberately **not**
`pos+6`, because `skip_variable`'s `C7 42 14 5B 00 00 00` would match that and bring the old bug
straight back.

Gate now reports **0 unrelocated operands**, with `verify_strings` and `verify_dataformats` still
passing.

## What is actually reachable now

The user drove the game manually - menus, car select, into a race - and it **loaded San Francisco
and started gameplay**: SDL geometry, decals, landmarks and facades, props, weather, then
`AIMAP.Init: Initialize the Physically collidable cars` / `AIMAP.Init: Done!` and `GameLoop` at
**14.4M heap** against 3.2M for the frontend. 318,292 lines of debug output.

To be clear about attribution: that was the user playing, not a fix reaching further on its own.

## The next defect, found by playing

An access violation in `?CollideInstances@dgPhysManager@@QAE_NPAVlvlInstance@@0@Z +0x2DB`:

```
mov ecx, [ebp-0x18]     ; first instance
mov eax, [ecx]          ; its vtable
call [eax+0x14]
mov [esi+0x14], eax
mov edx, [edi]          ; <-- faults; edi should be the second lvlInstance*
mov ecx, edi
call [edx+0x14]
mov [esi+0x18], eax
add esi, 0x44           ; stride 0x44: an array of collision records
```

`edi` holds `0x42084208`, which is the float `34.064`. A pointer is being read from a field that
holds a float, so this is a layout or relocation problem rather than a physics one - and
`lvlInstance` is one of the classes `docs/short_layouts.md` flags, where inlined derived
constructors would "prove" sizes of 0x3C/0x40/0x58 for a class that is really 0x14.

# Session six: the format-string corruption

## What was wrong

Four printf format strings were being relocated as if they were pointers:

| want | got |
|---|---|
| `%s_` | `` %c` `` |
| `#%d` | `#\x15e` |
| `%5d` | `%%e` |
| `%4d` | `%$e` |

`"%5d"` is the bytes `25 35 64 00` = `0x00643525`, which lands inside `.data`, so it looked exactly
like an address - and `+0xF000` turned it into `"%%e"`.

`verify_strings.py` could not see this. It decides what counts as text by counting letters, two or
more, because three printable bytes are otherwise indistinguishable from an address. Format strings
are short and mostly punctuation: each of those four has exactly one letter.

**Corrupting a format string is worse than corrupting text.** It decides how `sprintf` reads its
arguments, so the wrong one makes it misread the stack - and MM2 formats into small stack buffers
constantly. `MainMenu::DisplayDriverInfo` `sprintf`s into `[ebp-0xc]`, twelve bytes.

## What it was causing

The crash the user hit while playing:

```
Exception c0000005 at EIP=0x40f5f9        <- PlayerFillStats, mov [ebp-4], -1
ExceptMain: Abnormal exit.                <- MM2's own handler catches it
Fatal Error: memMemoryAllocator::FindHeap failed on ptr 11a5d2a0
```

`ebp` was odd, so the frame was already smashed before that instruction; `FindHeap` is the
*consequence* of the resulting heap corruption, not a separate bug. After the fix,
`PlayerFillStats` does not fault at all.

## The rule that works

A `%` followed by optional flags, width and precision and then a conversion character is not a
coincidence in the way three printable bytes are. So format strings are now identified exactly
rather than by letter count, and a printf conversion **vetoes every other rule** - including the
pointer-table rule, which was relocating `"%5d"` anyway because it sits among enough plausible
addresses to look like a table. Losing a real pointer to that veto costs a stale pointer; losing a
format string costs a smashed stack.

`tools/verify_formats.py` is gate 14. 1,241 format strings, 0 corrupted.

## A tooling correction that matters more than it sounds

`tools/faultwatch.cpp` was terminating on **first-chance** access violations. MM2 uses structured
exception handling throughout - one run handled 18,529 guard-page exceptions - so a first-chance
fault the game deals with routinely was being reported as a crash, and the user's play session was
being killed mid-drive. It now terminates only on a second-chance exception, which is what a real
crash looks like.

Several earlier conclusions rested on that flaw and have been withdrawn, including "the section
layout shift crashes the game", which was the stated reason 55 written functions were held back.

`tools/dbgwatch.cpp` is the companion: it reads `OutputDebugString` out of the `DBWIN_BUFFER` shared
memory the way DebugView does, so a session can be captured while someone plays without a debugger
being attached to it at all.

## Where it stands

The game loads San Francisco and reaches `GameLoop` at 14.4M heap. Two faults remain, both narrower
than what they replaced:

- `dgPhysManager::CollideInstances +0x2DF` reading `0x14` from a **null** second `lvlInstance*`.
  Previously this held float bits; now it is null, so the shape of the defect has changed.
- `datStack::DoTraceback +0x46` - MM2's own crash reporter faulting while printing the trace, so a
  secondary effect rather than a cause.

# The game is playable

Confirmed by the user, not by a boot loop: **Cruise and checkpoint races, driven to completion, at
2560x1440, with no crash.** The captured session (`tools/dbgwatch.exe`, which reads the debug log out
of shared memory without attaching) shows three full city loads and `GameLoop` at 13.0M heap, with
zero exceptions, zero fatal errors and no abnormal exit.

The UI scales: in-game text and the pause menu render larger at 1440p, which is the font fix working
- MM2 authors text at fixed pixel sizes for 640x480, and `mmText::CreateLocFont` now takes
`round(baseSize * m_iHeight / 480)`, so glyphs keep their physical size instead of shrinking.

The remaining log noise is content, not code: `sf: room count mismatch` and two unrecognised parser
tokens come from addon maps installed in the game folder, and the missing tree LODs and
`nodeGetBitmap` misses happen in retail too.

## The layout blocker never existed

55 written and verified functions - Matrix33/Matrix66 and round 4 - were held back for most of a
session on the belief that growing `.text` past `0x1B8000` crashed the game. It did not. Every piece
of evidence for it came from `tools/faultwatch.cpp` terminating on **first-chance** access
violations that MM2's own exception handlers deal with routinely.

All 55 are now registered: **243 ported functions**, 851,725 lines of output, zero access violations,
zero unhandled exceptions.

Two lessons, both about method rather than about MM2:

- **An uncontrolled bisect is not evidence.** The first conclusion, "the Matrix ports crash the
  game", came from comparing two runs that did not reach the same place. An agent overturned it by
  proving those functions are unreachable - three independent scans found nothing outside those two
  classes calls them - and then by reproducing the "crash" with a file of dead, uncalled padding.
- **Instrumentation is not the program.** Twice a conclusion rested on what the debugger did rather
  than what the game did. The fix was to make the tool honest (terminate only on second-chance
  exceptions) and to add a tool that cannot interfere at all.
