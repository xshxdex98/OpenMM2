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

#include "glscreen.h"

#include "gfx/gfxpipeline.h"

#include "data7/datargparser.h"
#include "data7/printer.h"
#include "gfx/gfxinterface.h"

define_dummy_symbol(agigl_glscreen);

#ifdef ARTS_AGIGL

// Both are in build/implibs/USER32.def, so these link rather than needing GetProcAddress.
//
// NOT available there, and worth knowing before reaching for them: EnumDisplayMonitors,
// GetMonitorInfoA and EnumDisplayDevicesA. midtown2.exe imports none of the three, so real
// multi-monitor support means adding them to the .def - see docs/opengl_plan.md, "What is
// blocked". Until then the synthesised list describes the primary display only.
extern "C" __declspec(dllimport) i32 ARTS_GLAPI GetSystemMetrics(i32 index);

struct agiRect
{
    i32 left;
    i32 top;
    i32 right;
    i32 bottom;
};

extern "C" __declspec(dllimport) i32 ARTS_GLAPI GetClientRect(HWND window, agiRect* rect);

// GetSystemMetrics indices, from winuser.h.
static constexpr i32 SM_CXSCREEN = 0;
static constexpr i32 SM_CYSCREEN = 1;

// The floor MM2 itself enforces. gfxMinScreenWidth/Height come from the localisation resource and
// ResCallback already rejects anything below them, so the synthesised list must respect the same
// floor or a Japanese build will list modes its own UI cannot draw in.
static constexpr i32 kFallbackMinWidth = 640;
static constexpr i32 kFallbackMinHeight = 480;

// What to fall back to with no display information at all - the same pair SafeBeginGfx retries
// with.
static constexpr i32 kDefaultWidth = 640;
static constexpr i32 kDefaultHeight = 480;

// Advertised on every synthesised mode. Used only by the video-memory comparisons in AutoDetect,
// which pick a quality preset, so it has to be large enough to land in the top bracket and is
// otherwise meaningless once DirectDraw is gone.
static constexpr u32 kSynthesizedVideoMemory = 128u << 20;

// Colour depth written into every synthesised mode. The framebuffer is RGBA8 whatever this says;
// the field exists because gfxPipeline::m_ColorDepth is read in a dozen places and 32 is the
// truthful answer.
static constexpr u16 kSynthesizedColorDepth = 32;

// Upper bound on the integer-downscale walk (height = native / i). A guard against a divisor that
// never reaches the minimum height, not a real limit.
static constexpr i32 kMaxIntegerScaleSteps = 100;

// Heights at or above this are remembered as the base the small-ratio table works from.
static constexpr i32 kSmallHeightThreshold = 1024;

agiGLScreen agiScreen;

// Values map to -scaling.
enum class agiScalingMode : i32
{
    StretchKeepAspect = 0, // Stretched, aspect preserved. The default.
    Stretch = 1, // Stretched to the whole window, aspect ignored
    Centered = 2, // 1:1, centred, no scaling
    CenteredInteger = 3, // Centred at the largest integer scale that fits
};

static i32 agiGreatestCommonDivisor(i32 a, i32 b)
{
    while (b != 0)
    {
        i32 t = a % b;

        a = b;
        b = t;
    }

    return a;
}

// datArgParser::Get is the game's own, still original machine code. The middle parameter is the
// index of the value after the flag; every call site in the binary that reads a single value
// passes 0.
static i32 agiGetArgInt(const char* name, i32 fallback)
{
    i32 value = 0;

    if (datArgParser::Get(name, 0, value))
        return value;

    return fallback;
}

static bool agiGetArgFlag(const char* name)
{
    return datArgParser::Get(name);
}

static void agiSortResolutions(gfxInterface& info)
{
    // An insertion sort rather than std::sort, and by hand rather than through <algorithm>. The
    // list is at most 64 entries so the complexity does not matter, and the link runs
    // -NODEFAULTLIB against the game's own 1999 CRT - the fewer modern library symbols the port
    // pulls in, the fewer chances there are to find out at link time that one of them is missing.
    for (u32 i = 1; i < info.ResolutionCount; ++i)
    {
        gfxResData key = info.Resolutions[i];
        u32 j = i;

        while (j > 0)
        {
            const gfxResData& prev = info.Resolutions[j - 1];

            bool after = (prev.ScreenHeight != key.ScreenHeight) ? (prev.ScreenHeight > key.ScreenHeight)
                                                                 : (prev.ScreenWidth > key.ScreenWidth);

            if (!after)
                break;

            info.Resolutions[j] = prev;
            --j;
        }

        info.Resolutions[j] = key;
    }
}

void agiGLSynthesizeModes()
{
    i32 desktop_width = GetSystemMetrics(SM_CXSCREEN);
    i32 desktop_height = GetSystemMetrics(SM_CYSCREEN);

    // GetSystemMetrics reports the VIRTUALISED desktop size unless the process is DPI aware, and
    // midtown2.exe is not - there is no manifest, and SetProcessDPIAware is not in its import
    // table. On a 2560x1440 display at 150% scaling this returns 1707x960, so the synthesised list
    // tops out below native and the 1440p goal is quietly missed. See docs/opengl_plan.md,
    // "What is blocked" - the fix is a manifest, not code.
    if (desktop_width <= 0 || desktop_height <= 0)
    {
        Errorf("GL: GetSystemMetrics returned %dx%d, falling back to %dx%d", desktop_width, desktop_height,
            kDefaultWidth, kDefaultHeight);

        desktop_width = kDefaultWidth;
        desktop_height = kDefaultHeight;
    }

    Displayf("GL: desktop is %dx%d", desktop_width, desktop_height);

    i32 min_width = (gfxMinScreenWidth > 0) ? gfxMinScreenWidth : kFallbackMinWidth;
    i32 min_height = (gfxMinScreenHeight > 0) ? gfxMinScreenHeight : kFallbackMinHeight;

    // The cap docs/resolution.md located lives in AutoDetectCallback, which is not on this path any
    // more. Set it from the desktop instead, so nothing downstream that still reads it rejects a
    // mode this function just advertised.
    gfxMaxScreenWidth = desktop_width;
    gfxMaxScreenHeight = desktop_height;

    gfxInterface& info = gfxInterfaces[0];

    for (usize i = 0; i < sizeof(gfxInterface); ++i)
        reinterpret_cast<u8*>(&info)[i] = 0;

    // The GUID is left all zeroes. gfxAutoDetect compares the first 16 bytes of a cached entry
    // against a freshly enumerated one and forces a redetect on a mismatch; with no DirectDraw
    // adapter there is no GUID to report, and a constant one means the comparison always agrees.
    static const char kName[] = "OpenGL";

    for (usize i = 0; i < sizeof(kName); ++i)
        info.Name[i] = kName[i];

    info.DeviceCaps = 3; // Hardware T&L - what DeviceCallback writes for a D3DDEVCAPS_HWTRANSFORMANDLIGHT device.
    info.Renderer = HardwareWithTnL;
    info.AcceptableDepths = static_cast<mmColorDepthFlag>(Depth32 | Depth24 | Depth16);
    info.AvailableMemory = kSynthesizedVideoMemory;
    info.ResolutionCount = 0;
    info.ResolutionChoice = 0;

    const auto add_resolution = [&](i32 width, i32 height) {
        // Odd sizes make the 4:3 pillarbox land on a half pixel and stop the integer-scale modes
        // being integers. Round up rather than down so nothing shrinks below the minimum.
        width += width & 1;
        height += height & 1;

        if (width < min_width || width > desktop_width)
            return;

        if (height < min_height || height > desktop_height)
            return;

        for (u32 i = 0; i < info.ResolutionCount; ++i)
        {
            const gfxResData& res = info.Resolutions[i];

            if (res.ScreenWidth == width && res.ScreenHeight == height)
                return;
        }

        // THE BOUND CHECK Open1560 DOES NOT HAVE. docs/resolution_blueprint.md records
        // add_resolution writing past a 32-entry array with nothing stopping it, and getting away
        // with it only because a 2160p display happens to yield about fourteen entries. The array
        // here is 64 entries and the generator below can exceed that on a tall display, so this is
        // not theoretical.
        if (info.ResolutionCount >= static_cast<u32>(kMaxResolutions))
        {
            Warningf("GL: resolution list full at %d entries, dropping %dx%d", kMaxResolutions, width, height);

            return;
        }

        gfxResData& res = info.Resolutions[info.ResolutionCount++];

        res.ScreenWidth = static_cast<u16>(width);
        res.ScreenHeight = static_cast<u16>(height);
        res.ColorDepth = kSynthesizedColorDepth;
        res.Flags = 6; // ResCallback writes 7 for 16-bit and 6 for anything else.
    };

    i32 scale = agiGreatestCommonDivisor(desktop_width, desktop_height);
    i32 aspect_w = desktop_width / scale;
    i32 aspect_h = desktop_height / scale;

    // Every height is offered twice, once at the display's own aspect and once at 4:3. The 4:3
    // entry is not vestigial - MM2's menus are authored for it, and someone who wants the menus
    // undistorted wants that mode to exist.
    const auto add_height = [&](i32 height) {
        add_resolution((height * aspect_w) / aspect_h, height);
        add_resolution((height * 4) / 3, height);
    };

    add_height(480);

    i32 small_height = desktop_height;

    for (i32 i = 1; i < kMaxIntegerScaleSteps; ++i)
    {
        i32 height = desktop_height / i;

        if (height >= kSmallHeightThreshold)
            small_height = height;

        if (height < min_height)
            break;

        add_height(height);
    }

    static const i32 small_height_ratios[][2] {
        {4, 5}, // 80%
        {3, 4}, // 75%
        {2, 3}, // 66%
    };

    static const i32 valid_small_heights[] {540, 576, 600, 720, 768, 800, 900, 960, 1080};

    for (const i32* ratio : small_height_ratios)
    {
        i32 height = (small_height * ratio[0]) / ratio[1];

        for (i32 valid : valid_small_heights)
        {
            if (height == valid)
            {
                add_height(height);

                break;
            }
        }
    }

    if (info.ResolutionCount == 0)
    {
        Warningf("GL: no synthesised mode fit between %dx%d and %dx%d", min_width, min_height, desktop_width,
            desktop_height);

        add_resolution(kDefaultWidth, kDefaultHeight);
    }

    agiSortResolutions(info);

    // Default to the desktop resolution, which after the integer-scale walk is always the last
    // entry: the sort is by height then width, and nothing larger than the desktop was added.
    info.ResolutionChoice = info.ResolutionCount - 1;

    gfxInterfaceCount = 1;
    gfxInterfaceChoice = 0;

    Displayf("GL: synthesised %d resolutions, default %dx%d", info.ResolutionCount,
        info.Resolutions[info.ResolutionChoice].ScreenWidth, info.Resolutions[info.ResolutionChoice].ScreenHeight);
}

void agiGLInitScaling(HWND window)
{
    const gfxInterface& info = gfxInterfaces[(gfxInterfaceChoice < gfxInterfaceCount) ? gfxInterfaceChoice : 0];

    i32 width = kDefaultWidth;
    i32 height = kDefaultHeight;

    if (info.ResolutionChoice < info.ResolutionCount)
    {
        width = info.Resolutions[info.ResolutionChoice].ScreenWidth;
        height = info.Resolutions[info.ResolutionChoice].ScreenHeight;
    }

    // -width and -height bypass the list entirely and are not validated against anything. That is
    // deliberate and copied from Open1560: it is the fastest way to try a resolution the generator
    // did not produce, and validating it would defeat the purpose.
    // THE PIPELINE OUTRANKS THE CONFIG, when it has an answer.
    //
    // The size above comes from gfxInterfaces[...].Resolutions[ResolutionChoice] - gfxconf.dat -
    // and that file cannot be relied on. With dgVoodoo gone the game enumerates no modes at all,
    // so ResolutionChoice indexes an empty table and this falls back to 640x480 while the frontend
    // is actually running at 1920x1080. agiScreen then describes a screen that does not exist: the
    // render target is sized for a 4:3 logical, the blit letterboxes it, and the menus arrive with
    // a black band down one side.
    //
    // gfxPipeline::m_iWidth is what the game just told SetRes, so it is true by construction. It is
    // zero before the first SetRes, which is why the config is still read as the fallback.
    if (gfxPipeline::Access_m_iWidth() > 0 && gfxPipeline::Access_m_iHeight() > 0)
    {
        width = gfxPipeline::Access_m_iWidth();
        height = gfxPipeline::Access_m_iHeight();
    }

    agiScreen.width = agiGetArgInt("width", width);
    agiScreen.height = agiGetArgInt("height", height);

    agiRect client {};

    if (!GetClientRect(window, &client))
    {
        Errorf("GL: GetClientRect failed, assuming the window matches the logical size");

        client.right = agiScreen.width;
        client.bottom = agiScreen.height;
    }

    agiScreen.horz_res = client.right - client.left;
    agiScreen.vert_res = client.bottom - client.top;

    if (agiScreen.horz_res <= 0 || agiScreen.vert_res <= 0)
    {
        agiScreen.horz_res = agiScreen.width;
        agiScreen.vert_res = agiScreen.height;
    }

    i32 scaling_mode = agiGetArgInt("scaling", static_cast<i32>(agiScalingMode::StretchKeepAspect));

    switch (scaling_mode)
    {
        case static_cast<i32>(agiScalingMode::StretchKeepAspect):
        case static_cast<i32>(agiScalingMode::Stretch): {
            agiScreen.blit_width = agiScreen.horz_res;
            agiScreen.blit_height = agiScreen.vert_res;

            if (scaling_mode == static_cast<i32>(agiScalingMode::StretchKeepAspect))
            {
                i32 w_factor = agiScreen.width * agiScreen.blit_height;
                i32 h_factor = agiScreen.height * agiScreen.blit_width;
                i32 difference = (w_factor > h_factor) ? (w_factor - h_factor) : (h_factor - w_factor);

                // Below one percent the pillarbox is a couple of pixels and letterboxing costs more
                // than the distortion does. Open1560 draws the line in the same place.
                if (difference > (agiScreen.height * agiScreen.blit_height) / 100)
                {
                    if (w_factor < h_factor)
                        agiScreen.blit_width = w_factor / agiScreen.height;
                    else
                        agiScreen.blit_height = h_factor / agiScreen.width;
                }
            }

            break;
        }

        case static_cast<i32>(agiScalingMode::Centered):
        case static_cast<i32>(agiScalingMode::CenteredInteger): {
            i32 factor = 1;

            if (scaling_mode == static_cast<i32>(agiScalingMode::CenteredInteger))
            {
                i32 fit_x = agiScreen.horz_res / agiScreen.width;
                i32 fit_y = agiScreen.vert_res / agiScreen.height;
                i32 fit = (fit_x < fit_y) ? fit_x : fit_y;

                factor = (fit > 1) ? fit : 1;
            }

            agiScreen.blit_width = agiScreen.width * factor;
            agiScreen.blit_height = agiScreen.height * factor;

            break;
        }

        default: {
            Warningf("GL: unknown -scaling %d, using stretch with aspect", scaling_mode);

            agiScreen.blit_width = agiScreen.horz_res;
            agiScreen.blit_height = agiScreen.vert_res;

            break;
        }
    }

    agiScreen.blit_x = (agiScreen.horz_res - agiScreen.blit_width) / 2;
    agiScreen.blit_y = (agiScreen.vert_res - agiScreen.blit_height) / 2;

    // Native rendering is on by default: rasterise at the size the blit lands at rather than at the
    // logical resolution. This is the setting that makes the logical resolution a UI coordinate
    // system rather than a rendering one, which is the whole point of the design.
    bool native_res = !agiGetArgFlag("nonativeres");

    agiScreen.render_x = 0;
    agiScreen.render_y = 0;
    agiScreen.render_width = native_res ? agiScreen.blit_width : agiScreen.width;
    agiScreen.render_height = native_res ? agiScreen.blit_height : agiScreen.height;

    agiScreen.msaa_level = agiGetArgInt("msaa", 0);

    agiScreen.blit_filter =
        (agiScreen.render_width > agiScreen.blit_width || agiScreen.render_height > agiScreen.blit_height) ? GL_LINEAR
                                                                                                          : GL_NEAREST;

    Displayf("GL: logical %dx%d, window %dx%d, blit %dx%d at %d,%d, render %dx%d, msaa %d", agiScreen.width,
        agiScreen.height, agiScreen.horz_res, agiScreen.vert_res, agiScreen.blit_width, agiScreen.blit_height,
        agiScreen.blit_x, agiScreen.blit_y, agiScreen.render_width, agiScreen.render_height, agiScreen.msaa_level);
}

#endif
