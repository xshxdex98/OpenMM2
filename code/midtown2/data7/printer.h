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

#include <cstdarg>

// The engine's printf family. Every one of these is still original machine code; the declarations
// exist so reimplemented C++ can log the same way the rest of the game does.
//
// Names and mangling confirmed against data/symbols.json by address. The variadic mangling is
// PBDZZ - a const char* followed by an ellipsis - which is why these cannot be declared with a
// format-attribute wrapper without changing the symbol.

// ?Printf@@YAXPBDZZ
ARTS_IMPORT void Printf(const char* format, ...);

// ?Displayf@@YAXPBDZZ
ARTS_IMPORT void Displayf(const char* format, ...);

// ?Debugf@@YAXPBDZZ
ARTS_IMPORT void Debugf(const char* format, ...);

// ?Warningf@@YAXPBDZZ
ARTS_IMPORT void Warningf(const char* format, ...);

// ?Errorf@@YAXPBDZZ
ARTS_IMPORT void Errorf(const char* format, ...);

// ?Quitf@@YAXPBDZZ
ARTS_IMPORT void Quitf(const char* format, ...);
