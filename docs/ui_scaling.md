# MM2's UI and HUD coordinates, and what 1440p actually needs

Investigated from the decompiled UI/HUD classes. This is the definitive picture of why MM2 does not
scale, and the shortest path to making it.

## Read this first: a decompiler mislabel that will mislead you

Throughout `MM2_PSEUDOCODE`, this appears constantly in UI code:

```c
std::function<float __cdecl (void)>::function<float __cdecl (void)>
```

**That is `gfxPipeline::m_iHeight`.** Every occurrence is a screen-height read. Miss this and half
the UI code reads as nonsense.

It lives at **`0x00683100`**, and the kit resolved that `.data` slot to a `std::function` copy
constructor — a *function* mangling on a data address, which is the tell. The linker map has the
real name `?m_iHeight@gfxPipeline@@0HA` there (`data/symbols.json`, which already has this right).
It is **not** adjacent to `m_iWidth`, which sits 0x28 bytes later at `0x00683128` with `gfxDebug`,
`gfxIcon`, `OrthoVP`, `m_Y`, `m_EvtFlags`, `lpWindowTitle`, `m_fHeight` and `m_Viewport` in between;
`gfxPipeline`'s statics are in link order, not declaration order. See `docs/gfx_layouts.md` for the
full static table and for seven more mislabels of the same kind in the `gfx` address range.

## The verdict

MM2 stores UI positions as **normalised 0..1 floats** and multiplies by `m_iWidth`/`m_iHeight` at
draw time — which looks resolution-independent and mostly is, for *position*. But:

- **There is no global UI scale factor anywhere.** Not `UIScale`, not `HudScale`, nothing. Every
  site reads the pipeline dimensions directly.
- **Text is fixed pixel size.** Fonts come from GDI `CreateFontA` at an absolute pixel height
  chosen from a **two-entry table** (320-wide vs 640-wide) in the localisation resource.
- **Every UI bitmap blits 1:1.** `gfxPipeline::CopyBitmap` is a `BltFast`, and its last two
  arguments are a *source* rect, not a destination size. Nothing stretches.

So at 2560x1440 the widget *frame* scales and its *contents* do not.

## The worst offender: menu.csv and widget.csv store raw pixels

`WArray::Read` and `MArray::Read` parse integer pixel coordinates with `atoi`, then divide by the
**live** screen size to normalise. The widget's `Init` later multiplies by the live screen size
again. The two cancel exactly:

```
authored_px / m_iWidth  * m_iWidth  ==  authored_px
```

Every CSV-positioned widget therefore lands at its authored 640x480 pixel offset **regardless of
resolution** — so at 1440p the menus collapse into the top-left 640x480 corner of the screen.

This is the single biggest visual break, and it is a two-character fix (see below).

## What already scales correctly

Worth knowing, because it should not be touched:

- **`mmHudMap`** — genuinely fractional (size 0.21/0.25, position 0.78/0.75) rendered through a
  real viewport. It is 3D geometry, so it is resolution-independent by construction.
- **`mmDashView` / `RadialGauge`** — the speedometer and tach are 3D models from `%s_dash.pkg`
  rendered through the perspective viewport. No `m_iWidth` anywhere in the gauge path.
- The in-game `ProgressCB` bar, `Card2D`, and the hand-coded fractional literals in `MainMenu`,
  `uiNavBar` and `PUResults`.
- **`mmCRHUD` number fonts** already scale linearly: `20 * m_iHeight / 480`, floored at 12px. The
  pattern to copy is right there in the game's own code.

## What breaks, precisely

| | |
|---|---|
| Menu and HUD text | fixed pixel height from a 2-entry table — unreadably small at 1440p |
| Line spacing | `MenuManager::ScaleX = glyph_h / 480` multiplied back by `m_iHeight` gives ~3x spacing around unchanged glyphs |
| All UI bitmaps | 640-set blitted 1:1 (`BltFast`) |
| Menu backgrounds | `asUnderlay::Cull` passes screen size as a *source* rect on a 640x480 surface |
| CSV widgets | pinned to the top-left 640x480 corner |
| `UIMenu` inset | a fixed 120px/80px margin, so the relative margin shrinks as resolution grows |
| `ProgressCB` loading bar | hard-coded `ProgressRect(349, 448, 640 * progress / 284, 10, ...)` |

## The fix, in four parts

1. **A virtual 640x480 design space with one scale factor**, applied in `mmTextNode::Init`,
   `Card2D::Cull` and `CopyBitmap`.
2. **`mmText::CreateLocFont` takes `round(baseSize * m_iHeight / 480.0)`** instead of the two-entry
   table. The game already does exactly this in `mmCRHUD` — copy that, including the 12px floor.
3. **A scaling blit to replace `BltFast`**, so bitmaps resample rather than land 1:1.
4. **`WArray::Read` / `MArray::Read` divide by literal `640.0`/`480.0`** instead of the live screen
   size. That alone un-pins every CSV widget.

## How this differs from MM1

Open1560 solved the menu side by pillarboxing the UI to 4:3 and resampling menu bitmaps at the
target size — see `docs/resolution_blueprint.md`. It **did not** solve the in-game HUD, which
remains native-pixel with a single 640 breakpoint.

MM2 is in a better starting position than MM1 was for the HUD specifically: the map and the dash
gauges are already 3D and already scale. The work is concentrated in text, 2D bitmaps, and the CSV
loader.
