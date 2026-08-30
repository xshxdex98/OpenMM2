# The section-layout blocker

**This is the top open issue.** It caps how much can be ported, and it will stop the next attempt
immediately, so read this before adding code.

## What happens

When `.text` grows past `0x1B8000`, the linker pushes `.rdata` from `0x1B9000` to `0x1BA000` and
`.data` with it. The game then dies at profile load, in
`?PlayerFillStats@mmInterface@@AAEXXZ +0x199`, writing to a misaligned address with an odd `ebp` -
a byte-granular stack smash, not a push/pop imbalance.

At the time of writing there are **187 ported functions and about 400 bytes of headroom.**

## What it is not

It is **not** the ports. That was measured, and the measurement is the useful part:

- Registering 28 `Matrix33`/`Matrix66` functions reproduced the crash. Those functions are
  **unreachable** - three independent scans over the retail image (every `rel32`, every absolute
  dword in every section, and every symbol reference in `game.asm.pristine`) found that nothing
  outside those two classes calls or references them. A reimplementation of dead code cannot crash
  anything by running.
- `code/midtown2/core/layoutpad.cpp` reproduces the crash with **no ports at all** - just enough
  dead, uncalled code to move the section boundary. Build with `-DARTS_LAYOUT_PAD` to see it.

So the trigger is the layout, and any few hundred bytes of anything will pull it.

## Reproducing it in one build

Add `"-DARTS_LAYOUT_PAD"` to the compile flags in `tools/build.py`, build, and check the section
table before running:

```
.rdata at 0x1B9000  -> unshifted, game reaches GameLoop, 0 access violations, ~66,700 log lines
.rdata at 0x1BA000  -> shifted,   game dies at profile load, ~61,200 lines, 1 access violation
```

## What has been ruled out

- **`PlayerFillStats` itself** has zero unrelocated references to `.rdata`/`.data`.
- **`lpOutputString`**, the message-prefix array behind every `Displayf`/`Warningf`/`Errorf`, *was*
  holding two stale pointers and is now fixed. It was a good suspect - the crash sits exactly
  between an `Error:` line and the next `Warning:` line - but fixing it did not stop the crash.
- `tools/verify_relocated.py` is down from 21 findings to 1 (`cbMultiByte+0x18`, the CRT's `.bat`
  extension entry). `verify_strings` and `verify_dataformats` pass.

## Where to look next

The mechanism is almost certainly a pointer that is wrong in *every* build and only becomes harmful
when the layout moves - harmless while it lands in zeroed BSS, fatal when it lands on live data.
That is exactly the shape of the `lpOutputString` defect, so there is likely another one.

Two concrete approaches:

1. **Diff the two builds' behaviour under the debugger.** `tools/faultwatch.exe` takes `+RVA` to
   plant a breakpoint and `@RVA` to dump memory. Break at `PlayerFillStats` entry in both a shifted
   and an unshifted build and compare the pointers it is handed. The string it overruns comes from
   somewhere, and that somewhere is the bug.
2. **Widen the relocation audit to `.text`.** `tools/unrelocated.py` bounds its search using
   `.data`'s *raw* size, which covers 11.9% of the address range and excludes all of BSS - where
   `lpOutputString` pointed. Its "89" is not the coverage it looks like.

## The alternative, if the cause stays hidden

Make the layout stop moving: pad `.text` to a fixed size so `.rdata` and `.data` sit at constant
RVAs no matter how much C++ is added. That does not fix the underlying defect - it makes the
displacement constant, so whatever is wrong stays wrong in the same way it is wrong today, which is
survivable. It is a worse answer than finding the bug, and a much better answer than a hard ceiling
on porting.
