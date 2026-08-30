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

class FixedHashEntry;
class HashEntry;
class HashPosition;

class HashTable
{
public:
    // ??0HashTable@@QAE@H_NH@Z
    ARTS_IMPORT HashTable(i32 arg1, bool arg2, i32 arg3);

    // ??1HashTable@@QAE@XZ
    ARTS_IMPORT ~HashTable();

    // ?Access@HashTable@@QAEPAXPBD@Z
    ARTS_IMPORT void* Access(const char* arg1);

    // ?Access@HashTable@@QAE_NPBDPAPBX@Z
    ARTS_IMPORT bool Access(const char* arg1, const void** arg2);

    // ?AccessData@HashTable@@QAEPAXH@Z
    ARTS_IMPORT void* AccessData(i32 arg1);

    // ?AccessName@HashTable@@QAEPBDH@Z
    ARTS_IMPORT const char* AccessName(i32 arg1);

    // ?Change@HashTable@@QAE_NPBD0@Z
    ARTS_IMPORT bool Change(const char* arg1, const char* arg2);

    // ?Delete@HashTable@@QAE_NPBD@Z
    ARTS_IMPORT bool Delete(const char* arg1);

    // ?GetEmptySlot@HashTable@@QAEAAVHashEntry@@PBDPAXPAV2@@Z
    ARTS_IMPORT HashEntry& GetEmptySlot(const char* arg1, void* arg2, HashEntry* arg3);

    // ?GetFirstEntry@HashTable@@QAE_NAAVHashPosition@@@Z
    ARTS_IMPORT bool GetFirstEntry(HashPosition& arg1);

    // ?GetNextEntry@HashTable@@QAE_NAAVHashPosition@@@Z
    ARTS_IMPORT bool GetNextEntry(HashPosition& arg1);

    // ?Insert@HashTable@@QAE_NPBDPAX@Z
    ARTS_IMPORT bool Insert(const char* arg1, void* arg2);

    // ?Kill@HashTable@@QAEXXZ
    ARTS_IMPORT void Kill();

    // ?KillAll@HashTable@@SAXXZ
    ARTS_IMPORT static void KillAll();

    // ?MakeEmpty@HashTable@@QAEXXZ
    ARTS_IMPORT void MakeEmpty();

    // ?MakePermanent@HashTable@@QAEXXZ
    ARTS_IMPORT void MakePermanent();

    // ??4HashTable@@QAEXABV0@@Z
    ARTS_IMPORT void operator=(const HashTable& arg1);

public:
    static HashTable*& Access_First() { return First; }

private:
    // ?ComputePrime@HashTable@@AAEHH@Z
    ARTS_IMPORT i32 ComputePrime(i32 arg1);

    // ?GetEntry@HashTable@@AAEPAVHashEntry@@H@Z
    ARTS_IMPORT HashEntry* GetEntry(i32 arg1);

    // ?Hash@HashTable@@AAEHPBD@Z
    ARTS_IMPORT i32 Hash(const char* arg1);

    // ?InitCommon@HashTable@@AAEXH@Z
    ARTS_IMPORT void InitCommon(i32 arg1);

    // ?InitFixed@HashTable@@AAEXHH@Z
    ARTS_IMPORT void InitFixed(i32 arg1, i32 arg2);

    // ?Recompute@HashTable@@AAEXH@Z
    ARTS_IMPORT void Recompute(i32 arg1);

    // ?First@HashTable@@0PAV1@A
    ARTS_IMPORT static HashTable* First;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u16 field_0; // 0x000
    u8 FixedSize; // 0x002
    u8 bool3; // 0x003
    FixedHashEntry* FixedHashEntrys; // 0x004
    i32 MaxSlots; // 0x008
    u32 UsedSlots; // 0x00C
    HashEntry** EntryList; // 0x010
    HashTable* Next; // 0x014
};

check_size(HashTable, 0x18);
