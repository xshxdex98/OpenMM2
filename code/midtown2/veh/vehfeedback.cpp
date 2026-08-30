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
// The fields at 0x18 and 0x1C, and everything from 0x24, were untyped when this file was first
// written, which is why SetActuatorValue and ClearAllSamples were left in the assembly. They are
// typed now - see data/layouts_corrections.json for the evidence - so both are here.
//
// WHAT LIVES AT 0x24. Two 132-byte channel blocks, each four 8-element arrays and a count:
//
//     +0x00  f32* SampleValues[8]   the pointer a sample writes its value through
//     +0x20  i32* SampleLabels[8]   +0x40  i32 SampleLength[8]  units in the sample
//     +0x60  i32  SamplePos[8]      -1 when free, 0 at the start, then a cursor
//     +0x80  i32  SampleCount
//
// and 0x24 + 2*132 = 0x12C, the size the binary allocates. They are flat per-channel arrays rather
// than a nested struct only because data/layouts.json cannot express one.

#include "vehfeedback.h"

// For Timer::Ticks(), which SetActuatorValue stamps into Timers[].
#include "misc/timer.h"

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
// `return this[2] != 0`, testing the ioPad at 0x08 (Pad). Its bool argument is accepted and ignored,
// which is why the parameter is unnamed rather than absent - dropping it would change the mangled
// name and the slot would no longer match.
i32 vehFeedback::SetFeedback(bool /*arg1*/)
{
    return Pad != nullptr;
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
    if (static_cast<f64>(arg1) >= static_cast<f64>(MaxTimingUnit))
        return 0;

    TimingUnit = arg1;
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

// ?SetActuatorValue@vehFeedback@@UAEHHM@Z - 0x004D57F0
//
// THE BOUND IS UNSIGNED IN THE ORIGINAL - `unsigned int a2` against `a2 <= 1` - so a negative
// index does not pass it, and the cast here is what preserves that. Comparing a signed i32 against
// 1 would let -1 through and write four bytes before ActuatorValues. The mangled name says the
// parameter is `H`, a signed int, so the signedness lives in the comparison rather than the type.
//
// Returns 0 unconditionally, including when it did nothing.
i32 vehFeedback::SetActuatorValue(i32 arg1, f32 arg2)
{
    if (Pad && static_cast<u32>(arg1) <= 1)
    {
        ActuatorValues[arg1] = arg2;
        Timers[arg1].StartTime = Timer::Ticks();
    }

    return 0;
}

// ?ClearAllSamples@vehFeedback@@UAEH_N@Z - 0x004D5910
//
// Frees every sample slot in both channels. The original walks all eight arrays in one loop over
// the slot index, writing the two counts redundantly on every iteration; written here as a loop
// per channel, which touches exactly the same bytes.
//
// SamplePos goes to -1 rather than 0 - that is the free marker, and the constructor sets it the
// same way. PlayFeedbackSample writes 0 there when it takes a slot.
i32 vehFeedback::ClearAllSamples(bool arg1)
{
    for (i32 i = 0; i < 8; ++i)
    {
        Ch0SampleValues[i] = nullptr;
        Ch0SampleLabels[i] = nullptr;
        Ch0SampleLength[i] = 0;
        Ch0SamplePos[i] = -1;

        Ch1SampleValues[i] = nullptr;
        Ch1SampleLabels[i] = nullptr;
        Ch1SampleLength[i] = 0;
        Ch1SamplePos[i] = -1;
    }

    Ch0SampleCount = 0;
    Ch1SampleCount = 0;

    // Zero both actuators, but only when asked and only if there is a pad to zero them on. The
    // original reaches SetActuatorValue through the vtable at slot 4; calling it directly is the
    // same dispatch, since it is virtual and this is the same object.
    if (Pad && arg1)
    {
        SetActuatorValue(0, 0.0f);
        SetActuatorValue(1, 0.0f);
    }

    return 1;
}

// ?PlayFeedbackSample@vehFeedback@@UAEHHHPAMPAH@Z - 0x004D5830
//
// Claims the first free slot in a channel and records a sample in it. The original:
//
//     v6 = &this[132 * a2];
//     for ( i = v6 + 36; *i; ++i )        // scan SampleValues for a null entry
//         if ( ++v5 >= 8 ) return 0;      // all eight taken
//     v9 = a2 + 32 * a2 + v5;             // == 33 * a2 + v5
//     this[4*v9 +  36] = a4;              // SampleValues[v5] = the f32* argument
//     this[4*v9 +  68] = a5;              // SampleLabels[v5] = the i32* argument
//     this[4*v9 + 100] = a3;              // SampleLength[v5]    = the i32 argument
//     this[128*a2 + 132 + 4*v5 + 4*a2] = 0;   // SamplePos[v5] = 0, i.e. in use
//     ++*((_DWORD *)v6 + 41);             // ++SampleCount
//
// Every one of those offsets is 132*a2 plus a multiple of 32 from the block base, which is what
// established the layout in data/layouts_corrections.json in the first place.
//
// THE ONE DIVERGENCE, AND WHY IT IS UNREACHABLE. The original applies no bounds check to the
// channel: it computes 132 * a2 for any value, so a3 of 2 would write past the end of the object.
// Selecting between the two channel array sets, as below, cannot do that. Reproducing the
// unbounded arithmetic would need a reinterpret_cast off the class base, which would throw away
// the typing that makes the rest of this function readable.
//
// It is unreachable regardless: the only callers in the binary are the two vtable dispatches in
// PlayFeedbackSampleID, which pass 0 and 1. Anything else would have been corrupting memory in
// 1999 as well.
i32 vehFeedback::PlayFeedbackSample(i32 arg1, i32 arg2, f32* arg3, i32* arg4)
{
    f32** values = arg1 ? Ch1SampleValues : Ch0SampleValues;
    i32** labels = arg1 ? Ch1SampleLabels : Ch0SampleLabels;
    i32* ids = arg1 ? Ch1SampleLength : Ch0SampleLength;
    i32* state = arg1 ? Ch1SamplePos : Ch0SamplePos;
    i32& count = arg1 ? Ch1SampleCount : Ch0SampleCount;

    // Stops before reading values[8]: the eighth failed test increments slot to 8 and returns.
    i32 slot = 0;

    while (values[slot])
    {
        if (++slot >= 8)
            return 0;
    }

    values[slot] = arg3;
    labels[slot] = arg4;
    ids[slot] = arg2;
    state[slot] = 0;
    ++count;

    return 1;
}

// ?GetNextUnit@vehFeedback@@UAEMH@Z - 0x004D5980
//
// Advances every playing sample in a channel by one unit and returns the value that should reach
// the hardware. Where two samples disagree the higher label wins, and between equal labels the
// larger value does - so the label is a priority and the result is a max, not a sum.
//
// THIS IS THE FUNCTION THAT NAMED THE FIELDS. Nothing that only fills the arrays could show what
// they mean; this is the one that reads them back:
//
//     v7 = SamplePos[slot];  SamplePos[slot] = v7 + 1;   a cursor, not a state flag
//     if ( v7 < SampleLength[slot] )                     the third array is a length
//     value = SampleValues[slot][v7]                     both pointers are indexed BY the cursor
//     label = SampleLabels[slot][v7]
//
// and when the cursor reaches the length the slot is released, which is where the constructor's -1
// and PlayFeedbackSample's 0 turn out to be a free marker and a start position.
//
// The cursor is advanced whether or not it is still in range, and the release test uses the
// ALREADY-INCREMENTED value - so a sample is freed on the call that consumes its last unit, not on
// the one after. Both are reproduced rather than tidied.
//
// The parameter is the channel, despite the kit naming it `label_ids`; every use of it in the
// original is `132 * it`. See the WRONG table in tools/kit_paramnames.py.
f32 vehFeedback::GetNextUnit(i32 arg1)
{
    f32** values = arg1 ? Ch1SampleValues : Ch0SampleValues;
    i32** labels = arg1 ? Ch1SampleLabels : Ch0SampleLabels;
    i32* length = arg1 ? Ch1SampleLength : Ch0SampleLength;
    i32* cursor = arg1 ? Ch1SamplePos : Ch0SamplePos;
    i32& count = arg1 ? Ch1SampleCount : Ch0SampleCount;

    f32 result = 0.0f;
    i32 best = -1;

    for (i32 slot = 0; slot < 8; ++slot)
    {
        if (!values[slot])
            continue;

        i32 at = cursor[slot];
        cursor[slot] = at + 1;

        if (at < length[slot])
        {
            i32 label = labels[slot][at];
            f32 value = values[slot][at];

            if (label > best)
            {
                result = value;
                best = label;
            }
            else if (label == best && value > result)
            {
                result = value;
            }
        }

        if (at + 1 == length[slot])
        {
            values[slot] = nullptr;
            labels[slot] = nullptr;
            length[slot] = 0;
            cursor[slot] = -1;
            --count;
        }
    }

    return result;
}

// ?Update@vehFeedback@@UAEHXZ - 0x004D5A80
//
// Drives each actuator from its sample queue, no more often than TimingUnit. For every actuator:
// if nothing is queued, release it back to zero if it is not already there; otherwise take the
// next unit and write it only when it differs from what the actuator already holds.
//
// The original reaches its own methods through the vtable - slot 3 for GetNumActuators, 4 for
// SetActuatorValue, 7 for GetNextUnit - which is what calling them directly does here, since all
// three are virtual and it is the same object. A derived class overriding any of them is dispatched
// to either way.
//
// The offsets decode as: this+2 is Pad, this+6 is ActuatorValues, (_DWORD *)v3 - 2 is
// Timers[i].StartTime, this[8] is TimingUnit, and v7 walks the two SampleCounts 33 ints - one
// 132-byte channel block - apart.
//
// THE ELAPSED TEST IS DONE IN DOUBLE, and written that way rather than in f32:
//
//     (double)(Timer::Ticks() - Timers[i].StartTime) * Timer::TicksToSeconds > TimingUnit
//
// TicksToSeconds is an f32 and so is TimingUnit, but the left side is widened first and the
// comparison follows in double - which is what x87 does under /arch:IA32 and is the behaviour, not
// a decompiler artifact. SetActuatorValue restamps Timers[i], which is what makes this a rate
// limit rather than a check that always passes.
//
// NOTHING HAPPENS IN THE BASE CLASS. GetNumActuators returns 0 here, so the loop never runs; this
// only does work once a derived class reports hardware. That is also why the same unbounded-index
// pattern as PlayFeedbackSample is unreachable: an actuator count above 2 would walk past both
// two-element arrays and both channel blocks, in the original as much as here.
i32 vehFeedback::Update()
{
    if (!Pad)
        return 0;

    i32 actuators = GetNumActuators();

    for (i32 i = 0; i < actuators; ++i)
    {
        f64 elapsed = static_cast<f64>(Timer::Ticks() - Timers[i].StartTime) * Timer::TicksToSeconds;

        if (!(elapsed > static_cast<f64>(TimingUnit)))
            continue;

        if ((i ? Ch1SampleCount : Ch0SampleCount) <= 0)
        {
            if (ActuatorValues[i] > 0.0f)
                SetActuatorValue(i, 0.0f);
        }
        else
        {
            f32 next = GetNextUnit(i);

            if (ActuatorValues[i] != next)
                SetActuatorValue(i, next);
        }
    }

    return 1;
}

// THE CANNED EFFECT PlayFeedbackSampleID PLAYS.
//
// The original passes three .rdata addresses, read out of an argument table at 0x005B2E40:
//
//     005B2E40: 00000004  005CF874  005CF894      channel 0: length, values, labels
//     005B2E50: 00000004  005CF884  005CF894      channel 1: length, values, labels
//
// and the bytes at those three addresses are, verified against midtown2.exe:
//
//     005CF874  1.0  0.0  1.0  0.0     a double pulse
//     005CF884  1.0  0.9  0.7  0.4     a decaying rumble
//     005CF894  1    1    1    1       every unit at priority 1
//
// DEFINED HERE RATHER THAN REFERENCED, which is a deliberate trade. None of the three carries a
// name in midtown2.map - they are unnamed constants - so there is no symbol to link against. The
// exporter reaches them as `?PtxName@vehWheelPtx@@2PAPADA + 620`, its nearest-symbol fallback,
// which relocates correctly but is not something C++ should depend on: it ties this function to an
// unrelated class's address by a magic offset. Exporting a generated label instead would mean
// growing LINKABLE_GLOBALS, which is deliberately not grown.
//
// The cost is that the original 48 bytes stay in .rdata unreferenced by our code. Nothing else in
// the binary reads them - the table at 0x005B2E40 is used by this function alone - and both the
// function that stores these pointers and the one that reads back through them are now C++, so the
// duplication is invisible outside this file.
static f32 SampleValuesPulse[4] = {1.0f, 0.0f, 1.0f, 0.0f};
static f32 SampleValuesDecay[4] = {1.0f, 0.9f, 0.7f, 0.4f};
static i32 SampleLabelsUnit[4] = {1, 1, 1, 1};

// ?PlayFeedbackSampleID@vehFeedback@@UAEHH@Z - 0x004D58B0
//
// Queues the one canned effect this class knows: a double pulse on the first actuator and a
// decaying rumble on the second. Effect 0 is the only one - any other id returns 0 without
// touching anything, which is what `!a2 &&` does at the top of the original.
//
// The two calls are chained with && in the original, so the second is skipped when the first finds
// no free slot. Reproduced with an early return rather than by evaluating both.
i32 vehFeedback::PlayFeedbackSampleID(i32 arg1)
{
    if (arg1 != 0)
        return 0;

    if (!PlayFeedbackSample(0, 4, SampleValuesPulse, SampleLabelsUnit))
        return 0;

    return PlayFeedbackSample(1, 4, SampleValuesDecay, SampleLabelsUnit);
}
