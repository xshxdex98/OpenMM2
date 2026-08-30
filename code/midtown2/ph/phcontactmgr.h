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
class Vector3;
class phColliderBase;
class phHeldContact;
class phImpact;

class phContactMgr
{
public:
    // ??0phContactMgr@@QAE@XZ
    ARTS_IMPORT phContactMgr();

    // ??1phContactMgr@@QAE@XZ
    ARTS_IMPORT ~phContactMgr();

    // ?AllocNewContact@phContactMgr@@QAEJJ@Z
    ARTS_IMPORT ilong AllocNewContact(ilong arg1);

    // ?ApplyImpact@phContactMgr@@QAEXPAVphImpact@@ABVVector3@@@Z
    ARTS_IMPORT void ApplyImpact(phImpact* arg1, const Vector3& arg2);

    // ?Calc2Impacts@phContactMgr@@QAEXPAVphImpact@@@Z
    ARTS_IMPORT void Calc2Impacts(phImpact* arg1);

    // ?Calc2ImpactsFixed@phContactMgr@@QAEXPAVphImpact@@_N@Z
    ARTS_IMPORT void Calc2ImpactsFixed(phImpact* arg1, bool arg2);

    // ?Calc3Impacts@phContactMgr@@QAEXHPAVphImpact@@@Z
    ARTS_IMPORT void Calc3Impacts(i32 arg1, phImpact* arg2);

    // ?Calc3ImpactsFixed@phContactMgr@@QAEXHPAVphImpact@@_N@Z
    ARTS_IMPORT void Calc3ImpactsFixed(i32 arg1, phImpact* arg2, bool arg3);

    // ?CalcContact@phContactMgr@@QAE_NAAVphImpact@@ABVVector3@@111PAV3@PAVMatrix34@@@Z
    ARTS_IMPORT bool CalcContact(phImpact& arg1, const Vector3& arg2, const Vector3& arg3, const Vector3& arg4, const Vector3& arg5, Vector3* arg6, Matrix34* arg7);

    // ?CalcContactHash@phContactMgr@@QAEJHHHJJ@Z
    ARTS_IMPORT ilong CalcContactHash(i32 arg1, i32 arg2, i32 arg3, ilong arg4, ilong arg5);

    // ?CalcHeldContact@phContactMgr@@QAEXHAAJABVVector3@@11PAV2@PAVMatrix34@@@Z
    ARTS_IMPORT void CalcHeldContact(i32 arg1, ilong& arg2, const Vector3& arg3, const Vector3& arg4, const Vector3& arg5, Vector3* arg6, Matrix34* arg7);

    // ?CalcHeldContacts@phContactMgr@@QAEXPAVphColliderBase@@0H@Z
    ARTS_IMPORT void CalcHeldContacts(phColliderBase* arg1, phColliderBase* arg2, i32 arg3);

    // ?CalcImpact@phContactMgr@@QAEXPAVphImpact@@M@Z
    ARTS_IMPORT void CalcImpact(phImpact* arg1, f32 arg2);

    // ?CalcNextOverSamples@phContactMgr@@QAEXXZ
    ARTS_IMPORT void CalcNextOverSamples();

    // ?CullImpactList@phContactMgr@@QAEXPAHPAVphImpact@@@Z
    ARTS_IMPORT void CullImpactList(i32* arg1, phImpact* arg2);

    // ?FindNextContact@phContactMgr@@QAE_NJPAJ0@Z
    ARTS_IMPORT bool FindNextContact(ilong arg1, ilong* arg2, ilong* arg3);

    // ?GetCMInvSeconds@phContactMgr@@QBEMXZ
    ARTS_IMPORT f32 GetCMInvSeconds() const;

    // ?GetCMSeconds@phContactMgr@@QBEMXZ
    ARTS_IMPORT f32 GetCMSeconds() const;

    // ?RemoveContact@phContactMgr@@QAEXJ@Z
    ARTS_IMPORT void RemoveContact(ilong arg1);

    // ?Reset@phContactMgr@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Resize@phContactMgr@@QAEXXZ
    ARTS_IMPORT void Resize();

    // ?ResolveThreePlusImpacts@phContactMgr@@QAEXHPAVphImpact@@ABVVector3@@11_N@Z
    ARTS_IMPORT void ResolveThreePlusImpacts(i32 arg1, phImpact* arg2, const Vector3& arg3, const Vector3& arg4, const Vector3& arg5, bool arg6);

    // ?SearchContact@phContactMgr@@QAE_NJPAJ@Z
    ARTS_IMPORT bool SearchContact(ilong arg1, ilong* arg2);

    // ?SeekContact@phContactMgr@@QAEJJABVphImpact@@@Z
    ARTS_IMPORT ilong SeekContact(ilong arg1, const phImpact& arg2);

    // ?SeekHeldContact@phContactMgr@@QAEHJJH@Z
    ARTS_IMPORT i32 SeekHeldContact(ilong arg1, ilong arg2, i32 arg3);

    // ?SetCMOversampleTime@phContactMgr@@QAEXM@Z
    ARTS_IMPORT void SetCMOversampleTime(f32 arg1);

    // ?TestHeldContact@phContactMgr@@QAE_NPBVphColliderBase@@0PAH@Z
    ARTS_IMPORT bool TestHeldContact(const phColliderBase* arg1, const phColliderBase* arg2, i32* arg3);

    // ?Update@phContactMgr@@QAEXXZ
    ARTS_IMPORT void Update();

private:
    // ?AddHCEntry@phContactMgr@@AAE_NHJ@Z
    ARTS_IMPORT bool AddHCEntry(i32 arg1, ilong arg2);

    // ?AddHeldContact@phContactMgr@@AAEHPBVphColliderBase@@0@Z
    ARTS_IMPORT i32 AddHeldContact(const phColliderBase* arg1, const phColliderBase* arg2);

    // ?ClearContactList@phContactMgr@@AAEXXZ
    ARTS_IMPORT void ClearContactList();

    // ?ClearHeldContactTable@phContactMgr@@AAEXXZ
    ARTS_IMPORT void ClearHeldContactTable();

    // ?EstMaxMoved@phContactMgr@@CAMABVMatrix34@@PBVphColliderBase@@1@Z
    ARTS_IMPORT static f32 EstMaxMoved(const Matrix34& arg1, const phColliderBase* arg2, const phColliderBase* arg3);

    // ?GetNextHeldContact@phContactMgr@@AAE_NHPAHPAJPA_N@Z
    ARTS_IMPORT bool GetNextHeldContact(i32 arg1, i32* arg2, ilong* arg3, bool* arg4);

    // ?HCContactGone@phContactMgr@@AAE_NPAVphHeldContact@@@Z
    ARTS_IMPORT bool HCContactGone(phHeldContact* arg1);

    // ?HeldContactResize@phContactMgr@@AAEXXZ
    ARTS_IMPORT void HeldContactResize();

    // ?RemoveHeldContact@phContactMgr@@AAEXH@Z
    ARTS_IMPORT void RemoveHeldContact(i32 arg1);
};

// check_size(phContactMgr, 0x0); // TODO: no layout in the IDB type library
