/*
    OpenMM2 - a reimplementation of Midtown Madness 2

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

// Dead weight, to answer one question: does the SIZE of the reimplementation matter?
//
// Registering 28 Matrix33/Matrix66 functions makes the game die at profile load. But an exhaustive
// scan proved those functions are unreachable - nothing in midtown2.exe outside those two classes
// calls or references them - so a reimplementation of them cannot crash anything by running.
//
// What porting them *does* do is add 3,232 bytes to .text, and .text had only 593 bytes of headroom
// before crossing 0x1B8000. Crossing it pushes .rdata from 0x1B9000 to 0x1BA000 and .data with it.
// That is the only other thing that changed.
//
// This file separates the two. It contributes roughly the same number of bytes of .text and calls
// nothing, so building with ARTS_LAYOUT_PAD and WITHOUT the matrix ports reproduces the section
// layout without reproducing the port. If the crash follows the layout, the ports are innocent and
// the harness has a size-dependent defect that the next few hundred bytes of any port would have
// found instead.
//
// Delete this file once the question is settled. It is an experiment, not infrastructure.

#include "core/arts.h"

#ifdef ARTS_LAYOUT_PAD

// Referenced through a volatile pointer so the linker cannot decide it is unreachable and discard
// it, which would defeat the whole purpose.
extern "C" void arts_layout_pad();

namespace
{
    volatile u32 pad_sink = 0;
}

void (*const arts_layout_pad_keep)() = arts_layout_pad;

// Each step is a read-modify-write through a volatile, so none of it folds away under -O2 and the
// byte count stays roughly proportional to the number of steps.
#define PAD_STEP(n) pad_sink = pad_sink * 1664525u + (n);
#define PAD_10(b)                                                                                  \
    PAD_STEP(b + 0) PAD_STEP(b + 1) PAD_STEP(b + 2) PAD_STEP(b + 3) PAD_STEP(b + 4)                \
    PAD_STEP(b + 5) PAD_STEP(b + 6) PAD_STEP(b + 7) PAD_STEP(b + 8) PAD_STEP(b + 9)
#define PAD_100(b)                                                                                 \
    PAD_10(b + 0) PAD_10(b + 10) PAD_10(b + 20) PAD_10(b + 30) PAD_10(b + 40)                      \
    PAD_10(b + 50) PAD_10(b + 60) PAD_10(b + 70) PAD_10(b + 80) PAD_10(b + 90)

extern "C" void arts_layout_pad()
{
    PAD_100(0)
    PAD_100(100)
    PAD_100(200)
    PAD_100(300)
}

#endif
