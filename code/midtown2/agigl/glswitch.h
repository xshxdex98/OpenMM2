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

// THE -gl SWITCH.
//
// This header is deliberately NOT guarded by ARTS_AGIGL and deliberately declares nothing that
// touches OpenGL. It is included by reimplemented functions that must exist and behave correctly
// in a build with no GL layer at all, so it has to compile and link in every configuration.
//
// The rule the whole activation design rests on:
//
//     agiGLEnabled() == false must mean the process does exactly what it does today.
//
// Not "close to". The DirectDraw path is the fallback, it currently boots to GameLoop with zero
// access violations (docs/STATUS.md), and it is the only thing that makes the GL path debuggable.
// Every function that carries this branch therefore has to transcribe the original body EXACTLY in
// its else arm - which is why only a small, carefully chosen set of functions may carry it at all.
// See docs/opengl_activation.md for which ones and why.
//
// WHY A RUNTIME FLAG AND NOT A COMPILE-TIME ONE. tools/asm.py strips the original PROC out of
// game.asm for every symbol listed in data/ported.json. That is a build-time, whole-binary
// decision: once BeginGfx2D is registered its 0x228 bytes of DirectDraw bring-up are replaced by
// 0x228 bytes of 0xCC and there is no configuration of the resulting executable that can get them
// back. A compile-time switch would therefore not be a switch - it would be a deletion.

// True when the process was started with -gl AND a GL layer was compiled in.
//
// The answer is latched on the first call that can be trusted, so every caller in a frame agrees.
// It is safe to call from anywhere, including before datArgParser has argv - see the note in the
// implementation about why an early call cannot latch the wrong answer.
bool agiGLEnabled();

// Force the answer. Two uses, both real:
//
//   agiGLSetEnabled(false) - the GL bring-up failed and the process is falling back to DirectDraw.
//                            Everything downstream must then take the original path, including
//                            functions that have already been asked once.
//   agiGLSetEnabled(true)  - tests and tools that want the GL path without a command line.
//
// Calling this after a frame has been drawn is not supported: the device layer holds a context and
// a framebuffer, and the DirectDraw path holds surfaces, and nothing reconciles the two.
void agiGLSetEnabled(bool enabled);
