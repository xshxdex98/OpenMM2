# OpenMM2

An open source re-implementation of **Midtown Madness 2** (build `MM3393`), for research and
preservation.

This follows the model [Open1560](https://github.com/0x1F9F1/Open1560) established for Midtown
Madness 1: rather than rewriting the game, it *de-ossifies* it one function at a time. The
original binary keeps running, and each function that gets reimplemented in C++ replaces its
original. The game is playable at every step, so nothing has to work before everything works.

Midtown Madness 2 runs the same Angel Studios ARTS engine as Midtown Madness 1, with most
subsystems renamed:

| Midtown Madness 1 | Midtown Madness 2 |
|---|---|
| `agi` | `gfx`, `dg`, `Age` |
| `mmcity` | `lvl`, `city` |
| `mmcar` | `veh` |
| `mmphysics` | `ph` |
| `mmaudio` | `aud`, `Aud`, `DMusic` |
| `arts7`, `data7` | `as`, `dat` |

`asNode`, `datParser`, `Matrix34` and friends survive unchanged, so Open1560's engine-level work
is directly informative here.

## Why this is tractable

`midtown2.map` is a linker map carrying **10,608 mangled MSVC symbols with addresses**. A mangled
name is not just a name — it encodes access, virtualness, calling convention, return type and
every parameter. So the map alone yields most of the declaration of **537 classes** without
decompiling anything, and every byte of the 1.68 MB `.text` section falls inside a named symbol.

It also preserved original 1999 names that no decompiler could recover, including nested enum
types like `audManager::AUDTYPE`, `gfxImage::gfxImageFormat` and `mmHudMap::IconType`.

## Layout

```
tools/symbols.py      midtown2.map  -> data/symbols.json   (parse + demangle)
tools/layouts.py      IDB type dump -> data/layouts.json    (member offsets, sizes)
tools/mm2types.py     MM2:: types   -> data/mm2types.json   (sizes, bases, vtable slots)
tools/asm.py          strips a PROC when a function flips to ARTS_EXPORT
tools/asm_vtables.py  hands a class's vftable over to C++, and checks the slot order
tools/vtable_order.py midtown2.exe -> data/vtable_order.json (true virtual slot order)
tools/implibs.py      builds import libs for DPLAYX / DINPUT from the import table
tools/link.py         links game.obj into build/OpenMM2.exe
tools/ghidra/         ApplySymbols.java, ExportAsm.java (run headless)
tools/mangled.py      qualified-name recovery from mangled symbols
tools/genheaders.py   data/symbols.json -> code/midtown2/**/*.h
tools/vtables.py      midtown2.exe  -> data/hierarchy.json  (inheritance from vftables)
tools/pe.py           PE section reader, shared
tools/progress.py     measures the port, writes docs/PROGRESS.md, records history
tools/inventory.py    data/symbols.json -> docs/inventory.md
code/midtown2/        534 generated class headers, 7,787 declarations
docs/inventory.md     what the binary contains, and the porting order
docs/vtables.md       how a polymorphic class is ported, and what can still go wrong
docs/vtable_order.md  the recovered slot order, and how it was checked
```

Regenerate everything:

```sh
py tools/symbols.py && py tools/vtables.py && py tools/genheaders.py && py tools/inventory.py
```

## Building

```sh
py tools/build.py
```

That is the whole pipeline: merge ported lists, regenerate headers, compile, **verify symbols**,
strip the ported functions out of `game.asm`, assemble, link, **verify layout**, record progress.

The two verification steps are the point, because both failures they catch are silent:

- **Symbols.** A reimplementation only replaces the original if its mangled name matches exactly.
  If it does not, the object just contributes an unreferenced symbol, the assembly's version stays
  in the link, and the game keeps running 1999 code — with no error anywhere. The port looks
  finished and is not. (`bool` mangles `_N` and `b32` mangles `H`; that one difference would have
  silently disabled every ported predicate in the codebase.)
- **Layout.** `midtown2.exe` has no relocation table, so an absolute address that was not
  symbolised is only correct while the image keeps its retail layout. A build that shifts `.text`
  still links and still looks fine, so the entry point and two known function addresses are
  checked against the retail binary on every build.

Porting one function is: write the C++, add its mangled symbol to `data/ported.json`, run the
build. `tools/asm.py` replaces the original `PROC` with padding of exactly the same length, so
every other function keeps its address.

## Conventions

House style is Open1560's, documented in `CODE_STANDARD/BRICK_STYLE_ANALYSIS.md`. The rules that
matter most here:

- **`ARTS_IMPORT`** means the function is still the original code, merely callable.
  **`ARTS_EXPORT`** means it has been reimplemented in C++. Everything starts as `ARTS_IMPORT`.
- **Every declaration carries its exact mangled symbol in a comment above it.** That comment is
  the documentation — it is the only thing tying a C++ declaration back to an address in
  `midtown2.exe`. Never edit or drop one.
- **PascalCase names are provenance, not style.** A PascalCase member or method name is a name
  recovered from 1999. A name we invented gets `snake_case_` with a trailing underscore. Never
  normalise one into the other; it destroys information.
- **Members are in memory-offset order**, never alphabetical, because the original code reads them
  at fixed offsets. Methods *are* alphabetical, after the constructor and destructor.
- Types come from the arts primitive vocabulary — `i32 u32 f32 f64 b32 u8 usize` — never `int`,
  `float` or `bool`. `b32` in particular is load-bearing: it is `i32` in hooked builds and a real
  `bool` standalone.

## Class hierarchy

`midtown2.exe` was built without RTTI (`/GR-`), so there are no type descriptors and the map says
nothing about inheritance. The **vftables** do.

A derived class's vftable begins with its base's layout: every slot the derived class did not
override still holds the base's function pointer, and the map identifies the owner of every
address. Reading the 340 vftables therefore recovers the ancestry directly — `tools/vtables.py`
resolves a base for **272 of 335** classes, and `asCullable` turns out to be the root of 121 of
them.

This was checked against Midtown Madness 1: Open1560's hand-written header declares
`class asNode : public asCullable`, and the same relation falls out of Midtown Madness 2's vtable
bytes independently.

> **`midtown2.map` has a bug worth knowing about.** Its `Rva+Base` column is **0x308 too high for
> `.rdata`** — exactly the `.rdata` start value printed in the map header. Addresses in `.text`
> are unaffected, so code resolves correctly and the error is invisible until you read a vftable,
> at which point every lookup silently lands on a neighbouring table. The `section:offset` column
> is correct; all tooling here recomputes addresses from it against the real PE section table.

## The RE kit

`MM2_RE_KIT/` is a 2023 community IDA database unpacked into readable C — 9,429 decompiled
functions at 91.8% naming, plus 11,686 type layouts with byte offsets. It is what supplies the
member offsets a linker map cannot.

The binary was verified against it before anything was used: file size, all three section hashes,
and both spot-check byte sequences match. Independently, addresses derived from `midtown2.map`
agree exactly (`sdlPage16::Draw` at `0x448330`, `aiPath::aiPath` at `0x543E40`).

Two documented caveats, both confirmed:

- **~349 functions carry ImGui/ImPlot/LuaIntf names** from a foreign symbol set pasted over part
  of the database. The code is real MM2; only the label is junk. They are quarantined in
  `_MISLABELED_FUNCTIONS.c` and excluded from anything generated here.
- **The IDB's recorded md5 does not match this binary, and that is expected.** No-CD patching
  changes the header, not `.text`.

The two sources complement each other rather than overlap. The map has 100% coverage and full
parameter types (a mangled name encodes every argument); the IDB has struct layouts and readable
pseudocode. Neither alone is enough.

## Class hierarchy

`midtown2.exe` was built without RTTI (`/GR-`), so there are no type descriptors. Inheritance is
recovered from **vftable contents** instead: a derived class's vtable keeps its base's function
pointers in every slot it did not override, and the map identifies the owner of every address.

Picking the *immediate* parent out of that took three attempts. "Owns the earliest slot" returns
the deepest ancestor. "Longest vtable" fails when a class overrides without adding virtuals.
"Contributes the most slots" fails when a near parent overrides very little. What holds is: the
immediate parent is the **most derived of the contributing ancestors** — the one that has all the
others among its own ancestors.

Cross-checked against MM2Hook's independently-maintained type set, that rule agrees on **110 of
120** shared classes, up from 44 before the fix. The remaining 10 are classes whose real parent has
no vftable of its own and so is invisible to a byte-level method; MM2Hook's explicit base wins
there, and `base_of()` prefers it.

> **`midtown2.map` has a bug worth knowing about.** Its `Rva+Base` column is **0x308 too high for
> `.rdata`** — exactly the `.rdata` start value printed in the map header. Addresses in `.text`
> are unaffected, so code resolves correctly and the error is invisible until you read a vftable,
> at which point every lookup silently lands on a neighbouring table. The `section:offset` column
> is correct; all tooling here recomputes addresses from it against the real PE section table.

## Classes with virtuals

335 of the 536 code-bearing classes carry a vftable - 58.5% of the bytes in named code symbols -
and until recently none of them could be started, because the first constructor written in C++
makes MSVC emit `??_7Class@@6B@` and the assembly already defines it. `tools/asm_vtables.py` does
for a vftable what `asm.py` does for a PROC, without moving a byte of `.rdata`.

The second half of the problem is quieter. A compiler assigns vtable slots in **declaration**
order, and `genheaders.py` sorts methods alphabetically. For a polymorphic class that means every
original call through the table reaches the wrong method of the right signature - no link error, no
crash. `data/vtable_order.json` holds the real order for all 335 classes, read out of `.rdata` and
agreeing with MM2Hook's independent `_vtbl` structs on 2,147 of 2,158 slots.

`py tools/asm_vtables.py --check <Class>` reads a header back and says whether it would produce the
binary's table. Run it before flipping the first `ARTS_EXPORT` on any class with virtuals. See
`docs/vtables.md`.

## The harness

See `docs/harness.md`. This is the piece everything depends on: the whole original binary as MASM
that assembles and links, so there is a runnable executable at 0% reimplemented and each ported
function is verified individually. `tools/asm.py` strips a `PROC` when its declaration flips from
`ARTS_IMPORT` to `ARTS_EXPORT`.

## State

The harness builds. `game.asm` is ~900,000 lines, assembles with zero errors, and links into a
2.72 MB `build/OpenMM2.exe` whose entry point, `.text` and `.rdata` sit at exactly the retail
addresses, with functions byte-identical and in place. It has not been run yet, and `.data` is
0x3000 high for a reason documented in `docs/harness.md`.

No game code is reimplemented yet, and that is deliberate: the harness comes first, so the first
function written can be tested the day it is written.
