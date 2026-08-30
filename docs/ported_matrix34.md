# Matrix34

`Matrix34` is the engine's 3x4 affine transform: a 3x3 rotation/scale block followed by a
translation row, twelve floats at offset 0, `sizeof == 0x30`, no virtuals. Every camera, every
vehicle instance, every physics body carries one, so it sits underneath most of the rest of the
binary — and unlike almost everything else at that depth it has no vtable, so nothing about it
depends on the class hierarchy being resolved first.

The map lists **82 methods, 16,528 bytes**. **72 are reimplemented here, 11,680 bytes — 70.7% of
the class by size.** The remaining ten are listed below with the reason each was left alone.

Sources, in the order they were trusted:

1. `data/symbols.json` — name, address, size and full mangled signature, from the retail linker
   map. Every function here was matched to the map **by address**, not by the label the RE kit
   attached to it.
2. `MM2_RE_KIT/MM2_PSEUDOCODE/Matrix34/Matrix34.c` — the decompiled bodies.
3. `Open1560/code/midtown/vector7/matrix34.{h,cpp}` — Midtown Madness 1's working C++ for the same
   class, used only to check the reading. Where the two disagree, MM2's binary won; the
   disagreements are recorded at the end, because they are the interesting part.

The RE kit's file claims 83 functions. Three of them — `MM2::Matrix34::Dot` at `0x406780`,
`MM2::Matrix34::Normalize` at `0x5A6090` and `MM2::Matrix34::Rotate` at `0x5A6460` — are at
addresses that hold no `Matrix34` symbol in the map. They are part of the foreign symbol set the
kit warns about and were discarded. Conversely, two real symbols (`Print` and `Transpose(void)`)
have **no body in the dump at all**.

## The calling-convention question

Hex-Rays guessed `__fastcall` on most of these and emitted `(this, int a2, <real args>)`, where
`a2` is the unused `edx` slot of a `__thiscall`. That reading is confirmed rather than assumed:

```c
void __thiscall Matrix34::Rotate(float *this, Matrix34 *a1, float a2)
{
    Matrix34 v4;
    Matrix34::MakeRotate(&v4, a1, a2);
    Matrix34::Dot3x3(this, (int)&v4, &v4.m00);   // <- &v4 in BOTH slots
}
```

The same local appears in the dummy slot and in the real argument slot, which only happens if the
dummy is register junk the decompiler invented. Every function ported here has that dummy dropped,
and the remaining arity then matches its mangled signature exactly. Where it did not match — where
a genuine argument was missing or a real one was mismapped — the function was rejected.

The mirror-image artefact also shows up: several bodies are typed `float` or `float *` and end with
`return v4;` or `return result;`. The mangled names all say `void` (`X`), so those returns are
`eax`/`st0` residue and were dropped.

## Float fidelity

The 1999 build is x87 at the CRT's default 53-bit precision control, so an intermediate is held
wider than `f32` and rounds only when it is stored. The dump distinguishes the two cases and so
does this port: a value the decompiler placed in a stack slot (`float v12; // [esp+0h]`) is written
as `f32`; one it left in an `st(n)` register (`double v9; // st3`) is written as `f64`, forced with
`f64 {x} * y` so the multiply cannot collapse to single precision.

That distinction is not cosmetic here. `FromEulersXYZ` computes `sz * sy` once and uses it twice:

```c
v9 = v13 * v10;      // double, st3
v12 = v9;            // float spill, [esp+0h]
a1[4] = v9 * v14 + v6 * v4;      // m11 <- the register copy
a1[7] = v12 * v4 - v6 * v14;     // m21 <- the rounded copy
```

`m11` and `m21` therefore see different values of the same product. `FromEulersXZY`,
`FromEulersYXZ` and `FromEulersZXY` all do the same thing with a different pair. Sub-expression
grouping and accumulation order are reproduced verbatim throughout for the same reason.

Trig is called with the float promoted to double, because that is what the original does — it
pushes a dword onto the x87 stack and calls the double-precision helper. `std::cos(f32)` would
select `cosf` and change the result.

## A build dependency this introduces

`midtown2.map` has `__CIasin` and `__CIacos` but **no `__CIcos`, `__CIsin`, `__CIatan2` or
`__CIsqrt`** — the 1999 compiler emitted those inline as `fcos`/`fsin`/`fpatan`/`fsqrt` and only
called out for the inverse functions. A modern `/arch:IA32` build calls the helper for all of them,
so the twenty-odd functions here that use `cos`, `sin` or `atan2` will need `__CIcos`, `__CIsin`
and `__CIatan2` in `code/midtown2/core/crtshim.cpp`, exactly as `_CIsqrt` is already supplied
there. This was not written speculatively: a naive `fcos; ret` is wrong outside |x| < 2^63, and the
correct shim needs the `fprem1` reduction loop, which is not something to guess at untested.

`_memset` is likewise absent from the map. The `RotateFull*` family's 12-byte `memset` is written
as three zero-float stores instead, which is byte-identical in effect and needs no CRT symbol.

## Ported (72 methods, 11,680 bytes)

| Method | Symbol | Bytes |
|---|---|--:|
| `Add(class Matrix34 const &)` | `?Add@Matrix34@@QAEXABV1@@Z` | 128 |
| `Add(class Matrix34 const &,class Matrix34 const &)` | `?Add@Matrix34@@QAEXABV1@0@Z` | 128 |
| `Add3x3(class Matrix34 const &)` | `?Add3x3@Matrix34@@QAEXABV1@@Z` | 96 |
| `Add3x3(class Matrix34 const &,class Matrix34 const &)` | `?Add3x3@Matrix34@@QAEXABV1@0@Z` | 96 |
| `AddScaled(class Matrix34 const &,float)` | `?AddScaled@Matrix34@@QAEXABV1@M@Z` | 128 |
| `Determinant3x3(void) const` | `?Determinant3x3@Matrix34@@QBEMXZ` | 112 |
| `Dot(class Matrix34 const &)` | `?Dot@Matrix34@@QAEXABV1@@Z` | 384 |
| `Dot(class Matrix34 const &,class Matrix34 const &)` | `?Dot@Matrix34@@QAEXABV1@0@Z` | 320 |
| `Dot3x3(class Matrix34 const &)` | `?Dot3x3@Matrix34@@QAEXABV1@@Z` | 288 |
| `Dot3x3(class Matrix34 const &,class Matrix34 const &)` | `?Dot3x3@Matrix34@@QAEXABV1@0@Z` | 240 |
| `Dot3x3CrossProdMtx(class Vector3 const &)` | `?Dot3x3CrossProdMtx@Matrix34@@QAEXABVVector3@@@Z` | 176 |
| `Dot3x3CrossProdTranspose(class Vector3 const &)` | `?Dot3x3CrossProdTranspose@Matrix34@@QAEXABVVector3@@@Z` | 176 |
| `Dot3x3Transpose(class Matrix34 const &)` | `?Dot3x3Transpose@Matrix34@@QAEXABV1@@Z` | 256 |
| `Dot3x3Transpose(class Matrix34 const &,class Matrix34 const &)` | `?Dot3x3Transpose@Matrix34@@QAEXABV1@0@Z` | 240 |
| `DotTranspose(class Matrix34 const &)` | `?DotTranspose@Matrix34@@QAEXABV1@@Z` | 368 |
| `DotTranspose(class Matrix34 const &,class Matrix34 const &)` | `?DotTranspose@Matrix34@@QAEXABV1@0@Z` | 320 |
| `FastInverse(class Matrix34 const &)` | `?FastInverse@Matrix34@@QAEXABV1@@Z` | 176 |
| `FastInverse(void)` | `?FastInverse@Matrix34@@QAEXXZ` | 48 |
| `FromEulers(class Vector3 const &,char *)` | `?FromEulers@Matrix34@@QAEXABVVector3@@PAD@Z` | 256 |
| `FromEulersXYZ(class Vector3 const &)` | `?FromEulersXYZ@Matrix34@@QAEXABVVector3@@@Z` | 272 |
| `FromEulersXZY(class Vector3 const &)` | `?FromEulersXZY@Matrix34@@QAEXABVVector3@@@Z` | 272 |
| `FromEulersYXZ(class Vector3 const &)` | `?FromEulersYXZ@Matrix34@@QAEXABVVector3@@@Z` | 288 |
| `FromEulersYZX(class Vector3 const &)` | `?FromEulersYZX@Matrix34@@QAEXABVVector3@@@Z` | 272 |
| `FromEulersZXY(class Vector3 const &)` | `?FromEulersZXY@Matrix34@@QAEXABVVector3@@@Z` | 272 |
| `FromEulersZYX(class Vector3 const &)` | `?FromEulersZYX@Matrix34@@QAEXABVVector3@@@Z` | 272 |
| `GetEulers(char const *) const` | `?GetEulers@Matrix34@@QBE?AVVector3@@PBD@Z` | 576 |
| `GetEulers(void) const` | `?GetEulers@Matrix34@@QBE?AVVector3@@XZ` | 80 |
| `GetLookAt(class Vector3 *,class Vector3 *,float) const` | `?GetLookAt@Matrix34@@QBEXPAVVector3@@0M@Z` | 80 |
| `GetPolar(class Vector4 *,class Vector3 *,float) const` | `?GetPolar@Matrix34@@QBEXPAVVector4@@PAVVector3@@M@Z` | 64 |
| `Identity(void)` | `?Identity@Matrix34@@QAEXXZ` | 48 |
| `Identity3x3(void)` | `?Identity3x3@Matrix34@@QAEXXZ` | 48 |
| `Inverse(class Matrix34 const &)` | `?Inverse@Matrix34@@QAEXABV1@@Z` | 32 |
| `Inverse(void)` | `?Inverse@Matrix34@@QAEXXZ` | 368 |
| `LookAt(class Vector3 const &,class Vector3 const &)` | `?LookAt@Matrix34@@QAEXABVVector3@@0@Z` | 304 |
| `MakeRotateUnitAxis(class Vector3 const &,float)` | `?MakeRotateUnitAxis@Matrix34@@QAEXABVVector3@@M@Z` | 208 |
| `MakeRotateX(float)` | `?MakeRotateX@Matrix34@@QAEXM@Z` | 64 |
| `MakeRotateY(float)` | `?MakeRotateY@Matrix34@@QAEXM@Z` | 64 |
| `MakeRotateZ(float)` | `?MakeRotateZ@Matrix34@@QAEXM@Z` | 64 |
| `MakeScale(float)` | `?MakeScale@Matrix34@@QAEXM@Z` | 48 |
| `MakeScale(float,float,float)` | `?MakeScale@Matrix34@@QAEXMMM@Z` | 48 |
| `Normalize(void)` | `?Normalize@Matrix34@@QAEXXZ` | 384 |
| `PolarView(float,float,float,float)` | `?PolarView@Matrix34@@QAEXMMMM@Z` | 80 |
| `Rotate(class Vector3 const &,float)` | `?Rotate@Matrix34@@QAEXABVVector3@@M@Z` | 48 |
| `RotateFull(class Vector3 const &,float)` | `?RotateFull@Matrix34@@QAEXABVVector3@@M@Z` | 64 |
| `RotateFullUnitAxis(class Vector3 const &,float)` | `?RotateFullUnitAxis@Matrix34@@QAEXABVVector3@@M@Z` | 64 |
| `RotateFullX(float)` | `?RotateFullX@Matrix34@@QAEXM@Z` | 64 |
| `RotateFullY(float)` | `?RotateFullY@Matrix34@@QAEXM@Z` | 64 |
| `RotateFullZ(float)` | `?RotateFullZ@Matrix34@@QAEXM@Z` | 64 |
| `RotateUnitAxis(class Vector3 const &,float)` | `?RotateUnitAxis@Matrix34@@QAEXABVVector3@@M@Z` | 48 |
| `RotateX(float)` | `?RotateX@Matrix34@@QAEXM@Z` | 48 |
| `RotateY(float)` | `?RotateY@Matrix34@@QAEXM@Z` | 48 |
| `RotateZ(float)` | `?RotateZ@Matrix34@@QAEXM@Z` | 48 |
| `Scale(float)` | `?Scale@Matrix34@@QAEXM@Z` | 96 |
| `Scale(float,float,float)` | `?Scale@Matrix34@@QAEXMMM@Z` | 96 |
| `ScaleFull(float)` | `?ScaleFull@Matrix34@@QAEXM@Z` | 128 |
| `ScaleFull(float,float,float)` | `?ScaleFull@Matrix34@@QAEXMMM@Z` | 128 |
| `Set(class Matrix34 const &)` | `?Set@Matrix34@@QAEXABV1@@Z` | 80 |
| `Subtract(class Matrix34 const &)` | `?Subtract@Matrix34@@QAEXABV1@@Z` | 128 |
| `Subtract(class Matrix34 const &,class Matrix34 const &)` | `?Subtract@Matrix34@@QAEXABV1@0@Z` | 128 |
| `Subtract3x3(class Matrix34 const &)` | `?Subtract3x3@Matrix34@@QAEXABV1@@Z` | 96 |
| `Subtract3x3(class Matrix34 const &,class Matrix34 const &)` | `?Subtract3x3@Matrix34@@QAEXABV1@0@Z` | 96 |
| `ToEulers(class Vector3 &,char *) const` | `?ToEulers@Matrix34@@QBEXAAVVector3@@PAD@Z` | 256 |
| `ToEulersXYZ(class Vector3 &) const` | `?ToEulersXYZ@Matrix34@@QBEXAAVVector3@@@Z` | 208 |
| `ToEulersXZY(class Vector3 &) const` | `?ToEulersXZY@Matrix34@@QBEXAAVVector3@@@Z` | 192 |
| `ToEulersYXZ(class Vector3 &) const` | `?ToEulersYXZ@Matrix34@@QBEXAAVVector3@@@Z` | 208 |
| `ToEulersYZX(class Vector3 &) const` | `?ToEulersYZX@Matrix34@@QBEXAAVVector3@@@Z` | 192 |
| `ToEulersZXY(class Vector3 &) const` | `?ToEulersZXY@Matrix34@@QBEXAAVVector3@@@Z` | 208 |
| `ToEulersZYX(class Vector3 &) const` | `?ToEulersZYX@Matrix34@@QBEXAAVVector3@@@Z` | 208 |
| `Transform(class Vector3 const &,class Vector3 &) const` | `?Transform@Matrix34@@QBEXABVVector3@@AAV2@@Z` | 256 |
| `Transpose(class Matrix34 const &)` | `?Transpose@Matrix34@@QAEXABV1@@Z` | 64 |
| `Transpose3x4(class Matrix34 const &)` | `?Transpose3x4@Matrix34@@QAEXABV1@@Z` | 160 |
| `Zero(void)` | `?Zero@Matrix34@@QAEXXZ` | 48 |

## Rejected (10 methods, 4,848 bytes)

### `Print(char const *) const` — `?Print@Matrix34@@QBEXPBD@Z`, 160 bytes, `0x4BF150`

**No body in the RE kit at all.** The symbol is in the map with a size; the pseudocode file skips
it. Nothing to transcribe from.

### `Transpose(void)` — `?Transpose@Matrix34@@QAEXXZ`, 48 bytes, `0x4BE990`

Same: absent from the dump. 48 bytes is about three in-place swaps, and it would be easy to guess
what those are — but not which order the stores happen in, and store order is exactly what decides
what an in-place transpose does. Guessing it produces code that looks right and is not.

### `Transpose3x4(void)` — `?Transpose3x4@Matrix34@@QAEXXZ`, 128 bytes, `0x4BEA00`

The body is present but begins with a call the kit labels `time(x_4)` on an uninitialised local —
one of the ~349 foreign labels. Everything about it says the call is really to `Transpose(void)`
above: it takes no set-up arguments (so it is an `ecx`-based `__thiscall`), it sits immediately
after `Transpose(void)` in the address space, and the arithmetic that follows only makes sense on
an already-transposed 3x3 (it reads `a1[3]` and `a1[6]` where the untransposed values would be
`a1[1]` and `a1[2]`). That is a strong inference and probably correct, but it is an inference about
a call target and about the contents of a function that was never decompiled. Left alone.

### `MakeRotate(class Vector3 const &,float)` — `?MakeRotate@Matrix34@@QAEXABVVector3@@M@Z`, 352 bytes, `0x4BCFA0`

This is the axis dispatcher — cardinal axes go to `MakeRotateX/Y/Z`, everything else normalises and
goes to `MakeRotateUnitAxis`. Its branch was reconstructed out of x87 status-word bits, and the
result contradicts itself:

```c
char v4;              // fps^1   (never assigned)
m01 = a1->m01;
v6 = v4;
v7 = m01 == 0.0;      // c3
if ( (v6 & 0x40) != 0 )     // c3 again -> m01 == 0.0
{
    m02 = a1->m02;
    if ( v7 )               // the same condition, so always true here
        ...
    if ( m02 == 0.0 )       // consequently unreachable
        ...
}
```

`(v6 & 0x40) != 0` and `v7` are the same C3 flag, which makes the `axis.y == 0` sub-branch dead —
and it obviously is not dead in the original, because it is the path that reaches `MakeRotateY`.
There are at least two `fnstsw`/`sahf` pairs here that Hex-Rays collapsed into one. Recovering the
real condition needs the disassembly, and the binary is not present in this tree.

### `RotateTo(class Vector3 const &,class Vector3 const &)` — `?RotateTo@Matrix34@@QAEXABVVector3@@0@Z`, 240 bytes, `0x4BD290`
### `RotateTo(class Vector3 const &,class Vector3 const &,float)` — `?RotateTo@Matrix34@@QAEXABVVector3@@0M@Z`, 256 bytes, `0x4BD380`

Both end the same way:

```c
v9  = v8 * v9;    // [ebp-Ch]  normalised cross x
v10 = v8 * v10;   // [ebp-8h]  normalised cross y
v11 = v8 * v6;    // [ebp-4h]  normalised cross z
Matrix34::RotateUnitAxis(this, (int)a3, (int)&v9, v11);
```

`&v9` is the three-float axis at `[ebp-0Ch]`, and the angle argument is `v11` — the axis's own z
component, read from the last slot of that same vector. That cannot be the rotation angle, and
there is no other candidate anywhere in the body: no `asin`, no `acos`, no magnitude kept
unnormalised. Hex-Rays matched the pushed angle to the wrong stack slot and the real source of the
value is not visible. Everything else in these two functions reads cleanly, which makes it more
tempting to guess and no more defensible.

### `FromQuaternion(class Quaternion const &)` — `?FromQuaternion@Matrix34@@QAEXABVQuaternion@@@Z`, 192 bytes, `0x4BDE80`

The body is clean and readable — it scales all four components by `1.4142135f` (float
`0x3FB504F3`) and builds the rotation from the products, with `a3[3]` as `w`. The blocker is
`Quaternion`: `code/midtown2/vector7/quaternion.h` has **no members**, because the IDB type library
carries no layout for it (`check_size` is commented out with a TODO). There is no way to write
`arg1.x` and no way to justify reading floats through a class of size 1. This one becomes free the
moment `Quaternion`'s layout lands.

### `Interpolate(class Matrix34 const &,class Matrix34 const &,float)` — `?Interpolate@Matrix34@@QAEXABV1@0M@Z`, 208 bytes, `0x4BF080`

Same blocker, harder: it builds two quaternions from `Quaternion::FromMatrix`, dot-products them
to pick the shorter arc, conditionally negates all four components, slerps, and calls
`FromQuaternion`. All of that needs `Quaternion`'s members.

### `SolveSVD(class Vector3 const &) const` — `?SolveSVD@Matrix34@@QBE?AVVector3@@ABV2@@Z`, 2,928 bytes, `0x4BF1F0`

The single biggest function in the class, and the most garbled. Ninety-odd locals, most of them
aliased onto the same stack slots under decompiler-invented names (`da`/`db`/`d`/`dc` are all
`[ebp-28h]`; `v73` through `v78` are all `[ebp-30h]`), plus x87 status-word booleans (`bool v20;
// c0`, `bool v21; // c3`) driving the iteration's convergence test. Midtown Madness 1 has a
`SolveSVD` and Open1560 implements it, so there is a reference — which is precisely why it should
not be transcribed from this dump: it would end up being MM1's algorithm with MM2's symbol on it,
and no way to tell where the two diverge.

### `Transform4(class Vector3 const *,class Vector4 *,int) const` — `?Transform4@Matrix34@@QBEXPBVVector3@@PAVVector4@@H@Z`, 336 bytes, `0x4BFD60`

A batch transform whose loop induction came out as nonsense — the count argument is aliased with
the output pointer (`v6 = (char *)a5 - 1; if ((int)a5 - 1 >= 0)`), `result` is used simultaneously
as the return value and as the walking input cursor, and the strides (`v7 -= 16`, `result -= 3`)
are folded against base addresses that are not distinguishable from each other in the output. The
arithmetic inside the loop is recognisable as `Transform` plus a zeroed `w`; the loop around it is
not recoverable.

## Where Midtown Madness 1 and Midtown Madness 2 disagree

Open1560's `Matrix34` was used only as a check. It disagrees with MM2's binary in eight places, and
in every one of them MM2's binary was followed.

**1. MM2's Matrix34 does not use `MathSpeed`.** This is the big one. MM1's `matrix34.cpp` routes
every trig call through a speed dial — `MathSpeed == 0` is libm, `1` is a linearly-interpolated
1024-entry cosine table, `2` is the nearest table entry — and has `rot_cos_sin`, `acos_fast`,
`atan2_fast`, `asin_fast` and `invsqrtf_fast` wrappers to do it. **There is no trace of that in
MM2's Matrix34.** Every one of the six `FromEulers*`, the three `MakeRotate*` and all six
`ToEulers*` calls the double-precision library function directly, with no global test in front of
it. The zero-angle short circuit and the degenerate-atan2 guard survived; the table lookup did not.

**2. MM2's class is far larger.** MM1 has 33 methods. MM2 has 82, and about thirty of them have no
MM1 counterpart at all: the whole `Add`/`Subtract`/`Add3x3`/`Subtract3x3`/`AddScaled` family,
`DotTranspose`, `Dot3x3Transpose`, `Dot3x3CrossProdMtx`, `Dot3x3CrossProdTranspose`,
`Transpose3x4`, `Determinant3x3`, `Zero`, `Set`, `Identity3x3`, `Transform`, `Transform4`,
`Interpolate`, and the `RotateFull*`/`ScaleFull` families. That is the shape of a class that grew
to serve a physics engine.

**3. `AddScaled` is 3x3 only.** Its name has no `3x3` in it and `Add`/`Subtract` next to it do all
twelve elements, but `AddScaled` touches only the rotation block. The instruction budget confirms
it independently: `Add` and `AddScaled` are both `0x73` bytes, which is twelve 9-byte
load/add/store groups in one case and nine 12-byte load/mul/add/store groups in the other.

**4. `Transpose(const Matrix34&)` drops the translation row.** MM1's ends with `m3 = src.m3`. MM2's
writes nine floats and stops, leaving the destination's translation row exactly as it found it. The
64-byte size leaves no room for the fourth row.

**5. `FastInverse(const Matrix34&)` is not aliasing-safe.** MM1 caches all nine elements in locals
first and comments that this makes `src` aliasing `*this` safe. MM2 interleaves reads and writes —
`m10` is written from `arg1.m01` before `arg1.m10` is read — so `a.FastInverse(a)` corrupts. That
is why `FastInverse(void)` exists and why it copies to a stack temporary first. It also builds the
translation through two float spills (`v11 = m00*m30`, then `v12 = m01*m31 + v11`) where MM1 uses
one `f32` expression, so the two round at different points.

**6. `LookAt` does not renormalise the up axis.** MM1 does `up.Cross(m2, m0); up = up * up.InvMag()`.
MM2 computes the cross and stores it, with no third `InvMag`. Mathematically redundant once `m0`
and `m2` are orthonormal, so this is MM2 removing work rather than dropping a step. MM2 also writes
the translation row last rather than first, and it computes the cross product with the literal zero
still in the expression (`v13 = 0.0f * m20`) rather than folding those terms away — which is
preserved here, because `0 * inf` is not `0`.

**7. `PolarView` is built differently.** MM1: `Identity(); m3.z = offz + m3.z; RotateFull(ZAXIS,
-rotz); RotateFull(XAXIS, -rotx); RotateFull(YAXIS, roty);` — five calls. MM2 assembles the Euler
vector `(-rotx, roty, rotz)` on the stack, calls `FromEulersZXY` once, and sets the translation to
`offz * m2` directly. Same intent, one seventh of the work, and different rounding. The parameter
order `(offz, roty, rotx, rotz)` is identical, which is what made the mapping legible in the first
place.

**8. `Scale`, `ScaleFull` and `MakeScale` mean three different things, and none of them is MM1's.**
MM1 has `Scale(f32)` delegating to `Scale(f32,f32,f32)`, plus `ScaleAbs` (replace the 3x3 with a
pure scale) and `ScaleFullAbs` (that, and zero the translation). MM2 has `Scale` (multiply the 3x3),
`ScaleFull` (multiply all twelve, translation included) and `MakeScale` (replace the 3x3, leaving
the translation alone) — so MM2's `MakeScale` is MM1's `ScaleAbs`, MM2's `ScaleFull` is a
multiply where MM1's `ScaleFullAbs` is an assignment, and MM2's `Scale(f32)` does not delegate: it
is its own loop, and it walks the matrix in row order while `Scale(f32,f32,f32)` walks it in
column order.

Two smaller things worth recording. MM2's `RotateTo` uses a cosine cutoff of `0.99998999f`
(`0x3F7FFF58`) and shouts `"Matrix34::RotateTo()- CHOKE!!!!!!!"` through `Errorf`; MM1 uses
`0.99900001f` (`0x3F7FBE77`) and `"RotateToAbs - Bad inputs"`, and additionally warns on a
zero-magnitude input, which MM2 does not check for. And every one of MM2's six `ToEulers*` clamps
its `asin` argument to that same `+/-0.99998999` rather than `+/-1`, so a perfectly axis-aligned
matrix comes back with a pitch of 89.9744 degrees rather than 90. MM1's `euler_asin` clamps to
`+/-1.0f` and returns exactly `+/-1.5707964f`. That is a real behavioural difference at the
singularity, not a rounding artefact, and it is preserved here.
