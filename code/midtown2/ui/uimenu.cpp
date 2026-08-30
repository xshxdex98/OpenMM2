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

#include "uimenu.h"

define_dummy_symbol(ui_uimenu);

// Three accessors, each transcribed from a disassembly that is three instructions or fewer.
//
// Every one was checked for the trap that makes most of Vector3 unportable: a `ret imm16` that
// disagrees with the parameter list its mangled name encodes, which is how a by-value return
// through a hidden slot hides behind a name that claims void. All three take no arguments and end
// in a bare `ret`, so there is no hidden slot and nothing to disagree about.

// ?ClearAction@UIMenu@@QAEXXZ
//
// `mov dword ptr [ecx+1Ch], 2 / ret` at 0x004E0AD0. Two is not a name here - the field is a raw
// state number and the original writes the literal.
void UIMenu::ClearAction()
{
    ActionState = 2;
}

// ?GetWidgetID@UIMenu@@QAEHXZ
//
// `mov eax, [ecx+74h] / ret` at 0x004E0B90.
i32 UIMenu::GetWidgetID()
{
    return static_cast<i32>(WidgetID);
}

// ?GetBstate@UIMenu@@QAEHXZ
//
// `mov eax, [ecx+68h] / mov eax, [eax] / ret` at 0x004E0BA0 - TWO loads, so the field at 0x68 is a
// pointer and the value wanted is what it points at. The generated header agrees: `u32*
// pCurrentWidgetID`.
//
// The null case is not guarded, deliberately. The original dereferences unconditionally, so a
// guard here would be a behaviour change that hides a caller using the menu before its widget list
// exists - and this project's rule is that the C++ does what the 1999 code did.
i32 UIMenu::GetBstate()
{
    return static_cast<i32>(*pCurrentWidgetID);
}
