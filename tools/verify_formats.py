# -*- coding: utf-8 -*-
"""Every printf format string must survive the export unchanged.

`verify_strings.py` already checks text, but it decides what counts as text by counting letters -
two or more - because three printable bytes are otherwise indistinguishable from an address. Format
strings are short and mostly punctuation, so they fall straight through that rule: `"%5d"` has one
letter, `"%4d"` one, `"#%d"` one, `"%s_"` one. All four were being relocated as pointers, because
`"%5d"` is `0x00643525`, which lands squarely inside `.data`.

They are worth a gate of their own because corrupting one is worse than corrupting ordinary text. A
format string decides how `sprintf` reads its arguments: turn a `%d` into a `%s` and it is handed an
integer to dereference, and writes until it finds a zero byte. That is how a 12-byte stack buffer
becomes a smashed frame, and MM2 formats into small stack buffers constantly.

The rule here needs no letter counting. A `%` followed by optional flags, width and precision and
then a conversion character is not a coincidence in the way that three printable bytes are, so this
finds format strings exactly and demands they be identical.
"""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)

from pe import IMAGE_BASE
from verify_data import sections

RETAIL = os.environ.get(
    "MM2_RETAIL", "C:/Users/xshxd/OneDrive/Desktop/PC Games/MM2/midtown2.exe")
BUILT = os.path.join(ROOT, "build", "OpenMM2.exe")

CONVERSION = re.compile(rb"%[-+ #0-9.]*[diouxXeEfgGcspn%]")
PRINTABLE = re.compile(rb"[ -~]{2,120}\x00")


def main():
    if not os.path.exists(BUILT):
        print("no build to check")
        return 0

    _, retail = sections(RETAIL)
    _, built = sections(BUILT)

    checked = 0
    bad = []

    for name in (".rdata", ".data"):
        if name not in retail or name not in built:
            continue

        rb = retail[name]["bytes"]
        bb = built[name]["bytes"]
        va = retail[name]["va"] + IMAGE_BASE

        # Our copy of a section does not always start at offset 0 of ours - the linker puts its own
        # import table at the front of .rdata.
        shift = bb.find(rb[:256])
        if shift < 0:
            print("  %s: cannot locate our copy of the section - skipped" % name)
            continue

        for m in PRINTABLE.finditer(rb):
            text = m.group()[:-1]
            if not CONVERSION.search(text):
                continue

            checked += 1
            got = bb[shift + m.start():shift + m.start() + len(text)]
            if got != text:
                bad.append((name, va + m.start(), text, got))

    print("  format strings checked : %d" % checked)

    if not bad:
        print("  PASS: every printf format string is byte-identical to retail")
        return 0

    print("  CORRUPTED              : %d" % len(bad))
    for name, addr, want, got in bad[:12]:
        print("    %s 0x%08X" % (name, addr))
        print("      want %r" % (want,))
        print("      got  %r" % (got,))
    if len(bad) > 12:
        print("    ... and %d more" % (len(bad) - 12))
    print("  FAIL: a format string was relocated as if it were a pointer -")
    print("        sprintf will misread its arguments wherever this one is used")
    return 1


if __name__ == "__main__":
    sys.exit(main())
