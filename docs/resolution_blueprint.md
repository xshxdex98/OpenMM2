# Arbitrary resolution: what Open1560 did, and what transfers to MM2

Midtown Madness 1 already solved this on the same engine family. This is that solution, distilled,
so the MM2 work does not re-derive it. Source: `Open1560Dev/Open1560/code/midtown/`.

## The structural decision

Open1560 **deleted DirectDraw mode enumeration outright.** `pcwindis/` no longer calls
`IDirectDraw::EnumDisplayModes` or `ChangeDisplaySettings` at all. Instead it enumerates *displays*
via SDL and **synthesizes** a resolution list from the desktop bounds.

The consequence is the whole design:

> Resolution stops being a display mode and becomes a **render target size**, blitted to the window.

Fullscreen is borderless-desktop (`SDL_SetWindowFullscreen`), never a mode change. The comment in
`agisdl/sdlsetup.cpp` states it plainly — the mode list is essentially cosmetic, because the GL
pipe renders at native resolution and framebuffers accept any size.

**This is what removes the need for dgVoodoo.** dgVoodoo exists to translate D3D7 mode-setting and
rasterisation to something modern. If the device layer never sets a mode and renders through GL,
there is nothing left to translate.

## Five coordinate systems, all live at once

Getting these separate is most of the job:

| | What |
|---|---|
| `horz_res_`/`vert_res_` | real window pixels (`SDL_GetWindowSizeInPixels` — HiDPI-correct) |
| `blit_*` | the rect inside the window the game occupies, from `-scaling` |
| `width_`/`height_` | the **logical** resolution; all game and UI coordinates live here |
| `render_*` | where 3D actually rasterises — native by default, decoupled via FBO |
| `UI_*` | the 4:3-locked, centred menu box derived from a 640x480 reference |

## The widescreen fix is one function

`asCamera::SetView`, gated on `-fovfix` (default on). It is **Hor+**: take the authored horizontal
FOV, back-solve the vertical FOV the designers intended on 4:3, then re-derive a wider horizontal
FOV for the real aspect.

```
horz_tan = tan(horz_fov / 2)
vert_tan = horz_tan / (640/480)        // the vertical FOV they meant
aspect   = pipe_width / pipe_height    // what we actually have
horz_fov = 2 * atan(vert_tan * aspect) // widen horizontally, keep vertical
```

Without it the engine holds horizontal FOV fixed and *shrinks* vertical — Vert-minus, which is
exactly why these games feel cropped on 16:9. Preserving vertical FOV also keeps the dashboard art
framed correctly, which is the author's stated reason.

## UI: pillarboxed and resampled, not stretched, not left at native

`agiPipeline::BeginAllGfx` locks the menu UI to 4:3, centres it, and derives `UI_ScaleX/Y` and
`UI_StartX/Y`. Every widget goes through one funnel (`MenuManager::GetScale`). Crucially, menu
bitmaps are **decoded at the target size** rather than point-scaled from 640x480, so backdrops stay
crisp.

**The in-game HUD was NOT solved.** Dash, gauges and lap timer use raw native pixels with a single
`>> half` breakpoint at 640 wide. At 4K the speedo cluster is physically tiny. `changes.md` only
ever claimed menu scaling. If MM2 should do better than MM1, this is the gap to close.

## Two bugs worth not repeating

Both are in Open1560 today and are the kind of thing that bites at unusual resolutions:

1. `add_resolution` writes `info.Resolutions[info.ResCount++]` into a **32-entry array with no
   bound check**. It happens not to overflow because a 2160p display yields ~14 entries.
2. `dxiReadConfigFile` reads `ResCount` straight out of `video.cfg` and writes that many entries,
   **also unclamped** — while the renderer count right above it *is* clamped.

## What did NOT need doing

No static maximum-resolution scratch buffer had to be grown. `agi` never had one: surfaces are
heap-allocated per-surface, and depth/colour buffers are GL renderbuffers sized at `BeginGfx`. The
software rasteriser that *would* have needed a fixed buffer was deleted rather than ported.

That matters for MM2 scoping — `sdlPage16` is the equivalent suspect and should be checked, but
the precedent is that this class of problem disappears with the software path.

## Config

Registry abandoned for graphics; resolution lives in plain-text `.\video.cfg`. Note the format
stores **the whole enumerated list plus an index**, not a width/height pair — so the index only
means anything relative to the cached list, and the loader re-validates against live displays and
re-enumerates on any mismatch.

## Command-line surface worth copying

`-width`/`-height` bypass the list entirely with **zero validation** — the fastest way to test an
arbitrary resolution. Plus `-nativeres` (render 3D at window size, default on), `-msaa`,
`-scaling` (stretch / keep-aspect / centred / integer), `-fovfix`, `-window`, `-config`.
