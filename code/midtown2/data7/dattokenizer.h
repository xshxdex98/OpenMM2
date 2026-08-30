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

#include "datasciitokenizer.h"

class Stream;

class datTokenizer : public datAsciiTokenizer
{
public:
    // ??0datTokenizer@@QAE@PBDPAVStream@@@Z
    ARTS_IMPORT datTokenizer(const char* arg1, Stream* arg2);

    // ??0datTokenizer@@QAE@XZ
    ARTS_IMPORT datTokenizer();
};

// ??_7datTokenizer@@6B@
// vtable at 0x005B2BC0
// check_size(datTokenizer, 0x1C); // size known, members are not - cannot verify
