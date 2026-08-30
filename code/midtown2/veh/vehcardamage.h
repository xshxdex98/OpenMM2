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

#include "arts7/asnode.h"
#include "data7/datcallback.h"
#include "vector7/vector3.h"

struct ImpactCBData;
class asBirthRule;
class asLineSparks;
class asParticles;
class datParser;
class fxShardManager;
class phCollider;
class vehCar;
struct vehDamageImpactInfo;

class vehCarDamage : public asNode
{
public:
    // ??0vehCarDamage@@QAE@XZ
    ARTS_IMPORT vehCarDamage();

    // ??1vehCarDamage@@UAE@XZ
    ARTS_IMPORT virtual ~vehCarDamage();

    // ?AddDamage@vehCarDamage@@QAEXM@Z
    ARTS_IMPORT void AddDamage(f32 arg1);

    // ?ClearDamage@vehCarDamage@@QAEXXZ
    ARTS_IMPORT void ClearDamage();

    // ?FileIO@vehCarDamage@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@vehCarDamage@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?GetDamageModifier@vehCarDamage@@UAEMPAVphCollider@@@Z
    ARTS_IMPORT virtual f32 GetDamageModifier(phCollider* arg1);

    // ?GetDirName@vehCarDamage@@UAEPBDXZ
    ARTS_IMPORT virtual const char* GetDirName();

    // ?Impact@vehCarDamage@@SAXPAV1@PAUImpactCBData@@@Z
    ARTS_IMPORT static void Impact(vehCarDamage* arg1, ImpactCBData* arg2);

    // ?Init@vehCarDamage@@QAEXPAVvehCar@@PBD@Z
    ARTS_IMPORT void Init(vehCar* arg1, const char* arg2);

    // ?Reset@vehCarDamage@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetGameCallback@vehCarDamage@@QAEXVdatCallback@@@Z
    ARTS_IMPORT void SetGameCallback(datCallback arg1);

    // ?Update@vehCarDamage@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    static f32& Access_RelaxTime() { return RelaxTime; }
    static f32& Access_ParticleMultiplier() { return ParticleMultiplier; }
    static bool& Access_bWobble() { return bWobble; }
    static asBirthRule*& Access_EngineSmokeRule() { return EngineSmokeRule; }
    static i32& Access_EngineSmokeRuleRef() { return EngineSmokeRuleRef; }

private:
    // ?ApplyImpact@vehCarDamage@@AAEXPAUvehDamageImpactInfo@@@Z
    ARTS_IMPORT void ApplyImpact(vehDamageImpactInfo* arg1);

    // ?InsertImpact@vehCarDamage@@AAEXPAUImpactCBData@@PAVphCollider@@@Z
    ARTS_IMPORT void InsertImpact(ImpactCBData* arg1, phCollider* arg2);

    // ?SpewSmoke@vehCarDamage@@AAEXPAVasParticles@@PAMAAVVector3@@M@Z
    ARTS_IMPORT void SpewSmoke(asParticles* arg1, f32* arg2, Vector3& arg3, f32 arg4);

    // ?RelaxTime@vehCarDamage@@0MA
    ARTS_IMPORT static f32 RelaxTime;

    // ?ParticleMultiplier@vehCarDamage@@0MA
    ARTS_IMPORT static f32 ParticleMultiplier;

    // ?bWobble@vehCarDamage@@0_NA
    ARTS_IMPORT static bool bWobble;

    // ?EngineSmokeRule@vehCarDamage@@0PAVasBirthRule@@A
    ARTS_IMPORT static asBirthRule* EngineSmokeRule;

    // ?EngineSmokeRuleRef@vehCarDamage@@0HA
    ARTS_IMPORT static i32 EngineSmokeRuleRef;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    datCallback* ImpactCallback; // 0x018
    datCallback* Callback; // 0x01C
    vehCar* pCar; // 0x020
    u8 DoublePivot; // 0x024
    u8 MirrorPivot; // 0x025
    u32 field_28; // 0x028
    u8 field_2C; // 0x02C
    u8 field_2D; // 0x02D
    f32 Damage; // 0x030
    f32 MaxDamage; // 0x034
    f32 MedDamage; // 0x038
    f32 ImpactThreshold; // 0x03C
    f32 RegenerateRate; // 0x040
    vehDamageImpactInfo DamagePackets[12]; // 0x044
    f32 field_344; // 0x344
    Vector3 SmokeOffset; // 0x348
    Vector3 SmokeOffset2; // 0x354
    asParticles* pParticles; // 0x360
    u8 field_364; // 0x364
    Vector3 field_368; // 0x368
    Vector3 field_374; // 0x374
    f32 field_380; // 0x380
    asLineSparks* pSparks; // 0x384
    f32 TextelDamageRadius; // 0x388
    fxShardManager* pShardManager; // 0x38C
    f32 field_390; // 0x390
    u8 PendingDamage; // 0x394
    Vector3 PendingDamagePos; // 0x398
};

// ??_7vehCarDamage@@6B@
// vtable at 0x005B2C28
check_size(vehCarDamage, 0x3A4);
