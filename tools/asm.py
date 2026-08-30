# -*- coding: utf-8 -*-
"""Keep game.asm in sync with what has been reimplemented.

This is the other half of the harness. game.asm holds the original machine code for every
function; a function is ported by writing the C++ and flipping its declaration from ARTS_IMPORT to
ARTS_EXPORT. This tool then removes that function's PROC from the assembly and declares it EXTERN
instead, so the linker resolves the symbol to the C++ rather than the original bytes.

That is what makes each function individually testable: exactly one thing changed, the game still
runs or it does not, and the bug is always in the function just written.

It also enforces the two invariants that keep the build honest:

  - every ARTS_IMPORT declaration must still have a PROC in the assembly, or the link will fail
    with an error that points nowhere useful
  - every ARTS_EXPORT must NOT, or the assembly silently wins and the C++ is never called - which
    looks exactly like "my reimplementation is correct" and is the worst possible failure mode

Run it after every port, alongside format.py, before building.
"""
import argparse
import json
import os
import re
import sys
from collections import OrderedDict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)

import encodings_table as encodings  # noqa: E402

CODE = os.path.join(ROOT, "code", "midtown2")
ASM = os.environ.get("MM2_ASM", os.path.join(ROOT, "code", "midtown2", "game.asm"))
EXTERNS = os.path.join(os.path.dirname(ASM), "game_externs.inc")

# The mangled symbol lives in a comment directly above its declaration. That comment is the only
# link between a C++ declaration and an address in midtown2.exe, which is why it is load-bearing
# and never edited by hand.
MANGLED = re.compile(r"^\s*//\s*(\?[^\s|]+)")
MARKER = re.compile(r"\bARTS_(IMPORT|EXPORT)\b")

PROC = re.compile(r"^(\S+)\s+PROC\b")
ENDP = re.compile(r"^(\S+)\s+ENDP\b")


def scan_headers(root):
    """{mangled: 'IMPORT'|'EXPORT'} for every declaration that carries a mangled-name comment."""
    marks = OrderedDict()
    pending = None

    for dirpath, _, files in os.walk(root):
        for name in sorted(files):
            if not name.endswith((".h", ".cpp")):
                continue

            path = os.path.join(dirpath, name)
            pending = None

            with open(path, encoding="utf-8", errors="replace") as f:
                for line in f:
                    m = MANGLED.match(line)
                    if m:
                        pending = m.group(1)
                        continue

                    if pending:
                        k = MARKER.search(line)
                        if k:
                            marks[pending] = k.group(1)
                            pending = None
                        elif line.strip() and not line.lstrip().startswith("//"):
                            pending = None  # a declaration with no marker: still original

    return marks


def read_procs(path):
    """{name: (start_line, end_line)} for every PROC block, plus the file's lines."""
    with open(path, encoding="utf-8", errors="replace") as f:
        lines = f.read().split("\n")

    procs = OrderedDict()
    open_name = None
    open_at = 0

    for i, line in enumerate(lines):
        m = PROC.match(line)
        if m:
            open_name = m.group(1)
            open_at = i
            continue

        m = ENDP.match(line)
        if m and open_name == m.group(1):
            procs[open_name] = (open_at, i)
            open_name = None

    return lines, procs


def emitted_bytes(block):
    """How many bytes a run of emitted directives assembles to.

    This decides how much padding replaces a ported function, and the padding must come to exactly
    the number of bytes removed. Undercount and .text shrinks, every address after it moves, and
    -FIXED -BASE:0x400000 stops describing the image - with no error anywhere, because the file
    still assembles and still links.

    That is why an unmeasurable line is a hard failure rather than a zero. game.asm used to be
    nothing but `db` and `dd`, so counting commas was enough; it now also carries real mnemonics
    (see tools/verify_encodings.py), whose lengths come from the same table that drove their
    emission.
    """
    table = encodings.load()
    total = 0

    for line in block:
        n = encodings.line_length(line.strip(), table)
        if n is None:
            raise ValueError("cannot measure this line, so the padding would be wrong: %r"
                             % line.strip())
        total += n

    return total


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true",
                    help="apply the changes; without it nothing is modified")
    args = ap.parse_args()

    if not os.path.exists(ASM):
        sys.exit("no assembly at %s - run ExportAsm.java first" % ASM)

    marks = scan_headers(CODE)
    lines, procs = read_procs(ASM)

    exports = [m for m, k in marks.items() if k == "EXPORT"]
    imports = [m for m, k in marks.items() if k == "IMPORT"]

    print("declarations: %d ARTS_IMPORT, %d ARTS_EXPORT" % (len(imports), len(exports)))
    print("assembly    : %d PROCs" % len(procs))

    # An ARTS_IMPORT with no PROC cannot link. Usually it means the mangled comment does not match
    # the binary, which is worth catching here rather than in a wall of linker errors.
    #
    # Static data members are declared ARTS_IMPORT too and have no PROC by definition - they are
    # labels in the data segment. Checking them reported 207 false failures, which is the fastest
    # way to teach someone to ignore this warning entirely.
    code_symbols = set()
    symbols_path = os.path.join(ROOT, "data", "symbols.json")

    if os.path.exists(symbols_path):
        with open(symbols_path, encoding="utf-8") as f:
            for s in json.load(f):
                if s.get("code"):
                    code_symbols.add(s["mangled"])

    ported_symbols = set()
    ported_path = os.path.join(ROOT, "data", "ported.json")
    if os.path.exists(ported_path):
        with open(ported_path, encoding="utf-8") as f:
            ported_symbols = set(json.load(f).get("ported", []))

    if code_symbols:
        imports = [m for m in imports if m in code_symbols]

    missing = [m for m in imports if m not in procs]
    if missing:
        print("\n%d ARTS_IMPORT symbols have no PROC in the assembly:" % len(missing))
        for m in missing[:15]:
            print("   %s" % m)
        if len(missing) > 15:
            print("   ... and %d more" % (len(missing) - 15))

    # Strip only what is BOTH declared ARTS_EXPORT and registered in data/ported.json.
    #
    # The dangerous direction is stripping a PROC with no C++ behind it: the original machine code
    # is gone and the link fails naming a function that was sitting right there. That used to be
    # guarded by refusing to build whenever a declaration had no registration.
    #
    # But the declaration scan is textual and does not understand the preprocessor, so an
    # ARTS_EXPORT inside a `#ifdef` that is not defined counts anyway. A GL file carrying a
    # correctly-guarded, deliberately-inactive `ARTS_EXPORT void BeginPhase(bool)` stopped every
    # build in the tree, for a port that could not possibly be compiled in.
    #
    # Requiring registration makes that harmless by construction rather than by prohibition: an
    # unregistered declaration simply does not strip, so there is nothing to fail. What remains
    # worth saying out loud is the mismatch itself.
    unregistered = [m for m in exports if m in code_symbols and m not in ported_symbols]         if code_symbols else []

    if unregistered:
        print()
        print("%d symbols are declared ARTS_EXPORT but are not in data/ported.json - "
              "their originals are KEPT:" % len(unregistered))
        for m in unregistered[:10]:
            print("   %s" % m)
        if len(unregistered) > 10:
            print("   ... and %d more" % (len(unregistered) - 10))

    exports = [m for m in exports if not code_symbols or m in ported_symbols]

    strip = [m for m in exports if m in procs]
    already = [m for m in exports if m not in procs]

    print("\n%d ARTS_EXPORT symbols still have a PROC and will be stripped" % len(strip))
    if already:
        print("%d already stripped" % len(already))

    if not strip:
        if not missing:
            print("\nnothing to do; assembly and headers agree")
        return 0 if not missing else 1

    # Replace each ported PROC with padding of exactly the same length, rather than deleting it.
    #
    # Deleting would shift every byte after it, and .text landing at its original addresses is what
    # keeps the unsymbolised absolute addresses valid - midtown2.exe has no relocation table, so a
    # shifted layout silently invalidates them (see docs/harness.md). Leaving 0xCC fill in the hole
    # keeps every other function exactly where it was. The reimplementation lives at a new address,
    # which is fine: all code references to it are symbolic, so the linker retargets them.
    # Symbols whose ORIGINAL machine code is kept, under an alias, instead of being padded out.
    #
    # This exists for the OpenGL activation and it is what makes it affordable. A function that
    # wants a -gl branch has to be registered, and registering it pads the original away - so the
    # else arm has to be a transcription, and a transcription of gfxBitmap::Load means porting four
    # pixel-format converters and a lock/convert/unlock loop before a single triangle is drawn.
    #
    # Renaming the PROC instead keeps those bytes exactly where they were and merely stops them
    # answering to the mangled name, which the C++ now defines. The C++ calls the alias for the
    # original behaviour. That is not just cheaper than transcribing, it is STRICTER: the rule in
    # agigl/glswitch.h is that with -gl absent the process must do exactly what it does today, and
    # calling the original code is the only way to guarantee that rather than hope for it. It also
    # leaves .text byte-identical to retail across the kept function, so step 11's verifier keeps
    # checking it instead of looking at 0xCC fill.
    keep_path = os.path.join(ROOT, "data", "keep_original.json")
    KEEP = {}

    if os.path.exists(keep_path):
        with open(keep_path, encoding="utf-8") as f:
            KEEP = {k: v for k, v in json.load(f).items() if not k.startswith("_")}

    sizes = {}
    symbols_path = os.path.join(ROOT, "data", "symbols.json")

    if os.path.exists(symbols_path):
        with open(symbols_path, encoding="utf-8") as f:
            for s in json.load(f):
                if s.get("code") and s.get("size"):
                    sizes[s["mangled"]] = s["size"]

    out = list(lines)
    padded = 0
    kept = 0

    for m in sorted(strip, key=lambda k: procs[k][0], reverse=True):
        start, end = procs[m]

        # The PUBLIC declaration sits on the line above the PROC and must go with it. Left behind,
        # it declares a symbol the assembly no longer defines while game_externs.inc declares the
        # same name EXTERN, and MASM rejects that as a redefinition.
        if start > 0 and lines[start - 1].strip() == "PUBLIC " + m:
            start -= 1

        # The kept-original case. Rename every occurrence of the mangled name inside the block -
        # the PUBLIC line, the PROC line and the ENDP line - and change nothing else, so not one
        # byte of the body moves. The mangled name is still declared EXTERN below, because the C++
        # defines it now and the surrounding assembly must reach the C++ rather than this.
        if m in KEEP:
            alias = "_arts_orig_" + KEEP[m]

            block = ["; %s -> C++, original kept as %s for the -gl fallback" % (m, alias)]
            block += [ln.replace(m, alias) for ln in lines[start:end + 1]]

            out[start:end + 1] = block
            kept += 1
            continue

        # Pad with exactly the number of bytes being removed, counted from the emitted directives.
        #
        # The map's size is the distance to the next symbol, which includes any inter-function
        # alignment - and ExportAsm already emits that alignment separately, outside the PROC. So
        # using the map size double-counts it: the first attempt grew .text by 775 bytes across 12
        # functions and moved sdlPage16::Draw 15 bytes, which defeats the whole point of padding.
        size = emitted_bytes(lines[start:end + 1])

        if size:
            # Keep any internal labels, at their original offsets inside the padding.
            #
            # MSVC puts a switch table in .text OUTSIDE the function it belongs to, so stripping the
            # function deletes the case labels while the table still names them - twelve
            # "undefined symbol : caseD_3_4C0107" errors the moment Vector3::GetVector2 and
            # HSVtoRGB were ported.
            #
            # Splitting the fill at each label's offset means the table resolves to exactly the
            # address it resolved to in retail, so those bytes stay byte-identical and the verifier
            # stays honest. The labels now point at 0xCC fill, which is correct: the function is
            # reimplemented, so nothing reaches that table any more.
            inner = []
            off = 0
            for ln in lines[start:end + 1]:
                lab = re.match(r"^([A-Za-z_?@$][\w?@$]*):$", ln.strip())
                if lab:
                    inner.append((off, lab.group(1)))
                else:
                    off += emitted_bytes([ln])

            block = ["; %s -> C++ (%d bytes of original code held as padding)" % (m, size)]
            prev = 0
            for at, name in inner:
                if at > prev:
                    block.append("    db %d dup (0CCh)" % (at - prev))
                    prev = at
                block.append(name + ":")
            if size > prev:
                block.append("    db %d dup (0CCh)" % (size - prev))

            padded += 1
        else:
            block = ["; %s -> C++ (nothing to pad; layout after this point may have SHIFTED)" % m]

        out[start:end + 1] = block

    # Call sites that now target C++ must become real instructions.
    #
    # A relative reference is emitted as `db 0E8h` + `dd SYM - ($ + 4)`, which MASM can only
    # evaluate for a symbol defined in the same module. Once SYM is EXTERN the subtraction is a
    # link-time relocation, and MASM rejects it as "invalid use of external symbol". Writing the
    # call as an instruction lets it emit a proper REL32 relocation instead, which is what makes
    # the linker retarget the call to the reimplementation.
    stripped = set(strip)
    rewritten = 0
    unhandled = 0

    for i, line in enumerate(out):
        m = re.match(r"^\s*dd (\S+) - \(\$ \+ 4\)\s*$", line)
        if not m or m.group(1) not in stripped:
            continue

        prev = out[i - 1].strip() if i > 0 else ""

        if prev == "db 0E8h":
            out[i - 1] = "    call " + m.group(1)
            out[i] = "    ; (rel32 above is now a linker relocation)"
            rewritten += 1
        elif prev == "db 0E9h":
            out[i - 1] = "    jmp " + m.group(1)
            out[i] = "    ; (rel32 above is now a linker relocation)"
            rewritten += 1
        else:
            # Some other encoding reaches this symbol relatively. Report which, rather than
            # emitting something plausible - a wrong instruction here is a silent miscompile.
            unhandled += 1
            if unhandled <= 5:
                print("  cannot rewrite %s at line %d (preceding bytes: %s)"
                      % (m.group(1), i + 1, prev))

    if unhandled:
        print("  %d relative references could not be rewritten" % unhandled)

    # Hand the stateless 1999 CRT routines over to the modern CRT by removing their PUBLIC
    # declaration. The PROC and its bytes stay exactly where they are - .text does not move by one
    # byte - the name simply stops being exported, so the linker resolves references from our C++
    # against libucrt instead. game.asm's own calls are internal and still reach the original.
    #
    # This is what makes dropping -NODEFAULTLIB mean anything. Without it the link succeeds but
    # libucrt is never searched, because game.obj already claims every name in the objects that
    # would have supplied it. See data/crt_shared.json for what may and may not go in the list.
    shared = set()
    crt_path = os.path.join(ROOT, "data", "crt_shared.json")
    if os.path.exists(crt_path):
        with open(crt_path, encoding="utf-8") as f:
            for key, names in json.load(f).items():
                if not key.startswith("_"):
                    shared.update(names)

    # Removing the PUBLIC line is NOT sufficient, and the failure is confusing: MASM makes a PROC
    # visible by default, so `__itoa PROC` still exports __itoa with no PUBLIC anywhere, and the
    # link still fails with "already defined in game.obj". The PROC must be declared PRIVATE -
    # which is why Open1560's game.asm is written `sub_4049E0 PROC PRIVATE` throughout.
    unshared, privatised = 0, 0
    if shared:
        for i, line in enumerate(out):
            s = line.strip()
            if s.startswith("PUBLIC ") and s[7:].strip() in shared:
                out[i] = "; " + s + "   ; shared with the modern CRT - data/crt_shared.json"
                unshared += 1
                continue
            m = PROC.match(line)
            if m and m.group(1) in shared and " PRIVATE" not in line:
                out[i] = line.rstrip() + " PRIVATE"
                privatised += 1
        print("\nCRT routines handed to the modern library: %d PUBLIC removed, %d PROC privatised"
              % (unshared, privatised))
        for name in sorted(n for n in shared if n not in procs):
            print("  no PROC in the assembly (data symbol or absent): %s" % name)

    if not args.write:
        print("\ndry run; pass --write to apply")
        return 0

    with open(ASM, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(out))

    # Every stripped function must now be declared EXTERN so the remaining assembly can still call
    # it. This section is REWRITTEN from the current set, never appended to.
    #
    # Appending leaks. A symbol ported in one run and not the next left its EXTERN behind forever,
    # so the assembly declared a symbol nothing defined and the link failed with 15 unresolved
    # externals naming functions whose PROCs were sitting right there in game.asm. The file had
    # accumulated two "moved to C++" sections and 122 EXTERNs for 112 ported symbols.
    #
    # Everything above the marker is ExportAsm's output (the import declarations) and is preserved.
    MARKER = "; ---- moved to C++ ----"

    head = []
    if os.path.exists(EXTERNS):
        with open(EXTERNS, encoding="utf-8", errors="replace") as f:
            for line in f:
                if line.rstrip("\n") == MARKER:
                    break
                head.append(line.rstrip("\n"))

    while head and not head[-1].strip():
        head.pop()

    with open(EXTERNS, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(head))
        f.write("\n\n%s\n" % MARKER)
        for m in sorted(strip):
            f.write("EXTERN %s:PROC\n" % m)

    print("\nstripped %d PROCs (%d padded, %d kept under an alias), rewrote %d call sites, %d EXTERNs"
          % (len(strip), padded, kept, rewritten, len(strip)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
