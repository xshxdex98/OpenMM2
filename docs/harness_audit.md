# Auditing the gates: looking for defect number six

Five defects have stood between "assembles and links" and "runs", and every one of them passed
every byte-level verifier that existed at the time. Each is now covered. This is an attempt to find
the sixth **before** it costs a night, by asking what question each gate asks and then looking for a
kind of wrongness that no question reaches.

Nothing here was found by running the game. Everything is measured against
`midtown2.exe` and `build/OpenMM2.exe` as they stand.

## The question each gate asks

| Gate | The question | What that question cannot reach |
|---|---|---|
| `verify_text.py` | does this `.text` byte differ from retail, and is the difference explainable? | a byte that did not change but should have; a change that is explainable and still wrong |
| `verify_data.py` | same, for `.rdata` and `.data` | identical |
| `verify_strings.py` | is every string retail has still that string in ours? | anything that is not a string |
| `verify_dataformats.py` | did every DirectInput GUID pointer move? | every other pointer |
| `verify_extents.py` | does retail point into the middle of anything we are about to strip? | reads retail only; says nothing about the built image |
| `verify_arity.py` | does a `__thiscall`'s `ret imm16` match its mangled parameter list? | reads retail only; and 417 symbols it declines to judge |
| `verify_symbols.py` | do the objects define exactly the mangled names on the ported list? | whether those names are the right ones |

The common shape, already named in `harness.md`: **a differential verifier cannot check what the
difference means, and it is completely blind to the absence of a difference.** Four of the five past
defects were one or the other.

Three of the seven gates never read the built binary at all. Of the four that do, three answer
"does this differ", and one — `verify_strings` — is the only one that asserts a positive fact about
content. That imbalance is where the search started.

## Ruled out, with the measurement

**`check_size` assertions that certify nothing.** `genheaders.py` was fixed: a `check_size` is
emitted live only when the class has a real member layout, and a known-size-unknown-members class
gets a commented guard instead. Counted across `code/midtown2`: **216 live, 319 commented out, 0
marked `UNRECONCILED`**, and exactly one live assertion sits on a class without offset-annotated
members — `agiPixelFormatDescriptor` in the hand-written `agigl/glcommon.h`, which is not generated
and does declare real fields. The 32-class hollow-assertion state is gone. Nothing to do.

**Unrelocated absolute operands in `.text`.** This is the `.text` twin of the defect below and would
be worse, so it was checked directly: every dword in `.text`, at every byte offset, whose retail
value lands anywhere in `.rdata` or `.data` *including the BSS tail*, and which is byte-identical in
our build. 6,218 candidates, of which **4 land exactly on a named data symbol** — and all four are
coincidences that dissolve on inspection:

```
0x0042EFE5: ... 68 00 00 C8 44   68 00 00 20 41   68 00 00 A0 3F
              push 44C80000h    push 41200000h   push 3FA00000h
                (1600.0f)         (10.0f)          (1.25f)
```

The "pointer" `0x006844C8` is a four-byte window straddling two `push` immediates. Same for
`0x006A3D4C`, which straddles `push 3D4CCCCDh` (0.05f). `.text` is clean of this class. That is
consistent with `harness.md`: only 3 references in the whole binary failed to resolve.

**Relocations applied to something that merely looks like an address.** The inverse defect — a float
or a length rewritten as a pointer, which is what happened to `"tga"`. `verify_strings` covers text;
nothing covers the rest. Tested with a positive-fact rule the existing gates do not use: a relocated
dword whose target lands *strictly inside* a letter-bearing NUL-terminated string is almost certainly
not a pointer. Across both sections, **one hit**: `0x005D71AC` points one byte into `"?break0"`,
which is the ordinary idiom of a table naming a string past a prefix character. No defect. There is
no evidence of over-relocation in the current build.

**Vftable slots pointing into a ported function's interior.** Already covered — that is exactly
`verify_extents.py`, and it passes for all 215 ported symbols. The neighbouring case, a vftable slot
that points at a ported function's *entry* and was left unrelocated, is **not** covered by anything,
because it is byte-identical to retail; it is currently clean (0 occurrences) and is now checked as
rule 2 of the new gate, since it becomes possible again with every port.

**Calling-convention mismatches.** `verify_arity.py` reports 6,315 agree, **12 mismatch**, 417
skipped. The skips are honest and stated — by-value class parameters, tail calls, no usable size.
The known hole is real but narrow: `?InvScale@Vector3@@QAEXM@Z` pops the right number of bytes while
its argument is a pointer rather than a float, and no `ret imm16` can distinguish those. Closing it
means comparing the *use* of the argument in the body (an `fld [ebp+8]` versus a `mov eax,[ebp+8]`),
which needs a disassembler and is a different tool. Left alone: it is a known, documented, one-symbol
gap, not a class of silent failure.

## Finding 1 — pointers a relocation skipped, inside tables it did not

**This is the one worth gating, and the current build fails it.**

`.rdata` and `.data` do not land at their retail addresses, so every pointer into them must be
rewritten. Measured on the build as this was written, `.rdata`'s content moves **+0xA310** (694
pointers agree) and `.data` moves **+0x10000** (242 agree). Those numbers are not constants — the
build was rebuilt from 187 to 215 ported functions midway through this audit, `.text` grew, and both
displacements changed (from +0x9310 and +0xF000). The gate re-measured them and returned the
identical 21 findings, which is the point of measuring the mode rather than reading a header.

Where the exporter decides a dword is not a pointer, it keeps its retail value — which produces **no
byte difference at all**, so `verify_data` reports it as correct and `verify_strings` never looks at
it.

Two of the five past defects were exactly this shape: the 307 DirectInput GUID pointers, and half of
`vehLoadNameList`. Both were found by running the game.

Counting is not the problem; deciding is. **349** dwords in `.rdata`/`.data` hold a plausible address
into a moved section and did not move, and most of them are four bytes of ASCII — `"cab"` is
`0x00626163`, `"bug"` is `0x00677562`, both numerically inside `.data`. Reporting those is how a gate
gets turned off.

What decides it is the same thing that decides text in `verify_strings`: **company**. A pointer lives
in a table, and the exporter relocated its neighbours. `tools/verify_relocated.py` reports a dword
only when all three hold:

1. it holds an address in a section that moved, and it did not move;
2. at least **two** other entries at the same stride, within four steps, moved by exactly that
   section's measured displacement;
3. those entries point at addresses **within 4 KB per step** of this one.

Rule 3 is load-bearing and was not obvious. Without it the checker reports `"bug"`, `"cab"`, `"age"`,
`" %d"` and `"z%d"` — the tails of the very strings that the adjacent pointer table points *at*,
which is precisely why they sit beside relocated pointers. Their values are 300–850 KB away from
what the table addresses; a real entry is 4 to 600 bytes away. Six orders of magnitude of margin.

That margin was measured, not assumed. Sweeping the three constants:

| near-per-step | reach 2 | reach 4 | reach 8 |
|---|--:|--:|--:|
| 0x200 – 0x4000 | 5 | **8** | 15 |
| 0x10000 | 5 | 8 | 19, **3 string false positives** |
| 0x40000 | 5 | 15, **6 FPs** | 28, **7 FPs** |

The chosen point (0x1000, reach 4, two witnesses) sits on a plateau that is completely stable across
a 32× range of the distance constant, and the first false positive appears only at 16× the chosen
value combined with double the reach. Loosening to reach 8 finds no new table — only more entries of
the two it already found — so the tighter setting costs nothing.

A separate, bounded pass then convicts the neighbours of an already-convicted entry: within one
table's reach, and pointing within 4 KB of what the convicted entry points at. Without it the gate
reports the *first* broken entry of a run and stays silent about the rest, because the rest have no
relocated witness left — their witnesses are the broken ones. That is the `vehLoadNameList` shape
exactly, and reporting half a fault is how the fourth defect happened in the first place.

### What it finds today: 21 pointers, in four tables

```
0x005CECF4  lpOutputString      -> 0x006A3D54  (?OutputSent@datOutput@@0_NA+0x8)
0x005CECF8  lpOutputString+4    -> 0x006A3D58
0x005DAA40                      -> 0x005DAA68  'need dictionary'
0x005DAA44                      -> 0x005DAA78  'stream end'
0x005DCD74  (CRT)               -> 0x005B6514  '.bat'
0x005DCF9C .. 0x005DD024 (CRT)  -> 0x005B6878 .. 0x005B6614   (16 entries)
```

**`lpOutputString` is on the hot path.** It is not a scalar; it is the base of a message-prefix array
indexed by severity, and `?DefaultPrinter@@YAXHPBDPAD@Z` — the function behind all 66,068 lines of
debug output — reads it six times as `mov eax, [esi*4 + lpOutputString]`, feeding the result to
`OutputDebugStringA`, `sprintf` and the `MessageBoxA` caption. Entries 3, 4 and 5 (`"Warning: "`,
`"Error: "`, `"Fatal Error: "`) relocated correctly, and so did entries 2 and 6. Entries 0 and 1 did
not, so for the two lowest severity levels the printer dereferences an address a whole section
displacement low.

Today that is invisible: 0x006A3D54 is in `.data`'s BSS, and a section displacement below it is also
BSS, so both read as an empty string and the output looks right. That is the profile exactly — a
wrong pointer with correct-looking behaviour, waiting for something to move underneath it.

**zlib's `z_errmsg`** is the other game-side one. `0x005DAA40` is the array `zError` indexes;
`ERR_MSG(err)` is `z_errmsg[Z_NEED_DICT - err]`, so the two unrelocated entries are precisely
`Z_NEED_DICT` and `Z_STREAM_END`. Eight of ten entries relocated. In our image those two addresses
now land in unrelated initialised `.data`, so `strm->msg` for those two statuses is garbage text.

The remaining 17 are MSVC CRT tables: the `_rterrs` runtime-error messages (`R6002 - floating
point...`), and the `.com`/`.exe`/`.bat`/`.cmd` extension list. Their targets are retail `.rdata`
addresses around 0x005B65xx, and in our image that range is inside `.text` (which now runs past
0x005B9000) — so `_amsg_exit` would print machine code. Fatal path only, but wrong.

Not reported, and worth knowing about: the `_matherr` function-name table at **0x005DD198** (stride
8, ~27 entries, `exp`/`pow`/`log`/`sinh`...) is in the same state, with only one entry relocated. It
has too few relocated witnesses left to convict under rules 2 and 3, and it sits outside the
propagation reach of the `_rterrs` table. The gate is deliberately conservative and this is the price;
the fix below removes it along with the rest.

### Why the exporter missed them

`ExportAsm.java`'s `pointerTableRun` is the rule that overrides the text heuristics, and it requires
a run that is **strictly ascending, at stride 4, in steps of at most 256**. Every table above fails
that for a reason that has nothing to do with being a table:

- `_rterrs` and the extension list **descend** — MSVC emitted their strings in reverse.
- `_rterrs` interleaves an `int` between pointers, so the real stride is **8**.
- the `lpOutputString` array ascends for three entries and then steps to a `.data` string, breaking
  the run at n = 3, one short of the threshold of 4.

The entries that *did* relocate got through the exact-symbol path instead, which is why each table is
broken in an arbitrary-looking pattern rather than wholesale. **The fix is to let `pointerTableRun`
accept a monotone run in either direction and at a stride other than 4.** Descending tables are as
ordinary as ascending ones; the rule was written against `vehLoadNameList`, which happens to ascend.

## Finding 2 — the retail resource directory is not in the built binary

Not a crash, and not gated here, but it is a straight loss that nothing reports.

```
retail  resource directory  rva=0x2B6000  size=1888
        ICON id 1 / 1033          744 bytes
        GROUP_ICON id 111 / 1033   20 bytes
        VERSION id 1 / 1033       876 bytes

built   resource directory  rva=0x2C6000  size=872
        MANIFEST id 1 / 1033      769 bytes      <- the DPI manifest, and nothing else
```

`ExportAsm.java` emits `.rsrc` into the `.CONST` segment (`emitDataBlock(w, ".rsrc", ".CONST", …)`),
so all 1,958 bytes of retail's resource image survive **verbatim** — as dead data at VA `0x005CB310`
inside our `.rdata`, unregistered in the PE resource directory and still carrying `OffsetToData` RVAs
of `0x2B60F0`/`0x2B6460`/`0x2B6748` that point at nothing. The linker builds a real `.rsrc` from
`-MANIFEST:EMBED` and that is the only resource directory the loader sees.

Consequence, checked rather than assumed: **no runtime path is broken.** The two `LoadIconA` call
sites push `0x7F00` (`IDI_APPLICATION`) with a null instance, so they ask for a system resource, not
ours; there is no `FindResource`, `LoadString`, `LoadMenu` or `GetFileVersionInfo` anywhere in
`game.asm`. What is lost is the executable's shell icon and its version information — visible in
Explorer, and to any external tool that identifies the game by version resource.

The fix is a link-line one, not an exporter one: extract retail's resources to a `.res` and add it to
`link.py`'s inputs so the linker merges them with the manifest, then drop the `.rsrc` block from
`emitDataBlock` so the dead copy stops occupying `.rdata`. Left undone deliberately — it changes the
link line, and this audit is not supposed to touch the build.

## Two smaller notes

**`tools/unrelocated.py` measures a range it does not mean to.** It bounds its target window with
`.data`'s *raw* size:

```python
hi = retail[".data"]["va"] + IMAGE_BASE + retail[".data"]["rsz"]
```

`.data` is `rsz` 0x1D000 but `vsz` 0xF3FB8, so this covers 11.9% of `.data`'s address range and
excludes the entire BSS tail — where most game globals live, and where `lpOutputString` points. Its
"whole `.text`: 89" is a count over that narrow window, and it is not a build gate, so nothing is
currently broken by it. But the number reads like coverage it does not have. Changing `rsz` to `vsz`
is a one-word fix; the section above establishes that the answer is still zero real defects, so it
would be a free correctness improvement to a diagnostic, not a new finding.

**The built image has an export directory; retail has none.** 7,700 bytes of it, at RVA 0x1CD470,
from the two-pass link that lets the assembly and the C++ resolve each other. Harmless, and
`verify_data` is immune because it compares only as far as retail's content reaches — but it is a
structural difference from retail that no gate mentions, and it is one of several reasons the built
`.rdata` is 0x5600 bytes bigger than retail's.

## The new gate: `tools/verify_relocated.py`

Reads the retail binary and `build/OpenMM2.exe` through `tools/pe.py` and `verify_data.py`'s
`sections()`, exactly like the others. Measures the section shift and the per-section displacement
rather than trusting a header — a real displacement is agreed on by hundreds of pointers, a defect by
a handful. Runs in 0.14 s.

It reports three numbers, and the third is the honest one:

```
pointer-valued dwords   : 936 relocated, 349 unchanged
unchanged, and sitting in a table whose other entries moved: 21
unchanged with no such evidence, so NOT judged here        : 328
```

The last line exists so that "21" is never mistaken for "there are 21 problems" or "the other 328 are
fine". It knows about 21; it declines to have an opinion about 328.

Each finding is printed as what is wrong rather than as a count — the address, its symbol, the value
it holds and what lives there, the value it should hold, and the sibling entries that prove the table
was relocated around it:

```
0x005CECF4  (lpOutputString)
    holds 0x006A3D54  (?OutputSent@datOutput@@0_NA+0x8), which is in .data, and did not move
    should now hold 0x006B3D54 (.data +0x10000)
    the table did move: stride 8, entries at 0x005CECFC, 0x005CED0C
```

### What it does not see

- a table where **nothing** was relocated. There is no witness, so there is no verdict.
- a two-entry table with one entry wrong.
- an unrelocated pointer that is not in a table at all — a lone `char *` global initialised to a
  string. `verify_dataformats` is the model for those: know the structure, check it by name.
- anything in `.text`. Different problem, and measured clean above.
- `.rsrc`, which is not a pointer problem.

### It is not wired into `build.py`

Deliberately. It fails on the current build, and a gate added to `build.py` today would block every
build until the exporter is fixed and re-run. Once `pointerTableRun` accepts descending and
non-4 strides and `ExportAsm.java` has been re-run, add it after `verify_strings`:

```python
step(13, "verifying every pointer that had to move did move")
res = subprocess.run([sys.executable, os.path.join(HERE, "verify_relocated.py")],
                     capture_output=True, text=True)
```

## Expected result

**It fails on the current build, with 21 findings in four tables, and it should.** Every one was
confirmed by hand against the retail bytes before the rule was written: the entries it names hold
retail addresses while two or three of their own table's entries, at a fixed stride, moved by
exactly the section's displacement. The `lpOutputString` pair is reachable from
`?DefaultPrinter@@YAXHPBDPAD@Z` on the debug-output path; the zlib pair is `z_errmsg[0..1]`.

It will pass once `ExportAsm.java` relocates monotone pointer runs in both directions and at strides
other than 4, and `game.asm` is re-exported — with the caveat from `harness.md` that the export must
then actually be the thing the build consumes.

Rule 2, the check for a data pointer left aiming at a function we have ported and filled with `0xCC`,
passes today: 0 of 215 ported symbols is pointed at by a stale dword.
