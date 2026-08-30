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

class Dialog_RaceEnvironment : public PUMenuBase
{
public:
    // ??0Dialog_RaceEnvironment@@QAE@HMMMMPAD@Z
    ARTS_IMPORT Dialog_RaceEnvironment(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, char* arg6);

    // ??1Dialog_RaceEnvironment@@UAE@XZ
    ARTS_IMPORT virtual ~Dialog_RaceEnvironment();

    // ?PreSetup@Dialog_RaceEnvironment@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?CancelCallback@Dialog_RaceEnvironment@@QAEXXZ
    ARTS_IMPORT void CancelCallback();

    // ?ControlPedSlider@Dialog_RaceEnvironment@@QAEXXZ
    ARTS_IMPORT void ControlPedSlider();

    // ?DoneCallback@Dialog_RaceEnvironment@@QAEXXZ
    ARTS_IMPORT void DoneCallback();

    // ?SetMultiRaceOptions@Dialog_RaceEnvironment@@QAEXH@Z
    ARTS_IMPORT void SetMultiRaceOptions(i32 arg1);
};

// ??_7Dialog_RaceEnvironment@@6B@
// vtable at 0x005B3FB8
// check_size(Dialog_RaceEnvironment, 0x0); // TODO: no layout in the IDB type library
