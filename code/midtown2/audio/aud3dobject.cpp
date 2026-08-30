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

// VIRTUALS PORTED WITHOUT HANDING OVER THE VFTABLE.
//
// tools/asm_vtables.py exists because defining a constructor or destructor for a polymorphic class
// makes MSVC emit its own ??_7C@@6B@, which collides with the copy in .rdata. Defining only
// virtuals that are neither does not, so no table is emitted and none has to be handed over.
//
// The original table then keeps working and repoints itself: game.asm holds
// `dd offset ?AssignSounds@Aud3DObject@@UAEXXZ`, tools/asm.py strips that PROC and declares the
// symbol EXTERN, and the linker resolves the table entry to the C++ below. Slot ORDER cannot go
// wrong here because the 1999 table is still the one in use - which makes this the cheapest way to
// begin on a polymorphic class, and worth reaching for before the full handover.
//
// WHY THE CONSTRUCTOR AND DESTRUCTOR ARE DELIBERATELY LEFT ALONE, and what has to be resolved
// before anyone ports them:
//
// midtown2.map names the destructor ??1Aud3DObject@@UAE@XZ, and `U` is public virtual - but it
// occupies no slot in this class's six-entry table, none in any of the five derived classes'
// tables, and no ??_G or ??_E was ever emitted for it. The dwords on both sides of the table are
// float constants, so it is not a table that runs longer than it appears. Measured across the
// binary: 228 classes have a virtual destructor WITH a slot, 47 have a non-virtual one without,
// and 15 have the `U` mangling and no slot - twelve of those fifteen are this family.
//
// That matters because the two readings need different C++ and only one can be right:
//
//   declare it `virtual`      MSVC puts a deleting destructor in slot 0 and shifts all six
//                             methods down one. The table would have seven entries where the
//                             binary has six.
//   declare it non-virtual    the table matches, but the destructor then mangles as
//                             ??1Aud3DObject@@QAE@XZ, so the assembly's copy under the old name is
//                             no longer the same symbol - and derived destructors still call it.
//
// Neither is free, so the question is left open rather than settled by whichever one links.
// vehFeedback is the class to port next if a full handover is wanted: ten slots, no base, and slot
// 0 is an ordinary ??_G deleting destructor with none of this ambiguity.

#include "aud3dobject.h"

// The five default virtuals, each a bare `ret` in the original. They exist to be overridden - the
// derived audio classes replace them - and the base is deliberately inert rather than abstract:
// no slot in the table is __purecall, so an Aud3DObject can be constructed on its own.
//
// Update(), the sixth, is left ARTS_IMPORT. A virtual that has not been ported is still declared
// in the header, so MSVC emits an undefined external and the linker resolves it against the PROC
// still in game.asm. Slot 3 therefore reaches the 1999 code while the other five reach this file.

// ?AssignSounds@Aud3DObject@@UAEXXZ - 0x005122F0
void Aud3DObject::AssignSounds()
{}

// ?UnAssignSounds@Aud3DObject@@UAEXH@Z - 0x00512300
//
// `ret 4` in the original: it takes its argument and ignores it. The parameter is unnamed rather
// than absent because removing it would change the mangled name, and the mangled name is what ties
// this definition to the slot.
void Aud3DObject::UnAssignSounds(i32 /*arg1*/)
{}

// ?UpdateAudio@Aud3DObject@@UAEXXZ - 0x00512310
void Aud3DObject::UpdateAudio()
{}

// ?SetNon3DParams@Aud3DObject@@UAEXXZ - 0x00512360
void Aud3DObject::SetNon3DParams()
{}

// ?Set3DParams@Aud3DObject@@UAEXXZ - 0x00512370
void Aud3DObject::Set3DParams()
{}
