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

#include "dg/dgstatepack.h"
#include "ui/netstartarray.h"

class mmStatePack : public dgStatePack
{
public:
    // ??0mmStatePack@@QAE@XZ
    ARTS_IMPORT mmStatePack();

    // ??1mmStatePack@@QAE@XZ
    ARTS_IMPORT ~mmStatePack();

    // ?ParseStateArgs@mmStatePack@@QAE_NXZ
    ARTS_IMPORT bool ParseStateArgs();

    // ?SetDefaults@mmStatePack@@QAEXPAD0@Z
    ARTS_IMPORT void SetDefaults(char* level, char* car);

public:
    // Members from 0x06C; everything below that belongs to dgStatePack.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8 CityName[40]; // 0x06C
    i8 CityLocale[40]; // 0x094
    i8 ReplayName[40]; // 0x0BC
    i32 VehicleId; // 0x0E4
    i32 InputDevice; // 0x0E8
    i32 field_EC; // 0x0EC
    i32 CopsChaseAI; // 0x0F0
    i32 field_F4; // 0x0F4
    i32 Shutdown; // 0x0F8
    f32 Difficulty; // 0x0FC
    i32 TransmissionType; // 0x100
    f32 PhysicsRealism; // 0x104
    i32 UseForceFeedback; // 0x108
    i32 DisableProfile; // 0x10C
    i8 VehicleName[80]; // 0x110
    i32 VehiclePaintjob; // 0x160
    i8 NetName[80]; // 0x164
    f32 TimeLimit; // 0x1B4
    i32 GameState; // 0x1B8
    i32 DisableRegen; // 0x1BC
    f32 SoundFXVolume; // 0x1C0
    f32 AudioBalance; // 0x1C4
    f32 MusicVolume; // 0x1C8
    i16 NumAudioChannels; // 0x1CC
    i16 field_1CE[10]; // 0x1CE
    i8 AudioDeviceName[200]; // 0x1E2
    u8 HasMusicCD; // 0x2AA
    i32 CnRMode; // 0x2AC
    i32 CnRLimitType; // 0x2B0
    i32 CnRTeam; // 0x2B4
    f32 CnRTimeLimit; // 0x2B8
    i32 CnRPointLimit; // 0x2BC
    i32 CnRGoldMass; // 0x2C0
    i32 field_2C4; // 0x2C4
    i32 field_2C8; // 0x2C8
    i8 field_2CC[160]; // 0x2CC
    u8 field_36C; // 0x36C
    i8 MapMode; // 0x36D
    u8 UseWideFOV; // 0x36E
    u8 ShowDash; // 0x36F
    u8 ShowMirror; // 0x370
    u8 field_371; // 0x371
    u8 field_372; // 0x372
    u8 ShowIcons; // 0x373
    u8 field_374; // 0x374
    u8 field_375; // 0x375
    u8 HudOrient; // 0x376
    u8 HudZoomIn; // 0x377
    i32 BestTextureFilter; // 0x378
    i32 InterlacedRendering; // 0x37C
    f32 FarClip; // 0x380
    i32 EnableSky; // 0x384
    f32 LightingQuality; // 0x388
    i32 UsePortals; // 0x38C
    i32 ObjectDetail; // 0x390
    i32 EnableReflections; // 0x394
    i32 CloudShadowQuality; // 0x398
    i32 IsCheating; // 0x39C
    f32 TimeLimitOverride; // 0x3A0
    i32 UnlockRewards; // 0x3A4
    u8 AllowCheaters; // 0x3A8
    u8 ShowMultiplayerResults; // 0x3A9
    u8 IsHost; // 0x3AA
    u8 UseDialup; // 0x3AB
    i32 UseIME; // 0x3AC
    HIMC IMEContext; // 0x3B0
    u8 InCrashCourse; // 0x3B4
    NetStartArray NetStartArray; // 0x3B8
};

check_size(mmStatePack, 0x3E0);
