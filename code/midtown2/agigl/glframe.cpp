/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2

    Structure, conventions and the arts/core runtime follow Open1560 by Brick,
    an Open Source Re-Implementation of Midtown Madness 1. Copyright (C) 2020 Brick.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "glframe.h"
#include "data7/datargparser.h" // -objdist

#include "core/arts.h"

#include "gfx/ddrawshim.h"
#include "lvl/lvlinstance.h" // lvlInstance::sm_ObjNoDrawThresh
#include "gfx/gfxpipeline.h"
#include "gfx/gfxviewport.h"

define_dummy_symbol(agigl_glframe);

#ifdef ARTS_AGIGL

#    include "data7/printer.h"
#    include "gfx/gfxpipeline.h"

// Declared here rather than included: gfxCreateFont has no generated header of its own, and
// agigl/glbeginphase.cpp declares it the same way for the same reason.
//
// ?gfxCreateFont@@YAXXZ at 0x004B1200
ARTS_IMPORT void gfxCreateFont();

#    include "glcontext.h"
#    include "glstub.h"
#    include "glpipe.h"
#    include "glscreen.h"

extern i32 agiGLProbeDraws; // TEMPORARY PROBE

// TEMPORARY PROBE: the projection and viewport in force when the frame's first geometry is drawn.
f32 agiGLProbeDrawProj[2];
i32 agiGLProbeMainDraws;
i32 agiGLProbeSmallDraws;
i32 agiGLProbeDrawVp[4];
bool agiGLProbeDrawSeen;


i32 agiGLProbeBeginFrames = 0; // TEMPORARY PROBE

// TEMPORARY PROBE: an ORDERED skeleton of a single in-race frame.
//
// Every previous probe sampled one thing in isolation - the state at one draw, the rect at one
// flush - and that is why each one confirmed a theory without ever showing the frame's shape. This
// records the sequence instead: viewport flushes, clears, begins and ends in the order they happen,
// with the number of draws that fell between each pair. If the world is being drawn into the wrong
// viewport, or cleared after it is drawn, the ordering shows it directly.
bool agiGLTrace = false;
i32 agiGLTracePending = 0;

// Flushes the count of draws seen since the last logged event, so each event line is preceded by
// "how much geometry went in under the state that was in force until now".
void agiGLTraceFlush()
{
    if (agiGLTracePending > 0)
    {
        Displayf("T   draws=%d", agiGLTracePending);
        agiGLTracePending = 0;
    }
}

void agiGLBeginFrame()
{
    ++agiGLProbeBeginFrames;

    // TEMPORARY PROBE. Samples every 30th frame for the whole run, because the interesting frames
    // are the ones during a race and the first forty are the menu. Remove when solved.
    {
        static i32 frames = 0;

        if ((++frames % 30) == 0)
            if (agiGLProbeVerbose())
                Displayf("FRAMEDRAWS frame=%d total=%d main=%d mirror=%d firstmain proj=%.3f,%.3f vp=%d,%d %dx%d",
                    frames, agiGLProbeDraws, agiGLProbeMainDraws, agiGLProbeSmallDraws,
                    agiGLProbeDrawProj[0], agiGLProbeDrawProj[1], agiGLProbeDrawVp[0], agiGLProbeDrawVp[1],
                    agiGLProbeDrawVp[2], agiGLProbeDrawVp[3]);

        // Arm the ordered trace on the first frame busy enough to be a race rather than a menu,
        // then follow the two frames after it. Two is enough to see whether the structure alternates.
        {
            static i32 armed = -1;

            if (armed < 0 && agiGLProbeDraws > 300)
                armed = 0;

            if (armed >= 0 && armed < 2)
            {
                ++armed;
                agiGLTrace = true;
            }
            else
            {
                agiGLTrace = false;
            }
        }

        agiGLProbeDraws = 0;
        agiGLProbeMainDraws = 0;
        agiGLProbeSmallDraws = 0;
        agiGLProbeDrawSeen = false;
    }

    // WHAT IS MISSING HERE, AND WHY IT DECIDES THE SIZE OF THE PORT.
    //
    // The original ends with `if (!m_RenderTarget) ForceSetViewport(m_Viewport);`, and
    // gfxPipeline::ForceSetViewport (0x004B2EE0) calls gfxViewport::DoFlush, whose first statement
    // is lpD3DDev->SetViewport. Under -gl lpD3DDev is null, so that call is an access violation and
    // the line cannot simply be transcribed.
    //
    // Leaving it out is correct for the loading screen - ProgressCB draws one bitmap and one
    // rectangle and reads no viewport - and WRONG for anything 3D, because m_Viewport is then never
    // assigned and the projection is never flushed. gfxViewport::DoFlush is therefore in the
    // minimum set for a game frame but not for the first visible frame.
    agiGL->MakeCurrent();

    // gfxCreateFont IS NOT CALLED HERE, and the original does not call it either - this was added
    // by the first draft of the GL frame loop to make sure the font existed.
    //
    // It cannot be called yet. gfxCreateFont asks gfxTexture::Create for the font page, that is
    // still the DirectDraw path, and under -gl it returns null - whereupon gfxCreateFont does
    // `mov ecx, [eax+0Ch]` on it at 0x004B1228 and dies. Worse than dying once: the result is
    // cached in font_texture only on success, so every frame would retry and fault again.
    //
    // Nothing on the loading screen needs a font. Text does, so this has to come back the moment
    // gfxTexture has a GL path - which is the same piece of work the menus are waiting on.
    agiGLBindRenderTarget();

    // ESTABLISH THE FRAME'S VIEWPORT, which the original does and this did not.
    //
    // gfxPipeline::BeginFrame ends with `if (!m_RenderTarget) ForceSetViewport(m_Viewport);`, and
    // ForceSetViewport flushes that viewport to the device. This port left the line out because
    // lpD3DDev was null at the time and the call faulted - that is no longer true, the shim device
    // has been in place for hours.
    //
    // Leaving it out meant the GL viewport at the top of a frame was whatever the PREVIOUS frame's
    // last pass left behind - usually the mirror's rectangle or the HUD's. The game's own Clear
    // scissors to the current viewport, correctly, so it was only ever clearing that stale rectangle:
    // the loading screen stayed on screen under gameplay, and everything outside the rectangle
    // alternated between swap-chain buffers and flickered.
    //
    // Flushing the viewport here rather than calling ForceSetViewport, which is private: the part
    // that matters for the frame is the rectangle and the projection, and that is exactly what
    // agiGLFlushViewport does.
    // The original guards this with `if (!m_RenderTarget)`. That guard is not reproduced here
    // because gfxPipeline::m_RenderTarget does not resolve at link time, and it would be a no-op
    // anyway: agigl renders into the default framebuffer and never sets a texture render target, so
    // the condition is always true under -gl.
    if (agiGLTrace)
        Displayf("T BEGIN frame=%d", agiGLProbeBeginFrames);

    {
        gfxViewport* vp = gfxPipeline::Access_m_Viewport();

        if (vp != nullptr && agiGLTrace)
        {
            const f32* m = reinterpret_cast<const f32*>(vp);

            Displayf("T  BEGINVP this=%p m10=%.6f m14=%.4f", vp, m[10], m[14]);
        }

        if (vp != nullptr)
        {
            // FLAGS 1, NOT 3 - THE RECTANGLE ONLY, NEVER THE PROJECTION.
            //
            // The original's ForceSetViewport sets Flags to -1 and so flushes both, but it does that
            // with a viewport whose projection is valid. Ours is not: measured in a race, this very
            // object (the 3D viewport from BeginGfx3D) holds m22 and m32 collapsed to about 1e-7
            // while the camera had already given it a good near=0.5 far=600 perspective earlier.
            // Forcing bit 2 here pushed that degenerate matrix into GL at the top of every frame.
            //
            // What that looks like on screen is the whole-world flicker. With m22 and m32 near zero
            // every vertex in the city projects to the same clip-space depth, so the depth test can
            // no longer order anything and geometry resolves purely by the order it was submitted -
            // which changes as the camera moves, so buildings wink in and out. It also explains why
            // the mirror always looked right: mmMirror::Cull calls ForceSetViewport on its OWN
            // viewport after this, which flushes its own valid projection over the bad one.
            //
            // Dropping bit 2 is not a workaround for a matrix we failed to compute. The projection
            // is device state that D3D retains, and gfxViewport::Perspective ALWAYS ends in a
            // DoFlush with bit 2 set, so every genuine change still reaches GL through the path the
            // game intends. Nothing here needs to restate it once a frame.
            //
            // The rectangle still has to be flushed, and that part is why this block exists: without
            // it the frame started with whatever rectangle the previous frame's last pass left
            // behind - the mirror's or the HUD's - and the game's own Clear scissors to the current
            // viewport, so it cleared only that stale rectangle.
            agiGLFlushViewport(reinterpret_cast<const f32*>(vp), 1, static_cast<i32>(vp->m_Viewport.dwX),
                static_cast<i32>(vp->m_Viewport.dwY), static_cast<i32>(vp->m_Viewport.dwWidth),
                static_cast<i32>(vp->m_Viewport.dwHeight), vp->m_Viewport.dvMinZ, vp->m_Viewport.dvMaxZ);
        }
    }

    // BeginFrame DOES NOT CLEAR. The original does not, and neither should this.
    //
    // gfxPipeline::BeginFrame in the kit only restores lost DirectDraw surfaces - there is no clear
    // in it at all. This used to blank colour and depth on every call, which was a reasonable
    // stand-in while IDirect3DDevice7::Clear was a do-nothing stub and nothing else ever cleared.
    //
    // With Clear implemented that stand-in became actively harmful: BeginFrame is called more than
    // once per visible frame - ProgressCB calls it, asCullManager calls it - so a clear here wipes
    // work already drawn and the picture flickers. The game issues its own clears, through the
    // device, at the points it intends.
}

// THE RENDER-TARGET CHECK IS MISSING, AND IT IS NOT AN OVERSIGHT.
//
// The original returns early when gfxPipeline::m_RenderTarget is set, because the frame was drawn
// into a texture - the rear-view mirror or the map - and must not be presented. That check cannot
// be written today: genheaders emits the declaration so that MSVC mangles it
// ?m_RenderTarget@gfxPipeline@@0QBVgfxTexture@@B, while the binary has
// ?m_RenderTarget@gfxPipeline@@0PBVgfxTexture@@B. Q against P - a const-placement disagreement
// between the generated header and retail, which is the project's standing trap of a mangled name
// that does not match the machine code. Nothing had ever called Access_m_RenderTarget before, so
// the bad symbol had never had to resolve.
//
// Leaving it out is correct for everything that draws today and WRONG the moment a render target
// exists: the mirror and the map would each present a frame of their own. Neither is reachable
// before Set C, so this has to be fixed as part of it and not after it.
i32 agiGLUnlitBigDraws = 0;
i32 agiGLFlatBigDraws = 0;
i32 agiGLFrameLightMin = 99;
i32 agiGLFrameLightMax = -1;

// THE OBJECT CULL DISTANCE, WHICH IS WHY THE CITY POPS IN.
//
// cityLevel::SetObjectDetail is a four-case switch over the graphics detail slider, and the first
// float it writes is the distance past which lvlInstance::IsVisible returns 0 for EVERY instance -
// roads, buildings, props alike. Read off the disassembly rather than the decompilation, because
// the kit names this global cityLevelLODFurthest while symbols.json calls the same address
// sm_ObjNoDrawThresh, which reads as an assignment between two variables and is not:
//
//     detail 0:  200      detail 2:  300
//     detail 1:  250      detail 3:  300      <- maximum, and the same 300 as detail 2
//
// The far clip is 1000, so 700 units of city sit inside the far plane, in plain view, and
// permanently empty. Objects appear as the camera crosses 300. That is the pop-in.
//
// WHY THIS IS A STORE HERE AND NOT A BINARY PATCH. tools/patches.py refuses addresses it cannot
// find byte-by-byte in game.asm, and this one is emitted as instructions rather than data. A store
// is also better suited to the question: the docs argue the right distance is a judgement about how
// the city looks in motion, so -objdist makes it answerable by driving instead of by rebuilding.
//
// Safe to write every frame because NOTHING ELSE WRITES IT. Scanning the retail image for the
// address finds nine references: four `mov dword [0x5c571c], imm32` (the four presets) and four
// `fcomp dword [0x5c571c]` reads. Re-applying each frame therefore only has to outlast the slider.
// FILL THE EMPTY LOD SLOTS, WHICH IS THE POP-IN ITSELF.
//
// lvlInstance's loader fills four slots - VL, L, M, H - and patches missing ones UPWARD only:
//
//     if (Lods[0] && !Lods[1]) Lods[1] = Lods[0];   // L <- VL
//     if (Lods[1] && !Lods[2]) Lods[2] = Lods[1];   // M <- L
//     if (Lods[2] && !Lods[3]) Lods[3] = Lods[2];   // H <- M
//
// So an object shipped with only a high LOD keeps VL and L NULL. At distance the game selects one
// of them, finds nothing, and draws nothing; the object appears when the camera is close enough to
// select H. The game names these objects itself, in numbers: "<name>: Missing medium and low LOD's".
//
// The blunt version of this fix - holding the LOD thresholds above the draw distance - forced EVERY
// object onto its high LOD at every distance and produced flickering triangles far out over the SF
// ocean. This is the narrow version: it touches only entries that have an H and nothing below it,
// so objects with a proper LOD chain keep using it exactly as MM2 intended.
//
// BOTH ADDRESSES ARE RESOLVED FROM THE INSTRUCTIONS THAT USE THEM, never hardcoded. Retail's data
// addresses are wrong in this build - see docs/gameplay_defects.md defect 8 - but code addresses are
// preserved, so the operand of an instruction is a reliable pointer to whatever the data became:
//
//     0x004CE7F1  mov eax, [ecx*4 + GeomTable]     in vehCarModel::DrawPart
//     0x0046395E  mov eax, [sm_InstanceCount]      in lvlInstance::LoadBoundOnLastEntry
//
// Entry layout is from the kit: GeomTableEntry is 0x20 bytes with modStatic* LOD[4] at offset 0,
// and entries 1..sm_InstanceCount are live (index 0 is used as a reference count).
static void agiGLFillMissingLods()
{
    struct GeomTableEntry
    {
        void* Lod[4];
        void* Shaders;
        void* Bound;
        f32 Radius;
        u8 NumShaders;
        u8 NumShadersPerPaintjob;
        u16 Pad;
    };

    static_assert(sizeof(GeomTableEntry) == 0x20, "GeomTableEntry must match the kit's 0x20 layout");

    static GeomTableEntry* table = nullptr;
    static const i32* count = nullptr;
    static bool resolved = false;
    static i32 filled = 0;

    if (!resolved)
    {
        resolved = true;

        const u8* const at_table = reinterpret_cast<const u8*>(0x004CE7F1);
        const u8* const at_count = reinterpret_cast<const u8*>(0x0046395E);

        // 8B 04 8D = mov eax, [ecx*4 + imm32];  A1 = mov eax, [imm32]. If either opcode is not what
        // it was read from, the image has moved and nothing is touched.
        if (at_table[0] == 0x8B && at_table[1] == 0x04 && at_table[2] == 0x8D && at_count[0] == 0xA1)
        {
            table = *reinterpret_cast<GeomTableEntry* const*>(at_table + 3);
            count = *reinterpret_cast<const i32* const*>(at_count + 1);
        }
        else
        {
            Errorf("GL: cannot resolve GeomTable/sm_InstanceCount - leaving LODs alone");
        }
    }

    // OFF BY DEFAULT, BECAUSE A NULL LOD SLOT IS A DELIBERATE "DO NOT DRAW".
    //
    // vehCarModel::DrawPart selects geometry as
    //
    //     v5 = lvlInstance::GeomTable[a3 + GeomSet].Lods[a2];
    //     if (v5) DrawPart(v5, ...);
    //
    // and that `if` is the switch MM2 uses to HIDE a part at a given detail level. Filling the empty
    // slots turns those intentional nulls into real meshes, so parts that were never meant to be
    // visible - wheel-well interiors and the near-black 0.008 pieces - draw as flat black rectangles
    // over the paint. That is the artefact in the vehicle preview, and it survived the lighting,
    // texture, blend, env-map and depth-write fixes because none of them was the cause.
    //
    // The pop-in this was written for is real (defect 7), but the remedy is wrong: it cannot tell a
    // slot left empty because the artist shipped no low LOD from one left empty on purpose. Kept
    // behind -objlod for experiments, with the draw-distance raise above still on by default.
    if (!datArgParser::Get("objlod"))
        return;

    if (table == nullptr || count == nullptr)
        return;

    const i32 live = *count;

    // A count outside this range means the pointer is not what it is believed to be.
    if (live <= 0 || live > 8192)
        return;

    for (i32 i = 1; i <= live; ++i)
    {
        GeomTableEntry& e = table[i];

        // Only the case the warning describes: a high LOD with nothing under it. Anything with a
        // real chain is left exactly as MM2 built it.
        if (e.Lod[3] == nullptr || e.Lod[0] != nullptr || e.Lod[1] != nullptr || e.Lod[2] != nullptr)
            continue;

        e.Lod[0] = e.Lod[3];
        e.Lod[1] = e.Lod[3];
        e.Lod[2] = e.Lod[3];

        ++filled;
    }

    static i32 said = 0;

    if (filled != said)
    {
        said = filled;

        Displayf("GL: filled %d geom sets that had a high LOD and nothing below it", filled);
    }
}

static void agiGLApplyObjectDrawDistance()
{
    static f32 wanted = -1.0f;
    static bool disabled = false;

    if (disabled)
        return;

    if (wanted < 0.0f)
    {
        f32 given = 0.0f;

        wanted = datArgParser::Get("objdist", 0, given) ? given : 1000.0f;

        // -objdist 0 LEAVES THE GAME ALONE ENTIRELY, which is the A/B this needs.
        //
        // Forcing every object onto its high LOD out to 1000 units draws distant geometry MM2 never
        // drew, and distant geometry is where depth precision is worst - so a new visual fault at
        // range is as likely to be this override as it is to be a pre-existing bug. Being able to
        // switch it off without a rebuild is the difference between knowing and guessing.
        if (wanted == 0.0f)
        {
            Displayf("GL: -objdist 0, leaving MM2's own draw distance and LOD thresholds alone");

            disabled = true;

            return;
        }

        if (wanted < 50.0f)
            wanted = 50.0f;

        Displayf("GL: object draw distance %.0f (retail maximum-detail preset is 300)", wanted);
    }

    // VERIFY THE ADDRESS BEFORE WRITING IT, EVERY TIME.
    //
    // The first version of this stored to 0x005C571C unconditionally and the game died with
    // "Exception c0000005 at EIP=0x5b5284" - an execution fault at a DATA address, which is what a
    // wild jump looks like after a function pointer has been overwritten. A retail data address is
    // not automatically valid in this build: the image is reassembled from game.asm, and assuming
    // otherwise corrupts whatever is actually there.
    //
    // The threshold can only legitimately hold one of the four presets - 200, 250 or 300 - or the
    // value written here on a previous frame. Anything else means the address is not what it is
    // believed to be, and the write is skipped and said once rather than guessed at.
    // ASSIGNED THROUGH THE DECLARED SYMBOL, NOT A LITERAL ADDRESS.
    //
    // The first version of this poked 0x005C571C and 0x005C5708 straight, on the strength of a
    // RETAIL disassembly. Both were wrong here, and provably so: reading the same virtual addresses
    // out of both images shows
    //
    //     retail midtown2.exe   0x005C5708 .data  300.0     0x005C571C .data  300.0
    //     our OpenMM2.exe       0x005C5708 .rdata 5.9e-39   0x005C571C .rdata 6.8e-39
    //
    // The harness reassembles the program and does NOT reproduce retail's data layout. The write to
    // 0x005C5708 therefore went into read-only .rdata and killed the game with an execution fault
    // in .rdata; the write to 0x005C571C was refused by its own guard, which is the only reason it
    // merely did nothing instead of doing the same. A retail data address is a hypothesis about
    // this build, and this one was false.
    //
    // lvl/lvlinstance.h already declares the threshold the correct way - ARTS_IMPORT static f32,
    // carrying the mangled name ?sm_ObjNoDrawThresh@lvlInstance@@2MA - so the linker resolves it
    // wherever it actually lives. No address, no guard, and it cannot be silently wrong.
    lvlInstance::sm_ObjNoDrawThresh = wanted;

    // AND THE LOD THRESHOLDS, WHICH ARE THE ACTUAL POP-IN.
    //
    // Raising the cull distance alone did nothing visible, and lvlInstance's LOD loader says why.
    // It fills four slots - VL, L, M, H - and then patches missing ones UPWARD only:
    //
    //     if (Lods[0] && !Lods[1]) Lods[1] = Lods[0];   // L <- VL
    //     if (Lods[1] && !Lods[2]) Lods[2] = Lods[1];   // M <- L
    //     if (Lods[2] && !Lods[3]) Lods[3] = Lods[2];   // H <- M
    //
    // There is no downward fallback. An object shipped with ONLY a high LOD - which is precisely
    // what the "Missing medium and low LOD's" warning reports, and the log is full of them - keeps
    // Lods[0] and Lods[1] null. At distance the game selects one of those, finds nothing, and draws
    // NOTHING. The object appears when the camera is close enough to select H. That is the pop-in,
    // and pushing the cull distance out only exposed more of it.
    //
    // The thresholds run downward - Med 70 < Low 130 < VLow 200 at maximum detail - so a distance
    // below Med selects the high LOD. Holding all three above the draw distance keeps every object
    // on the LOD it actually has. It costs high-detail geometry at range, which is the trade MM2
    // could not make in 1999 and modern hardware makes easily.
    // PINNING THE LOD THRESHOLDS IS OFF BY DEFAULT, BECAUSE IT CAUSED A WORSE BUG THAN IT FIXED.
    //
    // Holding all three above the draw distance forces EVERY object onto its high LOD at EVERY
    // distance. That draws full-detail geometry far out over the water, where depth precision is at
    // its worst, and the result was triangles flickering toward the camera from a fixed spot in the
    // San Francisco cruise spawn. Confirmed by A/B: -objdist 0 removes it.
    //
    // The defect it was meant to address is much narrower than the remedy. lvlInstance fills its
    // four LOD slots and patches missing ones UPWARD only, so an object shipped with only a high
    // LOD keeps VL and L null and draws NOTHING at distance. That is a handful of objects - the
    // ones the game itself warns about with "Missing medium and low LOD's" - not all of them.
    // Forcing the whole world to high detail to rescue those few is the wrong shape of fix.
    //
    // Kept behind -objlod so the experiment is still one flag away, and so the distance raise above
    // - which is cheap and did not cause the artefact - stays on by default.
    if (datArgParser::Get("objlod"))
    {
        lvlInstance::sm_ObjMedThresh = wanted;
        lvlInstance::sm_ObjLowThresh = wanted;
        lvlInstance::sm_ObjVLowThresh = wanted;
    }

    // THE CITY MESH DISTANCE, RESOLVED THROUGH THE INSTRUCTION THAT READS IT.
    //
    // Roads and building shells are city PAGES, culled by sdlPageLODFar - a different global from
    // the instance threshold above, which is why raising that alone left them popping. The harness
    // emits no symbol for it (nothing matching sdlPageLOD appears anywhere in game.asm), so there is
    // nothing to declare ARTS_IMPORT.
    //
    // A literal address is not an option either. Retail keeps these at 0x005C5708; this build keeps
    // them at 0x005DE708, shifted by 0x19000, and writing the retail address landed in .rdata and
    // killed the game twice.
    //
    // But CODE addresses are preserved by the harness even when data is not, so the reference can be
    // read out of the instruction itself. cityLevel::DrawRooms compares against sdlPageLODFar at
    // 0x00445969 with `fcomp dword ptr [imm32]` - D8 1D, then the address - so the four bytes at
    // 0x0044596B ARE the pointer, whatever the data layout turns out to be. That is self-correcting
    // across rebuilds in a way a baked address is not.
    {
        static f32* page_far = nullptr;
        static bool resolved = false;

        if (!resolved)
        {
            resolved = true;

            const u8* const instr = reinterpret_cast<const u8*>(0x00445969);

            // Only trust it if the opcode is still the fcomp this was read from, and the value it
            // points at is still the 300 retail ships. Either check failing means the image moved
            // under us, and doing nothing is the correct answer.
            if (instr[0] == 0xD8 && instr[1] == 0x1D)
            {
                f32* const candidate = *reinterpret_cast<f32* const*>(instr + 2);

                if (*candidate == 300.0f)
                    page_far = candidate;
                else
                    Errorf("GL: sdlPageLODFar candidate %p holds %g, not 300 - leaving it alone",
                        candidate, static_cast<f64>(*candidate));
            }
            else
            {
                Errorf("GL: 0x00445969 is not the expected fcomp - cannot resolve sdlPageLODFar");
            }

            if (page_far != nullptr)
                Displayf("GL: city mesh draw distance %.0f (was 300, via %p)", wanted, page_far);
        }

        if (page_far != nullptr)
            *page_far = wanted;
    }
}

void agiGLEndFrame()
{
    agiGLApplyObjectDrawDistance();
    agiGLFillMissingLods();

    // DIAGNOSTIC: a frame in which the city drew unlit is a fullbright frame.
    {
        static i32 frame = 0;

        ++frame;

        // Report only when the frame's light count differs from the previous frame - a steady scene
        // is silent, a flickering one prints on every change.
        static i32 last_min = -1;
        static i32 last_max = -1;

        if (agiGLFrameLightMax >= 0 && (agiGLFrameLightMin != last_min || agiGLFrameLightMax != last_max))
        {
            last_min = agiGLFrameLightMin;
            last_max = agiGLFrameLightMax;

            if (agiGLProbeVerbose())
                Displayf("FRAMELIGHT frame=%d lights=%d..%d unlit=%d flat=%d", frame, agiGLFrameLightMin,
                    agiGLFrameLightMax, agiGLUnlitBigDraws, agiGLFlatBigDraws);
        }

        agiGLUnlitBigDraws = 0;
        agiGLFlatBigDraws = 0;
        agiGLFrameLightMin = 99;
        agiGLFrameLightMax = -1;
    }

    // TEMPORARY PROBE: how many EndFrames (presents) happen per BeginFrame. More than one per frame
    // presents partially drawn buffers and flickers. Remove when solved.
    {
        extern i32 agiGLProbeBeginFrames;

        static i32 ends = 0;
        static i32 shown = 0;

        ++ends;

        if ((ends % 30) == 0 && shown < 40)
        {
            ++shown;
            if (agiGLProbeVerbose())
                Displayf("PRESENT begins=%d ends=%d", agiGLProbeBeginFrames, ends);
        }
    }

    if (agiGLTrace)
    {
        agiGLTraceFlush();
        Displayf("T END");
    }

    u32 fade = gfxPipeline::Access_FadeColor();

    if ((fade & 0xFF000000) != 0)
    {
        agiGLBegin2D();
        agiGLDrawQuad2D(nullptr, 0.0f, 0.0f, gfxPipeline::Access_m_fWidth(), gfxPipeline::Access_m_fHeight(), 0.0f, 0.0f, 0.0f, 0.0f,
            fade, false);
        agiGLEnd2D();
    }

    // TODO: the original advances the fade here, LerpRGBA(FadeColor, EndFadeColor, FadeValue) with
    // FadeValue += delta * FadeSpeed clamped to 1. The frame-time global the decompiler calls
    // `delta` has no confirmed symbol, so the advance is left out rather than guessed at - the
    // visible effect is a fade that starts and never finishes.

    agiGLPresent();
}

void agiGLRefreshFrame()
{
    // Same missing render-target check as agiGLEndFrame above, for the same reason.
    agiGLPresent();
}

// THE LAST 3D MATRICES THE GAME SET, KEPT SO 2D CAN PUT THEM BACK.
//
// agiGLBegin2D replaces the projection with a screen ortho and the modelview with identity, and
// there is no matrix stack loaded to push them onto. Without this the first 2D draw of a frame
// permanently destroys the 3D transform, and since the HUD and menus draw every frame, world
// geometry ends up projected into screen-ortho space and lands nowhere near the viewport - which
// is exactly why the car in the selection screen submitted triangles and showed nothing.
//
// Caching what the game hands us is better than saving GL's state: these ARE the game's matrices,
// set through gfxViewport::DoFlush and gfxRenderState::SetCamera, so restoring them cannot drift
// out of step with what it believes is current.
static i32 agiGLLastViewport[4];
static bool agiGLHasViewport;

// gfxPipeline::Clear needs the current viewport rectangle, because a D3D Clear with no rect list
// clears THAT and not the whole target. See agigl/glgfxpipeline.cpp.
bool agiGLGetViewportRect(i32 out[4])
{
    if (!agiGLHasViewport)
        return false;

    for (i32 i = 0; i < 4; ++i)
        out[i] = agiGLLastViewport[i];

    return true;
}
static f32 agiGLLastProjection[16];

// TEMPORARY PROBE. Defined here because it reads the two statics above.
void agiGLProbeNoteDraw()
{
    // SAMPLE THE 200th DRAW OF THE FRAME, NOT THE FIRST.
    //
    // cityLevel::Draw draws lvlSky first, and the kit shows lvlSky deliberately turns ZEnable and
    // ZWriteEnable OFF for the sky and restores them afterwards. Reading the first draw therefore
    // reports the sky's state - depth off - which is correct behaviour and not the bug. The city is
    // hundreds of draws into the frame.
    // SAMPLE THE FIRST DRAW THAT IS THE MAIN VIEW, NOT THE MIRROR.
    //
    // Every earlier sample landed in the rear-view mirror: it renders the whole world into its own
    // small viewport and accounts for hundreds of draws before the main view starts, so both "first
    // draw" and "200th draw" reported mirror state. The mirror's rectangle is ~1024x306; the main
    // view owns the full render target, so a wide viewport is the discriminator.
    // Which viewport is this draw going into? Wide means the main view, narrow means the mirror.
    if (agiGLLastViewport[2] < (agiScreen.render_width / 2))
    {
        ++agiGLProbeSmallDraws;

        return;
    }

    ++agiGLProbeMainDraws;

    if (agiGLProbeDrawSeen)
        return;

    agiGLProbeDrawSeen = true;
    agiGLProbeDrawProj[0] = agiGLLastProjection[0];
    agiGLProbeDrawProj[1] = agiGLLastProjection[5];

    for (i32 i = 0; i < 4; ++i)
        agiGLProbeDrawVp[i] = agiGLLastViewport[i];

    // Everything else that can reject geometry wholesale, read from the context's own cache so it
    // is what GL was actually told rather than what we think we told it.
    if (agiGLProbeVerbose())
        Displayf("DRAWSTATE depth=%d func=%X mask=%d cull=%d front=%X blend=%d(%X,%X) alpha=%d tex2d=%d",
            agiGL->ProbeCap(GL_DEPTH_TEST), agiGL->ProbeDepthFunc(), agiGL->ProbeDepthMask(),
            agiGL->ProbeCap(GL_CULL_FACE), agiGL->ProbeFrontFace(), agiGL->ProbeCap(GL_BLEND),
            agiGL->ProbeBlendSrc(), agiGL->ProbeBlendDst(), agiGL->ProbeCap(GL_ALPHA_TEST),
            agiGL->ProbeCap(GL_TEXTURE_2D));
}

static f32 agiGLLastView[16];
static bool agiGLHasProjection;
static bool agiGLHasView;

// Loads a D3D projection matrix, remapping its depth range on the way.
//
// Split out of agiGLFlushViewport because IDirect3DDevice7::SetTransform(PROJECTION) needs exactly
// the same thing, and the depth remap is the part that is easy to get wrong twice.
void agiGLSetProjectionMatrix(const f32* projection)
{
    if (projection == nullptr)
        return;

    // A D3D projection matrix can be handed to GL verbatim - the two conventions are transposes of
    // each other twice over, so the bytes match. The depth range is the real difference: D3D
    // projects z into [0, 1] and GL into [-1, 1], so the third column is remapped by z' = 2z - w or
    // everything lands in the near half of the depth buffer and z-fighting starts immediately.
    f32 m[16];

    for (i32 i = 0; i < 16; ++i)
        m[i] = projection[i];

    for (i32 row = 0; row < 4; ++row)
        m[(row * 4) + 2] = (2.0f * m[(row * 4) + 2]) - m[(row * 4) + 3];

    for (i32 i = 0; i < 16; ++i)
        agiGLLastProjection[i] = m[i];

    agiGLHasProjection = true;

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m);
    glMatrixMode(GL_MODELVIEW);
}

void agiGLRestore3DMatrices()
{
    // THE VIEWPORT IS PART OF THE 3D TRANSFORM, and restoring the matrices without it is only
    // half the job.
    //
    // agiGLBegin2D resets glViewport to the whole render target, because 2D must not inherit
    // whatever rectangle the 3D path last set. The reverse is equally true and was missing: the
    // vehicle showcase renders into a small panel, so its projection is paired with a small
    // viewport, and drawing that projection at full-screen viewport scales the car up by the ratio
    // between them. That is why it came out enormous while the geometry itself was correct.
    if (agiGLHasViewport)
        glViewport(agiGLLastViewport[0], agiGLLastViewport[1], agiGLLastViewport[2], agiGLLastViewport[3]);

    if (agiGLHasProjection)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(agiGLLastProjection);
    }

    // The composed world * view, not the view on its own. Reloading just the view here would drop
    // whatever world matrix was current and put the scene back at the origin after every HUD draw.
    if (agiGLHasView)
        agiGLApplyModelView();

    glMatrixMode(GL_MODELVIEW);
}

// The logical rectangle last handed to glViewport, so a viewport switch can be detected even
// when the game does not flag the rectangle as changed.
static i32 agiGLLastLogicalRect[4] = {-1, -1, -1, -1};

void agiGLFlushViewport(const f32* projection, i32 flags, i32 x, i32 y, i32 width, i32 height, f32 min_z, f32 max_z)
{
    // THE VIEWPORT RECTANGLE IS IN LOGICAL COORDINATES AND GL WANTS RENDER-TARGET ONES.
    //
    // Everything the game computes is in the logical resolution - 640x480 for the menus - while the
    // framebuffer is whatever the window is, 2560x1440 here. Passing the rectangle through
    // unscaled would confine the entire 3D view to the top-left corner, which is exactly the bug
    // the 2D path already had before agiGLBegin2D started scaling for it.
    //
    // Y IS ALSO FLIPPED. D3D measures the viewport from the top of the target and GL from the
    // bottom, so a rectangle that is correct in one is upside down in the other. This is only
    // invisible while the viewport covers the whole screen, which is why it has to be right before
    // anything renders to a partial one - the mirror and the map both do.
    // APPLY THE RECTANGLE WHENEVER IT DIFFERS FROM WHAT GL HOLDS, NOT ONLY WHEN THE GAME FLAGS IT.
    //
    // gfxViewport::Flags bit 0 means "the rectangle changed" - changed on THIS viewport object. MM2
    // has three of them live at once (measured by logging every rect change for a whole session):
    //
    //     0,0 2560x1440    the main view
    //     1791,1 768x230   the rear-view mirror
    //     1996,1080 527x350 the map
    //
    // and it switches between them constantly. The trace shows returns to the main view arriving
    // with flags=2 - projection only, no rectangle bit - because from that viewport OBJECT's point
    // of view its rectangle has not changed since it was last flushed. It has changed for GL, which
    // has meanwhile been pointed at the mirror or the map, so honouring bit 0 literally left the
    // main view rendering inside the mirror's rectangle for a frame. That is the flash reported as
    // "a weird flicker of what I'm looking at" when toggling the mirror with TAB.
    //
    // Keeping our own record of what was actually issued and comparing against it makes the GL
    // viewport follow the gfxViewport being flushed, which is what the game means by the call.
    // UNCONDITIONAL, WHICH IS WHAT OPEN1560 CONCLUDED TOO.
    //
    // Comparing against a record of what we last issued was not enough, because agigl is not the
    // only writer of glViewport: agigl/glpipe.cpp sets it in three places for the 2D and HUD paths.
    // Any of those leaves the record describing something GL no longer holds, and the next flush
    // then decides it has nothing to do.
    //
    // Open1560 hit the same class of bug and its renderweb.cpp says how it was settled:
    //
    //     FATI mirror/viewport fix (re-applied): the mirror/HUD-map/night-glow passes set the global
    //     NDC->screen mapping to their own small rect; the main view must recompute it every frame
    //     or it inherits that rect and draws the world squashed into a corner. Cheap + idempotent.
    //
    // Same three culprits here - MM2 runs a rear-view mirror at 1791,1 768x230 and a map at
    // 1996,1080 527x350, and the trace shows the main view returning with flags=2, no rectangle bit,
    // because from that viewport OBJECT's point of view its rectangle has not changed. So the flags
    // are not a reliable statement about GL, and the rectangle is simply re-issued every flush. It
    // is one glViewport call at a pass boundary.
    if (true)
    {
        const f32 sx = static_cast<f32>(agiScreen.render_width) / static_cast<f32>(agiScreen.width);
        const f32 sy = static_cast<f32>(agiScreen.render_height) / static_cast<f32>(agiScreen.height);

        const i32 gx = static_cast<i32>(static_cast<f32>(x) * sx);
        const i32 gw = static_cast<i32>(static_cast<f32>(width) * sx);
        const i32 gh = static_cast<i32>(static_cast<f32>(height) * sy);
        const i32 gy = agiScreen.render_height - (static_cast<i32>(static_cast<f32>(y) * sy) + gh);

        agiGLLastLogicalRect[0] = x;
        agiGLLastLogicalRect[1] = y;
        agiGLLastLogicalRect[2] = width;
        agiGLLastLogicalRect[3] = height;

        agiGLLastViewport[0] = gx;
        agiGLLastViewport[1] = gy;
        agiGLLastViewport[2] = gw;
        agiGLLastViewport[3] = gh;
        agiGLHasViewport = true;

        glViewport(gx, gy, gw, gh);

        // RE-ISSUE EVERY CACHED TOGGLE WHEN THE VIEWPORT CHANGES.
        //
        // This is the fix Open1560 needed for the same failure, and agiGLContext::ForceApplyCaps
        // exists for it: the toggle cache skips a glEnable it believes is already set, so any state
        // changed behind its back is not merely wrong but UNRECOVERABLE - EnableDisable will never
        // issue the call that would put it right.
        //
        // A viewport change is where that happens here, because it is exactly where MM2 switches
        // passes. The rear-view mirror renders the whole world into its own small rectangle before
        // the main view starts, and mmDashView does the same; both are noted in this file already.
        //
        // Why it shows up as FLICKER rather than as a constant error: the kit shows MM2 transmits
        // state only on change - gfxRenderState::DoFlush against m_TouchedMask, and all three
        // SetMaterial paths guarded by a comparison against LASTRSTATE. Nothing is ever re-sent, so
        // whether a given surface is lit depends on which pass last touched the state and whether
        // the cache happened to agree, which varies frame to frame with what is on screen.
        //
        // The cost is one pass over the toggle table at a pass boundary, not per draw.
        agiGL->ForceApplyCaps();

        if (agiGLTrace)
        {
            agiGLTraceFlush();
            Displayf("T  VP flags=%d src=%d,%d %dx%d -> gl=%d,%d %dx%d", flags, x, y, width, height, gx, gy, gw, gh);
        }

        // glDepthRange is deliberately not called. It is not in agigl/glcommon.h's entry-point
        // table, and adding it would be pure ceremony: D3DVIEWPORT7's dvMinZ and dvMaxZ are 0 and
        // 1 everywhere in this game, which is already GL's default depth range. If a viewport ever
        // arrives with a different pair this becomes wrong silently, so it is asserted rather than
        // assumed.
        if (min_z < -0.001f || min_z > 0.001f || max_z < 0.99f)
            Errorf("agiGLFlushViewport: depth range %g..%g is not 0..1 and glDepthRange is not loaded", min_z, max_z);
    }

    // Re-issued unconditionally for the same reason as the rectangle above: the flags describe
    // changes to the viewport OBJECT, not to what GL currently holds, and the 2D path overwrites the
    // projection as well.
    if (true) // was: flags & 2
    {
        // The near and far planes the projection encodes. For a D3D perspective matrix
        // m[10] = f/(f-n) and m[14] = -n*f/(f-n), so n = -m[14]/m[10] and f = n*m[10]/(m[10]-1).
        // An absurdly small near plane is the usual reason a large scene z-fights.
        if (agiGLTrace && projection != nullptr)
        {
            const f32 m10 = projection[10];
            const f32 m14 = projection[14];

            if (m10 != 0.0f && m10 != 1.0f)
            {
                const f32 near_z = -m14 / m10;

                agiGLTraceFlush();
                Displayf("T  PROJ near=%.4f far=%.1f (m10=%.6f m14=%.4f)", near_z,
                    (near_z * m10) / (m10 - 1.0f), m10, m14);
            }
        }

        agiGLSetProjectionMatrix(projection);
    }
}

void agiGLSetViewMatrix(const f32* view)
{
    // Handed to the transform state rather than loaded here, because the world matrix that
    // SetTransform supplies has to be composed with it. Loading straight into GL_MODELVIEW was
    // correct only while nothing else wrote that matrix - see the note in agigl/gldraw.cpp.
    for (i32 i = 0; i < 16; ++i)
        agiGLLastView[i] = view[i];

    agiGLHasView = true;

    agiGLSetViewTransform(view);
}


void agiGLProgressRect(i32 x, i32 y, i32 width, i32 height, u32 color)
{
    // The original spends most of its length packing the colour by hand. It asks lpdsRend for its
    // pixel format, compares the GREEN mask against 0x3E0 and 0x7E0 to tell 555 from 565, and then
    // builds the 16-bit value with the shifts that go with whichever it found - which is also a
    // convenient confirmation of the byte order: the red channel is read from the third byte of
    // the argument, green from the second and blue from the first, so the parameter is a plain
    // 0x00RRGGBB.
    //
    // None of that survives contact with GL, which takes the colour at full precision, so all that
    // is left of the original is the rectangle and the colour it fills with. The alpha byte is
    // forced opaque because the original had no alpha at all - a DDBLT_COLORFILL into a 16-bit
    // surface writes exactly what it is given - and passing the argument through unchanged would
    // make every progress bar invisible, its top byte being zero.
    agiGLBegin2D();

    agiGLDrawQuad2D(nullptr, static_cast<f32>(x), static_cast<f32>(y), static_cast<f32>(width),
        static_cast<f32>(height), 0.0f, 0.0f, 0.0f, 0.0f, 0xFF000000u | color, false);

    agiGLEnd2D();
}

#endif
