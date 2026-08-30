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

struct LocString;
class Vector4;
class mmPlayer;

class mmHUD : public asNode
{
public:
    // ??0mmHUD@@QAE@XZ
    ARTS_IMPORT mmHUD();

    // ??1mmHUD@@UAE@XZ
    ARTS_IMPORT virtual ~mmHUD();

    // ?Cull@mmHUD@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmHUD@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmHUD@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?ResChange@mmHUD@@UAEXHH@Z
    ARTS_IMPORT virtual void ResChange(i32 newWidth, i32 newHeight);

    // ?UpdatePaused@mmHUD@@UAEXXZ
    ARTS_IMPORT virtual void UpdatePaused();

    // ?ActivateDash@mmHUD@@QAEXXZ
    ARTS_IMPORT void ActivateDash();

    // ?ActivateGold@mmHUD@@QAEXXZ
    ARTS_IMPORT void ActivateGold();

    // ?AddPlayer@mmHUD@@QAEXPBDKHH@Z
    ARTS_IMPORT void AddPlayer(const char* arg1, ulong arg2, i32 arg3, i32 arg4);

    // ?DeactivateDash@mmHUD@@QAEXXZ
    ARTS_IMPORT void DeactivateDash();

    // ?DeactivateGold@mmHUD@@QAEXXZ
    ARTS_IMPORT void DeactivateGold();

    // ?Disable@mmHUD@@QAEXH@Z
    ARTS_IMPORT void Disable(i32 v);

    // ?Enable@mmHUD@@QAEXXZ
    ARTS_IMPORT void Enable();

    // ?GetPosHdg@mmHUD@@QAEXAAVVector4@@@Z
    ARTS_IMPORT void GetPosHdg(Vector4& arg1);

    // ?GetTime@mmHUD@@QAEXPADM@Z
    ARTS_IMPORT void GetTime(char* arg1, f32 window);

    // ?Init@mmHUD@@QAEXPADPAVmmPlayer@@H@Z
    ARTS_IMPORT void Init(char* window, mmPlayer* scale, i32 arg3);

    // ?IsDashActive@mmHUD@@QAEHXZ
    ARTS_IMPORT i32 IsDashActive();

    // ?PlayNetAlert@mmHUD@@QAEXXZ
    ARTS_IMPORT void PlayNetAlert();

    // ?PostChatMessage@mmHUD@@QAEXPAD@Z
    ARTS_IMPORT void PostChatMessage(char* c);

    // ?PostLapTime@mmHUD@@QAEX_N@Z
    ARTS_IMPORT void PostLapTime(bool arg1);

    // ?PostUpdate@mmHUD@@QAEXXZ
    ARTS_IMPORT void PostUpdate();

    // ?RemovePlayer@mmHUD@@QAEXK@Z
    ARTS_IMPORT void RemovePlayer(ulong arg1);

    // ?ResetTimers@mmHUD@@QAEXXZ
    ARTS_IMPORT void ResetTimers();

    // ?SetDash@mmHUD@@QAEXH@Z
    ARTS_IMPORT void SetDash(i32 arg1);

    // ?SetLapTime@mmHUD@@QAEXHH_N@Z
    ARTS_IMPORT void SetLapTime(i32 arg1, i32 arg2, bool arg3);

    // ?SetMessage@mmHUD@@QAEXPAULocString@@MH@Z
    ARTS_IMPORT void SetMessage(LocString* arg1, f32 window, i32 arg3);

    // ?SetMessage2@mmHUD@@QAEXPAULocString@@@Z
    ARTS_IMPORT void SetMessage2(LocString* arg1);

    // ?SetScore@mmHUD@@QAEXKH@Z
    ARTS_IMPORT void SetScore(ulong arg1, i32 arg2);

    // ?SetScore@mmHUD@@QAEXPAD@Z
    ARTS_IMPORT void SetScore(char* arg1);

    // ?SetStandings@mmHUD@@QAEXHH@Z
    ARTS_IMPORT void SetStandings(i32 arg1, i32 arg2);

    // ?SetTransparency@mmHUD@@QAEX_N@Z
    ARTS_IMPORT void SetTransparency(bool arg1);

    // ?SetWPCleared@mmHUD@@QAEXHH@Z
    ARTS_IMPORT void SetWPCleared(i32 arg1, i32 arg2);

    // ?ShowSplitTime@mmHUD@@QAEXXZ
    ARTS_IMPORT void ShowSplitTime();

    // ?StartTimers@mmHUD@@QAEXXZ
    ARTS_IMPORT void StartTimers();

    // ?StopTimers@mmHUD@@QAEXXZ
    ARTS_IMPORT void StopTimers();

    // ?Toggle@mmHUD@@QAEXXZ
    ARTS_IMPORT void Toggle();

    // ?ToggleExternalView@mmHUD@@QAEXXZ
    ARTS_IMPORT void ToggleExternalView();

    // ?TogglePositionDisplay@mmHUD@@QAEXH@Z
    ARTS_IMPORT void TogglePositionDisplay(i32 arg1);
};

// ??_7mmHUD@@6B@
// vtable at 0x005B0C20
// check_size(mmHUD, 0xBBC); // size known, members are not - cannot verify
