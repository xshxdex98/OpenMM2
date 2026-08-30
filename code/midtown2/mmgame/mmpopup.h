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

class mmGame;

class mmPopup : public asNode
{
public:
    // ??0mmPopup@@QAE@PAVmmGame@@MMMM@Z
    ARTS_IMPORT mmPopup(mmGame* window, f32 arg2, f32 arg3, f32 arg4, f32 arg5);

    // ??1mmPopup@@UAE@XZ
    ARTS_IMPORT virtual ~mmPopup();

    // ?Update@mmPopup@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmPopup@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?ChatCB@mmPopup@@QAEXXZ
    ARTS_IMPORT void ChatCB();

    // ?DisablePU@mmPopup@@QAEXH@Z
    ARTS_IMPORT void DisablePU(i32 arg1);

    // ?ForceRoster@mmPopup@@QAEXXZ
    ARTS_IMPORT void ForceRoster();

    // ?GetComment@mmPopup@@QAEPADXZ
    ARTS_IMPORT char* GetComment();

    // ?IsEnabled@mmPopup@@QAEHXZ
    ARTS_IMPORT i32 IsEnabled();

    // ?Lock@mmPopup@@QAEXXZ
    ARTS_IMPORT void Lock();

    // ?PlayPauseMusic@mmPopup@@QAEXXZ
    ARTS_IMPORT void PlayPauseMusic();

    // ?PlayReturnMusic@mmPopup@@QAEXXZ
    ARTS_IMPORT void PlayReturnMusic();

    // ?ProcessChat@mmPopup@@QAEXXZ
    ARTS_IMPORT void ProcessChat();

    // ?ProcessEscape@mmPopup@@QAEXH@Z
    ARTS_IMPORT void ProcessEscape(i32 arg1);

    // ?ProcessKeymap@mmPopup@@QAEXH@Z
    ARTS_IMPORT void ProcessKeymap(i32 window);

    // ?SaveReplay@mmPopup@@QAEXXZ
    ARTS_IMPORT void SaveReplay();

    // ?SetComment@mmPopup@@QAEXPAD@Z
    ARTS_IMPORT void SetComment(char* arg1);

    // ?ShowReplay@mmPopup@@QAEXXZ
    ARTS_IMPORT void ShowReplay();

    // ?ShowResults@mmPopup@@QAEXH@Z
    ARTS_IMPORT void ShowResults(i32 arg1);

    // ?ShowRoster@mmPopup@@QAEXXZ
    ARTS_IMPORT void ShowRoster();

    // ?Unlock@mmPopup@@QAEXXZ
    ARTS_IMPORT void Unlock();
};

// ??_7mmPopup@@6B@
// vtable at 0x005B0B44
// check_size(mmPopup, 0x60); // size known, members are not - cannot verify
