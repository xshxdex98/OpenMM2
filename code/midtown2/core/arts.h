/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2
    Copyright (C) 2026 The OpenMM2 contributors

    Adapted from Open1560 by Brick, an Open Source Re-Implementation of
    Midtown Madness 1. Copyright (C) 2020 Brick.

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

// A GUID held BY VALUE - netZoneScore has one at offset 0x004 - needs a definition, which a
// forward declaration cannot give. Spelled as Windows spells it and behind the same
// GUID_DEFINED guard, so a translation unit that also pulls in windows.h sees exactly one
// definition. The layout is fixed by the ABI: 4 + 2 + 2 + 8 = 16 bytes.
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
} GUID;
#endif

// Direct3D 7's colour value, held BY VALUE by gfxLight - which is D3DLIGHT7 field for
// field. Four floats, fixed by the D3D7 ABI. Guarded like GUID above so a translation
// unit that also pulls in d3dtypes.h sees one definition.
#ifndef D3DCOLORVALUE_DEFINED
#define D3DCOLORVALUE_DEFINED
typedef struct _D3DCOLORVALUE
{
    float r;
    float g;
    float b;
    float a;
} D3DCOLORVALUE;
#endif

// Direct3D 7's vector, held BY VALUE by gfxLight. The same three floats as Vector3, but
// deliberately NOT an alias of it: BUILTIN is derived from the type mapping's values, so
// aliasing would make Vector3 look like a builtin and stop every header that holds one by
// value from including vector3.h.
#ifndef D3DVECTOR_DEFINED
#define D3DVECTOR_DEFINED
typedef struct _D3DVECTOR
{
    float x;
    float y;
    float z;
} D3DVECTOR;
#endif

// Everything a generated class header needs: the type vocabulary, the ARTS_IMPORT/ARTS_EXPORT
// markers, and check_size.
//
// Open1560 gets these from a precompiled header instead. Here every generated header includes
// this one explicitly, so any header in the tree compiles on its own - which matters when 534 of
// them are machine-written and need to be checkable individually.

#include "defines.h"
#include "hooking.h"
#include "primitives.h"
