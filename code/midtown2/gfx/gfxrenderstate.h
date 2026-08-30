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

#include "vector7/matrix44.h"

#include "gfxrenderstatedata.h"

enum gfxBlendSet : i32;

class Matrix34;
class Vector3;
class Vector4;
struct gfxLight;

class gfxRenderState
{
public:
    // ?Default@gfxRenderState@@QAEXXZ
    ARTS_IMPORT void Default();

    // ?DisableAllLights@gfxRenderState@@QAEXXZ
    ARTS_IMPORT void DisableAllLights();

    // ?DoFlush@gfxRenderState@@QAEXPAVgfxRenderStateData@@@Z
    ARTS_IMPORT void DoFlush(gfxRenderStateData* arg1);

    // ?GetLight@gfxRenderState@@QAEXHPAUgfxLight@@@Z
    ARTS_IMPORT void GetLight(i32 arg1, gfxLight* arg2);

    // ?LerpRGBA@gfxRenderState@@QAEIIIM@Z
    ARTS_IMPORT u32 LerpRGBA(u32 arg1, u32 arg2, f32 arg3);

    // ?LightEnable@gfxRenderState@@QAEXH_N@Z
    ARTS_IMPORT void LightEnable(i32 arg1, bool arg2);

    // ?SetBlendMatrix@gfxRenderState@@QAEXHABVMatrix44@@@Z
    ARTS_IMPORT void SetBlendMatrix(i32 arg1, const Matrix44& arg2);

    // ?SetBlendSet@gfxRenderState@@QAEXW4gfxBlendSet@@E@Z
    ARTS_IMPORT void SetBlendSet(gfxBlendSet arg1, u8 arg2);

    // ?SetCamera@gfxRenderState@@SAXABVMatrix34@@@Z
    ARTS_IMPORT static void SetCamera(const Matrix34& arg1);

    // ?SetCamera@gfxRenderState@@SAXABVMatrix44@@@Z
    ARTS_EXPORT static void SetCamera(const Matrix44& arg1);

    // ?SetCameraFull@gfxRenderState@@SAXABVMatrix34@@@Z
    ARTS_IMPORT static void SetCameraFull(const Matrix34& arg1);

    // ?SetCard@gfxRenderState@@SAXABVVector3@@@Z
    ARTS_IMPORT static void SetCard(const Vector3& arg1);

    // ?SetCard@gfxRenderState@@SAXABVVector4@@@Z
    ARTS_IMPORT static void SetCard(const Vector4& arg1);

    // ?SetLight@gfxRenderState@@QAEXHPBUgfxLight@@@Z
    ARTS_IMPORT void SetLight(i32 arg1, const gfxLight* arg2);

    // ?SetTexMatrix@gfxRenderState@@QAEXHABVMatrix44@@@Z
    ARTS_IMPORT void SetTexMatrix(i32 arg1, const Matrix44& arg2);

    // ?SetTexMipmapBias@gfxRenderState@@QAEXHM@Z
    ARTS_IMPORT void SetTexMipmapBias(i32 arg1, f32 arg2);

    // ?SetTexSource@gfxRenderState@@QAEXHH@Z
    ARTS_IMPORT void SetTexSource(i32 arg1, i32 arg2);

    // ?SetTexTransform@gfxRenderState@@QAEXHH_N@Z
    ARTS_IMPORT void SetTexTransform(i32 arg1, i32 arg2, bool arg3);

public:
    static i32& Access_sm_MaxActiveLights() { return sm_MaxActiveLights; }
    static i32& Access_sm_MaxLights() { return sm_MaxLights; }
    static Matrix44& Access_sm_Camera() { return sm_Camera; }
    static bool& Access_sm_SupportsBlendWithOne() { return sm_SupportsBlendWithOne; }
    static Matrix44& Access_sm_Composite() { return sm_Composite; }
    static Matrix44& Access_sm_World() { return sm_World; }
    static Matrix44& Access_sm_FullComposite() { return sm_FullComposite; }
    static Matrix44& Access_sm_View() { return sm_View; }
    static Matrix44& Access_sm_Modelview() { return sm_Modelview; }
    static i32& Access_m_Touched() { return m_Touched; }
    static i32& Access_sm_MaxBlendMatrices() { return sm_MaxBlendMatrices; }
    static i32& Access_sm_MaxTextures() { return sm_MaxTextures; }

private:
    // ?CheckSet@gfxRenderState@@AAEXAAEE@Z
    ARTS_IMPORT void CheckSet(u8& arg1, u8 arg2);

    // ?Init@gfxRenderState@@AAEXXZ
    ARTS_IMPORT void Init();

    // ?Regenerate@gfxRenderState@@CAXXZ
    ARTS_IMPORT static void Regenerate();

    // ?sm_MaxActiveLights@gfxRenderState@@0HA
    ARTS_IMPORT static i32 sm_MaxActiveLights;

    // ?sm_MaxLights@gfxRenderState@@0HA
    ARTS_IMPORT static i32 sm_MaxLights;

    // ?sm_Camera@gfxRenderState@@0VMatrix44@@A
    ARTS_IMPORT static Matrix44 sm_Camera;

    // ?sm_SupportsBlendWithOne@gfxRenderState@@0_NA
    ARTS_IMPORT static bool sm_SupportsBlendWithOne;

    // ?sm_Composite@gfxRenderState@@0VMatrix44@@A
    ARTS_IMPORT static Matrix44 sm_Composite;

    // ?sm_World@gfxRenderState@@0VMatrix44@@A
    ARTS_IMPORT static Matrix44 sm_World;

    // ?sm_FullComposite@gfxRenderState@@0VMatrix44@@A
    ARTS_IMPORT static Matrix44 sm_FullComposite;

    // ?sm_View@gfxRenderState@@0VMatrix44@@A
    ARTS_IMPORT static Matrix44 sm_View;

    // ?sm_Modelview@gfxRenderState@@0VMatrix44@@A
    ARTS_IMPORT static Matrix44 sm_Modelview;

    // ?m_Touched@gfxRenderState@@0HA
    ARTS_IMPORT static i32 m_Touched;

    // ?sm_MaxBlendMatrices@gfxRenderState@@0HA
    ARTS_IMPORT static i32 sm_MaxBlendMatrices;

    // ?sm_MaxTextures@gfxRenderState@@0HA
    ARTS_IMPORT static i32 sm_MaxTextures;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    gfxRenderStateData State; // 0x000
};

check_size(gfxRenderState, 0x50);
