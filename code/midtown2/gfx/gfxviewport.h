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

#include "vector7/matrix44.h"

enum gfxCullStatus : i32;

class Vector4;

class gfxViewport
{
public:
    // ?ComputeVFOV@gfxViewport@@QBEMM@Z
    ARTS_IMPORT f32 ComputeVFOV(f32 height) const;

    // ?DoPerspective@gfxViewport@@SAXPAV1@@Z
    ARTS_IMPORT static void DoPerspective(gfxViewport* matrix);

    // ?IsSphereVisible@gfxViewport@@QBE?AW4gfxCullStatus@@MMMMPAM@Z
    ARTS_IMPORT gfxCullStatus IsSphereVisible(f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32* arg5) const;

    // ?Ortho@gfxViewport@@QAEXMMMMMM@Z
    ARTS_IMPORT void Ortho(f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);

    // ?Perspective@gfxViewport@@QAEXMMMM@Z
    ARTS_EXPORT void Perspective(f32 fov, f32 aspect, f32 arg3, f32 arg4);

    // ?Project@gfxViewport@@QAEXAAVVector4@@0@Z
    ARTS_IMPORT void Project(Vector4& arg1, Vector4& arg2);

    // ?ResetWindow@gfxViewport@@QAEXXZ
    ARTS_IMPORT void ResetWindow();

    // ?SetWindow@gfxViewport@@QAEXHHHHMM@Z
    ARTS_IMPORT void SetWindow(i32 x, i32 y, i32 w, i32 h, f32 arg5, f32 arg6);

private:
    // ??0gfxViewport@@AAE@XZ
    ARTS_IMPORT gfxViewport();

    // ?DoFlush@gfxViewport@@AAEXXZ
    ARTS_EXPORT void DoFlush();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Matrix44 m_Projection; // 0x000
    Matrix44 m_Billboard; // 0x040
    Matrix44 Camera; // 0x080
    Matrix44 World; // 0x0C0
    i32 field_100; // 0x100
    i32 field_104; // 0x104
    i32 field_108; // 0x108
    i32 field_10C; // 0x10C
    i32 field_110; // 0x110
    i32 field_114; // 0x114
    i32 field_118; // 0x118
    i32 field_11C; // 0x11C
    i32 Flags; // 0x120
    f32 DefaultAspect; // 0x124
    i32 field_128; // 0x128
    f32 field_12C; // 0x12C
    f32 field_130; // 0x130
    f32 field_134; // 0x134
    f32 field_138; // 0x138
    f32 field_13C; // 0x13C
    f32 field_140; // 0x140
    f32 field_144; // 0x144
    f32 field_148; // 0x148
    f32 field_14C; // 0x14C
    f32 field_150; // 0x150
    f32 field_154; // 0x154
    f32 field_158; // 0x158
    D3DVIEWPORT7 m_Viewport; // 0x15C
    f32 Fov; // 0x174
    f32 Aspect; // 0x178
};

check_size(gfxViewport, 0x17C);
