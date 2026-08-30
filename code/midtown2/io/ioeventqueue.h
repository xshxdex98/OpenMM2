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

struct ioEvent;

class ioEventQueue
{
public:
    // ?Command@ioEventQueue@@SAXPAX@Z
    ARTS_IMPORT static void Command(void* modifiers);

    // ?Peek@ioEventQueue@@SA_NAAUioEvent@@AAH@Z
    ARTS_IMPORT static bool Peek(ioEvent& event, i32& index);

    // ?Pop@ioEventQueue@@SA_NAAUioEvent@@@Z
    ARTS_IMPORT static bool Pop(ioEvent& arg1);

    // ?Queue@ioEventQueue@@SAXW4ioEventType@ioEvent@@HHH@Z
    ARTS_IMPORT static void Queue(ioEvent::ioEventType type, i32 x, i32 y, i32 modifiers);
};

// check_size(ioEventQueue, 0x1); // size known, members are not - cannot verify
