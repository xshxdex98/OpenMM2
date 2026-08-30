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

class DMusicWaveBuffer;
struct IDirectSound;
struct _DMUS_PORTCAPS;

class DMusicObject
{
public:
    // ??0DMusicObject@@QAE@H@Z
    ARTS_IMPORT DMusicObject(i32 arg1);

    // ??1DMusicObject@@QAE@XZ
    ARTS_IMPORT ~DMusicObject();

    // ?Activate@DMusicObject@@QAEHH@Z
    ARTS_IMPORT i32 Activate(i32 arg1);

    // ?AllocateMotifs@DMusicObject@@QAEXH@Z
    ARTS_IMPORT void AllocateMotifs(i32 arg1);

    // ?CleanUpSegmentWrappers@DMusicObject@@QAEXXZ
    ARTS_IMPORT void CleanUpSegmentWrappers();

    // ?CreateComposer@DMusicObject@@QAEHXZ
    ARTS_IMPORT i32 CreateComposer();

    // ?FindBestPort@DMusicObject@@QAEHAAU_DMUS_PORTCAPS@@@Z
    ARTS_IMPORT i32 FindBestPort(_DMUS_PORTCAPS& arg1);

    // ?FindMSSoftWareSynth@DMusicObject@@QAEHAAU_DMUS_PORTCAPS@@@Z
    ARTS_IMPORT i32 FindMSSoftWareSynth(_DMUS_PORTCAPS& arg1);

    // ?GetDMusicWaveBuffer@DMusicObject@@QAEPAVDMusicWaveBuffer@@XZ
    ARTS_IMPORT DMusicWaveBuffer* GetDMusicWaveBuffer();

    // ?GetNumPorts@DMusicObject@@QAEHXZ
    ARTS_IMPORT i32 GetNumPorts();

    // ?HandleNotifications@DMusicObject@@QAEXXZ
    ARTS_IMPORT void HandleNotifications();

    // ?Init@DMusicObject@@QAEHPAUIDirectSound@@KHKK@Z
    ARTS_IMPORT i32 Init(IDirectSound* arg1, ulong arg2, i32 arg3, ulong arg4, ulong arg5);

    // ?IsPlaying@DMusicObject@@QAEHXZ
    ARTS_IMPORT i32 IsPlaying();

    // ?LoadBand@DMusicObject@@QAEHPADHH@Z
    ARTS_IMPORT i32 LoadBand(char* arg1, i32 arg2, i32 arg3);

    // ?LoadMotif@DMusicObject@@QAEHPAD0H@Z
    ARTS_IMPORT i32 LoadMotif(char* arg1, char* arg2, i32 arg3);

    // ?OpenSegmentFile@DMusicObject@@QAEHPADH_N@Z
    ARTS_IMPORT i32 OpenSegmentFile(char* renderer, i32 DrawList, bool cull_rect);

    // ?PlayBand@DMusicObject@@QAEHHH@Z
    ARTS_IMPORT i32 PlayBand(i32 arg1, i32 arg2);

    // ?PlayMotif@DMusicObject@@QAEHHK@Z
    ARTS_IMPORT i32 PlayMotif(i32 arg1, ulong arg2);

    // ?PlaySegment@DMusicObject@@QAEHK@Z
    ARTS_IMPORT i32 PlaySegment(ulong arg1);

    // ?ScaleMasterTempo@DMusicObject@@QAEHM@Z
    ARTS_IMPORT i32 ScaleMasterTempo(f32 arg1);

    // ?ScaleMasterVolume@DMusicObject@@QAEHJ@Z
    ARTS_IMPORT i32 ScaleMasterVolume(ilong arg1);

    // ?ScanDirectory@DMusicObject@@QAEHXZ
    ARTS_IMPORT i32 ScanDirectory();

    // ?SegmentSwitch@DMusicObject@@QAEHH@Z
    ARTS_IMPORT i32 SegmentSwitch(i32 arg1);

    // ?SegmentSwitch@DMusicObject@@QAEHHGK@Z
    ARTS_IMPORT i32 SegmentSwitch(i32 arg1, u16 arg2, ulong arg3);

    // ?SetSearchDirectory@DMusicObject@@QAEHPAD@Z
    ARTS_IMPORT i32 SetSearchDirectory(char* lpMultiByteStr);

    // ?StopSegment@DMusicObject@@QAEHH@Z
    ARTS_IMPORT i32 StopSegment(i32 arg1);

private:
    // ?AssignPChannelBlocks@DMusicObject@@AAEHK@Z
    ARTS_IMPORT i32 AssignPChannelBlocks(ulong arg1);

    // ?InitLoader@DMusicObject@@AAEHXZ
    ARTS_IMPORT i32 InitLoader();

    // ?InitNotificationThread@DMusicObject@@AAEHXZ
    ARTS_IMPORT i32 InitNotificationThread();

    // ?InitPerformance@DMusicObject@@AAEHPAUIDirectSound@@@Z
    ARTS_IMPORT i32 InitPerformance(IDirectSound* arg1);

    // ?InitPort@DMusicObject@@AAEHPAUIDirectSound@@KHKK@Z
    ARTS_IMPORT i32 InitPort(IDirectSound* arg1, ulong arg2, i32 arg3, ulong arg4, ulong arg5);

    // ?RatePort@DMusicObject@@AAEHAAU_DMUS_PORTCAPS@@@Z
    ARTS_IMPORT i32 RatePort(_DMUS_PORTCAPS& arg1);

    // ?SetGrooveLevel@DMusicObject@@AAEHEE@Z
    ARTS_IMPORT i32 SetGrooveLevel(u8 arg1, u8 arg2);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    unsigned __int8[56] _buffer; // 0x000
};

check_size(DMusicObject, 0x38);
