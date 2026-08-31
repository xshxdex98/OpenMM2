/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2
    Copyright (C) 2026 The OpenMM2 contributors

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

// The few CRT helpers a modern compiler emits that the game's own 1999 CRT does not provide.
//
// midtown2.exe statically linked its CRT, and every routine of it is already in game.asm - which
// is why the link runs with -NODEFAULTLIB. Adding a modern CRT to satisfy a couple of symbols
// would put two CRTs in one image, with two heaps and two sets of stdio state, and the failures
// from that are neither obvious nor local.
//
// So supply only what is actually missing. Both of these exist because of how the compiler lowers
// floating point under /arch:IA32, not because the game needs them: the 1999 build emitted `fsqrt`
// inline where MSVC now calls a helper.

extern "C" {

// Set by the compiler in any translation unit that uses floating point; the CRT normally defines
// it. The value is the conventional one and is never read - only its presence matters.
int _fltused = 0x9875;

// x87 sqrt helper. The argument arrives on the FPU stack in ST(0) and the result is returned
// there, so the whole function is one instruction. Written naked because any prologue would
// disturb the FPU stack the caller set up.
__declspec(naked) void __cdecl _CIsqrt()
{
    __asm
    {
        fsqrt
        ret
    }
}

// sin and cos, with the argument reduction the bare instruction does not do.
//
// `fsin` and `fcos` are only defined for |x| < 2^63. Outside that range they set C2 in the status
// word and leave the operand untouched - so a naive one-instruction shim would silently return
// the ANGLE instead of its sine, with no error anywhere. The angles here are Euler angles and
// almost certainly in range, but "almost certainly" is not a reason to leave a silent wrong
// answer in the arithmetic underneath the physics.
//
// The reduction is the standard one: push 2*pi, then `fprem1` repeatedly until it reports the
// remainder is exact (C2 clear), and retry.

__declspec(naked) void __cdecl _CIsin()
{
    __asm
    {
        fsin
        fnstsw  ax
        test    ah, 4          ; C2 - argument was out of range, nothing was computed
        jz      done
        ; falls through here when C2 was set: reduce and retry
        fldpi
        fadd    st(0), st(0)   ; 2*pi
        fxch    st(1)
    partial:
        fprem1                 ; IEEE remainder, may need several passes
        fnstsw  ax
        test    ah, 4
        jnz     partial
        fstp    st(1)          ; drop the 2*pi
        fsin
    done:
        ret
    }
}

__declspec(naked) void __cdecl _CIcos()
{
    __asm
    {
        fcos
        fnstsw  ax
        test    ah, 4
        jz      done
        ; falls through here when C2 was set: reduce and retry
        fldpi
        fadd    st(0), st(0)
        fxch    st(1)
    partial:
        fprem1
        fnstsw  ax
        test    ah, 4
        jnz     partial
        fstp    st(1)
        fcos
    done:
        ret
    }
}

// atan2. `fpatan` computes arctan(ST(1) / ST(0)), pops both and pushes the result - which is
// exactly this function's contract, with y in ST(1) and x in ST(0). It handles the quadrants and
// the x == 0 cases itself, so there is nothing to add.
__declspec(naked) void __cdecl _CIatan2()
{
    __asm
    {
        fpatan
        ret
    }
}

}

// Float-to-integer conversion, as MSVC emits it.
//
// A plain `(int)` cast on a float compiles to a call to this helper rather than to an instruction,
// because C requires truncation toward zero while the x87 rounding mode is round-to-nearest. The
// helper saves the control word, forces truncation, converts, and restores - which is exactly why
// the compiler does not inline it.
//
// It returns a 64-bit result in EDX:EAX and takes its argument in ST(0), popping it. Naked, for
// the same reason as _CIsqrt: any prologue would disturb the FPU stack the caller set up.
//
// Declared _ftol2_sse: __cdecl prepends an underscore, so the symbol the linker
// resolves is __ftol2_sse - the name that appears in the error.
//
// The name says SSE, but the calling convention is the x87 one. MSVC picks this variant when it
// knows SSE is available and simply expects the same contract; under /arch:IA32 the x87 form is
// what the rest of this binary uses, and matching the original's rounding is the whole point of
// building that way.
extern "C" __declspec(naked) void __cdecl _ftol2_sse()
{
    __asm
    {
        push    ebp
        mov     ebp, esp
        sub     esp, 12

        fnstcw  word ptr [ebp - 2]          // save the caller's rounding mode
        movzx   eax, word ptr [ebp - 2]
        or      eax, 0x0C00                 // RC = 11, truncate toward zero
        mov     word ptr [ebp - 4], ax
        fldcw   word ptr [ebp - 4]

        fistp   qword ptr [ebp - 12]        // convert and pop
        fldcw   word ptr [ebp - 2]          // restore it, whatever it was

        mov     eax, dword ptr [ebp - 12]
        mov     edx, dword ptr [ebp - 8]

        mov     esp, ebp
        pop     ebp
        ret
    }
}
