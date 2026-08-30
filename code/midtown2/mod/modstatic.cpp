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

#include "modstatic.h"

define_dummy_symbol(mod_modstatic);

// The two counting accessors, read off the instruction bytes at 0x004A4DB0 and 0x004A4DE0. Both
// are pure walks of the packet lists and touch nothing else in the engine.
//
// WHY gfx/gfxpacket.h IS NOT INCLUDED HERE. Two independent reasons, and either alone is enough:
//
//  1. It does not compile. Line 38 declares a parameter as `unsigned const i16*`, and `unsigned`
//     cannot be applied to a typedef-name - tools/genheaders.py emits it that way for every
//     `unsigned short const *` in the map. Nothing in the tree has ever included that header;
//     agigl/glpacket.h mirrors the struct for the same reason and says so at length.
//
//  2. Four of its member NAMES are wrong, docs/gfx_layouts.md §16.1. The offsets and the 0x2C size
//     are right - it is only the names - but 0x14 is an ADJUNCT (vertex) count and not a
//     `PacketSize`, and 0x16 is an INDEX count and not a `TriCount`. Both mistakes are exactly
//     what these two functions disprove: GetAdjunctCount sums 0x14 and calls the result adjuncts,
//     and GetTriCount reads 0x16 and divides by three to get triangles.
//
// So the two fields these functions need are mirrored below at their proven offsets, with
// static_asserts that stop the build rather than read the wrong bytes if anything moves.
struct modStaticPacketFields
{
    modStaticPacketFields* Next; // 0x00
    u8 Skipped04[0x10]; // 0x04  VertexBuffer, LockedVerts, Vertices, VertexTypeDesc, StartVertex
    u16 AdjunctCount; // 0x14  vertices, gfxpacket.h calls this PacketSize
    u16 IndexCount; // 0x16  indices, gfxpacket.h and MM2Hook both call this TriCount
    u8 Skipped18[0x14]; // 0x18  Indices, MatrixIndices, AdjunctMatrixGroups, ReskinCount, Reskin
};

check_size(modStaticPacketFields, 0x2C);

static_assert(offsetof(modStaticPacketFields, Next) == 0x00, "gfxPacket::Next must be at 0x00");
static_assert(offsetof(modStaticPacketFields, AdjunctCount) == 0x14, "gfxPacket adjunct count must be at 0x14");
static_assert(offsetof(modStaticPacketFields, IndexCount) == 0x16, "gfxPacket index count must be at 0x16");

// ?GetAdjunctCount@modStatic@@QBEHXZ
i32 modStatic::GetAdjunctCount() const
{
    modStaticPacketFields** packets = reinterpret_cast<modStaticPacketFields**>(ppPackets);

    i32 total = 0;

    // `mov dl, byte ptr [ecx]` then a signed test, so a PacketCount of zero is the only way out
    // early. The inner walk follows Next at 0x00 and adds the zero-extended 16-bit count.
    for (i32 i = 0; i < PacketCount; ++i)
    {
        for (modStaticPacketFields* packet = packets[i]; packet; packet = packet->Next)
            total += packet->AdjunctCount;
    }

    return total;
}

// ?GetTriCount@modStatic@@QBEHXZ
i32 modStatic::GetTriCount() const
{
    modStaticPacketFields** packets = reinterpret_cast<modStaticPacketFields**>(ppPackets);

    i32 total = 0;

    for (i32 i = 0; i < PacketCount; ++i)
    {
        for (modStaticPacketFields* packet = packets[i]; packet; packet = packet->Next)
        {
            // The original is `(n * 21846) >> 16`, built out of seven `lea`s and a `shl`, and that
            // is transcribed rather than written as `n / 3`.
            //
            // The two are NOT the same function. 21846/65536 is a shade under a third, so the
            // magic is exact only while n <= 32767; from 32768 up it is one low, and MSVC would
            // never emit `n / 3` this way for a value it had zero-extended from 16 bits. Writing
            // the division would therefore be a deliberate behaviour change in the one direction
            // this project has already been burned by, for a packet with more than 32767 indices
            // that probably cannot exist. The original's arithmetic wins.
            total += (packet->IndexCount * 21846) >> 16;
        }
    }

    return total;
}
