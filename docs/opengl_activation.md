# Turning the OpenGL layer on

`-gl` selects the OpenGL device layer. Without it the process does exactly what it does today.

This document is about the switch itself: where it can be placed, where it cannot, which symbols
have to be registered together, in what order, and what breaks if the set is short.
`docs/opengl_plan.md` is the route; this is the gate.

## The constraint everything follows from

The harness has no notion of a build variant. `tools/asm.py` reads the registration list, finds each
symbol's `PROC` in `game.asm`, and overwrites its body with `db N dup (0CCh)`. That is a
whole-binary, one-time decision:

> **Registering a function destroys its original machine code.** There is no configuration of the
> resulting executable that gets it back.

So "the D3D path is the fallback and it currently works" and "reimplement the function with a
runtime branch" are only compatible when the **else arm can be written**. And the else arm is not
the original code — it is a fresh C++ retyping of it, which has to be correct on the first try,
because the thing it is replacing is gone.

That gives one selection rule, and the whole design is its consequence:

> A function may carry the `-gl` branch **only if its original body contains no DirectDraw or
> Direct3D call**.

The reason to be strict about this is in `docs/harness.md`. Retyping a COM call means writing a
vtable index into a build that cannot see `ddraw.h` — `tools/build.py` puts only `code/midtown2`,
the MSVC include directory and the SDK's `ucrt` on the include path. A wrong index produces a call
that assembles, links, and runs the wrong function. Every verifier in this project asks "is this
difference from retail explainable"; none of them can ask "does this value mean the right thing".
`agigl/glsurface.h` already refuses to fake a 49-slot `IDirectDrawSurface7` vtable for exactly this
reason, and the same argument applies to `IDirectDraw7` and `IDirect3DDevice7`.

## The flag

`-gl`, read through `datArgParser`, in `agigl/glswitch.cpp`.

`datArgParser::Init(argc, argv)` splits the command line into `-name value value …` entries, strips
the leading `-`, and puts each in a hash table. `?Get@datArgParser@@SA_NPBD@Z` — which the
pseudocode kit calls `Exists`, at `0x004C6190` — is a presence test and nothing more, which is what
a bare `-gl` needs. Same mechanism as `-window`, `-sw`, `-triple` and the rest; the flag names live
in the string table at `ClassName+444` and `gfxPipeline::SetRes` reads all of them.

**One ordering hazard, and it is not hypothetical.** `datArgParser::Get` self-initialises:

```c
if (!datArgParser::Argc) datArgParser::Init();
```

and the no-argument `Init()` at `0x004C5BB0` is a single `ret`. It does not read a command line. So
before `Main()` has called `Init(argc, argv)`, every `Get` returns false — silently, with no error.
`agiGLEnabled()` latches its answer so that all callers in a frame agree, so a query made in that
window would latch a permanent `false` that looks exactly like "the user did not pass `-gl`".
`agiGLEnabled()` therefore refuses to latch while `datArgParser::Argc == 0`.

### Open question 4 in `docs/opengl_plan.md` is now closed

That document lists `datArgParser::Get(name, index, value)`'s middle parameter as an assumption. It
is not one. `?Get@datArgParser@@SA_NPBDIAAH@Z` at `0x004C61C0` is

```c
entry = HashTable::Access(&ArgHash, name);
if (!entry || index >= entry->WordCount) return false;
*out = atoi(entry->Words[index]);
```

so `index` subscripts the values collected after the flag. `gfxPipeline::SetRes` passes `0` for
`-width`/`-height`/`-cdepth`/`-zdepth` in its own machine code. `-width`, `-height`, `-scaling` and
`-msaa` in `glscreen.cpp` read correctly.

## Where the switch goes

| Symbol | Address | Size | Original body |
|---|---|---:|---|
| `?InitDirectDraw@@YAXXZ` | `0x00401690` | `0x3D` | `BeginGfx2D`, `gfxSafeMode`, `MessageBoxA`, `exit` |
| `?BeginGfx@gfxPipeline@@SA_NXZ` | `0x004A9350` | `0x1B` | `return BeginGfx2D() && BeginGfx3D();` |
| `?EndGfx@gfxPipeline@@SAXXZ` | `0x004AA750` | `0x0A` | `EndGfx3D(); EndGfx2D();` |
| `?BeginPhase@@YAX_N@Z` | `0x00401AA0` | `0x330` | the boot phase; calls `BeginGfx3D` directly |

The first three are in `agigl/glactivate.cpp` and are enabled. The fourth is in
`agigl/glbeginphase.cpp`, is written, and is **not** enabled - see "The real activation boundary"
below for why it is needed and what blocks it. None of the four touches COM, so all four transcribe
exactly.

**This section used to claim that three was enough, and it was wrong.** The correction is the
subject of the last two sections. The complete caller set is below, produced twice: once by walking
every `PROC` in `game.asm.pristine` for symbol references, and once by disassembling
`midtown2.exe` with `capstone` and matching every `rel32` and every absolute dword in every section
against the target addresses. The two agree exactly, and neither finds an indirect reference or a
pointer to any of these in a data section:

```
BeginGfx2D  0x004A9370  <- InitDirectDraw (x2), gfxPipeline::BeginGfx
BeginGfx3D  0x004A96C0  <- BeginPhase, gfxPipeline::BeginGfx
EndGfx2D    0x004AAA10  <- Main (x2), EndPhase, gfxPipeline::EndGfx
EndGfx3D    0x004AA760  <- EndPhase, gfxPipeline::EndGfx
BeginGfx    0x004A9350  <- gfxPipeline::SafeBeginGfx (x4), rglOpenPipe
EndGfx      0x004AA750  <- gfxPipeline::BeginGfx2D (x4), gfxPipeline::BeginGfx3D (x20), rglClosePipe
```

`gfxPipeline::SafeBeginGfx` itself has **no callers at all** - it is dead code in the retail image,
which is worth knowing before anyone spends time on its software rungs.

With all four activation functions in place and `-gl` on, nothing can reach `BeginGfx2D` or
`BeginGfx3D`. With `-gl` off, all of it runs from its own untouched bytes. **`BeginGfx2D`, `BeginGfx3D`, `EndGfx2D` and `EndGfx3D` are therefore never
registered**, and the GL device bring-up lives under different names —
`agiGLBeginGfx2D` / `agiGLBeginGfx3D` / `agiGLEndGfx2D` / `agiGLEndGfx3D` in `agigl/glgfx.cpp`.
Renaming them is what makes the fallback the real original machine code rather than a retyping of
it.

Two details in the dispatchers that are not arbitrary:

- **`gfxWindowCreate` is called from `gfxPipeline::BeginGfx`, not from the device layer.** It is a
  private static (`?gfxWindowCreate@gfxPipeline@@CAXPBD@Z` — `C` is private), access is part of the
  mangled name, and a free function cannot reach it. `BeginGfx` is a member, so it can. That is a
  large part of why the branch sits there.
- **`InitDirectDraw`'s GL arm calls `BeginGfx`**, bringing up 2D and 3D together, because under GL
  they are one object: a WGL context and one framebuffer. `agiGLBeginGfx2D`/`3D` are idempotent, so
  `rglOpenPipe`'s later call finds the device up and only reapplies the resolution.

## Two findings that shrink the job

**`gfxPipeline::SetRes` does not need to be registered.** It is `docs/opengl_plan.md`'s blocker 2 —
`JUMPOUT(0x4A8CE1)`, `0x3CF` bytes, never decompiled, on the boot path, and the writer of
`m_iWidth`/`m_iHeight`. Read off the disassembly (transcribed in full in `agigl/glsetres.cpp`), its
shape is:

```
if (arg5) { …twenty command-line flags, then -width/-height/-cdepth/-zdepth… }

m_fWidth = (float) width;   useSysMem = useSoftware;
m_ColorDepth = cdepth;      m_ZDepth  = zdepth;
m_iWidth = width;           m_fHeight = (float) height;   m_iHeight = height;

if (lpDD) {                                   // ← every DirectDraw reference is inside this
    if (inWindow) { centre and size the window with MoveWindow ×2 }
    else          { lpDD->GetDisplayMode / lpDD->SetDisplayMode }
}

ioMouse::m_InvWidth  = 1.0f / m_fWidth;
ioMouse::m_InvHeight = 1.0f / m_fHeight;
```

Under `-gl`, `BeginGfx2D` never runs, so `lpDD` is null and the original already does the right
thing: it writes the seven globals, skips the mode set, and sets the mouse scale. The window is not
sized here either — `gfxWindowCreate` reads `m_iWidth`/`m_iHeight`/`m_X`/`m_Y` and calls
`MoveWindow` itself, which its own reference set confirms. So a GL boot needs nothing from `SetRes`
that the original does not already give it. `glsetres.cpp` is guarded by `ARTS_AGIGL_SETRES`, which
nothing defines; it exists as the record of those `0x3CF` bytes and for the later case of a
**runtime** resolution change, which does need the window arm to run with `lpDD` null.

**The mode list does not have to be synthesised for a first frame.** `agiGLSynthesizeModes` belongs
in `gfxAutoDetect` (`0x004ABE00`), whose only caller is `Main()` — a `JUMPOUT` stub that cannot be
reimplemented. There is no COM-free function above it, so registering `gfxAutoDetect` is a one-way
door of exactly the kind this design avoids. It is also not needed: whatever resolution DirectDraw
enumerated is a resolution a framebuffer object can be built at. The first milestone is "GL renders
at a DirectDraw-enumerated resolution"; raising the ceiling is a separate step. `-glmodes` forces
the synthesis for anyone testing it, and is off by default because calling it from `BeginGfx` would
run it *after* `gfxLoadSettings` and `SetRes` have already read the table.

## What the boundary actually is

Everything below this line is a **one-way door**. Each function dereferences `lpD3DDev`, `lpdsRend`
or `lpdsFront` in its original body, so there is no else arm to write: registering any of them
replaces the DirectDraw frame loop for every run, with or without `-gl`. They go in as a set.

Under `-gl`, `lpDD`, `lpD3DDev`, `lpdsRend` and `lpdsFront` are all null and `useSoftware` is 0. So
the boundary is not a wave number — it is **the transitive closure of what a frame touches that
dereferences one of those four pointers.**

### Set A — one frame through `ProgressCB` (the loading screen)

`ProgressCB` at `0x004010F0` is the shortest complete frame in the game and uses no 3D:
`BeginFrame`, one `CopyBitmap`, one `ProgressRect`, `EndFrame`.

| Symbol | Why it is in the set |
|---|---|
| `?BeginFrame@gfxPipeline@@SAXXZ` | `lpD3DDev->BeginScene`, and `lpDD->TestCooperativeLevel` on the restore path |
| `?EndFrame@gfxPipeline@@SAXXZ` | `lpD3DDev->EndScene`, `lpdsFront->Blt`/`Flip`, `WaitForVerticalBlank` |
| `?CopyBitmap@gfxPipeline@@SAXHHPAVgfxBitmap@@HHHH_N@Z` | `lpdsRend->BltFast` |
| `?ProgressRect@@YAXHHHHI@Z` | `lpdsRend->GetPixelFormat`, `lpdsRend->Blt` — a free function, not `gfxPipeline` |
| `?Create@gfxBitmap@@…`, `Load`, `Clear`, `~gfxBitmap` | build the surface `CopyBitmap` reads; `lpDD->CreateSurface` |

**What breaks if the set is short.** Every one of these is a null dereference at a known address, not
a subtle wrong pixel — which is the good case, and is why `agiGLSurfaceFromSlot` `Quitf`s rather
than tolerating a foreign surface pointer. Specifically:

- **`BeginFrame` missing** → access violation in `lpD3DDev->lpVtbl->BeginScene` on the first frame.
- **`EndFrame` missing** → the frame is drawn into the FBO and never presented; then AV on
  `lpdsFront`.
- **`CopyBitmap` missing** → AV on `gfxBitmap::Surface`, which now holds an `agiGLSurface` with no
  vtable. Deliberate: a call through it crashes at an address that names the unported function.
- **`gfxBitmap::Create` missing** → `lpDD->CreateSurface` on a null `lpDD` at load time, long before
  a frame.

**One thing is deliberately absent from the GL `BeginFrame`.** The original ends with
`if (!m_RenderTarget) ForceSetViewport(m_Viewport);`, and `gfxPipeline::ForceSetViewport`
(`0x004B2EE0`) calls `gfxViewport::DoFlush`, whose first statement is `lpD3DDev->SetViewport`. That
line cannot be transcribed under GL. Leaving it out is correct for the loading screen — `ProgressCB`
reads no viewport — and wrong for anything 3D, because `m_Viewport` is then never assigned. That is
the split between Set A and Set B.

### Set B — one frame through `asCullManager::Update` (the game)

Adds, on top of Set A:

| Symbol | Why |
|---|---|
| `?DoFlush@gfxViewport@@AAEXXZ` | `lpD3DDev->SetViewport`, `SetTransform(PROJECTION)` — reached from `BeginFrame` |
| `?ForceSetViewport@gfxPipeline@@CAXPAVgfxViewport@@@Z` | only to restore the `m_Viewport` assignment `BeginFrame` dropped |
| `?EndScene@gfxPipeline@@SAXXZ` | calls `gfxRenderState::DoFlush` whenever the dirty mask is set |
| `gfxRenderState::DoFlush` and the class | ~88 `SetRenderState`/`SetTextureStageState`/`SetTexture` sites |
| `?Clear@gfxPipeline@@SAXHIMI@Z` | `lpD3DDev->Clear` |
| `?ClearRect@gfxPipeline@@SAXHHHHI@Z` | `Blt` with `DDBLT_COLORFILL` |
| `gfxTexture::Create`, `Load`, and the rest of Wave 3 | 53 surface sites |

`gfxTexture::InitCache` is **not** in the set: `gfxTexture::EnableCache(false)` runs at boot
(`aiMap.c:118`), so `sm_UseInternalCache` is false and `InitCache` returns immediately.

**`ClearRect` changes its colour convention when it lands.** The original takes a value in the
surface's own pixel format; the replacement takes ARGB. `mmExternalView.c:160` and `ProgressRect`
both pre-convert today and both must stop in the same change. A missed one is a wrong colour, not a
crash — the only failure in this whole document that is not immediately visible.

### Set C — one frame of geometry (the draw path)

Set A puts a bitmap on the screen and Set B gives it a frame loop with a viewport and a render
state. Neither draws a triangle. This is the set that does, and it is written down here because the
temptation is to register `gfxPacket::Draw` on its own and it does not work on its own.

**The retail draw surface above `lpD3DDev` is not `gfxPipeline`.** `docs/gfx_layouts.md` §15
established, and a fresh scan of every `rel32` and every absolute dword in every section of
`midtown2.exe` confirms, that these have **no callers at all**:

```
?Draw@gfxPipeline@@SAXW4gfxDrawMode@@IHABUgfxStridedData@@@Z            none
?Draw@gfxPipeline@@SAXW4gfxDrawMode@@IHPAX@Z                            none
?DrawIdx@gfxPipeline@@SAXW4gfxDrawMode@@IHABUgfxStridedData@@IPAG@Z     none
?DrawIdx@gfxPipeline@@SAXW4gfxDrawMode@@IHPAXIPAG@Z                     none
?DrawVB@gfxPipeline@@SAXW4gfxDrawMode@@IIPAUgfxVertexBuffer@@@Z         none
?DrawIdxVB@gfxPipeline@@SAXW4gfxDrawMode@@IIPAUgfxVertexBuffer@@IPAG@Z  none
?CreateVertexBuffer@gfxPipeline@@SAPAUgfxVertexBuffer@@II_N@Z           none
?BlitText@gfxPipeline@@SAXABVVector4@@PAGPAEHI@Z                        none
```

**One correction to that finding, made while doing this, because it is stated too strongly
elsewhere.** `gfxPipeline::Render` and `RenderIdx` are *not* callerless:

```
Render     0x004B5440  <- vglBegin+0x46D, vglEnd+0x42, gfxPipeline::DrawVB+0x94
RenderIdx  0x004B54B0  <- Render+0x2F, and an unnamed wrapper at 0x004B73C0
```

They are nevertheless dead **under `-gl`**, and the reason is worth having in one place: every one
of those call sites is inside an `if (sm_UseInternal)`. `vglBegin` reads `mov bl, [0x684500]` at
`+0x42F` and `vglEnd` reads it at `+0x20`, and in both the `je` goes to a
`lpD3DDev->DrawIndexedPrimitive` instead. `agiGLBeginGfx3D` now clears `sm_UseInternal`, so the
software arm is unreachable and so is everything below it.

**There are two live draw surfaces, not one, and only one of them is `gfxPacket`.**

| Surface | Draws | Reached from |
|---|---|---|
| `gfxPacket::Draw` x2, `OrthoMap` | models — cars, props, peds, breakables, the dash | `modStatic::Draw`, `modModel::Draw` |
| `vglBegin` / `vglEnd` | **the city**, and every particle, spark, flare, shadow and debug primitive | 44 callers, including every `psdl_draw_*` |

`vglEnd`'s D3D arm is `DrawIndexedPrimitive` at vtable `+0x68` over a static vertex block at
`0x00661B44`, a static index block at `0x005C9EBC`, an FVF at `0x00681B44` and a primitive type at
`0x00682B78`. It is a second, completely separate submission path, and **it is not written**. A GL
frame with Set C registered but no `vgl` replacement draws the cars and not the road.

#### The complete `lpD3DDev` reference set

Produced by matching every absolute dword in every section of `midtown2.exe` against `0x006830C8`.
Forty-nine functions read it. Under `-gl` it is null, so **each of these is an access violation at a
known address the moment it runs**, and that is the whole of "what breaks if the set is short".

Nineteen are already excluded, and the exclusions are what make the set finite:

| Excluded | Why it never runs |
|---|---|
| `gfxPipeline::Draw` x2, `DrawIdx` x2, `DrawVB`, `DrawIdxVB`, `CreateVertexBuffer`, `BlitText` | zero callers, measured twice |
| `gfxPipeline::Render`, `RenderIdx`, and the wrapper at `0x004B73C0` | only reached under `sm_UseInternal`, which `agiGLBeginGfx3D` clears |
| `gfxPipeline::BeginGfx3D`, `EndGfx3D` | never registered — the GL bring-up has different names (`glgfx.h`) |
| `gfxPipeline::OptimizeVertexBuffer` | `gfxVertexBuffer` path, no callers |
| `gfxTexture::ShutdownCache`, `gfxTextureCacheEntry::Lease` | `gfxTexture::EnableCache(false)` at boot (`aiMap.c:118`) |
| `gfxPacket::AllocateVertexBuffer`, `DoLock`, `DoUnlock`, `Persist` | only reached when `useNativeVBs` is set, which `agiGLBeginGfx3D` clears |
| `gfxTexture::MakeResident`, `MakeAllUnresident` | no callers |

**Those two globals are the single most load-bearing fact in this wave**, and neither was being
written before. A scan finds each of them written in exactly one function in the whole image, and it
is `gfxPipeline::BeginGfx3D` both times — the function `-gl` replaces. So with `-gl` on nothing set
them at all and they kept whatever `.data?` left them:

- **`?sm_UseInternal@@3_NA` (`0x00684500`)** selects MM2's own software submission. Read by
  `vglBegin`, `vglEnd`, `gfxPipeline::DrawVB`/`DrawIdxVB`, `gfxViewport::SetWindow`/`Perspective`,
  `gfxRenderState::DoFlush` and **both `gfxPacket::Draw` overloads**.
- **`?useNativeVBs@@3_NA` (`0x006844F8`)** is read by the **`gfxPacket` constructor** at
  `0x004B38E5`. When it is set and the packet has no matrix indices, the constructor calls
  `AllocateVertexBuffer`, whose body is `lpD3D->CreateVertexBuffer`. That is a null dereference **at
  model load time, long before any frame** — not a draw bug at all, and it would be found by
  bisecting the wrong thing. With the flag clear the constructor writes `StartVertex = 0xFFFF` at
  `0x004B3A97`, every packet is system memory, and the entire vertex-buffer half of `gfxPacket`
  becomes unreachable.

`agiGLBeginGfx3D` in `agigl/glgfx.cpp` now clears both, on every call, with that argument written
above the declarations. `?useHWTnL@@3_NA` (`0x00684501`) is deliberately left alone: the same scan
shows it read only by `AllocateVertexBuffer`, `gfxPacket::Persist`, `gfxVertexBuffer::RestoreAll`
and two `gfxPipeline` vertex-buffer functions, all unreachable once `useNativeVBs` is clear.

#### The set

They go in together. Sizes are from `data/symbols.json`; the two marked **written** are what this
round produced.

| Symbol | Address | Size | Why it is in the set | Status |
|---|---|---:|---|---|
| `?Draw@gfxPacket@@QBEXI@Z` | `0x004B3340` | `0x160` | `DrawIndexedPrimitive` `+0x68`, `DrawIndexedPrimitiveStrided` `+0x78`, `DrawIndexedPrimitiveVB` `+0x80` | **written** |
| `?Draw@gfxPacket@@QBEXPBVMatrix44@@I@Z` | `0x004B34E0` | `0x3C0` | the CPU skin, ending in `DrawIndexedPrimitiveStrided` `+0x78` | **written** |
| `?OrthoMap@gfxPacket@@QAEXM@Z` | `0x004B4760` | `0x100` | `DrawIndexedPrimitive` `+0x68`; sole caller `modStatic::DrawOrthoMapped` | not written |
| `?Draw@modStatic@@QBEXPAVmodShader@@@Z` | `0x004A4550` | `0x180` | `SetMaterial` `+0x40`; 34 callers, everything visible in the world | not written |
| `?DrawEnvMapped@modStatic@@QBEXPAVmodShader@@PAVgfxTexture@@M@Z` | `0x004A4A50` | `0xE0` | same | not written |
| `?Draw@modModel@@QBEXPBVMatrix44@@PAVmodShader@@I@Z` | `0x00597D00` | `0x1A0` | picks between the two `gfxPacket::Draw` overloads per packet | not written |
| `?BeginEnvMap@modShader@@SAXPAVgfxTexture@@ABVMatrix34@@@Z` | `0x004A41B0` | `0x270` | 7 `lpD3DDev` sites; reached from every car draw | not written |
| `?EndEnvMap@modShader@@SAXXZ` | `0x004A4420` | `0x130` | 4 sites | not written |
| `?DoFlush@gfxRenderState@@QAEXPAVgfxRenderStateData@@@Z` | `0x004B4C40` | `0x760` | 41 `SetRenderState` / `SetTextureStageState` / `SetTexture` sites | Set B |
| `?Default@gfxRenderState@@QAEXXZ` | `0x004B1DA0` | `0x3B0` | 19 sites; runs at bring-up | Set B |
| `?Init@gfxRenderState@@AAEXXZ` | `0x004B1C70` | `0x130` | 7 sites | Set B |
| `gfxRenderState::LightEnable`, `SetLight`, `GetLight`, `SetTexSource`, `SetTexTransform`, `SetTexMipmapBias`, `SetTexMatrix`, `SetBlendMatrix`, `SetCamera`, `SetCameraFull` | `0x004B2220` … `0x004B2A80` | `0x20`–`0xD0` each | one `lpD3DDev` site each | not written |
| `?DoFlush@gfxViewport@@AAEXXZ` | `0x004B18D0` | `0x90` | `SetViewport`, `SetTransform(PROJECTION)` | Set B |
| `?DoPerspective@gfxViewport@@SAXPAV1@@Z` | `0x004B2B50` | `0x190` | `SetTransform` | not written |
| `?Blit2D@gfxPipeline@@SAXABVVector4@@PAGPAEHI@Z` | `0x004AAEC0` | `0x300` | one `DrawPrimitive` per glyph quad; sole caller `gfxDrawFont` | not written |
| `?SetRenderTarget@gfxPipeline@@SA_NPBVgfxTexture@@@Z` | `0x004AB570` | `0xF0` | reached from `ltProjection::OffScreenBegin`/`End` | not written |
| `?Blit@gfxTexture@@QAEXHHPAV1@HHHH@Z` | `0x004AE1E0` | `0x70` | `lpD3DDev->Load`; live callers | Wave 3 |
| `?PreLoad@gfxTexture@@QAEXXZ` | `0x004AD790` | `0x50` | `lpD3DDev->PreLoad`; caller `modShader::PreLoad` | Wave 3 |
| `?vglBegin@@YAXW4gfxDrawMode@@H@Z` | `0x004A5500` | `0x590` | **the city.** `DrawIndexedPrimitive` `+0x68` | not written |
| `?vglEnd@@YAXXZ` | `0x004A5A90` | `0x80` | same | not written |

**Two things deliberately *not* in the set, and it is worth saying why so nobody adds them.**

- `?DrawList@gfxPacket@@SAXPBVgfxPacketList@@@Z` (`0x004B4870`, `0x70`) and
  `?MakeList@gfxPacket@@SAPAVgfxPacketList@@PAV1@@Z`. `MakeList` returns its argument unchanged —
  which is *why* `gfxPacketList` and `gfxPacket` are the same `0x2C` bytes. `DrawList` is nine
  instructions walking `Next` and calling `?Draw@gfxPacket@@QBEXI@Z` with `-1`, through a `rel32` to
  the mangled symbol. Neither contains COM, so both *could* carry a `-gl` branch, and neither needs
  one: once `Draw` is ours, `DrawList`'s untouched 1999 machine code calls straight into the C++.
  Registering it strips `0x70` bytes and buys nothing.
- The `gfxPacket` **constructor**. It is COM-free once `useNativeVBs` is clear, and leaving it as
  assembly is what keeps the two 1999 defects in `docs/gfx_layouts.md` §16.3 — a copy constructor
  that never writes `0x24`/`0x28` and shallow-copies `0x1C`/`0x20`, and a class with no destructor at
  all — reproduced by construction rather than by a C++ author remembering to reproduce them.

#### What breaks if the set is short

Every failure here is a null dereference at a named address, which is the good case. Specifically:

- **`gfxPacket::Draw(uint)` alone, without `modStatic::Draw`** → AV in `lpD3DDev->SetMaterial` at
  `modStatic::Draw+0x82`, before the first packet is reached. Nothing is drawn and the crash names a
  function nobody was thinking about.
- **Both `Draw` overloads without `gfxRenderState::DoFlush`** → geometry submitted into whatever GL
  state the 2D path left behind: depth test off, blending on, texture 0 bound to the last menu
  bitmap. Not a crash. This is the one failure in this set that produces a picture rather than an
  address, and it looks exactly like a bad transcription of the draw path.
- **Only one of the two `Draw` overloads** → `modModel::Draw` picks between them per packet, so half
  a car renders and the other half calls `0xCC`.
- **Without `gfxViewport::DoFlush`** → no projection is ever set. Every vertex lands outside the
  frustum and the screen stays the clear colour, with no error anywhere.
- **Without `vglBegin`/`vglEnd`** → AV at `vglEnd+0x51`, `mov edx, [eax]` on a null `lpD3DDev`, the
  first time any city block, particle or shadow is submitted. This is the failure that will actually
  be hit first, because the city is drawn before the cars.
- **Without clearing `useNativeVBs`** → AV in `gfxPacket::AllocateVertexBuffer` at
  `lpD3D->CreateVertexBuffer`, during model loading, before any frame. Fixed in `agiGLBeginGfx3D`.
- **Without clearing `sm_UseInternal`** → `gfxPacket::Draw(uint)` `Quitf`s naming the internal
  rasteriser, and `Draw(const Matrix44*, uint)` silently draws nothing, which is what the original
  does.

#### Two prerequisites outside this document

1. **`data/layouts.json` says `sizeof(gfxMaterial)` is `0x44` and it is `0x48`**, proven four ways
   (`docs/gfx_layouts.md` §7, §12, §20). `modShader::AddStaticMaterial` strides a pool of them, so
   the wrong size is live corruption, and `modStatic::Draw` — which is in this set — reads
   `material+0x0C` to decide alpha blending. The hand edit is still outstanding.
2. **`data/layouts.json`'s `gfxPacket` carries four wrong member names**, which
   `tools/genheaders.py` then emits into `gfx/gfxpacket.h`: `0x08` as `pPositions` when it is the
   `Lock` out-parameter, `0x14` as `PacketSize` when it is a vertex count, `0x16` as **`TriCount`
   when it is an index count**, and `0x28` as `u32` when it is a `gfxReskin*`. The offsets and the
   size are right; only the labels lie. `agigl/glpacket.h` works around it with a mirrored struct
   and a `static_assert` per offset, so the port is not blocked on the edit — but the generated
   header should be fixed before anyone else reads it. `0x16` is the dangerous one, and
   **layouts.json and MM2Hook agreeing about it is not corroboration, because they are wrong the
   same way.**

#### What this round wrote

- `agigl/glpacket.h` — the FVF vocabulary, `gfxPacket`'s confirmed layout mirrored with its four
  names corrected and a `static_assert` on every offset, `gfxReskin` (which no generated header will
  ever emit, because no symbol in the binary is scoped to it), and the stream description.
- `agigl/glpacket.cpp` — the FVF decode, the CPU skin and one `glDrawElements`. Guarded by
  `ARTS_AGIGL` **alone**, so it compiles and is type-checked today without registering anything.
- `agigl/glgfxpacket.cpp` — the two `gfxPacket::Draw` definitions. Guarded by `ARTS_AGIGL` **and**
  `ARTS_AGIGL_PACKET`, which nothing defines, because these are the one-way door.
- `data/ported_agigl_draw.json` — **empty**, with the three reasons in it.

Four GL entry points and two enumerators were added to `glcommon.h`/`glload.cpp` for this:
`glNormalPointer`, `glColor4ubv`, `glSecondaryColorPointer`, `glSecondaryColor3ubv`,
`GL_NORMAL_ARRAY` and `GL_COLOR_SUM`. `GL_COLOR_SUM` also joins `agiGLContext`'s toggle cache, since
the packet path sets it per packet.

#### Four traps found in the translation, recorded so they are not re-found

1. **`gfxPacket+0x16` is an index count.** Sizing an index buffer as `3 * [0x16]` over-allocates
   threefold and looping `[0x16]` triangles reads three times past the end. Four independent proofs
   in `docs/gfx_layouts.md` §16.1; the shortest is that `modStatic::GetTriCount` reads it and
   **divides by three**.
2. **D3D's `dwStride = 0` and GL's `stride = 0` mean opposite things.** D3D reads the same element
   for every vertex — that is exactly how `Draw(uint)` paints a constant colour, by pointing
   `diffuse.lpvData` at the address of its own `colour` argument. GL reads it as "tightly packed,
   derive the stride from the format". Forwarding one to `glVertexPointer` walks off the end of a
   single vertex instead of repeating it. `agiGLDrawIndexedStrided` refuses a zero stride on every
   stream except the two colours, which become `glColor4ubv` instead.
3. **A `D3DCOLOR` is `0xAARRGGBB`, so its bytes in memory are B, G, R, A** — the opposite of what
   `glColorPointer(4, GL_UNSIGNED_BYTE, …)` reads. `GL_ARB_vertex_array_bgra` (core since 3.2) lets
   the array be consumed in place; without it every colour is swizzled on the CPU. Getting this
   wrong swaps red and blue everywhere and looks like a texture-format bug.
4. **`gfxFVFSize` treats `D3DFVF_TEX1` and `D3DFVF_TEX2` as independent bits worth eight bytes
   each**, but D3D7 reads `0x100`/`0x200` as a two-bit texture *count*: `0x200` alone means two
   coordinate sets and sixteen bytes, not eight. Either MM2 never uses more than one set — which is
   what the `LVERTEX` and model formats suggest — or this is a latent 1999 bug that would already
   have made every stride in the game wrong. `agiGLBuildPacketStreams` warns once and draws set 0.

#### Open questions this round did not close

- **The `vgl` immediate-mode path has no GL replacement**, and it draws more of the screen than
  `gfxPacket` does. `vglBegin` is `0x590` bytes and reads four static globals whose types are not
  recovered (`0x00661B44` the vertex block, `0x005C9EBC` the index block, `0x00681B44` the FVF,
  `0x00682B78` the primitive type). That is the next round, not this one.
- **`gfxPacket::OrthoMap` is not written.** It is the one method that walks `Vertices` with a
  hardcoded `0x20` stride instead of calling `gfxFVFSize`, on the assumption that the packet's FVF is
  already `D3DFVF_LVERTEX` (`0x1E2`) — which is what the `gfxForceLVERTEX` branch of the constructor
  produces. Anything ported around it has to preserve that assumption or make it explicit.
- **An `XYZRHW` or blend-weighted packet cannot be decoded**, and not only by the GL layer:
  `gfxFVFOffset` itself answers `0` for position and `12` for normal before it looks at a single
  format flag, so the whole 1999 offset machinery already assumes a 12-byte `D3DFVF_XYZ` position.
  Both builders refuse such an FVF and say so, rather than guessing.
- **The winding order is not settled.** D3D7 culls counter-clockwise by default in a left-handed
  space and `gfxRenderState::Cullmode` overrides it; nothing in either `Draw` overload touches it,
  so it belongs to `gfxRenderState::DoFlush` and is that function's problem. Recorded here because a
  whole world rendered inside-out is the most likely first symptom of a Set C that is otherwise
  correct.
- **The skinned path's 2048-adjunct ceiling.** The original has two adjacent `0x6000`-byte scratch
  buffers and bounds-checks against neither, so a larger packet writes the normals of adjunct 2048
  onward over the positions of adjuncts 0, 1 and 2 — garbage geometry, silently. `agiGLSkinPacket`
  refuses the packet and says so instead, because reproducing an out-of-bounds write is not available
  to C++ at all. Whether any shipped model exceeds 2048 adjuncts is not known.

## Order of work

1. **`-DARTS_AGIGL` in `tools/build.py`.** Nothing is registered; every object is still held back by
   `quarantine_unregistered`. The game must be unchanged. This is also the first time any of
   `agigl/` has ever been compiled, and the first place the two link-level defects below will show.
2. **Register the first three activation symbols**, still without `-DARTS_AGIGL_PIPELINE`. With
   `-gl` absent the game must be byte-for-byte identical in behaviour; that is what tests the
   transcriptions. Done and verified.
3. **Register `BeginPhase`** - wave two, and the correction to the "three is enough" claim. It
   closes the last direct caller of the DirectDraw bring-up, and it is blocked on one change to
   `ExportAsm.java`. Same success condition as step 2: with `-gl` absent, an identical log. With
   `-gl` on, the crash moves from `BeginGfx3D` to `BeginFrame`, which is the next milestone's
   starting point. See "The real activation boundary".
4. **`-DARTS_AGIGL_PIPELINE`**. CORRECTED - this step cannot be taken on its own, and the two
   reasons given for it are both out of date.

   The header blockers are gone: `gfxbitmap.h` now carries `struct IDirectDrawSurface7;` and
   `gfximage.h` declares `gfxImage::gfxImageFormat` as an opaque enum with a fixed underlying
   type, which is complete enough to pass by value. Defining the flag compiles all of `agigl/`
   with **zero** compile errors, so that half of the claim is simply finished.

   What it cannot do is change nothing. The claim "still nothing registered" assumed the flag only
   ADDS code, and it does not - the guards are complementary. `gfx/gfxpipeline.cpp` is wrapped in
   `#if !(defined(ARTS_AGIGL) && defined(ARTS_AGIGL_PIPELINE))` and `agigl/glgfxpipeline.cpp` in
   the positive of the same condition, so the flag switches the DirectDraw transcription OFF at the
   same moment it switches the GL one on. But the GL objects are still held back by
   `quarantine_unregistered` - `glgfxpipeline.obj` has 11 symbols and `glbitmap.obj` 3 that are not
   in `data/ported.json` - so the exported symbol ends up defined by nobody. The build fails at
   symbol verification with `MISSING ?CopyBitmap@gfxPipeline@@SAXHHPAVgfxBitmap@@HHHH_N@Z`, which
   is the gate working exactly as intended: a port would silently not take effect.

   Registering those symbols is not a fix for that on its own either, because the GL replacements
   are unconditional. `gfxPipeline::CopyBitmap` in `glgfxpipeline.cpp` goes straight to
   `agiGLSurfaceFromSlot` / `agiGLDrawQuad2D` with no DirectDraw path, so with `-gl` ABSENT the
   loading screen would stop being drawn at all. That breaks the default configuration, which is
   the one being played.

   So step 4 and step 5 are one step, not two, and it needs a third thing neither mentions: a
   runtime branch on the `-gl` flag, so the DirectDraw transcription stays reachable when GL is
   off. Until that exists the flag must stay out of `tools/build.py`; it was tried, it failed this
   way, and it was reverted.
5. **Register Set A as one change.** Success condition: the loading screen under `-gl`, and no
   change at all without it.
6. **Register Set B.** Success condition: a menu. **Not a drivable frame** - this step used to
   claim one and it was wrong, because nothing in Set A or Set B draws a triangle. Geometry is
   Set C.
7. **The `gfxMaterial` `0x44` -> `0x48` hand edit in `data/layouts.json`**, then regenerate headers.
   `modStatic::Draw` is in Set C and strides a material pool, so this has to be right before step 8
   rather than after it.
8. **`-DARTS_AGIGL_PACKET`, then register Set C as one change.** Success condition: a drivable
   frame with cars but, until the `vgl` path is written, no city. See "Set C - one frame of
   geometry".
9. Only then delete BLADE, `AgeDevice`, the texture cache classes, and the software rungs of
   `SafeBeginGfx`. Earlier removes the fallback that makes step 4 debuggable.

## Blockers found while doing this

1. **`__stdcall` imports do not link, and this affects every file under `agigl/` already.**
   `tools/ghidra/ExportAsm.java` emits imports undecorated (`__imp__MessageBoxA`) because that is
   all Ghidra reports, and `tools/implibs.py` builds the import libraries from undecorated `.def`
   files — `dumpbin` on `build/implibs/USER32.lib` reads `Name type: no prefix` on every entry. But
   `extern "C" __declspec(dllimport) int __stdcall MessageBoxA(...)` emits a reference to
   `__imp__MessageBoxA@16`, which does not exist. `glload.cpp`, `glscreen.cpp` and `glcontext.cpp`
   all declare their imports this way and have never been link-tested.
   `agigl/glwin32.h` reconciles the two with `/alternatename` and is included from `glcommon.h`, so
   the fix reaches the existing files too. Declaring the imports `__cdecl` instead would resolve the
   name and corrupt the stack, so the convention has to stay and the *name* has to move.
2. **`exit` must be spelled by hand.** `InitDirectDraw` ends in `call _exit`, which `game.asm` emits
   as a real `PROC` at `0x00581944` — the game's own 1999 CRT. `<cstdlib>` would bind to the modern
   CRT, which this link does not have. `glactivate.cpp` declares
   `extern "C" void __cdecl exit(int)`, which decorates to exactly `_exit`. Untested.
3. **DPI awareness, unchanged from `docs/opengl_plan.md` and still the thing that decides whether
   1440p works.** `GetSystemMetrics` reports virtualised sizes for a non-DPI-aware process. The fix
   is a manifest at link time, not code.
4. **`SetRes` under `-gl` cannot yet rebuild the framebuffer.** A runtime resolution change needs
   `agiGLInitScaling` and `agiGLBeginGfx3D` rerun afterwards, and `SetRes` runs at boot before the
   device exists, so it needs a flag it does not have. Noted in `glsetres.cpp`; it is not a first-
   frame problem because the resolution does not change during boot.
5. **`gfxAutoDetect` has no COM-free caller**, so the synthesised mode list cannot be reached without
   a one-way registration. See above; it is not needed for a first frame.
6. **`useInterface`/`interfaceCount` (`0x006830D8`, `0x006830DC`) are not
   `gfxInterfaceChoice`/`gfxInterfaceCount` (`0x006844C8`, `0x006844C0`).** The first pair belongs to
   `gfxPipeline::EnumDDAdapters`, the second to the `gfxconf.dat` adapter table. `glscreen.cpp`
   writes the second; `SetRes` and `SafeBeginGfx` read the first. Under GL the first pair is dead,
   but the names are close enough to cost someone an afternoon.
7. **Undecorated globals are not linkable, and this is what holds wave two.**
   `tools/ghidra/ExportAsm.java` emits `PUBLIC` only for symbols beginning with `?`, so `useIME`,
   `audioFlags` and `inputDevice` are module-local labels in `game.asm` and no C++ can name them.
   Any port of a function that touches a global the linker map did not decorate hits this, not just
   `BeginPhase`. The fix is one line in the exporter plus an `/alternatename` on the C++ side for
   the leading underscore; both halves are described in "What blocks it, and it is not about
   OpenGL".
8. **`gfxPipeline::SafeBeginGfx` has no callers at all.** Confirmed against the bytes: nothing in
   the image references `0x004A92B0`. Its software-fallback ladder is already dead code, so the
   deletion step at the end of the order of work is cheaper than it reads.
9. **The `vgl` immediate-mode path has no GL replacement, and it draws most of the screen.**
   `vglBegin` (`0x004A5500`, `0x590`) and `vglEnd` (`0x004A5A90`, `0x80`) submit every `psdl_draw_*`
   city block, every particle, spark, flare and shadow through `lpD3DDev->DrawIndexedPrimitive`, out
   of static buffers at `0x00661B44` and `0x005C9EBC`. It is a second draw surface entirely
   independent of `gfxPacket`, and Set C without it renders the cars and not the road. See "Set C".
10. **`data/layouts.json`'s `gfxPacket` member names are wrong at four offsets**, and
   `tools/genheaders.py` emits them into `gfx/gfxpacket.h`. `agigl/glpacket.h` mirrors the struct
   with the corrections and a `static_assert` per offset, so nothing is blocked - but the generated
   header still misleads anyone who reads it. `0x16` is `IndexCount`, not `TriCount`.

## Wave one is enabled and verified (measured, not predicted)

`?InitDirectDraw@@YAXXZ`, `?BeginGfx@gfxPipeline@@SA_NXZ` and `?EndGfx@gfxPipeline@@SAXXZ` are
registered and the whole `agigl/` layer now compiles and links (`-DARTS_AGIGL` is in `build.py`).

**Without `-gl`, the game is behaviourally identical.** Same 66,068 lines of debug output, same zero
access violations, same arrival at `GameLoop`. Normalising per-run hex and thread ids, the logs
before and after activation differ in nothing but Windows' own `tid(...)` values. That is the result
the wave was designed to produce, and it means the three fallback transcriptions are faithful.

Three things had to be fixed to get there, none of them about OpenGL:

1. **A free function needs a declaration to be stripped.** `asm.py` works from `ARTS_EXPORT` markers
   in headers, and `genheaders.py` does not generate one for free functions - `misc/freefuncs.h` is
   hand-maintained. Registering `InitDirectDraw` in `ported.json` alone left the original `PROC` in
   place and the link failed with "already defined in game.obj".
2. **The quarantine's idea of a "game symbol" was too broad.** It held back every object in this
   layer for defining names like `?agiGLBeginDevice@@YA_NXZ`, which appear in no version of
   midtown2.exe and therefore cannot shadow an unported function. The test is now membership in
   `data/symbols.json`, which also retires the `?Access_` prefix special case.
3. **Exceptions are off** (`-EHs-c-`). `-EHsc` makes the compiler emit unwind tables referencing
   `__CxxFrameHandler3`, and this build has no CRT to supply it. The game has no C++ exception
   handling anyway, so a reimplementation that unwinds where the original could not would diverge
   in a way nothing here can check.

## The real activation boundary

With `-gl` actually on, the game gets through adapter detection - banner, both adapters, the 1440p
cap - and then dies reading null at `?BeginGfx3D@gfxPipeline@@SA_NXZ +0x90`. That is `0x004A9750`,
`mov edx, [eax]` on the vtable of a null `IDirectDraw7`, four instructions into the function.

The cause is the caller scan above. **`BeginPhase` calls `gfxPipeline::BeginGfx3D()` directly**,
immediately after `SetRes` and before `vglSetFormat`, and under `-gl` nothing created `lpDD`.

### Which of the new callers actually matter

Three callers were missing from the old list. Only one of them is a problem, and that was checked
rather than assumed.

- **`BeginPhase` -> `BeginGfx3D`. Fatal, and this is the whole of wave two.** The call is
  unconditional and `BeginGfx3D` dereferences `lpDD` before doing anything else. There is no early
  return to reach: the `inWindow` test at `+0x32` only chooses which of two arms dereferences it.
- **`EndPhase` -> `EndGfx3D`, `EndGfx2D`. Harmless.** Every COM pointer in both functions is
  null-tested before use - `lpDD`, `lpdsFront`, `lpdsBack`, `lpdsBack2`, `lpdsRend`, `lpdsZ`,
  `lpD3D`, `lpD3DDev`, `lpClip`, without exception. Under `-gl` they run to completion and do
  nothing but `ShowCursor(TRUE)`, `DestroyWindow(hwndMain)`, `gfxPipeline::EndInternal`, and the
  `gfxTexture` / `gfxBitmap` list teardown.
- **`Main` -> `EndGfx2D` (twice). Harmless, same reason.**

The teardown path is not free of consequences, though, and it is worth writing down because
`MainPhase` runs `BeginPhase ... EndPhase` in a **loop**: `EndGfx2D` destroys `hwndMain` on every
phase change and nothing releases the WGL context that was on it, so the GL layer has to survive
having its window pulled out from under it and rebuilt. That is a teardown defect, not a crash, and
it belongs to the same milestone as a first frame rather than to this one.

### Why the branch goes in `BeginPhase` and not in `BeginGfx3D`

`BeginGfx3D` is `0xA70` bytes of nothing but COM, so under the rule at the top of this document it
can never carry the branch: its else arm would be a hand retyping of
`CreateSurface` / `QueryInterface` / `CreateDevice` against vtables this build has no headers for.

`BeginPhase` qualifies. There is no COM anywhere in its `0x330` bytes - every call in it is a
direct call or a call through a named function-pointer global (`datDisplayUsed`, `gfxLoadImage`).
The transcription is `agigl/glbeginphase.cpp`.

**It carries exactly one conditional, at the `BeginGfx3D` call site.** Both arms share the entire
rest of the body. That is deliberate, and it is the argument for why a `0x330`-byte boot-path
transcription is acceptable at all: the else arm is not a rarely-taken fallback, it is what every
run executes, so the transcription is **verifiable by the test that signed off wave one** - run
without `-gl`, diff the 66,068 lines, nothing but `tid` values may differ. Contrast Set A and Set B
above, where the ported code runs only under `-gl` and there is no before-and-after to compare.

### What blocks it, and it is not about OpenGL

**Three of the globals `BeginPhase` touches cannot be named from C++.**
`tools/ghidra/ExportAsm.java` writes a data label as

```java
if (sym.startsWith("?")) w.println("PUBLIC " + sym);
w.println(sym + " LABEL BYTE");
```

so a mangled global is `PUBLIC` and linkable, and an undecorated one is a module-local label the
linker never sees. Most of what `BeginPhase` reads is mangled and fine. These three are not:

| Global | Address | In `game.asm.pristine` |
|---|---|---|
| `useIME` | `0x006B19BC` | `useIME LABEL BYTE`, line 881874, no `PUBLIC` |
| `audioFlags` | `0x006B1670` | `audioFlags LABEL BYTE`, line 881668, no `PUBLIC` |
| `inputDevice` | `0x006B16F8` | `inputDevice LABEL BYTE`, line 881702, no `PUBLIC` |

Registering `BeginPhase` today strips its `0x330` bytes and the link then fails on three unresolved
externals. So `data/ported_agigl_wave2.json` is **empty**, and that is the finding rather than an
omission.

The fix is one change in `ExportAsm.java` - emit `PUBLIC` for undecorated data labels too - plus a
name reconciliation the C++ side already has. `extern "C" i32 useIME;` emits a reference to
`_useIME`, because `__cdecl` prepends an underscore on x86, while the label is `useIME`;
`glbeginphase.cpp` bridges the two with `/alternatename`, exactly as `glwin32.h` bridges
`__imp__MessageBoxA` with `__imp__MessageBoxA@16`. `/alternatename` can only redirect an undefined
symbol to a **defined** one, so it does nothing until the label is actually public - which means
the failure mode while this is half-done is an unresolved external, not a silently wrong address.

A fourth global has no name at all. `0x005E0CFC` is four bytes of `.data?` passed to `ProgressCB`;
the only reference to it in the entire image is that one push, and nothing anywhere reads or writes
its contents, so it is permanently zero. `game.asm` reaches it as `?GraphicsPreviousMenu@@3EA + 1`
and so does the transcription - `""` would be observably identical but would assume something about
`ProgressCB` that does not need assuming.

### The alternative, for whoever decides the harness question

The blocker above is a symptom of the axiom this document opens with: registering a symbol destroys
its machine code, so a caller has to be ported whole in order to change one call inside it.

There is a way to make that untrue, and it is worth stating because it would dissolve the Set A and
Set B one-way doors as well as this one. If `asm.py` could **rename** a `PROC` instead of stripping
it - `?BeginGfx3D@gfxPipeline@@SA_NXZ` becomes `?ddBeginGfx3D@@YA_NXZ`, keeping its bytes, its
internal branches and its symbolic operands - then the C++ could own the mangled name and its else
arm would be `return ddBeginGfx3D();`. The else arm would then be **the original machine code**,
not a retyping of it, and the "no COM in the original body" rule would stop being necessary at all.

That is a change to `tools/asm.py` and to the export, with a blast radius across every ported
symbol, so it is not made here. It is the right question to ask before Set B, not after.

### Where the 640x480 lives now

`BeginPhase` is where the hardcoded frontend resolution is: `gfxPipeline::SetRes(640, 480, 16, 16,
false)` on the `arg1 != 0` path, and the same 640x480x16 in the `ChangeDisplaySettingsA` call above
it. `MainPhase` calls `BeginPhase(splashScreen == 0)`, so that path is the one taken when there is
no splash screen. Porting `BeginPhase` makes all of it reachable from C++ for the first time, which
matters to the 1440p work.

**It is not changed.** The transcription uses `kFrontendWidth` / `kFrontendHeight` /
`kFrontendColorDepth` so that the three places the pair is used - the display mode, `SetRes`, and
the `gfxImage::Scale` that sizes the loading screen - are visibly one decision, and it holds the
same values the original does. Changing it and landing the transcription in the same edit would
destroy the only test either of them has: the point of the log diff is that nothing may differ.

One thing in the transcription is deliberately **not** literal, and it is recorded here so nobody
has to find it in a diff. The original hands `ChangeDisplaySettingsA` 148 bytes of **uninitialised**
stack with only `dmSize`, `dmFields`, `dmBitsPerPel`, `dmPelsWidth` and `dmPelsHeight` written; the
driver also reads `dmDriverExtra`. The transcription zeroes the buffer. Reading an uninitialised
local is undefined behaviour that MSVC is entitled to exploit, so the literal form is not available
to C++ at all, and zeroing can only change behaviour in the case where the original passed garbage.

The other deliberate omission is the MSVC exception frame. `BeginPhase` opens one at entry and
steps `[ebp-4]` through unwind states 0, 1 and 2 around its three allocations. `-EHs-c-` cannot
emit that, nothing in this codebase throws, and a structured exception walking the chain gets
`ExceptionContinueSearch` from `__CxxFrameHandler` for a non-C++ record. `?BeginPhase@@YAX_N@Z_SEH`
at `0x005A8E54` simply becomes an unreferenced `PROC`.

### Enabling wave two, in order

1. `ExportAsm.java` emits `PUBLIC` for undecorated data labels; re-export and reimport.
2. `-DARTS_AGIGL_PHASE` in `tools/build.py`.
3. `ARTS_EXPORT void BeginPhase(bool arg1);` under its mangled-name comment in `misc/freefuncs.h`.
   `asm.py` works from the `ARTS_EXPORT` markers in headers and `genheaders.py` emits none for a
   free function, so a json entry alone leaves the original `PROC` in place and the link fails with
   "already defined in game.obj" - the exact trap `InitDirectDraw` hit in wave one.
4. `?BeginPhase@@YAX_N@Z` into `data/ported.json` and into `data/ported_agigl_wave2.json`.
5. Run **without** `-gl` and diff the log. Only then look at the `-gl` arm, where the next stop is
   `BeginFrame` and Set A.

## BeginPhase: the linker blocker is gone, three header blockers remain

`ExportAsm.java` now exports `useIME`, `audioFlags` and `inputDevice`, so the reason
`?BeginPhase@@YAX_N@Z` could not link is resolved. Compiling it (`-DARTS_AGIGL_PHASE`) surfaces
three more, all in generated headers rather than in the port:

1. **`check_size(eqEventHandler, 0x148)` fails.** This is the short-layout problem: `BeginPhase`'s
   own `new` literal says `0x160`. The recovered members do not reach the real size, and the moment
   any C++ includes that header the assertion fires. `mmInput` is in the same state (`0x248` vs
   `0x1DC`).
2. **`eqEvent` is undeclared in `mminput.h`** - a missing forward declaration for a type used by
   pointer.
3. **`gfxImage::Release()` does not exist.** `~gfxImage` is private and 1999 inlined the release at
   every call site, so no `?Release@gfxImage@@` symbol exists to import. An agent added a public
   inline to `gfx/gfximage.h` - and the next build regenerated that header and wiped it.

Point 3 is the one worth generalising. **A hand edit to a generated header does not survive**, because
`build.py` step 2 runs `genheaders.py` on every build and it emits from `data/layouts.json` and
`data/symbols.json` alone. Anything a port needs from a generated header has to come from the
generator, or from a hand-written header that includes it.

The attempted shortcut on the linker side is also worth recording. Exporting *every* recovered name
rather than an explicit three made 12,221 symbols public, and the game began jumping to a stack
address inside `start` - while `.text` still verified byte-perfect against retail, so the damage was
entirely in link-time resolution. An explicit allowlist has no blast radius; the general version
needs to understand what it is colliding with first.

## The activation boundary is closed

`?BeginPhase@@YAX_N@Z` is ported and registered. With `-gl` absent the game is behaviourally
identical: **308,030 log lines against 308,237 before, zero access violations**, same city load, same
`GameLoop` at 14.4M heap. That is the test the whole wave was designed around - the else arm is what
every ordinary run executes, so a faithful transcription is proved by the game simply working.

Three things had to be cleared to compile it, and all three were in generated headers rather than in
the port:

1. **`check_size(eqEventHandler, 0x148)`** - `layouts.json` carried `u32 field_0` at offset 0, so
   `genheaders` emitted it *alongside* the compiler's implicit vptr and `sizeof` came out 0x164.
   Renaming member 0 to `void** vTable` (which `emit_members` skips) fixed it. The real size is
   0x160.
2. **`eqEvent` undeclared in `mminput.h`** - resolved by the member-type forward declarations.
3. **`gfxImage::Release()` did not exist.** 1999 inlined the release idiom at every call site, so
   there is no such symbol, and `~gfxImage` is private. A hand edit to the generated header was
   wiped by the next build, which is the lesson: **anything a port needs from a generated header
   must come from the generator.** `genheaders.py` now emits a public inline `Access_Destroy()` for
   any class with a private or protected destructor - the same precedent as the `Access_` accessors
   for private statics. It deliberately does not free the storage or touch the refcount; that logic
   differs per class and belongs at the call site, written to match the original.

## Where `-gl` reaches now

Past `BeginGfx3D`, which is where it used to die. It now runs through `BeginPhase`, the bank
manager and on to the loading screen, and stops at

```
?Create@gfxBitmap@@SAPAV1@HH_N@Z +0x34   reading 0x00000000
```

That is exactly the boundary `docs/opengl_activation.md` predicted: **Set A, one loading-screen
frame.** `gfxBitmap::Create` wants a DirectDraw surface and under `-gl` there is none, so the null it
reads is the surface pointer. Nothing is wrong here that is not simply unwritten - the frame loop is
the next wave, and it is a one-way door because every function in it dereferences `lpD3DDev`,
`lpdsRend` or `lpdsFront` and so has no fallback arm to write.

---

# WHERE IT LANDED (2026-08-28)

**dgVoodoo has been removed.** With `DDraw.dll`, `D3DImm.dll` and `D3D8.dll` renamed out of the MM2
folder, `-gl` reaches `GameLoop` with zero access violations and the game enumerates real hardware
instead of "dgVoodoo DirectX Wrapper". That was the point of all of this.

## The plan above was not the plan that worked

Two decisions did most of the work, and neither is in the staged Set A / Set B / Set C above.

**1. `data/keep_original.json` - registering a function without losing the original.** `asm.py`
normally replaces a ported PROC with 0xCC padding, which forced every `-gl` branch to carry a hand
transcription of the DirectDraw body in its else arm. Listed symbols now have their PROC *renamed*
to `_arts_orig_<stem>` instead, and `gfx/origfallback.h` calls it through a cast. That is cheaper
and STRICTER: the fallback is the 1999 machine code itself, so "identical with `-gl` absent" holds
by construction rather than by careful transcription.

**2. Shim COM objects instead of porting sixty-nine functions.** Every function touching `lpD3DDev`,
`lpDD` or `lpdsRend` failed on a null pointer, and the staged plan was to port them all.
`agigl/glstubdevice.cpp` and `agigl/glddshim.cpp` instead give the game a Direct3D device whose
methods do nothing and a DirectDraw whose `CreateSurface`, `Lock`, `Unlock` and `GetDC` are real
and GL-backed. The original texture loader and the GDI text path then run UNMODIFIED. It is also
where the renderer grows: replacing a stub with a real implementation is how `SetTexture`,
`SetRenderState` and the two draw calls were done, one slot at a time.

## The rules this work established

- **A stub that hands back an object must FAIL, not report success.** `GetAttachedSurface`
  returning S_OK with an untouched out-parameter made `gfxTexture::Load` believe in a mip level and
  Release a stale pointer.
- **A stub that enumerates must actually enumerate.** `EnumTextureFormats` returning S_OK without
  calling its callback describes a device supporting no format at all, which surfaced far away as
  "Texture ptx_wheel not found".
- **Staging memory comes out of MM2's 32 MB heap, not DirectDraw's.** One buffer per surface took a
  city load from 9.6M to 24.5M and the allocator answered "Heap overrun". Allocate on Lock, free on
  Unlock.
- **`agiGLBegin2D` clobbers state the 3D path needs, and every piece must be cached and restored in
  `agiGLEnd2D`.** So far: the projection and modelview matrices, the render state (it disables depth
  testing every frame), and the viewport (the vehicle showcase draws into a small panel, so its
  projection at full-screen viewport scales the car up by the ratio between them). This one has
  caught four separate bugs and is the first thing to check when something 3D looks wrong.
- **Turning the UI scaling layer on exposes everything that guessed wrong about design space versus
  screen space,** and all of it fails silently at 640x480 where the scale factor is 1.0.

## What is left

Texture stage state beyond `COLOROP` is untranslated and multitexture is not handled, so effects
that combine two textures will be wrong. Vertex buffers are stubs - safe today because
`useNativeVBs` is forced false, so MM2 submits system-memory arrays.

MM2Hook must stay disabled (`dinput.dll.mm2hook-off`): it injects through a `dinput.dll` proxy and
patches midtown2.exe at hardcoded addresses, which do not mean the same thing in a binary whose
`.rdata` and `.data` sit 0x13000 higher.
