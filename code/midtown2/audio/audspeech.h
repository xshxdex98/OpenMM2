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

class AudSpeech
{
public:
    // ??0AudSpeech@@QAE@_N@Z
    ARTS_IMPORT AudSpeech(bool renderer);

    // ??1AudSpeech@@UAE@XZ
    ARTS_IMPORT virtual ~AudSpeech();

    // ?AllocateQueuePlayData@AudSpeech@@QAEXXZ
    ARTS_IMPORT void AllocateQueuePlayData();

    // ?AllocateSpeechData@AudSpeech@@QAEXPADMM@Z
    ARTS_IMPORT void AllocateSpeechData(char* arg1, f32 arg2, f32 arg3);

    // ?GetCatAddValue@AudSpeech@@QAEHH@Z
    ARTS_IMPORT i32 GetCatAddValue(i32 arg1);

    // ?GetEndSufixNumber@AudSpeech@@QAEHH@Z
    ARTS_IMPORT i32 GetEndSufixNumber(i32 arg1);

    // ?IsPlaying@AudSpeech@@QAE_NXZ
    ARTS_IMPORT bool IsPlaying();

    // ?Play@AudSpeech@@QAEXHHM@Z
    ARTS_IMPORT void Play(i32 arg1, i32 arg2, f32 arg3);

    // ?Play@AudSpeech@@QAEXHM@Z
    ARTS_IMPORT void Play(i32 arg1, f32 arg2);

    // ?SetExtension@AudSpeech@@QAEXPAD@Z
    ARTS_IMPORT void SetExtension(char* renderer);

    // ?SetSubPath@AudSpeech@@QAEXPAD@Z
    ARTS_IMPORT void SetSubPath(char* arg1);

    // ?SetVolume@AudSpeech@@QAEXM@Z
    ARTS_IMPORT void SetVolume(f32 arg1);

    // ?Stop@AudSpeech@@QAEXXZ
    ARTS_IMPORT void Stop();

    // ?Update@AudSpeech@@QAEXXZ
    ARTS_IMPORT void Update();

private:
    // ?EmptyQueue@AudSpeech@@AAEXXZ
    ARTS_IMPORT void EmptyQueue();

    // ?PlayOneShot@AudSpeech@@AAEXHHM@Z
    ARTS_IMPORT void PlayOneShot(i32 arg1, i32 arg2, f32 arg3);

    // ?PlayOneShot@AudSpeech@@AAEXHM@Z
    ARTS_IMPORT void PlayOneShot(i32 arg1, f32 arg2);

    // ?PlayStream@AudSpeech@@AAEXHHM@Z
    ARTS_IMPORT void PlayStream(i32 renderer, i32 DrawList, f32 cull_rect);

    // ?PlayStream@AudSpeech@@AAEXHM@Z
    ARTS_IMPORT void PlayStream(i32 renderer, f32 DrawList);

    // ?PutInQueue@AudSpeech@@AAEXHHM@Z
    ARTS_IMPORT void PutInQueue(i32 arg1, i32 arg2, f32 arg3);

    // ?SetOneShotVolume@AudSpeech@@AAEXM@Z
    ARTS_IMPORT void SetOneShotVolume(f32 arg1);

    // ?SetStreamVolume@AudSpeech@@AAEXM@Z
    ARTS_IMPORT void SetStreamVolume(f32 arg1);
};

// check_size(AudSpeech, 0x0); // TODO: no layout in the IDB type library
