# -*- coding: utf-8 -*-
"""Minimal PE section reader, shared by the tools that touch midtown2.exe."""
import struct
import sys

IMAGE_BASE = 0x400000


def load_sections(path):
    """(data, sections) where each section is (vaddr, vend, rawptr)."""
    with open(path, "rb") as f:
        data = f.read()

    pe_off = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe_off:pe_off + 4] != b"PE\0\0":
        sys.exit("not a PE file: %s" % path)

    num_sections = struct.unpack_from("<H", data, pe_off + 6)[0]
    opt_size = struct.unpack_from("<H", data, pe_off + 20)[0]
    table = pe_off + 24 + opt_size

    sections = []
    for i in range(num_sections):
        base = table + i * 40
        vsize, vaddr, rawsize, rawptr = struct.unpack_from("<IIII", data, base + 8)
        sections.append((vaddr, vaddr + max(vsize, rawsize), rawptr))

    return data, sections


def to_offset(sections, rva):
    for vaddr, vend, rawptr in sections:
        if vaddr <= rva < vend:
            return rawptr + (rva - vaddr)
    return None


def va_from_section(sections, section, offset):
    """Virtual address from the map's authoritative `section:offset` pair.

    The Rva+Base column of midtown2.map is NOT reliable. For `.rdata` it is 0x308 too high - the
    exact `.rdata` start value printed in the map header - which silently pointed every vftable
    lookup at a neighbouring table. `section:offset` plus the real PE section address is correct
    for every section, so that is what everything here uses.
    """
    idx = section - 1  # the map numbers sections from 1
    if idx < 0 or idx >= len(sections):
        return None

    return IMAGE_BASE + sections[idx][0] + offset
