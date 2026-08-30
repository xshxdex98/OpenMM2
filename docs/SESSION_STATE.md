# Stopping point

Paused deliberately here. Everything below is committed to disk and the build is green.

## Where the build is

`BUILD OK` — **272 / 9307 functions ported (2.01%)**, all 16 gates passing.
Up from 259 at the start of the session. Nothing is half-applied.

## What went in

**Round 5** (`data/ported_round5.json`) — four math methods, smoke-tested for 75 s with zero
exceptions:

| symbol | note |
|---|---|
| `?Scale@Vector3@@QAEXABV1@M@Z` | writes *through* its `const Vector3&`; that is the original |
| `?SubtractScaled@Vector3@@QAEXABV1@M@Z` | the name says subtract, every instruction is `fadd` |
| `?Transpose@Matrix34@@QAEXXZ` | three swaps, bit-exact |
| `?Transpose3x4@Matrix34@@QAEXXZ` | calls Transpose then rewrites the translation as -(R^T·t) |

**Round 6** (`data/ported_round6.json`) — eight `gfxTexture` entry points that are **empty in
retail** (the mipmap-environment feature was compiled out; porting them as no-ops is exact, not
lazy), plus `?Zero@crAnimFrame@@QAEXXZ`. New TU: `code/midtown2/cr/cranimframe.cpp`.

## The screen that picks candidates

Rounds 5's rejects each cost a build to discover, so candidates are now filtered up-front by three
gates, all applied **before** any C++ is written:

1. `tools/verify_arity.py`'s test — the mangled name's stack bytes must equal the code's `ret imm16`.
2. Build gate 6's test — no dword in `.data`/`.rdata` may point strictly inside the extent.
3. A purity test — no `call`, no absolute address, so nothing is needed from `LINKABLE_GLOBALS`
   and no unrecovered layout is involved.

**544** unported functions pass the first two. **146** pass all three. That 146 is the honest
ready-to-port list and is where the next round should start.

## Rejected, with the reason (do not re-derive these)

- `Vector3::Add`, `Vector3::Subtract`, `Vector3::operator*=`, `Vector3::AddScaled` — the name and
  the code disagree on stack size. Four of the twelve such symbols in the image.
- `Vector3::InvScale` — passes the arity check and still cannot be ported: takes a pointer where
  the name says `float`, returns a dot product in st0 where the name says `void`.
- `Matrix34::Transform` — **already tried and rejected in an earlier session**; the note is in
  `matrix34.cpp` itself. Four dwords in `.data` are a *switch jump table* pointing inside it, and
  `game.asm:797348` spells one as `?Transform@... + 96`. Porting re-points that at the C++ body.
  I re-derived this from scratch before noticing the existing note — read the target file first.
- `aiPath` — supplies 35 of the 146 pure candidates and none are takeable yet. The `SharpTurn*`
  accessors read `[this+0x24]` as a dword and index `[this+0x28]`, but `data/layouts.json` types
  0x24 as `u8` and has **no member at 0x28**. Correcting aiPath's layout unlocks the richest
  remaining vein.

## Next step

Take another batch from the 146. Regenerate it with the screen described above; `aiPath` becomes
available once its layout is fixed.

## Scar tissue from this session

Backing a port out by anchoring a string search on its comment truncated `matrix34.cpp` to EOF,
because an identical comment already existed earlier in the file. Recovered from
`checkpoints/GOLDGATE`. **There is no git here** — the checkpoints are the only safety net, so
prefer line-ranged edits over `index()`-and-truncate.

---

# How to launch OpenMM2 (read this before debugging a "broken build")

**`-gl` is mandatory.** dgVoodoo was deliberately removed by the OpenGL work: `DDraw.dll`,
`D3DImm.dll` and `D3D8.dll` are renamed to `*.dgvoodoo-off` in the MM2 folder. Without a wrapper
the 1999 DirectDraw path cannot change display mode on a modern driver, so

```
Error: lpDD->SetDisplayMode(m_iWidth, m_iHeight, m_ColorDepth, 0, 0) failed, code=80004001
```

`80004001` is `E_NOTIMPL`, and the game then re-runs autodetect and reports that **the display does
not meet requirements**. That presentation is misleading: nothing is wrong with the binary or the
config, it is the wrong renderer. Launched with `-gl` the same exe reports:

```
GL: -gl given, the OpenGL device layer is active
GL: desktop is 2560x1440
GL: synthesised 17 resolutions, default 2560x1440
OpenGL Renderer: AMD Radeon RX 9060 XT   (4.6 Compatibility, 298 extensions)
```

323 lines of healthy output against 44 for the failing run.

**The working directory must be the MM2 install.** MM2 loads `EBUeula.dll` and opens the `.ar`
archives by relative path. Started anywhere else it quits with "Failed to load EBUeula.dll or user
declined." The exe itself is fine - it runs from `build/` provided the working directory is right.

**Two launchers exist, both in `build/` and both surviving a rebuild** (`build.py` only deletes
`.obj` files):

- `build\Play OpenMM2.lnk` - double-click, no console. Targets the deployed exe, passes `-gl`.
- `build\Play OpenMM2.cmd` - defaults to `-gl`, passes through any arguments you give it, and
  clears a stale `MM2.lock` first.

## Two ways this bites

**A stale `MM2.lock`.** Killing the game with `taskkill` leaves a zero-byte `MM2.lock` in the game
folder. The `.cmd` deletes it on the way in; delete it by hand otherwise.

**`gfxconf.dat` rewritten by a killed run.** The game rewrites its mode table on every launch. A run
killed during startup left 85 changed bytes inside the `+0x7C` mode table that `tools/setres.py`
documents. Copy `gfxconf.dat` aside before any test run you intend to kill. `gfxconf.dat.backup` is
the pre-session copy; `gfxconf.dat.broken-2026-08-29` is the damaged one, kept as evidence.

**Deployment is manual.** `tools/build.py` does NOT copy the exe into the game folder, so the two
drift apart silently - the game folder was a full round behind when this came up. Either copy it
after each build or add a deploy step.

---

# Today in order (2026-08-29)

1. **Logo work — reverted entirely at the user's request.** Built an "OpenMM2" wordmark from MM2's
   own glyphs lifted out of `jpg/splash.jpg`. Deleted on request: both PNGs, `tools/mklogo.py`,
   the temp extraction dir. No source was touched. Nothing references it.
2. **Round 5** — `Vector3::Scale`, `Vector3::SubtractScaled`, `Matrix34::Transpose`,
   `Matrix34::Transpose3x4`. Smoke-tested 75 s, zero exceptions.
3. **`Matrix34::Transform` attempted and backed out** — gate 6 refused it. It had already been
   tried and rejected in an earlier session; the note was sitting in `matrix34.cpp` the whole time.
4. **The backout truncated `matrix34.cpp` to EOF** — anchored a string search on a comment that
   already existed earlier in the file. Restored from `checkpoints/GOLDGATE`. There is no git here.
5. **Candidate pre-screen built** — 544 unported functions pass arity + gate 6; **146** also pass
   the purity test. This is the reusable outcome of the day.
6. **Round 6** — eight `gfxTexture` entry points that are empty in retail, plus
   `crAnimFrame::Zero`. New TU `code/midtown2/cr/cranimframe.cpp`. **272/9307, BUILD OK.**
7. **Launcher / "display doesn't meet requirements"** — diagnosed to a missing `-gl`, not a bad
   build. See the launch section above. Both launchers fixed and verified; game folder and
   `build/` re-synced (identical md5).

**Open question left with the user:** whether `tools/build.py` should deploy the exe to the game
folder automatically. It currently does not, which is why the two drifted apart.
