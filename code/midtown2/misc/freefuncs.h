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

#include "core/arts.h"

class gfxBitmap;

// The free functions - the ones that belong to no class at all.
//
// midtown2.exe has 1,368 of them, about 224 KB, and they are the cheapest code in the binary to
// reimplement: with no `this` there is no instance layout to recover first, so a free function is
// portable the moment its body is legible. This header holds the ones that have been done.
//
// Every name here was resolved by ADDRESS against data/symbols.json (the retail linker map) and
// then read back off the disassembly of midtown2.exe. The names in MM2_RE_KIT are not trusted:
// several hundred of them carry ImGui/ImPlot labels pasted in from a foreign symbol set, and
// _INDEX.csv is wrong in places too. The map is the only ground truth.
//
// These live in misc/ because that is the subsystem tools/symbols.py assigns to an unprefixed
// global. They are not otherwise related to one another.

class Vector3;

// ?AddDeadZone@@YAMMM@Z
ARTS_EXPORT f32 AddDeadZone(f32 arg1, f32 arg2);

// ?CleanName@@YAPBDPBD@Z
ARTS_EXPORT const char* CleanName(const char* arg1);

// ?cmpPtx@@YAHPBX0@Z
ARTS_EXPORT i32 cmpPtx(const void* arg1, const void* arg2);

// ?FindTValueSegToOrigin@@YAMABVVector3@@0@Z
ARTS_EXPORT f32 FindTValueSegToOrigin(const Vector3& arg1, const Vector3& arg2);

// ?frand@@YAMH@Z
ARTS_EXPORT f32 frand(i32 arg1);

// ?frand@@YAMXZ
ARTS_EXPORT f32 frand();

// ?gfxFVFOffset@@YAIII@Z
ARTS_EXPORT u32 gfxFVFOffset(u32 arg1, u32 arg2);

// ?gfxFVFSize@@YAII@Z | still assembly
//
// Declared, not ported. The GL packet draw path needs the vertex stride and this is the only thing
// in the image that computes it: a 16-byte table at 0x005CD670 indexed by `fvf & 0xF` gives the
// position size (2 -> 12, 4 -> 16, 6 -> 16, 8 -> 20, 10 -> 24, 12 -> 28, 14 -> 32), then +12 for
// D3DFVF_NORMAL, +4 each for D3DFVF_RESERVED1, D3DFVF_DIFFUSE and D3DFVF_SPECULAR, and +8 each for
// D3DFVF_TEX1 and D3DFVF_TEX2.
//
// That last pair is worth knowing before anyone ports it: 0x100 and 0x200 are treated as
// INDEPENDENT BITS, but in D3D7 they are a two-bit texture COUNT, so an FVF of 0x200 (two
// coordinate sets, 16 bytes) is measured as 8. Either MM2 never uses more than one set, or this is
// a latent 1999 bug. See agigl/glpacket.cpp.
ARTS_IMPORT u32 gfxFVFSize(u32 arg1);

// ?ilog2@@YAHH@Z
ARTS_EXPORT i32 ilog2(i32 arg1);

// ?inRange@@YAHMMM@Z
ARTS_EXPORT i32 inRange(f32 arg1, f32 arg2, f32 arg3);

// ?irand@@YAHH@Z
ARTS_EXPORT i32 irand(i32 arg1);

// ?irand@@YAHXZ
ARTS_EXPORT i32 irand();

// ?IsNearZero@@YAHMM@Z
ARTS_EXPORT i32 IsNearZero(f32 arg1, f32 arg2);

// ?IsOnStack@@YA_NPAX@Z
ARTS_EXPORT bool IsOnStack(void* arg1);

// ?IsPointBehindPlane@@YA_NABVVector3@@00M@Z
ARTS_EXPORT bool IsPointBehindPlane(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3, f32 arg4);

// ?IsPointInBox@@YA_NABVVector3@@MMM@Z
ARTS_EXPORT bool IsPointInBox(const Vector3& arg1, f32 arg2, f32 arg3, f32 arg4);

// ?IsPointInRegion@@YA_NMMPBVVector3@@H@Z
ARTS_EXPORT bool IsPointInRegion(f32 arg1, f32 arg2, const Vector3* arg3, i32 arg4);

// ?IsPointNearPlane@@YA_NABVVector3@@00M@Z
ARTS_EXPORT bool IsPointNearPlane(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3, f32 arg4);

// ?jdiv_round_up@@YAJJJ@Z
ARTS_EXPORT ilong jdiv_round_up(ilong arg1, ilong arg2);

// ?jround_up@@YAJJJ@Z
ARTS_EXPORT ilong jround_up(ilong arg1, ilong arg2);

// ?Lerp@@YAMMMM@Z
ARTS_EXPORT f32 Lerp(f32 arg1, f32 arg2, f32 arg3);

// ?max@@YAMMM@Z
ARTS_EXPORT f32 max(f32 arg1, f32 arg2);

// ?Max@@YAMMM@Z
ARTS_EXPORT f32 Max(f32 arg1, f32 arg2);

// ?min@@YAMMM@Z
ARTS_EXPORT f32 min(f32 arg1, f32 arg2);

// ?nodeGetBitmap@@YAPAVgfxBitmap@@PBD_N1@Z
//
// Declared, not ported: mmLinearGauge needs to CALL it, and an ARTS_IMPORT declaration is what
// lets the linker resolve that against the PROC still in game.asm. Returns a bitmap from the UI
// node cache, refcounted - see the release path in mmLinearGauge::Init.
ARTS_IMPORT gfxBitmap* nodeGetBitmap(const char* arg1, bool arg2, bool arg3);

// ?OrderIntersections@@YAXPAM000PAH1@Z
ARTS_EXPORT void OrderIntersections(f32* arg1, f32* arg2, f32* arg3, f32* arg4, i32* arg5, i32* arg6);

// ?RealQuadratic@@YAHMMPAM0M@Z
ARTS_EXPORT i32 RealQuadratic(f32 arg1, f32 arg2, f32* arg3, f32* arg4, f32 arg5);

// ?UpperCase@@YAXPAD@Z
ARTS_EXPORT void UpperCase(char* arg1);

// ?wpmax@@YAMMM@Z
ARTS_EXPORT f32 wpmax(f32 arg1, f32 arg2);

// ?wpmin@@YAMMM@Z
ARTS_EXPORT f32 wpmin(f32 arg1, f32 arg2);

// The two globals the random number generator runs on. Both are properly mangled in the map, so
// they can be declared here and the linker resolves them straight into the remaining assembly's
// .data - which matters, because the assembly still reads and writes the same two words.

// ?InitDirectDraw@@YAXXZ
//
// Reimplemented in code/midtown2/agigl/glactivate.cpp, where it carries the -gl runtime branch.
// It is one of only three functions that can: its original body calls BeginGfx2D, gfxSafeMode,
// MessageBoxA and exit, and no COM at all, so the fallback arm is a faithful transcription rather
// than a retyping of DirectDraw against vtables this build has no headers for.
//
// This declaration is what makes tools/asm.py strip the original. asm.py works from the
// ARTS_EXPORT markers in headers, and free functions have no generated header - so registering the
// symbol in data/ported.json alone leaves the original PROC in place and the link fails with
// "already defined in game.obj".
ARTS_EXPORT void InitDirectDraw();

// ?gfxAutoDetect@@YA_NPA_N@Z
//
// Reimplemented in code/midtown2/gfx/gfxautodetect.cpp with a -gl branch. The original probes every
// video adapter in the machine by creating a DirectDraw and a Direct3D device on each one, which is
// what makes the window sit black, flicker and hop between monitors before the intro plays. Under
// -gl none of that is needed; with -gl absent the original machine code runs, kept under an alias
// by data/keep_original.json.
//
// Declared here for the same reason InitDirectDraw is: it is a free function, so no generated
// header carries its ARTS_EXPORT, and without one asm.py leaves the original PROC in place.
ARTS_EXPORT bool gfxAutoDetect(bool* successOut);

// ?BeginPhase@@YAX_N@Z
//
// Reimplemented in code/midtown2/agigl/glbeginphase.cpp, where it carries the -gl runtime branch.
// It closes the OpenGL activation boundary: BeginPhase calls gfxPipeline::BeginGfx3D directly, and
// BeginGfx3D is 0xA70 bytes of pure COM, so it can never carry the branch itself. BeginPhase can -
// its own body contains no COM at all.
//
// With -gl absent the else arm is an exact transcription, so the game must behave identically. That
// is the only test this port has, and it is a good one: every run exercises it.
ARTS_EXPORT void BeginPhase(bool arg1);

// ?gRandSeed@@3HA
extern i32 gRandSeed;

// ?LogRandomCalls@@3P6AXXZA
extern void(__cdecl* LogRandomCalls)();
