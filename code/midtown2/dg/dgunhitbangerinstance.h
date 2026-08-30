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

#include "dgbangerinstance.h"

class Matrix34;
class Vector3;
class dgBangerData;
class dgHitBangerInstance;
class lvlInstance;

class dgUnhitBangerInstance : public dgBangerInstance
{
public:
    // ??0dgUnhitBangerInstance@@QAE@XZ
    ARTS_IMPORT dgUnhitBangerInstance();

    // ??1dgUnhitBangerInstance@@QAE@XZ
    ARTS_IMPORT ~dgUnhitBangerInstance();

    // ?Reset@dgUnhitBangerInstance@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@dgUnhitBangerInstance@@UAEHPBDABVMatrix34@@H@Z
    ARTS_IMPORT virtual i32 Init(const char* arg1, const Matrix34& arg2, i32 arg3);

    // ?Impact@dgUnhitBangerInstance@@UAEXPAVlvlInstance@@PAVVector3@@@Z
    ARTS_IMPORT virtual void Impact(lvlInstance* arg1, Vector3* arg2);

    // ?ImpactCB@dgUnhitBangerInstance@@UAEXPAVdgHitBangerInstance@@@Z
    ARTS_IMPORT virtual void ImpactCB(dgHitBangerInstance* arg1);

    // ?InitBound@dgUnhitBangerInstance@@QAE_NPBD0PAVdgBangerData@@H@Z
    ARTS_IMPORT bool InitBound(const char* arg1, const char* arg2, dgBangerData* arg3, i32 arg4);

    // ?InitBreakables@dgUnhitBangerInstance@@QAEXPBDPAVdgBangerData@@@Z
    ARTS_IMPORT void InitBreakables(const char* arg1, dgBangerData* arg2);

    // ?RequestBanger@dgUnhitBangerInstance@@SAPAV1@PBDH@Z
    ARTS_IMPORT static dgUnhitBangerInstance* RequestBanger(const char* arg1, i32 arg2);
};

// ??_7dgUnhitBangerInstance@@6B@
// vtable at 0x005B56D4
// check_size(dgUnhitBangerInstance, 0x18); // size known, members are not - cannot verify
