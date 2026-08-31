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

// The multiplayer session browser. Seven of its entry points are empty in the retail build.
//
// These are the seams of a session list that shipped simpler than it was designed: the widget
// reparenting and switching that would have driven a multi-page browser, the per-protocol filters,
// and the description line. MM2 ships one flat session list, so each of these was reduced to a
// `ret` rather than removed - their addresses are still referenced.
//
// All seven are non-virtual, so none of them is reachable through a vtable and none affects the
// class's layout.

#include "netselectmenu.h"

// ?SetDescription@NetSelectMenu@@QAEXPAULocString@@@Z - 0x00504AA0
//
// LocString is a struct, not a class - `U` in the mangled name - which the generated header
// already reflects.
void NetSelectMenu::SetDescription(LocString* arg1)
{
    (void) arg1;
}

// ?ShowTCPIPNetSessions@NetSelectMenu@@QAEXXZ - 0x00504AC0
//
// The protocol filter. Retail lists whatever the transport reports without filtering by protocol.
void NetSelectMenu::ShowTCPIPNetSessions()
{}

// ?DisableSessions@NetSelectMenu@@QAEXXZ - 0x00504AD0
void NetSelectMenu::DisableSessions()
{}

// ?ReparentWidgets@NetSelectMenu@@QAEXH@Z - 0x00504DE0
//
// Would have moved the list's widgets between pages. The shipped browser is a single page.
void NetSelectMenu::ReparentWidgets(i32 arg1)
{
    (void) arg1;
}

// ?WidgetSwitch@NetSelectMenu@@QAEXHH@Z - 0x00504DF0
void NetSelectMenu::WidgetSwitch(i32 arg1, i32 arg2)
{
    (void) arg1;
    (void) arg2;
}

// ?AddWidgetToList@NetSelectMenu@@QAEXHPAVuiWidget@@@Z - 0x00504E00
void NetSelectMenu::AddWidgetToList(i32 arg1, uiWidget* arg2)
{
    (void) arg1;
    (void) arg2;
}

// ?SetTCPLocalButton@NetSelectMenu@@QAEXH@Z - 0x00504F60
void NetSelectMenu::SetTCPLocalButton(i32 arg1)
{
    (void) arg1;
}

define_dummy_symbol(ui_netselectmenu);
