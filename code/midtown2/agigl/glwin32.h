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

// RECONCILING TWO SPELLINGS OF THE SAME IMPORT.
//
// This is a link-time defect in everything under agigl/ that declares a Win32 function, and it will
// surface the moment -DARTS_AGIGL is turned on. It is fixed here centrally rather than in each
// file, because the cause is one thing and there is no per-file variation.
//
// The two spellings:
//
//   The ASSEMBLY references imports UNDECORATED. tools/ghidra/ExportAsm.java emits
//   `dd offset __imp__MessageBoxA`, because an undecorated name is all Ghidra reports -
//   reconstructing the __stdcall @N suffix would mean recovering every API's parameter byte count.
//   tools/implibs.py then builds the import libraries from a .def full of undecorated names, so
//   build/implibs/USER32.lib really does define `__imp__MessageBoxA` and nothing else. Confirmed
//   with dumpbin: every entry reads "Name type: no prefix".
//
//   The COMPILER references them DECORATED. `extern "C" __declspec(dllimport) int __stdcall
//   MessageBoxA(...)` emits a reference to `__imp__MessageBoxA@16`, because that is what __stdcall
//   means on x86. There is no such symbol in the import library, and the link fails with an
//   unresolved external naming a function that is plainly right there.
//
// Declaring the imports __cdecl instead would resolve the name and corrupt the stack: a __stdcall
// callee pops the arguments and a __cdecl caller pops them again. So the convention has to stay
// __stdcall and the NAME has to be reconciled, which is what /alternatename does - "if this symbol
// is undefined, use that one instead". Both spellings denote the same IAT slot, so the call is
// identical either way.
//
// Every entry below is one Win32 function declared somewhere under agigl/, with its @N computed
// from its parameter list on x86: four bytes per argument, HWND/HDC/HMODULE and every pointer
// included. A wrong @N here does not misbehave - it simply fails to reconcile and the original
// unresolved-external error comes back, naming the decorated form. That is the good kind of
// failure, and it is why this is done with a linker alias rather than by declaring the functions
// through raw IAT pointers.

#define ARTS_ALIAS_IMPORT(NAME, BYTES) \
    __pragma(comment(linker, "/alternatename:__imp__" #NAME "@" #BYTES "=__imp__" #NAME))

// KERNEL32 - glload.cpp
ARTS_ALIAS_IMPORT(LoadLibraryA, 4)
ARTS_ALIAS_IMPORT(GetProcAddress, 8)
ARTS_ALIAS_IMPORT(FreeLibrary, 4)

// USER32 - glcontext.cpp, glscreen.cpp, glactivate.cpp, glsetres.cpp
ARTS_ALIAS_IMPORT(GetDC, 4)
ARTS_ALIAS_IMPORT(ReleaseDC, 8)
ARTS_ALIAS_IMPORT(GetSystemMetrics, 4)
ARTS_ALIAS_IMPORT(GetClientRect, 8)
ARTS_ALIAS_IMPORT(MessageBoxA, 16)
ARTS_ALIAS_IMPORT(MoveWindow, 24)

// glbeginphase.cpp - ChangeDisplaySettingsA(DEVMODEA*, DWORD), two arguments.
ARTS_ALIAS_IMPORT(ChangeDisplaySettingsA, 8)

// GDI32 - glsetres.cpp
ARTS_ALIAS_IMPORT(GetDeviceCaps, 8)
