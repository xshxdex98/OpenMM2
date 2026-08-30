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

#include "citylevel.h"

#include "lvl/lvlinstance.h"

define_dummy_symbol(city_citylevel);

// THE GRAPHICS DETAIL SLIDER, AND WHY IT COULD NEVER MOVE THE HORIZON.
//
// This is the whole of the "Object Detail" setting: a four-entry table of LOD distances, written
// straight into four lvlInstance statics. Nothing else in the game decides how far away scenery is
// drawn - lvlInstance::IsVisible compares against exactly these four values.
//
// The retail table is the finding, and it is visible at a glance once the switch is written out:
//
//     detail   NoDraw   VLow    Low    Med
//        0      200.0   150.0   70.0   20.0
//        1      250.0   175.0   90.0   30.0
//        2      300.0   200.0  100.0   40.0
//        3      300.0   200.0  130.0   70.0
//
// Presets 2 and 3 carry the SAME draw distance. Moving the slider from "high" to "maximum" buys
// nearer-LOD detail on things already being drawn and does not push the horizon out by a single
// unit, because the far plane sits at 400 while scenery stops at 300 - a 100-unit band of city
// that is inside the view frustum and deliberately not drawn. That is docs/gameplay_defects.md
// defect 1, and it is a data defect, not a bug: the table is simply conservative for 1999 hardware.
//
// kMaxDrawDistance is what this port is for. It was previously two byte patches into this
// function's immediates (draw-distance-high and draw-distance-max in tools/patches.py, now
// removed) - the only way to reach a value baked into an instruction. Both presets move together
// because they were already identical; changing only one would invent a difference between high
// and maximum that the original did not have.
//
// Going past the far plane means moving the far plane too, and that is a judgement about how the
// city looks in motion rather than a number to raise freely - use tools/lodtune.cpp, which sets all
// five values in the running game. It also costs depth precision: MM2 has a 16-bit depth buffer,
// and z-fighting on distant coplanar surfaces shows up long before any frame-rate cost does.
//
// sm_ObjMaxThresh is NOT written here. It is the fifth threshold and the original leaves it alone,
// so this does too.

namespace
{
    // Retail writes 300.0f for presets 2 and 3. 400.0f is the far plane they are drawn inside of.
    constexpr f32 kMaxDrawDistance = 400.0f;

    struct ObjectDetailPreset
    {
        f32 NoDraw;
        f32 VLow;
        f32 Low;
        f32 Med;
    };

    // Index is the slider value. Order matches the original's four stores, which run in descending
    // threshold order: NoDraw > VLow > Low > Med.
    constexpr ObjectDetailPreset kObjectDetail[4] = {
        {200.0f, 150.0f, 70.0f, 20.0f},
        {250.0f, 175.0f, 90.0f, 30.0f},
        {kMaxDrawDistance, 200.0f, 100.0f, 40.0f},
        {kMaxDrawDistance, 200.0f, 130.0f, 70.0f},
    };
} // namespace

// ?SetObjectDetail@cityLevel@@UAEXH@Z
void cityLevel::SetObjectDetail(i32 detail)
{
    // The original is a jump table guarded by `cmp eax, 3 / ja default`, and the default case
    // writes nothing at all. An out-of-range slider therefore leaves the previous distances in
    // place rather than falling back to a preset, so the same is done here - silently, because the
    // original is silent and a warning here would fire on whatever the caller already tolerates.
    if (static_cast<u32>(detail) > 3)
        return;

    const ObjectDetailPreset& preset = kObjectDetail[detail];

    lvlInstance::sm_ObjNoDrawThresh = preset.NoDraw;
    lvlInstance::sm_ObjVLowThresh = preset.VLow;
    lvlInstance::sm_ObjLowThresh = preset.Low;
    lvlInstance::sm_ObjMedThresh = preset.Med;
}
