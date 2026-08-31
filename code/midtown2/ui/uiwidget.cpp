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

// uiWidget is the base of every menu control, and these are the defaults it hands the ones that do
// not care: a widget that takes no input ignores Action, one with no tooltip returns no
// description, one that is not laid out by pixel has no screen height.
//
// Only the empty defaults are ported. Everything of uiWidget that actually does something - the
// layout, the hit testing, the draw - is still in game.asm.

#include "uiwidget.h"

#include "eventq7/eqevent.h"

// ?Action@uiWidget@@UAEXTeqEvent@@@Z - 0x004E73A0
//
// The event is taken BY VALUE - `T` in the mangled name is MSVC's tag for a union passed by value,
// where a pointer would mangle PAT - so the signature has to match or the symbol does not.
void uiWidget::Action(eqEvent arg1)
{
    (void) arg1;
}

// ?CaptureAction@uiWidget@@UAEXTeqEvent@@@Z - 0x004E73B0
//
// The variant called while this widget holds the input capture. Same default: ignore it.
void uiWidget::CaptureAction(eqEvent arg1)
{
    (void) arg1;
}

// ?EvalMouseX@uiWidget@@UAEXM@Z - 0x004E73C0
//
// Only the sliders track the horizontal mouse position; every other widget discards it.
void uiWidget::EvalMouseX(f32 arg1)
{
    (void) arg1;
}

// ?ReturnDescription@uiWidget@@UAEPADXZ - 0x004E73D0
//
// Null, not an empty string - the caller tests the pointer before showing a description line.
char* uiWidget::ReturnDescription()
{
    return nullptr;
}

// ?SetPosition@uiWidget@@UAEXMM@Z - 0x004E73E0
//
// Ignored by the base. A widget that can be moved keeps its own rectangle and overrides this.
void uiWidget::SetPosition(f32 arg1, f32 arg2)
{
    (void) arg1;
    (void) arg2;
}

// ?GetScreenHeight@uiWidget@@UAEMXZ - 0x004E73F0
//
// Zero height by default, so a list laying widgets out vertically stacks them at one point unless
// they report a real size.
f32 uiWidget::GetScreenHeight()
{
    return 0.0f;
}

// ?AddToolTip@uiWidget@@QAEXPAVUIMenu@@PAULocString@@@Z - 0x004E7470
//
// A bare `ret`, and the only non-virtual of the seven. Tooltips are registered on the menu rather
// than the widget in MM2, so the widget-side entry point was left empty.
void uiWidget::AddToolTip(UIMenu* arg1, LocString* arg2)
{
    (void) arg1;
    (void) arg2;
}

define_dummy_symbol(ui_uiwidget);
