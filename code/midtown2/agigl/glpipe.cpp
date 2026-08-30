/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2
    Copyright (C) 2026 The OpenMM2 contributors

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

#include "glpipe.h"

#include "data7/printer.h"
#include "gfx/gfxpipeline.h"

#include "glcontext.h"
#include "glframe.h"
#include "glstub.h"
#include "glscreen.h"
#include "glsurface.h"

define_dummy_symbol(agigl_glpipe);

#ifdef ARTS_AGIGL

// One quad's worth of screen-space geometry. Interleaved rather than three arrays because it is
// built and submitted in the same breath and never persists.
struct agiGLScreenVertex
{
    f32 x;
    f32 y;
    f32 u;
    f32 v;
    u8 rgba[4];
};

check_size(agiGLScreenVertex, 0x14);

static u32 agiGLFramebuffer;
static u32 agiGLRenderbuffers[2];

// Alpha at or below this is discarded when a blit asks for the colour key. Anything above zero
// would do, since the upload writes 0 or 255 and nothing between; 0.5 stays correct if a later
// change makes the key soft.
static constexpr f32 kColorKeyAlphaRef = 0.5f;

static void agiGLUnpackColor(u32 color, u8* rgba)
{
    // MM2 packs colours the D3D way, 0xAARRGGBB in a dword. glColorPointer with GL_UNSIGNED_BYTE
    // reads them in memory order, so they have to be laid out R, G, B, A rather than handed over as
    // the dword - which would come out B, G, R, A on a little-endian machine and turn every red car
    // blue.
    rgba[0] = static_cast<u8>((color >> 16) & 0xFF);
    rgba[1] = static_cast<u8>((color >> 8) & 0xFF);
    rgba[2] = static_cast<u8>(color & 0xFF);
    rgba[3] = static_cast<u8>((color >> 24) & 0xFF);
}

bool agiGLCreateFramebuffer()
{
    agiGLFramebuffer = 0;
    agiGLRenderbuffers[0] = 0;
    agiGLRenderbuffers[1] = 0;

    bool have_fbo = (glGenFramebuffers != nullptr) && (glGenRenderbuffers != nullptr) && (glBlitFramebuffer != nullptr);

    // Rendering straight into the window when the sizes already agree is not an optimisation, it is
    // the removal of a whole blit. Open1560 makes the same call.
    bool needed = (agiScreen.render_width != agiScreen.blit_width) ||
        (agiScreen.render_height != agiScreen.blit_height) || (agiScreen.msaa_level != 0);

    if (!have_fbo || !needed)
    {
        if (!have_fbo && needed)
        {
            Warningf("GL: no framebuffer object support, forcing the render size to the window");

            agiScreen.render_width = agiScreen.blit_width;
            agiScreen.render_height = agiScreen.blit_height;
            agiScreen.msaa_level = 0;
        }

        agiScreen.render_x = agiScreen.blit_x;
        agiScreen.render_y = agiScreen.blit_y;

        glViewport(agiScreen.render_x, agiScreen.render_y, agiScreen.render_width, agiScreen.render_height);

        Displayf(
            "GL: rendering into the default framebuffer at %dx%d", agiScreen.render_width, agiScreen.render_height);

        return true;
    }

    agiScreen.render_x = 0;
    agiScreen.render_y = 0;

    glGenFramebuffers(1, &agiGLFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, agiGLFramebuffer);
    glGenRenderbuffers(2, agiGLRenderbuffers);

    i32 samples = agiScreen.msaa_level;

    if (samples != 0 && glRenderbufferStorageMultisample == nullptr)
    {
        Warningf("GL: no multisample renderbuffers, ignoring -msaa %d", samples);

        samples = 0;
        agiScreen.msaa_level = 0;
    }

    glBindRenderbuffer(GL_RENDERBUFFER, agiGLRenderbuffers[0]);

    if (samples != 0)
        glRenderbufferStorageMultisample(
            GL_RENDERBUFFER, samples, GL_RGBA8, agiScreen.render_width, agiScreen.render_height);
    else
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, agiScreen.render_width, agiScreen.render_height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, agiGLRenderbuffers[0]);

    glBindRenderbuffer(GL_RENDERBUFFER, agiGLRenderbuffers[1]);

    // 24-bit depth, where MM2 asked DirectDraw for 16. gfxPipeline::m_ZDepth existed to pick a
    // DirectDraw Z format from what the card enumerated and has no meaning here; 16-bit depth
    // z-fights visibly across a city block, and there is no reason to reproduce that.
    if (samples != 0)
        glRenderbufferStorageMultisample(
            GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, agiScreen.render_width, agiScreen.render_height);
    else
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, agiScreen.render_width, agiScreen.render_height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, agiGLRenderbuffers[1]);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        Errorf("GL: framebuffer incomplete (0x%08X) at %dx%d, msaa %d", status, agiScreen.render_width,
            agiScreen.render_height, samples);

        agiGLDestroyFramebuffer();

        return false;
    }

    glViewport(0, 0, agiScreen.render_width, agiScreen.render_height);

    Displayf("GL: render framebuffer %dx%d, msaa %d, blitting to %dx%d at %d,%d", agiScreen.render_width,
        agiScreen.render_height, samples, agiScreen.blit_width, agiScreen.blit_height, agiScreen.blit_x,
        agiScreen.blit_y);

    return true;
}

void agiGLDestroyFramebuffer()
{
    if (agiGLFramebuffer != 0)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &agiGLFramebuffer);
        agiGLFramebuffer = 0;
    }

    if (agiGLRenderbuffers[0] != 0 || agiGLRenderbuffers[1] != 0)
    {
        glDeleteRenderbuffers(2, agiGLRenderbuffers);
        agiGLRenderbuffers[0] = 0;
        agiGLRenderbuffers[1] = 0;
    }
}

void agiGLBindRenderTarget()
{
    if (glBindFramebuffer != nullptr)
        glBindFramebuffer(GL_FRAMEBUFFER, agiGLFramebuffer);
}

void agiGLPresent()
{
    if (agiGLFramebuffer != 0)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, agiGLFramebuffer);

        glBlitFramebuffer(agiScreen.render_x, agiScreen.render_y, agiScreen.render_x + agiScreen.render_width,
            agiScreen.render_y + agiScreen.render_height, agiScreen.blit_x, agiScreen.blit_y,
            agiScreen.blit_x + agiScreen.blit_width, agiScreen.blit_y + agiScreen.blit_height, GL_COLOR_BUFFER_BIT,
            agiScreen.blit_filter);
    }

    agiGL->Present();
    agiGL->CheckErrors(true);
}

void agiGLBegin2D()
{
    // The projection is over the LOGICAL resolution, and it has to be.
    //
    // Projecting over the 640x480 authoring space instead looks like the obvious fix for the
    // corner-pinning docs/ui_scaling.md describes, and it was tried: everything came out roughly
    // four times too big. The reason is that this project ALREADY scales the UI, just earlier -
    // gfxBitmap::Create(gfxImage*, bool) resamples through uiScale::PixelsW/PixelsH at LOAD time,
    // so by the time a bitmap reaches a blit its coordinates are in logical space, not design
    // space. Scaling again here scales what is already scaled.
    //
    // What remains unscaled is CSV widget POSITIONS, and that is a fix in the widget loader rather
    // than in the projection - see docs/ui_scaling_progress.md.
    // THE VIEWPORT IS RESET TO THE WHOLE TARGET FIRST, AND THAT IS NOT HOUSEKEEPING.
    //
    // 2D in the original does not go through the Direct3D viewport at all. BltFast and Blt write
    // to the surface directly, so whatever viewport the 3D path last set has no bearing on where a
    // menu bitmap lands. GL has no such separation: glViewport applies to everything drawn, and
    // leaving the last 3D viewport in place squeezes the entire 2D layer into it.
    //
    // That is precisely what happened. The frontend sets a 640x480 viewport, which
    // agiGLFlushViewport scaled to 853x640 at y=800 on a 2560x1440 target, and the whole menu -
    // background and widgets together, correctly proportioned relative to each other - appeared in
    // a small block in the top-left corner with black around it. The loading screen looked right
    // only because it is drawn before anything sets a viewport.
    glViewport(0, 0, agiScreen.render_width, agiScreen.render_height);

    // THE PROJECTION IS OVER THE SPACE THE GAME BELIEVES IT IS DRAWING IN.
    //
    // That is gfxPipeline::m_iWidth/m_iHeight, and it is NOT always agiScreen.width/height. The
    // two disagreed exactly where it showed: in the frontend the game had m_i = 640x480 while the
    // GL layer had logical 1920x1080, so every menu bitmap - authored 640x480 and left unscaled,
    // because uiScale derives its factor from m_iWidth and therefore saw a factor of one - was
    // drawn into a projection three times too wide and landed in the top-left third of the screen.
    //
    // Taking the size from the game closes that loop by construction: whatever resolution it
    // thinks it has, that rectangle is mapped onto the whole window. It is right for the frontend
    // at 640x480, right for gameplay at the chosen mode, and right again if either changes,
    // without the two having to be kept in step by hand.
    const f32 space_w = static_cast<f32>(gfxPipeline::Access_m_iWidth());
    const f32 space_h = static_cast<f32>(gfxPipeline::Access_m_iHeight());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, static_cast<GLdouble>(space_w > 0.0f ? space_w : static_cast<f32>(agiScreen.width)),
        static_cast<GLdouble>(space_h > 0.0f ? space_h : static_cast<f32>(agiScreen.height)), 0.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    agiGL->EnableDisable(GL_DEPTH_TEST, false);
    agiGL->EnableDisable(GL_CULL_FACE, false);
    agiGL->EnableDisable(GL_FOG, false);

    // LIGHTING OFF, and this line is newer than the three above it for a reason worth recording.
    //
    // 2D is unlit by definition: it stands in for BltFast, which copied pixels and knew nothing
    // about lights. It was safe to omit only while nothing ever enabled GL_LIGHTING - and nothing
    // did, because the shim device's SetRenderState was a no-op. The moment that started
    // translating D3DRENDERSTATE_LIGHTING, agiGLEnd2D began restoring it and every menu quad drawn
    // afterwards was lit by whatever lights the world had left set.
    //
    // The general rule this is the fourth instance of: anything agiGLEnd2D restores for 3D must
    // have a matching disable here, or the two paths fight over it a frame at a time.
    agiGL->EnableDisable(GL_LIGHTING, false);

    agiGL->DepthMask(false);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void agiGLEnd2D()
{
    // Put the 3D transform back. Begin2D clobbered both matrices and the HUD draws every frame, so
    // without this the first 2D draw of a frame leaves world geometry projected into screen-ortho
    // space for the rest of it.
    agiGLRestore3DMatrices();
    agiGLApplyRenderState();

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    agiGL->EnableDisable(GL_ALPHA_TEST, false);

    // THE DEPTH MASK IS NOT FORCED ON HERE ANY MORE.
    //
    // This used to end with DepthMask(true), two lines after agiGLApplyRenderState had already
    // restored the game's own write mask - so it overrode it, on every 2D pass, and the HUD draws
    // one every frame.
    //
    // lvlSky draws the sky with ZWriteEnable deliberately OFF, because everything is behind it.
    // Forcing writes back on let the sky stamp depth across the whole screen and occlude the city,
    // which showed as the world flickering in and out along the bottom of the view.
    //
    // agiGLApplyRenderState above is what sets the mask, and it uses the state the game asked for.
}

void agiGLDrawQuad2D(const agiGLSurface* surface, f32 dst_x, f32 dst_y, f32 dst_width, f32 dst_height, f32 src_u0,
    f32 src_v0, f32 src_u1, f32 src_v1, u32 color, bool alpha_test)
{
    // DIAGNOSTIC: every distinct 2D quad, with the space the game thinks it is drawing in.
    //
    // The speedometer digits do NOT come through gfxPipeline::CopyBitmap in gameplay - a whole race
    // produced exactly one blit there, the full-screen one - so the probe has to sit at the sink
    // every 2D draw passes through instead. mmHUD lays the digits out as
    // m_iWidth/2 - 3*digitWidth - colonWidth and then advances by each bitmap's own width, so the
    // numbers that matter are the destination x and the drawn size against m_iWidth.
    {
        static u32 seen[40] = {};
        static i32 count = 0;

        const u32 key = (static_cast<u32>(static_cast<i32>(dst_width)) << 16) |
            (static_cast<u32>(static_cast<i32>(dst_height)) & 0xFFFFu);

        bool known = false;

        for (i32 k = 0; k < count; ++k)
        {
            if (seen[k] == key)
                known = true;
        }

        // The speedometer sits in the bottom-left of the screenshot and never appeared in this
        // probe, because deduping by SIZE alone hides any draw whose dimensions another element
        // already claimed - and reports the first one's position, not this one's. Anything in that
        // corner is logged unconditionally, which is the only way to see a thing whose size is not
        // unique.
        // The HUD starts OFF and is toggled with H, so earlier captures had nothing to record. The
        // filter is also widened from "bottom-left corner" to "digit-shaped, anywhere": the digits
        // may not be where the old screenshot showed them, since the viewport handling has changed
        // since it was taken. A digit at 4x the authored size is roughly 100-500 wide and 100-300
        // tall, which excludes the full-screen quads and the small bars without assuming a position.
        const bool digit_shaped = dst_width >= 60.0f && dst_width <= 600.0f && dst_height >= 80.0f &&
            dst_height <= 320.0f && surface != nullptr;

        const bool bottom_left = digit_shaped;

        if (bottom_left && agiGLProbeVerbose())
        {
            static i32 shown = 0;

            if (shown < 60)
            {
                ++shown;

                Displayf("HUDQUAD dst=%.0f,%.0f size=%.0fx%.0f tex=%d", static_cast<f64>(dst_x),
                    static_cast<f64>(dst_y), static_cast<f64>(dst_width), static_cast<f64>(dst_height),
                    surface != nullptr ? 1 : 0);
            }
        }

        if (!known && count < 40 && agiGLProbeVerbose())
        {
            seen[count++] = key;

            Displayf("QUAD2D dst=%.0f,%.0f size=%.0fx%.0f tex=%d space=%dx%d", static_cast<f64>(dst_x),
                static_cast<f64>(dst_y), static_cast<f64>(dst_width), static_cast<f64>(dst_height),
                surface != nullptr ? 1 : 0, gfxPipeline::Access_m_iWidth(), gfxPipeline::Access_m_iHeight());
        }
    }

    agiGLScreenVertex verts[4];

    verts[0].x = dst_x;
    verts[0].y = dst_y;
    verts[0].u = src_u0;
    verts[0].v = src_v0;

    verts[1].x = dst_x + dst_width;
    verts[1].y = dst_y;
    verts[1].u = src_u1;
    verts[1].v = src_v0;

    verts[2].x = dst_x;
    verts[2].y = dst_y + dst_height;
    verts[2].u = src_u0;
    verts[2].v = src_v1;

    verts[3].x = dst_x + dst_width;
    verts[3].y = dst_y + dst_height;
    verts[3].u = src_u1;
    verts[3].v = src_v1;

    for (i32 i = 0; i < 4; ++i)
        agiGLUnpackColor(color, verts[i].rgba);

    if (surface != nullptr)
    {
        agiGL->EnableDisable(GL_TEXTURE_2D, true);
        agiGL->BindTexture2D(surface->Texture);

        // Point sampling, matching BltFast. A bitmap the ortho projection is now scaling up would
        // look better filtered, but the menu art has hard one-pixel borders that bleed under a
        // linear filter, and MM2's own answer - decode the bitmap at the target size - is the
        // better one. See docs/ui_scaling.md.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        agiGL->EnableDisable(GL_TEXTURE_2D, false);
    }

    if (alpha_test)
    {
        agiGL->EnableDisable(GL_ALPHA_TEST, true);
        glAlphaFunc(GL_GREATER, kColorKeyAlphaRef);
    }
    else
    {
        agiGL->EnableDisable(GL_ALPHA_TEST, false);
    }

    // Blending on for a translucent colour, off otherwise. The fade in EndFrame is the only caller
    // that passes a partial alpha, and it is the same special case gfxPipeline::Clear makes when it
    // draws a full-screen quad instead of calling the device Clear.
    bool blend = ((color >> 24) & 0xFF) != 0xFF;

    agiGL->EnableDisable(GL_BLEND, blend);

    if (blend)
        agiGL->BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glVertexPointer(2, GL_FLOAT, sizeof(agiGLScreenVertex), &verts[0].x);
    glTexCoordPointer(2, GL_FLOAT, sizeof(agiGLScreenVertex), &verts[0].u);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(agiGLScreenVertex), &verts[0].rgba[0]);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

#endif
