# -*- coding: utf-8 -*-
"""Build import libraries for the DLLs midtown2.exe uses.

The harness references every Windows API through its `__imp__` slot, so the link needs an import
library for each DLL. Most come with the Windows SDK, but this is a 1999 game: it imports
`DPLAYX.dll` (DirectPlay, removed from the SDK long ago) and `DINPUT.dll` (DirectInput 7 - the SDK
ships `dinput8.lib`, which is a different DLL with a different export set). Neither can be
satisfied from what is installed.

They do not have to be. An import library carries nothing but names and ordinals, and the binary's
own import table lists exactly the names it needs. So the import table is read back out and turned
into a .def per DLL, and `lib.exe` makes a real import library from that.

The result is link-only: it resolves symbols and records the DLL to load at runtime. It contains
no Microsoft code.
"""
import os
import struct
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

EXE = os.environ.get("MM2_EXE", r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\midtown2.exe")
OUT = os.path.join(ROOT, "build", "implibs")

# Every DLL, not just the missing ones.
#
# The harness references imports as undecorated __imp__CreateFileA, because that name is all
# Ghidra reports - reconstructing the stdcall @N suffix would mean recovering every API's
# parameter byte count. Generating our own import libraries from an undecorated .def sidesteps
# that: lib.exe emits exactly the symbol we asked for. These libraries carry names and a DLL to
# load, nothing else, so using ours instead of the SDK's costs nothing.
SYNTHESISE = None  # None means all

# DirectPlay and DirectSound are imported BY ORDINAL, so the PE carries no name for them - but the
# disassembly references them by name, because Ghidra knows what those ordinals are. Emitting
# "Name @N NONAME" gives both: lib.exe produces the __imp__Name symbol the assembly needs, while
# the import itself still binds by ordinal, exactly as the original does.
ORDINAL_NAMES = {
    ("dplayx.dll", 1): "DirectPlayCreate",
    ("dsound.dll", 1): "DirectSoundCreate",
    ("dsound.dll", 2): "DirectSoundEnumerateA",
}


def sections(data, pe):
    n = struct.unpack_from("<H", data, pe + 6)[0]
    tbl = pe + 24 + struct.unpack_from("<H", data, pe + 20)[0]
    out = []

    for i in range(n):
        b = tbl + i * 40
        vsz, va, rsz, rp = struct.unpack_from("<IIII", data, b + 8)
        out.append((va, va + max(vsz, rsz), rp))

    return out


def to_off(secs, rva):
    for va, end, rp in secs:
        if va <= rva < end:
            return rp + (rva - va)
    return None


def cstr(data, off):
    return data[off:data.index(b"\0", off)].decode("ascii", "replace")


def read_imports(path):
    """{dll: [(name, ordinal_or_None), ...]} from the import directory."""
    with open(path, "rb") as f:
        data = f.read()

    pe = struct.unpack_from("<I", data, 0x3C)[0]
    secs = sections(data, pe)
    opt = pe + 24
    imp_rva = struct.unpack_from("<I", data, opt + 96 + 8)[0]

    p = to_off(secs, imp_rva)
    result = {}

    while True:
        ilt, _, _, name_rva, iat_rva = struct.unpack_from("<IIIII", data, p)
        if name_rva == 0:
            break

        dll = cstr(data, to_off(secs, name_rva))
        names = []

        # The lookup table is the reliable one; fall back to the IAT when it is absent.
        table = to_off(secs, ilt or iat_rva)
        q = table

        while True:
            entry = struct.unpack_from("<I", data, q)[0]
            if entry == 0:
                break

            if entry & 0x80000000:
                names.append((None, entry & 0xFFFF))  # imported by ordinal
            else:
                hint_off = to_off(secs, entry & 0x7FFFFFFF)
                names.append((cstr(data, hint_off + 2), None))

            q += 4

        result[dll] = names
        p += 20

    return result


def main():
    imports = read_imports(EXE)

    print("import table: %d DLLs" % len(imports))
    for dll, names in imports.items():
        mark = "" if SYNTHESISE is None else ("  (synthesising)" if dll.lower() in SYNTHESISE else "")
        print("   %-16s %4d symbols%s" % (dll, len(names), mark))

    os.makedirs(OUT, exist_ok=True)

    lib = os.environ.get("LIB_EXE")
    if not lib:
        import glob
        found = glob.glob(r"C:\Program Files\Microsoft Visual Studio\*\*\VC\Tools\MSVC"
                          r"\*\bin\Hostx64\x86\lib.exe")
        lib = found[0] if found else None

    if not lib:
        sys.exit("lib.exe not found; set LIB_EXE")

    made = 0

    for dll, names in imports.items():
        if SYNTHESISE is not None and dll.lower() not in SYNTHESISE:
            continue

        stem = os.path.splitext(dll)[0]
        deff = os.path.join(OUT, stem + ".def")

        with open(deff, "w", encoding="ascii", newline="\n") as f:
            f.write("LIBRARY %s\n" % dll)
            f.write("EXPORTS\n")
            for name, ordinal in names:
                if name:
                    f.write("    %s\n" % name)
                else:
                    known = ORDINAL_NAMES.get((dll.lower(), ordinal))
                    if known:
                        f.write("    %s @%d NONAME\n" % (known, ordinal))
                    else:
                        f.write("    Ordinal%d @%d NONAME\n" % (ordinal, ordinal))

        out_lib = os.path.join(OUT, stem + ".lib")
        res = subprocess.run([lib, "-def:" + deff, "-out:" + out_lib, "-machine:x86"],
                             capture_output=True, text=True)

        if res.returncode != 0:
            print("  lib.exe failed for %s:\n%s" % (dll, res.stdout + res.stderr))
            continue

        print("  wrote %s" % out_lib)
        made += 1

    print("\n%d import libraries built in %s" % (made, OUT))


if __name__ == "__main__":
    main()
