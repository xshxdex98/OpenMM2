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

class gfxImage
{
public:
    enum gfxImageFormat : i32;

    enum gfxImageFormat : i32;

    // ?Create@gfxImage@@SAPAV1@HHW4gfxImageFormat@1@0H@Z
    ARTS_IMPORT static gfxImage* Create(i32 arg1, i32 arg2, gfxImage::gfxImageFormat arg3, gfxImage::gfxImageFormat arg4, i32 arg5);

    // ?Flip@gfxImage@@QAEXXZ
    ARTS_IMPORT void Flip();

    // ?GenerateMipmaps@gfxImage@@QAEXXZ
    ARTS_IMPORT void GenerateMipmaps();

    // ?GetDominantColor@gfxImage@@QBEIXZ
    ARTS_IMPORT u32 GetDominantColor() const;

    // ?GetFont@gfxImage@@SAPAV1@AAH0@Z
    ARTS_IMPORT static gfxImage* GetFont(i32& arg1, i32& arg2);

    // ?Halve@gfxImage@@QAEXXZ
    ARTS_IMPORT void Halve();

    // ?Reformat@gfxImage@@QAEXW4gfxImageFormat@1@0@Z
    ARTS_IMPORT void Reformat(gfxImage::gfxImageFormat newType, gfxImage::gfxImageFormat arg2);

    // ?Scale@gfxImage@@QAEXHH@Z
    ARTS_IMPORT void Scale(i32 width, i32 height);

    // ?sm_Use565@gfxImage@@2_NA
    ARTS_IMPORT static bool sm_Use565;

    // ?sm_Font@gfxImage@@2PAV1@A
    ARTS_IMPORT static gfxImage* sm_Font;

public:
    // The destructor is private in midtown2.exe; this reaches it without
    // changing its mangled name. It does not free the storage.
    void Access_Destroy() { this->~gfxImage(); }

private:
    // ??1gfxImage@@AAE@XZ
    ARTS_IMPORT ~gfxImage();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u16 Width; // 0x000
    u16 Height; // 0x002
    u16 BytesPerRow; // 0x004
    u8 Type; // 0x006
    u8 PaletteType; // 0x007
    u32 TexEnv; // 0x008
    void* pImageData; // 0x00C
    void* PaletteData; // 0x010
    u32 RefCount; // 0x014
    gfxImage* Next; // 0x018
};

check_size(gfxImage, 0x1C);
