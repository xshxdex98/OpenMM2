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

#include "vector7/vector3.h"

class Matrix34;
class datParser;
struct gfxLight;
class gfxTexture;
class ltProjection;

class ltLight
{
public:
    // ??0ltLight@@QAE@XZ
    ARTS_IMPORT ltLight();

    // ??1ltLight@@QAE@XZ
    ARTS_IMPORT ~ltLight();

    // ?ComputeDistance@ltLight@@QAEMABVVector3@@@Z
    ARTS_IMPORT f32 ComputeDistance(const Vector3& arg1);

    // ?ComputeIntensity@ltLight@@QAEMABVVector3@@M@Z
    ARTS_IMPORT f32 ComputeIntensity(const Vector3& This, f32 arg2);

    // ?Default@ltLight@@QAEXXZ
    ARTS_IMPORT void Default();

    // ?Draw@ltLight@@QAEXM@Z
    ARTS_IMPORT void Draw(f32 arg1);

    // ?DrawGlow@ltLight@@QAEXABVVector3@@@Z
    ARTS_IMPORT void DrawGlow(const Vector3& arg1);

    // ?DrawGlowBegin@ltLight@@SAXXZ
    ARTS_IMPORT static void DrawGlowBegin();

    // ?DrawGlowEnd@ltLight@@SAXXZ
    ARTS_IMPORT static void DrawGlowEnd();

    // ?DrawHighlight@ltLight@@QAEXABVVector3@@ABVMatrix34@@@Z
    ARTS_IMPORT void DrawHighlight(const Vector3& arg1, const Matrix34& arg2);

    // ?FileIO@ltLight@@QAEXAAVdatParser@@@Z
    ARTS_IMPORT void FileIO(datParser& arg1);

    // ?GetClosestLight@ltLight@@SAPAV1@XZ
    ARTS_IMPORT static ltLight* GetClosestLight();

    // ?GetNumPointLights@ltLight@@SAHXZ
    ARTS_IMPORT static i32 GetNumPointLights();

    // ?GetPointLight@ltLight@@SAPAV1@H@Z
    ARTS_IMPORT static ltLight* GetPointLight(i32 arg1);

    // ?Illuminate@ltLight@@QAEXAAVVector3@@00@Z
    ARTS_IMPORT void Illuminate(Vector3& arg1, Vector3& arg2, Vector3& arg3);

    // ?Random@ltLight@@QAEXXZ
    ARTS_IMPORT void Random();

    // ?SetGfxLight@ltLight@@QAE_NAAUgfxLight@@ABVVector3@@@Z
    ARTS_IMPORT bool SetGfxLight(gfxLight& This, const Vector3& arg2);

    // ?SetUpGfxLight@ltLight@@QAEXXZ
    ARTS_IMPORT void SetUpGfxLight();

    // ?SetUpGfxLightBegin@ltLight@@SAXABVVector3@@@Z
    ARTS_IMPORT static void SetUpGfxLightBegin(const Vector3& arg1);

    // ?SetUpGfxLightEnd@ltLight@@SAXXZ
    ARTS_IMPORT static void SetUpGfxLightEnd();

    // ?SetUpProjection@ltLight@@QAEXAAVltProjection@@@Z
    ARTS_IMPORT void SetUpProjection(ltProjection& arg1);

    // ?ShutdownLights@ltLight@@SAXXZ
    ARTS_IMPORT static void ShutdownLights();

public:
    static f32& Access_GlowScale() { return GlowScale; }
    static f32& Access_GlowIntensity() { return GlowIntensity; }
    static f32& Access_AmbientScale() { return AmbientScale; }
    static f32& Access_DiffuseMinIntensity() { return DiffuseMinIntensity; }
    static f32& Access_DiffuseMaxIntensity() { return DiffuseMaxIntensity; }
    static i32& Access_NumGfxLights() { return NumGfxLights; }

private:
    // ?GlowScale@ltLight@@0MA
    ARTS_IMPORT static f32 GlowScale;

    // ?GlowIntensity@ltLight@@0MA
    ARTS_IMPORT static f32 GlowIntensity;

    // ?AmbientScale@ltLight@@0MA
    ARTS_IMPORT static f32 AmbientScale;

    // ?DiffuseMinIntensity@ltLight@@0MA
    ARTS_IMPORT static f32 DiffuseMinIntensity;

    // ?DiffuseMaxIntensity@ltLight@@0MA
    ARTS_IMPORT static f32 DiffuseMaxIntensity;

    // ?NumGfxLights@ltLight@@0HA
    ARTS_IMPORT static i32 NumGfxLights;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 Type; // 0x000
    Vector3 Position; // 0x004
    Vector3 Direction; // 0x010
    Vector3 Color; // 0x01C
    f32 Intensity; // 0x028
    u32 field_2C; // 0x02C
    f32 SpotExponent; // 0x030
    gfxTexture* HightlightTexture; // 0x034
    u32 field_38; // 0x038
    f32 ProjectionSize; // 0x03C
    u8 EnableProjection; // 0x040
    u8 field_41[3]; // 0x041
    u32 ShadowMode; // 0x044
    u8 Enabled; // 0x048
};

check_size(ltLight, 0x4C);
