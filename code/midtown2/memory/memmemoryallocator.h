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

struct memMemStats;

class memMemoryAllocator
{
public:
    // ??0memMemoryAllocator@@QAE@XZ
    ARTS_IMPORT memMemoryAllocator();

    // ??1memMemoryAllocator@@QAE@XZ
    ARTS_IMPORT ~memMemoryAllocator();

    // ?Allocate@memMemoryAllocator@@QAEPAXI_N@Z
    ARTS_IMPORT void* Allocate(u32 arg1, bool arg2);

    // ?CheckPointer@memMemoryAllocator@@QAEXPAX@Z
    ARTS_IMPORT void CheckPointer(void* arg1);

    // ?CompareHeap@memMemoryAllocator@@QAEIPAV1@@Z
    ARTS_IMPORT u32 CompareHeap(memMemoryAllocator* arg1);

    // ?CopyHeap@memMemoryAllocator@@QAE_NPAV1@@Z
    ARTS_IMPORT bool CopyHeap(memMemoryAllocator* arg1);

    // ?DisplayUsed@memMemoryAllocator@@SAXPBD@Z
    ARTS_IMPORT static void DisplayUsed(const char* arg1);

    // ?FindHeap@memMemoryAllocator@@SAPAV1@PAX@Z
    ARTS_IMPORT static memMemoryAllocator* FindHeap(void* arg1);

    // ?Free@memMemoryAllocator@@QAEXPAX_N@Z
    ARTS_IMPORT void Free(void* arg1, bool arg2);

    // ?GetStats@memMemoryAllocator@@QAEXPAUmemMemStats@@_N@Z
    ARTS_IMPORT void GetStats(memMemStats* arg1, bool arg2);

    // ?Init@memMemoryAllocator@@QAEXPAXI_N1@Z
    ARTS_IMPORT void Init(void* arg1, u32 arg2, bool arg3, bool arg4);

    // ?Kill@memMemoryAllocator@@QAEXXZ
    ARTS_IMPORT void Kill();

    // ?LogAllocations@memMemoryAllocator@@SAXH@Z
    ARTS_IMPORT static void LogAllocations(i32 arg1);

    // ?Reallocate@memMemoryAllocator@@QAEPAXPAXI@Z
    ARTS_IMPORT void* Reallocate(void* arg1, u32 arg2);

    // ?SanityCheck@memMemoryAllocator@@QAEXXZ
    ARTS_IMPORT void SanityCheck();

public:
    static memMemoryAllocator*& Access_Current() { return Current; }
    static memMemoryAllocator*& Access_First() { return First; }

private:
    // ?Link@memMemoryAllocator@@AAEXPAUnode@1@@Z
    ARTS_IMPORT void Link(memMemoryAllocator::node* arg1);

    // ?Unlink@memMemoryAllocator@@AAEXPAUnode@1@@Z
    ARTS_IMPORT void Unlink(memMemoryAllocator::node* arg1);

    // ?VerifyBlock@memMemoryAllocator@@AAEXPAX@Z
    ARTS_IMPORT void VerifyBlock(void* arg1);

    // ?Current@memMemoryAllocator@@0PAV1@A
    ARTS_IMPORT static memMemoryAllocator* Current;

    // ?First@memMemoryAllocator@@0PAV1@A
    ARTS_IMPORT static memMemoryAllocator* First;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    memMemoryAllocator* Prev; // 0x000
    void* m_pHeap; // 0x004
    i32 m_HeapSize; // 0x008
    i32 m_HeapOffset; // 0x00C
    i32 m_LockCount; // 0x010
    u8 m_UseNodes; // 0x014
    u8 m_Initialized; // 0x015
    u8 m_Debug; // 0x016
    u8 field_17; // 0x017
    memMemoryAllocator::freenode* m_Buckets[32]; // 0x018
    i32 m_AlignWaste[16]; // 0x098
    void* m_Last; // 0x0D8
};

check_size(memMemoryAllocator, 0xDC);
