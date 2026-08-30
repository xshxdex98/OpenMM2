# Replacing DirectDraw and Direct3D 7 with OpenGL

The goal is that OpenMM2 renders through OpenGL natively, at up to 2560x1440, with dgVoodoo
uninstalled. This is the plan for getting there and the state of the code that already exists.

Read `docs/resolution_blueprint.md` first — it is the same problem solved once already, on the same
engine family, in Open1560. This document is what changes when that solution meets MM2.

## The one decision everything else follows from

> Resolution stops being a display mode and becomes a **render-target size**, blitted to the window.

dgVoodoo exists to translate DirectDraw mode setting and D3D7 rasterisation into something a modern
driver accepts. A device layer that never sets a mode and rasterises through GL has nothing left for
it to translate. So the removal of dgVoodoo is not a separate task — it is a consequence of doing
this one correctly.

Concretely: `IDirectDraw7::SetDisplayMode` and `IDirectDraw7::EnumDisplayModes` are deleted, not
reimplemented. The resolution list is **synthesised from the desktop bounds**, every entry in it is
achievable by construction because a framebuffer object accepts any size, and fullscreen becomes a
borderless window rather than a mode change.

## What MM2 does that Open1560 did not have to handle

Seven differences, in rough order of how much work each one adds.

### 1. MM2 talks D3D7 almost directly; MM1 had an abstraction to swap

Midtown Madness 1's `agi` layer is a virtual `agiPipeline` with a rasteriser interface behind it, so
Open1560 could add `agiGLPipeline` as one more subclass and delete the software one. MM2 has no such
seam. `gfxPipeline::Draw` **is** `lpD3DDev->DrawPrimitiveStrided`, `gfxRenderState::DoFlush` **is** a
loop of `SetRenderState`/`SetTextureStageState`, and the vertex format is a raw D3D FVF code
(`0x1C4` = `D3DFVF_XYZRHW|DIFFUSE|SPECULAR|TEX1`) passed through from the caller.

There is nothing to subclass. The port is done by reimplementing the `gfxPipeline` statics one at a
time, exactly as the harness intends, and the GL device code they call lives in `code/midtown2/agigl/`.

**Consequence for effort:** the 3D draw path is a larger job here than in MM1, because it has to
consume D3D FVF-encoded interleaved vertex streams rather than a typed `agiVtxType`. That is the
single biggest piece of extra work in this port and it is not on the critical path to a first frame.

### 2. `gfxPipeline` is a namespace of statics, so there is no object to hold device state

Confirmed independently in `docs/gfx_layouts.md`: all 55 `gfxPipeline` functions mangle as `@@SA…`,
`@@CA…` or `@@CG…`, never `@@QAE…`. `sizeof(gfxPipeline) == 1` and there is no constructor, no
vftable and no `this` anywhere.

So the device state — the GL context, the framebuffer, the screen geometry — lives in file-scope
objects under `agigl/`, and `agiGL` is a global rather than a member. `docs/gfx_layouts.md` also
lists the eighteen `.data` addresses the still-assembly code reads directly (`m_iWidth` at
`0x00683128`, `m_iHeight` at `0x00683100`, and so on); those are the real contract, not a struct
layout.

### 3. MM2's window is also its input window

`gfxPipeline::gfxWindowProc` forwards nearly everything to `InputWindowProc`, which is the entry to
the whole DirectInput path — `ioInput`, `ioMouse`, `ioKeyboard`, `ioJoystick`. Open1560 replaced
MM1's window with an SDL one and took the GL context from `SDL_GL_CreateContext`. Doing that here
would mean porting input at the same time as the renderer, and neither half would be independently
testable.

**So MM2 keeps its HWND and gets its context through WGL.** `gfxWindowCreate` stays as it is.

### 4. No SDL, no GLAD, and the build cannot reach the Windows SDK headers

`tools/build.py` compiles with `-I code/midtown2`, the MSVC include directory, and the SDK's
**ucrt** directory only. `um/` and `shared/` — where `windows.h` and `GL/gl.h` live — are not on the
path. `tools/link.py` links `-NODEFAULTLIB` against import libraries generated from midtown2.exe's
own import table, and midtown2.exe imports neither `opengl32.dll` nor the GDI pixel-format entry
points.

`code/midtown2/agigl/glcommon.h` therefore declares the GL vocabulary itself, and everything GL and
WGL is resolved at runtime through `LoadLibraryA`/`GetProcAddress` — the same way the original
resolves `DirectDrawCreateEx` out of `DDRAW.DLL`. **Nothing in the build has to change for agigl to
link.** The two Win32 calls it does link, `GetDC` and `ReleaseDC`, are already in
`build/implibs/USER32.def`.

The same constraint rules out the modern CRT. `<cstdio>`'s `sscanf` lowers to
`__stdio_common_vsscanf`, which the 1999 CRT does not provide; the GL version string is parsed by
hand in `glcontext.cpp` for that reason, and the resolution list is sorted with a hand-written
insertion sort rather than `std::sort`.

### 5. The `IDirectDrawSurface7*` slots are inside pinned class layouts

`gfxBitmap::Surface` at offset `0x008` and `gfxTexture::m_Surface` at `0x014` are read by machine
code that has not been ported. The field cannot move, cannot change size, and cannot hold a bare
`GLuint`: a texture name of 3 in a pointer slot passes every `if (surface)` test in the binary and
then dereferences address 3.

`agigl/glsurface.h` puts an `agiGLSurface` pointer there instead — a real allocation with a magic
number in its first word. Null checks keep working; a stray dereference reads our memory and is
recognisable in a dump. A stray **call** through it still crashes, deliberately: faking a 49-entry
`IDirectDrawSurface7` vtable from memory risks one wrong slot silently calling the wrong function,
and a crash at a known address is a better outcome. The functions that must be ported in the same
wave are listed below.

### 6. Three renderers, not one

`gfxPipeline::SafeBeginGfx` is a four-step retry ladder that ends by setting
`useInterface = 0; useSoftware = 1; useAgeSoftware = 1; useBlade = 1`. Two of those are software
renderers:

- **BLADE.DLL** — an external software DirectDraw/D3D implementation, loaded in place of
  `DDRAW.DLL`. This is what the "Software" option in the graphics menu selects, via
  `gfxResetAdapter`.
- **`AgeDevice`** — MM2's own hand-written `IDirect3DDevice7` (50 functions in
  `MM2_PSEUDOCODE/AgeDevice/AgeDevice.c`) that locks the render surface and rasterises into raw
  16-bit pixels. It is **unreachable**: `BeginGfx3D` tests `useBlade` first, and `SafeBeginGfx` sets
  both, so the `useAgeSoftware` branch is dead code unless something patches `useBlade` to 0.

Both go. Open1560 deleted its software rasteriser too, and Phase 2 of that project went further and
removed the renderer from the enumeration entirely so it cannot be selected.

One useful correction to `docs/resolution_blueprint.md` while here: it names `sdlPage16` as the
"equivalent suspect" for a fixed-size scratch buffer. It is not. `sdl*` in MM2 is scene-description
city geometry and its collision bounds — `sdlPage16::Draw` emits through `vglBegin`/`vglEnd` and
never touches a locked surface. There is no static maximum-resolution buffer to grow, exactly as in
MM1.

### 7. 2D and 3D share one surface, in an order the frame loop enforces

`asCullManager::Update` runs:

```
BeginFrame  Clear  [2D background]  BeginScene  [3D]  EndScene  [2D foreground]  EndFrame
```

The 2D passes are DirectDraw `BltFast` calls and the 3D pass is D3D, into the same pixels of
`lpdsRend`. There is no equivalent split in GL and none is needed: a `BltFast` becomes a textured
quad with depth testing off, into the same framebuffer, and the ordering falls out of the draw order
that is already there.

The one thing that does not survive is the colour key. `DDBLTFAST_SRCCOLORKEY` keys on black, which
is why `gfxBitmap::Load8888` bumps opaque black to `(1,1,1)` — a hack that exists only because the
key is a colour rather than an alpha. The GL upload writes a real alpha channel and the hack goes.

## The entry-point inventory

Every DirectDraw or D3D7 entry point in the graphics layer, what replaces it, and whether it is
needed to reach a rendered frame. Addresses are from `data/symbols.json`; behaviour from
`MM2_RE_KIT/MM2_PSEUDOCODE/`.

### Wave 0 — adapter detection and configuration. Needed before anything.

| Function | Address | What it does now | Replacement |
|---|---|---|---|
| `gfxAutoDetect` | `0x004ABE00` | loads DDRAW.DLL, enumerates adapters twice, compares GUIDs against `gfxconf.dat`, picks by video memory | Call `agiGLSynthesizeModes()`. No DLL, no GUIDs. |
| `AutoDetectCallback` | `0x004AC030` | fills `gfxInterfaces[n]`, enumerates D3D devices and display modes, sets the 1600x1200 cap | **Deleted.** Its whole output is what `agiGLSynthesizeModes` now writes. |
| `ResCallback` | `0x004AC6F0` | the `EnumDisplayModes` callback; appends to `Resolutions[]` **with no bound check** | Deleted. The synthesiser clamps. |
| `DeviceCallback` | `0x004AC3D0` | D3D device caps gates, `badvideo.txt` blacklist | Deleted. Every cap it tests is guaranteed. |
| `gfxLoadVideoDatabse` | `0x004AC540` | parses `badvideo.txt` | Deleted with `DeviceCallback`. |
| `gfxPipeline::EnumDDAdapters` | `0x004A9640` | `DirectDrawEnumerateExA` → `MultiMonCallback` → `DDEnumProc` | Deleted. |
| `MultiMonCallback`, `DDEnumProc` | `0x004A9270`, `0x004A95A0` | adapter GUID selection | Deleted. |
| `gfxFindSafeAdapter` | `0x004AC820` | forces adapter 0 to Software and hunts for 640x480x16 | Keep the shape, drop the Software forcing. The 640x480 entry is still in the synthesised list. |
| `gfxResetAdapter` | `0x004AC870` | pushes `Renderer` into `allowHWTnL`/`useSoftware`/`useBlade` | Force `useSoftware = useBlade = false` unconditionally. |
| `gfxLoadSettings` / `gfxSaveSettings` | `0x004ABD70` / `0x004ABD00` | read/write `gfxconf.dat`: magic `'GFX5'`, choice, count, then `624 * count` bytes **unclamped** | Keep the file, **clamp the count**. Same bug Open1560 has in `dxiReadConfigFile`. |
| `InitDirectDraw` | `0x00401690` | `BeginGfx2D`, retry via `gfxFindSafeAdapter`, else MessageBox and exit | Unchanged. It only calls `BeginGfx2D`. |

Two errors in existing docs to correct here, both from `docs/gfx_layouts.md`:

- `docs/resolution.md` says the resolution cap's height is "in the adjacent slot" to
  `gfxMaxScreenWidth` (`0x006844FC`). It is not: `gfxMaxScreenHeight` is at `0x006844D8`, `0x24`
  bytes earlier. The two `mov`s in `AutoDetectCallback` target unrelated slots.
- `mmGfxCFG` sounds like it belongs in this list and does not. It is a snapshot of `MMSTATE`
  quality knobs — texture quality, reflections, far clip — with no file I/O and no reference to
  `gfxInterfaces`. Leave it alone.

### Wave 1 — device bring-up. Needed to reach a cleared screen.

| Function | Address | Now | Replacement |
|---|---|---|---|
| `gfxPipeline::BeginGfx2D` | `0x004A9370` | `LoadLibraryA("DDRAW.DLL")`, `DirectDrawCreateEx`, `GetCaps`, `gfxWindowCreate`, `SetCooperativeLevel`, `SetDisplayMode` | Load `opengl32.dll`, synthesise modes, `gfxWindowCreate`, create the WGL context, `agiGLInitScaling`. **Written** — `agigl/glgfxpipeline.cpp`. |
| `gfxPipeline::BeginGfx3D` | `0x004A96C0` | front/back/render/Z surfaces, clipper, `QueryInterface(IID_IDirect3D7)`, `EnumZBufferFormats`, `CreateDevice`, `EnumTextureFormats`, `GetCaps` | One framebuffer object with a colour and a depth renderbuffer. Every capability it negotiated is guaranteed. **Written.** |
| `gfxPipeline::SetRes` | `0x004A8CE0` | *not decompiled* (`JUMPOUT`) | Sets `m_iWidth`/`m_iHeight`/`m_ColorDepth`/`m_ZDepth` and the `m_f*` mirrors. Must be read off the disassembly — see "What is blocked". |
| `gfxPipeline::SetWindow` | `0x004A8CA0` | sets `inWindow`, zeroes `m_X`/`m_Y` | Unchanged; no DirectDraw in it. |
| `gfxPipeline::gfxWindowCreate` | `0x004A8A90` | *not decompiled* | **Keep as assembly.** It is the input window too. |
| `gfxPipeline::gfxWindowProc` | `0x004A88F0` | focus, `WM_CLOSE`, power broadcast, forwards to `InputWindowProc` | Keep as assembly for now. Needs one addition later: `WM_SIZE` must re-run `agiGLInitScaling`. |
| `gfxPipeline::EndGfx2D` / `EndGfx3D` | `0x004AAA10` / `0x004AA760` | `RestoreDisplayMode`, release every COM object, `EnumSurfaces` leak check | Destroy the context and framebuffer. `DestroyWindow` must stay — a pixel format can be set on a window **once**, and `SafeBeginGfx` retries up to four times. **Written, minus the `DestroyWindow`.** |
| `gfxPipeline::SafeBeginGfx` | `0x004A92B0` | four-step retry ladder ending in software | Collapse to two steps: try, then try 640x480. Delete the software rungs. |
| `gfxEnumZ` (`operator___2`) | `0x004A91A0` | Z-format enumeration callback | Deleted. Depth is a renderbuffer. |
| `gfxPipeline::gfxEnumTexs` | `0x004A91F0` | texture-format enumeration; sets `g_Tex565`/`g_Tex555` | Deleted. Both globals become meaningless — see the format note below. |

### Wave 2 — the frame loop and the 2D path. This is where the loading screen appears.

This is the shortest route to visible output, because `ProgressCB` is a complete frame that uses no
3D at all: `BeginFrame`, one `CopyBitmap`, one `ProgressRect`, `EndFrame`.

| Function | Address | Now | Replacement | Status |
|---|---|---|---|---|
| `BeginFrame` | `0x004AA130` | lost-surface restore loop, `gfxCreateFont`, `InitCache`, `lpD3DDev->BeginScene` | `MakeCurrent`, `gfxCreateFont`, bind the FBO, clear | **written** |
| `BeginScene` / `EndScene` | `0x004AA270` / `0x004AA2B0` | software-only `BeginScene`/`EndScene` | empty | **written** |
| `EndFrame` | `0x004AA330` | fade quad, `EndScene`, `WaitForVerticalBlank`, `lpdsFront->Blt` or `Flip` | fade quad, `glBlitFramebuffer` to the window, `SwapBuffers` | **written**, fade advance is a TODO |
| `RefreshFrame` | `0x004AA5C0` | re-present, vblank either side | `agiGLPresent` | **written** |
| `FlushFrame` | `0x004AA5B0` | one `ret` | empty | **written** |
| `Clear` | `0x004AADC0` | `lpD3DDev->Clear`, or a full-screen `D3DTLVERTEX` quad when the alpha is partial | `glClear`, or a blended quad. **Two GL-only hazards**: `glClear` respects the depth-write mask and the scissor, `IDirect3DDevice7::Clear` with a null rect list did not. Both are forced off. | **written** |
| `ClearRect` | `0x004AB520` | `Blt` with `DDBLT_COLORFILL`; `arg5` is a **native pixel value**, not ARGB | untextured quad, ARGB. Both call sites must stop pre-converting | **written**, callers pending |
| `CopyBitmap` | `0x004AB4C0` | `lpdsRend->BltFast`, 1:1, colour key on black | textured quad in logical coordinates | **written** |
| `CopyClippedBitmap` ×2 | `0x004B2D40`, `0x004B2D80` | integer scissor, then `CopyBitmap` | transcribed unchanged | **written** |
| `Blit2D` | `0x004AAEC0` | one `DrawPrimitive` **per glyph quad**, FVF `0x1C4` | the same quad path, batched. Its only caller is `gfxDrawFont` | not written |
| `BlitText` | `0x004AB1C0` | same but with independent screen and texel sizes | **no callers anywhere in the image.** Port last or not at all | not written |
| `Readback` | `0x004AACC0` | `lpdsRend->Lock` and row-copy; also the only writer of `gfxImage::sm_Use565` | `glReadPixels` | not written |
| `CreateReadbackImage` | `0x004AAC90` | *decompilation failed* | almost certainly `gfxImage::Create(m_iWidth, m_iHeight, …)` — verify against the binary | not written |

`ProgressRect` (`0x00401010`) is a free function and needs porting alongside `ClearRect`: it does
the 555/565 conversion that `ClearRect` will no longer need. `asUnderlay::Cull` (`0x004A22B0`) is a
full-screen `CopyBitmap` and needs no change beyond `CopyBitmap` itself.

**The loading bar is hard-coded** — `ProgressRect(349, 448, 640 * progress / 284, 10, …)` on the
splash path. `docs/ui_scaling.md` lists it; with the logical-space projection below it will at least
scale rather than sit in a corner.

### Wave 3 — textures. Needed before any 3D geometry is textured.

| Function | Address | Now | Replacement |
|---|---|---|---|
| `gfxTexture::Create(w,h,type,pal,mips)` | `0x004ACA90` | trial-and-error format negotiation: `CreateSurface` once per candidate `DDPIXELFORMAT`, falling back on failure | One `agiGLSurfaceCreate` with a chosen internal format. The negotiation exists because a 1999 card might not support a format; none of them can fail here. |
| `gfxTexture::Load` | `0x004AD100` | picks a per-row CPU converter from the created surface's masks crossed with `gfxImage::Type`, then `Lock`/convert/`Unlock` per mip level, walking the DirectDraw mip chain and `gfxImage::Next` in lockstep | `glTexSubImage2D` per level with the right `GL_UNSIGNED_SHORT_*` type. Most converters vanish. |
| `gfxTexture::Blit` | `0x004AE1E0` | `lpD3DDev->Load` surface copy | FBO blit or `glCopyTexSubImage2D` |
| `gfxTexture::Clone` | `0x004AE250` | `GetSurfaceDesc`, recreate, `Blit` | same shape |
| `gfxTexture::CreateRenderTarget` | `0x004ADF00` | `CreateSurface` with `DDSCAPS_3DDEVICE\|VIDEOMEMORY` | texture plus FBO — the `Framebuffer` field in `agiGLSurface` |
| `gfxTexture::CreateCubeMap` | `0x004ADFD0` | `DDSCAPS2_CUBEMAP` plus six `GetAttachedSurface` | `GL_TEXTURE_CUBE_MAP` |
| `gfxTexture::PreLoad`, `MakeResident`, `MakeAllUnresident` | | `lpD3DDev->PreLoad`, `EvictManagedTextures` | no-ops |
| `gfxTexture::SetLODs`, `MarkFirstUse`, `MarkHigherUse` | `0x004AC9F0` … | `m_Surface->SetLOD` | `GL_TEXTURE_BASE_LEVEL` |
| `gfxTextureCacheEntry` / `gfxTextureCachePool` (all) | | manual VRAM lease/evict with `lpD3DDev->Load` | **Delete.** `gfxTexture::EnableCache(false)` is called at boot (`aiMap.c:118`), so on the shipping path the whole subsystem is already inert. |
| `gfxBitmap::Create` / `Load` / `Clear` / `~` | `0x004AE4D0` … | sysmem surface cloning the render target's pixel format, colour key black | RGBA8 texture, key folded into alpha. **Written** — `agigl/glbitmap.cpp` |

**A format finding worth not re-deriving.** `gfxImage`'s `RGB_0555` payload is actually **5-6-5**.
The proof is in `gfxTexture::Load`'s converter selection: for a 555 surface it runs the down-shift
`(w & 0x1F) | ((w >> 1) & 0x7FE0)`, and for a 565 surface it runs a plain `memcpy`. A converter that
shifts green down by one to reach 555 can only be reading 565, and a `memcpy` into a 565 surface can
only be writing 565. `ARGB_1555` really is 1555. Both mappings are in `glbitmap.cpp` with the same
argument written out.

Related: `g_Tex565` and `g_Tex555` were only ever "what did the card enumerate". They become dead
once `gfxEnumTexs` goes, and any code still branching on them is choosing between DirectDraw texture
formats that no longer exist.

### Wave 4 — the 3D draw path. The largest piece, and not needed for a menu.

| Function | Address | Now |
|---|---|---|
| `Draw` ×2 | `0x004AAAF0`, `0x004AAB50` | `DrawPrimitiveStrided`, `DrawPrimitive` |
| `DrawIdx` ×2 | `0x004AABB0`, `0x004AAC20` | `DrawIndexedPrimitiveStrided`, `DrawIndexedPrimitive` |
| `DrawVB`, `DrawIdxVB` | `0x004ABA50`, `0x004ABB40` | the same against an `IDirect3DVertexBuffer7` |
| `Render`, `RenderIdx` | `0x004B5440`, `0x004B54B0` | the internal (software) submission path — delete with the software renderers |
| `CreateVertexBuffer`, `DestroyVertexBuffer`, `DoLock/DoUnlockVertexBuffer`, `OptimizeVertexBuffer`, `PersistVertexBuffer` | `0x004AB830` … | `IDirect3D7::CreateVertexBuffer` and its `Lock`/`Unlock` | 
| `gfxRenderState::DoFlush` and the whole class | `gfxRenderState.c` | ~88 `SetRenderState`/`SetTextureStageState`/`SetTexture`/`SetLight` sites |
| `gfxViewport::DoFlush`, `DoPerspective` | `gfxViewport.c` | `SetViewport`, `SetTransform(PROJECTION)` |
| `gfxPipeline::SetRenderTarget` | `0x004AB570` | moves the attached Z-buffer between surfaces, `SetRenderTarget`, then re-reads `m_iWidth`/`m_iHeight` from `GetSurfaceDesc` |
| `gfxPacket` submission | `gfxPacket.c` | 19 `DrawIndexedPrimitive*` sites |
| `modShader`, `modStatic`, `modModel` | | `SetMaterial`, `SetRenderState`, light enable |

The FVF decoding is the substance of this wave. `0x1C4` (`D3DFVF_XYZRHW|DIFFUSE|SPECULAR|TEX1`, a
32-byte `D3DTLVERTEX`) is the 2D one; the 3D paths use `0x1C4`, `0x144` and others, and
`gfxFVFOffset` — already ported in `misc/freefuncs.h` — is the function that decodes them.

## The co-port set

These dereference `gfxBitmap::Surface` or `gfxTexture::m_Surface` as a COM object. Each one must be
ported before, or in the same change as, the first thing that puts an `agiGLSurface` in that slot.
Anything missed calls through a pointer that has no vtable and crashes at a known address —
`agiGLSurfaceFromSlot` is what turns the subtler version of this (a real DirectDraw surface still
alive somewhere, producing wrong pixels rather than a crash) into a `Quitf`.

| File | Sites | What |
|---|---|---|
| `gfxBitmap.c` | 9 | `CreateSurface`, `Lock`, `Blt`, `GetDC` |
| `gfxTexture.c` | 53 | create, lock, mip walk, `SetPalette`, `SetLOD` |
| `gfxRenderState.c` | 88 | `SetTexture` at flush time |
| `gfxTextureCacheEntry.c`, `gfxTextureCachePool.c` | 7 | inert at runtime, delete rather than port |
| `mmText.c` | 2 | **`GetDC`/`ReleaseDC` on a DirectDraw surface**, for GDI text. See below. |
| `gfxPipeline.c` | 132 | the device layer itself |
| `_FREE_FUNCTIONS.c` | 25 | `ProgressRect`, `RestoreFocus`, `EnumAllSurfCallback` |
| `gfxViewport.c` | 5 | `SetViewport` only — no surface |
| `modShader.c`, `modStatic.c`, `modModel.c` | 15 | `SetMaterial`, lights |
| `AgeDevice.c` | 31 | delete |

**`mmText` is the awkward one.** `mmText::GetDC` calls `IDirectDrawSurface7::GetDC` so that GDI can
`DrawTextA` straight onto the surface, and `mmText::CreateFitBitmap` builds a `gfxBitmap` that way.
A GL texture has no HDC. The replacement is a CPU-side DIB section rendered with GDI and then
uploaded, which is a self-contained job but not a small one. Until it exists, menu text drawn
through `mmText` will not appear.

## What exists in the tree now

All of it is under `code/midtown2/agigl/`, in the same shape as Open1560's `agigl`/`agisdl`.

| File | What |
|---|---|
| `glcommon.h` | the GL/WGL vocabulary, declared not included, plus `agiPixelFormatDescriptor` |
| `glload.cpp` | resolves `opengl32.dll` and `gdi32.dll` through `GetProcAddress` |
| `glcontext.h` / `.cpp` | `agiGLContext`: pixel format, WGL context, version and extension query, the driver-state cache |
| `glscreen.h` / `.cpp` | `agiGLScreen`, mode synthesis from desktop bounds, `-width`/`-height`/`-scaling`/`-msaa` |
| `glsurface.h` / `.cpp` | `agiGLSurface`, what goes in the `IDirectDrawSurface7*` slots |
| `glpipe.h` / `.cpp` | the render framebuffer, the present blit, and the screen-space quad path |
| `glbitmap.cpp` | `gfxBitmap::Create` / `Load` / `Clear` on a GL texture |
| `glgfxpipeline.cpp` | the `gfxPipeline` statics themselves |

Plus two supporting headers outside `agigl/`: `code/midtown2/data7/printer.h` (the `Displayf`
family, which nothing had declared) and `code/midtown2/gfx/gfxinterface.h` (`gfxInterface` and
`gfxResData`, which `tools/genheaders.py` will never emit because no symbol in the binary is scoped
to them — `docs/gfx_layouts.md` reaches the same conclusion independently).

**The scaling blit comes for free.** `agiGLBegin2D` sets an orthographic projection over the
**logical** resolution rather than the render one, so a widget handing over 640x480 coordinates is
stretched across the render target instead of landing in its top-left corner. That is item 3 of the
four-part fix in `docs/ui_scaling.md` — "a scaling blit to replace `BltFast`" — obtained as a
consequence rather than as work. Items 1, 2 and 4 (the design space, `CreateLocFont`, and the CSV
loaders dividing by literal 640/480) are still outstanding and are independent of this port.

## Wiring it in

Everything under `agigl/` is compiled out today. Two guards, in this order:

1. **`ARTS_AGIGL`** gates the whole directory. Add `-DARTS_AGIGL` to the `cmd` list in
   `compile_sources()` in `tools/build.py` to turn it on. Until then each translation unit
   contributes only its `define_dummy_symbol`, so adding these files cannot break a green build.
2. **`ARTS_AGIGL_PIPELINE`** additionally gates `glgfxpipeline.cpp` and `glbitmap.cpp`, the two
   files that include generated `gfx/` headers. Those headers do not currently compile — see below.

**Nothing is registered in `data/ported.json`, and nothing should be until it has been run.** The
harness treats a registered function as ported and strips the original out of `game.asm`; registering
these early would replace a working DirectDraw path with untested C++. Compiled-but-unregistered is
the safe half-state, and `tools/build.py` handles it by design: it holds the object back and says so.

When the time comes, register through a new `data/ported_agigl.json` rather than editing
`data/ported.json` — `merge_ported()` exists so concurrent agents cannot clobber a shared file.
That file now exists and is deliberately empty. **`docs/opengl_activation.md` is where the `-gl`
switch design and the registration order live**, and it revises this section on one point: the
device bring-up entry points (`BeginGfx2D`, `BeginGfx3D`, `EndGfx2D`, `EndGfx3D`) are never
registered at all, because registering them would destroy the DirectDraw fallback they are supposed
to be an alternative to. The switch lives one level up instead, in the three functions whose
original bodies contain no COM.

## Generated headers that do not compile

Found while writing this. These are `tools/genheaders.py` bugs, not port decisions, and each one
blocks a specific piece:

1. **`gfx/gfxpipeline.h` has a syntax error.** `EnumDDAdapters` is emitted as
   `i32 (__stdcall*)(_GUID*,char*,char*,void*) arg2` — an unnamed function-pointer *type* followed
   by a parameter name. MSVC rejects it. The generator needs to place the parameter name inside the
   parentheses.
2. **`class HWND__;` should be `struct HWND__;`.** MSVC encodes a class as `V` and a struct as `U`,
   so the current declaration would mangle `hwndMain` to `?hwndMain@@3PAVHWND__@@A` while the retail
   map has `PAU`. The generator's own comment above `gfxWindowProc` reads `PAUHWND__`, so the header
   contradicts itself. The same applies to every `HWND__`, `_GUID` and `_DDPIXELFORMAT`.
3. **Missing forward declarations.** `gfx/gfxbitmap.h` names `IDirectDrawSurface7` without declaring
   it. `gfx/gfxtexture.h` names `IDirectDrawPalette` and `gfxTextureCacheEntry`. `gfx/gfxviewport.h`
   names `Matrix44`, `D3DVIEWPORT7` and `gfxCullStatus`. `gfx/gfxpipeline.h` names `gfxDrawMode`,
   `_GUID` and `_DDPIXELFORMAT`.
4. **Nested enums are declared by use but never defined.** `gfxImage::gfxImageFormat` appears in
   signatures in both `gfximage.h` and `gfxtexture.h` and is defined in neither. Its values are in
   `MM2_PSEUDOCODE/gfxImage/gfxImage.h` (`ARGB_8888 = 1` … `Palette4 = 6`); `glbitmap.cpp` repeats
   them locally as a stopgap and should stop once the generator emits them.
5. **`gfxPipeline::m_RenderTarget` is emitted `const`** (`const gfxTexture* const`), which makes
   `SetRenderTarget` — which writes it — impossible to reimplement. The trailing `B` in
   `?m_RenderTarget@gfxPipeline@@0PBVgfxTexture@@B` is being read as a const *variable* rather than
   a pointer-to-const.

Fixing 1–4 is what lets `ARTS_AGIGL_PIPELINE` be turned on. None of them are blocked on anything.

## What is blocked

Ordered by how much it costs to leave unresolved.

1. **DPI awareness, and this one decides whether 1440p actually works.** `GetSystemMetrics`
   reports the *virtualised* desktop size unless the process is DPI aware, and midtown2.exe is not:
   there is no manifest and `SetProcessDPIAware` is not in its import table. On a 2560x1440 display
   at 150% scaling it returns 1707x960, so the synthesised list tops out below native and the whole
   goal is quietly missed. The fix is an application manifest with `<dpiAware>true</dpiAware>`
   embedded at link time — `tools/link.py`, not code. **Verify this before concluding the resolution
   work failed.**
2. ~~**`gfxPipeline::SetRes` is not decompiled**~~ **READ.** `JUMPOUT(0x4A8CE1)`, 0x3CF bytes, now
   transcribed in full in `code/midtown2/agigl/glsetres.cpp`. The finding is that it does **not**
   block the port: every DirectDraw reference in it is inside one `if (lpDD)`, so with `lpDD` null
   under `-gl` the original already writes `m_iWidth`/`m_iHeight`/`m_fWidth`/`m_fHeight`/
   `m_ColorDepth`/`m_ZDepth` and `ioMouse`'s inverses, and skips the mode set; `gfxWindowCreate`
   sizes the window itself. See `docs/opengl_activation.md`. `AutoDetectCallback` (0x399),
   `gfxWindowCreate` (0x206) and `gfxWindowProc` (0x168) are still undecompiled and are still being
   deleted or kept as assembly.
3. **Multi-monitor.** `EnumDisplayMonitors`, `GetMonitorInfoA` and `EnumDisplayDevicesA` are not in
   midtown2.exe's import table, so `build/implibs/USER32.def` has none of them. The synthesised list
   describes the primary display only. Adding the three names to the `.def` is the fix; it is a
   build change, so it is deliberately not made here.
4. ~~**`datArgParser::Get(name, index, value)`**~~ **CLOSED.** The middle parameter is the word
   index, confirmed from `?Get@datArgParser@@SA_NPBDIAAH@Z` at `0x004C61C0`:
   `if (!entry || index >= entry->WordCount) return false; *out = atoi(entry->Words[index]);`.
   `-width`/`-height`/`-scaling`/`-msaa` read correctly. See `docs/opengl_activation.md`.
5. **The fade advance in `EndFrame`.** The original does
   `FadeColor = LerpRGBA(FadeColor, EndFadeColor, FadeValue)` with `FadeValue += delta * FadeSpeed`.
   The frame-time global the decompiler calls `delta` has no confirmed symbol in `data/symbols.json`,
   so the advance is left out rather than guessed. Visible effect: a fade that starts and never
   finishes.
6. **`ClearRect`'s colour convention changes.** The original takes a native-format pixel value; the
   replacement takes ARGB. `mmExternalView.c:160` and `ProgressRect` both pre-convert today and both
   must stop when `ClearRect` is registered as ported. A missed one is a wrong colour, not a crash.
7. **`mmText`'s GDI-on-surface path**, described above. Blocks menu text.
8. **`gfxPipeline::CreateReadbackImage` failed to decompile** (0x2A bytes). Its size and the
   `Readback` contract make `gfxImage::Create(m_iWidth, m_iHeight, <16-bit format>, 0, 0)` the only
   plausible body, but that is inference and it should be read off the binary before it is written.
9. **Window resize.** `gfxWindowProc` has no `WM_SIZE` handling that re-runs `agiGLInitScaling`, so
   a resized window will keep blitting to the old rectangle. Not a blocker for fullscreen or a fixed
   window.
10. **Non-power-of-two textures.** `agiGLSurfaceCreate` does not round up, on the assumption of
    GL 2.0 / `GL_ARB_texture_non_power_of_two`. `gfxTexture::Create` used to round with `ToPow2`.
    A bitmap of arbitrary size — the 640x480 loading screen — depends on this. Guard it on the
    extension when the fallback matters.

## Suggested order of work

Each step is independently testable against the running game, which is the whole point of the
harness.

1. Fix `tools/genheaders.py` items 1–4 above. Nothing else can proceed without them.
2. Embed a DPI-aware manifest. Confirm `GetSystemMetrics` reports native.
3. Read `gfxPipeline::SetRes` off the disassembly and write it.
4. Turn on `-DARTS_AGIGL`, then `-DARTS_AGIGL_PIPELINE`. Confirm everything still compiles and the
   objects are held back. The game must still run unchanged at this point.
5. Register Wave 0 and Wave 1. **The success condition is a black window that does not crash** —
   `BeginGfx2D`/`BeginGfx3D` returning true and `EndGfx` cleaning up.
6. Register Wave 2 and `gfxBitmap`. **The success condition is the loading screen**, which needs no
   3D at all.
7. Wave 3, textures. Confirm with a menu backdrop.
8. Wave 4, the 3D path. This is where the FVF decoding happens and it is most of the remaining work.
9. Only then delete BLADE, `AgeDevice` and the cache classes, and remove the software rungs from
   `SafeBeginGfx`. Deleting them earlier removes the fallback that makes step 5 debuggable.
