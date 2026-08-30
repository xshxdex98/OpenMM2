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
class Stream;
union eqEvent;
class mmIO;

class mmInput : public asNode
{
public:
    // ??0mmInput@@QAE@XZ
    ARTS_IMPORT mmInput();

    // ??1mmInput@@UAE@XZ
    ARTS_IMPORT virtual ~mmInput();

    // ?Update@mmInput@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmInput@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?AssignIO@mmInput@@QAEXPAVmmIO@@HH@Z
    ARTS_IMPORT void AssignIO(mmIO* arg1, i32 arg2, i32 arg3);

    // ?AttachToPipe@mmInput@@QAEHXZ
    ARTS_IMPORT i32 AttachToPipe();

    // ?AutoSetup@mmInput@@QAEXXZ
    ARTS_IMPORT void AutoSetup();

    // ?BinaryLoadConfig@mmInput@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 BinaryLoadConfig(Stream* getter1);

    // ?BinarySaveConfig@mmInput@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 BinarySaveConfig(Stream* arg1);

    // ?BuildCaptureIO@mmInput@@QAEHHPAVmmIO@@TeqEvent@@@Z
    ARTS_IMPORT i32 BuildCaptureIO(i32 arg1, mmIO* arg2, eqEvent arg3);

    // ?CaptureState@mmInput@@QAEXH@Z
    ARTS_IMPORT void CaptureState(i32 arg1);

    // ?ClearEventHitFlags@mmInput@@QAEXXZ
    ARTS_IMPORT void ClearEventHitFlags();

    // ?DeviceConnected@mmInput@@QAEHXZ
    ARTS_IMPORT i32 DeviceConnected();

    // ?DoingFF@mmInput@@QAEHXZ
    ARTS_IMPORT i32 DoingFF();

    // ?FFIsPlaying@mmInput@@QAEHH@Z
    ARTS_IMPORT i32 FFIsPlaying(i32 arg1);

    // ?FFPlay@mmInput@@QAEHH@Z
    ARTS_IMPORT i32 FFPlay(i32 ys);

    // ?FFSetValues@mmInput@@QAEHHMM@Z
    ARTS_IMPORT i32 FFSetValues(i32 arg1, f32 arg2, f32 arg3);

    // ?FFStop@mmInput@@QAEHH@Z
    ARTS_IMPORT i32 FFStop(i32 arg1);

    // ?FilterGamepadSteering@mmInput@@QAEMXZ
    ARTS_IMPORT f32 FilterGamepadSteering();

    // ?FlagIODevChanged@mmInput@@QAEXXZ
    ARTS_IMPORT void FlagIODevChanged();

    // ?Flush@mmInput@@QAEXXZ
    ARTS_IMPORT void Flush();

    // ?ForceAssignment@mmInput@@QAEXPAVmmIO@@@Z
    ARTS_IMPORT void ForceAssignment(mmIO* arg1);

    // ?GamepadConnected@mmInput@@QAEHXZ
    ARTS_IMPORT i32 GamepadConnected();

    // ?GetBrakes@mmInput@@QAEMXZ
    ARTS_IMPORT f32 GetBrakes();

    // ?GetBrakesVal@mmInput@@QAEMXZ
    ARTS_IMPORT f32 GetBrakesVal();

    // ?GetCamPan@mmInput@@QAEMXZ
    ARTS_IMPORT f32 GetCamPan();

    // ?GetHandBrake@mmInput@@QAEMXZ
    ARTS_IMPORT f32 GetHandBrake();

    // ?GetSteering@mmInput@@QAEMP6AMM@Z@Z
    ARTS_IMPORT f32 GetSteering(f32 (__cdecl* ys)(f32));

    // ?GetThrottle@mmInput@@QAEMXZ
    ARTS_IMPORT f32 GetThrottle();

    // ?GetThrottleVal@mmInput@@QAEMXZ
    ARTS_IMPORT f32 GetThrottleVal();

    // ?Init@mmInput@@QAEXH@Z
    ARTS_IMPORT void Init(i32 getter);

    // ?IOInit@mmInput@@QAEXHPAULocString@@J@Z
    ARTS_IMPORT void IOInit(i32 arg1, LocString* arg2, ilong arg3);

    // ?IsAlreadyAssigned@mmInput@@QAEHPAVmmIO@@HHH@Z
    ARTS_IMPORT i32 IsAlreadyAssigned(mmIO* arg1, i32 arg2, i32 arg3, i32 arg4);

    // ?JoystickConnected@mmInput@@QAEHXZ
    ARTS_IMPORT i32 JoystickConnected();

    // ?JoystickHasCoolie@mmInput@@QAEHXZ
    ARTS_IMPORT i32 JoystickHasCoolie();

    // ?JoystickHasThrottle@mmInput@@QAEHXZ
    ARTS_IMPORT i32 JoystickHasThrottle();

    // ?LoadConfig@mmInput@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 LoadConfig(Stream* getter1);

    // ?PollStates@mmInput@@QAEHXZ
    ARTS_IMPORT i32 PollStates();

    // ?PollSuperQ@mmInput@@QAEHXZ
    ARTS_IMPORT i32 PollSuperQ();

    // ?PopEvent@mmInput@@QAEHPAH@Z
    ARTS_IMPORT i32 PopEvent(i32* getter1);

    // ?PrintIODev@mmInput@@QAEXXZ
    ARTS_IMPORT void PrintIODev();

    // ?RestoreDefaultConfig@mmInput@@QAEXH@Z
    ARTS_IMPORT void RestoreDefaultConfig(i32 arg1);

    // ?ReturnStateCaptured@mmInput@@QAEHXZ
    ARTS_IMPORT i32 ReturnStateCaptured();

    // ?SanityCheck@mmInput@@QAEHPAVmmIO@@HH@Z
    ARTS_IMPORT i32 SanityCheck(mmIO* arg1, i32 arg2, i32 arg3);

    // ?SaveCB@mmInput@@QAEXXZ
    ARTS_IMPORT void SaveCB();

    // ?SaveCodeConfig@mmInput@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 SaveCodeConfig(Stream* getter1);

    // ?SaveConfig@mmInput@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 SaveConfig(Stream* getter1);

    // ?SetDeadZone@mmInput@@QAEXM@Z
    ARTS_IMPORT void SetDeadZone(f32 arg1);

    // ?SetDefaultConfig@mmInput@@QAEXH@Z
    ARTS_IMPORT void SetDefaultConfig(i32 getter);

    // ?SetForceFeedbackScale@mmInput@@QAEXM@Z
    ARTS_IMPORT void SetForceFeedbackScale(f32 ys);

    // ?SetRoadForceScale@mmInput@@QAEXM@Z
    ARTS_IMPORT void SetRoadForceScale(f32 ys);

    // ?StopAllFF@mmInput@@QAEXXZ
    ARTS_IMPORT void StopAllFF();

    // ?ToggleFFEnabled@mmInput@@QAEHH@Z
    ARTS_IMPORT i32 ToggleFFEnabled(i32 arg1);

    // ?WheelConnected@mmInput@@QAEHXZ
    ARTS_IMPORT i32 WheelConnected();

    // ?WheelHas3Axis@mmInput@@QAEHXZ
    ARTS_IMPORT i32 WheelHas3Axis();

private:
    // ?EventToButton@mmInput@@AAEHH@Z
    ARTS_IMPORT i32 EventToButton(i32 arg1);

    // ?FilterDiscreteSteering@mmInput@@AAEMM@Z
    ARTS_IMPORT f32 FilterDiscreteSteering(f32 xs);

    // ?GetBufferedKeyboardData@mmInput@@AAEXXZ
    ARTS_IMPORT void GetBufferedKeyboardData();

    // ?GetNextKeyboardEvent@mmInput@@AAEHPATeqEvent@@@Z
    ARTS_IMPORT i32 GetNextKeyboardEvent(eqEvent* arg1);

    // ?PollContinuous@mmInput@@AAEXPAVmmIO@@@Z
    ARTS_IMPORT void PollContinuous(mmIO* xs);

    // ?ProcessEvents@mmInput@@AAEXXZ
    ARTS_IMPORT void ProcessEvents();

    // ?ProcessJoyEvents@mmInput@@AAEXXZ
    ARTS_IMPORT void ProcessJoyEvents();

    // ?ProcessKeyboardEvents@mmInput@@AAEXXZ
    ARTS_IMPORT void ProcessKeyboardEvents();

    // ?ProcessMouseEvents@mmInput@@AAEXXZ
    ARTS_IMPORT void ProcessMouseEvents();

    // ?ProcessStates@mmInput@@AAE_JXZ
    ARTS_IMPORT i64 ProcessStates();

    // ?PutEventInQueue@mmInput@@AAEX_J@Z
    ARTS_IMPORT void PutEventInQueue(i64 arg1);

    // ?ScanForEvent@mmInput@@AAE_JPATeqEvent@@@Z
    ARTS_IMPORT i64 ScanForEvent(eqEvent* xs);

    // ?ScanState@mmInput@@AAE_JPAVmmIO@@@Z
    ARTS_IMPORT i64 ScanState(mmIO* xs);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_18; // 0x018
    u32 field_1C; // 0x01C
    u8 field_20[4]; // 0x020
    u32 field_24; // 0x024
    u8 field_28[284]; // 0x028
    u32 field_144; // 0x144
    u32 field_148; // 0x148
    u32 field_14C; // 0x14C
    u8 field_150[16]; // 0x150
    u32 field_160; // 0x160
    u8 field_164[4]; // 0x164
    f32 field_168; // 0x168
    f32 field_16C; // 0x16C
    u8 field_170[20]; // 0x170
    u32 field_184; // 0x184
    u32 field_188; // 0x188
    u32 field_18C; // 0x18C
    u8 field_190[20]; // 0x190
    u32 field_1A4; // 0x1A4
    f32 field_1A8; // 0x1A8
    f32 field_1AC; // 0x1AC
    f32 field_1B0; // 0x1B0
    f32 field_1B4; // 0x1B4
    f32 field_1B8; // 0x1B8
    f32 field_1BC; // 0x1BC
    f32 field_1C0; // 0x1C0
    f32 field_1C4; // 0x1C4
    f32 field_1C8; // 0x1C8
    f32 field_1CC; // 0x1CC
    f32 field_1D0; // 0x1D0
    u32 field_1D4; // 0x1D4
    u32 field_1D8; // 0x1D8
    i32 field_1DC; // 0x1DC
    i32 field_1E0; // 0x1E0
    f32 field_1E4; // 0x1E4
    f32 field_1E8; // 0x1E8
    f32 field_1EC; // 0x1EC
    f32 field_1F0; // 0x1F0
    i32 field_1F4; // 0x1F4
    i32 field_1F8; // 0x1F8
    u32 field_1FC; // 0x1FC
    i8 KeyboardBuffer[64]; // 0x200
    i32 KeyboardBufferCount; // 0x240
    u32 field_244; // 0x244
};

// ??_7mmInput@@6B@
// vtable at 0x005B5250
check_size(mmInput, 0x248);
