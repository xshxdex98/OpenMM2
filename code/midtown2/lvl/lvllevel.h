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

#include "arts7/ascullable.h"
#include "data7/datcallback.h"

class Vector3;
class Vector4;
class gfxTexture;
class lvlInstance;
class lvlIntersection;
class lvlSegment;

class lvlLevel : public asCullable
{
public:
    // ??0lvlLevel@@QAE@XZ
    ARTS_IMPORT lvlLevel();

    // ??1lvlLevel@@UAE@XZ
    ARTS_IMPORT virtual ~lvlLevel();

    // ?Update@lvlLevel@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?GetVisitList@lvlLevel@@UAEHPAHHABVVector3@@1HH@Z
    ARTS_IMPORT virtual i32 GetVisitList(i32* arg1, i32 arg2, const Vector3& arg3, const Vector3& arg4, i32 arg5, i32 arg6);

    // ?Collide@lvlLevel@@UAE_NHAAVlvlSegment@@PAVlvlIntersection@@@Z
    ARTS_IMPORT virtual bool Collide(i32 arg1, lvlSegment& arg2, lvlIntersection* arg3);

    // ?GetBoundSphere@lvlLevel@@UAE_NAAVVector4@@H@Z
    ARTS_IMPORT virtual bool GetBoundSphere(Vector4& arg1, i32 arg2);

    // ?SetObjectDetail@lvlLevel@@UAEXH@Z
    ARTS_IMPORT virtual void SetObjectDetail(i32 arg1);

    // ?ClampToWorld@lvlLevel@@UAE_NAAVVector3@@@Z
    ARTS_IMPORT virtual bool ClampToWorld(Vector3& arg1);

    // ?LoadInstances@lvlLevel@@UAE_NPBD0@Z
    ARTS_IMPORT virtual bool LoadInstances(const char* arg1, const char* arg2);

    // ?GetEnvMap@lvlLevel@@UAEPAVgfxTexture@@HABVVector3@@AAM@Z
    ARTS_IMPORT virtual gfxTexture* GetEnvMap(i32 arg1, const Vector3& arg2, f32& arg3);

    // ?MoveToRoom@lvlLevel@@QAEXPAVlvlInstance@@H@Z
    ARTS_IMPORT void MoveToRoom(lvlInstance* arg1, i32 arg2);

    // ?RegisterDrawable@lvlLevel@@QAEXHVdatCallback@@@Z
    ARTS_IMPORT void RegisterDrawable(i32 arg1, datCallback arg2);

    // ?ResetCallbacks@lvlLevel@@QAEXXZ
    ARTS_IMPORT void ResetCallbacks();

    // ?ResetInstances@lvlLevel@@QAEXXZ
    ARTS_IMPORT void ResetInstances();

public:
    static lvlLevel*& Access_sm_Singleton() { return sm_Singleton; }
    static bool& Access_sm_PhysicsMode() { return sm_PhysicsMode; }

protected:
    // ?CallCallbacks@lvlLevel@@IAEXH@Z
    ARTS_IMPORT void CallCallbacks(i32 arg1);

    // ?sm_Singleton@lvlLevel@@1PAV1@A
    ARTS_IMPORT static lvlLevel* sm_Singleton;

    // ?sm_PhysicsMode@lvlLevel@@1_NA
    ARTS_IMPORT static bool sm_PhysicsMode;
};

// ??_7lvlLevel@@6B@
// vtable at 0x005B1A1C
// check_size(lvlLevel, 0x1C); // size known, members are not - cannot verify
