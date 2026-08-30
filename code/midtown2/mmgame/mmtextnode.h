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

#include "arts7/asnode.h"
#include "vector7/vector2.h"

#include "mmtext.h"

struct LocString;
class Vector4;
class gfxBitmap;
struct mmTextData;

class mmTextNode : public asNode
{
public:
    // ??0mmTextNode@@QAE@XZ
    ARTS_IMPORT mmTextNode();

    // ??1mmTextNode@@UAE@XZ
    ARTS_IMPORT virtual ~mmTextNode();

    // ?AddText@mmTextNode@@QAEHPBXPBULocString@@HMM@Z
    ARTS_IMPORT i32 AddText(const void* arg1, const LocString* arg2, i32 arg3, f32 arg4, f32 arg5);

    // ?Cull@mmTextNode@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?GetEffects@mmTextNode@@QAEHH@Z
    ARTS_IMPORT i32 GetEffects(i32 arg1);

    // ?GetFGColor@mmTextNode@@QAEIXZ
    ARTS_IMPORT u32 GetFGColor();

    // ?GetTextDimensions@mmTextNode@@QAEXPBXPBULocString@@AAM2@Z
    ARTS_IMPORT void GetTextDimensions(const void* arg1, const LocString* arg2, f32& arg3, f32& arg4);

    // ?Init@mmTextNode@@QAEXHHHHHH@Z
    ARTS_EXPORT void Init(i32 arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5, i32 arg6);

    // ?Init@mmTextNode@@QAEXMMMMHH@Z
    ARTS_IMPORT void Init(f32 arg1, f32 arg2, f32 arg3, f32 arg4, i32 arg5, i32 arg6);

    // ?Printf@mmTextNode@@QAAXPBDZZ
    ARTS_IMPORT void Printf(const char* arg1, ...);

    // ?RenderText@mmTextNode@@QAEXPAVgfxBitmap@@PAUmmTextData@@HI@Z
    ARTS_IMPORT void RenderText(gfxBitmap* arg1, mmTextData* arg2, i32 arg3, u32 arg4);

    // ?SetBGColor@mmTextNode@@QAEXAAVVector4@@@Z
    ARTS_IMPORT void SetBGColor(Vector4& arg1);

    // ?SetEffects@mmTextNode@@QAEXHH@Z
    ARTS_IMPORT void SetEffects(i32 arg1, i32 arg2);

    // ?SetFGColor@mmTextNode@@QAEXAAVVector4@@@Z
    ARTS_IMPORT void SetFGColor(Vector4& arg1);

    // ?SetHlColor@mmTextNode@@QAEXAAVVector4@@@Z
    ARTS_IMPORT void SetHlColor(Vector4& arg1);

    // ?SetPosition@mmTextNode@@QAEXHMM@Z
    ARTS_IMPORT void SetPosition(i32 arg1, f32 arg2, f32 arg3);

    // ?SetString@mmTextNode@@QAEXHPBULocString@@@Z
    ARTS_IMPORT void SetString(i32 arg1, const LocString* arg2);

    // ?SetTextPosition@mmTextNode@@QAEXHMM@Z
    ARTS_IMPORT void SetTextPosition(i32 arg1, f32 arg2, f32 arg3);

    // ?Update@mmTextNode@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Vector2 Pos; // 0x018
    u32 EntryCount; // 0x020
    u32 MaxEntries; // 0x024
    u32 DrawBits; // 0x028
    mmText field_2C; // 0x02C
    mmTextData* pTextEntries; // 0x030
    gfxBitmap* Bitmap; // 0x034
    i32 bModified; // 0x038
    u32 field_3C; // 0x03C
    u32 field_40; // 0x040
    u8 field_44; // 0x044
    u32 FGColor; // 0x048
    u32 BGColor; // 0x04C
    u32 HiglightColor; // 0x050
};

// ??_7mmTextNode@@6B@
// vtable at 0x005B52F0
check_size(mmTextNode, 0x54);
