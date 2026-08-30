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

class Matrix34;
class Stream;
class Vector3;

class Aud3DObject
{
public:
    // ??0Aud3DObject@@QAE@XZ
    ARTS_IMPORT Aud3DObject();

    // ??1Aud3DObject@@UAE@XZ
    ARTS_IMPORT virtual ~Aud3DObject();

    // ?AssignSounds@Aud3DObject@@UAEXXZ
    ARTS_EXPORT virtual void AssignSounds();

    // ?UnAssignSounds@Aud3DObject@@UAEXH@Z
    ARTS_EXPORT virtual void UnAssignSounds(i32 arg1);

    // ?UpdateAudio@Aud3DObject@@UAEXXZ
    ARTS_EXPORT virtual void UpdateAudio();

    // ?Update@Aud3DObject@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?SetNon3DParams@Aud3DObject@@UAEXXZ
    ARTS_EXPORT virtual void SetNon3DParams();

    // ?Set3DParams@Aud3DObject@@UAEXXZ
    ARTS_EXPORT virtual void Set3DParams();

    // ?AddTo3DMgr@Aud3DObject@@QAEXXZ
    ARTS_IMPORT void AddTo3DMgr();

    // ?CalcDistToClosestHeads2@Aud3DObject@@QAEXXZ
    ARTS_IMPORT void CalcDistToClosestHeads2();

    // ?CalcMultiPlayerPan@Aud3DObject@@QAEXXZ
    ARTS_IMPORT void CalcMultiPlayerPan();

    // ?CalcSinglePlayerPan@Aud3DObject@@QAEXXZ
    ARTS_IMPORT void CalcSinglePlayerPan();

    // ?CalculateAttenuation@Aud3DObject@@QAEMXZ
    ARTS_IMPORT f32 CalculateAttenuation();

    // ?CalculateDoppler@Aud3DObject@@QAEMM@Z
    ARTS_IMPORT f32 CalculateDoppler(f32 arg1);

    // ?CalculatePan@Aud3DObject@@QAEMXZ
    ARTS_IMPORT f32 CalculatePan();

    // ?GetDistToClosestHead2@Aud3DObject@@QAEMXZ
    ARTS_IMPORT f32 GetDistToClosestHead2();

    // ?GetLeftPositionPtr@Aud3DObject@@QAEPAVVector3@@XZ
    ARTS_IMPORT Vector3* GetLeftPositionPtr();

    // ?GetPriority@Aud3DObject@@QAEHXZ
    ARTS_IMPORT i32 GetPriority();

    // ?GetRightPositionPtr@Aud3DObject@@QAEPAVVector3@@XZ
    ARTS_IMPORT Vector3* GetRightPositionPtr();

    // ?PastMaxDistance@Aud3DObject@@QAE_NXZ
    ARTS_IMPORT bool PastMaxDistance();

    // ?ReadVectorPoints@Aud3DObject@@QAE_NPAVStream@@@Z
    ARTS_IMPORT bool ReadVectorPoints(Stream* arg1);

    // ?RemoveFrom3DMgr@Aud3DObject@@QAEXXZ
    ARTS_IMPORT void RemoveFrom3DMgr();

    // ?Reset@Aud3DObject@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Set3D@Aud3DObject@@QAEX_N@Z
    ARTS_IMPORT void Set3D(bool arg1);

    // ?SetDropOffs@Aud3DObject@@QAEXMM@Z
    ARTS_IMPORT void SetDropOffs(f32 arg1, f32 arg2);

    // ?SetPositionPtr@Aud3DObject@@QAEXPAVVector3@@@Z
    ARTS_IMPORT void SetPositionPtr(Vector3* arg1);

    // ?UpdateNonVirtual@Aud3DObject@@QAEXXZ
    ARTS_IMPORT void UpdateNonVirtual();

    // ?WithinMaxDistance@Aud3DObject@@QAE_NM@Z
    ARTS_IMPORT bool WithinMaxDistance(f32 arg1);

    // ?WithinMaxDistance@Aud3DObject@@QAE_NXZ
    ARTS_IMPORT bool WithinMaxDistance();

private:
    // ?CalcDistToClosestHead2@Aud3DObject@@AAEMPAPAVMatrix34@@PAVVector3@@H@Z
    ARTS_IMPORT f32 CalcDistToClosestHead2(Matrix34** arg1, Vector3* arg2, i32 arg3);

    // ?CalcDistToHead2@Aud3DObject@@AAEMPAVMatrix34@@PAVVector3@@@Z
    ARTS_IMPORT f32 CalcDistToHead2(Matrix34* arg1, Vector3* arg2);

    // ?CalcPercentToMaxDist2@Aud3DObject@@AAEMM@Z
    ARTS_IMPORT f32 CalcPercentToMaxDist2(f32 arg1);

    // ?CalcPseudoDistToClosestHead@Aud3DObject@@AAEMPAPAVMatrix34@@PAVVector3@@H@Z
    ARTS_IMPORT f32 CalcPseudoDistToClosestHead(Matrix34** arg1, Vector3* arg2, i32 arg3);

    // ?CalcPseudoDistToHead@Aud3DObject@@AAEMPAVMatrix34@@PAVVector3@@@Z
    ARTS_IMPORT f32 CalcPseudoDistToHead(Matrix34* arg1, Vector3* arg2);

    // ?GetClosestPositionPtr@Aud3DObject@@AAEPAVVector3@@PAPAVMatrix34@@H@Z
    ARTS_IMPORT Vector3* GetClosestPositionPtr(Matrix34** arg1, i32 arg2);

    // ?SetClosestPositionPtr@Aud3DObject@@AAEXXZ
    ARTS_IMPORT void SetClosestPositionPtr();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 Pan; // 0x004
    f32 field_8; // 0x008
    f32 Attenuation; // 0x00C
    f32 LastDoppler; // 0x010
    f32 MinDropOffSqr; // 0x014
    f32 MaxDropOffSqr; // 0x018
    i32 field_1c; // 0x01C
    f32 PositionPtrPseudoDistChangeRate; // 0x020
    f32 PositionPtrPseudoDist; // 0x024
    f32 PositionPtrLastPseudoDist; // 0x028
    f32 LeftPositionPtrDistSqr; // 0x02C
    f32 RightPositionPtrDistSqr; // 0x030
    f32 DropOffDivisor; // 0x034
    f32 LeftPercentToMaxDist; // 0x038
    f32 RightPercentToMaxDist; // 0x03C
    i32 Priority; // 0x040
    i32 ObjectId3D; // 0x044
    bool Is3D; // 0x048
    u8 field_49; // 0x049
    u8 field_4a; // 0x04A
    u8 field_4b; // 0x04B
    Vector3* LeftPositionPtr; // 0x04C
    Vector3* RightPositionPtr; // 0x050
    Vector3** VectorPoints; // 0x054
    i32 VectorPointMode; // 0x058
    i32 NumVectorPoints; // 0x05C
};

// ??_7Aud3DObject@@6B@
// vtable at 0x005B4CCC
check_size(Aud3DObject, 0x60);
