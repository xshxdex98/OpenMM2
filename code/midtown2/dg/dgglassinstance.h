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

#include "dgunhitmtxbangerinstance.h"

class Vector3;
class gfxTexture;
class lvlInstance;
class ptxGlass;

class dgGlassInstance : public dgUnhitMtxBangerInstance
{
public:
    // ??0dgGlassInstance@@QAE@XZ
    ARTS_IMPORT dgGlassInstance();

    // ?Reset@dgGlassInstance@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Draw@dgGlassInstance@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 lod);

    // ?DrawGlow@dgGlassInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawGlow();

    // ?SizeOf@dgGlassInstance@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

    // ?Impact@dgGlassInstance@@UAEXPAVlvlInstance@@PAVVector3@@@Z
    ARTS_IMPORT virtual void Impact(lvlInstance* arg1, Vector3* arg2);

    // ?InitStaticSystems@dgGlassInstance@@SAXXZ
    ARTS_IMPORT static void InitStaticSystems();

public:
    static dgGlassInstance**& Access_sm_ActiveGlassInstance() { return sm_ActiveGlassInstance; }
    static ptxGlass*& Access_sm_GlassParticles() { return sm_GlassParticles; }
    static gfxTexture*& Access_sm_Texture() { return sm_Texture; }

private:
    // ?sm_ActiveGlassInstance@dgGlassInstance@@0PAPAV1@A
    ARTS_IMPORT static dgGlassInstance** sm_ActiveGlassInstance;

    // ?sm_GlassParticles@dgGlassInstance@@0PAVptxGlass@@A
    ARTS_IMPORT static ptxGlass* sm_GlassParticles;

    // ?sm_Texture@dgGlassInstance@@0PAVgfxTexture@@A
    ARTS_IMPORT static gfxTexture* sm_Texture;
};

// ??_7dgGlassInstance@@6B@
// vtable at 0x005B1628
// check_size(dgGlassInstance, 0x0); // TODO: no layout in the IDB type library
