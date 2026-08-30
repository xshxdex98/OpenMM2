# Running the game

## The two things that must be right

**1. `-gl` is required.** dgVoodoo was removed by the OpenGL work: `DDraw.dll`, `D3DImm.dll` and
`D3D8.dll` are renamed to `*.dgvoodoo-off` in the MM2 folder. Without a wrapper the 1999 DirectDraw
path cannot change display mode on a modern driver, so it fails with

    Error: lpDD->SetDisplayMode(m_iWidth, m_iHeight, m_ColorDepth, 0, 0) failed, code=80004001

`80004001` is `E_NOTIMPL`. The game then re-runs autodetect and reports that **the display does not
meet requirements**, which is misleading: nothing is wrong with the binary or the config, it is the
wrong renderer. The same executable launched with `-gl` reports

    GL: -gl given, the OpenGL device layer is active
    GL: desktop is 2560x1440
    GL: synthesised 17 resolutions, default 2560x1440
    OpenGL Renderer: AMD Radeon RX 9060 XT   (4.6 Compatibility, 298 extensions)

**2. The working directory must be the MM2 install.** The game loads `EBUeula.dll` and opens its
`.ar` archives by relative path. Started anywhere else it quits with "Failed to load EBUeula.dll or
user declined." The executable itself is fine and runs from `build/` provided the working directory
is correct.

## Launchers

Two are generated into `build/`, both surviving a rebuild (`build.py` only deletes `.obj` files):

| | |
|---|---|
| `build\Play OpenMM2.lnk` | double-click, no console; passes `-gl` |
| `build\Play OpenMM2.cmd` | defaults to `-gl`, passes through any arguments, clears a stale lock first |

## Two things that bite

**A stale `MM2.lock`.** Killing the game with `taskkill` leaves a zero-byte `MM2.lock` in the game
folder. The `.cmd` deletes it on the way in; otherwise remove it by hand.

**`gfxconf.dat` rewritten by a killed run.** The game rewrites its enumerated mode table on every
launch. A run killed *during startup* can leave that table inconsistent - 85 changed bytes inside
the `+0x7C` mode table that `tools/setres.py` documents - and the next launch then fails autodetect.
Two GPUs are enumerated on a typical machine (discrete and integrated), so a shuffled table can also
select the wrong adapter.

**Copy `gfxconf.dat` aside before any test run you intend to kill.** `tools/setres.py` selects a
resolution in it; its docstring carries the file format.

## Deployment

`tools/build.py` does **not** copy the executable into the game folder. The two therefore drift
apart silently, and the game folder has been found a full round of ports behind. Either copy it
after each build, or launch the one in `build/` via the shortcut above, which always runs the fresh
binary.
