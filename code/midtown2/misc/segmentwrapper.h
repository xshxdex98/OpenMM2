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

struct IDirectMusicLoader;
struct IDirectMusicPerformance;

class SegmentWrapper
{
public:
    // ??0SegmentWrapper@@QAE@XZ
    ARTS_IMPORT SegmentWrapper();

    // ??1SegmentWrapper@@QAE@XZ
    ARTS_IMPORT ~SegmentWrapper();

    // ?CleanUp@SegmentWrapper@@QAEXXZ
    ARTS_IMPORT void CleanUp();

    // ?IsPlaying@SegmentWrapper@@QAE_NXZ
    ARTS_IMPORT bool IsPlaying();

    // ?LoadBand@SegmentWrapper@@QAE_NPAD@Z
    ARTS_IMPORT bool LoadBand(char* arg1);

    // ?LoadSegmentBands@SegmentWrapper@@QAE_NXZ
    ARTS_IMPORT bool LoadSegmentBands();

    // ?OpenSegmentFile@SegmentWrapper@@QAE_NPAD_N@Z
    ARTS_IMPORT bool OpenSegmentFile(char* arg1, bool arg2);

    // ?Play@SegmentWrapper@@QAE_NKK@Z
    ARTS_IMPORT bool Play(ulong arg1, ulong arg2);

    // ?PlayBand@SegmentWrapper@@QAE_NXZ
    ARTS_IMPORT bool PlayBand();

    // ?SetGrooveLevel@SegmentWrapper@@QAE_NEE@Z
    ARTS_IMPORT bool SetGrooveLevel(u8 arg1, u8 arg2);

    // ?SetLoaderPtr@SegmentWrapper@@SAXPAUIDirectMusicLoader@@@Z
    ARTS_IMPORT static void SetLoaderPtr(IDirectMusicLoader* arg1);

    // ?SetPerformancePtr@SegmentWrapper@@SAXPAUIDirectMusicPerformance@@@Z
    ARTS_IMPORT static void SetPerformancePtr(IDirectMusicPerformance* arg1);

    // ?Stop@SegmentWrapper@@QAE_NXZ
    ARTS_IMPORT bool Stop();

    // ?UnloadSegmentBands@SegmentWrapper@@QAE_NXZ
    ARTS_IMPORT bool UnloadSegmentBands();
};

// check_size(SegmentWrapper, 0x0); // TODO: no layout in the IDB type library
