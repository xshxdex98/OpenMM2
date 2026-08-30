# The graphics classes, laid out

What an OpenGL device layer has to agree with, byte for byte. Method and refusal rules follow
`docs/layout_sources.md`; the machine-readable half of this is `data/layouts_gfx_recovered.json`.

Read the summary table, then the class that concerns you. Some entries below say a currently
recorded number or name is **wrong** — those are the ones to read before writing any code. The two
that can silently corrupt memory are `gfxMaterial`'s size (§7, §12, §20) and `gfxPacket`'s
`0x16` (§16.1).

## Summary

| Class | Size | Status | Where it stands today |
|---|---|---|---|
| `gfxPipeline` | 0x1 | **CONFIRMED — no instance layout exists** | correct as-is; keep the guard commented |
| `gfxTexture` | 0x34 | CONFIRMED | correct in `layouts.json`, live `check_size` |
| `gfxImage` | 0x1C | CONFIRMED | correct in `layouts.json`, live `check_size` |
| `gfxBitmap` | 0x14 | CONFIRMED | correct in `layouts.json`, live `check_size` |
| `gfxViewport` | 0x17C | CONFIRMED | correct in `layouts.json`, live `check_size` |
| `gfxInterface` | 0x270 | CONFIRMED | correct in `layouts.json`, **no header is generated** |
| `gfxResData` | 0x8 | CONFIRMED | **missing** under that spelling; emitted for merge |
| `gfxRenderState` | 0x50 | CONFIRMED | correct in `layouts.json`; **MM2Hook's 0x98 is wrong** |
| `gfxRenderStateData` | 0x50 | CONFIRMED | correct in `layouts.json` |
| `gfxLight` | 0x68 | CONFIRMED | correct in `layouts.json` |
| `gfxMaterial` | **0x48** | CONFIRMED | **`layouts.json` has 0x44 — 4 bytes short** |
| `gfxVertexBuffer` | 0x10 | CONFIRMED *(Round 2, §11)* | correct in `layouts.json`; two unions documented |
| `gfxStridedData` | **0x30** | CONFIRMED *(Round 2, §10)* | **missing**; emitted in `data/layouts_gfx_round2.json` |
| `gfxPacket` | 0x2C | CONFIRMED *(Round 3, §16)* | offsets correct in `layouts.json`; **four names are wrong** |
| `gfxPacketList` | 0x2C | CONFIRMED *(Round 3, §21)* | it *is* a `gfxPacket`; the duplicate entry is correct |
| `gfxReskin` | **0x10** | CONFIRMED *(Round 3, §17)* | **missing**; emitted in `data/layouts_gfx_round3.json`, merges cleanly |
| `modStatic` | 0x10 | CONFIRMED *(Round 3, §20)* | correct in `layouts.json` |
| `modShader` | 0x8 | CONFIRMED *(Round 3, §20)* | correct in `layouts.json` |

---

## 1. `gfxPipeline` — a namespace of statics, not an object

**CONFIRMED. There is nothing to recover, and that is the answer, not a gap.**

MM2Hook's model (`sizeof == 1`) is right, and for the right reason: the class is genuinely empty.

The evidence is the mangling, which cannot be annotated away. All 55 `gfxPipeline` functions in
`data/symbols.json` carry `static: true`, and every mangled name is one of `@@SA…` (public static
`__cdecl`), `@@CA…` (private static `__cdecl`) or `@@CG…` (private static `__stdcall`). Not one is
`@@QAE…` or `@@AAE…`, the `__thiscall` forms that a member function would need. Counted over
`MM2_RE_KIT/MM2_PSEUDOCODE/gfxPipeline/gfxPipeline.c`:

```
 4  CAXP…   14  SAXX…    8  SAXW…    6  SA_N…    5  SAXH…
 2  CGJP…    4  SAXP…    3  SAPA…    2  SAXA…    2  SAXI…   1 SAHX…  1 SAX_…
```

There is also no constructor, no destructor, no vftable, no `this` parameter in any body, and no
`operator new` site anywhere in the kit that produces a `gfxPipeline`.

**So the layout question is the wrong question for this class.** The contract an OpenGL device layer
has to honour is not a set of field offsets but a set of `.data` addresses, because the
still-assembly code reads and writes these globals directly:

| RVA | Symbol | Type |
|---|---|---|
| `0x006830E4` | `m_ZDepth` | `i32` |
| `0x006830EC` | `m_X` | `i32` |
| `0x006830F4` | `m_fWidth` | `f32` |
| `0x006830F8` | `m_ColorDepth` | `i32` |
| `0x006830FC` | `VP` | `gfxViewport*` (public) |
| `0x00683100` | `m_iHeight` | `i32` — see the mislabel note below |
| `0x0068310C` | `OrthoVP` | `gfxViewport*` (public) |
| `0x00683110` | `m_Y` | `i32` |
| `0x00683114` | `m_EvtFlags` | `u32` |
| `0x00683120` | `m_fHeight` | `f32` |
| `0x00683124` | `m_Viewport` | `gfxViewport*` |
| `0x00683128` | `m_iWidth` | `i32` |
| `0x006844F4` | `m_RenderTarget` | `const gfxTexture* const` |
| `0x0068578C` | `FadeValue` | `f32` |
| `0x00685790` | `StartFadeColor` | `u32` |
| `0x00685794` | `EndFadeColor` | `u32` |
| `0x00685798` | `FadeSpeed` | `f32` |
| `0x0068579C` | `FadeColor` | `u32` |

These are **not contiguous and not in declaration order** — `gfxDebug`, `gfxIcon`, `ATOM_class`,
`gfxLostCallback` and `lpWindowTitle` are interleaved among them, because the linker placed each
static independently. Anyone tempted to model `gfxPipeline` as a struct spanning `0x006830E4` to
`0x0068312C` would be modelling the link order of a translation unit, not a class.

`code/midtown2/gfx/gfxpipeline.h` already declares all 18 correctly as `ARTS_IMPORT static`.

It also declares two that are not statics at all, and they will not compile:

```cpp
// ?CreateViewport@gfxPipeline@@SAPAVgfxViewport@@XZ_SEH
ARTS_IMPORT static void CreateViewport;
// ?CreateVertexBuffer@gfxPipeline@@SAPAUgfxVertexBuffer@@II_N@Z_SEH
ARTS_IMPORT static void CreateVertexBuffer;
```

Those two `_SEH` symbols (`0x005AB54B`, `0x005AB56B`) are the compiler's exception-handling tables
for the functions of the same name, not data members — and each redeclares, as a `void` object, a
name already declared a few lines above as a static function. This is not specific to `gfxPipeline`:
`data/symbols.json` has **681** `kind: "data"` symbols whose mangled name ends in `_SEH`, spread
over 200-odd classes (`UIMenu` has 22, `mmInterface` 17), so `tools/genheaders.py` should filter the
`_SEH` suffix the same way `COMPILER_GENERATED` filters backtick names. Out of scope for this
recovery, but it is the first thing that will stop `gfxpipeline.h` compiling.

The last line —

```cpp
// check_size(gfxPipeline, 0x1); // size known, members are not - cannot verify
```

— should **stay commented**. `tools/genheaders.py` already reasons this out at line 393: an empty
class is `sizeof == 1` on both sides, so a live guard passes while certifying nothing. This
recovery adds the missing half of that reasoning: 0x1 is not merely unverifiable here, it is
*correct*, and there is no future recovery that will make the guard meaningful.

`tools/merge_layouts.py` will also refuse `gfxPipeline` (`no members or no size`), which is the
right outcome. It is deliberately **not** listed under `layouts` in
`data/layouts_gfx_recovered.json` — a rejection reading "no members" would misrepresent a class
that correctly has none. It is recorded under `static_only_classes` instead.

### The mislabel, corrected

`docs/ui_scaling.md` is right that

```c
std::function<float __cdecl (void)>::function<float __cdecl (void)>
```

is `gfxPipeline::m_iHeight`, and that missing it makes half the UI code unreadable. Its
*justification* is wrong, and worth fixing so nobody re-derives the wrong address from it:

- the mislabelled symbol is at **`0x00683100`**, between `VP` and `gfxDebug`
- `m_iWidth` is at `0x00683128`, **0x28 bytes later**, not immediately before it

The identification does not rest on adjacency at all. The linker map carries the real mangled name
`?m_iHeight@gfxPipeline@@0HA` at `0x00683100` (`data/symbols.json`), while the kit's IDB has
`??0?$function@$$A6AMXZ@std@@QAE@ABV01@@Z` at the same address — a *function* mangling attached to a
`.data` slot, which is what gives it away. Two usage sites confirm the semantics independently:
`AboutMenu.c:45-47` multiplies `Position.y` and `Position.w` by it while `Position.x` gets
`m_iWidth`, and `asCamera.c:99` computes `m_iWidth * a / (X * b)` — an aspect ratio.

`data/symbols.json` already has this right. I have corrected the one sentence in
`docs/ui_scaling.md`.

---

## 2. `gfxTexture` — 0x34, CONFIRMED

Already correct in `data/layouts.json` and emitted with a live `check_size(gfxTexture, 0x34)`.
Re-derived here from three independent directions:

- **five allocation sites** spell the size as a literal: `(gfxTexture *)operator new(52)` at
  `gfxTexture.c:376`, `:1024`, `:1112`, `:1128`, `:1218`
- **constructor extent** (`data/layouts_from_ctors.json`, ctor `0x4AC8E0`) reaches `0x32`, which
  aligns to `0x34`, and writes 9 offsets that match the type library exactly
- IDA type library and MM2Hook agree on 52 and on every field

| Offset | Type | Name |
|---|---|---|
| `0x00` | `u32` | `VglBindIndex` |
| `0x04` | `const char*` | `Name` |
| `0x08` | `u16` | `Width` |
| `0x0A` | `u16` | `Height` |
| `0x0C` | `u32` | `TexEnv` |
| `0x10` | `u32` | `DominantColor` |
| `0x14` | `IDirectDrawSurface7*` | `m_Surface` |
| `0x18` | `IDirectDrawPalette*` | `m_Palette` |
| `0x1C` | `gfxTextureCacheEntry*` | `CacheEntry` |
| `0x20` | `gfxTextureCachePool*` | `CachePool` |
| `0x24` | `u32` | `m_RefCount` |
| `0x28` | `gfxTexture*` | `m_Next` |
| `0x2C` | `gfxTexture*` | `NextLOD` |
| `0x30` | `u8` | `m_LOD` |
| `0x31` | `u8` | `m_MaxLOD` |
| `0x32` | `u8[2]` | `EndPadding` |

For the GL layer: `0x14` and `0x18` are the two slots that stop meaning anything once DirectDraw is
gone. They are the natural home for a GL texture name and a sampler/PBO handle, and they are
pointer-width, so a replacement fits without moving anything after them.

---

## 3. `gfxImage` — 0x1C, CONFIRMED

Already correct in `data/layouts.json`. Size proven by `(gfxImage *)operator new(0x1C)` at
`_MISLABELED_FUNCTIONS.c:3809`; MM2Hook and the IDA library agree.

| Offset | Type | Name |
|---|---|---|
| `0x00` | `u16` | `Width` |
| `0x02` | `u16` | `Height` |
| `0x04` | `u16` | `BytesPerRow` |
| `0x06` | `u8` | `Type` |
| `0x07` | `u8` | `PaletteType` |
| `0x08` | `u32` | `TexEnv` |
| `0x0C` | `void*` | `pImageData` |
| `0x10` | `void*` | `PaletteData` |
| `0x14` | `u32` | `RefCount` |
| `0x18` | `gfxImage*` | `Next` |

**`0x04` is a row pitch, not a byte count.** MM2Hook names it `Size`, which would be a costly thing
to believe. `gfxImage.c:158` sets it as `this->BytesPerRow = v6 * Width` (bytes-per-pixel times
width), and every consumer uses it as a stride — `:191` `&v12[this->BytesPerRow]`, `:305`
`lpImage += 2 * BytesPerRow`, `:706` `&pImageData[v2 * BytesPerRow]`. The 1999 name in the IDA
library is the correct one and `layouts.json` already carries it. Confidence: CONFIRMED.

`Type` at `0x06` is the `gfxImageFormat` enum: `ARGB_8888=1, RGB_0888=2, ARGB_1555=3, RGB_0555=4,
Palette8=5, Palette4=6` (`gfxImage.h` in the kit). That is the whole set of formats a GL upload path
has to handle.

---

## 4. `gfxBitmap` — 0x14, CONFIRMED

Already correct in `data/layouts.json`. `(gfxBitmap *)operator new(20)` at `gfxBitmap.c:167`;
constructor `0x4AE370` writes `0x0/0x4/0x6/0x8/0xC/0x10` and stops, which both bounds the object and
matches the field list.

| Offset | Type | Name |
|---|---|---|
| `0x00` | `const char*` | `Name` |
| `0x04` | `u16` | `Width` |
| `0x06` | `u16` | `Height` |
| `0x08` | `IDirectDrawSurface7*` | `Surface` |
| `0x0C` | `u32` | `RefCount` |
| `0x10` | `gfxBitmap*` | `Prev` |

The two sources disagree on the name of `0x10` only: the 1999 struct says `Prev`, MM2Hook says
`Next`. It is the intrusive list link either way; the offset is not in doubt.

`0x08` is the second DirectDraw slot the GL layer has to repurpose. `gfxPipeline::CopyBitmap` is a
`BltFast` against this surface — see `docs/ui_scaling.md` for why replacing it with a *scaling* blit
is one of the four things arbitrary resolution needs.

---

## 5. `gfxInterface` and the resolution list

### `gfxResData` — 0x8, CONFIRMED, and missing from `layouts.json`

`data/layouts.json` has this struct twice, under `mmGraphicsInterface::mmResolution` and
`MM2::gfxResData`, but **not** under the bare `gfxResData` that `tools/genheaders.py` would look up.
It is emitted in `data/layouts_gfx_recovered.json` and will merge cleanly.

| Offset | Type | Name |
|---|---|---|
| `0x00` | `u16` | `ScreenWidth` |
| `0x02` | `u16` | `ScreenHeight` |
| `0x04` | `u16` | `ColorDepth` |
| `0x06` | `u16` | `Flags` |

The best evidence is a loop the decompiler could not disguise, in `gfxFindSafeAdapter` (`0x4AC820`,
`_FREE_FUNCTIONS.c:19612`):

```c
p_ScreenHeight = &gfxInterfaces[0].Resolutions[0].ScreenHeight;   // u16*
do {
    if ( *(p_ScreenHeight - 1) == 640 && *p_ScreenHeight == 480 && p_ScreenHeight[1] == 16 )
        gfxInterfaces[0].ResolutionChoice = i;
    ++i;
    p_ScreenHeight += 4;                                          // 4 * u16 = 8 byte stride
} while ( i < (int)gfxInterfaces[0].ResolutionCount );
```

The `+= 4` on a `u16*` is the compiler's own arithmetic and pins the stride at 8. The three
comparisons pin width at `-1`, height at `0`, depth at `+1` relative to `ScreenHeight` — i.e.
`0x0`, `0x2`, `0x4`. `ResCallback` (`0x4AC6F0`) writes the same three fields by name.

`0x6` is the one field whose *name* is unsettled: the 1999 struct calls it `Is16BitColor`, MM2Hook
calls it `Flags`, and `ResCallback` stores `(dwBitDepth == 16) + 6` — 7 for 16-bit, 6 otherwise.
That is a small enum, so `Is16BitColor` is misleading. Kept as `Flags`. The offset and width are not
in doubt.

### `gfxInterface` — 0x270, CONFIRMED

Already correct in `data/layouts.json`.

| Offset | Type | Name |
|---|---|---|
| `0x00` | `GUID` | `GUID` |
| `0x10` | `char[64]` | `Name` |
| `0x50` | `u32` | `DeviceCaps` |
| `0x54` | `mmRenderer` | `Renderer` (`Software=0, Hardware=1, HardwareWithTnL=2`) |
| `0x58` | `u32` | `ResolutionCount` |
| `0x5C` | `u32` | `ResolutionChoice` |
| `0x60` | `mmColorDepthFlag` | `AcceptableDepths` (`Depth32=0x100, Depth24=0x200, Depth16=0x400`) |
| `0x64` | `u32` | `AvailableMemory` |
| `0x68` | `u32` | `VendorID` |
| `0x6C` | `u32` | `DeviceID` |
| `0x70` | `gfxResData[64]` | `Resolutions` |

`0x70 + 64*8 = 0x270`, so the members tile the struct exactly. The size is independently pinned by
the array: `gfxInterfaces` sits at `0x00683130` and the next `.data` symbol, `gfxMinScreenWidth`, is
at `0x006844B0` — an extent of `0x1380`, which is exactly `8 × 0x270`. So the global is
`gfxInterface gfxInterfaces[8]`. MM2Hook agrees on 624 independently.

**No header is generated for this class**, because no function or data symbol in the binary is
scoped to `gfxInterface` — the array is a free-standing global. `tools/genheaders.py` is
symbol-driven, so `code/midtown2/gfx/` has no `gfxinterface.h` and never will without a hand-written
one. The same applies to `gfxResData`. Both will have to be written by hand for the device layer.

### The globals around it

| RVA | Symbol |
|---|---|
| `0x00683130` | `gfxInterface gfxInterfaces[8]` |
| `0x006844B0` | `gfxMinScreenWidth` |
| `0x006844C0` | `gfxInterfaceCount` |
| `0x006844C8` | `gfxInterfaceChoice` |
| `0x006844CC` | `gfxMinScreenHeight` |
| `0x006844D4` | `gfxInterfaceEnumIdx` |
| `0x006844D8` | `gfxMaxScreenHeight` |
| `0x006844FC` | `gfxMaxScreenWidth` |

Two notes for whoever raises the resolution cap:

1. `docs/resolution.md` says the cap width lands in `0x006844FC` with "height in the adjacent slot".
   The width address is right — it is `gfxMaxScreenWidth` — but the height is **`0x006844D8`**
   (`gfxMaxScreenHeight`), 0x24 bytes earlier, not adjacent. The two `mov`s in `AutoDetectCallback`
   target unrelated slots.
2. `ResCallback` writes `interface->Resolutions[interface->ResolutionCount]` and then increments,
   **with no bound check against the 64-entry array**. This is the identical unclamped-`add_resolution`
   bug `docs/resolution_blueprint.md` records in Open1560. MM2 gets away with it for the same reason:
   DirectDraw enumerates far fewer than 64 modes. A synthesized list must clamp.

---

## 6. `gfxRenderState` — 0x50, CONFIRMED. **MM2Hook's 0x98 is wrong.**

`data/layouts.json` has `gfxRenderState` at `0x50` with a single member, `gfxRenderStateData State`
at `0x00`, and `code/midtown2/gfx/gfxrenderstate.h` carries a live `check_size(gfxRenderState, 0x50)`.
**That is correct and should not be changed.**

It looks wrong, which is why this section exists. Three things point at `0x98`:

- `data/mm2types.json` gives `gfxRenderState` a `sizeof` of 152
- `MM2::gfxRenderState` in the kit's `_ALL_TYPES.h` is `sizeof=0x98`, as
  `{ gfxRenderStateData Data; gfxMaterial Material; }`
- `data/symbols.json` lists `RSTATE` at `0x006856A0` with `size: 152`

All three collapse under inspection.

The first two are the same source counted twice. `MM2::…` types in `_ALL_TYPES.h` are MM2Hook's own
C++ headers, compiled into the hook DLL and dumped back out of the IDB; `mm2types.json` is MM2Hook's
`sizeof` table. They are one claim, not two.

The third is an artefact of the gap-derived sizing in `data/symbols.json`. Sizes there are the
distance to the next known symbol, and the project's linker map is **missing**
`?FlatWhite@gfxMaterial@@2U1@A`. `MM2_RE_KIT/inventory/symbols.json` has it, at `0x006856F0`:

```
0x006856A0   ?RSTATE@@3VgfxRenderState@@A          (project map)
0x006856F0   ?FlatWhite@gfxMaterial@@2U1@A         (kit inventory)   <- 0x50 later
0x00685738   ?sm_Modelview@gfxRenderState@@0VMatrix44@@A              <- 0x48 after that
```

Two distinct objects cannot overlap, so `sizeof(gfxRenderState) <= 0x50`; it contains a
`gfxRenderStateData`, so it is `>= 0x50`. It is exactly `0x50`. **MM2Hook mistook the adjacent
`gfxMaterial::FlatWhite` for a trailing member of `RSTATE`.**

Corroborating from the code side: every instance access in
`MM2_RE_KIT/MM2_PSEUDOCODE/gfxRenderState/gfxRenderState.c` — all 21 functions — is `this->State.…`.
Not one reaches past `0x50`.

`gfxRenderStateData` at `0x50` is separately pinned by `LASTRSTATE` at `0x006854A0` running to
`gfxRenderState::sm_SupportsBlendWithOne` at `0x006854F0`. Its member list in `layouts.json` is
correct; the two sources differ only on names (`TextureFactor`/`TextureArg`, `BlendState`/`BlendSet`,
`Cullmode`/`CullMode`, `m_COLOROP1`/`ColorOp1`, `field_3D`/`ColorOp2`). `FogDensity` at `0x38` is
typed `i32` in `layouts.json` and `float` by MM2Hook — a naming/typing question, not a layout one,
but the GL layer should treat it as a float.

`gfxRenderState`'s statics matter as much as its instance data, since the render state is effectively
a singleton (`RSTATE`) with a shadow copy (`LASTRSTATE`):

| RVA | Symbol |
|---|---|
| `0x005CD600` | `sm_MaxActiveLights` |
| `0x00685460` | `sm_Camera` (`Matrix44`) |
| `0x006854A0` | `LASTRSTATE` (`gfxRenderStateData`, a free global) |
| `0x006854F0` | `sm_SupportsBlendWithOne` |
| `0x006854F8` | `sm_Composite` |
| `0x00685538` | `sm_World` |
| `0x006855E0` | `sm_Billboard` — kit only, absent from the project map |
| `0x00685620` | `sm_FullComposite` |
| `0x00685660` | `sm_View` |
| `0x006856A0` | `RSTATE` (`gfxRenderState`, a free global) |
| `0x00685738` | `sm_Modelview` |
| `0x00685778` | `m_Touched` |
| `0x0068577C` | `sm_MaxBlendMatrices` |
| `0x00685780` | `m_TouchedMask` — kit only |
| `0x00685784` | `sm_MaxLights` — kit only at this address |
| `0x00685788` | `sm_MaxTextures` |

`m_Touched` is the dirty-bit word the whole class is built around (`|= 1` for render state, `|= 4`
for material), and it is what a GL backend's flush would key off.

---

## 7. `gfxMaterial` — 0x48, and `layouts.json` says 0x44

**This is a live error, and the only one this recovery cannot fix on its own.**

`data/layouts.json` has `gfxMaterial` at `0x44` with five anonymous `D3DMATERIAL7` unions —
it is `D3DMATERIAL7` copied verbatim. The real struct is `D3DMATERIAL7` **plus one appended field**:

| Offset | Type | Name | |
|---|---|---|---|
| `0x00` | `Vector4` | `Diffuse` | `D3DMATERIAL7.dcvDiffuse` |
| `0x10` | `Vector4` | `Ambient` | `D3DMATERIAL7.dcvAmbient` |
| `0x20` | `Vector4` | `Specular` | `D3DMATERIAL7.dcvSpecular` |
| `0x30` | `Vector4` | `Emissive` | `D3DMATERIAL7.dcvEmissive` |
| `0x40` | `f32` | `Shininess` | `D3DMATERIAL7.dvPower` |
| `0x44` | `u32` | `Color` | **MM2's own** |

Three pieces of evidence, and the first is decisive on its own:

1. `gfxMaterial::Reset` (`0x4B1BB0`) ends with `this[1].diffuse.r = NAN;`. That is Hex-Rays
   rendering a store to `this + 0x44` against its own `0x44`-wide struct — the only way to express
   an offset that its type says does not exist. A store at `0x44` means the object is at least
   `0x48`.
2. `gfxRenderState::Default` (`0x4B1DA0`) writes `dword_685734 = -1` immediately before setting
   `FlatWhite`'s colours. `0x00685734` is `FlatWhite + 0x44`, and `-1` is opaque-white ARGB.
3. `gfxMaterial::FlatWhite` at `0x006856F0` runs to `sm_Modelview` at `0x00685738` — an extent of
   exactly `0x48`. `MM2::gfxMaterial` in the same type library independently says `0x48` with
   `ColorARGB Color` at `0x44`, and MM2Hook's `sizeof` says 72.

(This is the one place MM2Hook is right and the unqualified 1999 struct is wrong — the mirror image
of `gfxRenderState` above. Neither source wins by default.)

### What has to happen, and why the tool cannot do it

`code/midtown2/gfx/gfxmaterial.h` currently ends with

```cpp
// check_size(gfxMaterial, 0x44); // size known, members are not - cannot verify
```

That comment is wrong by four bytes. It is inert today — the five `layouts.json` members have
`type: null` and `name: null`, so `emit_members` drops them all and the guard stays commented —
but it is a wrong number sitting where a reader will trust it.

A corrected `gfxMaterial` **is** in `data/layouts_gfx_recovered.json`, tiling `0x48` exactly. It
will be **refused** by `tools/merge_layouts.py`, with the reason `already in the IDA type library`.
That is rule 1 working as designed and the rejection should not be worked around: the rule exists
precisely so that a recovery cannot silently overwrite the most-verified source. Applying this needs
a deliberate human edit to `data/layouts.json`, made with the three pieces of evidence above in view.

Until then: **do not size a `gfxMaterial` from `layouts.json`.** A `gfxMaterial[]` built on `0x44`
would stride wrongly through `modShader::AddStaticMaterial`'s pool.

---

## 8. `gfxViewport` — 0x17C, CONFIRMED

Already correct in `data/layouts.json` with a live `check_size(gfxViewport, 0x17C)`. Size proven by
the allocation in `gfxPipeline::CreateViewport` (`0x4A90B0`), which the kit mislabels
`ImIsPowerOfTwo`:

```c
viewport = operator new(0x17C);
if ( viewport ) return gfxViewport::gfxViewport((gfxViewport *)viewport);
```

The constructor at `0x4B13A0` independently reaches `0x17C` (`data/layouts_from_ctors.json`).

| Offset | Type | Name |
|---|---|---|
| `0x000` | `Matrix44` | `m_Projection` |
| `0x040` | `Matrix44` | `m_Billboard` (MM2Hook: `m_ViewModel`) |
| `0x080` | `Matrix44` | `Camera` |
| `0x0C0` | `Matrix44` | `World` |
| `0x100`–`0x11C` | `i32 × 8` | `field_100` … `field_11C` |
| `0x120` | `i32` | `Flags` |
| `0x124` | `f32` | `DefaultAspect` |
| `0x128` | `i32` | `field_128` |
| `0x12C` | `f32` | `field_12C` — MM2Hook names it `Near` |
| `0x130` | `f32` | `field_130` — MM2Hook names it `Far` |
| `0x134`–`0x158` | `f32 × 10` | `field_134` … `field_158` |
| `0x15C` | `D3DVIEWPORT7` | `m_Viewport` |
| `0x174` | `f32` | `Fov` |
| `0x178` | `f32` | `Aspect` |

The offsets are CONFIRMED. Twelve of the names are not — `field_100`…`field_158` are unrecovered,
and the `Near`/`Far` naming at `0x12C`/`0x130` is MM2Hook's guess with no corroboration found here.
Treat those two as PROBABLE. This matters for the Hor+ FOV fix
(`docs/resolution_blueprint.md`): `Fov` at `0x174` and `Aspect` at `0x178` are the two the fix
touches, and both are firmly named by both sources.

---

## 9. What is not confirmed

> **Superseded by Round 2 (below).** Four of these five rows have since been resolved. Read
> §10–§14 before acting on this table: `gfxStridedData` is 0x30 (§10), `gfxVertexBuffer` = 0x10 is
> now CONFIRMED and the "no literal allocation site" claim below is simply wrong (§11), nine of the
> `gfxViewport` names are recovered (§13), and `field_2C` is proven dead rather than merely
> unexplained (§14). Only `0x006830E0` is still open. The suggested route for `gfxStridedData` —
> "decoding the reads inside `Draw`" — is a dead end: `Draw` performs no reads.

| | Why |
|---|---|
| `gfxStridedData` | Appears only in `gfxPipeline::Draw`/`DrawIdx` signatures. **No definition exists** in `_ALL_TYPES.h` under any spelling, and no constructor or allocation site was found. Everything about it is unknown, including its size. It is a parameter type on four hot draw entry points, so it will have to be recovered before those are ported — most likely by decoding the reads inside `Draw` (`0x4AAAF0`) against the D3D7 `D3DDRAWPRIMITIVESTRIDEDDATA` shape it plainly mirrors. Not attempted here. |
| `gfxVertexBuffer` = `0x10` | The IDA type library is the only source. MM2Hook has no `sizeof` for it, no constructor extent exists in `data/layouts_from_ctors.json`, and `gfxPipeline::CreateVertexBuffer` (`0x4AB830`) allocates through a path that does not spell a literal. The layout is plausible and internally consistent (`Prev`, `m_Verts`, `VertCount`, `m_ElementSize`, `m_FVF`) but is **not independently corroborated**. |
| `gfxViewport` `0x100`–`0x158` names | Offsets confirmed, names not recovered. |
| `gfxRenderStateData` `0x2C` | `field_2C` / `unk_2C` in both sources. Written by nothing in the class's own code. |
| `.data` slot `0x006830E0` | Between `interfaceCount` and `gfxPipeline::m_ZDepth`, named in neither source. |

---

## Appendix: kit mislabels found while doing this

Each of these is a wrong *name* in `MM2_RE_KIT`, resolved against the linker map in
`data/symbols.json`. Listed because the next person to read this pseudocode will hit them.

| Address | The kit says | It actually is |
|---|---|---|
| `0x00683100` | `??0?$function@$$A6AMXZ@std@@…` | `gfxPipeline::m_iHeight` |
| `0x004A90B0` | `ImIsPowerOfTwo` | `gfxPipeline::CreateViewport` |
| `0x004A9110` | `operator__8` | `gfxPipeline::Manage` |
| `0x004E88C0` | returns `ImPlot::TransformerLinLog*` | `gfxPipeline::GetWidth`, returns `i32` |
| `0x006844B0` | `??0LuaTableIterator@LuaIntf@@…` | `gfxMinScreenWidth` (appears in `ResCallback` as `*(int *)LuaIntf::LuaTableIterator::LuaTableIterator`) |
| `0x00685790` | `??1?$CppBindClass@VgfxTextureCachePool@MM2@@…` | `gfxPipeline::StartFadeColor` |
| `0x00685450` | `??1?$CppBindClass@VdgBangerData@MM2@@…` | `g_Allow8BitImages` |
| `0x00684520` | `??0_Ref_count_base@std@@IAE@XZ` | `gfxVertexBuffer::sm_First` |

The pattern is consistent: the kit's IDB was opened with MM2Hook's own symbols loaded, and MM2Hook's
C++ template instantiations collided with 1999 `.data` slots. Where the kit gives a `std::`,
`LuaIntf::` or `ImPlot::` name to something in the `gfx` address range, **distrust it and check the
map** — and note that the traffic goes both ways, since `gfxMaterial::FlatWhite` and
`gfxRenderState::sm_Billboard` exist only in the kit and not in the map.

---

# Round 2 — the four gaps, closed

Everything below was derived by disassembling `C:\mm2ghidra\bin\midtown2.exe` directly with
`capstone` 5.0.7 through a PE section-mapping helper, so every offset quoted is read off raw
instruction bytes. Where Hex-Rays and the bytes disagree, the bytes are quoted. The machine-readable
half is `data/layouts_gfx_round2.json`.

The summary table above gains three rows and loses one unknown:

| Class | Size | Status after round 2 |
|---|---|---|
| `gfxStridedData` | **0x30** | CONFIRMED — emitted for merge, will be accepted |
| `gfxVertexBuffer` | 0x10 | **CONFIRMED** — was UNVERIFIED; the literal allocation exists after all |
| `gfxRenderState` | 0x50 | **CONFIRMED independently** — the 0x98 claim is refuted, and its cause identified |
| `gfxMaterial` | 0x48 | **CONFIRMED independently** — `layouts.json`'s 0x44 is wrong; hand edit still required |

And one gap stays open on purpose: `gfxViewport` `0x100`–`0x11C` (§13) and
`gfxRenderStateData::field_2C` (§14) are *provably* untouched by the retail binary, so no name
exists to recover.

---

## 10. `gfxStridedData` — 0x30, CONFIRMED

The route suggested for this class — decode the reads inside `gfxPipeline::Draw` at `0x004AAAF0`
— does not exist. **`Draw` performs no reads.** Here is its entire use of the parameter, from
`game.asm.pristine` and confirmed against the binary:

```
mov edx, [ebp+0x10]     ; the gfxStridedData const& - just the pointer
push edx
call [ecx+0x74]         ; IDirect3DDevice7::DrawPrimitiveStrided
```

All four entry points are pass-throughs. Not one field offset is touched. The layout had to come
from somewhere else.

### The binary spells the relationship out in the original source text

MM2's `Errorf` calls quote the failing source line verbatim, and those strings survive in `.data`:

```
lpD3DDev->DrawPrimitiveStrided((D3DPRIMITIVETYPE)dm,vflags, (D3DDRAWPRIMITIVESTRIDEDDATA*)&sd,count,flags)
lpD3DDev->DrawIndexedPrimitiveStrided((D3DPRIMITIVETYPE)dm,vflags, (D3DDRAWPRIMITIVESTRIDEDDATA*)&sd,count,idx,ic,flags)
```

`(D3DDRAWPRIMITIVESTRIDEDDATA*)&sd` is a C-style cast of the address of the `gfxStridedData`
parameter. That settles two things at once: `gfxStridedData` is layout-compatible with the D3D7
struct, and it is a **distinct type of MM2's own** — you do not cast a type to itself. It is the
same "mirror the D3D struct so the gfx headers need not include `d3d.h`" pattern that
`gfxViewport::m_Viewport` uses (its own source line, also preserved, is
`lpD3DDev->SetViewport((LPD3DVIEWPORT7)&m_Viewport)`).

### Two independent stack frames give the size, and it is not the SDK's

The public DX7 struct is

```c
typedef struct _D3DDP_PTRSTRIDE { LPVOID lpvData; DWORD dwStride; } D3DDP_PTRSTRIDE;
typedef struct _D3DDRAWPRIMITIVESTRIDEDDATA {
    D3DDP_PTRSTRIDE position, normal, diffuse, specular;
    D3DDP_PTRSTRIDE textureCoords[D3DDP_MAXTEXCOORD];   /* D3DDP_MAXTEXCOORD == 8 */
} D3DDRAWPRIMITIVESTRIDEDDATA;
```

which is `12 x 8 = 0x60`. **MM2's mirror is 0x30, with two texture-coordinate sets, not eight.**
The only two places in the whole binary that construct such a block are the two `gfxPacket::Draw`
overloads, and both agree.

`gfxPacket::Draw(uint)` at `0x004B3340` — the frame is `sub esp, 0x30` and the struct is the
*only* local, so the allocation is the declared size:

| Frame slot | Struct offset | Written with | D3D field |
|---|---|---|---|
| `ebp-0x30` | `0x00` | `this->Vertices` | `position.lpvData` |
| `ebp-0x2C` | `0x04` | `gfxFVFSize(fvf)` | `position.dwStride` |
| `ebp-0x28` | `0x08` | `Vertices + gfxFVFOffset(fvf, 0x010)` | `normal.lpvData` |
| `ebp-0x24` | `0x0C` | `gfxFVFSize(fvf)` | `normal.dwStride` |
| `ebp-0x20` | `0x10` | `lea edx,[ebp+8]` — the address of the colour argument | `diffuse.lpvData` |
| `ebp-0x1C` | `0x14` | `0` — stride zero, one colour for every vertex | `diffuse.dwStride` |
| `ebp-0x18` | `0x18` | *not written* (this FVF has no specular) | `specular.lpvData` |
| `ebp-0x14` | `0x1C` | *not written* | `specular.dwStride` |
| `ebp-0x10` | `0x20` | `Vertices + gfxFVFOffset(fvf, 0x100)` | `textureCoords[0].lpvData` |
| `ebp-0x0C` | `0x24` | `gfxFVFSize(fvf)` | `textureCoords[0].dwStride` |
| `ebp-0x08` | `0x28` | *not written* | `textureCoords[1].lpvData` |
| `ebp-0x04` | `0x2C` | *not written* | `textureCoords[1].dwStride` |

The `0x010`, `0x040` and `0x100` passed to `gfxFVFOffset` are `D3DFVF_NORMAL`, `D3DFVF_DIFFUSE` and
`D3DFVF_TEX1`, so the field order is not assumed from the SDK header — the code names each slot by
the FVF bit whose offset it stores there.

`gfxPacket::Draw(const Matrix44*, uint)` at `0x004B34E0` is the independent check. Its frame is
`sub esp, 0x48` and the block passed to D3D is `lea esi, [ebp-0x48]`. It writes `+0x00` through
`+0x24` including the specular pair this time (`+0x18` gets the address of a zeroed local, `+0x1C`
gets stride 0), and then the *next distinct local* sits at `ebp-0x18` — i.e. `struct base + 0x30`.
The struct therefore ends at `0x30` here too.

One frame is exactly `0x30` with nothing else in it; the other is bounded above at `0x30` by the
next local. **`sizeof(gfxStridedData) == 0x30`.**

### What is proven and what is inferred

Proven: the 0x30 six-pair block, its field order, and that this is the only strided shape MM2 ever
builds. Inferred: that the declared type of those two locals is spelled `gfxStridedData` rather than
an ad-hoc array. There is no other candidate — the type exists, it has exactly this shape, and it is
the parameter type of the only functions that consume such a block.

The inference is also safe in the one direction that matters. `0x30` cannot break an ABI, because
**no assembly caller of these functions exists** (§15), so both sides of the boundary will be our
own header. And if a port declared the full `0x60` instead, the six unwritten texture-coordinate
pairs would be uninitialised stack whenever an FVF asked for more than two texture sets. `0x30` is
both what the binary builds and the safer declaration.

### The one false lead, recorded so nobody re-chases it

A byte-pattern scan for `ff 51 74` (`call [ecx+0x74]`) turns up `0x00570D8F` as a third apparent
`DrawPrimitiveStrided` call. It is not: it is DirectPlay. `[ecx+0x74]` there is
`IDirectPlay4::GetPlayerData`, the `0x8877xxxx` values it compares against are `DPERR_` codes, and
the strings it pushes are `DPLAY::GetPlayerData -- DPERR_INVALIDFLAGS` and friends. It builds no
struct.

---

## 11. `gfxVertexBuffer` — 0x10, now CONFIRMED

The previous round graded this UNVERIFIED because `gfxPipeline::CreateVertexBuffer` "allocates
through a path that does not spell a literal". It does spell one, twice — Hex-Rays had folded it
away. From the bytes of `0x004AB830`:

```
004ab8dd  6a10        push 0x10
004ab8df  e87cba0c00  call 0x577360        ; operator new
004ab8e4  83c404      add esp, 4
004ab8e7  85c0        test eax, eax
004ab8e9  744e        je 0x4ab939
004ab8eb  8b55f0      mov edx, [ebp-0x10]
004ab8ee  8bc8        mov ecx, eax
004ab8f0  52          push edx
004ab8f1  e86afdffff  call 0x4ab660        ; gfxVertexBuffer::gfxVertexBuffer(IDirect3DVertexBuffer7*)
```

and again at `0x004AB904` in the other branch. `sizeof(gfxVertexBuffer) == 0x10`, as a literal, at
the only two sites that ever allocate one.

Both constructors corroborate it and pin every offset. `ctor(IDirect3DVertexBuffer7*)` at
`0x004AB660` writes `[this]`, `[this+4]`, `[this+8]`, `[this+0xC]` and returns.
`ctor(uint count, uint fvf)` at `0x004AB690` is the decisive one because it uses **16-bit** stores:

```
mov  [esi], eax                 ; eax = sm_First   -> link at 0x00
mov  [0x684520], esi            ; sm_First = this
call gfxFVFSize ; imul eax, count ; call operator new
mov  [esi+4], eax               ; m_Verts        -> 0x04
mov  word ptr [esi+8], bx       ; bx = count     -> VertCount, u16 at 0x08
call gfxFVFSize
mov  word ptr [esi+0xA], ax     ; m_ElementSize, u16 at 0x0A
mov  [esi+0xC], edi             ; edi = fvf      -> m_FVF, u32 at 0x0C
```

Nothing in the class, or in `gfxPipeline`'s ten vertex-buffer functions, touches `0x10` or beyond.

### The real 1999 names, from the binary's own source text

Four more preserved `Errorf` lines name the fields directly:

```
lpD3DDev->DrawPrimitiveVB((D3DPRIMITIVETYPE)dm,vb->m_VB,startOffset,count,0)
lpD3DDev->DrawPrimitive((D3DPRIMITIVETYPE)dm,vb->m_FVF,(void*)((u32)vb->m_Verts + vb->m_ElementSize * startOffset),count,0)
lpD3DDev->DrawIndexedPrimitiveVB((D3DPRIMITIVETYPE)dm,vb->m_VB,startOffset,vcount,idx,ic,0)
lpD3DDev->DrawIndexedPrimitive((D3DPRIMITIVETYPE)dm,vb->m_FVF,(void*)((u32)vb->m_Verts + vb->m_ElementSize * startOffset),vcount,idx,ic,0)
```

`m_VB` does not appear in `layouts.json`'s member list at all, which looks like a contradiction in a
0x10-byte struct that already has five members. It is not — **there are two unions**, and
`gfxPipeline::DrawVB` (`0x004ABA50`) shows the discriminator:

```
mov ecx, [eax+0x0C]      ; m_FVF
test ecx, ecx
jne  <system-memory path>
   ; m_FVF == 0: a real D3D vertex buffer
   mov eax, [eax+4]      ; m_VB
   call [edx+0x7C]       ; DrawPrimitiveVB
<system-memory path>:
   mov esi, [eax+4]      ; m_Verts
   mov  dx, [eax+0x0A]   ; m_ElementSize
   imul edx, startOffset
   add  edx, esi         ; m_Verts + m_ElementSize * startOffset
   call [esi+0x64]       ; DrawPrimitive
```

| Offset | `m_FVF != 0` (system memory) | `m_FVF == 0` (D3D vertex buffer) |
|---|---|---|
| `0x00` | list link (`layouts.json` says `Prev`; the ctor pushes and the dtor unlinks, so it is a `Next`) | same |
| `0x04` | `void* m_Verts` | `IDirect3DVertexBuffer7* m_VB` |
| `0x08` | `u16 VertCount` | `void*` — the system-memory backup |
| `0x0A` | `u16 m_ElementSize` | (part of the pointer above) |
| `0x0C` | `u32 m_FVF` | `0`, and that zero *is* the discriminator |

The `0x08` union is real, not a guess: `gfxPipeline::PersistVertexBuffer` (`0x004ABC50`) locks the
D3D buffer, `malloc`s, stores the result in `[vb+8]` and copies into it; `RestoreAll`
(`0x004AB730`) `rep movsd`s back *from* `[vb+8]` after a re-lock; and the destructor frees `[this+8]`
on exactly that branch. In the D3D-VB constructor `[this+8]` is zeroed, so the `free` is a harmless
`free(NULL)` when nothing was persisted.

`layouts.json`'s entry has the right size and the right offsets, so **no merge is needed**. The
union structure and the `Prev`/`Next` correction are name-level improvements recorded in
`data/layouts_gfx_round2.json` under `pending_hand_edits`.

The appendix entry claiming `0x00684520` is `gfxVertexBuffer::sm_First` is also confirmed in
passing: both constructors push `this` onto the list headed there, and `KillAll` (`0x004AB800`)
drains it.

---

## 12. The two contested corrections — both CONFIRMED, independently

Neither verdict rests on the previous round's reasoning. Both were re-derived from raw bytes.

### `gfxMaterial` is 0x48. `layouts.json`'s 0x44 is wrong.

`gfxMaterial::Reset` at `0x004B1BB0`, in full, is nineteen stores and a return:

```
004b1bb0  b80000803f      mov eax, 0x3f800000        ; 1.0f
004b1bb5  8901            mov [ecx], eax
   ... +0x04 +0x08 +0x0C +0x10 +0x14 +0x18 +0x1C  all 1.0f   (Diffuse, Ambient)
004b1bcc  33c0            xor eax, eax
004b1bce  894120          mov [ecx+0x20], eax
   ... +0x24 ... +0x40  all zero                        (Specular, Emissive, Shininess)
004b1be9  c74144ffffffff  mov dword ptr [ecx+0x44], 0xFFFFFFFF
004b1bf0  c3              ret
```

`c7 41 44 ff ff ff ff` is a four-byte store at offset `0x44` of opaque-white ARGB. The previous
round inferred this from Hex-Rays rendering it as `this[1].diffuse.r = NAN`; here it is as the
instruction. A dword written at `0x44` means the object is at least `0x48`.

Two more proofs, both new:

- `0x004B1B80` is a two-instruction thunk — `mov ecx, 0x006856F0` / `jmp 0x004B1BB0` — which applies
  `Reset` to `gfxMaterial::FlatWhite`. That fixes FlatWhite's address independently of the kit's
  symbol table, which is where the previous round got it.
- FlatWhite's extent is then bounded on both sides: `gfxRenderState::Default` (`0x004B1DA0`) writes
  `0x006856F0` through `0x0068570C` and `0x00685734` (= FlatWhite + `0x44`), and the next symbol,
  `sm_Modelview`, begins at `0x00685738` = FlatWhite + `0x48`.

**Verdict: the previous round is right.** `data/layouts.json` must go from `0x44` to `0x48` with a
`u32 Color` at `0x44`. `merge_layouts.py` rule 1 will refuse the corrected entry that already sits in
`data/layouts_gfx_recovered.json`, so the hand edit is still required.

### `gfxRenderState` is 0x50. MM2Hook's 0x98 is wrong — and here is why it went wrong.

The previous round argued from a symbol gap. That argument holds, but it depends on trusting the
kit's `?FlatWhite@gfxMaterial@@2U1@A` at `0x006856F0`, which the project's own linker map lacks.
Two stronger, symbol-free proofs:

**1. The last field of the object is a pointer at `0x4C`, and it points at FlatWhite.**
`gfxRenderState::Default` opens with:

```
004b1dfe  8b4e4c      mov ecx, [esi+0x4C]
004b1e01  b8f0566800  mov eax, 0x006856F0
004b1e06  3bc8        cmp ecx, eax
004b1e08  740f        je  ...
004b1e0a  89464c      mov [esi+0x4C], eax
```

`data/layouts.json` already carries `gfxMaterial* m_Material` at `0x4C` inside `gfxRenderStateData`.
`0x4C + 4 = 0x50`.

**2. Nothing in the binary reads RSTATE + 0x50 or beyond.** Scanning every absolute dword in `.text`
that falls in `[0x685690, 0x685745)` gives a sharp picture:

```
0x006856A0 ... 0x006856EC   hundreds of references, from all over the game   <- RSTATE's fields
0x006856F0 ... 0x00685734   referenced ONLY by gfxRenderState::Default and the Reset thunk at 0x004B1B80
0x00685738 ...              sm_Modelview, referenced as a Matrix44
```

The last of RSTATE's own fields to be touched is `0x006856EC` = RSTATE + `0x4C`. If
`gfxRenderState` were `0x98`, the region `0x006856F0`-`0x00685737` would be its members and the game
would read them; instead only the two functions that initialise `gfxMaterial::FlatWhite` ever go
there.

**The cause of MM2Hook's error is now exact.** `gfxRenderState` is
`{ gfxRenderStateData State; }` whose `State.m_Material` at `0x4C` is a **pointer** defaulting to
`&gfxMaterial::FlatWhite`. Because FlatWhite happens to be linked immediately after `RSTATE`,
MM2Hook saw a `gfxMaterial`-shaped blob adjacent to the object and modelled it as an embedded
`gfxMaterial Material` member — `0x50 + 0x48 = 0x98`. It is the pointee, not a member.

**Verdict: the previous round is right.** `check_size(gfxRenderState, 0x50)` is correct and should
not be touched.

---

## 13. `gfxViewport` `0x100`-`0x158` — nine names recovered, eight refused

The offsets were already CONFIRMED. Nine of the names are now settled from the code, and the
`Near`/`Far` pair is promoted from PROBABLE to CONFIRMED.

| Offset | Name | How it was established |
|---|---|---|
| `0x120` | `Flags` (dirty mask) | `DoFlush` (`0x004B18D0`) tests bit 0, then bit 1, then stores `0`. Bit 0 leads to `lea edx,[esi+0x15C]` — a `SetViewport` on `m_Viewport`. Bit 1 is set by both `Perspective` and `Ortho` (`or ecx, 2`) and means the projection is stale. |
| `0x124` | `DefaultAspect` = width/height | `SetWindow` does `fild [w]; fidiv [h]; fstp [ecx+0x124]`. |
| `0x128` | projection-mode flags, **bit 0 = orthographic** | `Perspective` does `and al, 0xFE`; `Ortho` does `or al, 1`; `SetWindow` tests bit 0 to decide whether to re-run `Perspective` or merely flush. The constructor initialises it to `2`. MM1's counterpart ends with `b32 Orthographic`. |
| `0x12C` | **`Near`** — CONFIRMED | `Perspective`'s third argument lands here (`mov eax,[ebp+0x10]; mov [esi+0x12C],eax`), and `IsSphereVisible` compares the sphere's z against it. |
| `0x130` | **`Far`** — CONFIRMED | `Perspective`'s fourth argument lands here; the constructor defaults it to `0x447A0000` = `1000.0f`; `IsSphereVisible` does `fld z; fsub radius; fcomp [esi+0x130]`. |
| `0x134` | `TanHalfFovX` = `tan(Fov*pi/360) * Aspect` | computed and stored by `Perspective`. |
| `0x138` | `TanHalfFovY` = `tan(Fov*pi/360)` | computed and stored by `Perspective`. |
| `0x13C`,`0x140` | `Vector2 LeftPlane` | see below |
| `0x144`,`0x148` | `Vector2 RightPlane` | see below |
| `0x14C`,`0x150` | `Vector2 TopPlane` | see below |
| `0x154`,`0x158` | `Vector2 BottomPlane` | see below |

### The frustum is four `Vector2` planes, and MM1 names them

`gfxViewport::IsSphereVisible` (`0x004B19D0`) evaluates exactly four two-term dot products against
the radius, after the near/far test:

```
x * [esi+0x13C] + z * [esi+0x140]   vs radius
x * [esi+0x144] + z * [esi+0x148]   vs radius
y * [esi+0x14C] + z * [esi+0x150]   vs radius
y * [esi+0x154] + z * [esi+0x158]   vs radius
```

The first two multiply **x**, the last two multiply **y**. `Perspective` builds them consistently:
it takes `a = Fov*pi/360` (the vertical half-angle) and `b = atan(tan(a)*Aspect)` (the horizontal
half-angle), and writes `(-+cos b, -sin b)` into the two x-planes and the `cos a`/`sin a` pair into
the two y-planes. Two x-planes from the horizontal half-angle, two y-planes from the vertical one —
the frustum's left/right and top/bottom sides.

MM1's `agiViewParameters` (`Open1560/code/midtown/agi/viewport.h`) declares, in this order:

```cpp
Vector2 LeftPlane;
Vector2 RightPlane;
Vector2 TopPlane;
Vector2 BottomPlane;
```

consumed by `SphereVisible(Vector3& center, f32 radius)` — the same algorithm under the earlier
name. Same codebase family, same declaration order, same four `Vector2`s. That is where these names
come from.

**Caveat, stated plainly:** the *grouping* into four `Vector2` planes is CONFIRMED from MM2's own
code. Which member of each **pair** is Left rather than Right (and Top rather than Bottom) rests on
MM1's declaration order, not on MM2 evidence — and Open1560's header carries its own
`// TODO: Are these in the right order?` against Top/Bottom. Treat the pair assignment as PROBABLE.

### `Fov` at `0x174` is the VERTICAL field of view, in degrees

This is worth stating loudly because `docs/resolution_blueprint.md`'s Hor+ FOV fix turns on it.

`Perspective(fov, aspect, near, far)` computes `t = tan(fov * pi/360)`, stores `t` at `0x138`, and
stores `t * aspect` at `0x134`. `Aspect` is width/height (`SetWindow`, above). So the *vertical*
half-extent at unit depth is `t` and the *horizontal* one is `t * aspect` — meaning `fov` is the
vertical FOV, and it is in degrees (`pi/360` halves it and converts). The constructor's default is
`0x42700000` = `60.0f`, a textbook default vertical FOV, alongside `Far = 1000.0f`.

`gfxViewport::ComputeVFOV` (`0x004B19B0`) exists to feed it: it computes
`2*atan( tan(arg/2) / DefaultAspect )`, i.e. horizontal to vertical. (Note it takes and returns
**radians** — it multiplies by `0.5`, not `pi/360` — so it is not simply a converter for the `Fov`
field's own units.)

The consequence for arbitrary resolution: because the projection is parameterised on a **vertical**
FOV, widening the aspect ratio widens the horizontal field of view automatically. MM2's frustum is
already Hor+ *provided* `Aspect`/`DefaultAspect` is derived from the true width/height — which
`SetWindow` does. The Hor+ work is therefore about making sure the true aspect reaches
`SetWindow`, not about rewriting the projection.

### `0x100`-`0x11C` — deliberately still `field_100` ... `field_11C`

**Nothing in the retail binary reads or writes any of these eight dwords.** None of
`gfxViewport`'s ten methods touches an offset in `[0x100, 0x120)`; and disassembling a 0x140-byte
window around all 73 sites that reference `gfxPipeline::VP`, `OrthoVP` or `m_Viewport` produces only
false positives (a constructor at `0x00520110` that installs a vftable — `gfxViewport` has none —
and a base register returned from an unrelated call at `0x00414E2F`).

There is a tempting hypothesis and it is **not** being written into any file. MM1's
`agiViewParameters` carries exactly eight scalar projection terms — `ProjX, ProjY, ProjZZ, ProjZW,
ProjXZ, ProjYZ, ProjRight, ProjBottom` — immediately before its matrices. MM2 replaced the
hand-rolled projection with a real `Matrix44 m_Projection` at `0x00`, which would leave those eight
floats declared but unconsumed, exactly matching an eight-dword dead hole at `0x100`. The count, the
width and the deadness all line up. It is still a hypothesis: no MM2 instruction confirms a single
one of those names, and the whole point of the discipline in this document is that a plausible name
is not a recovered one. They stay `field_100` ... `field_11C`.

What the GL layer can take from this: the region is inert. It does not need to be preserved
semantically, only positionally.

---

## 14. `gfxRenderStateData::field_2C` — dead, and provably so

The previous round recorded it as "written by nothing in the class's own code". That is now a much
stronger statement, because the class is effectively a singleton with two fixed addresses and both
access routes can be checked exhaustively:

- **By absolute address.** `RSTATE` is at `0x006856A0` and `LASTRSTATE` at `0x006854A0`, so inlined
  accesses to `field_2C` would appear as the constants `0x006856CC` and `0x006854CC`. A scan of every
  absolute dword in `.text` finds **neither, not once** — while finding, for comparison, 92 references
  to `RSTATE+0x20` and 94 to `RSTATE+0x01`.
- **Through `this`.** None of the class's 21 methods contains an access at `+0x2A`, `+0x2B` or
  `+0x2C`. `DoFlush` (`0x004B4C40`, 1885 bytes) is the exhaustive consumer of the structure and its
  accesses run to `+0x25`, then `lea [esi+0x26]`, `lea [esi+0x28]`, then jump straight past to `+0x30`.
- **Through a register base.** Disassembling a 0xC0-byte window around all 131 sites that load the
  `RSTATE`/`LASTRSTATE` base address yields two apparent `+0x2C` hits, both false: `0x0044379E`
  reads an unrelated game object, and `0x004A435B` is copying matrix row 3 into `sm_World`.

So `field_2C` — and the `0x2A`/`0x2B` gap in front of it — are dead storage. **No name can be
recovered because there is nothing to recover it from.** Leave the name as it is; the offsets around
it are unaffected.

(The `DoFlush` sweep incidentally re-confirms `AddressU` at `0x26` and `AddressV` at `0x28` as
two-element arrays — `lea ebx,[esi+0x26]` and `lea ebx,[esi+0x28]` head loops over two texture
stages, with individual `[edi+0x26]`/`[edi+0x27]` and `[edi+0x28]`/`[edi+0x29]` accesses.)

---

## 15. The finding that changes the port order: the draw entry points are dead

An exhaustive scan of the whole image — every `E8`/`E9` relative displacement resolved to its target,
plus every absolute dword that could be a function pointer in a table or vftable — returns **zero
callers** for all of these:

| Function | Address |
|---|---|
| `gfxPipeline::Draw(gfxDrawMode, uint, int, gfxStridedData const&)` | `0x004AAAF0` |
| `gfxPipeline::Draw(gfxDrawMode, uint, int, void*)` | `0x004AAB50` |
| `gfxPipeline::DrawIdx(gfxDrawMode, uint, int, gfxStridedData const&, uint, ushort*)` | `0x004AABB0` |
| `gfxPipeline::DrawIdx(gfxDrawMode, uint, int, void*, uint, ushort*)` | `0x004AAC20` |
| `gfxPipeline::DrawVB` | `0x004ABA50` |
| `gfxPipeline::DrawIdxVB` | `0x004ABB40` |
| `gfxPipeline::CreateVertexBuffer` | `0x004AB830` |
| `gfxPipeline::DestroyVertexBuffer` | `0x004AB950` |
| `gfxPipeline::DoLockVertexBuffer` | `0x004AB970` |
| `gfxPipeline::PersistVertexBuffer` | `0x004ABC50` |
| `gfxPipeline::CreateReadbackImage` | `0x004AAC90` |

They are exported, reachable, and never called. Every draw in retail MM2 goes through
`gfxPacket::Draw` — `0x004B3340` (3 callers) and `0x004B34E0` (2 callers) — which calls the
`lpD3DDev` vtable directly: `DrawIndexedPrimitive` at `+0x68`, `DrawIndexedPrimitiveStrided` at
`+0x78`, `DrawIndexedPrimitiveVB` at `+0x80`.

Two consequences:

1. **`gfxStridedData` was not blocking a hot path.** It was blocking four dead functions. The class
   is worth having recovered — it is the shape the GL layer's strided path must accept, and
   `gfxPacket` builds exactly that shape inline — but the port-order urgency belongs to `gfxPacket`,
   not to `gfxPipeline::Draw`.
2. **The entire vertex-buffer creation path is vestigial.** Nothing in retail ever calls
   `CreateVertexBuffer`, so no `gfxVertexBuffer` is ever constructed; only `RestoreAll` (from
   device-lost handling at `0x004AA1FF`) and `KillAll` (`0x004AA7E0`) are reached, and both walk an
   always-empty list. `gfxPacket::Draw`'s `DrawIndexedPrimitiveVB` branch — taken when
   `StartVertex >= 0` — is therefore unreachable in practice. An OpenGL device layer does **not**
   need a vertex-buffer object to reach parity; it needs `gfxPacket`'s system-memory FVF path.

---

## Appendix B: additions to the mislabel table

| Address | The kit / project map says | It actually is |
|---|---|---|
| `0x00570D70` | `asNetwork::SetPlayerData` (nearest preceding map symbol) | a DirectPlay `GetPlayerData` wrapper — unsymbolised; its `call [ecx+0x74]` is `IDirectPlay4::GetPlayerData`, not `DrawPrimitiveStrided` |
| `0x004B1B80` | unnamed | `gfxMaterial::Reset` applied to `FlatWhite` — a `mov ecx, 0x006856F0` / `jmp 0x004B1BB0` thunk |

And a confirmation rather than a correction: `SetWindow` (`0x004B14C0`) clamps its x/width against
`0x00683128` and its y/height against **`0x00683100`**, which independently re-proves the previous
round's identification of `0x00683100` as `gfxPipeline::m_iHeight`.

---

# Round 3 — `gfxPacket`, the class the real draw path is built on

§15 established that `gfxPipeline::Draw` and the whole vertex-buffer creation path have zero
callers, and that every draw in retail goes through `gfxPacket::Draw`. This round recovers
`gfxPacket` and everything the two `Draw` overloads dereference.

Method is the same as Round 2: `C:\mm2ghidra\bin\midtown2.exe` disassembled directly with
`capstone` 5.0.7 through `tools/pe.py`. **Every offset below was read off instruction bytes.** The
machine-readable half is `data/layouts_gfx_round3.json`; only `gfxReskin` is offered for merge, and
`tools/merge_layouts.py` accepts it (dry run: `merged 1 classes: gfxReskin 0x10 4 members`).

The summary table gains:

| Class | Size | Status after round 3 |
|---|---|---|
| `gfxPacket` | 0x2C | **CONFIRMED** — every offset and the size read off bytes; four names in `layouts.json` are wrong |
| `gfxPacketList` | 0x2C | CONFIRMED — it *is* a `gfxPacket` |
| `gfxReskin` | **0x10** | CONFIRMED — **missing** from `layouts.json`; emitted and merges cleanly |
| `modStatic` | 0x10 | CONFIRMED independently |
| `modShader` | 0x8 | CONFIRMED independently — and it re-proves `gfxMaterial` = 0x48 a fourth way |

---

## 16. `gfxPacket` — 0x2C, CONFIRMED, and four of its names are wrong

### The size

Four separate `push 0x2c` / `call operator new` sites, each immediately followed by a `gfxPacket`
constructor call:

```
004b4128  6a2c   push 0x2c      ; gfxPacket::Clone      -> copy ctor 0x004B4170
00598ba0  6a2c   push 0x2c      ; modModel::LoadAscii   -> 13-arg ctor 0x004B38A0
005997f3  6a2c   push 0x2c      ; modModel::LoadBinary  -> 13-arg ctor
004a5195  6a2c   push 0x2c      ; modGetStatic          -> 13-arg ctor
```

(`modGetStatic` at `0x004A51D4` is a **fourth** construction site. There is no destructor symbol
anywhere: `gfxPacket`s are never freed.)

### The members

| Offset | Type | Name | How it was fixed |
|---|---|---|---|
| `0x00` | `gfxPacket*` | `Next` | `DrawList` (`0x004B4870`) walks `mov esi,[esi]`; the copy ctor `Clone`s `[src+0]` into `[dst+0]`; `modStatic::GetAdjunctCount` walks the same chain |
| `0x04` | `IDirect3DVertexBuffer7*` | `VertexBuffer` | `AllocateVertexBuffer` stores `table[i].vb` here; `DoLock`/`DoUnlock` call its vtable `+0x0C`/`+0x10`; `Draw` passes it as `DrawIndexedPrimitiveVB`'s `lpd3dVertexBuffer` |
| `0x08` | `void*` | **`LockedVerts`** | see §16.1 — `layouts.json` says `i32* pPositions`, which is wrong |
| `0x0C` | `void*` | `Vertices` | the system-memory vertex block; `gfxFVFSize(FVF) * AdjunctCount` bytes in the ctor, copy ctor and `CopyFrom`; passed as `DrawIndexedPrimitive`'s `lpvVertices` |
| `0x10` | `u16` | `VertexTypeDesc` (FVF) | `mov word [ebx+0x10], ax` from ctor arg 1; every method feeds it to `gfxFVFSize`/`gfxFVFOffset`; passed as `dwVertexTypeDesc` |
| `0x12` | `i16` | `StartVertex` | **signed**: `cmp word [esi+0x12], 0 / jl` in `Draw`, `GetPosition`, the copy ctor and `CopyFrom`; `movsx` before it is passed as `dwStartVertex`; set to `0xFFFF` on the system-memory path |
| `0x14` | `u16` | **`AdjunctCount`** | `layouts.json` says `PacketSize`, which is wrong — see §16.1 |
| `0x16` | `u16` | **`IndexCount`** | `layouts.json` says `TriCount`, which is wrong — see §16.1. This is the important one |
| `0x18` | `u16*` | `Indices` | `2 * [0x16]` bytes allocated and copied alongside; `GetTri` (`0x004B3EC0`) reads `word[[0x18] + i*6 + 0/2/4]` |
| `0x1C` | `u8*` | `MatrixIndices` *(shape CONFIRMED, name PROBABLE)* | counted byte array: ctor allocates `arg10+1`, writes `arg10` at `[0]`, copies `arg11` into `[1..arg10]` |
| `0x20` | `u8*` | `AdjunctMatrixGroups` *(shape CONFIRMED, name PROBABLE)* | `AdjunctCount` bytes, one per adjunct; ctor copies `arg12` in |
| `0x24` | `u8` | **`ReskinCount`** | both `modModel` loaders do `mov byte [eax+0x24], bl` with the reskin record count right after construction; `Draw(Matrix44*,uint)` uses it as the loop bound |
| `0x25`–`0x27` | — | padding | never read or written by anything |
| `0x28` | `gfxReskin*` | **`Reskin`** | both loaders store here the same pointer they pass as the ctor's 8th argument, whose type the linker map spells `class gfxReskin *` |

`0x25`–`0x27` is natural alignment padding, so `code/midtown2/gfx/gfxpacket.h` reproduces the object
correctly today without stating it.

### 16.1 The four wrong names, with the evidence

**`0x08` is not a positions array.** `gfxPacket::DoLock` (`0x004B4720`) is eight instructions:

```
004b4720  8b4104   mov eax, [ecx+4]        ; the IDirect3DVertexBuffer7*
004b4723  83c108   add ecx, 8              ; ecx = this + 8
004b4726  6a00     push 0                  ; lpdwSize
004b4728  51       push ecx                ; ppbData  <-- this+8 is the OUT parameter
004b4729  8b10     mov edx, [eax]
004b472b  6a00     push 0                  ; dwFlags
004b472d  50       push eax
004b472e  ff520c   call [edx+0xc]          ; IDirect3DVertexBuffer7::Lock
```

and `DoUnlock` (`0x004B4740`) calls `Unlock` and then `mov dword [esi+8], 0`. `0x08` holds the
**mapped base of the shared vertex buffer**, valid only between a `DoLock` and its `DoUnlock` and
null at every other time. `GetPosition`/`GetNormal`/`SetPosition` read it only on the
`StartVertex >= 0` branch, adding `gfxFVFSize(FVF) * (StartVertex + i)`. The first port that
dereferences it as `i32* pPositions` reads the wrong thing.

**`0x14` is a vertex count, not a byte size.** The ctor stores arg 3 there and *separately* computes
`arg3 * gfxFVFSize(FVF)` as the allocation size. `Draw` passes it as `dwVertexCount` /
`dwNumVertices`. The 1999 name is fixed by the linker map: `modStatic::GetAdjunctCount`
(`0x004A4DB0`) and `modModel::GetAdjunctCount` do nothing but sum `word[packet+0x14]` over the
`Next` chain. `MM2::gfxPacket` in `layouts.json` already says `AdjunctCount`; the bare `gfxPacket`
entry does not.

**`0x16` is an index count, not a triangle count — and both sources are wrong the same way, so
their agreement is not corroboration.** Four independent proofs:

1. *Argument position.* `gfxPacket::Draw(uint)` reads it at `0x004B346C` and pushes it at
   `0x004B3475` as the `dwIndexCount` argument of `IDirect3DDevice7::DrawIndexedPrimitive` (the
   call is at `0x004B3491`). The DX7 signature fixes the position, and the six arguments around it
   are independently identified.
2. *Allocation.* Both the ctor (`0x004B3AC2`) and the copy ctor (`0x004B420F`) do `shl eax, 1` on
   it and allocate that many bytes for a `u16` array.
3. *The caller.* `modModel::LoadAscii` computes `lea ecx, [edi + edi*2]` — three times the triangle
   count — and passes *that* as the argument the ctor stores at `0x16` (`0x00598C01`).
4. *`modStatic::GetTriCount`* (`0x004A4DE0`) reads `word[packet+0x16]` and **divides it by three**
   before summing. The `lea` chain at `0x004A4E00`–`0x004A4E14` evaluates to `21846 * n >> 16`, and
   `21846 / 65536 = 0.33333` — MSVC's magic-number division by 3.

A triangle count divided by three is not a triangle count. Sizing an index buffer as `3 * [0x16]`
over-allocates threefold; looping `[0x16]` triangles reads three times past the end.

**`0x28` is a `gfxReskin*`, not a `u32`.** The width is right, the type is not. See §17.

`data/layouts.json` already has `gfxPacket` at 0x2C with all thirteen offsets correct, so
`merge_layouts.py` rule 1 will (rightly) refuse a corrected entry. These four are recorded under
`pending_hand_edits` in `data/layouts_gfx_round3.json` and need a deliberate human edit.
**None of them moves an offset or changes a size**, so no `check_size` can break.

### 16.2 The thirteen-argument constructor, mapped

The linker map's signature is

```
gfxPacket(unsigned int, unsigned int, unsigned int, unsigned char const*, unsigned int,
          unsigned short const*, unsigned int, class gfxReskin*, unsigned int, unsigned char,
          unsigned char const*, unsigned char const*, bool)
```

`ret 0x34` = 52 bytes = 13 dwords, so `argN` is at `[ebp + 4 + 4N]`. Mapped against the body
(`0x004B38A0`) and against all four call sites:

| # | Slot | Meaning | Read by the ctor? |
|---|---|---|---|
| 1 | `+0x08` | FVF → `[this+0x10]` | yes |
| 2 | `+0x0C` | — (`LoadAscii` passes the constant `4`) | **never** |
| 3 | `+0x10` | adjunct count → `[this+0x14]` | yes |
| 4 | `+0x14` | vertex bytes → copied into `[this+0x0C]` | yes |
| 5 | `+0x18` | index count → `[this+0x16]` | yes |
| 6 | `+0x1C` | `u16*` indices → copied into `[this+0x18]` | yes |
| 7 | `+0x20` | reskin count (the caller writes `[this+0x24]` itself) | **never** |
| 8 | `+0x24` | `gfxReskin*` (the caller writes `[this+0x28]` itself) | **never** |
| 9 | `+0x28` | — | **never** |
| 10 | `+0x2C` | matrix-index count → `[this+0x1C][0]`; **also the system-memory gate** | yes |
| 11 | `+0x30` | `u8*` matrix indices → `[this+0x1C][1..n]` | yes |
| 12 | `+0x34` | `u8*` per-adjunct groups → `[this+0x20]` | yes |
| 13 | `+0x38` | `bool` | **never** |

Argument 8's declared type — `class gfxReskin *` — is what names `gfxReskin`, and the mapping is
self-checking: it is the sixth push at every call site, and at `modModel::LoadAscii` `0x00598BF9`
the value pushed there is exactly the pointer the caller then stores at `packet+0x28`.

**Argument 10 does double duty**, which is worth stating because it looks like a bool at first
glance. `0x004B38E1` reads it as `test al, al` and skips `AllocateVertexBuffer` when non-zero;
`0x004B3B56` reads the *same slot* as `and esi, 0xff` / `lea edx, [esi+1]` and uses it as a count.
A packet with matrix indices is a skinned packet, and a skinned packet must be system memory. There
is no separate flag: **the gate is "the matrix-index count is zero", not argument 13.**

### 16.3 The copy constructor does not copy the skin

`gfxPacket::gfxPacket(const gfxPacket*)` at `0x004B4170` writes `0x00, 0x08, 0x0C, 0x10, 0x12,
0x14, 0x16, 0x18, 0x1C, 0x20` and `CopyFrom` (`0x004B4260`) writes `0x04, 0x0C, 0x10, 0x12, 0x14,
0x16, 0x18`. **Neither touches `0x24` or `0x28`**, so a cloned packet inherits whatever
`operator new(0x2c)` last left in those four bytes. It also *shallow-copies* `0x1C` and `0x20` —
the clone aliases the original's matrix arrays rather than owning them.

Both are 1999 defects, not misreadings, and both matter to the port: a C++ reimplementation with a
defaulted or hand-written copy constructor will behave *differently* — arguably better — than the
original. Record the divergence deliberately rather than reproducing it by accident. (In practice
`Clone` is reached only from `modStatic::Clone` and from its own `Next`-chain recursion, so nothing
observed here proves a skinned packet is ever cloned.)

---

## 17. `gfxReskin` — 0x10, CONFIRMED, and missing from `layouts.json`

MM2's per-vertex skin-blend record. This is the one class this round offers for merge, and it
merges cleanly.

| Offset | Type | Name | Evidence |
|---|---|---|---|
| `0x00` | `u8` | `AdjunctIndex` | `Draw(Matrix44*,uint)` at `0x004B366B` reads `byte [rec+0]`, does `lea edx,[edx+edx*2]` and indexes the 12-byte-stride transformed-position scratch at `0x0068BCE8` — it overwrites one already-transformed adjunct |
| `0x01` | `u8` | `MatrixGroup` | `0x004B3643` reads `byte [rec+1]` and uses it as `g` in `MatrixIndices[g+1]`, which then indexes the caller's `Matrix44` palette (`shl ecx, 6`) |
| `0x02` | `u16` | `Weight` | written at load as `__ftol(GetFloat() * 32767.0f)` (the constant at `0x005B873C`, verified `= 32767.0`) and read at draw as `fild` + `fmul [0x005B2A24]` (verified `= 3.0518509e-05` = 1/32767). Zero-extended on read, so **unsigned** |
| `0x04` | `Vector3` | `Position` | the load loop passes `&rec[4]` to the tokenizer's `Vector3` reader (vtable `+0x0C`); `Draw` multiplies `[rec+4]`, `[rec+8]`, `[rec+0xC]` against matrix columns 0/1/2 exactly as it does a vertex position |

`0 + 1 + 1 + 2 + 12 = 16`. The members tile the struct exactly.

**Size, three ways.** `shl reg, 4` immediately before `operator new` at `0x00598AA2`
(`modModel::LoadAscii`) and `0x00599729` (`modModel::LoadBinary`); `add ebx, 0x10` per record in
each of those two fill loops; and `add eax, 0x10` per record in the consumer loop of
`gfxPacket::Draw(const Matrix44*, uint)` at `0x004B371E`.

**Field order, independently.** `modModel::SaveToAscii` at `0x0059A930`–`0x0059A989` reads back
`byte [edi-4]`, `byte [edi-3]`, `word [edi-2]` (zero-extended) and `float [edi]`, `[edi+4]`,
`[edi+8]` and hands all six to `fprintf` with the format at `0x005DDE54`:

```
\treskin %5d %3d %6.2f\t%f\t%f\t%f\n
```

Six conversions for two bytes, one word and three floats, in that order. The reader keyword is
`"reskin"` at `0x005DDAB4`/`0x005DDC30`. (The `%6.2f` is fed the raw `u16` as an *integer* — a
genuine format/argument mismatch in the 1999 ASCII exporter. It does not affect the layout, and it
is recorded here only so the next reader does not conclude the field is a float.)

**No symbol in the binary is scoped to `gfxReskin`**, so `tools/genheaders.py` will not emit a
`gfxreskin.h`. Like `gfxInterface` and `gfxResData`, it must be hand-written — merging it still
matters, because it is what that hand-written header has to agree with.

The skinning arithmetic itself, for the GL layer: for each record, take the caller's
`Matrix44 palette[MatrixIndices[rec.MatrixGroup + 1]]`, transform `rec.Position` by it, and **lerp**
the already-transformed position of adjunct `rec.AdjunctIndex` toward that result by
`rec.Weight / 32767`. It is a fixed-function CPU skin that rewrites entries in the shared scratch
buffer, not a second vertex stream.

---

## 18. What the two `Draw` overloads actually do

Both are `const` methods on a packet and neither touches a `gfxTexture` — texture and material
binding happen earlier, in `modStatic::Draw` and `gfxRenderState::DoFlush`. There is no gap in
`gfxTexture` for this round to close.

### `gfxPacket::Draw(uint colour)` — `0x004B3340`, four branches

| Branch | Condition | D3D call |
|---|---|---|
| VB | `StartVertex >= 0` | `DrawIndexedPrimitiveVB(D3DPT_TRIANGLELIST, VertexBuffer, StartVertex, AdjunctCount, Indices, IndexCount, 0)` at vtable `+0x80` |
| internal | `sm_UseInternal` (`0x00684500`) | the internal rasteriser at `0x004B73C0`, `__cdecl(4, AdjunctCount, FVF, Vertices, IndexCount, Indices)` |
| strided | `colour != -1` **and** the FVF lacks `D3DFVF_DIFFUSE` | `DrawIndexedPrimitiveStrided(…, FVF|0x40, &sd, …)` at `+0x78` |
| plain | otherwise | `DrawIndexedPrimitive(…, FVF, Vertices, AdjunctCount, Indices, IndexCount, 0)` at `+0x68` |

The strided branch builds a `gfxStridedData` at `ebp-0x30` in a frame that is exactly `sub esp,
0x30` — this is the frame §10 used, re-read here byte for byte, so `sizeof(gfxStridedData) == 0x30`
is re-confirmed. Its purpose is narrow and worth knowing: **it exists only to paint a per-draw
constant colour onto vertices whose FVF has no diffuse component**, by pointing `diffuse.lpvData`
at the address of the `colour` argument with `diffuse.dwStride = 0`.

### `gfxPacket::Draw(const Matrix44* palette, uint colour)` — `0x004B34E0`

Returns immediately if `sm_UseInternal`. Otherwise it is the **CPU skinning path**, and it always
ends in one call — `DrawIndexedPrimitiveStrided` at `+0x78`. It never uses `VertexBuffer`,
`LockedVerts` or `StartVertex`.

1. Reads `RSTATE.State.Lighting` (`0x006856A1`, i.e. `gfxRenderStateData + 0x01`). If lighting is
   on it transforms **positions and normals**; if off, positions only.
2. For each adjunct `i`: `g = AdjunctMatrixGroups[i]`, `m = palette[MatrixIndices[g+1]]`, transform
   `Vertices[i]`'s position (and normal) by `m`, write to the scratch buffers.
3. For each `gfxReskin` record: lerp as §17 describes.
4. Fill a `0x30` `gfxStridedData` at `ebp-0x48` and draw.

The strided block it builds — the second independent `0x30` frame from §10:

| Offset | Written with |
|---|---|
| `0x00` | `0x0068BCE8` — the transformed-position scratch |
| `0x04` | `0x0C` |
| `0x08` | `0x00685CE8` — the transformed-normal scratch |
| `0x0C` | `0x0C` |
| `0x10` | `Vertices + gfxFVFOffset(FVF, D3DFVF_DIFFUSE)` if the FVF has one, else `&colour` |
| `0x14` | `gfxFVFSize(FVF)` in the first case, `0` in the second |
| `0x18` | the address of a zeroed local at `ebp-0x18` |
| `0x1C` | `0` |
| `0x20` | `Vertices + gfxFVFOffset(FVF, D3DFVF_TEX1)` |
| `0x24` | `gfxFVFSize(FVF)` |

`ebp-0x18` is `base + 0x30`, which is what bounds the struct above.

### The `IDirect3DDevice7` vtable offsets this port has to honour

Five call sites, all mutually consistent, pin the slots the real draw path uses. Recorded because an
OpenGL device layer that replaces `lpD3DDev` has to answer at exactly these offsets for as long as
any of the surrounding code is still assembly:

| Offset | Method | Seen at |
|---|---|---|
| `+0x40` | `SetMaterial` | `modStatic::Draw` `0x004A45D2` |
| `+0x64` | `DrawPrimitive` | `gfxPipeline::DrawVB` (§11) |
| `+0x68` | `DrawIndexedPrimitive` | `gfxPacket::Draw(uint)` `0x004B3491`, `OrthoMap` `0x004B4850` |
| `+0x74` | `DrawPrimitiveStrided` | `gfxPipeline::Draw` (§10) |
| `+0x78` | `DrawIndexedPrimitiveStrided` | both `gfxPacket::Draw` overloads |
| `+0x7C` | `DrawPrimitiveVB` | `gfxPipeline::DrawVB` (§11) |
| `+0x80` | `DrawIndexedPrimitiveVB` | `gfxPacket::Draw(uint)` `0x004B337A` |

The DX7 header order that makes these consistent includes **`GetClipStatus`** between
`SetClipStatus` and `DrawPrimitiveStrided`; omitting it shifts everything after
`DrawIndexedPrimitive` by four and makes `+0x80` look like `ComputeSphereVisibility`. It is not —
the call at `0x004B337A` pushes seven arguments plus `this`, which is `DrawIndexedPrimitiveVB`'s
arity, not `ComputeSphereVisibility`'s.

`IDirect3DVertexBuffer7` is used at `+0x08` `Release`, `+0x0C` `Lock`, `+0x10` `Unlock`, and
nowhere else.

---

## 19. The shared vertex-buffer pool — described, not named

`gfxPacket::AllocateVertexBuffer` (`0x004B43E0`) and `ReserveVertexBuffer` (`0x004B4570`) share a
suballocator that is **not** `gfxVertexBuffer` and has no type symbol. Its 1999 name is `table`,
from the preserved source line at `0x005CD6D4`:

```
lpD3D->CreateVertexBuffer(&desc,&table[i].vb,0)
```

Entry stride is `0x10` (`add ecx, 0x10` in both search loops), and the array runs from `0x006857E0`
to the next object at `0x006858E0`, so it holds **at most 16 entries**.

| Offset | Meaning |
|---|---|
| `+0x00` | `u32` FVF — the search key (`cmp [ecx], edx` against `word[packet+0x10]`) |
| `+0x04` | `IDirect3DVertexBuffer7* vb` — the out-parameter of `CreateVertexBuffer` |
| `+0x08` | `u32` used vertices — a packet's `StartVertex` is this value *before* the bump, and it is bumped by `AdjunctCount` |
| `+0x0C` | `u32` capacity in vertices — `max(4096, AdjunctCount)`; free space is `[+0x0C] - [+0x08]` |

Because capacity defaults to `0x1000`, `StartVertex` stays inside `i16`'s positive range, which is
why `0x12` can be a signed 16-bit field at all.

The name is **deliberately not invented**: no symbol in the binary scopes a type here, so it is
recorded under `static_data_recovered` rather than under `layouts`.

### The other globals this path reads

| RVA | What it is |
|---|---|
| `0x006830AC` | `IDirect3D7* lpD3D` (map) |
| `0x006830C8` | `IDirect3DDevice7* lpD3DDev` (map) — every draw goes through this pointer |
| `0x00684500` | `bool sm_UseInternal` (map) |
| `0x00684501` | `bool useHWTnL` (map) — also gates whether a system-memory backup is kept |
| `0x006844F8` | `bool useNativeVBs` (map) — checked *after* the argument-10 gate |
| `0x006857D8` | `bool gfxForceLVERTEX` (map) |
| `0x006857E0` | `table[16]`, above |
| `0x006858E0` | the system-memory backup pointer for each created buffer, indexed by `vbtableCount` |
| `0x00685CE0` | `gfxPacket::sm_VertScale` (map) |
| `0x00685CE4` | `int vbtableCount` (map) — indexes `0x006858E0` and `0x006A1CF0`, **not** `table` |
| `0x00685CE8` | `f32[2048][3]` transformed-**normal** scratch, `0x6000` bytes |
| `0x0068BCE8` | `f32[2048][3]` transformed-**position** scratch, `0x6000` bytes, immediately after |
| `0x00691CE8` | `int` — live entries in `table`. Unsymbolised in both sources; distinct from `vbtableCount` |
| `0x00691CF0` | `0x20`-stride LVERTEX scratch used by `OrthoMap`; `0x006A1CF0 - 0x00691CF0 = 0x10000` = **2048 entries** |
| `0x006A1CF0` | `IDirect3DVertexBuffer7** vblist` (map) — every buffer ever created, drained by `FreeAllVertexBuffers` |
| `0x006A3C34` | `int datCurrentMemoryAlign` (map) — set to 4 around every `operator new` in this subsystem and restored after |

**Two 2048-adjunct scratch buffers are the hard ceiling on a single skinned packet**, and nothing
in `Draw` bounds-checks against it.

One consequence for the GL layer that is easy to miss: on a VB-backed packet, `Vertices` (`0x0C`)
does **not** point into the D3D buffer. `gfxPacket::Persist` (`0x004B45D0`) returns
`backups[i] + offset`, so `Vertices` points into the system-memory shadow at `0x006858E0[i]`.
`RestoreAllVertexBuffers` (`0x004B4690`) copies that shadow back after a device loss. The shadow
exists only when `useHWTnL` is set; otherwise `Persist` returns `NULL` and `Vertices` is null on
that path.

`gfxPacket::OrthoMap` (`0x004B4760`) is the one method that reads `Vertices` with a **hardcoded**
`0x20` stride instead of calling `gfxFVFSize` — it assumes the packet's own FVF is already
`D3DFVF_LVERTEX` (`0x1E2`), which is what the `gfxForceLVERTEX` branch of the constructor produces.
Anything ported around it must preserve that assumption or make it explicit.

---

## 20. `modStatic` = 0x10 and `modShader` = 0x8, both CONFIRMED

`layouts.json` is already right about both. Re-derived independently:

**`modStatic`** — `modStatic::Clone` (`0x004A4CA0`) does `push 0x10; call operator new`, then
allocates `PacketCount * 4` twice into `[+0x08]` and `[+0x0C]` and copies `byte [+0x00]`,
`byte [+0x01]`, `word [+0x02]`, `dword [+0x04]` — every recorded member and nothing else.

**`modShader`** — `modShader::LoadShaderSet` (`0x004A3F60`) allocates `lea ecx, [eax*8]` for the
array, and `modStatic::Draw` indexes it with `lea esi, [esi + edx*8]`. Two members, both pinned by
what they are compared against:

| Offset | Type | Proof |
|---|---|---|
| `0x00` | `gfxTexture*` | compared against `RSTATE + 0x44` (`0x006856E4`), which `layouts.json` names `gfxRenderStateData::m_Texture`; a mismatch sets `m_Touched |= 2` |
| `0x04` | `gfxMaterial*` | compared against `LASTRSTATE + 0x4C` (`0x006854EC`) = `m_Material`, written to both `RSTATE + 0x4C` and `LASTRSTATE + 0x4C`, and passed to `IDirect3DDevice7::SetMaterial` (`+0x40`) |

`modStatic::Draw` (`0x004A4550`) is the real draw path in miniature and worth reading once in full:
flush `RSTATE` if `m_Touched & m_TouchedMask`, then per packet-list — set material, set texture,
re-flush, decide alpha blending from the material's diffuse alpha (`fld [material+0x0C]`), and
finally `gfxPacket::DrawList`. That `+0x0C` read is itself a small independent confirmation of
`gfxMaterial`'s `Vector4 Diffuse` at offset `0`.

### A fourth, two-sided proof that `gfxMaterial` is 0x48

§7 and §12 both concluded `0x48` against `layouts.json`'s `0x44`. Here is the cleanest proof yet,
from a class nobody was looking at:

```
004a3b30  55       push ebp
004a3b31  8bec     mov ebp, esp
004a3b33  83ec48   sub esp, 0x48          ; the whole frame
...
004a3b75  8d4db8   lea ecx, [ebp - 0x48]  ; a gfxMaterial at the frame's base
004a3b78  e833e000 call 0x4b1bb0          ; gfxMaterial::Reset
```

`modShader::Load` has **exactly one stack local**, a `gfxMaterial`, and the frame is exactly `0x48`.
Every `[ebp-…]` reference in the function is one of `-0x48`, `-0x38`, `-0x28`, `-0x18` — the four
`Vector4` colour members, read straight off the stream in `0x10`-byte chunks. That bounds
`sizeof(gfxMaterial) <= 0x48` from above; `Reset`'s `mov dword [ecx+0x44], -1` bounds it at
`>= 0x48` from below. **It is exactly `0x48`.**

`data/layouts.json` still says `0x44`. The hand edit recorded in §7 and §12 is still required, and
this is now the fourth independent line of evidence for it. `modShader::AddStaticMaterial`
(`0x004A3930`) strides a `gfxMaterial` pool, so a `0x44` stride there is live corruption, not a
cosmetic error.

---

## 21. `gfxPacketList` is a `gfxPacket`

`gfxPacket::MakeList` (`0x004B4860`) is `push ebp; mov ebp,esp; mov eax,[ebp+8]; pop ebp; ret` — it
returns its `gfxPacket*` argument unchanged as a `gfxPacketList*`. `DrawList` (`0x004B4870`) then
walks `[obj+0]` calling `gfxPacket::Draw(uint)` with `-1`. The two byte-identical `layouts.json`
entries are correct, not a duplication bug.

The whole live call graph, for the port order:

```
modStatic::Draw / DrawEnvMapped / DrawWithTexGenAndTexMatrix
    -> gfxPacket::DrawList -> gfxPacket::Draw(uint)
modStatic::DrawOrthoMapped
    -> gfxPacket::OrthoMap
modModel::Draw / modModel::DrawPlain
    -> gfxPacket::Draw(const Matrix44*, uint)   (skinned)
    -> gfxPacket::Draw(uint)                    (unskinned)
```

Those five functions are the entire retail draw surface above `lpD3DDev`.

---

## 22. What this round did not settle

Stated plainly, because a gap is worth more than a guess:

- **`0x1C` and `0x20`'s names.** The *shapes* are certain — a counted byte array whose `[0]` is the
  count, and an `AdjunctCount`-long array of group indices. The names are not recovered.
  `MatrixIndices` and `AdjunctMatrixGroups` are descriptions. The model file spells the
  corresponding token `"mtx"` (`0x005DDAC0`), which is a file-format keyword, not a member name.
- **Constructor arguments 2, 7, 8, 9 and 13 are never read by the constructor body.** Arguments 7
  and 8 are explained — the callers write `[this+0x24]`/`[this+0x28]` themselves — but 2, 9 and 13
  have no recovered purpose. `LoadAscii` passes the constant `4` for argument 2 and `0` for
  argument 9; `modGetStatic` passes `1` for argument 13. Nothing reads them.
- **The `table` entry struct at `0x006857E0` has no name.** Its four fields and `0x10` stride are
  CONFIRMED; the type name is not in the binary, so it is described in `static_data_recovered`
  rather than emitted as a layout.
- **`gfxPacket` has no destructor**, in the binary or in the map. Nothing frees a packet, its vertex
  block, its index array, its two byte arrays, or its reskin array. That is the original behaviour,
  not a gap in the recovery — but a port that adds a destructor changes observable lifetime.
