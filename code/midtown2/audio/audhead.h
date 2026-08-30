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

struct _audvector3_;
class audManager;

class audHead
{
public:
    // ?Create@audHead@@QAEPAV1@PAVaudManager@@@Z
    ARTS_IMPORT audHead* Create(audManager* amount);

    // ?Destroy@audHead@@QAEXXZ
    ARTS_IMPORT void Destroy();

    // ?GetHandle@audHead@@QAEHXZ
    ARTS_IMPORT i32 GetHandle();

    // ?GetHeadFacingDir@audHead@@QAEPAU_audvector3_@@XZ
    ARTS_IMPORT _audvector3_* GetHeadFacingDir();

    // ?GetHeadPosition@audHead@@QAEPAU_audvector3_@@XZ
    ARTS_IMPORT _audvector3_* GetHeadPosition();

    // ?GetHeadVelocity@audHead@@QAEPAU_audvector3_@@XZ
    ARTS_IMPORT _audvector3_* GetHeadVelocity();

    // ?GetHeadVolumePanPitch@audHead@@QAE_NPAU_audvector3_@@0PAM11@Z
    ARTS_IMPORT bool GetHeadVolumePanPitch(_audvector3_* timer, _audvector3_* index, f32* arg3, f32* arg4, f32* arg5);

    // ?Init@audHead@@QAEXXZ
    ARTS_IMPORT void Init();

    // ?SetHandle@audHead@@QAEXH@Z
    ARTS_IMPORT void SetHandle(i32 arg1);

    // ?SetHeadFacingDir@audHead@@QAEXPAU_audvector3_@@_N@Z
    ARTS_IMPORT void SetHeadFacingDir(_audvector3_* vec, bool arg2);

    // ?SetHeadLimits@audHead@@QAEXPAU_audvector3_@@0@Z
    ARTS_IMPORT void SetHeadLimits(_audvector3_* arg1, _audvector3_* arg2);

    // ?SetHeadMinLimit@audHead@@QAEXM@Z
    ARTS_IMPORT void SetHeadMinLimit(f32 arg1);

    // ?SetHeadPosition@audHead@@QAEXPAU_audvector3_@@@Z
    ARTS_IMPORT void SetHeadPosition(_audvector3_* arg1);

    // ?SetHeadTF@audHead@@QAEXPAU_audvector3_@@0@Z
    ARTS_IMPORT void SetHeadTF(_audvector3_* arg1, _audvector3_* arg2);

    // ?SetHeadVelocity@audHead@@QAEXPAU_audvector3_@@@Z
    ARTS_IMPORT void SetHeadVelocity(_audvector3_* arg1);
};

// check_size(audHead, 0x0); // TODO: no layout in the IDB type library
