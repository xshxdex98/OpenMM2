# The hunt for pointers that are wrong in every build

Companion to `docs/layout_blocker.md`. That document establishes the shape of the defect being
looked for; this one is the audit that closes the four known gaps in it, and what it found.

## The arithmetic everything below rests on

Measured from the two images, not assumed.

| | retail | built (unshifted) |
|---|---|---|
| `.text` | `0x401000`, vsz `0x1AE1BD`, rsz `0x1AF000` | `0x401000`, rsz `0x1B8000` |
| `.rdata` | `0x5B0000`, vsz `0x11FDB` | `0x5B9000` |
| `.data` | `0x5C2000`, vsz `0xF3FB8`, **rsz `0x1D000`** | `0x5D1000` |
| `.rsrc` | `0x6B6000`, vsz `0x7A6` | `0x752000` |

Retail `.data` is only `0x1D000` initialised; **BSS runs `0x5DF000` .. `0x6B5FB8`**, which is 88%
of the section's address range. Our assembler emits that BSS as real zero bytes, so at load the
content is the same either way.

Content displacements, measured by `find_alignment`:

- `.rdata` **+0x9310** (0x9000 section move, plus the 0x310 the linker's own import table takes at
  the front)
- `.data` **+0xF000**

When `.text` crosses `0x1B8000` and the sections move a page, these become **+0xA310** and
**+0x10000**.

**The lemma that makes the hunt tractable.** A stale pointer holds retail address `R` and
dereferences address `R` at runtime. Our image places retail content `X` at `X + delta`. So the
byte a stale pointer reads is retail content `R - delta` — and therefore:

> what a stale pointer sees in the **shifted** build is what lives **0x1000 earlier** than what it
> sees in the unshifted build.

Every stale pointer slides exactly one page across the game's globals when the layout moves. That
is the entire mechanism, and it is why a defect that is constant does damage that is not.

## Gap 1 — `unrelocated.py` bounds the target window with `.data`'s RAW size

Confirmed, and worse than advertised in both directions.

Opening the target range from `.data`'s raw end (`0x5DF000`) to its true virtual end (`0x6B5FB8`)
takes the reported count from **89 to 6218**. 6129 of those — 98.6% — target BSS and were
invisible to the tool. That is precisely the region `lpOutputString` pointed into.

**But 6218 is not 6218 bugs, and this is the more important half of the finding.** A sliding
4-byte window has no idea where an instruction begins. It reads the `E8` opcode byte of a
`call rel32` as the top byte of an address, so `call 0x411670` inside `mmInterface::Update` gets
reported as a stale pointer to `0x005D30E8`. When I filtered the 6218 down to the exact shape
being hunted, the seven best candidates that survived were **all** this artefact — seven different
`call` instructions, every one ending in `E8`.

`tools/unrelocated.py` is a progress counter, not a gate, and its number should not be read as
either coverage or a bug count. The replacement is below.

## Gap 2 — `.rsrc` has never been audited

**Clean. Ruled out.**

Retail `.rsrc` is `0x7A6` bytes. Scanning every offset for dwords landing in `.rdata`/`.data`
yields ~50 hits, and every single one is a pair of UTF-16 code units from the VERSIONINFO block:
`0x00690072` is `"ri"`, `0x0065006C` is `"le"`, `0x00610070` is `"pa"` — the characters of
`OriginalFilename`, `FileDescription`, `ProductName` and friends. There are no pointers in there
at all. Resource RVAs live in the resource *directory* structures, which the linker rebuilds from
scratch, so there is nothing here for the exporter to get wrong.

## Gap 3 — `verify_relocated`'s remaining finding, `cbMultiByte+0x18`

**Real, but cannot matter. Ruled out.**

`0x005DCD74` holds `0x005B6514`, which in retail is the string `".bat"` inside the CRT's
`_spawn`/`_exec` extension table (`".com"`, `".exe"`, `".bat"`, `".cmd"`). It genuinely did not
move while its three neighbours did, so the gate is right to flag it.

It cannot be the layout bug, for two independent reasons:

1. **Nothing reaches it.** That table is consumed only by `_spawnvpe`/`_execvpe`. MM2 never spawns
   a process.
2. **It does not have the hunted shape.** `0x5B6514` is below our `.text` end (`0x5B8EAF`), so in
   *both* builds it lands inside our own code section — read-only bytes, never zeroed BSS turning
   into live data. It is `0x9310` low today and `0xA310` low after the shift, and both land in
   `.text`. Worth fixing for cleanliness; it is not the cause.

## Gap 4 — pointers inside `.text` (the one that paid off)

The previous audit only reported hits landing on a *named* symbol, which is a weak filter when
most of `.data` has no symbol. Replaced with something decidable.

`tools/verify_operands.py` disassembles all **9306** sized functions with capstone and reads
**only** the 4-byte displacement and immediate fields at the offsets capstone reports for them —
the only two places a 32-bit x86 instruction can encode an absolute address. Branch immediates are
excluded (a `rel32` is not an address). Functions are bounded by `data/symbols.json` sizes so the
decoder always starts on a real instruction boundary.

That takes 6218 window hits down to **5 true findings**, with no judgement calls left and no
false-positive class remaining.

All five are in the CRT's multibyte-codepage setup, and **all five are writes**:

```
0x005841D3  __setmbcp     or  byte ptr [eax + 0x6b4e81], 8
0x005843E9  _setSBUpLow   or  byte ptr [eax + 0x6b4e81], 0x20
0x005843F9  _setSBUpLow   and byte ptr [eax + 0x6b4d80], 0
0x0058441A  _setSBUpLow   or  byte ptr [eax + 0x6b4e81], 0x10
0x00584438  _setSBUpLow   or  byte ptr [eax + 0x6b4e81], 0x20
```

`0x6B4D80` is `_mbctype`, `0x6B4E81` is `_mbcasemap`. Both should have moved to `+0xF000`. The
tables themselves *were* relocated everywhere else — of the 6 references to `0x6B4D80` in `.text`,
5 moved; of the 25 references to `0x6B4E81`, 21 moved. Only these 5 write instructions were
missed, which is why nothing noticed: the reads go to the right place, so the tables behave, and
the writes quietly land somewhere else entirely.

## Ranked candidates

### 1. `_setSBUpLow` / `__setmbcp` — 5 stale byte-writes into `Stream::sm_Buffers`

**This is the strongest candidate by a wide margin and the only one with the full hunted shape.**

`eax` in those instructions is a character code, 0..255, so each is the body of a loop that walks
256 bytes. In our image the writes land at `0x6B4D80` and `0x6B4E81`, which correspond to retail
content at:

- unshifted: `0x6A5D80` / `0x6A5E81`
- shifted: `0x6A4D80` / `0x6A4E81`

Both fall inside **`Stream::sm_Buffers`** (`?sm_Buffers@Stream@@0PAY0BAAA@DA`, retail
`0x6A3D7C`, `0xC018` = 49176 bytes — twelve 4 KB file-I/O buffers). So in *every* build, CRT
startup scribbles ~256 bytes into the game's stream buffer pool. What changes with the layout is
**which buffer slot** it lands in: the buffers are `0x1000` apart and the shift is exactly
`0x1000`, so moving the sections moves the corruption **exactly one Stream buffer earlier**.

That is a precise match for the reported failure:

- The damage is byte-granular (`or byte ptr`, `and byte ptr`), matching "a byte-granular stack
  smash, not a push/pop imbalance".
- `or byte ptr [...], 0x20` turns a `0x00` into `0x20` (space). **It destroys NUL terminators.** A
  string read out of that buffer becomes overlong, and copying it into a fixed stack buffer is
  exactly how `PlayerFillStats` smashes its frame.
- The crash is at **profile load** — a file read, through a Stream buffer.
- It explains why it is invisible today: in the unshifted layout the scribble lands in a buffer
  slot whose stale tail is never read back.

Caveat, stated honestly: these run once at CRT init, before the game opens any stream. The
corrupted bytes therefore sit in the buffer pool as pre-existing garbage and only matter where a
short read leaves them exposed past the fill point — plausible for the tail of a profile file, but
not proven. I could not confirm the final link without running the game, which I am not permitted
to do. **Fix it regardless** — it is an unambiguous defect, it is cheap, and it is the only
surviving candidate of the right shape.

Suggested confirmation for whoever owns the runtime: break at `0x5843E9` and `0x5843F9` and dump
`Stream::sm_Buffers` before and after, in both layouts. `tools/faultwatch.exe` takes `+RVA` and
`@RVA` for exactly this.

### 2. `cbMultiByte+0x18` → `".bat"`

Real but unreachable, and lands in `.text` in both layouts. See gap 3. Fix for cleanliness.

### 3. Nothing else — three regions came back clean

These are negative results, and they are worth as much as the positive one because they narrow
where the parent session should look next.

- **Stale pointers stored in `.rdata`/`.data`: 303 exist, none has the hunted shape.** I tested
  all 303 4-aligned stale pointer-valued dwords against the lemma above — does the target read
  `NUL` today and non-`NUL` after a `0x1000` shift — and got **zero** hits. Whatever is left in
  those 302 unjudged dwords, none of them flips from harmless to harmful across the shift in the
  way `lpOutputString` did.
- **`.rsrc`: no pointers at all.**
- **`.text` operands: exactly 5, all accounted for above.**

## What was added

- `tools/verify_operands.py` — the seventh gate. Currently **FAILS with 5 findings**; it will pass
  once the `_setSBUpLow`/`__setmbcp` displacements are relocated. Run it alongside the other
  `verify_*` tools.

## What is still not covered

Stated so the numbers above are not mistaken for total coverage.

- Functions **without** a size in `data/symbols.json` are not disassembled. 9306 of 9307 code
  symbols have one, so this is one function, but it is not zero.
- Bytes of `.text` not inside any known function extent (padding, hand-written stubs, jump-table
  islands) are not decoded at all.
- The 302 stale `.rdata`/`.data` dwords that `verify_relocated` declines to judge remain unjudged.
  They fail the hunted-shape test, but that test only asks about the first byte at the target.
- Unaligned pointer-valued dwords in `.data` are not scanned; `verify_relocated` steps by 4.
