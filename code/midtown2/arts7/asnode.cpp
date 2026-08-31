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

// asNode is the base of the whole scene graph, and these three are its serialization hooks. A node
// with nothing to persist implements none of them, which is most of them - the DAT file format
// drives loading through datParser, and only the nodes with tunable fields register any.
//
// GetClassName and GetClassNameA are NOT ported: both return the string literal at 0x005C2D78,
// and their return type is a non-const char*, so they need the literal handed over without a
// const_cast. That belongs with the rest of the string handling rather than in this file.

#include "asnode.h"

#include "data7/datparser.h"

// ?FileIO@asNode@@UAEXAAVdatParser@@@Z - 0x00403330
//
// Where a node registers its fields with the parser. The base has none, so the DAT reader walks
// straight past it.
void asNode::FileIO(datParser& arg1)
{
    (void) arg1;
}

// ?AfterLoad@asNode@@UAEXXZ - 0x00403340
//
// The hook for fixing up whatever FileIO read. Nothing was read, so nothing needs fixing.
void asNode::AfterLoad()
{}

// ?BeforeSave@asNode@@UAEXXZ - 0x00403350
void asNode::BeforeSave()
{}

define_dummy_symbol(arts7_asnode);
