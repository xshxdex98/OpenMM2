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

// THE FIRST VFTABLE HANDOVER WHERE SLOT ORDER COULD ACTUALLY BE WRONG.
//
// mmCityInfo went over with one slot, where there is only one way to arrange it. Aud3DObject
// showed that virtuals can be ported without handing the table over at all. This class is the case
// both of those left untested: ten slots, and an order nothing like alphabetical.
//
//     0 ~vehFeedback   1 SetFeedback     2 SetTimingUnit   3 GetNumActuators  4 SetActuatorValue
//     5 PlayFeedbackSample   6 PlayFeedbackSampleID   7 GetNextUnit   8 Update   9 ClearAllSamples
//
// MSVC assigns slots from DECLARATION order, so the header must declare them in exactly that
// sequence; sorted alphabetically the table would be scrambled with no link error and no crash at
// the call site. tools/genheaders.py emits virtuals in vtable order for this reason, and
// `py tools/asm_vtables.py --check vehFeedback` reads the header back and confirms it. Run that
// before touching a declaration in this class.
//
// The destructor is an ordinary virtual one here - it holds slot 0 and MSVC emits a deleting
// destructor for it, exactly as ??_GvehFeedback@@UAEPAXI@Z does in the binary. None of the
// ambiguity that stopped Aud3DObject's constructor and destructor from being ported applies.
//
// FIVE OF THE TEN VIRTUALS ARE STILL ARTS_IMPORT. Each is declared in the header, so MSVC emits an
// undefined external and the linker resolves it against the PROC still in game.asm - the C++ table
// therefore points partly here and partly at 1999 code. That is what makes a ten-slot class
// portable a few functions at a time, and it is the property this file exercises.
//
// SetActuatorValue is deliberately among them. It writes floats into the fields at 0x18 and 0x1C,
// which data/layouts.json types u32, and reaching them would need a bit-cast. The offsets are not
// in doubt - the original is `*(float *)&this[a2 + 6] = a3` - but writing a float through a u32
// field is the kind of thing that reads as though nobody checked. It goes over when those two
// fields are typed properly, alongside the 0x24 channel arrays described below.
//
// WHAT LIVES AT 0x24, for whoever types it. The constructor and PlayFeedbackSample agree that it
// is TWO 132-byte channel blocks, not the opaque byte arrays the header currently shows.
// PlayFeedbackSample indexes `this[132 * channel]`, and the constructor's loop writes eight base
// offsets at stride 4 for eight iterations. Each block is:
//
//     +0x00  f32* values[8]      +0x20  i32* labels[8]     +0x40  i32 ids[8]
//     +0x60  i32  state[8]       (-1 from the constructor, 0 once a sample plays)
//     +0x80  i32  count
//
// and 0x24 + 2*132 comes to 0x12C, which is the class size the binary allocates.

#include "vehfeedback.h"

// ??1vehFeedback@@UAE@XZ - 0x004D5740
//
// `mov dword ptr [ecx], offset ??_7vehFeedback@@6B@` and nothing else, so the body is empty: that
// single instruction is the vptr store MSVC writes at the top of any destructor for a polymorphic
// class. The class owns no memory - the channel arrays are inline storage, not pointers it
// allocated.
vehFeedback::~vehFeedback()
{}

// ?SetFeedback@vehFeedback@@UAEH_N@Z - 0x004D57A0
//
// Reports whether force feedback is available rather than enabling anything: the original is
// `return this[2] != 0`, testing the ioPad at 0x08. Its bool argument is accepted and ignored,
// which is why the parameter is unnamed rather than absent - dropping it would change the mangled
// name and the slot would no longer match.
i32 vehFeedback::SetFeedback(bool /*arg1*/)
{
    return field_8 != 0;
}

// ?SetTimingUnit@vehFeedback@@UAEHM@Z - 0x004D57B0
//
// Rejects anything at or above the limit at 0x0C, which the constructor sets to 3.0f, and
// otherwise stores it at 0x20, initialised to 0.1f.
//
// The comparison is `a2 >= (double)this[3]` in the original - both sides widened to double before
// comparing, which is what x87 does with two f32 operands and no truncation in between. Written
// the same way here rather than as an f32 compare, because /arch:IA32 keeps this on x87 and the
// widening is the behaviour, not an artifact of the decompiler.
i32 vehFeedback::SetTimingUnit(f32 arg1)
{
    if (static_cast<f64>(arg1) >= static_cast<f64>(field_C))
        return 0;

    field_20 = arg1;
    return 1;
}

// ?GetNumActuators@vehFeedback@@UAEHXZ - 0x004D57E0
//
// `xor eax, eax; ret` - the base class reports no actuators and leaves it to a derived class that
// knows the hardware to say otherwise.
i32 vehFeedback::GetNumActuators()
{
    return 0;
}
