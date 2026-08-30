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

struct HWND__;
struct IDirectInputDevice2A;
class mmEffectFF;

class mmJoystick
{
public:
    // ??0mmJoystick@@QAE@XZ
    ARTS_IMPORT mmJoystick();

    // ??1mmJoystick@@QAE@XZ
    ARTS_IMPORT ~mmJoystick();

    // ?DisableAutoCenter@mmJoystick@@QAEHXZ
    ARTS_IMPORT i32 DisableAutoCenter();

    // ?GetAxis@mmJoystick@@QAEMH@Z
    ARTS_IMPORT f32 GetAxis(i32 getter);

    // ?GetButton@mmJoystick@@QAEMH@Z
    ARTS_IMPORT f32 GetButton(i32 arg1);

    // ?GetFFEffect@mmJoystick@@QAEPAVmmEffectFF@@H@Z
    ARTS_IMPORT mmEffectFF* GetFFEffect(i32 getter);

    // ?GetNumButtons@mmJoystick@@QAEHXZ
    ARTS_IMPORT i32 GetNumButtons();

    // ?Init@mmJoystick@@QAEHPAUIDirectInputDevice2A@@PAUHWND__@@@Z
    ARTS_IMPORT i32 Init(IDirectInputDevice2A* arg1, HWND__* arg2);

    // ?InputCreateEffect@mmJoystick@@QAEXXZ
    ARTS_IMPORT void InputCreateEffect();

    // ?InputInitEffect@mmJoystick@@QAEXXZ
    ARTS_IMPORT void InputInitEffect();

    // ?inputPrepareDevice@mmJoystick@@QAEHXZ
    ARTS_IMPORT i32 inputPrepareDevice();

    // ?InputStopEffect@mmJoystick@@QAEXXZ
    ARTS_IMPORT void InputStopEffect();

    // ?PlayCollision@mmJoystick@@QAEHXZ
    ARTS_IMPORT i32 PlayCollision();

    // ?PlayFriction@mmJoystick@@QAEHXZ
    ARTS_IMPORT i32 PlayFriction();

    // ?PlayShake@mmJoystick@@QAEHXZ
    ARTS_IMPORT i32 PlayShake();

    // ?PlaySteer@mmJoystick@@QAEHXZ
    ARTS_IMPORT i32 PlaySteer();

    // ?Poll@mmJoystick@@QAEKXZ
    ARTS_IMPORT ulong Poll();

    // ?PrintDeviceCaps@mmJoystick@@QAEXXZ
    ARTS_IMPORT void PrintDeviceCaps();

    // ?ResetAxisCapture@mmJoystick@@QAEXXZ
    ARTS_IMPORT void ResetAxisCapture();

    // ?SetDeadZone@mmJoystick@@QAEHM@Z
    ARTS_IMPORT i32 SetDeadZone(f32 arg1);

    // ?SetFriction@mmJoystick@@QAEHJ@Z
    ARTS_IMPORT i32 SetFriction(ilong arg1);

    // ?SetShake@mmJoystick@@QAEHKK@Z
    ARTS_IMPORT i32 SetShake(ulong arg1, ulong arg2);

    // ?SetSteer@mmJoystick@@QAEHJ@Z
    ARTS_IMPORT i32 SetSteer(ilong arg1);

    // ?StopFriction@mmJoystick@@QAEHXZ
    ARTS_IMPORT i32 StopFriction();

    // ?StopShake@mmJoystick@@QAEHXZ
    ARTS_IMPORT i32 StopShake();

    // ?StopSteer@mmJoystick@@QAEHXZ
    ARTS_IMPORT i32 StopSteer();

    // ?Update@mmJoystick@@QAEXXZ
    ARTS_IMPORT void Update();
};

// check_size(mmJoystick, 0x0); // TODO: no layout in the IDB type library
