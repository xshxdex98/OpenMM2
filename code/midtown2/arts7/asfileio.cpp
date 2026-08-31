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

// asFileIO is the mixin that gives a class DAT-file persistence. Its three hooks are protected
// (`MAE` in the mangled name, not `UAE`) and all three are empty in the base: a class with nothing
// to persist inherits them and registers no fields.
//
// GetClassName is not here. It returns the string literal at 0x005DD920 through a const char*,
// and the string ports are grouped separately.

#include "asfileio.h"

#include "data7/datparser.h"

// ?FileIO@asFileIO@@MAEXAAVdatParser@@@Z - 0x005979B0
void asFileIO::FileIO(datParser& arg1)
{
    (void) arg1;
}

// ?AfterLoad@asFileIO@@MAEXXZ - 0x00595AE0
void asFileIO::AfterLoad()
{}

// ?BeforeSave@asFileIO@@MAEXXZ - 0x00595AF0
void asFileIO::BeforeSave()
{}

define_dummy_symbol(arts7_asfileio);
