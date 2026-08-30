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

#include "glcontext.h"

#include "data7/printer.h"

define_dummy_symbol(agigl_glcontext);

#ifdef ARTS_AGIGL

// GetDC and ReleaseDC are the only two Win32 calls agigl links rather than resolves, because they
// are the only two midtown2.exe already imports. Everything else goes through GetProcAddress; see
// the note at the top of glcommon.h.
extern "C" __declspec(dllimport) HDC ARTS_GLAPI GetDC(HWND window);
extern "C" __declspec(dllimport) i32 ARTS_GLAPI ReleaseDC(HWND window, HDC device_context);

// Stop reporting GL errors after this many, so a per-primitive mistake cannot fill the log.
static constexpr i32 kMaxGLErrors = 100;

// Colour and depth bits asked of ChoosePixelFormat. 24-bit depth where MM2 asks DirectDraw for 16:
// gfxPipeline::m_ZDepth exists to pick a DirectDraw Z format and has no meaning once the depth
// buffer is a renderbuffer, and 16-bit depth z-fights visibly across a city block.
static constexpr u8 kColorBits = 32;
static constexpr u8 kDepthBits = 24;

// Indices into agiGLContext::capabilities_. Order must match GetCapabilityIndex and
// kNumTrackedCaps in glcontext.h.
enum agiGLCapIndex : u32
{
    CAP_BLEND = 0,
    CAP_CULL_FACE,
    CAP_DEPTH_TEST,
    CAP_SCISSOR_TEST,
    CAP_ALPHA_TEST,
    CAP_FOG,
    CAP_TEXTURE_2D,

    // GL_COLOR_SUM, for D3DFVF_SPECULAR. The packet draw path toggles it per packet, which is why
    // it is cached rather than issued raw - see agigl/glpacket.cpp.
    CAP_COLOR_SUM,

    // GL_LIGHTING. Tracked because gfxRenderState toggles D3DRENDERSTATE_LIGHTING per material
    // and the shim device now translates it - without an entry here EnableDisable rejected it
    // outright and killed the process with "invalid capability B50".
    CAP_LIGHTING,

    // GL_COLOR_MATERIAL and GL_NORMALIZE, both toggled per draw by the lighting translation in
    // agigl/gllight.cpp.
    CAP_COLOR_MATERIAL,
    CAP_NORMALIZE,

    // GL_LIGHT0 through GL_LIGHT7. D3D7 addresses lights by index and MM2 enables and disables
    // them per object, so these are toggled far more often than anything else here.
    CAP_LIGHT0,
    CAP_LIGHT1,
    CAP_LIGHT2,
    CAP_LIGHT3,
    CAP_LIGHT4,
    CAP_LIGHT5,
    CAP_LIGHT6,
    CAP_LIGHT7,

    // GL_TEXTURE_GEN_S/T/R, toggled per draw by the environment-map translation.
    CAP_TEXTURE_GEN_S,
    CAP_TEXTURE_GEN_T,
    CAP_TEXTURE_GEN_R,

    CAP_COUNT
};

static_assert(CAP_COUNT == kNumTrackedCaps, "kNumTrackedCaps must match CAP_COUNT");

agiGLContext* agiGL;

// The CRT is the game's own, statically linked and reproduced in game.asm, and the link runs
// -NODEFAULTLIB. Anything the modern headers lower to a ucrt-only helper - sscanf is the obvious
// one, since <cstdio> turns it into __stdio_common_vsscanf - has no definition to resolve against.
// So the two string operations agigl needs are written out here rather than included.
static bool agiTokenEquals(const char* text, const char* name, usize length)
{
    for (usize i = 0; i < length; ++i)
    {
        if (text[i] != name[i])
            return false;
    }

    return true;
}

static usize agiTextLength(const char* text)
{
    usize length = 0;

    while (text[length] != '\0')
        ++length;

    return length;
}

// Reads the leading "major.minor" out of a GL_VERSION string and returns major*100 + minor*10, the
// encoding Open1560 uses. Returns 0 when the string does not start with a digit, which is how a
// driver that answered with something unparsable is told apart from a 1.0 driver.
static i32 agiParseGLVersion(const char* version)
{
    if (version == nullptr || version[0] < '0' || version[0] > '9')
        return 0;

    i32 major = 0;
    i32 minor = 0;
    const char* c = version;

    for (; *c >= '0' && *c <= '9'; ++c)
        major = (major * 10) + (*c - '0');

    if (*c == '.')
    {
        for (++c; *c >= '0' && *c <= '9'; ++c)
            minor = (minor * 10) + (*c - '0');
    }

    return (major * 100) + (minor * 10);
}

static u32 GetCapabilityIndex(u32 cap)
{
    // GL_LIGHT0..GL_LIGHT7 are consecutive and so are CAP_LIGHT0..CAP_LIGHT7, so the eight fold
    // into one test rather than eight cases.
    if (cap >= GL_LIGHT0 && cap < GL_LIGHT0 + 8)
        return CAP_LIGHT0 + (cap - GL_LIGHT0);

    switch (cap)
    {
        case GL_BLEND: return CAP_BLEND;
        case GL_CULL_FACE: return CAP_CULL_FACE;
        case GL_DEPTH_TEST: return CAP_DEPTH_TEST;
        case GL_SCISSOR_TEST: return CAP_SCISSOR_TEST;
        case GL_ALPHA_TEST: return CAP_ALPHA_TEST;
        case GL_FOG: return CAP_FOG;
        case GL_TEXTURE_2D: return CAP_TEXTURE_2D;
        case GL_COLOR_SUM: return CAP_COLOR_SUM;
        case GL_LIGHTING: return CAP_LIGHTING;
        case GL_COLOR_MATERIAL: return CAP_COLOR_MATERIAL;
        case GL_NORMALIZE: return CAP_NORMALIZE;
        case GL_TEXTURE_GEN_S: return CAP_TEXTURE_GEN_S;
        case GL_TEXTURE_GEN_T: return CAP_TEXTURE_GEN_T;
        case GL_TEXTURE_GEN_R: return CAP_TEXTURE_GEN_R;

        default: Quitf("agiGLContext: invalid capability %X", cap);
    }

    return CAP_BLEND;
}

static const char* GetGLErrorName(u32 error)
{
    switch (error)
    {
        case GL_INVALID_ENUM: return "Invalid Enum";
        case GL_INVALID_VALUE: return "Invalid Value";
        case GL_INVALID_OPERATION: return "Invalid Operation";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid Framebuffer Operation";
        case GL_OUT_OF_MEMORY: return "Out Of Memory";
        case GL_STACK_UNDERFLOW: return "Stack Underflow";
        case GL_STACK_OVERFLOW: return "Stack Overflow";
        default: return "Unknown";
    }
}

agiGLContext::agiGLContext(HWND window, i32 debug_level)
    : window_(window)
    , debug_level_(debug_level)
{
    device_context_ = GetDC(window_);

    if (device_context_ == nullptr)
    {
        Errorf("agiGLContext: GetDC failed");

        return;
    }

    // A plain ChoosePixelFormat rather than wglChoosePixelFormatARB. The ARB version is what MSAA
    // on the default framebuffer would need, but agigl does not render into the default
    // framebuffer - it renders into an FBO and blits, exactly as docs/resolution_blueprint.md
    // describes - so multisampling is a renderbuffer property and never a pixel format one.
    agiPixelFormatDescriptor pfd {};

    pfd.nSize = sizeof(agiPixelFormatDescriptor);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = kColorBits;
    pfd.cDepthBits = kDepthBits;
    pfd.iLayerType = PFD_MAIN_PLANE;

    i32 format = agiChoosePixelFormat(device_context_, &pfd);

    if (format == 0)
    {
        Errorf("agiGLContext: ChoosePixelFormat found no %d-bit RGBA format", kColorBits);

        return;
    }

    // SetPixelFormat can only ever be called once per window. If MM2 restarts the pipeline - and
    // SafeBeginGfx does exactly that, up to four times - the second call fails, so the window has
    // to be destroyed and remade in between. gfxPipeline::EndGfx2D already destroys it.
    if (!agiSetPixelFormat(device_context_, format, &pfd))
    {
        Errorf("agiGLContext: SetPixelFormat(%d) failed - was this window already set up?", format);

        return;
    }

    gl_context_ = wglCreateContext(device_context_);

    if (gl_context_ == nullptr)
    {
        Errorf("agiGLContext: wglCreateContext failed");

        return;
    }

    MakeCurrent();
    agiGLLoadExtensions();
    InitVersioning();

    // WHAT THE DRIVER ACTUALLY GAVE US, not what we asked for.
    //
    // ChoosePixelFormat picks the NEAREST available format, not an exact match, and it will happily
    // return one with fewer depth bits - or none at all - without failing. A city-sized scene has a
    // huge near-to-far ratio, so depth precision is the difference between a solid world and one
    // that shimmers as geometry wins and loses the depth test from frame to frame. That is
    // indistinguishable from "flicker" on screen, so the granted depth size is worth stating plainly
    // rather than assuming it matches kDepthBits.
    {
        // Asked of GL itself rather than of DescribePixelFormat: this reports the depth buffer the
        // context is actually rendering into, which is the number that matters.
        GLint depth_bits = 0;
        GLint stencil_bits = 0;

        glGetIntegerv(0x0D56 /* GL_DEPTH_BITS */, &depth_bits);
        glGetIntegerv(0x0D57 /* GL_STENCIL_BITS */, &stencil_bits);

        Displayf("agiGLContext: format %d asked depth=%d, GL reports depth=%d stencil=%d", format,
            kDepthBits, depth_bits, stencil_bits);
    }
    InitState();
}

agiGLContext::~agiGLContext()
{
    if (gl_context_ != nullptr)
    {
        wglMakeCurrent(device_context_, nullptr);
        wglDeleteContext(gl_context_);
        gl_context_ = nullptr;
    }

    if (device_context_ != nullptr)
    {
        ReleaseDC(window_, device_context_);
        device_context_ = nullptr;
    }

    if (agiGL == this)
        agiGL = nullptr;
}

void agiGLContext::ActiveTexture(u32 unit)
{
    if (unit == active_texture_unit_)
        return;

    active_texture_unit_ = unit;

    // Null before 1.3. A driver that old has one texture unit, so unit is always 0 and there is
    // nothing to select - hence a silent return rather than an error.
    if (glActiveTexture != nullptr)
        glActiveTexture(GL_TEXTURE0 + unit);
}

void agiGLContext::BindTexture2D(u32 texture)
{
    if (texture == bound_texture_)
        return;

    bound_texture_ = texture;
    glBindTexture(GL_TEXTURE_2D, texture);
}

void agiGLContext::BlendFunc(u32 sfactor, u32 dfactor)
{
    if (blend_func_s_ == sfactor && blend_func_d_ == dfactor)
        return;

    blend_func_s_ = sfactor;
    blend_func_d_ = dfactor;
    glBlendFunc(sfactor, dfactor);
}

void agiGLContext::CheckErrors(bool lazy)
{
    if (error_count_ < (lazy ? 1 : 0))
        return;

    if (error_count_ >= kMaxGLErrors)
        return;

    for (u32 error = glGetError(); error != GL_NO_ERROR; error = glGetError())
    {
        Errorf("GL Error: %s (%08X)", GetGLErrorName(error), error);

        if (++error_count_ >= kMaxGLErrors)
        {
            Errorf("GL: too many errors, no longer reporting");

            break;
        }
    }
}

void agiGLContext::DepthFunc(u32 func)
{
    if (depth_func_ == func)
        return;

    depth_func_ = func;
    glDepthFunc(func);
}

void agiGLContext::DepthMask(bool enable)
{
    if (depth_mask_ == enable)
        return;

    depth_mask_ = enable;
    glDepthMask(enable ? 1 : 0);
}

bool agiGLContext::ProbeCap(u32 cap)
{
    return capabilities_[GetCapabilityIndex(cap)];
}

void agiGLContext::EnableDisable(u32 cap, bool enable)
{
    bool& enabled = capabilities_[GetCapabilityIndex(cap)];

    if (enabled == enable)
        return;

    enabled = enable;

    if (enable)
        glEnable(cap);
    else
        glDisable(cap);
}

void agiGLContext::ForceApplyCaps()
{
    // One entry per agiGLCapIndex, in that order. GL_COLOR_SUM is core since 1.4, which agigl
    // already requires elsewhere; on anything older both the enable and the disable would raise
    // GL_INVALID_ENUM and CheckErrors would say so every frame.
    // ONE ENTRY PER agiGLCapIndex, AND THE COUNT IS CHECKED. This list used to stop at GL_LIGHTING
    // while the enum ran on through GL_COLOR_MATERIAL, GL_NORMALIZE, the eight lights and the three
    // texgen toggles. A short brace-initialised array is zero-filled rather than rejected, so every
    // index past the end asked GL to enable capability 0 - a stream of GL_INVALID_ENUM, and none of
    // those toggles restored, which is precisely the state this function exists to rebuild.
    //
    // It went unnoticed because nothing called ForceApplyCaps until the viewport-change fix in
    // agigl/glframe.cpp; the bug was latent in code written for a caller that did not exist yet.
    static constexpr u32 caps[CAP_COUNT] {GL_BLEND, GL_CULL_FACE, GL_DEPTH_TEST, GL_SCISSOR_TEST, GL_ALPHA_TEST,
        GL_FOG, GL_TEXTURE_2D, GL_COLOR_SUM, GL_LIGHTING, GL_COLOR_MATERIAL, GL_NORMALIZE, GL_LIGHT0, GL_LIGHT0 + 1,
        GL_LIGHT0 + 2, GL_LIGHT0 + 3, GL_LIGHT0 + 4, GL_LIGHT0 + 5, GL_LIGHT0 + 6, GL_LIGHT0 + 7, GL_TEXTURE_GEN_S,
        GL_TEXTURE_GEN_T, GL_TEXTURE_GEN_R};

    // Catches the next truncation at compile time instead of as invalid-enum spam at runtime. A
    // brace-initialised array that is too short is zero-filled rather than rejected, so the last
    // entry being non-zero is what says the list actually reaches the end of the enum.
    static_assert(caps[CAP_COUNT - 1] != 0, "caps[] is shorter than agiGLCapIndex");

    for (u32 i = 0; i < CAP_COUNT; ++i)
    {
        if (capabilities_[i])
            glEnable(caps[i]);
        else
            glDisable(caps[i]);
    }

    // The non-toggle caches have the same hazard and need the same treatment. A stuck depth-write
    // mask is the one that reads as a rendering bug rather than a state bug: geometry goes
    // see-through and nothing in the draw path is wrong.
    glDepthMask(depth_mask_ ? 1 : 0);
    glDepthFunc(depth_func_);
    glFrontFace(front_face_);
    glBlendFunc(blend_func_s_, blend_func_d_);
    glPolygonMode(GL_FRONT_AND_BACK, polygon_mode_);
    glBindTexture(GL_TEXTURE_2D, bound_texture_);
}

void agiGLContext::FrontFace(u32 face)
{
    if (front_face_ == face)
        return;

    front_face_ = face;
    glFrontFace(face);
}

bool agiGLContext::HasExtension(const char* name) const
{
    usize length = agiTextLength(name);

    // From 3.0 the monolithic string is deprecated and returns null on a core profile, so the
    // indexed query is the only one that works. Both forms are kept because MM2 must still start
    // on a 1.x software fallback, where glGetStringi does not exist.
    if (glGetStringi != nullptr && num_extensions_ != 0)
    {
        for (i32 i = 0; i < num_extensions_; ++i)
        {
            const char* ext = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(i)));

            if (ext != nullptr && agiTokenEquals(ext, name, length) && ext[length] == '\0')
                return true;
        }

        return false;
    }

    if (extension_string_ == nullptr)
        return false;

    for (const char* c = extension_string_; *c != '\0';)
    {
        // A whole-token match. A plain substring search would report GL_ARB_framebuffer_object
        // present because GL_ARB_framebuffer_object_ARB is - the classic way this check goes wrong.
        if (agiTokenEquals(c, name, length) && (c[length] == ' ' || c[length] == '\0'))
            return true;

        while (*c != ' ' && *c != '\0')
            ++c;

        while (*c == ' ')
            ++c;
    }

    return false;
}

void agiGLContext::InitState()
{
    error_count_ = (debug_level_ < 0) ? -1 : ((debug_level_ > 0) ? 1 : 0);

    max_anisotropy_ = 0;

    if (HasExtension("GL_EXT_texture_filter_anisotropic"))
    {
        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy_);
        Displayf("GL: max anisotropy %d", max_anisotropy_);
    }

    // The cache starts holding GL's documented initial values, not ours. Every entry here is the
    // spec default, so the first EnableDisable of each cap is correctly elided rather than
    // wrongly.
    for (u32 i = 0; i < CAP_COUNT; ++i)
        capabilities_[i] = false;

    depth_mask_ = true;
    polygon_mode_ = GL_FILL;
    depth_func_ = GL_LESS;
    front_face_ = GL_CCW;
    blend_func_s_ = GL_ONE;
    blend_func_d_ = GL_ZERO;
    bound_texture_ = 0;
    active_texture_unit_ = 0;

    CheckErrors();
}

void agiGLContext::InitVersioning()
{
    const char* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    gl_version_ = agiParseGLVersion(version);

    if (gl_version_ == 0)
    {
        Errorf("GL: could not parse version string '%s'", (version != nullptr) ? version : "(null)");

        gl_version_ = 100;
    }

    Displayf("OpenGL Version: %s", version);
    Displayf("OpenGL Vendor: %s", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    Displayf("OpenGL Renderer: %s", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

    legacy_compat_ = true;
    num_extensions_ = 0;
    extension_string_ = nullptr;

    if (HasVersion(300))
    {
        i32 major = 0;
        i32 minor = 0;

        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        gl_version_ = (major * 100) + (minor * 10);

        glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions_);

        if (HasVersion(320))
        {
            i32 profile_mask = 0;

            glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile_mask);
            legacy_compat_ = (profile_mask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) != 0;
        }
    }
    else
    {
        extension_string_ = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
    }

    // wglCreateContext with no attribute list gives the highest compatibility profile the driver
    // supports, so this should never fire. It is here because if it ever does, every fixed
    // function call draws nothing and reports nothing, and that is a very expensive silence.
    if (!legacy_compat_)
        Errorf("GL: got a core profile context. MM2's renderer is fixed function and needs compatibility.");

    Displayf("OpenGL %d %s, %d extensions", gl_version_, legacy_compat_ ? "Compatibility" : "Core", num_extensions_);
}

bool agiGLContext::IsCurrent() const
{
    return wglGetCurrentContext() == gl_context_;
}

void agiGLContext::MakeCurrent()
{
    if (!IsCurrent())
        wglMakeCurrent(device_context_, gl_context_);

    agiGL = this;
}

void agiGLContext::PolygonMode(u32 mode)
{
    if (polygon_mode_ == mode)
        return;

    polygon_mode_ = mode;
    glPolygonMode(GL_FRONT_AND_BACK, mode);
}

void agiGLContext::Present()
{
    agiSwapBuffers(device_context_);
}

void agiGLContext::SetSwapInterval(i32 interval)
{
    if (wglSwapIntervalEXT != nullptr)
        wglSwapIntervalEXT(interval);
    else
        Warningf("GL: WGL_EXT_swap_control missing, vsync is whatever the driver decides");
}

#endif
