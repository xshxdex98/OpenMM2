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

#pragma once

#include "glcommon.h"

// Number of GL capability toggles the EnableDisable cache tracks. Must stay in sync with
// GetCapabilityIndex() in glcontext.cpp - the index subscripts capabilities_.
// Must equal CAP_COUNT. It did not: CAP_LIGHTING was added as the ninth index while this still
// said 8, so every EnableDisable(GL_LIGHTING, ...) read and wrote one past the end of
// capabilities_. A static_assert in glcontext.cpp now ties the two together so the next capability
// added fails the build instead of corrupting the object.
static constexpr usize kNumTrackedCaps = 22;

// The GL context, bound to the window gfxPipeline::gfxWindowCreate already made.
//
// THIS IS WHERE OpenMM2 DIVERGES FROM Open1560 MOST VISIBLY. Open1560 replaced Midtown Madness 1's
// window with an SDL one and took the context from SDL_GL_CreateContext. MM2 cannot follow. Its
// window is created by gfxWindowCreate, its messages run through gfxPipeline::gfxWindowProc, and
// that WindowProc forwards nearly everything to InputWindowProc - the entry to the whole
// DirectInput path (ioInput, ioMouse, ioKeyboard, ioJoystick). Swapping in SDL would mean porting
// input at the same time as the renderer, and neither half would be independently testable.
//
// So MM2 keeps its HWND and gets the context through WGL. The rest of this class is the same idea
// as Open1560's agiGLContext: cache the driver state the render-state flusher touches on every
// primitive, because a redundant glEnable is still a driver call and there are thousands a frame.
class agiGLContext
{
public:
    agiGLContext(HWND window, i32 debug_level);
    ~agiGLContext();

    ARTS_NON_COPYABLE(agiGLContext);

    void ActiveTexture(u32 unit);
    void BindTexture2D(u32 texture);
    void BlendFunc(u32 sfactor, u32 dfactor);
    void CheckErrors(bool lazy = false);
    void DepthFunc(u32 func);
    void DepthMask(bool enable);
    void EnableDisable(u32 cap, bool enable);

    // Re-issues every cached toggle unconditionally. Needed after anything that changes GL behind
    // the cache's back - Open1560 hit exactly this with its rear-view mirror pass, where GL_BLEND
    // was left disabled while the cache still believed it on, so EnableDisable never re-issued the
    // glEnable. MM2 has the same shape of problem in the mirror and in mmDashView.
    void ForceApplyCaps();

    void FrontFace(u32 face);
    bool HasExtension(const char* name) const;
    bool IsCurrent() const;
    void MakeCurrent();
    void PolygonMode(u32 mode);
    void Present();
    void SetSwapInterval(i32 interval);

    i32 GetVersion() const
    {
        return gl_version_;
    }

    bool HasVersion(i32 version) const
    {
        return gl_version_ >= version;
    }

    // True when the fixed-function pipeline is available. MM2's renderer is fixed function all the
    // way down - gfxRenderState maps one to one onto D3DRENDERSTATE_* - so a core-profile context
    // would mean writing shaders for every state combination before a single frame appeared.
    // agigl therefore asks for a compatibility context and complains loudly without one.
    bool IsLegacyCompat() const
    {
        return legacy_compat_;
    }

    i32 GetMaxAnisotropy() const
    {
        return max_anisotropy_;
    }

    bool IsValid() const
    {
        return gl_context_ != nullptr;
    }

private:
    void InitState();
    void InitVersioning();

    HWND window_ {};
    HDC device_context_ {};
    HGLRC gl_context_ {};

    i32 gl_version_ {};
    i32 debug_level_ {};
    i32 error_count_ {};
    i32 max_anisotropy_ {};
    i32 num_extensions_ {};

    // Only valid before 3.0, where the whole list comes back as one space-separated string. From
    // 3.0 the query is indexed and HasExtension walks it with glGetStringi instead.
    const char* extension_string_ {};

    bool legacy_compat_ {};

    u32 active_texture_unit_ {};
    u32 bound_texture_ {};
    bool capabilities_[kNumTrackedCaps] {};

public:
    // TEMPORARY: read-only views for the draw-state probe in glframe.cpp.
    bool ProbeCap(u32 cap);

    u32 ProbeDepthFunc() const
    {
        return depth_func_;
    }

    bool ProbeDepthMask() const
    {
        return depth_mask_;
    }

    u32 ProbeFrontFace() const
    {
        return front_face_;
    }

    u32 ProbeBlendSrc() const
    {
        return blend_func_s_;
    }

    u32 ProbeBlendDst() const
    {
        return blend_func_d_;
    }

private:

    bool depth_mask_ {};
    u32 polygon_mode_ {};
    u32 depth_func_ {};
    u32 front_face_ {};
    u32 blend_func_s_ {};
    u32 blend_func_d_ {};
};

// The one live context. A global rather than a member because the reimplemented gfxPipeline is all
// static - MM2 has no pipeline object to hang it off, unlike Open1560's agiPipeline instance.
extern agiGLContext* agiGL;
