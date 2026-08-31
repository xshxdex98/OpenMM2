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

// The Windows mixer control - the master, wave and CD volume sliders in the audio options.
//
// Three of its entry points are inert in the retail build. MM2 sets volume through DirectSound and
// the CD audio interface directly, so the mixer API it was written against is never driven: the
// refresh does nothing and both getters report silence.
//
// WindowProc is NOT ported. It is private (`AAE` in the mangled name) and takes an HWND__*, so it
// needs the Win32 window handle type; that belongs with the rest of the platform surface in
// pcwindis rather than pulled in here for one stub.

#include "mixerctl.h"

// ?RefreshAll@MixerCTL@@QAEXK@Z - 0x0051C2E0
//
// Would have re-read every slider from the mixer device after an external change.
void MixerCTL::RefreshAll(ulong arg1)
{
    (void) arg1;
}

// ?GetWaveVolume@MixerCTL@@QAEMXZ - 0x0051C300
//
// Zero rather than a cached value: nothing ever populated the mixer's wave line.
f32 MixerCTL::GetWaveVolume()
{
    return 0.0f;
}

// ?GetCDVolume@MixerCTL@@QAEMXZ - 0x0051C360
f32 MixerCTL::GetCDVolume()
{
    return 0.0f;
}

define_dummy_symbol(misc_mixerctl);
