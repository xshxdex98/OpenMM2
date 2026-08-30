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

struct GUID;
struct IDirectPlayLobby3;
class IZoneScore;
struct _GUID;

class netZoneScore
{
public:
    // ??0netZoneScore@@QAE@XZ
    ARTS_IMPORT netZoneScore();

    // ??1netZoneScore@@QAE@XZ
    ARTS_IMPORT ~netZoneScore();

    // ?Init@netZoneScore@@QAE_NU_GUID@@PAUIDirectPlayLobby3@@@Z
    ARTS_IMPORT bool Init(_GUID arg1, IDirectPlayLobby3* arg2);

    // ?InitResults@netZoneScore@@QAEXH_N@Z
    ARTS_IMPORT void InitResults(i32 arg1, bool arg2);

    // ?SendGameEnd@netZoneScore@@QAEXXZ
    ARTS_IMPORT void SendGameEnd();

    // ?SendGameSettings@netZoneScore@@QAEXPAD@Z
    ARTS_IMPORT void SendGameSettings(char* arg1);

    // ?SendGameStart@netZoneScore@@QAEXXZ
    ARTS_IMPORT void SendGameStart();

    // ?SendGameStartStaging@netZoneScore@@QAEXXZ
    ARTS_IMPORT void SendGameStartStaging();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    IDirectPlayLobby3* pLobby; // 0x000
    GUID GUID; // 0x004
    IZoneScore* pZoneScore; // 0x014
};

check_size(netZoneScore, 0x18);
