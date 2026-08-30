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

struct _cd_position_;

class audCDObject
{
public:
    // ?GetCDEndPosition@audCDObject@@QAEHPAU_cd_position_@@@Z
    ARTS_IMPORT i32 GetCDEndPosition(_cd_position_* arg1);

    // ?GetCDPosition@audCDObject@@QAEHPAU_cd_position_@@@Z
    ARTS_IMPORT i32 GetCDPosition(_cd_position_* arg1);

    // ?GetCDStartPosition@audCDObject@@QAEHPAU_cd_position_@@@Z
    ARTS_IMPORT i32 GetCDStartPosition(_cd_position_* arg1);

    // ?GetFramesBetween@audCDObject@@QAEHPAU_cd_position_@@0@Z
    ARTS_IMPORT i32 GetFramesBetween(_cd_position_* arg1, _cd_position_* arg2);

    // ?GetNumCDTracks@audCDObject@@QAEHXZ
    ARTS_IMPORT i32 GetNumCDTracks();

    // ?GetTrackInfo@audCDObject@@QAE_NH@Z
    ARTS_IMPORT bool GetTrackInfo(i32 arg1);

    // ?GetTrackTime@audCDObject@@QAEHXZ
    ARTS_IMPORT i32 GetTrackTime();

    // ?Init@audCDObject@@QAEXXZ
    ARTS_IMPORT void Init();

    // ?SetCDPosition@audCDObject@@QAE_NPAU_cd_position_@@@Z
    ARTS_IMPORT bool SetCDPosition(_cd_position_* arg1);
};

// check_size(audCDObject, 0x0); // TODO: no layout in the IDB type library
