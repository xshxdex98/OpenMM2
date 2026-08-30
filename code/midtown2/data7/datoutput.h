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

class datOutput
{
public:
    // ?CallAfterMsgBoxFunction@datOutput@@SAXXZ
    ARTS_IMPORT static void CallAfterMsgBoxFunction();

    // ?CallBeforeMsgBoxFunction@datOutput@@SAXXZ
    ARTS_IMPORT static void CallBeforeMsgBoxFunction();

    // ?CloseLog@datOutput@@SAXXZ
    ARTS_IMPORT static void CloseLog();

    // ?OpenLog@datOutput@@SA_NPBD@Z
    ARTS_IMPORT static bool OpenLog(const char* fileName);

    // ?SetAfterMsgBoxFunction@datOutput@@SAXP6AXXZ@Z
    ARTS_IMPORT static void SetAfterMsgBoxFunction(void (__cdecl* arg1)(void));

    // ?SetBeforeMsgBoxFunction@datOutput@@SAXP6AXXZ@Z
    ARTS_IMPORT static void SetBeforeMsgBoxFunction(void (__cdecl* arg1)(void));

    // ?SetOutputMask@datOutput@@SAXI@Z
    ARTS_IMPORT static void SetOutputMask(u32 arg1);

public:
    static bool& Access_OutputSent() { return OutputSent; }

private:
    // ?OutputSent@datOutput@@0_NA
    ARTS_IMPORT static bool OutputSent;
};

// check_size(datOutput, 0x1); // size known, members are not - cannot verify
