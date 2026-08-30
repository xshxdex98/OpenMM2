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

#include "core/arts.h"

define_dummy_symbol(agigl_gltexdump);

#ifdef ARTS_AGIGL

#    include "data7/printer.h"

#    include "data7/datargparser.h"

#    include "glcommon.h"
#    include "glcontext.h"

// A DIAGNOSTIC, NOT PART OF THE RENDERER.
//
// Reads a GL texture back and writes it out as a .bmp, so a texture the game bound can simply be
// looked at rather than reasoned about. It exists because the vehicle showcase draws car body
// panels four times per frame, and two of those passes bind a single 256x256 texture over every
// panel - and no amount of staring at draw-call state says WHAT that texture is.
//
// Deliberately does not touch MM2's heap: the readback buffer comes from VirtualAlloc, because a
// 256x256x4 allocation out of the game's 32 MB pool is exactly the sort of thing that has already
// produced "Fatal Error: Heap overrun" here once.

extern "C" __declspec(dllimport) void* __stdcall LoadLibraryA(const char* name);
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);

namespace
{
    using PFNVirtualAlloc = void*(__stdcall*)(void*, u32, u32, u32);
    using PFNVirtualFree = i32(__stdcall*)(void*, u32, u32);
    using PFNCreateFileA = void*(__stdcall*)(const char*, u32, u32, void*, u32, u32, void*);
    using PFNWriteFile = i32(__stdcall*)(void*, const void*, u32, u32*, void*);
    using PFNCloseHandle = i32(__stdcall*)(void*);

    PFNVirtualAlloc pVirtualAlloc = nullptr;
    PFNVirtualFree pVirtualFree = nullptr;
    PFNCreateFileA pCreateFileA = nullptr;
    PFNWriteFile pWriteFile = nullptr;
    PFNCloseHandle pCloseHandle = nullptr;

    bool LoadKernel()
    {
        if (pWriteFile != nullptr)
            return true;

        void* k = LoadLibraryA("KERNEL32.dll");

        if (k == nullptr)
            return false;

        pVirtualAlloc = reinterpret_cast<PFNVirtualAlloc>(GetProcAddress(k, "VirtualAlloc"));
        pVirtualFree = reinterpret_cast<PFNVirtualFree>(GetProcAddress(k, "VirtualFree"));
        pCreateFileA = reinterpret_cast<PFNCreateFileA>(GetProcAddress(k, "CreateFileA"));
        pWriteFile = reinterpret_cast<PFNWriteFile>(GetProcAddress(k, "WriteFile"));
        pCloseHandle = reinterpret_cast<PFNCloseHandle>(GetProcAddress(k, "CloseHandle"));

        return pVirtualAlloc != nullptr && pCreateFileA != nullptr && pWriteFile != nullptr;
    }

    // Ids already written, so a texture bound every frame produces one file rather than thousands.
    // 24 was too small to reach the car textures: the city binds dozens before a car is drawn, so
    // the interesting ids (1300-1600) were never written out and the black-body question could not
    // be answered from the dump. -gltexdump is opt-in, so the only cost of a larger table is disk.
    u32 g_dumped[512];
    i32 g_dumped_count = 0;

    void PutU32(u8* p, u32 v)
    {
        p[0] = static_cast<u8>(v);
        p[1] = static_cast<u8>(v >> 8);
        p[2] = static_cast<u8>(v >> 16);
        p[3] = static_cast<u8>(v >> 24);
    }
} // namespace

// Off unless -gltexdump is on the command line. Reading a texture back stalls the GL pipeline, so
// this is a diagnostic to reach for deliberately, not something to leave running.
// THE PER-DRAW DIAGNOSTICS, SILENT UNLESS ASKED FOR WITH -glprobe.
//
// These probes earned their place - they named the degenerate projection, the ambient swing that
// was the fullbright flicker, and the light values behind the dark scene - and they are not worth
// paying for every frame: one session ran to 5.7 MB of log.
//
// GATE THE Displayf, NEVER THE FUNCTION. An earlier attempt at this put an early return in front
// of a probe's print, and that return landed ahead of the real bookkeeping in agiGLProbeNoteDraw,
// which crashed the game on entering water. A log line is not a control-flow boundary.
bool agiGLProbeVerbose()
{
    static i32 enabled = -1;

    if (enabled < 0)
        enabled = datArgParser::Get("glprobe") ? 1 : 0;

    return enabled != 0;
}

bool agiGLTextureDumpEnabled()
{
    static i32 enabled = -1;

    if (enabled < 0)
        enabled = datArgParser::Get("gltexdump") ? 1 : 0;

    return enabled != 0;
}

void agiGLDumpTexture(u32 texture, i32 width, i32 height)
{
    if (texture == 0 || width <= 0 || height <= 0 || glGetTexImage == nullptr)
        return;

    for (i32 i = 0; i < g_dumped_count; ++i)
    {
        if (g_dumped[i] == texture)
            return;
    }

    if (g_dumped_count >= static_cast<i32>(sizeof(g_dumped) / sizeof(g_dumped[0])) || !LoadKernel())
        return;

    g_dumped[g_dumped_count++] = texture;

    const u32 pixel_count = static_cast<u32>(width) * static_cast<u32>(height);
    const u32 image_bytes = pixel_count * 4;
    const u32 total = 54 + image_bytes;

    u8* buffer = static_cast<u8*>(pVirtualAlloc(nullptr, total, 0x1000 /* MEM_COMMIT */, 0x04 /* RW */));

    if (buffer == nullptr)
        return;

    // BITMAPFILEHEADER, then a 40-byte BITMAPINFOHEADER, 32bpp. Height is written negative so the
    // rows go top-down and match GL's readback order without a flip pass.
    buffer[0] = 0x42; // 'B'
    buffer[1] = 0x4D; // 'M'
    PutU32(buffer + 2, total);
    PutU32(buffer + 6, 0);
    PutU32(buffer + 10, 54);
    PutU32(buffer + 14, 40);
    PutU32(buffer + 18, static_cast<u32>(width));
    PutU32(buffer + 22, static_cast<u32>(-height));
    buffer[26] = 1;
    buffer[27] = 0;
    buffer[28] = 32;
    buffer[29] = 0;
    PutU32(buffer + 30, 0);
    PutU32(buffer + 34, image_bytes);
    PutU32(buffer + 38, 2835);
    PutU32(buffer + 42, 2835);
    PutU32(buffer + 46, 0);
    PutU32(buffer + 50, 0);

    // BGRA, because that is what a 32-bit BMP stores and what every viewer expects.
    agiGL->BindTexture2D(texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, buffer + 54);

    // No CRT string formatting here - the game's CRT is the one reproduced in game.asm, and
    // <cstdio> lowers to ucrt helpers with no definition to link against. The name is assembled by
    // hand instead.
    char name[64];
    i32 n = 0;

    const char* prefix = "gltex_";

    while (prefix[n] != 0)
    {
        name[n] = prefix[n];
        ++n;
    }

    char digits[12];
    i32 d = 0;
    u32 id = texture;

    do
    {
        digits[d++] = static_cast<char>(0x30 + (id % 10));
        id /= 10;
    } while (id != 0 && d < 11);

    while (d > 0)
        name[n++] = digits[--d];

    const char* suffix = ".bmp";
    i32 s = 0;

    while (suffix[s] != 0)
        name[n++] = suffix[s++];

    name[n] = 0;

    void* file =
        pCreateFileA(name, 0x40000000 /* GENERIC_WRITE */, 0, nullptr, 2 /* CREATE_ALWAYS */, 0x80, nullptr);

    if (file != nullptr && file != reinterpret_cast<void*>(-1))
    {
        u32 written = 0;

        pWriteFile(file, buffer, total, &written, nullptr);
        pCloseHandle(file);

        Displayf("TEXDUMP wrote %s (%dx%d)", name, width, height);
    }

    pVirtualFree(buffer, 0, 0x8000 /* MEM_RELEASE */);
}

#endif
