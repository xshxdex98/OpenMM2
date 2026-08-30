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

class gfxLight
{
public:
    // ?Reset@gfxLight@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Sun@gfxLight@@2U1@A
    ARTS_IMPORT static gfxLight Sun;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 dltType; // 0x000
    D3DCOLORVALUE dcvDiffuse; // 0x004
    D3DCOLORVALUE dcvSpecular; // 0x014
    D3DCOLORVALUE dcvAmbient; // 0x024
    D3DVECTOR dvPosition; // 0x034
    D3DVECTOR dvDirection; // 0x040
    f32 dvRange; // 0x04C
    f32 dvFalloff; // 0x050
    f32 dvAttenuation0; // 0x054
    f32 dvAttenuation1; // 0x058
    f32 dvAttenuation2; // 0x05C
    f32 dvTheta; // 0x060
    f32 dvPhi; // 0x064
};

check_size(gfxLight, 0x68);
