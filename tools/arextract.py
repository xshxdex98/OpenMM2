# -*- coding: utf-8 -*-
"""List and extract MM2 'DAVE' archives (mm2core.ar, mm2tex.ar, mm2aud.ar, mm2audex.ar).

The format was settled by disassembling ?Init@zipFile@@QAE_NPBD@Z at 0x00573480 rather than by
pattern-matching the bytes, because guessing gets the stride and the base wrong in ways that look
plausible for the first few hundred entries and then drift. What Init actually does:

    seek(0x800); read(Entries, EntryTableSize); read(NamesBuffer, NamesSize)
    for i in range(EntryCount): *(u32*)(Entries + i*0x10) += NamesBuffer

That fixup loop is the whole answer. The table lives at a FIXED offset of 0x800 - the two header
sizes are the lengths of the two blocks, not their positions, which is the trap - the stride is
0x10, and the first u32 of each entry is a byte offset into the name blob that the loader turns
into the `const char* Name` the in-memory zipEntry shows. So the on-disk entry is the in-memory
struct with the pointer still in its unrelocated, offset form.

zipFile::Open picks the codec with `entry->UncompresedSize == entry->CompressedSize` - equal sizes
mean the bytes are stored verbatim; otherwise inflateInit2_(..., -15, ...) means raw deflate with no
zlib header, i.e. zlib.decompressobj(-15). There is no per-entry flag to read.

Names ending in '/' are zero-length directory markers left over from the CVS trees the archives were
built from; they carry no data and are skipped on extract.
"""
import argparse
import os
import struct
import sys
import zlib

MAGIC = b"DAVE"
TABLE_OFFSET = 0x800
ENTRY_SIZE = 0x10


class Entry(object):
    __slots__ = ("name", "offset", "size", "packed_size")

    def __init__(self, name, offset, size, packed_size):
        self.name = name
        self.offset = offset
        self.size = size
        self.packed_size = packed_size

    @property
    def stored(self):
        return self.size == self.packed_size

    @property
    def is_dir(self):
        return self.name.endswith("/")


def read_archive(path):
    """Return (entries, blob). The whole file is held in memory - the largest is ~21 MB."""
    with open(path, "rb") as f:
        blob = f.read()

    magic, count, table_size, names_size = struct.unpack_from("<4sIII", blob, 0)
    if magic != MAGIC:
        raise ValueError("%s: not a DAVE archive (magic %r)" % (path, magic))

    names_at = TABLE_OFFSET + table_size
    names = blob[names_at:names_at + names_size]

    entries = []
    for i in range(count):
        name_off, offset, size, packed = struct.unpack_from(
            "<IIII", blob, TABLE_OFFSET + i * ENTRY_SIZE)
        end = names.index(b"\0", name_off)
        # Paths are stored with forward slashes already; latin-1 because a handful of names in
        # mm2aud carry stray high bytes and must not raise.
        entries.append(Entry(names[name_off:end].decode("latin-1"),
                             offset, size, packed))
    return entries, blob


def unpack(entry, blob):
    raw = blob[entry.offset:entry.offset + entry.packed_size]
    if entry.stored:
        return raw
    return zlib.decompressobj(-15).decompress(raw, entry.size)


def matches(entry, patterns):
    if not patterns:
        return True
    low = entry.name.lower()
    return any(p.lower() in low for p in patterns)


def main():
    ap = argparse.ArgumentParser(description="List/extract MM2 DAVE (.ar) archives")
    ap.add_argument("archive")
    ap.add_argument("patterns", nargs="*",
                    help="case-insensitive substrings; an entry matching any one is included")
    ap.add_argument("--list", action="store_true", help="list entries instead of extracting")
    ap.add_argument("-o", "--out", default=None, help="output directory (default: <archive stem>/)")
    ap.add_argument("--verify", action="store_true",
                    help="decompress every match and check the length against the recorded size")
    # intermixed, because argparse otherwise refuses positionals that follow an option and the
    # natural invocation is `arextract.py foo.ar -o out geometry/`.
    args = ap.parse_intermixed_args()

    entries, blob = read_archive(args.archive)
    picked = [e for e in entries if matches(e, args.patterns)]

    if args.list:
        print("%-10s %-10s %-9s %s" % ("OFFSET", "SIZE", "PACKED", "NAME"))
        for e in picked:
            print("%-10X %-10d %-9s %s" % (
                e.offset, e.size, "stored" if e.stored else e.packed_size, e.name))
        packed = sum(e.packed_size for e in picked)
        raw = sum(e.size for e in picked)
        print("\n%d/%d entries, %d bytes packed -> %d bytes (%.1f%%)"
              % (len(picked), len(entries), packed, raw,
                 100.0 * packed / raw if raw else 100.0))
        return 0

    if args.verify:
        bad = 0
        for e in picked:
            if e.is_dir:
                continue
            try:
                got = len(unpack(e, blob))
            except zlib.error as exc:
                print("FAIL %s: %s" % (e.name, exc))
                bad += 1
                continue
            if got != e.size:
                print("FAIL %s: got %d, header says %d" % (e.name, got, e.size))
                bad += 1
        print("verified %d entries, %d bad" % (len(picked), bad))
        return 1 if bad else 0

    out = args.out or os.path.splitext(os.path.basename(args.archive))[0]
    written = 0
    for e in picked:
        if e.is_dir:
            continue
        dest = os.path.join(out, *e.name.split("/"))
        d = os.path.dirname(dest)
        if d:
            try:
                os.makedirs(d)
            except OSError:
                pass
        data = unpack(e, blob)
        if len(data) != e.size:
            print("FAIL %s: got %d, header says %d" % (e.name, len(data), e.size))
            continue
        with open(dest, "wb") as f:
            f.write(data)
        written += 1
    print("extracted %d files to %s" % (written, out))
    return 0


if __name__ == "__main__":
    sys.exit(main())
