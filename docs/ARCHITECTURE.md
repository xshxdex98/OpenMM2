# OpenMM2 architecture

Written for someone who has never seen this tree and is sceptical of it. It describes what the
project actually is, why it is built the way it is, and where it is weak. If you read two sections,
read **The model** and **Known weaknesses**.

---

## What this is

An in-progress reimplementation of *Midtown Madness 2* (Angel Studios, 2000) in C++, plus an OpenGL
renderer intended to replace the DirectDraw / Direct3D 7 path so the game runs on modern hardware
without a wrapper such as dgVoodoo.

It follows the method and conventions of [Open1560](https://github.com/0x1F9F1/Open1560) by Brick,
which did the same for *Midtown Madness 1*. Where a decision here looks strange, Open1560 usually
made it first and for a documented reason.

Current state, measured rather than estimated:

| | |
|---|---|
| Functions reimplemented in C++ | **272 of 9307** |
| `.text` reimplemented, by bytes | **35,248 of 1,611,585 - 2.19%** |
| Classes with a recovered member layout | **245 of 535** |
| C++ translation units | 48 |
| Build gates, all passing | 17 |

**98% of what executes is still the original 1999 machine code.** That is the most important piece
of context for reading anything else here, bug reports included.

---

## The model

This is not a from-scratch rewrite and it is not a wrapper. It is a **de-ossification harness**, the
same approach Open1560 uses.

The retail binary is disassembled to MASM source (`game.asm`, produced by a Ghidra exporter in
`tools/ghidra/`). That assembly is reassembled and linked *together with* our C++ objects into a new
`OpenMM2.exe`. Every function exists exactly once in the output: either as the original machine
code, or as our C++.

Which one you get is decided by a single marker in the generated header:

    ARTS_IMPORT void Transpose();   // still the original assembly
    ARTS_EXPORT void Transpose();   // reimplemented in C++

Registering a symbol in `data/ported.json` makes `tools/asm.py` replace the original PROC with
**padding of exactly the length removed**, so every other function keeps its retail address. The C++
definition then satisfies the symbol at link time.

### Why this and not a rewrite

Because it runs from day one and keeps running. A from-scratch rewrite of a 1.6 MB game is not
playable until it is essentially finished, and every bug in it is a bug in code with nothing to
compare against. Here a newly ported function can be checked against the original's behaviour
immediately, the game is playable at every commit, and a regression bisects to one function.

The cost is that the build is unusual and the constraints below are not negotiable.

### Link constraints, all forced

- **No modern CRT, in practice.** `-NODEFAULTLIB` itself is gone, so a symbol `game.obj` does not
  define can be satisfied from `libcmt` rather than being a hard link error needing a hand-written
  stub. That is the whole of the gain, and it is small. Three walls still stand, all measured:

  1. `libucrt`'s objects are not self-contained. Pulling any one drags the UCRT runtime with it,
     and that references **stdcall-decorated** Win32 imports (`__imp__SetLastError@4`) which the
     import libraries here deliberately do not provide.
  2. Nothing needing CRT **initialisation** works - the entry point is the game's 1999 `start`, so
     the modern initialiser never runs. No `printf`, no `malloc`, so no `std::` containers.
  3. The floating-point routines must stay 1999 regardless, or `/arch:IA32` below is pointless.

  In practice: use the game's `Printf`/`Displayf` and its `operator new`.
- **`operator new` / `delete` are the game's**, at `0x577360` / `0x577380`, routing into
  `memMemoryAllocator`. C++ `new` must resolve to those.
- **`-FIXED -BASE:0x400000`.** There is no relocation table; the image loads at one address.
- **`/arch:IA32`.** The original is x87. Building SSE2 collapses 80-bit intermediates to single
  precision and silently changes physics results.

---

## What this repository may not contain

`build/` and `MM2_RE_KIT/` are gitignored for **copyright, not merely size**:

- `build/OpenMM2.exe` is the retail binary with our code linked into it.
- `MM2_RE_KIT/` is Hex-Rays output derived from `midtown2.exe` (36 MB).
- `game.asm` is generated outside the tree entirely.

A builder supplies their own copy of the game. Nothing derived from the retail binary is
distributed here.

---

## Layout

    code/midtown2/   the C++, mirroring the game's own subsystem names (agi, gfx, mm, veh, ph, ...)
      agigl/         the OpenGL device layer - the largest body of genuinely new code
    data/            the source of truth, see below
    tools/           build pipeline, verifiers, and RE utilities
    docs/            findings, defect write-ups, and this file
    checkpoints/     pre-VCS snapshots; superseded by git, kept because the docs reference GOLDGATE

### The data files, and which is authoritative

| File | Role |
|---|---|
| `data/symbols.json` | Every symbol from the retail linker map. **`rva` is an absolute VA - never add the image base to it.** |
| `data/layouts.json` | Recovered class member offsets. A class cannot be ported without these. |
| `data/ported.json` | Registry of reimplemented symbols. **Additive: the build merges every `ported_*.json` in and never removes anything.** Backing a port out means editing this by hand as well. |
| `data/keep_original.json` | Symbols whose original PROC is *renamed* rather than padded, so a `-gl` branch can still call the 1999 body as its fallback. See `gfx/origfallback.h`. |

The retail linker map is ground truth for names and addresses. `MM2_RE_KIT` is **not** trusted:
several hundred of its labels are pasted in from a foreign symbol set, and its index is wrong in
places.

---

## The build

`py tools/build.py` runs everything. Seventeen steps, and the verification is the point:

     1 merging ported lists                     10 verifying layout against the retail binary
     2 checking layouts.json against the         11 verifying .text byte-for-byte against retail
       binary's own allocation sizes             12 verifying .rdata/.data byte-for-byte
     3 regenerating headers                      13 verifying DirectInput data formats
     4 compiling                                 14 verifying printf format strings
     5 verifying symbols                         15 verifying string data survived the export
     6 verifying ported extents do not           16 verifying relocated operands are whole operands
       swallow other code                        17 recording progress
     7 stripping ported functions
     8 assembling
     9 linking

Steps 11 and 12 are differential: every byte outside a ported function must match retail exactly.
Most of the rest exist because a specific class of silent corruption got through once. Three are
worth understanding before judging the design:

- **Step 5, `verify_symbols`.** If a mangled name is wrong by one character the linker quietly keeps
  the assembly version and the C++ never runs - indistinguishable from a working port. This step is
  the only thing that catches it.
- **Step 6, `verify_extents`.** A function is stripped by its recorded extent, and that extent is
  distance-to-next-symbol. If unnamed code lives in the gap, stripping deletes it. It also catches
  the subtler case where `.data` holds a pointer *inside* a function - a switch jump table - which
  cannot survive the symbol being repointed at C++.
- **Step 16, `verify_operand_windows`.** The exporter once invented relocations by scanning 4-byte
  windows for values landing in `.data`. A window can end where an operand ends and still begin
  inside the opcode, silently rewriting a **structure offset**. Byte-differential gates cannot see
  this even in principle, because a false relocation differs from a correct one by exactly the
  section delta. This step decodes instructions and requires every changed dword to be a whole
  operand.

The lesson those encode, and the one worth carrying: **when a differential verifier reports zero,
ask what it cannot express.**

---

## Porting a function

1. Find a candidate, and screen it **before** writing any code against three tests:
   - `tools/verify_arity.py` - the mangled name's stack bytes must equal the code's `ret imm16`.
   - Step 6's test - no dword in `.data`/`.rdata` may point strictly inside the extent.
   - Purity - no calls and no absolute addresses, so it needs no named global and no unrecovered
     layout.

   **3,360 unported functions pass all three.** That figure was 146 until step 6 was corrected to
   measure the PROC it actually strips rather than the map's distance-to-the-next-symbol, which is
   larger than the function for 6,813 of 9,306 code symbols and was rejecting 895 functions on
   dwords lying in a NEIGHBOUR. `verify_arity.py` was never affected - it already read the kit's
   real extents - and it rules out only 12 symbols outright.

   A function needing a global is no longer blocked either: `data/globals.json` gives one a
   linkable name and `tools/asm.py` publishes it over the label ExportAsm already emits.
2. Read the **disassembly** (`py tools/disasm.py name=VA`), not only the decompilation.
3. Write the C++ and flip `ARTS_IMPORT` to `ARTS_EXPORT`.
4. Add the mangled symbol to a `data/ported_*.json`.
5. `py tools/build.py`.

### Traps, each of which cost a wrong answer first

- **`bool` mangles `_N`; `b32` mangles `H`.** MM1 used int-as-bool so Open1560 spells it `b32`; MM2
  uses real `bool`. Get it wrong and the port silently does not take effect.
- **The map's signature can disagree with the machine code.** Twelve symbols in the image encode one
  stack size and pop another, four of them in `Vector3`. Those cannot be ported *as declared* at all.
  `verify_arity.py` lists them.
- **Padding must equal bytes REMOVED, not the map's size**, which includes inter-function alignment
  the exporter already emits separately.
- **`check_size` is meaningless without members.** A class modelled as an empty struct has
  `sizeof == 1` on both sides, so the assertion passes and certifies nothing.
- **Hex-Rays guesses `__fastcall` on `__thiscall`**, emitting a body with one parameter too many,
  and drops x87 return values. If decompiled arity disagrees with the mangled signature, skip it.

---

## The OpenGL layer

`code/midtown2/agigl/` is the largest body of new code and the least like the rest: it is not a
transcription of anything, it is a Direct3D 7 device implemented on OpenGL.

`-gl` reaches the menus and gameplay at 2560x1440 with dgVoodoo removed from the game folder.
**`-gl` is mandatory** once dgVoodoo is gone: without a wrapper the original DirectDraw path cannot
set a display mode on a modern driver, and the failure presents misleadingly as "your display does
not meet requirements". See `docs/running.md`.

Two facts shaped the design and are easy to get wrong:

- **MM2 pushes render state, it does not pull it.** `DoFlush` transmits only the difference against
  `LASTRSTATE`, its own mirror of the device. A stub getter that returns a plausible lie makes the
  game overwrite its own state with fiction.
- **Never zero an out-parameter.** `GetLight` reads a light back into the same global the level
  builder is populating; helpfully zeroing it corrupts the level's lighting.

---

## Known weaknesses

Stated plainly, because this is the honest answer to "is it maintainable".

- **There is no test suite.** Verification is differential against the retail binary, which proves
  the *unported* code is untouched. It cannot prove a ported function is semantically correct: a
  wrong body that assembles cleanly passes every gate. Correctness rests on reading disassembly
  carefully, and that has failed before.
- **Only 2% is ported.** Most behaviour, and most bugs, are still the 1999 binary's.
- **`.rdata`/`.data` are displaced** (+0x1000 / +0x4000) because the linker builds its own import
  table over the original IAT and the C++ grows `.text`. Retail data addresses are therefore **not
  valid in this build** - a literal retail address will fault. The real fix is symbolising the
  remaining ~3,520 data pointers, and it is not done.
- **335 classes have a vftable the assembly declares PUBLIC.** Defining a constructor for one is a
  duplicate-symbol error, and `asm.py` can strip PROCs but not data. About 66 portable classes have
  no vtable; that is the current safe runway.
- **Paths default to the author's install.** The Ghidra output directory and the game binary are
  read from `MM2_ASM_DIR`, `MM2_EXE` and friends, so they are overridable rather than truly
  hard-coded - but the defaults are absolute paths on one machine, and nothing prompts for them.
- **Comment density is deliberately high.** Much of this code encodes a fact that took a day to
  establish and is invisible in the code itself. That is a considered trade, not an accident, but it
  is a fair thing to dislike.
- **Open defects** are in `docs/gameplay_defects.md`, along with what was investigated and
  *disproven*. Several confident wrong answers are recorded there on purpose.

---

## Building

Requires Visual Studio (x86 toolchain), Python 3, and a copy of Midtown Madness 2. Paths are
currently hard-coded; see the constants at the top of `tools/build.py` and `tools/link.py`.

    py tools/build.py                  # 17 steps; prints BUILD OK and the ported percentage
    py tools/build.py --stamp-export   # after re-running ExportAsm.java, and only then

The second command records the SHA-256 of the exporter that produced the current
`game.asm.pristine`, in `data/export_stamp.json`. The build refuses to run if the exporter has
changed since the export, because a fix made in the generator and not re-exported verifies
correctly and is silently absent from the binary. The check is on content rather than timestamp:
mtime cannot distinguish an edited generator from one a `git clone` merely touched.

The game must run with the MM2 install as its working directory, because it opens `EBUeula.dll` and
its `.ar` archives by relative path.
