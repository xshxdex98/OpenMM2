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

#ifdef ARTS_AGIGL
#    include "agigl/glstub.h" // agiGLProbeVerbose
#endif

#include "ddrawshim.h"
#include "gfxviewport.h"

#include "data7/printer.h"

#include "origfallback.h"

#ifdef ARTS_AGIGL
#    include "agigl/glframe.h"
#    include "agigl/glswitch.h"
#endif

define_dummy_symbol(gfx_gfxviewport);

// THE FUNCTION THE WHOLE 3D PATH WAS WAITING ON.
//
// docs/opengl_activation.md names this as the thing that decides where Set A stops: the original
// gfxPipeline::BeginFrame ends with ForceSetViewport, ForceSetViewport calls DoFlush, and DoFlush's
// first statement is lpD3DDev->SetViewport - so under -gl it faulted, and every route that touched
// a viewport had to be avoided rather than ported. Leaving it out was correct for the loading
// screen, which reads no viewport, and made anything 3D unreachable.
//
// Both halves of the original are here. The flags are the original's: bit 0 means the rectangle
// changed and bit 1 the projection, and both are cleared at the end whichever path ran, because a
// viewport that keeps its dirty bits set re-flushes forever.
//
// ?DoFlush@gfxViewport@@AAEXXZ
void gfxViewport::DoFlush()
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        // The projection matrix is the FIRST member of gfxViewport, which is why the original
        // casts `this` straight to a D3DMATRIX rather than taking the address of a member. Doing
        // the same here keeps that dependency visible instead of hiding it behind a name.
        // TEMPORARY PROBE: every viewport flush from startup, with the projection it carries.
        // gfxViewport::Perspective always ends in DoFlush with bit 2 set, so a viewport that never
        // appears here with a sane m10 has never been given a perspective.
        {
            // Log only when the projection actually CHANGES, at full precision. The matrix is
            // valid at setup and degenerate in a race, so what matters is the transition, not the
            // steady state - printing every flush buries it.
            const f32* m = reinterpret_cast<const f32*>(this);

            static const void* last_this = nullptr;
            static f32 last_m10 = 0.0f;
            static f32 last_m14 = 0.0f;
            static i32 shown = 0;

            // NOT CAPPED, AND KEYED ON THE RECTANGLE.
            //
            // The previous form stopped after 60 entries and deduped on the projection, so it went
            // silent long before the rear-view mirror was ever toggled and made it look as though
            // the mirror never set a viewport at all. A capped probe is not a sample - it is a
            // window onto the start of the session, and reading it as absence of behaviour has now
            // cost two wrong conclusions (see also the DRAWKIND table in docs/gameplay_defects.md).
            //
            // Keyed on the rectangle instead: every change of viewport rect prints, for as long as
            // the game runs, which is exactly the mirror-to-main transition being investigated.
            static u32 last_rect[4] = {0xFFFFFFFFu, 0, 0, 0};

            const bool rect_changed = m_Viewport.dwX != last_rect[0] || m_Viewport.dwY != last_rect[1] ||
                m_Viewport.dwWidth != last_rect[2] || m_Viewport.dwHeight != last_rect[3];

            if (rect_changed)
            {
                last_rect[0] = m_Viewport.dwX;
                last_rect[1] = m_Viewport.dwY;
                last_rect[2] = m_Viewport.dwWidth;
                last_rect[3] = m_Viewport.dwHeight;

                if (agiGLProbeVerbose())
                    Displayf("VPRECT rect=%u,%u %ux%u flags=%d", m_Viewport.dwX, m_Viewport.dwY,
                        m_Viewport.dwWidth, m_Viewport.dwHeight, Flags);
            }

            if (shown < 60 && (this != last_this || m[10] != last_m10 || m[14] != last_m14))
            {
                ++shown;
                last_this = this;
                last_m10 = m[10];
                last_m14 = m[14];

                if (agiGLProbeVerbose())
                    Displayf("VPFLUSH this=%p flags=%d rect=%u,%u %ux%u m0=%g m5=%g m10=%g m11=%g m14=%g",
                        this, Flags, m_Viewport.dwX, m_Viewport.dwY, m_Viewport.dwWidth, m_Viewport.dwHeight,
                        m[0], m[5], m[10], m[11], m[14]);
            }
        }

        agiGLFlushViewport(reinterpret_cast<const f32*>(this), Flags, static_cast<i32>(m_Viewport.dwX),
            static_cast<i32>(m_Viewport.dwY), static_cast<i32>(m_Viewport.dwWidth),
            static_cast<i32>(m_Viewport.dwHeight), m_Viewport.dvMinZ, m_Viewport.dvMaxZ);

        Flags = 0;

        return;
    }
#endif

    ArtsOrigViewportDoFlush(this);
}

// ?Perspective@gfxViewport@@QAEXMMMM@Z
//
// THE WORLD-FLICKER FIX, AT THE ONE PLACE THE BAD MATRIX IS ACTUALLY BUILT.
//
// Under -gl the race world winked in and out as the camera moved while the HUD, markers,
// checkpoints and minimap stayed rock steady. That split is the diagnosis: markers and
// checkpoints do not depend on depth ordering and the world does. The 3D viewport held a
// DEGENERATE projection in-race - m22 and m32 collapsed to ~1e-7 - so every vertex projected to
// one clip-space depth, the depth test could not order anything, and geometry resolved by
// submission order, which changes with the camera.
//
// The zero arrives as the FAR PLANE, through 1999 code that has no guard anywhere along it:
//
//   CreateViewport()            returns a ZEROED gfxViewport, so field_130 (far) starts at 0
//   mmGame                      caches this+1204 = gfxPipeline::VP->field_130
//   PUGraphics::SetFarClip(v)   gfxFarClip = v, then Perspective(VP, fov, aspect, near, v)
//   mmGame::FarClipCB           *(float*)0x005D4420 = VP->field_130 * (useSoftware ? 0.5 : 1)
//   every gameplay camera       Perspective(m_Viewport, fov, 0, near, *(float*)0x005D4420)
//
// and Perspective then computes m22 = far/(far-near), m32 = -near*far/(far-near). With far at
// zero both collapse.
//
// SEEDING THE VIEWPORT AT CREATION WAS NOT ENOUGH, which is why that earlier fix did not hold:
// SetFarClip re-runs Perspective with gfxFarClip, so a zero there overwrites the seed. Guarding
// here instead catches the zero whichever of those five steps produced it, and does so at the
// last moment before the matrix is built.
//
// The DirectDraw path is untouched. It never sees a zero far because its own bring-up path
// assigns one before any camera reads it, and in any case this must not change what the game does
// without -gl.
void gfxViewport::Perspective(f32 arg1, f32 arg2, f32 arg3, f32 arg4)
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        // NaN-safe comparisons: written as negated `>` so a NaN, which fails every comparison,
        // takes the substitution rather than sailing through an `if (x <= 0)`.
        // A FAR PLANE BELOW kMinSaneFar IS WRONG EVEN IF IT IS GREATER THAN NEAR, and missing that
        // is why the world still went flat sky-colour after the first version of this guard.
        //
        // The gameplay chase camera asks for far = 1.171875. With a near of 0.1 that satisfies
        // far > near, so the original condition passed it through and built a projection with a
        // 1.17-unit far plane - from ground level there is nothing inside it but sky. During the
        // start-of-race pan the same camera's near is larger (2.195, then 1.203), which DID trip
        // far > near, which is exactly why the pan looked correct and the moment it settled behind
        // the car did not.
        //
        // The threshold is read off the measured traffic, not chosen: every legitimate caller in a
        // race asks for 100 (rear-view), 600, 1000 (main and interior) or 1600 (minimap). The bad
        // value is 1.17. Fifty separates them by a factor of two in one direction and a factor of
        // twenty in the other.
        //
        // This treats the symptom. The source is MMSTATE.FarClip, which PUGraphics::FixClip feeds
        // to Perspective and which has itself become 1.171875 - that is a draw-distance SETTING
        // holding a nonsense value, and fixing it there is the real repair.
        constexpr f32 kMinSaneFar = 50.0f;

        const bool bad_far = !(arg4 > 0.0f) || !(arg4 > arg3) || !(arg4 >= kMinSaneFar);
        const bool bad_near = !(arg3 > 0.0f);
        const bool bad_fov = !(arg1 > 0.0f);

        // TEMPORARY PROBE: every Perspective call, first 24 only. The blanket far=600 substitution
        // is suspected of wrecking depth precision for the car preview, which is a small object
        // close to the camera - so what matters is what each CALLER actually asks for.
        {
            // SAMPLED INTO THE RACE, not just the first 24 calls. Every value reasoned about so far
            // came from the menu, because the budget was spent before a race ever started - which is
            // how "the cameras ask for near=1 far=1000" became a belief about gameplay when it was
            // only ever true of the frontend.
            //
            // The symptom to explain: the world is visible during the start-of-race camera pan and
            // goes flat sky-colour the moment the camera drops to the car. That is what a NEAR plane
            // far too large looks like - everything inside it is clipped, so from ground level there
            // is nothing left but sky, while the high pan still clears it. So near matters here at
            // least as much as far, and both are printed every time along with whether the guard
            // fired.
            static i32 seen = 0;

            // EVERY SUBSTITUTION IS LOGGED, unsampled. The sampled form above hid the case that
            // matters: mmDashView::Cull calls Perspective once per frame with the dashboard's own
            // near/far, and a dashboard sits centimetres from the camera, so its far plane is small
            // by design. If kMinSaneFar rewrites it to 600 the dash is drawn through a projection
            // meant for the world, which is what "the speedometer numbers are huge and smushed into
            // their spot" would look like. A once-per-frame call is exactly what a 1-in-200 sample
            // misses.
            if ((bad_far || bad_near || bad_fov) || seen < 24 || (seen % 200) == 0)
            {
                if (agiGLProbeVerbose())
                    Displayf("PERSP vp=%p fov=%g aspect=%g near=%g far=%g bad=%d", this, arg1, arg2, arg3, arg4,
                        (bad_far || bad_near || bad_fov) ? 1 : 0);
            }

            ++seen;
        }

        if (bad_far || bad_near || bad_fov)
        {
            // Said once rather than every frame - this runs per camera per flush, and a per-frame
            // line would bury the log. If it never appears, the flicker was something else and
            // this whole branch is dead weight worth deleting.
            static bool told = false;

            if (!told)
            {
                told = true;

                Displayf("GL: Perspective given fov=%g aspect=%g near=%g far=%g - substituting, "
                         "a zero far is what collapses the projection and flickers the world",
                    arg1, arg2, arg3, arg4);
            }

            // 0.5 and 600 are the game's own numbers, not invented ones: 0x005D4420 ships as
            // 600.0f in .data and the healthy setup projection measured m10=1.000834 m14=-0.5004,
            // which is exactly near=0.5 far=600.
            if (bad_fov)
                arg1 = 70.0f;

            if (bad_near)
                arg3 = 0.5f;

            if (bad_far)
                arg4 = 600.0f;
        }
    }
#endif

    ArtsOrigPerspective(this, arg1, arg2, arg3, arg4);
}
