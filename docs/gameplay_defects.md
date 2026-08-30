# The three things that are wrong with MM2 while you play

Reported from play, not from the disassembly: semi-frequent hitches, aggressive LOD and pop-in,
and odd shading on particular cars. All three are inherited from 1999 rather than introduced by
this project. What follows is what each one actually is, with the evidence.

## 1. Draw distance and pop-in - diagnosed, and it is a real defect

`cityLevel::SetObjectDetail` (0x00443E50) holds every distance in the game as four presets:

| detail | draw distance | very low | low | medium |
|--------|---------------|----------|-----|--------|
| 0      | 200           | 150      | 70  | 20     |
| 1      | 250           | 175      | 90  | 30     |
| 2      | **300**       | 200      | 100 | 40     |
| 3      | **300**       | 200      | 130 | 70     |

Two things are wrong here.

**The graphics slider cannot move the horizon.** Presets 2 and 3 have the same draw distance. Going
from high to maximum detail pushes the LOD transitions outward and leaves the distance at which
objects wink out exactly where it was. There is no setting a player can change that affects it.

**The horizon disagrees with the camera.** `gfxFarClip` defaults to 400 - written once, in
`mmStatePack::SetDefaults` at 0x00523459 - while `lvlInstance::sm_ObjNoDrawThresh` is 300.
`lvlInstance::IsVisible` returns 0 for anything past that threshold, so there is a hundred-unit
band of city that is inside the far plane, plainly in view, and permanently empty. That band is
where things appear from.

Closing the gap is the fix, but the right width for it is a judgement about how the city looks in
motion and not something to derive from a disassembly. `tools/lodtune.cpp` sets all five values in
the running game and holds them against the presets being re-applied, so the numbers can be chosen
by driving around. Once a set earns its place it belongs in `tools/patches.py`, verified against
retail like every other patch.

Raising the far plane is not free: MM2 is a 16-bit-depth-buffer game and pushing the far plane out
costs depth precision. Shimmering on distant coplanar surfaces is the far clip, not the draw
distance.

### What is not the cause

192 props log `Missing medium and low LOD's` during a city load - traffic lights, streetlights,
trees, the breakable props. It looks like the obvious culprit and it is not:
`dgBangerInstance::ComputeLod` (0x004415C0) is eleven bytes long and returns 0 unconditionally.
Bangers always draw at their highest LOD, so LODs they do not have are never asked for.

## 2. Hitches - not what it first looked like

Two hypotheses died here, and both are worth writing down because both were convincing.

**Timer resolution.** `datTimeManager::Update` derives every frame's delta from `timeGetTime`, and
`timeBeginPeriod` appears nowhere in midtown2.exe - not imported, not fetched by name, the string
is not in the binary. That should mean 15.6 ms granularity and a frame delta quantised into
uselessness at any real frame rate. `tools/timeprobe.cpp` measured it instead of assuming:
**1.05 ms**, because something else on the machine holds the system timer resolution down and this
process inherits it. Real on paper, absent in practice.

**Timestep capping.** `SampleStep`'s initialiser in `.data` is 1/60, and `Update` caps the
simulation step at it - which would mean the world runs in slow motion below 60 fps and snaps back
above it, exactly what a hitch feels like. It does not happen. `MainPhase` calls
`datTimeManager::RealTime(0.0f)` at 0x0040170A - `push ebx` directly after `xor ebx, ebx` - which
sets `SampleStep` to zero and takes the uncapped branch, and `datTimeManager::FixedFrame` has no
callers at all. **A `.data` initialiser is not a runtime value.** The timestep is honest real time,
clamped only to [0.1 ms, 100 ms].

So the hitches are genuine frame-time spikes and the question is what causes them, which needs a
distribution rather than an average - a run averaging 200 fps with an 80 ms frame every few seconds
feels far worse than a steady 60, and no average will ever show it. `tools/framewatch.cpp` polls
`FrameCount` in the running game and buckets `ActualSeconds`.

The in-game log is no help: MM2 prints nothing per frame, and the only thing it emits during play
is 2,912 failed `Win32Helper::RegistryHelper::OpenSubKey` lookups from the audio system - about one
every 23 seconds, far too sparse to be a hitch you would notice.

## 3. Car shading - not diagnosed

Reported as a specific corner of a specific car reading darker than the rest of the body.

`cityLevel::GetLightingIntensity` (0x00445290) is a stub returning 1.0, so per-position lighting
lookup is not the cause. The remaining candidates are `cityLevel::SetupPerRoomLighting`, and - more
likely - vertex normals or baked vertex colours in the shipped model. MM2 cars are assembled from
separate breakable parts, and a mirrored half with flipped normals would produce exactly a
one-corner-darker artefact that follows the car everywhere.

Settling it means reading the mesh, which means an extractor for the `.ar` archives. That is worth
building for the modding goal regardless.

## Tools

| tool | what it does | safe while playing |
|------|--------------|--------------------|
| `build/framewatch.exe [secs]` | frame-time distribution from the running game | yes - read-only, never attaches |
| `build/lodtune.exe [draw ...]` | set and hold draw distance and LOD thresholds live | yes - writes only those five floats |
| `build/timeprobe.exe` | measures `timeGetTime` granularity | n/a - standalone |
| `build/dbgwatch.exe [secs]` | captures the game's log without attaching | yes |

`faultwatch` is the exception: it owns the process it inspects and will end a play session. Use it
deliberately, never alongside a session that matters.

## The .ar archives - solved

`tools/arextract.py` lists and extracts them. Modes: `--list [patterns]`, `--verify`, and extract
with `-o DIR`.

The format, from `zipFile::Init` at 0x00573480:

| off | type | meaning |
|-----|------|---------|
| 0x00 | char[4] | `DAVE` |
| 0x04 | u32 | entry count |
| 0x08 | u32 | entry table SIZE in bytes (count*16 rounded up to 2048) |
| 0x0C | u32 | name blob SIZE in bytes |

The trap that cost several wrong attempts: **the last two header fields are lengths, not offsets.**
The entry table sits at a hard-coded 0x800 and the name blob at `0x800 + tableSize`; reading them
as offsets makes validation drift across the array and look like a wrong stride.

Entries are 16 bytes, sorted by name (`zipFile::Open` bsearches them): `u32 NameOffset` (into the
name blob - `Init` adds the blob's base to turn it into the `const char*` of the in-memory
`zipEntry`), `u32 Data` (absolute file offset, 2048-aligned), `u32 UncompressedSize`,
`u32 CompressedSize`. There is no codec flag: equal sizes mean stored, otherwise raw deflate
(`inflateInit2_(..., -15, ...)`, so `zlib.decompressobj(-15)`).

Verified by decompressing every entry in all four archives - mm2core 5680, mm2tex 4365, mm2aud
1868, mm2audex 1425 - with no size mismatches.

## Open thread: the car shading artefact

The car is `vpford`, MM2's 1968 Mustang Fastback. `geometry/vpford.pkg` extracts cleanly (57682
bytes) and is `PKG3`, a container of named sub-meshes with LOD suffixes - the first is `BODY_H`.
Reading vertex normals or baked vertex colours out of it needs the PKG geometry format, which is
the next step, along with the user confirming which corner of the car is dark.

## 4. Lighting flickers to fullbright under -gl - OURS, still open

Reported as lighting popping to fullbright and back on buildings, the player car and traffic, with
the geometry itself steady. Retail midtown2.exe under dgVoodoo, same city and same mods, does NOT
do it (checked twice), so this is the GL layer and not inherited from 1999.

Eliminated by measurement, in order. None of these is the cause:

| hypothesis | how it died |
|---|---|
| Material re-apply cache going stale | flicker present in builds predating that code |
| `ForceApplyCaps` cap-table truncation | real bug, fixed, flicker unchanged |
| `GL_LIGHTING` off during world draws | zero frames in a full session |
| Global ambient varying per area | `agiGLSetAmbient` is called zero times |
| `GetLightEnable` not writing its out-param | it is correctly implemented and wired |
| Every light disabled while lighting stays on | zero frames |

TWO INSTRUMENTATION TRAPS COST TIME HERE, both worth knowing before trusting a probe:

- **A capped dedup table is not a sample.** The DRAWKIND probe recorded the first 96 distinct draw
  shapes and then went silent, so it never saw a draw above 45 vertices and looked like proof that
  the city does not go through that path. It does; the table was simply full.
- **Gating a log line must not touch control flow.** Wrapping the DRAWSTATE probe by inserting an
  early `return` put that return ahead of real bookkeeping in `agiGLProbeNoteDraw` and crashed the
  game on entering water. Gate the Displayf, never the function.

Next step is a probe that fires per draw at the point the world geometry actually goes through -
`agiGLStubDrawIndexedPrimitive`, which is where LITDRAW sees the big draws - rather than in
`agiGLDrawIndexedStrided`, and reporting light COUNT and light VALUES, since lighting is on and lit
throughout.

## 5. Global ambient was a fifth of grey brighter than retail - FIXED

`D3DRENDERSTATE_AMBIENT` defaults to black; `GL_LIGHT_MODEL_AMBIENT` defaults to (0.2, 0.2, 0.2, 1).
MM2 never sets it - measured, zero calls in a full session - so retail renders with no global
ambient while agigl added 0.2 grey to every lit surface. `agiGLResetAmbientToD3DDefault`, called
from `agiGLBeginGfx3D`, starts where D3D does. Brightest effect on dark materials, since ambient is
the one term that does not fall off with the light.

## 6. Pedestrians vanish when you get CLOSE - narrowed, not yet fixed

Inverted from an ordinary LOD fault, which drops things at distance. The kit says why the shape is
inverted, and rules out the obvious suspect:

- `aiPedestrianInstance::ComputeLod` (0x0057B5D0) is eight bytes long and `return 1;`. Pedestrians
  have NO distance LOD, so the "Missing medium and low LOD's" warnings in the log are a red herring.
- `aiPedestrianInstance::Draw` sets the world matrix and calls `pedAnimationInstance::Draw(bool)`.
  The bool is a float comparison the decompiler dropped (it shows as an uninitialised `c0` flag).
- `pedAnimationInstance::Draw(bool drawModel)` branches on it: TRUE draws
  `modModel::Draw(model, skeleton matrices, shaders, ~0)` - the SKINNED path - and FALSE takes a
  cheaper call. Close-up peds are the skinned branch, which is exactly the "fine far away, gone up
  close" shape.

The gap on that path: `D3DRENDERSTATE_VERTEXBLEND` is 0x97 (151), `gfxRenderState::DoFlush` sets it,
and `agiGLStubSetRenderState` has no case for it. GL fixed function has no vertex blending, so it is
ignored. Whether that is the cause is measured by two probes (run with `-glprobe`):

    VERTEXBLEND <n>            - does MM2 ever enable it, and with what value
    BLENDFVF fvf=... v=... i=... - does any draw arrive with blend weights (position type != XYZ)

If both fire near pedestrians, the fix is CPU skinning, and `agiGLSkinPacket` in agigl/glpacket.cpp
is already written for it - it is unused today only because `gfxPacket::Draw` is not registered.
If neither fires, peds are culled before reaching the device and the cull is what to trace.

## 7. Pop-in - it is MISSING LODs, not the draw distance

Raising the draw distance twice changed nothing visible, and lvlInstance's LOD loader says why. It
fills four slots - VL, L, M, H - and patches missing ones UPWARD only:

    if (Lods[0] && !Lods[1]) Lods[1] = Lods[0];   // L <- VL
    if (Lods[1] && !Lods[2]) Lods[2] = Lods[1];   // M <- L
    if (Lods[2] && !Lods[3]) Lods[3] = Lods[2];   // H <- M

There is NO downward fallback. An object shipped with only a high LOD keeps Lods[0] and Lods[1]
null, so at distance the game selects one of them, finds nothing, and draws NOTHING - appearing only
when the camera is close enough to select H. That is the pop-in. The game reports these objects
itself, in numbers: "<name>: Missing medium and low LOD's".

The thresholds run DOWNWARD (maximum detail: Med 70 < Low 130 < VLow 200), so a distance below Med
selects H. Holding all three above the draw distance keeps every object on the LOD it actually has.
agiGLApplyObjectDrawDistance in agigl/glframe.cpp does that, tunable with -objdist.

MM1 does not have this problem because it FADES: Open1560's fix_clip derives FadeScale/FadeConstant
from min(FarClip, ObjectMaxDist) and fades objects over the last 20% of the distance. MM2 has no
object distance fade at all - only gfxPipeline::StartFade, which is the screen fade - so it pops by
design and the LOD selection is the only lever.

## 8. RETAIL DATA ADDRESSES ARE NOT VALID IN THIS BUILD

Cost two crashes and several silently-ineffective changes, so it is worth stating plainly.

    retail midtown2.exe   0x005C5708 .data  300.0     0x005C571C .data  300.0
    our OpenMM2.exe       0x005C5708 .rdata garbage   0x005C571C .rdata garbage
                          0x005DE708 .data  300.0     0x005DE71C .data  300.0

CODE addresses are preserved by the harness. DATA addresses are shifted, here by 0x19000. Writing a
retail data address put a float into read-only .rdata and killed the game with an access violation
inside our own code - and because our .text extends past retail's, the faulting EIP looked like an
execution fault in .rdata, which sent the diagnosis the wrong way for a while.

Three ways to reach a game global, in order of preference:
  1. An ARTS_IMPORT declaration, if one exists - lvl/lvlinstance.h already had sm_ObjNoDrawThresh,
     sm_ObjVLowThresh, sm_ObjLowThresh and sm_ObjMedThresh. The linker resolves it; it cannot be
     silently wrong.
  2. Resolve it from the instruction that references it. sdlPageLODFar has no symbol anywhere in
     game.asm, but cityLevel::DrawRooms reads it at 0x00445969 with `fcomp dword ptr [imm32]`, so
     the four bytes at 0x0044596B are the pointer whatever the data layout is. Guard on both the
     opcode and the expected value before writing.
  3. Never a literal retail address.

## 9. Flickering triangles far out over the ocean (San Francisco cruise spawn) - OPEN

Reported at a fixed spot: facing the ocean from the SF cruise spawn, triangles flicker toward the
camera. Reproducible from the same position every time.

Most likely caused by defect 7's fix rather than being independent of it. Pinning the LOD thresholds
draws distant objects at their HIGH level out to the full draw distance - geometry MM2 never drew at
that range - and distance is exactly where depth precision is worst. Defect 1 in this file already
warned about the shape of it: "Shimmering on distant coplanar surfaces is the far clip, not the draw
distance."

`-objdist 0` disables the override completely and restores MM2's own distances and thresholds, which
is the A/B that settles whether this is ours or inherited. `-objdist 600` keeps the pop-in fix while
pulling the far geometry in.

Open1560 offers no depth-precision fix to copy: its reversed-Z and GL_ARB_clip_control paths in
agigl/glrsys.cpp are both inside `#if 0`, disabled as broken on Intel.

## 10. Speedometer digits oversized and off the left edge - OPEN, narrowed

The HUD time readout (mmHUD::Cull, 0x0042DF40) draws eight glyphs with colons at positions 2 and 5:

    v4 = m_iWidth/2 - 3*digitWidth - colonWidth;   // start, centred
    CopyBitmap(v4, y, bitmap[i], 0, 0, w, h, flag);
    v4 += bitmapWidth(next);                        // advance

Every term is a gfxBitmap Width read raw at offset +4 as a u16. Our gfxBitmap layout matches retail
there (Name 0x000, Width 0x004, Height 0x006), so the read itself is sound.

THE ARITHMETIC, WITH THE REAL ASSETS. DIGI_0.tga is 21x32 and DIGI_COLON.tga is 10x32 (extracted
from mm2tex.ar). gfxBitmap::Create(gfxImage*, bool) resamples through uiScale::PixelsW/PixelsH at
load, so at 2560 the objects should report 84x128 and 40x128, giving

    v4 = 1280 - 3*84 - 40 = 988

The screenshot shows correctly SIZED digits (~84 px wide) starting at about x=38, which requires
3*digitWidth + colonWidth to be about 1242 rather than 292 - roughly a 404-wide "digit". A 404-wide
surface does keep appearing in the 2D traces. So the fault is in WHAT mmHUD is measuring, not in the
layout code, the scaling, or the drawn quads.

Next step: dump with -gltexdump during a race and identify the digit artwork by eye, then work back
from the asset. That is cheaper than more probing - see below.

INSTRUMENTATION FAILURES THAT COST THE MOST TIME HERE. Eight capture attempts missed the digits, and
every one was the probe's fault, not the game's:
  - a 96-entry dedup table that filled during the menus and went silent;
  - a viewport probe capped at 60 that stopped before the mirror was ever toggled;
  - dedup by SIZE only, which hides any draw whose dimensions another element already claimed and
    reports the first one's position instead of this one's;
  - a 40-call budget consumed by full-screen backdrop blits before the HUD was switched on;
  - filtering on gfxBitmap::Name, which is not assigned until after Load returns;
  - a session too short to reach mmHUD::Init.
A probe that finds nothing is a claim about the probe until proven otherwise.

## 11. Vehicle preview rendered black - FIXED (lighting), shadow follow-up

Two separate faults, found in that order.

**The car was unlit.** Every lit mesh in the showcase drew with lights=0 while its material was
1.000 white and its paint texture was bound and real - so it was never the texture or material path
that caused the in-game black cars. The kit shows the arrangement MM2 relies on:

    gfxRenderState::Default():          SetLight(0, &gfxLight::Sun);  LightEnable(0, 1);
    MenuManager:                        SetLight(0, &gfxLight::Sun);   // positions it, never enables
    gfxRenderState::DisableAllLights(): every light off, sm_MaxLights = 0

MenuManager only POSITIONS the sun and depends on Default having enabled light 0, so whichever of
Default and DisableAllLights ran last decides whether the menu is lit. agiGLEnsureSomeLightEnabled
restores the invariant instead of guessing at the order: lighting on with no light enabled can only
render black, so light 0 is switched on when SetLight has actually configured it. Dead code in
gameplay, where lights are always enabled - measured.

**The shadow then drew as opaque black rectangles at each wheel arch.** vehCarModel::DrawShadow uses
SetBlendSet(RSTATE, 0, 128), which is SRCALPHA/INVSRCALPHA - the shadow is meant to be translucent -
and the alpha it blends with rides on the TEXTURE FACTOR. agiGLPacketConstantColor returned white
unconditionally because D3DTSS_COLOROP was never translated, so the shadow got alpha 255.

The earlier attempt at restoring the factor was reverted for a good reason: it fired whenever
COLORARG2 pointed at the factor, including in races where COLOROP is SELECTARG1 - an op that ignores
ARG2 - and darkened geometry the game never meant to darken. COLOROP is now tracked, and the factor
is honoured only by ops that actually read ARG2 (SELECTARG2 through ADD, values 3..7).

## 12. Vehicle preview car renders see-through - OPEN, and it is OURS

Retail under dgVoodoo shows a clean, solid car in the showcase. Ours shows the car as if it does not
occlude itself: the dark interior and far-side geometry read through the near panels, appearing as
flat black rectangles at the wheel arches and along the doors. The user's description is the most
accurate one: "the car is just somehow transparent".

MEASURED, so these do not need re-testing:
  - Lighting is correct since the light-0 invariant fix: lights=1, materials 1.000 for paint.
  - Every texture involved is real. tex 33 is the env sphere map (dark sky, max 255), the paint
    textures are 128x128 and 256x128 with normal content, and no black texture is bound anywhere.
  - The env-map pass is correctly ADDITIVE, src=ONE dst=ONE, so a dark map adds nothing.
  - Every draw runs depth TEST on, depth WRITES off - mask=0 - and gfxRenderStateData::ZWriteEnable
    at 0x01A really is 0. The offset is confirmed against five kit headers and our device state
    agrees, so this is MM2's own state faithfully translated, not a translation bug.

TRIED AND REJECTED:
  - Forcing opaque draws to write depth. No visual change, and it departs from Open1560, whose
    glrsys.cpp honours the game's ZWrite exactly (agiGL->DepthMask(zwrite), no override).
  - Blaming agiGLFillMissingLods. Disabling it changed nothing here. It stays disabled anyway,
    because an empty LOD slot is how vehCarModel::DrawPart HIDES a part - `if (v5) DrawPart(...)` -
    so filling those slots draws geometry MM2 means to be invisible. That is a real defect of that
    approach, just not this symptom.

WHERE TO GO NEXT. Both paths receive the same render state and D3D looks right, so the difference is
in what the two APIs do with it, not in the state itself. The cheapest way to find it is to diff one
preview frame's draw sequence against the D3D call order rather than to keep forming hypotheses
about which pass renders black - four rounds of that produced four dead ends.

