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

#include "audio/audcreaturecontainer.h"

class Vector3;

class aiPedAudio : public AudCreatureContainer
{
public:
    // ??0aiPedAudio@@QAE@XZ
    ARTS_IMPORT aiPedAudio();

    // ??1aiPedAudio@@UAE@XZ
    ARTS_IMPORT virtual ~aiPedAudio();

    // ?LoadFemaleVoices@aiPedAudio@@QAE_NPAD_N@Z
    ARTS_IMPORT bool LoadFemaleVoices(char* arg1, bool arg2);

    // ?LoadMaleVoices@aiPedAudio@@QAE_NPAD_N@Z
    ARTS_IMPORT bool LoadMaleVoices(char* arg1, bool idx);

    // ?LoadNumFemaleChoices@aiPedAudio@@SAXPAD@Z
    ARTS_IMPORT static void LoadNumFemaleChoices(char* arg1);

    // ?LoadNumMaleChoices@aiPedAudio@@SAXPAD@Z
    ARTS_IMPORT static void LoadNumMaleChoices(char* arg1);

    // ?SetCSVCatString@aiPedAudio@@SAXPAD@Z
    ARTS_IMPORT static void SetCSVCatString(char* arg1);

public:
    static i32& Access_s_iFemaleFileNum() { return s_iFemaleFileNum; }
    static char*& Access_s_CSVCatString() { return s_CSVCatString; }
    static i32& Access_s_iMaleFileNum() { return s_iMaleFileNum; }

private:
    // ?s_iFemaleFileNum@aiPedAudio@@0HA
    ARTS_IMPORT static i32 s_iFemaleFileNum;

    // ?s_CSVCatString@aiPedAudio@@0PADA
    ARTS_IMPORT static char* s_CSVCatString;

    // ?s_iMaleFileNum@aiPedAudio@@0HA
    ARTS_IMPORT static i32 s_iMaleFileNum;
};

// ??_7aiPedAudio@@6B@
// vtable at 0x005B5568
// check_size(aiPedAudio, 0x60); // size known, members are not - cannot verify
