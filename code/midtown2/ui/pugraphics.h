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

#include "pumenubase.h"

class PUGraphics : public PUMenuBase
{
public:
    // ??0PUGraphics@@QAE@HMMMM@Z
    ARTS_IMPORT PUGraphics(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);

    // ??1PUGraphics@@UAE@XZ
    ARTS_IMPORT virtual ~PUGraphics();

    // ?PreSetup@PUGraphics@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?CancelAction@PUGraphics@@QAEXXZ
    ARTS_IMPORT void CancelAction();

    // ?FixClip@PUGraphics@@QAEXXZ
    ARTS_IMPORT void FixClip();

    // ?RenderQualityCB@PUGraphics@@QAEXXZ
    ARTS_IMPORT void RenderQualityCB();

    // ?SetFarClip@PUGraphics@@QAEXM@Z
    ARTS_IMPORT void SetFarClip(f32 arg1);
};

// ??_7PUGraphics@@6B@
// vtable at 0x005B46D8
// check_size(PUGraphics, 0x0); // TODO: no layout in the IDB type library
