# The assemble-and-link harness

This is the piece everything else depends on, and it is worth understanding before writing any
game code.

## Why

The naive way to do a decompilation project is to reimplement classes until there is enough to
link an executable. That gives nothing testable for months, and when it finally runs there are
thousands of untested functions and no way to tell which is wrong.

Midtown Madness 1 did the opposite, and it is why that project got to 90%. It starts from the
**whole original executable as assembly**, linked into the build. So there is a working binary on
day one: 0% reimplemented, 100% original machine code, but it runs. Then, one function at a time:

1. Write the C++ for one function.
2. Flip its declaration from `ARTS_IMPORT` to `ARTS_EXPORT`.
3. Run `tools/asm.py --write`, which strips that `PROC` out of `game.asm` and declares it `EXTERN`.
4. Rebuild. The linker now resolves that symbol to the C++ instead of the original bytes.
5. Run the game. If it still works, the function was right.

Every function is verified against the real game the moment it is written. A bug is always in the
one function just written. The assembly shrinks as the port advances, so "how much is left" is
literally the size of `game.asm`.

## How this one is built

`ExportAsm.java` (a Ghidra script, run headless) walks every function and emits a `PROC` per
function under its exact MSVC mangled name.

**It emits a mnemonic only where that mnemonic provably assembles back to retail's exact bytes,
and raw bytes everywhere else.** Operands holding an address become real symbols, patched in as
`dd`:

```asm
PUBLIC ?Update@mmGameManager@@UAEXXZ
?Update@mmGameManager@@UAEXXZ PROC
    push ebp
    mov ebp, esp
    db 08Bh, 00Dh
    dd offset ?Instance@asCullManager@@1PAV1@A     ; mov ecx, [asCullManager::Instance]
    db 068h
    dd offset ?AIMAP@@3VaiMap@@A                   ; push offset AIMAP
?Update@mmGameManager@@UAEXXZ ENDP
```

A relative call becomes `dd SYMBOL - ($ + 4)`, so when the target is later ported to C++ the
linker retargets it automatically. That is the entire mechanism.

### Why the mnemonic has to be proven, not just plausible

This file was originally all `db`, on the grounds that Ghidra's syntax is not MASM's — operand
order, size prefixes, hex literal spelling and segment overrides all differ — and each difference
is a chance to silently alter an instruction.

The reasoning was sound but the conclusion was too strong. The real hazard is narrower and
sharper: **x86 has redundant encodings**, so a correctly-spelled mnemonic can still assemble to
different bytes of a *different length*, which moves every address after it and breaks the layout
guarantee below. It does so silently, because the file still assembles and still links.

`tools/verify_encodings.py` settles which mnemonics are safe by measurement rather than argument.
It recovers every distinct encoding in the binary, assembles each one in its own 16-byte `ALIGN`
slot with the real `ml.exe`, and keeps only those that come back byte-identical. The survivors go
to `data/encodings.tsv`, and `ExportAsm.java` emits a mnemonic *only* for a byte sequence in that
table — so the text emitted is exactly the text that was verified, never a re-derivation.

Over 530,096 testable instructions:

| | count | share |
|---|---|---|
| emitted as mnemonic (byte-identical) | 529,228 | 99.84% |
| must stay `db` (MASM re-encodes) | 857 | 0.16% |
| must stay `db` (syntax unsupported) | 11 | 0.00% |

The 857 are genuine ambiguities. `fadd st, st` is `DC C0` in retail but `D8 C0` from MASM;
`test edx, eax` and `test eax, edx` differ only in the direction bit; `add eax, 0` has a short and
a long form. Those keep their bytes forever, and the fallback is per *instruction*, not per file.

Two further rules the table cannot express, both enforced in `ExportAsm.java`:

- **An instruction with a label inside it keeps its bytes.** `emitBytes` can place a label between
  two `db` directives, which is how a reference *into* an instruction resolves — a jump table
  computed off a base, or a jump into the middle of an instruction, both of which this binary
  really does. A single mnemonic line has nowhere to put that label.
- **The prologue must match the one `verify_encodings.py` assembles against**, `ASSUME GS:NOTHING`
  included. Otherwise the table's guarantee does not apply to the emitted file — one
  `add byte ptr gs:[eax], al`, a run of zero bytes in data-in-code, fails the assembly without it.

Anything that measures `game.asm` must go through `tools/encodings_table.py`, which owns the
text→bytes map. `tools/asm.py` uses it to size the padding that replaces a ported function, and
`tools/patches.py` to walk `.text` and locate a patch by address. Both used to count commas in
`db` lines; both would silently miscount a mnemonic line, and `asm.py` miscounting means `.text`
shrinks and every later address moves. Neither may treat an unmeasurable line as zero — they
raise instead.

## Layout must be preserved exactly

`.text` is emitted in **strict address order, with functions and jump tables interleaved and no
alignment padding**, so the section comes out byte-identical to the retail one.

This is not tidiness, it is correctness, and it was learned the expensive way. The first working
version emitted all functions in one pass and the jump tables in another, with `ALIGN 16` before
each `PROC`. It assembled and linked cleanly — and produced a binary in which every function had
moved: `sdlPage16::Draw` went from `0x448330` to `0x445950`, because the padding grew `.text` by
about 90 KB.

With no relocation table (see below), any absolute address that was not symbolised then points
somewhere wrong, and nothing reports it. Walking the section once, in order, keeps every address
valid whether or not it was symbolised — and as a bonus `.rdata` and `.data` land back at their
original virtual addresses, because section alignment puts them there once `.text` is the right
size.

## The relocation problem

`midtown2.exe` has **`IMAGE_FILE_RELOCS_STRIPPED` set and an empty base-relocation directory**.
There is no relocation table, so nothing in the file says which four-byte words in `.data` are
pointers and which are integers or floats that merely look like addresses.

Consequences, and what is done about them:

- **Code** is fine. Ghidra's analysis determines from context which operands are addresses, and
  those references drive the patching. After filtering out stack and register references — which
  Ghidra also reports as references but which are not addresses at all — only **3** references in
  the whole binary fail to resolve to a symbol.
- **Data** is emitted as one contiguous block per section with a label at every known symbol,
  which preserves the original intra-section layout exactly. A data word is turned into a symbol
  reference **only on an exact match against a known symbol address**. This is deliberately
  conservative: guessing more aggressively would silently corrupt data, and silent data corruption
  in a 1999 physics engine is not a debuggable failure.
- **Imports** are resolved through the IAT: a call into a DLL encodes the slot address, which is
  mapped back to the imported name and emitted as an `EXTERN __imp__Name:DWORD`.

## What builds today

```sh
py tools/implibs.py     # import libraries, from the binary's own import table
ml.exe -c -coff -Fogame.obj game.asm
py tools/link.py        # -> build/OpenMM2.exe
```

`game.asm` is ~900,000 lines and assembles with **zero errors**; the link produces a 2.72 MB
executable. Verified against the retail binary:

| | original | OpenMM2 |
|---|---|---|
| entry RVA | `0x181F5C` | `0x181F5C` |
| `.text` VA | `0x001000` | `0x001000` |
| `.rdata` VA | `0x1B0000` | `0x1B0000` |
| `.data` VA | `0x1C2000` | `0x1C5000` |
| `sdlPage16::Draw` | `0x448330` | `0x448330` |
| `aiPath::aiPath` | `0x543E40` | `0x543E40` |

**The one known gap: `.data` is 0x3000 high.** The linker builds its own import table and places it
in `.rdata`, on top of the original IAT that we already emit as data. `/MERGE:.idata=...` is
rejected outright (`LNK1272`), so it cannot be fixed from the link line.

The consequence is bounded but real. Code references into `.data` are fine — Ghidra's analysis
identifies them, and all 32,033 absolute references are symbolised. What is *not* covered is data
words that are pointers into `.data` which Ghidra did not flag: roughly 1,281 of them. Those still
hold their original absolute values and now point 0x3000 low. Closing this means either getting the
import table out of `.rdata`, or symbolising the remaining data pointers; the second is the more
robust fix, since it removes the dependency on layout entirely.

**The executable has not been run.** It links and its layout matches; whether it boots is the next
thing to establish, and the `.data` shift above is the most likely reason it would not.

## The whole of .text is verified, not a sample

`tools/verify_text.py` compares **every byte** of the built `.text` against retail, and runs as a
gate on every build. The earlier check tested the entry point and two anchor functions, which
catches a wholesale shift and nothing else — and ~11,000 functions with ~75,000 relocations have a
great deal of room for a single mis-encoded operand that assembles, links, and then misbehaves.

It immediately found one.

**A `call rel32` was being emitted as an absolute address.** `ExportAsm` asked "is this an import?"
before "is this encoding relative?", and a plain `call` targets a *thunk* — which Ghidra resolves
straight through to the import. So the import substitution fired and wrote an IAT slot address
into a relative field: `call +0x113E` became `e8 64 10 5b 00`, a branch to nowhere. Fixed by only
applying the import substitution to an absolute encoding.

Reaching a clean PASS meant teaching the comparison four things that legitimately differ. Each was
its own finding:

| | bytes still unexplained |
|---|--:|
| start | 41,582 |
| section displacement, **measured empirically** rather than read from headers | 13,642 |
| IAT slots — `call [__imp__X]` points into *our* import table | 1,410 |
| calls retargeted into ported C++ — the point of porting | 16 |
| CRT `_initterm` bounds, which our link places in `.rdata` though retail had them in `.data` | **0** |

The empirical calibration matters: `.rdata`'s *contents* move +0x1310 while its header moves
+0x1000, because the linker puts its import table at the front of the section. Assuming the header
value flags thousands of correct references as corruption. Measuring the mode of `built - retail`
per section is self-calibrating — a real displacement is shared by thousands of references, a
defect by a handful.

That calibration is also independent evidence the harness is sound: **4,341 `.text` references
agree on a displacement of exactly +0x0.**

```
bytes differing inside ported regions   : 2068   (expected)
bytes differing as relocated addresses  : 40167  (expected)
bytes differing as calls into ported C++: 2176   (expected)
bytes differing ANYWHERE ELSE           : 0
```

## Current numbers

| | |
|---|--:|
| Functions emitted | 11,076 |
| Absolute relocations | 32,033 |
| Relative relocations | 40,055 |
| Import relocations | 773 |
| Data pointers symbolised | 6,826 |
| References unresolved | 3 |
| Imports | 183 |

## Regenerating

Requires Ghidra with the project at `C:/mm2ghidra/proj` and symbols applied
(`ApplySymbols.java`, from `data/ghidra_symbols.tsv`):

```sh
analyzeHeadless.bat C:/mm2ghidra/proj MM2 -process midtown2.exe -noanalysis \
    -scriptPath C:/mm2ghidra/scripts -postScript ExportAsm.java
```

Two environment notes that cost real time here: the headless launcher **re-splits arguments on
spaces**, so every path must be space-free and forward-slashed; and Git Bash **converts `/c`-style
flags into `C:/`**, so MASM must be invoked with dash flags (`-c -coff`) rather than slash flags.

## The stale-pristine trap

`build.py` restores `game.asm` from `game.asm.pristine` before every build. That is correct and
must stay: stripping ported functions is not idempotent, and re-stripping an already-stripped file
pads the padding.

What was missing is that **nothing refreshed the snapshot.** It was written once, the first time a
build ran, and every export afterwards was overwritten by it at build time. The failure mode is
brutal precisely because it is silent:

- `ExportAsm` reports its work correctly.
- `game.asm` on disk contains exactly the right output — 256 recovered GUID pointers, verifiable
  by hand.
- The build copies the old snapshot over it, assembles that, and every gate passes, because the
  old assembly *is* internally consistent.
- The binary is byte-verified against retail and still has the bug you just fixed.

Hours went into a DirectInput fix that was landing in `game.asm` and being thrown away by the next
line of the build. The chase went through four wrong theories — an over-strict stride rule, a
Ghidra database polluted by an earlier pass, a disassembler re-covering the entries, and a symbol
that supposedly failed to relocate — none of which were the cause, and two of which produced
plausible-looking evidence.

Two things now prevent it:

1. **`ExportAsm.java` writes `game.asm.pristine` itself.** The export *is* the pristine copy, so
   the build no longer has to guess when the snapshot is stale.
2. **`build.py` refuses to build if `ExportAsm.java` is newer than the exported assembly**, which
   catches the other half of the same mistake — editing the generator and not re-exporting.

The general lesson, which applies beyond this file: a verifier that only compares the build against
retail cannot catch a build that silently used the wrong input. Check that what you generated is
what you consumed.

## Counting adds is not counting things

`findTextPointers` reported "pointer dwords embedded in .text: 66,985". The real number is **307**.

The counter incremented on every `Set.add`, and the stride walk re-adds the same array entries from
every anchor it finds — roughly two hundred times each. Three separate rule changes were evaluated
against that number as if it meant coverage, and it never did; the set had exactly the same 307
members each time. Report `set.size()`, not a counter, whenever the thing being counted is
deduplicated.

## Three bugs with one shape

Three separate failures this session shared a single blind spot, and it is worth naming because the
verifiers were all passing while each one was live.

| What broke | What the bytes looked like |
|---|---|
| DirectInput GUID pointers never relocated | **identical to retail** — because they were never touched |
| `"tga"` rewritten as a pointer | **differs by exactly the section delta** — same as a correct relocation |
| A live instruction rewritten as a pointer | **differs by exactly the section delta** — same as a correct relocation |

`verify_text` and `verify_data` compare against retail and ask whether each difference is
*explainable*. Both classes of bug are invisible to that question. An untouched pointer produces no
difference at all; a corrupted string or instruction produces exactly the difference a correct
relocation would. The verifier reported "0 unexplained" every time.

What actually caught them:

- **The GUID pointers**: a gate that knows the structure (`tools/verify_dataformats.py`). A
  DIDATAFORMAT states its own `dwNumObjs` and `rgodf`, so every entry can be checked by name rather
  than by delta.
- **The string**: a gate that knows what text is (`tools/verify_strings.py`). It never reasons about
  deltas — it finds strings in retail and demands they be present, unchanged, in ours.
- **The instruction**: running the game. Nothing static caught it.

The general lesson: **a differential verifier cannot check what the difference means.** Every check
of the form "our bytes differ from retail in a way we can account for" is blind to a wrong value
that happens to be accountable. Gates that assert positive facts about content — this structure has
seven entries, this string says "tga" — catch what delta arithmetic cannot.

## Telling a pointer from a string, and why it took four attempts

In `.data` a four-byte run of ASCII is numerically indistinguishable from an address. A pointer into
this image has a zero high byte and a second byte in `0x40..0x6B`, which is squarely inside the
printable range. `"tga\0"` is `0x00616774`, and the binary has a symbol at exactly `0x00616774`.

Four rules were tried:

1. **Exact symbol match** (the original). Relocated `"tga"`. With ~10,000 data symbols, collisions
   are ordinary, not exotic.
2. **Ghidra's string typing.** Missed it — Ghidra had never typed those bytes as a string.
3. **A strided run of pointer candidates.** Missed it, for a different reason: the dword had a
   recorded reference, so it never went through the exact-symbol path at all. It got a name from
   `nearest()`, which answered `?PLAYER@@3PAVmmPlayer@@A + 219700`. A pointer 214 KB past the
   nearest symbol should have been the tell.
4. **What works:** two letters in the dword, plus a neighbouring string on either side, past
   padding. Letters separate text from an address that merely prints — `b',w\'` is `0x005C772C` and
   has one letter, while `"tga"` has three. Company separates a table of strings from a table of
   pointers. Both directions are needed: entries at the start or end of a table only have a
   neighbour on one side.

The same rule is implemented twice on purpose — in the exporter, which must decide, and in
`verify_strings.py`, which must catch the exporter being wrong.

## The database is state, and state accumulates damage

Ghidra's database is written back to by every pass that clears or creates a listing. An
experimental rule here cleared roughly 32,000 listings; the clears persisted after the rule was
reverted.

The consequence surfaced much later, as an illegal-instruction crash inside the JPEG decoder:
`skip_variable` contains `C7 42 14 5B 00 00 00` (`mov dword ptr [edx+0x14], 0x5B`), and the four
bytes at `+1` are `0x005B1442`. With the listing cleared, nothing recorded that those bytes were
part of an instruction, so the exporter read them as an `.rdata` pointer and relocated them by
`+0x4310`.

No rule change could have recovered from that — the evidence had been deleted. The fix was
`C:\mm2ghidra\reimport.sh`, which rebuilds the database from the binary.

Two consequences worth keeping:

- **Measure a rule change against a freshly imported database**, or the result reflects the sum of
  every previous experiment rather than the change under test.
- **`protectTextPointers` now acts only on pointers found by reading a structure's own header**,
  never on heuristic ones. It deletes instructions, and a heuristic that is wrong about one address
  would have it erase real code and emit a relocation in its place — which, per the table above, no
  byte comparison can see.
