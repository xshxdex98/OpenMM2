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

#include "core/arts.h"

define_dummy_symbol(agigl_glsetres);

// gfxPipeline::SetRes, READ OFF THE DISASSEMBLY.
//
// docs/opengl_plan.md lists this as blocker 2: the pseudocode kit has it as JUMPOUT(0x4A8CE1) and
// never decompiled a line of it, and it is on the boot path, and it is what writes m_iWidth and
// m_iHeight. It is 0x3CF bytes at 0x004A8CE0 and every statement below is transcribed from the
// instruction bytes in C:/mm2ghidra/out/game.asm.pristine. Where a value could not be read from the
// bytes it is not here.
//
// THE FINDING THAT MATTERS MOST IS THAT THIS FUNCTION DOES NOT NEED TO BE REGISTERED.
//
// Every DirectDraw reference in it is inside a single `if (lpDD)`. With -gl, BeginGfx2D never runs,
// lpDD stays null, and the ORIGINAL machine code already does exactly the right thing: it writes
// m_iWidth/m_iHeight/m_fWidth/m_fHeight/m_ColorDepth/m_ZDepth and ioMouse's inverse dimensions, and
// skips the mode set. The window is not sized here either - gfxPipeline::gfxWindowCreate reads
// m_iWidth/m_iHeight/m_X/m_Y and calls MoveWindow itself, which the reference dump of its PROC
// confirms - so a GL boot needs nothing from this function that the original does not already give
// it.
//
// So this file exists for two later reasons, not for a first frame:
//
//   1. It is the record of what those 0x3CF bytes do, which nothing else in the tree has.
//   2. A RUNTIME resolution change under GL needs the window-sizing arm to run with lpDD null, and
//      needs the framebuffer rebuilt afterwards. That is the `|| gl` on the outer test below.
//
// It is NOT in data/ported_agigl.json and should not be until someone has a reason to change the
// resolution at runtime under GL. Registering it buys nothing today and puts two hand-written COM
// vtable indices on the boot path of the working DirectDraw build.
#ifdef ARTS_AGIGL_SETRES

// Every include is inside the guard. gfx/gfxtexture.h pulls in gfx/gfximage.h, which names
// gfxImage::gfxImageFormat in signatures and never defines it (docs/opengl_plan.md, generated
// header 4), so this file must not cost anything until it is deliberately switched on.
#    include "data7/datargparser.h"
#    include "data7/printer.h"
#    include "gfx/gfxpipeline.h"
#    include "gfx/gfxtexture.h"
#    include "io/iomouse.h"

#    include "glswitch.h"
#    include "glwin32.h"

// The renderer-selection globals. None of these have a generated header - they are free statics
// with no class scope - so they are declared here under the exact mangled names in
// data/symbols.json. `bool` is _NA and `int` is HA; getting either wrong produces a symbol that
// does not exist rather than one that misbehaves.
ARTS_IMPORT extern bool useSoftware; // ?useSoftware@@3_NA      0x006830D4
ARTS_IMPORT extern bool useReference; // ?useReference@@3_NA    0x006830D3
ARTS_IMPORT extern bool useBlade; // ?useBlade@@3_NA            0x006830D6
ARTS_IMPORT extern bool useAgeSoftware; // ?useAgeSoftware@@3_NA 0x006830D5
ARTS_IMPORT extern bool useSysMem; // ?useSysMem@@3_NA          0x006830D7
ARTS_IMPORT extern bool tripleBuffer; // ?tripleBuffer@@3_NA    0x006830D2
ARTS_IMPORT extern bool useMultiTexture; // ?useMultiTexture@@3_NA 0x005CA3EE
ARTS_IMPORT extern bool novblank; // ?novblank@@3_NA            0x0068451D
ARTS_IMPORT extern bool enableHWTnL; // ?enableHWTnL@@3_NA      0x005CA664
ARTS_IMPORT extern bool pageFlip; // ?pageFlip@@3_NA            0x005CA3EC
ARTS_IMPORT extern bool inWindow; // ?inWindow@@3_NA            0x006830D0
ARTS_IMPORT extern bool isMaximized; // ?isMaximized@@3_NA      0x006830D1
ARTS_IMPORT extern i32 useInterface; // ?useInterface@@3HA      0x006830D8

// ?lpDD@@3PAUIDirectDraw7@@A, 0x006830A8.
//
// Typed void* on purpose. The two calls below go through hand-built vtable indices that were read
// out of the instruction bytes, and nothing else in this file may touch it - declaring a fake
// IDirectDraw7 with 30-odd stubs would put an ordering assumption in the tree that no verifier here
// can check. See docs/harness.md, "Three bugs with one shape".
ARTS_IMPORT extern void* lpDD;

// ?hwndMain@@3PAUHWND__@@A
ARTS_IMPORT extern HWND__* hwndMain;

struct agiWin32Rect
{
    i32 left;
    i32 top;
    i32 right;
    i32 bottom;
};

extern "C" __declspec(dllimport) i32 __stdcall GetSystemMetrics(i32 index);
extern "C" __declspec(dllimport) HDC__* __stdcall GetDC(HWND__* window);
extern "C" __declspec(dllimport) i32 __stdcall ReleaseDC(HWND__* window, HDC__* device_context);
extern "C" __declspec(dllimport) i32 __stdcall GetDeviceCaps(HDC__* device_context, i32 index);
extern "C" __declspec(dllimport) i32 __stdcall GetClientRect(HWND__* window, agiWin32Rect* rect);
extern "C" __declspec(dllimport) i32 __stdcall MoveWindow(
    HWND__* window, i32 x, i32 y, i32 width, i32 height, i32 repaint);

// From the instruction bytes: `push 10h` and `push 11h` into GetSystemMetrics. winuser.h calls
// these SM_CXFULLSCREEN and SM_CYFULLSCREEN - the client area of a maximised window, NOT the screen
// (SM_CXSCREEN is 0). That distinction is the whole point of the -max flag.
static constexpr i32 SM_CXFULLSCREEN = 16;
static constexpr i32 SM_CYFULLSCREEN = 17;

// From `push 8` and `push 0Ah` into GetDeviceCaps: HORZRES and VERTRES.
static constexpr i32 HORZRES = 8;
static constexpr i32 VERTRES = 10;

// IDirectDraw7 vtable indices, DERIVED FROM THE CALL SITES, not from memory.
//
// The instruction bytes are `FF 51 30` (call [ecx+0x30]) and `FF 56 54` (call [esi+0x54]) - byte
// offsets into the vtable, so the indices are 0x30/4 and 0x54/4. They agree with the documented
// IDirectDraw7 layout, where GetDisplayMode is the thirteenth entry and SetDisplayMode the
// twenty-second, which is the corroboration rather than the source.
static constexpr usize kIDirectDraw7_GetDisplayMode = 0x30 / 4;
static constexpr usize kIDirectDraw7_SetDisplayMode = 0x54 / 4;

// DDSURFACEDESC2. Only three fields are touched and all three offsets are in the bytes:
// `mov dword [ebp-8Ch], 7Ch` is dwSize at +0x00, `cmp [ebp-84h], edx` is dwHeight at +0x08, and
// `cmp [ebp-80h], ecx` is dwWidth at +0x0C. The rest is opaque padding to the 0x7C the structure
// declares itself to be.
static constexpr usize kDDSurfaceDescSize = 0x7C;
static constexpr usize kDDSurfaceDescHeight = 0x08 / 4;
static constexpr usize kDDSurfaceDescWidth = 0x0C / 4;

// ?SetRes@gfxPipeline@@SAXHHHH_N@Z
void gfxPipeline::SetRes(i32 width, i32 height, i32 cdepth, i32 zdepth, bool parse_args)
{
    // 0x004A8CE9: `mov al, [ebp+18h]; test al, al; jz 0x4A8F3A`. The whole command-line block is
    // conditional on the fifth argument, which is why SafeBeginGfx's retry passes false: it wants
    // 640x480 and must not have -width put back over it.
    if (parse_args)
    {
        // The renderer chain. Note that "ref" jumps past everything else (EB 6B from 0x4A8D15 to
        // 0x4A8D82), so it is an else-if chain and not four independent tests.
        if (datArgParser::Get("ref"))
        {
            useSoftware = true;
            useReference = true;
        }
        else if (datArgParser::Get("blade") || datArgParser::Get("bladed"))
        {
            useSoftware = true;
            useBlade = true;
        }
        else if (datArgParser::Get("swage"))
        {
            useSoftware = true;
            useAgeSoftware = true;
        }
        else if (datArgParser::Get("sw"))
        {
            useSoftware = true;
        }

        if (datArgParser::Get("sysmem"))
            useSysMem = true;

        if (datArgParser::Get("triple"))
            tripleBuffer = true;

        if (datArgParser::Get("nomultitexture") || datArgParser::Get("nomt"))
            useMultiTexture = false;

        if (datArgParser::Get("novblank"))
            novblank = true;

        if (datArgParser::Get("nohwtnl"))
            enableHWTnL = false;

        if (datArgParser::Get("tex32"))
            gfxTexture::sm_Allow32 = true;

        // -primary pins the DirectDraw adapter to 0; -display <n> selects one. This is NOT
        // gfxInterfaceChoice: useInterface (0x006830D8) and interfaceCount (0x006830DC) belong to
        // gfxPipeline::EnumDDAdapters, while gfxInterfaceChoice (0x006844C8) and gfxInterfaceCount
        // (0x006844C0) belong to the gfxconf.dat adapter table. Two similarly named pairs, two
        // different subsystems. Under GL, EnumDDAdapters never runs and this pair is dead.
        if (datArgParser::Get("primary"))
            useInterface = 0;
        else
            datArgParser::Get("display", 0, useInterface);

        if (datArgParser::Get("single"))
            pageFlip = false;

        if (datArgParser::Get("window"))
        {
            inWindow = true;
        }
        else if (datArgParser::Get("max"))
        {
            isMaximized = true;
            inWindow = true;
        }
        else if (datArgParser::Get("fs") || datArgParser::Get("fullscreen"))
        {
            inWindow = false;
        }

        // The four value flags write straight over the incoming arguments - `lea eax, [ebp+8]` and
        // friends pass the parameter slots by address. Passing 0 as the middle argument is the
        // original's own use of the word index, and it is what settles open question 4 in
        // docs/opengl_plan.md.
        datArgParser::Get("width", 0, width);
        datArgParser::Get("height", 0, height);
        datArgParser::Get("cdepth", 0, cdepth);
        datArgParser::Get("zdepth", 0, zdepth);

        if (isMaximized)
        {
            width = GetSystemMetrics(SM_CXFULLSCREEN);
            height = GetSystemMetrics(SM_CYFULLSCREEN);

            gfxPipeline::m_Y = 0;
            gfxPipeline::m_X = 0;
        }
    }

    // 0x004A8F3A. The order here is the original's, including the one line that looks like a typo
    // and is not: useSysMem is copied FROM useSoftware, unconditionally, every time.
    gfxPipeline::m_fWidth = static_cast<f32>(width);
    useSysMem = useSoftware;
    gfxPipeline::m_ColorDepth = cdepth;
    gfxPipeline::m_ZDepth = zdepth;
    gfxPipeline::m_iWidth = width;
    gfxPipeline::m_fHeight = static_cast<f32>(height);
    gfxPipeline::m_iHeight = height;

    const bool gl = agiGLEnabled();

    // `test eax, eax; jz 0x4A9085` on lpDD. Everything in here is skipped when there is no
    // DirectDraw object yet, which is both how the first SetRes at boot behaves and how every
    // SetRes behaves under -gl. The `|| gl` is the only added condition in this function.
    if (lpDD != nullptr || gl)
    {
        if (inWindow)
        {
            // Centre the window on the desktop and size its CLIENT area to the resolution. The
            // second MoveWindow is not redundant: the first sizes the whole window, GetClientRect
            // then reports how much of that the border ate, and the second adds the difference
            // back. `2 * m_iWidth - rc.right` is the original's arithmetic, not a rewrite of it.
            HDC__* dc = GetDC(nullptr);

            i32 desktop_width = GetDeviceCaps(dc, HORZRES);
            i32 desktop_height = GetDeviceCaps(dc, VERTRES);

            ReleaseDC(nullptr, dc);

            // `sar esi, 1`, an arithmetic shift. Written as >> rather than / 2 because the two
            // differ when the difference is negative - a window larger than the desktop - and the
            // shift is what the original does.
            gfxPipeline::m_X = (desktop_width - gfxPipeline::m_iWidth) >> 1;
            gfxPipeline::m_Y = (desktop_height - gfxPipeline::m_iHeight) >> 1;

            MoveWindow(hwndMain, gfxPipeline::m_X, gfxPipeline::m_Y, gfxPipeline::m_iWidth, gfxPipeline::m_iHeight, 0);

            agiWin32Rect client {};

            GetClientRect(hwndMain, &client);

            MoveWindow(hwndMain, gfxPipeline::m_X, gfxPipeline::m_Y, 2 * gfxPipeline::m_iWidth - client.right,
                2 * gfxPipeline::m_iHeight - client.bottom, 1);
        }
        else if (!gl)
        {
            // The display-mode set, and the only DirectDraw in the function. Deleted on the GL
            // path rather than translated: a framebuffer object accepts any size, so there is no
            // mode to change, and fullscreen becomes a borderless window sized by the device layer.
            void** vtbl = *reinterpret_cast<void***>(lpDD);

            u32 desc[kDDSurfaceDescSize / 4] {};

            desc[0] = kDDSurfaceDescSize;

            using GetDisplayModeFn = i32(__stdcall*)(void*, void*);
            using SetDisplayModeFn = i32(__stdcall*)(void*, u32, u32, u32, u32, u32);

            i32 hr = reinterpret_cast<GetDisplayModeFn>(vtbl[kIDirectDraw7_GetDisplayMode])(lpDD, desc);

            bool already_set = (hr == 0) && (desc[kDDSurfaceDescWidth] == static_cast<u32>(gfxPipeline::m_iWidth)) &&
                (desc[kDDSurfaceDescHeight] == static_cast<u32>(gfxPipeline::m_iHeight));

            if (!already_set)
            {
                reinterpret_cast<SetDisplayModeFn>(vtbl[kIDirectDraw7_SetDisplayMode])(lpDD,
                    static_cast<u32>(gfxPipeline::m_iWidth), static_cast<u32>(gfxPipeline::m_iHeight),
                    static_cast<u32>(gfxPipeline::m_ColorDepth), 0, 0);
            }
        }
    }

    // 0x004A9085, and easy to miss because it is pure x87: 1.0f / m_fWidth into ioMouse::m_InvWidth
    // and 1.0f / m_fHeight into ioMouse::m_InvHeight. The constant at 0x005B0364 is 1.0f, read out
    // of .rdata. Both are private statics of ioMouse, hence the generated accessors.
    ioMouse::Access_m_InvWidth() = 1.0f / gfxPipeline::m_fWidth;
    ioMouse::Access_m_InvHeight() = 1.0f / gfxPipeline::m_fHeight;

    // TODO: under -gl a resolution change also has to rerun agiGLInitScaling and rebuild the render
    // framebuffer, which agiGLBeginGfx2D/agiGLBeginGfx3D already do idempotently. It is not called
    // from here because SetRes runs at boot BEFORE the device exists, and distinguishing the two
    // cases needs a flag this file does not have. Whoever registers this function should add it.
}

#endif
