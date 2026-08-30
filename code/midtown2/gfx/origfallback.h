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

#pragma once

#include "core/arts.h"

// CALLING THE ORIGINAL 1999 CODE FROM C++.
//
// Registering a symbol in data/ported.json makes tools/asm.py replace its PROC with 0xCC padding,
// because normally the C++ is a complete reimplementation and the original is dead weight. The
// OpenGL work needs something different: a function that has a -gl branch is only reimplemented on
// ONE of its two paths, and the other has to keep behaving exactly as it always did.
//
// So for the symbols listed in data/keep_original.json, asm.py renames the PROC instead of padding
// it. Not one byte moves; the original simply stops answering to the mangled name, which the C++
// now defines, and answers to _arts_orig_<stem> instead. These declarations are the C++ end of
// that.
//
// WHY extern "C" AND A CAST RATHER THAN A DIRECT DECLARATION. An extern "C" function declared
// cdecl gets a single leading underscore in 32-bit MSVC, which is exactly the label asm.py emits -
// that is the whole reason the alias is spelled the way it is. But most of these originals are not
// cdecl: a QAE method is __thiscall, taking `this` in ecx, and there is no way to spell "a free
// function that is __thiscall" as a declaration. Taking the address of an opaque symbol and
// casting it to the right function-pointer type is the standard way out, and it costs nothing at
// runtime - the cast is a compile-time reinterpretation of a link-time address.
//
// EVERY SIGNATURE HERE IS READ OFF THE MANGLED NAME AND CHECKED AGAINST THE DISASSEMBLY. Getting
// one wrong does not fail to build; it corrupts the stack at runtime, in original code, with no
// symbol anywhere near the damage. SAX is a static returning void with cdecl cleanup. QAE is a
// public non-const method, __thiscall, and the callee pops its stack arguments - which is why
// gfxBitmap::Clear ends in `ret 4` and gfxBitmap::Create, a static, ends in a bare `ret`.

class gfxBitmap;
class gfxImage;

extern "C" {

// ?BeginFrame@gfxPipeline@@SAXXZ at 0x004AA130
void arts_orig_gfxPipeline_BeginFrame();

// ?EndFrame@gfxPipeline@@SAXXZ at 0x004AA330
void arts_orig_gfxPipeline_EndFrame();

// ?ProgressRect@@YAXHHHHI@Z at 0x00401010 - a free function, so this one really is cdecl and the
// declaration below could have been used directly. It goes through the same opaque-symbol dance
// only so that every fallback in this file reads the same way.
void arts_orig_ProgressRect();

// ?Create@gfxBitmap@@SAPAV1@HH_N@Z at 0x004AE4D0
void arts_orig_gfxBitmap_Create();

// ?Load@gfxBitmap@@QAE_NPAVgfxImage@@@Z at 0x004AE5C0
void arts_orig_gfxBitmap_Load();

// ?Clear@gfxBitmap@@QAEXI@Z at 0x004AE580
void arts_orig_gfxBitmap_Clear();

// ?DoFlush@gfxViewport@@AAEXXZ at 0x004B18D0
void arts_orig_gfxViewport_DoFlush();

// ?BeginGfx3D@gfxPipeline@@SA_NXZ at 0x004A96C0
void arts_orig_gfxPipeline_BeginGfx3D();
void arts_orig_gfxAutoDetect();

// ?SetCamera@gfxRenderState@@SAXABVMatrix44@@@Z at 0x004B2A20
void arts_orig_gfxRenderState_SetCamera44();

// ?Perspective@gfxViewport@@QAEXMMMM@Z at 0x004B1640
void arts_orig_gfxViewport_Perspective();

} // extern "C"

// The typed views of the above. Written as small inline wrappers rather than as macros so that a
// wrong argument is a compile error here instead of a corrupted stack there.

inline void ArtsOrigBeginFrame()
{
    reinterpret_cast<void(__cdecl*)()>(&arts_orig_gfxPipeline_BeginFrame)();
}

inline void ArtsOrigEndFrame()
{
    reinterpret_cast<void(__cdecl*)()>(&arts_orig_gfxPipeline_EndFrame)();
}

inline void ArtsOrigProgressRect(i32 arg1, i32 arg2, i32 arg3, i32 arg4, u32 arg5)
{
    reinterpret_cast<void(__cdecl*)(i32, i32, i32, i32, u32)>(&arts_orig_ProgressRect)(
        arg1, arg2, arg3, arg4, arg5);
}

inline gfxBitmap* ArtsOrigBitmapCreate(i32 arg1, i32 arg2, bool arg3)
{
    return reinterpret_cast<gfxBitmap*(__cdecl*)(i32, i32, bool)>(&arts_orig_gfxBitmap_Create)(arg1, arg2, arg3);
}

inline bool ArtsOrigBitmapLoad(gfxBitmap* self, gfxImage* image)
{
    return reinterpret_cast<bool(__thiscall*)(gfxBitmap*, gfxImage*)>(&arts_orig_gfxBitmap_Load)(self, image);
}

inline void ArtsOrigViewportDoFlush(class gfxViewport* self)
{
    reinterpret_cast<void(__thiscall*)(class gfxViewport*)>(&arts_orig_gfxViewport_DoFlush)(self);
}

inline bool ArtsOrigBeginGfx3D()
{
    return reinterpret_cast<bool(__cdecl*)()>(&arts_orig_gfxPipeline_BeginGfx3D)();
}

// ?gfxAutoDetect@@YA_NPA_N@Z - __cdecl, one bool* out-parameter, returns bool.
inline bool ArtsOrigAutoDetect(bool* success_out)
{
    return reinterpret_cast<bool(__cdecl*)(bool*)>(&arts_orig_gfxAutoDetect)(success_out);
}

inline void ArtsOrigSetCamera44(const class Matrix44& m)
{
    reinterpret_cast<void(__cdecl*)(const class Matrix44&)>(&arts_orig_gfxRenderState_SetCamera44)(m);
}

// __thiscall, four floats: fov, aspect, near, far.
inline void ArtsOrigPerspective(class gfxViewport* self, f32 fov, f32 aspect, f32 near_z, f32 far_z)
{
    reinterpret_cast<void(__thiscall*)(class gfxViewport*, f32, f32, f32, f32)>(
        &arts_orig_gfxViewport_Perspective)(self, fov, aspect, near_z, far_z);
}

inline void ArtsOrigBitmapClear(gfxBitmap* self, u32 color)
{
    reinterpret_cast<void(__thiscall*)(gfxBitmap*, u32)>(&arts_orig_gfxBitmap_Clear)(self, color);
}
