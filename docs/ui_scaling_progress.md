# UI scaling: what has landed, and what is left

Implements `docs/ui_scaling.md`. That document is the analysis; this one is the ledger. Read it
first — nothing here re-derives it.

Nothing in this work has been compiled or run by its author. Every function was read off the
disassembly of `midtown2.exe` with helpers resolved by address against `data/symbols.json`; the
Hex-Rays output in `MM2_RE_KIT` was used to confirm the reading, never as the source. Where the
kit and the disassembly disagree the disassembly won, and each of those is called out below.

## Status of the four fixes

| | Fix | State |
|---|---|---|
| 1 | Virtual 640x480 design space with one scale factor | **Core landed.** Applied at `mmTextNode::Init` and at bitmap load; deliberately NOT applied at `Card2D::Cull`. |
| 2 | `mmText::CreateLocFont` scales off the vertical resolution | **Landed.** |
| 3 | A scaling blit to replace `BltFast` | **Landed, in the other place.** Bitmaps resample at LOAD, in `gfxBitmap::Create(gfxImage*, bool)`. `CopyBitmap` still blits 1:1 for every caller whose rectangle fits, and only stretches for the one that today draws nothing. See "Round two" below. |
| 4 | `WArray::Read` / `MArray::Read` divide by 640/480 | **Landed.** Both. |

## What was written

| File | |
|---|---|
| `code/midtown2/ui/uiscale.h` | New. The design space. Header-only, all `static` inline. |
| `code/midtown2/misc/warray.cpp` | `WArray::Read` — fix 4 |
| `code/midtown2/misc/marray.cpp` | `MArray::Read` — fix 4 |
| `code/midtown2/mmgame/mmtext.cpp` | `mmText::CreateLocFont` — fix 2 |
| `code/midtown2/mmgame/mmtextnode.cpp` | `mmTextNode::Init(int x6)` — fix 1 |
| `data/ported_ui_scaling.json` | The four mangled names |
| `tools/genheaders.py` | Two generator fixes; see "Generated headers", below |

Round two added:

| File | |
|---|---|
| `code/midtown2/gfx/gfxbitmap.cpp` | New. `gfxBitmap::Create(gfxImage*, bool)` - fix 3 |
| `code/midtown2/gfx/gfxpipeline.cpp` | New. `gfxPipeline::CopyBitmap` - the oversize source rect |
| `code/midtown2/gfx/gfxbitmap.h` | Member access, and the ctor/dtor moved to `private` |
| `code/midtown2/ui/uiscale.h` | `UI_SCALE_BITMAPS`, and what `PixelsW`/`PixelsH` mean for art |
| `data/ported_ui_round2.json` | The two new mangled names |
| `tools/genheaders.py` | Two more generator fixes; see "Round two", below |

## The one design decision worth arguing with

**The design-space mapping is applied at the PRODUCER, never at the consumer.**

MM2 stores UI positions as 0..1 fractions and multiplies by the live screen size at draw time.
Those multiplications live in roughly forty functions that are all still assembly. Mapping at the
consumer would mean porting every one of them; mapping at the producer — the handful of places
where authored pixels first become a fraction — means a fraction that leaves `uiScale` is a
fraction of the real screen, and all forty keep working untouched.

The direct consequence is that **`Card2D::Cull` must not be touched**, even though
`docs/ui_scaling.md` names it. Its `Pos`/`Size` arrive from `UIMenu::GetDimensions` (so from
`menu.csv`, so through `MArray::Read`) and from hand-written fractional literals in `MainMenu`,
`uiNavBar` and `PUResults`. The first population is already mapped by the time it gets there, and
the second was never in design space at all. Applying the scale in `Cull` would scale the first
twice and the second once when it should be zero times. `Card2D::Cull` is correct as it stands and
porting it verbatim would achieve nothing.

### STRETCH, not pillarbox, by default

`uiScale` implements both and defaults to `UI_SCALE_STRETCH`, which is *not* what Open1560 does
for MM1 (`docs/resolution_blueprint.md`: pillarboxed to 4:3, centred).

The reason is the second population above. MM2's UI is authored two ways at once — `.csv` widgets
in pixels, and a significant amount of menu furniture as hand-written fractions of the whole
screen. Under a pillarbox the two land in different coordinate systems and the menus come apart.
Under STRETCH a `.csv` widget lands exactly where it lands at 640x480 *relative to the screen*,
which is the rule the hand-written fractions already follow. Glyphs do not stretch either way:
`uiScale::Scale()` follows the vertical axis alone, so text stays square.

`-DUI_SCALE_MODE=UI_SCALE_UNIFORM` switches to the MM1 behaviour. It is worth trying once the
menus are visible at 1440p, but it should not be the default until the hand-written fractional
sites have been converted too.

## Findings that changed the plan

**`mmText::CreateLocFont` is missing from `MM2_PSEUDOCODE`.** The kit filed 0x005321C0 under
`ImPlot::LineStripRenderer<...>` in `_MISLABELED_FUNCTIONS.c`, and that transcription **lost the
branch the function exists for** — it shows both authored heights assigned to the same variable in
sequence, with the second overwriting the first, and the `int` parameter unused. The disassembly
has `cmp ecx, 0x280 / jl` selecting between them on the screen width. Anyone working from the kit
alone would have concluded there was no two-entry table.

The `LocString` row is `<face>,<height at 320>,<height at 640>,<charset>,<weight>`, and every
caller passes `gfxPipeline::m_iWidth` as the second argument (checked at the eight consecutive
`MenuManager` sites that build the menu fonts).

**The four CSV floats are stored rotated.** `WArray::Read` and `MArray::Read` write the X column
into the Vector4's `w` and shift the other three down one slot. This is not a decompiler artifact:
the stores are to `[ebp-0Ch], [ebp-18h], [ebp-14h], [ebp-10h]` against a Vector4 based at
`[ebp-18h]`, and `RetrieveWidgetData` / `RetrieveMenuData` read them back in the same rotated
order. Both halves agree, so neither may be "corrected".

**`mmTextNode::Init` has two overloads and only one of them is broken.** The float overload at
0x00532840 takes 0..1 fractions and sizes its bitmap off the live screen, so it is already
proportional — that is the one `mmCRHUD` uses, and it is left alone. The integer overload at
0x00532750 is the one that takes authored pixels, and it has exactly **one** caller in the whole
binary: `mmDropDown::InitString` at 0x004F2358 (found by scanning `.text` for `call rel32`
targeting it, not by grepping the kit). Fix 1's payoff at this site is therefore small; what
matters about it is that it is the site that pairs with fix 2. Once fonts scale, a design-sized
text bitmap clips the glyphs it is asked to hold, so `Init` now creates the bitmap at the scaled
size and `mmTextNode::Cull`'s 1:1 blit stays correct.

**`_memset` does not exist as a linkable symbol.** `mmTextNode::Init` zero-fills its entry array,
and the 1999 compiler inlined that as `rep stosd`. A plain zeroing loop in C++ is recognised by
MSVC and lowered to a call to `_memset`, which is nowhere in `midtown2.exe`. The fill is written
through a `volatile u32*` to defeat the idiom. This will bite every future port that touches a
buffer; see `docs/ported_freefuncs.md`.

**`gfxPipeline::m_iWidth` and `m_iHeight` are not adjacent.** `docs/ui_scaling.md` explains the
`std::function<float __cdecl (void)>` mislabel as the two sitting next to each other in `.data`.
They do not — `m_iHeight` is at 0x683100 and `m_iWidth` at 0x683128. The mislabel is real and the
document's substantive claim is right; only that detail of the explanation is wrong.

## Generated headers: a class-key bug that silently unports a function

`tools/build.py` quarantined `mmtext.obj` on the first pass through this work, and the reason
generalises to every future port.

`mmtext.h` forward declared `class LocString;`. The retail mangling spells that type `U`
(struct) — `?CreateLocFont@mmText@@SAPAXPBULocString@@H@Z` — so the C++ compiled cleanly and
emitted `?CreateLocFont@mmText@@SAPAXPB**V**LocString@@H@Z`. Different symbol. The linker would
have kept the assembly and the reimplementation would never have run; the quarantine caught it
only because the emitted name was not in `data/ported.json`.

`tools/genheaders.py` emitted `class %s;` for every forward declaration. It now derives the
class-key **from the mangled names themselves**: a `U<Name>@@` occurrence anywhere in
`data/symbols.json` means the 1999 compiler saw a struct. That is 281 names, and it picks up
`LocString`, `IDirectDrawSurface7`, `mmTextData`, `coreFileMethods`, `gfxStridedData`,
`gfxVertexBuffer` and `HWND__` without anyone having to notice them one at a time. Deriving it
beats a hand-written list, which would rot.

The same run also fixes pointer-to-array statics. `?sm_Buffers@Stream@@0PAY0BAAA@DA` has a
`type` field that truncates at the open paren (`"char (*"`), and the generator emitted
`static char (* sm_Buffers;`, which does not parse — so `stream/stream.h` could not be included
at all. The extent now comes from the demangled name, which spells it in full (`[4096]`). Three
symbols in the binary have that shape.

**Still unfixed in the generator:** the class-key of a class DEFINITION (`class %s : public %s`)
is not derived the same way, so a type that is a struct in the binary is still defined as a
`class`. That is the same bug one level up and will bite whoever first ports a member of
`gfxStridedData` or `gfxVertexBuffer`. It was left alone here because it changes the key on
several hundred headers at once and could not be compiled to check.

## Round two: how fix 3 was actually done

### The objection that blocked it, restated - because it still holds

`gfxPipeline::CopyBitmap` (0x004AB4C0) is nine lines: build a source RECT and call
`IDirectDrawSurface7::BltFast`. Replacing it with a stretching `Blt` is easy to write, and the
first pass through this work refused to write it for a reason that has not gone away.

**It would make the HUD worse, not better.** The callers compute their layout in assembly from
*unscaled* `gfxBitmap::Width`/`Height`. `mmHUD` advances across a row of digits with
`v4 += bmp->Width`; `mmSlider`, `mmNumber`, `mmSpeedIndicator` and `mmIcons` do the same. Scaling
at blit time draws each element 3x larger and still advances one width, so every multi-part HUD
element overlaps itself. Of the ~40 call sites, exactly one is improved by a scaling blit -
`asUnderlay::Cull`, and that one is improved because it is broken rather than because it scales.

**The correct mechanism is to resample at bitmap LOAD**, so `gfxBitmap::Width`/`Height` report the
scaled size and the assembly's arithmetic follows automatically. That is also what Open1560 does
for MM1's menu backdrops (`docs/resolution_blueprint.md`: "decoded at the target size").

The other two blockers the first pass listed - `gfx/gfxpipeline.h` not compiling, and
`?lpdsRend@@3PAUIDirectDrawSurface7@@A` needing `struct` where the tree said `class` - were both
closed by the generator fixes made while that work was in flight.

**So `gfxPipeline::CopyBitmap` still does not scale.** What follows changed where the scale is
applied, not what the blit does.

### Resampling at load, in `gfxBitmap::Create(gfxImage*, bool)`

`code/midtown2/gfx/gfxbitmap.cpp`, symbol `?Create@gfxBitmap@@SAPAV1@PAVgfxImage@@_N@Z`
(0x004AE480). A decoded UI image is resampled to the screen scale before its surface is built, so
`gfxBitmap::Width` and `Height` report the scaled size and all forty assembly callers follow along
untouched.

**MM2 already does this to itself, which is the strongest argument that it is safe.**
`PUMenuBase::PUMenuBase` at 0x0050CC90 is literally `gfxLoadImage` ->
`gfxImage::Scale(image, m_iWidth, m_iHeight)` -> `gfxBitmap::Create(image, 0)`, and then reads the
resulting bitmap's `Width` and `Height` back out to derive its own fractional size. This change is
that sequence applied to every named UI bitmap instead of to one popup background. `gfxImage::Scale`
is the game's own bilinear resampler, and its domain - `Type` 1 (`ARGB_8888`) and 2 (`RGB_0888`)
only - is *exactly* the set of formats `gfxBitmap::Load` accepts. That coincidence is what makes it
the right resampler here rather than a convenient one.

**The overload matters.** `Create(i32, i32, bool)` takes an explicit size from a caller that has
already decided - `mmTextNode::Init` (which already sizes through `uiScale`, so scaling again would
square the factor), `mmText::CreateFitBitmap`, `PUMenuBase::CreateDummyBitmap`, render scratch - and
is untouched. `Create(gfxImage*, bool)` means "a decoded image file", and every path into it is UI
art: `gfxGetBitmap`, hence every `nodeGetBitmap` caller, plus the two loading screens. That is the
"way to tell a UI bitmap from a render target" the blocker asked for, and it needed no heuristic.

**Why the caller list confirms this is the right mechanism.** Every source rectangle in the binary
is derived from the bitmap, never from a literal: `mmHUD` reads `bmp->Width`/`bmp->Height` at
`+4`/`+6` and advances `v4 += bmp->Width`; `mmSlider::LoadBitmap` sets `ActiveHeight` to
`Bitmap->Height / 3` for its three-frame strip; `ccStatus::LoadBitmap` sets its whole rect from
`Bitmap->Width` and `Bitmap->Height / 3`. A bigger bitmap slices and advances correctly on its own.

**`gfxImage::Scale` reads one sample past the image, and that had to be fixed first.** Per output
row it takes `v8 = pImageData + BytesPerRow * (acc >> 15)` and `v11 = v8 + BytesPerRow`; on the last
output row `acc >> 15` is `Height - 1`, so `v11` is row `Height`. Within a row it reads pixel
`(x >> 15) + 1`, which on the last column is pixel `Width`. `PUMenuBase` has lived with this since
1999 because it does it once for one image; doing it for every UI bitmap turns a one-row cosmetic
artifact into a real chance of touching an unmapped page, since a 640x480x32 image is 1.2MB and an
allocation that size comes straight from the heap manager. `PadImageForScale` reallocates the buffer
with one extra row plus one extra pixel and duplicates the last row into it, so the edge sample
lands on a clamped copy of the edge. `BytesPerRow` is deliberately not changed - the column
overread of row N lands inside row N+1, and only the pad row needs the trailing slack. `Scale` ends
by calling `operator delete` on `pImageData`, so it frees the padded buffer and not the original.
The image buffer is exactly `BytesPerRow * Height` bytes, which `gfxImage::Halve` and `Scale`'s own
allocation both confirm, so the copy that builds the padded buffer reads nothing it should not.

**The order of operations is load-bearing.** The scaled surface is created FIRST and the image is
resampled only once that has succeeded, because `Scale` frees the source pixels as it goes and there
is no way back from it. A driver that refuses the larger surface costs nothing: the image is
untouched and the original path runs.

Guards, and each exists for a reason: palettised images and any `Type` other than 1 or 2 are skipped
(`Load` rejects them and `Scale` would `Errorf`); images narrower or shorter than 2 pixels are
skipped (`Scale` divides by `width - 1`); the size must grow, so **at 640x480 the function is the
original byte for byte**; and the result may never exceed the screen, which is what lets
`PUMenuBase`'s already-screen-sized image pass straight through.

`-DUI_SCALE_BITMAPS=0` turns the whole thing off and leaves the rest of the scaling in place. It is
the first switch to try if the UI misbehaves.

### The oversize source rect, in `gfxPipeline::CopyBitmap`

`code/midtown2/gfx/gfxpipeline.cpp`, symbol
`?CopyBitmap@gfxPipeline@@SAXHHPAVgfxBitmap@@HHHH_N@Z` (0x004AB4C0).

For every caller whose source rectangle fits inside its bitmap - which is all of them but one -
this is a literal transcription: the same RECT, the same `BltFast` at vtable slot 0x1C, the same
`DDBLTFAST_WAIT | (key ? DDBLTFAST_SRCCOLORKEY : 0)`. An empty or inverted rectangle counts as
fitting and behaves exactly as before.

`asUnderlay::Cull` at 0x004A22B0 is the exception. It passes `m_iWidth`/`m_iHeight` as the source
extent, which `BltFast` rejects outright on a smaller surface, so **at 1440p there is currently no
menu background at all**. That case is not one that works today - it is one that draws nothing - so
reading the extent as what `asUnderlay` plainly meant by it, the size to cover, and stretching the
whole bitmap into it with `Blt` cannot regress anything. `asUnderlay::Cull` itself is untouched, and
`CopyClippedBitmap` inherits the fix because it tail-calls `CopyBitmap`.

Once bitmaps resample at load these two fixes compose rather than overlap: a 640x480 backdrop
becomes exactly 2560x1440, the rectangle fits, and the fast path draws it 1:1. The stretch is what
catches a backdrop that is any other size.

The stretch is point sampled - no `DDBLTFX`, no `DDBLTFX_ARITHSTRETCHY`. Every bitmap surface is
created with a source colour key of zero, and a filtered stretch would blend key pixels into their
neighbours and leave a fringe the key no longer catches.

All four DirectDraw constants are confirmed twice over. `DDBLTFAST_WAIT` (0x10) and
`DDBLTFAST_SRCCOLORKEY` (0x01) are readable straight off the original's
`xor edx, edx / test bl, bl / setnz dl / add edx, 10h`; `DDBLT_WAIT` (0x01000000) is confirmed by
`gfxPipeline::ClearRect` pushing 0x01000400, which is `DDBLT_WAIT | DDBLT_COLORFILL`. All four agree
with `ddraw.h`. The vtable indices are confirmed the same way: `CopyBitmap` calls `[ecx+1Ch]` and
`gfxBitmap::Clear` calls `[ecx+14h]`, which are `BltFast` (7) and `Blt` (5) in the `ddraw.h`
declaration order.

The build has **no Windows headers on its include path** - `tools/build.py` adds only the UCRT
directory - so `RECT`, the surface vtable and the four flags are all spelled out by hand.

### Two more generator fixes, both of which would otherwise have failed the link

**Instance members inherited the last access section, which is decided by the class's statics.**
`gfxBitmap`'s only static is the private `sm_First`, so `Name`, `Width`, `Height` and `Surface` came
out private - while `gfxTexture`, which has no private static, got the identical fields public. An
instance member's name is not part of any mangled symbol, so this is pure C++ bookkeeping with no
effect on what links, but it makes a field the original reads from everywhere (`ProgressCB`,
`mmHUD`, `RestoreFocus`, `CopyBitmap`) impossible to port. `emit_members` now emits an explicit
`public:` before the member block.

**`access_of` could not see a constructor or destructor's access.** It looks for `@<class>@@` in
the mangled name, and a constructor spells the class immediately after `??0`/`??1` with no leading
`@` - `??1gfxBitmap@@AAE@XZ`, not `??1@gfxBitmap@@AAE@XZ` - so the lookup missed and fell through to
its public default. That is not cosmetic. `gfxBitmap`'s constructor and destructor are both private
(`AAE`), and the public declaration made the compiler emit `??1gfxBitmap@@QAE@XZ`, a symbol nothing
in `midtown2.exe` defines. `gfxBitmap::Create(gfxImage*, bool)` destroys a bitmap on a failed load,
so this would have been an unresolved external naming the C++ spelling rather than the mismatch -
the exact failure mode the `?m_iWidth@gfxPipeline@@0HA` comment in that function already warns
about. `gfx/gfxbitmap.h` has been corrected by hand to match.

### What round two deliberately did NOT do

- **`asUnderlay::Cull` was not ported.** It did not need to be, once `CopyBitmap` handles the
  oversize rect, and not porting it avoids defining a virtual member function whose class has a
  retail vftable at 0x005B2970 that our object file must not also emit.
- **`gfxPipeline::CopyClippedBitmap` was not ported.** Both overloads reach `CopyBitmap`, so they
  get the same behaviour for free.
- **No caller of `CopyBitmap` was touched.** That is the whole point of doing this at load.
- **Bitmaps are never shrunk**, only enlarged, so no resolution below the design space changes.

### The cost this accepts

Under `UI_SCALE_STRETCH`, a bitmap picks up the horizontal factor on its width and the vertical
factor on its height, so at 16:9 art is 33% wider than it is tall. That is the same distortion the
widget box it sits in already has, which is the argument for it: the bitmap now matches the rest of
the UI instead of being the one thing that does not. `-DUI_SCALE_MODE=UI_SCALE_UNIFORM` makes both
factors equal and keeps art square, at the cost of the pillarboxing described above.

The other cost is the handful of widgets positioned by hard-coded pixels rather than through the
CSV or a fraction - `ccStatus::LoadBitmap(name, 179, 225, ...)` is the clearest - where the art is
now four times the size but still anchored at pixel 179. That position was already wrong at 1440p;
a bigger bitmap makes it more obvious. Those sites need the same treatment `WArray::Read` got and
are the natural next piece of work.

`gfxGetBitmap` caches by name in `BitmapHash`, and the scale is fixed at creation, so a resolution
change mid-session would leave every already-loaded bitmap at the old scale. `mmHUD::ResChange` and
`sfPointer::ResChange` re-request their bitmaps and would get the cached ones back. Nothing depended
on resolution before, so this is new, and it is a reason to keep treating resolution as a
restart-time setting until the cache is invalidated on `ResChange`.

## Not addressed, and named in `docs/ui_scaling.md` as broken

- `MenuManager::ScaleX` line spacing — `glyph_h / 480` multiplied back by `m_iHeight` gives about
  3x the spacing around unchanged glyphs. Fix 2 changes the glyphs, so this needs rechecking
  before it is touched; it may now be self-consistent.
- `UIMenu`'s fixed 120px/80px inset.
- `ProgressCB`'s hard-coded `ProgressRect(349, 448, 640 * progress / 284, 10, ...)`.
- `gfxPipeline::CopyClippedBitmap` — no longer an open item. Both overloads tail-call
  `CopyBitmap`, so they inherit round two's behaviour without being touched.

## First things to check once this compiles and runs

1. **`mmText::CreateLocFont` links.** The `_imp__CreateFontA` function-pointer declaration in
   `mmgame/mmtext.cpp` is the single least certain line in this change. It exists because
   `tools/implibs.py` names imports undecorated (`build/implibs/GDI32.lib` carries
   `__imp__CreateFontA`, not `__imp__CreateFontA@56`), so neither obvious declaration of a
   `__stdcall` API works: the `__declspec(dllimport)` one asks for a name that is not there, and a
   `__cdecl` one finds the thunk and then unwinds the stack twice. Declaring the IAT slot itself
   is how the original reaches it. If the spelling is wrong it fails loudly at link time; if it
   somehow resolves and the convention is wrong, it corrupts the stack, so check the link map
   rather than assuming.
2. **At 640x480 nothing should look different.** All four functions are written to return exactly
   the original's values at the design resolution. A difference there is a transcription bug, not
   a scaling one, and is the cheapest possible place to catch one.
3. **Then 2560x1440.** Menus should fill the screen instead of the top-left corner, and menu text
   should be roughly three times its current height.

**Both round-two symbols are now in `data/ported.json` as well as in
`data/ported_ui_round2.json`**, because a concurrent build merged the part file while this work was
still in flight and the part file then went missing from `data/`. It has been restored, so
`merge_ported` will report both as duplicates. That message is about a second *registration*, not a
second definition - `merge_ported` starts from `data/ported.json` and skips what it has already
seen - and deleting `data/ported_ui_round2.json` is safe if the noise is unwanted.

Worth knowing while reading any of this: **`tools/asm.py` decides what to strip from the `ARTS_EXPORT`
markers in the generated headers, not from `data/ported.json`**, and it treats an `ARTS_EXPORT` with
no entry in `data/ported.json` as fatal. `tools/genheaders.py` runs the relationship the other way,
deriving its markers *from* `data/ported.json`. So a symbol registered in that file but never
implemented is inert only until the next `genheaders.py` run, which would flip its header and hand
`asm.py` a function whose original is stripped with nothing to replace it.

Round two adds four more, in the order they would fail:

4. **`?lpdsRend@@3PAUIDirectDrawSurface7@@A` resolves.** `gfx/gfxpipeline.cpp` declares that global
   by hand, and the class-key is the thing to get wrong: the retail mangling spells the type `U`,
   so a `class IDirectDrawSurface7` forward declaration anywhere ahead of it would produce
   `?lpdsRend@@3PAVIDirectDrawSurface7@@A` and not link. `gfx/gfxbitmap.h` says `struct`, which is
   what the file relies on. Note that `agigl/glbitmap.cpp` and `agigl/glgfxpipeline.cpp` both still
   forward declare it as a `class` — they are behind `#if defined(ARTS_AGIGL)` and are not
   compiled, but they are wrong and should be corrected before that guard is ever lifted.
5. **`??1gfxBitmap@@AAE@XZ` resolves, not `??1gfxBitmap@@QAE@XZ`.** This is the reason
   `gfx/gfxbitmap.h` now declares the constructor and destructor in a `private:` section. If the
   link asks for the `QAE` spelling, the header edit did not take.
6. **No duplicate `??_7`.** Nothing in round two defines a virtual function, precisely so that no
   object file can emit a vftable that `game.asm` already defines as a plain `PUBLIC`. If a
   duplicate-symbol error names a `??_7...@@6B@`, something reintroduced one.
7. **At 640x480 nothing should look different, again.** `gfxBitmap::Create(gfxImage*, bool)` will
   not resample when both scale factors are one, and `gfxPipeline::CopyBitmap` cannot reach its
   stretching path when the underlay bitmap is the size of the screen. Both are written so that
   the design resolution exercises the original code path exactly.

Then at 1440p, in increasing order of what they prove: the menu background exists at all
(`CopyBitmap`'s stretch, or the resampled backdrop taking the fast path); HUD icons and the mouse
pointer are physically larger; and a multi-part element — the lap/time digits, a slider — is
larger *without overlapping itself*, which is the whole claim of doing this at load rather than at
the blit.

If a build rejects any of this, the two round-two `.cpp` files and `data/ported_ui_round2.json`
belong in `wip/` **together**, and the same is true of the first pass's four `.cpp` files and
`data/ported_ui_scaling.json` — the registration and the sources are one unit, and separating them
leaves `code/` compiling functions nothing has registered. The two rounds are independent: round
two touches no file the first pass wrote except `ui/uiscale.h`, and only adds to it.

Round two also edits `code/midtown2/gfx/gfxbitmap.h`, which is generated output. Both edits are
reproduced by the `tools/genheaders.py` fixes above, so a regeneration keeps them — but if
`gfxbitmap.h` ever comes back with private members or a public destructor, that is the generator
having been rolled back, not a new bug.
