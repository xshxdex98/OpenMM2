"""
Build an MM2 'DAVE' archive.

The counterpart to tools/arextract.py, and it follows that file's reading of
`?Init@zipFile@@QAE_NPBD@Z`:

    header  "DAVE", EntryCount, EntryTableSize, NamesSize      (4 u32, at offset 0)
    entries at the FIXED offset 0x800, stride 0x10, four u32 each:
            name offset (into the names blob), data offset, uncompressed size, packed size
    names   immediately after the entry table, NUL-terminated, in the same order
    data    anywhere after that

The two header sizes are LENGTHS, not positions - that is the trap arextract.py calls out, and
writing them as positions produces an archive that looks right and loads nothing.

COMPRESSION IS CHOSEN THE WAY THE LOADER DETECTS IT. `zipFile::Open` has no per-entry flag: it
compares the two sizes, and equal sizes mean the bytes are stored verbatim, otherwise it inflates
with `inflateInit2_(..., -15, ...)` - raw deflate, no zlib header. So this deflates with wbits=-15
and falls back to storing whenever compression would not actually shrink the entry, because an
entry that "compressed" to the same size would be read back as stored and come out as garbage.

Names are stored with forward slashes, lower case, exactly as the archives the game ships use, and
the table is sorted by name so a binary search in the loader behaves.
"""
import os
import struct
import sys
import zlib

MAGIC = b"DAVE"
TABLE_OFFSET = 0x800
STRIDE = 0x10


def collect(root):
    """Every file under `root`, as (archive name, bytes), archive names relative and slash-joined."""
    out = []

    for base, _dirs, files in os.walk(root):
        for fn in sorted(files):
            full = os.path.join(base, fn)
            rel = os.path.relpath(full, root).replace("\\", "/")
            out.append((rel, open(full, "rb").read()))

    out.sort(key=lambda e: e[0].lower())
    return out


def build(entries, out_path):
    names_blob = bytearray()
    name_offsets = []

    for name, _data in entries:
        name_offsets.append(len(names_blob))
        names_blob += name.encode("latin-1") + b"\0"

    table_size = len(entries) * STRIDE
    names_size = len(names_blob)

    # Data starts after the table and the names, rounded up so entries stay tidily aligned - the
    # loader does not require it, but every shipped archive is laid out that way.
    data_start = TABLE_OFFSET + table_size + names_size
    data_start = (data_start + 0x7FF) & ~0x7FF

    blobs = []
    table = bytearray()
    cursor = data_start

    for i, (name, data) in enumerate(entries):
        # raw deflate, no zlib header or adler trailer - what inflateInit2_(..., -15, ...) expects
        co = zlib.compressobj(9, zlib.DEFLATED, -15)
        packed = co.compress(data) + co.flush()

        if len(packed) >= len(data):
            packed = data                    # store verbatim; equal sizes is how the loader knows

        table += struct.pack("<IIII", name_offsets[i], cursor, len(data), len(packed))
        blobs.append(packed)
        cursor += len(packed)

    with open(out_path, "wb") as f:
        f.write(struct.pack("<4sIII", MAGIC, len(entries), table_size, names_size))
        f.write(b"\0" * (TABLE_OFFSET - 16))
        f.write(table)
        f.write(names_blob)
        f.write(b"\0" * (data_start - (TABLE_OFFSET + table_size + names_size)))

        for b in blobs:
            f.write(b)

    return data_start, cursor


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        print("usage: arpack.py <folder> <out.ar>")
        return 2

    root, out_path = sys.argv[1], sys.argv[2]
    entries = collect(root)

    if not entries:
        print("nothing to pack under %s" % root)
        return 1

    start, end = build(entries, out_path)
    stored = 0

    print("packed %d file(s) -> %s (%d bytes)" % (len(entries), out_path, os.path.getsize(out_path)))
    for name, data in entries[:6]:
        print("   %-40s %d bytes" % (name, len(data)))
    if len(entries) > 6:
        print("   ... and %d more" % (len(entries) - 6))
    return 0


if __name__ == "__main__":
    sys.exit(main())
