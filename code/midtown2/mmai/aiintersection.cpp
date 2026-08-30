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

#include "aiintersection.h"

#include "data7/printer.h"

#include "aiobstacle.h"
#include "aipath.h"
#include "aitrafficlightset.h"
#include "aivehicle.h"

define_dummy_symbol(mmai_aiintersection);

// Read off the instruction bytes of midtown2.exe with capstone, not transcribed from
// MM2_RE_KIT/MM2_PSEUDOCODE/aiIntersection - that file inherits the ImGui/LuaIntf identifier
// contamination described in Trap 4, and its `aiPath` member reads come back as offsets into
// unrelated templates.
//
// WHAT IS DELIBERATELY NOT HERE, AND WHY. Three of this class's functions dispatch through a
// vtable, and `py tools/asm_vtables.py --check asNode` reports that asnode.h declares its eleven
// virtuals alphabetically rather than in the order the binary's table has them. A virtual call
// written against that header computes a slot index from the declaration order, so it would land
// on the wrong entry with nothing to notice:
//
//   ?Reset@aiIntersection@@QAEXXZ            tail-jumps `[[eax]+0xC]` - slot 3 of
//                                            aiTrafficLightSet, which is its Reset override.
//   ??1aiIntersection@@QAE@XZ                calls asNode::RemoveChild and then slot 0, the
//                                            scalar deleting destructor.
//   ?StopSignOkayToGo@aiIntersection@@QAE_N  calls slots 11 and 18 of aiVehicle.
//
// ?SetFourWay@ is here because the call it makes is to the NON-virtual
// ?SetFourWay@aiTrafficLightSet@@QAEXXZ, which is a direct call and needs no table at all.
//
// Init, ReadBinary and AddBangersToObsMap each own an `_SEH` funclet symbol of their own
// (Trap 10) and are left alone with it; CreateRoadMap, SaveBinary and the three IsXxxStart
// predicates are simply larger than this round's remit.

// ??0aiIntersection@@QAE@H@Z
aiIntersection::aiIntersection(i32 arg1)
{
    IntersectionId = static_cast<u16>(arg1);
    RoadCount = 0;
    TrafficLights = nullptr;
    RoomId = 0;

    // Roads, PrevVeh, NextVeh, Center, Vehicles and Bangers are NOT touched here. The original
    // writes exactly four fields - 0x12, 0x04, 0x20 and 0x10 - and memMemoryAllocator::Allocate
    // does not zero, so the rest is whatever the heap last held. Do not "fix" this: AddRoad below
    // reads Roads only when RoadCount is above zero, which is what makes it safe, and the same
    // guard is what the destructor uses before freeing it.
}

// ?AddBanger@aiIntersection@@QAEXPAVaiObstacle@@@Z
void aiIntersection::AddBanger(aiObstacle* arg1)
{
    arg1->Next = Bangers;
    Bangers = arg1;
}

// ?AddRoad@aiIntersection@@QAEXPAVaiPath@@@Z
void aiIntersection::AddRoad(aiPath* road)
{
    if (!road)
        return;

    // Grow by one every time, copy, then free. There is no null test on the result - the original
    // uses it immediately - and the old array is released only when there was one.
    aiPath** roads = static_cast<aiPath**>(operator new(static_cast<usize>(RoadCount * 4 + 4)));

    for (i32 i = 0; i < RoadCount; ++i)
        roads[i] = Roads[i];

    roads[RoadCount] = road;

    if (RoadCount > 0)
        operator delete(Roads);

    ++RoadCount;
    Roads = roads;
}

// ?AddToStopSignCntl@aiIntersection@@QAEXPAVaiVehicle@@@Z
void aiIntersection::AddToStopSignCntl(aiVehicle* arg1)
{
    // Appends at the tail of the PrevVeh list, which is the queue of vehicles waiting at the sign.
    // The link is aiVehicle::Next at 0x00C, not the aiObstacle::Next at 0x004 that AddVehicle uses.
    aiVehicle* tail = PrevVeh;

    if (tail)
    {
        while (tail->Next)
            tail = tail->Next;

        tail->Next = arg1;
    }
    else
    {
        PrevVeh = arg1;
    }

    arg1->Next = nullptr;
}

// ?AddVehicle@aiIntersection@@QAEXPAVaiObstacle@@@Z
void aiIntersection::AddVehicle(aiObstacle* arg1)
{
    arg1->Next = Vehicles;
    Vehicles = arg1;
}

// ?Dump@aiIntersection@@QAEXXZ
void aiIntersection::Dump()
{
    Displayf("\t\tRoad Count = %d", RoadCount);

    // Both ids are read with movsx, so they reach the varargs as signed shorts even though the
    // recovered layout calls them u16. Kept, because it is what the format string receives.
    for (i32 i = 0; i < RoadCount; ++i)
    {
        aiPath* road = Roads[i];

        Displayf("\t\t\tPath: %d, Dest: %d", static_cast<i16>(road->Id),
            static_cast<i16>(road->Sink->IntersectionId));
    }
}

// ?NumAvailSinks@aiIntersection@@QAEHPAVaiPath@@@Z
i32 aiIntersection::NumAvailSinks(aiPath* arg1)
{
    i32 count = 0;

    for (i32 i = 0; i < RoadCount; ++i)
    {
        aiPath* road = Roads[i];

        if (road == arg1)
            continue;

        if (road->Source == this)
        {
            if (!(road->Flags2 & 1))
                ++count;
        }
        else if (road->SidewalkVerticesThing2 && !(road->Flags & 1))
        {
            ++count;
        }
    }

    return count;
}

// ?NumOneWays@aiIntersection@@QAEHXZ
i32 aiIntersection::NumOneWays()
{
    i32 count = 0;

    // No null test on the element, unlike Road() above. Transcribed as it stands.
    for (i32 i = 0; i < RoadCount; ++i)
    {
        if (Roads[i]->Flags & 1)
            ++count;
    }

    return count;
}

// ?NumSinks@aiIntersection@@QAEHXZ
i32 aiIntersection::NumSinks()
{
    i32 count = 0;

    for (i32 i = 0; i < RoadCount; ++i)
    {
        aiPath* road = Roads[i];

        if (road->Source == this)
        {
            if (road->Flags2 & 1)
                ++count;
        }
        else if (road->SidewalkVerticesThing2 && (road->Flags & 1))
        {
            ++count;
        }
    }

    return count;
}

// ?NumSources@aiIntersection@@QAEHXZ
i32 aiIntersection::NumSources()
{
    i32 count = 0;

    for (i32 i = 0; i < RoadCount; ++i)
    {
        aiPath* road = Roads[i];

        if (road->Sink == this || road->SidewalkVerticesThing2)
            ++count;
    }

    return count;
}

// ?Path@aiIntersection@@QAEPAVaiPath@@H@Z
aiPath* aiIntersection::Path(i32 arg1)
{
    // Byte for byte the same function as Road below, emitted twice at two addresses.
    if (arg1 < 0 || arg1 > RoadCount)
        return nullptr;

    return Roads[arg1];
}

// ?RemoveBanger@aiIntersection@@QAEXPAVaiObstacle@@@Z
void aiIntersection::RemoveBanger(aiObstacle* arg1)
{
    aiObstacle* prev = nullptr;

    for (aiObstacle* cur = Bangers; cur; prev = cur, cur = cur->Next)
    {
        if (cur == arg1)
        {
            if (prev)
                prev->Next = cur->Next;
            else
                Bangers = cur->Next;

            return;
        }
    }
}

// ?RemoveFromStopSignCntl@aiIntersection@@QAEXPAVaiVehicle@@@Z
void aiIntersection::RemoveFromStopSignCntl(aiVehicle* arg1)
{
    aiVehicle* prev = nullptr;

    for (aiVehicle* cur = NextVeh; cur; prev = cur, cur = cur->Next)
    {
        if (cur == arg1)
        {
            if (cur == NextVeh)
                NextVeh = cur->Next;
            else
                prev->Next = cur->Next;

            return;
        }
    }

    // Falling off the end is the error path, and it is the only thing in this function that is
    // visible from outside. Both ids reach the varargs through movsx.
    Displayf("ERROR: Vehicle %d did not have permission to traverse intersection %d.",
        static_cast<i16>(arg1->VehicleId), static_cast<i16>(IntersectionId));
}

// ?RemoveTotalFromStopSignCntl@aiIntersection@@QAEXPAVaiVehicle@@@Z
void aiIntersection::RemoveTotalFromStopSignCntl(aiVehicle* arg1)
{
    // The same unlink twice, over the two lists that share aiVehicle::Next as their link: the one
    // headed by NextVeh (vehicles currently granted the intersection) and the one headed by
    // PrevVeh (vehicles queued for it). Unlike RemoveFromStopSignCntl there is no complaint when
    // the vehicle is in neither.
    aiVehicle* prev = nullptr;

    for (aiVehicle* cur = NextVeh; cur; prev = cur, cur = cur->Next)
    {
        if (cur == arg1)
        {
            if (cur == NextVeh)
                NextVeh = cur->Next;
            else
                prev->Next = cur->Next;

            break;
        }
    }

    prev = nullptr;

    for (aiVehicle* cur = PrevVeh; cur; prev = cur, cur = cur->Next)
    {
        if (cur == arg1)
        {
            if (cur == PrevVeh)
                PrevVeh = cur->Next;
            else
                prev->Next = cur->Next;

            break;
        }
    }
}

// ?RemoveVehicle@aiIntersection@@QAEXPAVaiObstacle@@@Z
void aiIntersection::RemoveVehicle(aiObstacle* arg1)
{
    aiObstacle* prev = nullptr;

    for (aiObstacle* cur = Vehicles; cur; prev = cur, cur = cur->Next)
    {
        if (cur == arg1)
        {
            if (prev)
                prev->Next = cur->Next;
            else
                Vehicles = cur->Next;

            return;
        }
    }
}

// ?Road@aiIntersection@@QAEPAVaiPath@@H@Z
aiPath* aiIntersection::Road(i32 arg1)
{
    // The upper bound is `jg`, not `jge`, so an index equal to RoadCount passes the guard. AddRoad
    // allocates exactly RoadCount slots, so that index reads one dword past the end of the array.
    // It is an off-by-one in the 1999 code and it is reproduced rather than corrected.
    if (arg1 < 0 || arg1 > RoadCount)
        return nullptr;

    return Roads[arg1];
}

// ?SetFourWay@aiIntersection@@QAEXXZ
void aiIntersection::SetFourWay()
{
    // Clearing the banger list here looks unrelated to traffic lights and is exactly what the
    // original does - `mov dword ptr [ecx+0x28], 0` before the forward. Reset does the same to the
    // same field, so the two read as a pair.
    Bangers = nullptr;

    if (TrafficLights)
        TrafficLights->SetFourWay();
}

// ?StopSources@aiIntersection@@QAEXH@Z
void aiIntersection::StopSources(i32 arg1)
{
    for (i32 i = 0; i < RoadCount; ++i)
    {
        aiPath* road = Roads[i];

        // Which end of the road this intersection is on picks which of the two 16-bit fields is
        // tested. The original holds it in `ax` and tests `test ax, ax` then `cmp ax, 1`; the
        // value is zero-extended out of memory, so widening it here changes neither comparison.
        i32 state = (road->Sink == this) ? road->field_11E : road->field_142;

        if (state == 0 || state == 1)
            road->AllwaysStop(arg1);
    }
}
