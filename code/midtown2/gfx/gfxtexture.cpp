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

#include "gfxtexture.h"

define_dummy_symbol(gfx_gfxtexture);

// Three of gfxTexture's small methods, each checked against its machine code as well as against the
// decompilation in MM2_RE_KIT. Every one was read for the trap that makes most of Vector3
// unportable - a `ret imm16` that disagrees with the parameter list the mangled name encodes, which
// is how a by-value return through a hidden slot hides behind a name claiming otherwise. None of
// the three has a hidden slot, and the two that take no argument both end in a bare `ret`.
//
// The member offsets below are not guesses either: the disassembly names them, and the generated
// header agrees. TexEnv is at 0x00C, DominantColor at 0x010, NextLOD at 0x02C.

// GetColor is NOT here. Its body is four bytes - `mov eax, [ecx+0x10] / ret` - but the extent
// recorded in data/symbols.json is 128 bytes and .data points at 0x0045D190, inside it. Build gate
// 6 refuses the port because stripping that extent would delete code still referenced, which is the
// right call. Narrowing the recorded extent is what unblocks it.

// ?SetTexEnv@gfxTexture@@QAEXI@Z
//
//     004AE1A0  push ebp / mov ebp, esp
//     004AE1A3  mov eax, dword ptr [ebp + 8]
//     004AE1A6  mov dword ptr [ecx + 0xc], eax
//     004AE1A9  pop ebp / ret 4
//
// `ret 4` for one four-byte argument, which is what __thiscall with a single u32 wants - so the
// name and the code agree.
void gfxTexture::SetTexEnv(u32 texEnv)
{
    TexEnv = texEnv;
}

// ?MarkFirstUse@gfxTexture@@AAEXXZ
//
//     004ACA10  mov eax, dword ptr [0x684d3c]      ; sm_FirstActive
//     004ACA15  mov dword ptr [ecx + 0x2c], eax    ; this->NextLOD = sm_FirstActive
//     004ACA18  mov dword ptr [0x684d3c], ecx      ; sm_FirstActive = this
//     004ACA1E  jmp 0x4aca30                       ; MarkHigherUse
//
// The last instruction is a TAIL CALL, not a fallthrough - MarkHigherUse runs with this same `this`
// and its return becomes this function's. Written as an ordinary call here, which is the same thing
// for a void function and does not pretend the stack trick matters.
//
// Pushing onto the front of the list means the most recently used texture is the head, which is the
// order the LOD walk downstream expects; keeping that order is the whole point of the function.
void gfxTexture::MarkFirstUse()
{
    NextLOD = sm_FirstActive;
    sm_FirstActive = this;

    MarkHigherUse();
}

// THE MIPMAP-ENVIRONMENT GROUP, WHICH IS EMPTY IN RETAIL.
//
// The eight bodies below are not unfinished. Every one of them is a bare `ret` in midtown2.exe -
// the feature was compiled out before the game shipped, and the entry points were left behind so
// the call sites would still link. Reimplementing them as no-ops is therefore an exact port, and
// writing anything more useful would be a behaviour change, not a fix.
//
// The two Get forms are the ones to be careful about: they take references and DO NOT WRITE THEM,
// so a caller reads whatever was in its own locals. That is retail's behaviour and it is preserved
// deliberately - "helpfully" zeroing an out-parameter here would be the same mistake that
// agiGLStubGetLight had to be corrected for.
//
// Verified individually: SetMIPMapEnv and GetMIPMapEnv are `ret 8`, matching the four bytes each
// of their two arguments; MakeUnresident is a bare `ret`; the four statics are __cdecl, so the
// caller cleans and a bare `ret` is correct whatever the parameter list says.

// ?SetDefaultMIPMapEnv@gfxTexture@@SAXIM@Z
void gfxTexture::SetDefaultMIPMapEnv(u32 /*arg1*/, f32 /*arg2*/)
{}

// ?GetDefaultMIPMapEnv@gfxTexture@@SAXAAIAAM@Z
void gfxTexture::GetDefaultMIPMapEnv(u32& /*unk1*/, f32& /*unk2*/)
{}

// ?SetMIPMapEnv@gfxTexture@@QAEXIM@Z
void gfxTexture::SetMIPMapEnv(u32 /*arg1*/, f32 /*arg2*/)
{}

// ?GetMIPMapEnv@gfxTexture@@QAEXAAIAAM@Z
void gfxTexture::GetMIPMapEnv(u32& /*arg1*/, f32& /*arg2*/)
{}

// ?CalcMIPMapEnv@gfxTexture@@SAXHHAAG0@Z
void gfxTexture::CalcMIPMapEnv(i32 /*arg1*/, i32 /*arg2*/, u16& /*arg3*/, u16& /*arg4*/)
{}

// ?EnableTrilinearMipMaping@gfxTexture@@SAX_N@Z
void gfxTexture::EnableTrilinearMipMaping(bool /*arg1*/)
{}

// ?MakeUnresident@gfxTexture@@QAEXXZ
void gfxTexture::MakeUnresident()
{}

// ?CreateFromTexture@gfxTexture@@QAEPAV1@PBDHHHH@Z
//
// `xor eax, eax / ret 0x14` - the one member of the group that is not quite empty, because it has
// a return value to supply. Twenty bytes of arguments, which is the pointer plus four ints, so the
// name and the code agree.
gfxTexture* gfxTexture::CreateFromTexture(
    const char* /*arg1*/, i32 /*arg2*/, i32 /*arg3*/, i32 /*arg4*/, i32 /*arg5*/)
{
    return nullptr;
}
