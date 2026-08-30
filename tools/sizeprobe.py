# -*- coding: utf-8 -*-
"""Ask the compiler what sizeof(X) ACTUALLY is for classes whose check_size fails.

check_size is a static_assert, so a failure says only what the size must be, never what it is -
and without the difference there is nothing to reason from. `char (*p)[sizeof(X)] = 1;` puts the
real number in the error text ("cannot convert from 'int' to 'char (*)[52]'"), which is enough to
name the cause: short by the size of a base means the base is wrong, short by four means a missing
vptr or a padding assumption, over by a multiple of four usually means a member counted twice.

    py tools/sizeprobe.py                # every class whose check_size currently fails
    py tools/sizeprobe.py aiMap sdlPage16
"""
import glob
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
CODE = os.path.join(ROOT, "code", "midtown2")


def find(pattern, what):
    hits = sorted(glob.glob(pattern))
    if not hits:
        sys.exit("could not find %s (looked for %s)" % (what, pattern))
    return hits[-1]


def failing():
    """(class, required size) for every check_size the header gate reports."""
    log = os.path.join(ROOT, "build", "headers.log")
    if not os.path.exists(log):
        sys.exit("no build/headers.log - run tools/verify_headers.py first")
    out = {}
    with open(log, encoding="utf-8", errors="replace") as f:
        for ln in f:
            m = re.search(r"sizeof\((\w+)\) must be 0x([0-9A-Fa-f]+)", ln)
            if m:
                out[m.group(1)] = int(m.group(2), 16)
    return out


def main():
    want = failing()
    picked = sys.argv[1:]
    if picked:
        want = {c: want.get(c, 0) for c in picked}

    headers = sorted(glob.glob(os.path.join(CODE, "**", "*.h"), recursive=True))
    cl = find(r"C:\Program Files\Microsoft Visual Studio\*\*\VC\Tools\MSVC"
              r"\*\bin\Hostx64\x86\cl.exe", "cl.exe")
    msvc_inc = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(
        os.path.dirname(cl)))), "include")
    sdk_inc = find(r"C:\Program Files (x86)\Windows Kits\10\Include\*\ucrt", "the SDK headers")
    sdk_root = os.path.dirname(sdk_inc)

    out = os.path.join(ROOT, "build")
    os.makedirs(out, exist_ok=True)
    tu = os.path.join(out, "sizeprobe.cpp")
    probe_line = {}

    with open(tu, "w", encoding="utf-8", newline="\n") as f:
        # check_size is what fails; defining it away leaves the class definitions intact so their
        # real sizes can be measured. Measuring with the assertion live would just fail again.
        #
        # It is defined in core/hooking.h, which every generated header pulls in, so defining it
        # here first is simply overwritten. Include the real definition, then replace it - the
        # #pragma once means hooking.h will not put its own version back later.
        f.write('#include "core/hooking.h"\n')
        f.write("#undef check_size\n")
        f.write('#define check_size(T, N) static_assert(true, "")\n')
        for h in headers:
            f.write('#include "%s"\n' % os.path.relpath(h, CODE).replace(os.sep, "/"))
        f.write("\n")
        # MSVC reports the size but NOT the name - "cannot convert from 'int' to 'char (*)[280]'" -
        # so the probes are matched back by LINE NUMBER, recorded as they are written.
        line = sum(1 for _ in open(tu, encoding="utf-8")) if os.path.exists(tu) else 0
        line = 3 + len(headers) + 1  # the three prologue lines, the includes, the blank
        for cls in sorted(want):
            line += 1
            probe_line[line] = cls
            f.write("char (*probe_%s)[sizeof(%s)] = 1;\n" % (cls, cls))

    res = subprocess.run(
        [cl, "-nologo", "-c", "-EHs-c-", "-O1", "-arch:IA32", "-w", "-GS-", "-GR-", "-MT",
         "-Zc:sizedDealloc-", "-DARTS_AGIGL", "-DARTS_AGIGL_PHASE",
         "-I" + CODE, "-I" + msvc_inc, "-I" + sdk_inc,
         "-I" + os.path.join(sdk_root, "um"), "-I" + os.path.join(sdk_root, "shared"),
         "-Fo" + os.path.join(out, "sizeprobe.obj"), tu],
        capture_output=True, text=True)

    text = (res.stdout or "") + (res.stderr or "")
    actual = {}
    for m in re.finditer(r"sizeprobe\.cpp\((\d+)\).*?char \(\*\)\[(\d+)\]", text):
        cls = probe_line.get(int(m.group(1)))
        if cls:
            actual[cls] = int(m.group(2))

    print("%-24s %8s %8s %8s" % ("class", "actual", "required", "delta"))
    unknown = []
    for cls in sorted(want):
        need = want[cls]
        got = actual.get(cls)
        if got is None:
            unknown.append(cls)
            continue
        print("  %-22s %8X %8X %+8d" % (cls, got, need, got - need))
    if unknown:
        print("\nno size reported for %d class(es): %s" % (len(unknown), ", ".join(unknown[:8])))
        print("(usually means the class failed to compile for another reason)")


if __name__ == "__main__":
    main()
