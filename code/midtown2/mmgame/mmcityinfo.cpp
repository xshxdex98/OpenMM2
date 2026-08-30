/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2

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

// THE FIRST POLYMORPHIC CLASS PORTED.
//
// Everything before this was a plain class or a free function, where porting is only a matter of
// getting the code right. A class with virtuals brings a second thing that must be right and that
// nothing at runtime will complain about: MSVC emits its own ??_7mmCityInfo@@6B@ the moment a
// constructor or destructor is defined here, and tools/asm_vtables.py hands the original table in
// .rdata over to it. See docs/vtables.md.
//
// mmCityInfo was chosen to go first because it is the smallest such class in the binary: one
// vftable slot, no base, and a constructor and destructor that are between them nine instructions.
// If the handover mechanism is wrong, it is wrong here in a place small enough to see.

#include "mmcityinfo.h"

// ??0mmCityInfo@@QAE@XZ - 0x0052A540
//
//     mov  eax, ecx
//     xor  ecx, ecx
//     mov  dword ptr [eax], 5B524Ch      ; the vptr, which the compiler writes for us
//     mov  dword ptr [eax + 88h], ecx    ; isValid
//     mov  dword ptr [eax + 84h], ecx    ; circuitNames
//     mov  dword ptr [eax + 80h], ecx    ; checkpointNames
//     mov  dword ptr [eax + 7Ch], ecx    ; blitzNames
//     ret
//
// Four fields, and deliberately NOT the three counts beside them. blitzCount, checkpointCount and
// circuitCount at 0x8C, 0x90 and 0x94 are left holding whatever the allocation held, because Load
// writes each count and its name pointer together and nothing reads a count before then. Zeroing
// them here would be tidier and would not match, so they stay untouched.
mmCityInfo::mmCityInfo()
{
    blitzNames = nullptr;
    checkpointNames = nullptr;
    circuitNames = nullptr;
    isValid = 0;
}

// ??1mmCityInfo@@UAE@XZ - 0x0052A560
//
//     mov  dword ptr [ecx], 5B524Ch
//     ret
//
// An empty body. The single instruction is the vptr store every MSVC destructor for a polymorphic
// class begins with, so writing nothing here reproduces it exactly. The class owns no memory:
// blitzNames, checkpointNames and circuitNames point into a buffer Load does not own either.
mmCityInfo::~mmCityInfo()
{}
