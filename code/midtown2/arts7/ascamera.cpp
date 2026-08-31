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

// asCamera's two inert entry points.
//
// GetViewportWidth and GetViewportHeight are NOT ported, though their bodies are one line each:
// both read through gfxPipeline::m_Viewport at offsets 0x164 and 0x168, so they depend on
// gfxViewport's layout being right at those offsets rather than on anything about asCamera. They
// are grouped with the gfxViewport work.

#include "ascamera.h"

#include "vector7/matrix34.h"

// ?SetWorld@asCamera@@QAEXAAVMatrix34@@@Z - 0x004A27E0
//
// The parameter really is unused - the generated header names it `unused`, and the binary is a
// bare `ret`. The camera's world transform is written through its node, not set here.
void asCamera::SetWorld(Matrix34& unused)
{
    (void) unused;
}

// ?Regen@asCamera@@AAEXXZ - 0x004A3230
//
// Private (`AAE`). Would have rebuilt the cached projection; the projection is recomputed in place
// by the callers instead.
void asCamera::Regen()
{}

define_dummy_symbol(arts7_ascamera);
