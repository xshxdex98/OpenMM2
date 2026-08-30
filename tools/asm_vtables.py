# -*- coding: utf-8 -*-
"""Hand a class's vftable over from game.asm to the C++ side.

tools/asm.py strips a PROC when a function flips to ARTS_EXPORT. That covers code and nothing
else, and it is why every polymorphic class in the binary is currently unportable: the moment a
constructor or destructor for a class with virtuals is defined in C++, MSVC emits its own
`??_7Class@@6B@` and the link fails with a duplicate symbol against the copy already sitting in
.rdata inside game.asm. 335 classes carry a vftable - 58.5% of the bytes in named code symbols.

This is the other half of the mechanism. It does for a vftable what asm.py does for a PROC.

WHAT IT DOES NOT DO - AND WHY THAT IS THE WHOLE DESIGN

asm.py can pad a stripped function with 0xCC because a PROC is a self-contained run of bytes with
nothing else living inside it. A vftable is not. Measured against game.asm, every one of the 7,191
references to a vftable symbol takes one of four forms and there are no others:

    340  PUBLIC ??_7Class@@6B@
    340  ??_7Class@@6B@ LABEL BYTE
    693  dd offset ??_7Class@@6B@
  5,818  dd offset ??_7Class@@6B@ + N

and every single one of those 5,818 has N BEYOND the end of the table - up to +33,996. They are
not vtable slots. They are Ghidra naming an unsymbolised .rdata constant by the nearest preceding
symbol, and the nearest preceding symbol is very often a vftable label, because vftables are the
densest named thing in .rdata. Delete or blank a vftable's bytes and you do not lose a vtable, you
lose up to 34 KB of anchored float constants and string tables per table, silently.

So nothing is removed and nothing is padded. The transformation is:

    PUBLIC ??_7asNode@@6B@              ->  ; PUBLIC ??_7asNode@@6B@   (commented out)
    ??_7asNode@@6B@ LABEL BYTE          ->  __vft_orig_005B28E8 LABEL BYTE
    dd offset ??_7asNode@@6B@ + 52      ->  dd offset __vft_orig_005B28E8 + 52
    dd offset ??_7asNode@@6B@           ->  (untouched)
                                        +   EXTERN ??_7asNode@@6B@:BYTE in game_externs.inc

The bytes of the old table stay exactly where they were, under a private label, as dead data. The
+N anchors keep pointing at the same addresses. The plain references - the vptr stores in the
constructors that are still assembly, and any type-identity compare - become external and the
linker retargets them to the C++ table. .rdata does not move by one byte, the line count does not
change, and the emitted byte count does not change; `verify_byte_count` asserts the last of those
on every run. That is what makes this safe on a binary with no relocation table, and it is why the
`.data` shift documented in docs/harness.md cannot get worse from a vtable handover.

WHY PARTIAL PORTING WORKS

A vtable is a list of external symbol references and the linker does not care where they come
from. A virtual that is still ARTS_IMPORT is declared in the header but defined nowhere in C++, so
MSVC emits an undefined external for it and the linker resolves it against the PROC still sitting
in game.asm under the same mangled name. A class can therefore have its constructor, destructor
and vtable in C++ while most of its virtuals are still 1999 machine code. That is the property
that makes a 26-slot class portable at all.

The price is that the slot ORDER must be exact, and that the set of declared virtuals must be
exact too. MSVC assigns slots from the declaration order of virtuals in the header, base class
first. Declare one virtual too many, too few, or in the wrong order, and every original call
through that table dispatches to the wrong method - no link error, no crash at the call site, just
a game that behaves strangely. The generated headers sort methods ALPHABETICALLY (house style), so
for a polymorphic class the generated order is essentially always wrong.

`--manifest CLASS` prints the order the binary requires. `--check CLASS` reads the header back and
says whether it would produce that table; that check is the one thing standing between a port and
a silent, undebuggable failure, so run it before every polymorphic class goes over.

USE FROM asm.py - a single call

    import asm_vtables

    h = asm_vtables.hand_over(lines, marks)     # marks = scan_headers(CODE)
    print(h.report)
    if not h.ok:
        return 1
    lines = h.lines                             # same length, same emitted bytes
    strip |= h.strip_procs                      # PROCs asm.py must ALSO remove
    ...                                         # asm.py's own PROC stripping, unchanged
    asm_vtables.add_externs(h.externs)          # EXTERN <vftable>:BYTE

It must run BEFORE asm.py's PROC stripping, because that is the only ordering in which asm.py's
line indices stay valid: hand_over() preserves the line count exactly, PROC stripping does not.
See docs/vtables.md.

Standalone:

    py tools/asm_vtables.py                    # dry run against the headers
    py tools/asm_vtables.py --write
    py tools/asm_vtables.py --manifest asNode  # the required virtual declaration order
    py tools/asm_vtables.py --check asNode     # does the header actually produce that table?
    py tools/asm_vtables.py --audit            # every vftable, portability gates
"""
import argparse
import json
import os
import re
import sys
from collections import OrderedDict, namedtuple

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

CODE = os.path.join(ROOT, "code", "midtown2")
ASM = os.environ.get("MM2_ASM", os.path.join(ROOT, "code", "midtown2", "game.asm"))
EXTERNS = os.path.join(os.path.dirname(ASM), "game_externs.inc")

SYMBOLS = os.path.join(ROOT, "data", "symbols.json")
HIERARCHY = os.path.join(ROOT, "data", "hierarchy.json")
MM2TYPES = os.path.join(ROOT, "data", "mm2types.json")
VTABLE_ORDER = os.path.join(ROOT, "data", "vtable_order.json")

# ExportAsm.java emits every .rdata symbol beginning with '?' as `PUBLIC <sym>` followed by
# `<sym> LABEL BYTE` (ExportAsm.java:670-672), then one `dd offset <target>` per pointer word.
# All 340 vftables in the binary have exactly this shape; there are no exceptions.
VFT_LABEL = re.compile(r"^(\?\?_7\S*) LABEL BYTE\s*$")
VFT_PUBLIC = re.compile(r"^PUBLIC (\?\?_7\S*)\s*$")
DD_OFFSET = re.compile(r"^(\s*)dd offset (\S+)(\s*\+\s*(\d+))?\s*$")

# A slot target that Ghidra never named. mmMultiCircuit's destructor slot is the only one in the
# binary, and it is still a real code pointer.
FUN_NAME = re.compile(r"^(FUN|LAB|SUB)_[0-9A-Fa-f]{8}$")

MANGLED = re.compile(r"^\s*//\s*(\?[^\s|]+)")
MARKER = re.compile(r"\bARTS_(IMPORT|EXPORT)\b")


def label_for(rva):
    """The private name the original table keeps.

    Derived from the address so it is unique by construction even for the five classes that carry
    two vftables, and so that a reader can find the table in midtown2.exe from the label alone.
    """
    return "__vft_orig_%08X" % rva


# ---------------------------------------------------------------------------------------------
# data


_cache = {}


def load(path, what):
    if path not in _cache:
        if not os.path.exists(path):
            sys.exit("missing %s (%s)" % (path, what))
        with open(path, encoding="utf-8") as f:
            _cache[path] = json.load(f)
    return _cache[path]


def symbol_index():
    if "index" not in _cache:
        syms = load(SYMBOLS, "run tools/symbols.py")
        _cache["index"] = {s["mangled"]: s for s in syms}
    return _cache["index"]


def vftable_symbols():
    """{mangled: symbol} for every `vftable' in the binary."""
    if "vfts" not in _cache:
        syms = load(SYMBOLS, "run tools/symbols.py")
        _cache["vfts"] = OrderedDict(
            (s["mangled"], s) for s in sorted(syms, key=lambda s: s["rva"])
            if s["kind"] == "vftable")
    return _cache["vfts"]


def vftables_of(cls):
    """Every vftable belonging to a class - two, for the five multiple-inheritance classes."""
    return [m for m, s in vftable_symbols().items() if s["class"] == cls]


def order_of(cls):
    """data/vtable_order.json's entry for a class - the slot order read out of midtown2.exe.

    That file, not this one, is the authority on which table is primary and what is in it. It
    reads the executable directly; this module reads the assembly Ghidra emitted from it. The two
    agree on the slot count of all 340 tables, which is the cross-check that matters.
    """
    return load(VTABLE_ORDER, "run tools/vtable_order.py").get(cls)


def primary_vftable(cls):
    """The table the vptr at offset 0 points to.

    Usually `??_7C@@6B@`. The five multiple-inheritance classes have no such symbol - MSVC names
    their tables after the base subobjects instead - and picking the lower address is WRONG:
    vehTrailer's `@@6BasNode@@@` sits below `@@6BdgPhysEntity@@@` and is the SECONDARY table.
    vtable_order.json decides this, by looking for the table with no adjustor thunk in it; see
    docs/vtable_order.md.
    """
    want = "??_7%s@@6B@" % cls
    if want in vftable_symbols():
        return want
    e = order_of(cls)
    return e["vftable_symbol"] if e else None


# ---------------------------------------------------------------------------------------------
# reading game.asm


VTable = namedtuple("VTable", "name cls rva label_line public_line slots targets refs stripped")


def scan_vtables(lines):
    """{mangled: VTable} for every vftable the assembly still defines, plus its references.

    `slots` is counted from the emitted directives, not from the map. The map's size for a data
    symbol is the distance to the next symbol, which for a vftable includes whatever unnamed
    .rdata follows it - for mmGameManager that is 24 bytes of float constants past a 52-byte
    table. Counting the `dd offset <code symbol>` run instead gives the true length, and it agrees
    with a direct read of midtown2.exe on 340 of 340 tables.
    """
    index = symbol_index()
    vfts = vftable_symbols()

    found = OrderedDict()
    refs = {}

    for i, line in enumerate(lines):
        m = VFT_LABEL.match(line)
        if not m:
            continue

        name = m.group(1)
        sym = vfts.get(name)
        if sym is None:
            continue

        public_line = i - 1 if i > 0 and VFT_PUBLIC.match(lines[i - 1] or "") \
            and VFT_PUBLIC.match(lines[i - 1]).group(1) == name else None

        targets = []
        j = i + 1
        while j < len(lines):
            d = DD_OFFSET.match(lines[j])
            if not d or d.group(3):          # `+ N` never appears inside a table
                break
            t = d.group(2)
            s = index.get(t)
            if not ((s and s["code"]) or FUN_NAME.match(t)):
                break                        # the table ended; this is ordinary .rdata
            targets.append(t)
            j += 1

        found[name] = VTable(name=name, cls=sym["class"], rva=sym["rva"], label_line=i,
                             public_line=public_line, slots=len(targets), targets=targets,
                             refs=[], stripped=False)

    # Second pass for references. Done separately because a reference can precede its definition.
    for i, line in enumerate(lines):
        if "??_7" not in line:
            continue
        d = DD_OFFSET.match(line)
        if not d:
            continue
        name = d.group(2)
        if name not in vfts:
            continue
        addend = int(d.group(4)) if d.group(4) else 0
        refs.setdefault(name, []).append((i, addend))

    out = OrderedDict()
    for name, v in found.items():
        out[name] = v._replace(refs=refs.get(name, []))

    # A vftable with no LABEL BYTE left has already been handed over.
    for name, sym in vfts.items():
        if name not in out:
            out[name] = VTable(name=name, cls=sym["class"], rva=sym["rva"], label_line=None,
                               public_line=None, slots=0, targets=[],
                               refs=refs.get(name, []), stripped=True)

    return out


# ---------------------------------------------------------------------------------------------
# deciding what to hand over


def scan_headers(root=CODE):
    """{mangled: 'IMPORT'|'EXPORT'} - the same scan tools/asm.py performs.

    Duplicated rather than imported so this module stays usable while asm.py is being edited.
    """
    marks = OrderedDict()

    for dirpath, _, files in os.walk(root):
        for fname in sorted(files):
            if not fname.endswith((".h", ".cpp")):
                continue

            pending = None
            with open(os.path.join(dirpath, fname), encoding="utf-8", errors="replace") as f:
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
                            pending = None

    return marks


def claimed_classes(marks):
    """Classes whose vftable the C++ side is now defining.

    Derived, not marked. MSVC emits `??_7C@@6B@` as a COMDAT in every translation unit that
    defines a constructor or a destructor of C, because those are the functions that store the
    vptr - and in no other. So "the C++ owns this vtable" is exactly "a ctor or dtor of C is
    ARTS_EXPORT", and there is no new convention to keep in sync with anything.

    Defining an ordinary virtual does NOT emit the vtable, so virtuals can be ported freely on a
    class whose constructor is still assembly. That is worth knowing: it is a much larger set of
    work than docs/port_order.md Trap 1 currently allows for.
    """
    index = symbol_index()
    out = OrderedDict()

    for mangled, kind in marks.items():
        if kind != "EXPORT":
            continue
        s = index.get(mangled)
        if s is None or s["kind"] not in ("constructor", "destructor") or not s.get("class"):
            continue
        out.setdefault(s["class"], []).append(mangled)

    return out


Plan = namedtuple("Plan", "handover skip problems warnings companion_procs orphans externs")


def plan(marks=None, lines=None, classes=None):
    """Work out which vftables to hand over, and everything that must happen with them.

    `classes` forces a set of class names, for driving this by hand; otherwise the decision comes
    from the headers.
    """
    marks = marks if marks is not None else scan_headers()
    if lines is None:
        lines = read_asm()[0]

    vtables = scan_vtables(lines)
    index = symbol_index()

    if classes is None:
        wanted = claimed_classes(marks)
    else:
        wanted = OrderedDict((c, ["(forced)"]) for c in classes)

    handover, skip, problems, warnings = [], [], [], []
    companion, orphans, externs = [], [], []

    for cls in wanted:
        names = vftables_of(cls)

        if not names:
            warnings.append("%s: exported ctor/dtor but no vftable in .rdata - "
                            "nothing to hand over, and nothing will collide" % cls)
            continue

        # The five multiple-inheritance classes carry two tables. MSVC emits both from the same
        # constructor, so they must move together or the second one duplicates.
        live = [n for n in names if not vtables[n].stripped]
        if not live:
            skip.extend(names)
            continue

        ok = True
        for name in live:
            v = vtables[name]

            if v.label_line is None:
                problems.append("%s: no `%s LABEL BYTE` in the assembly" % (cls, name))
                ok = False
                continue

            if v.public_line is None:
                problems.append("%s: `%s` has no PUBLIC line above its label; refusing to touch a "
                                "block that is not the shape ExportAsm emits" % (cls, name))
                ok = False
                continue

            if v.slots == 0:
                problems.append("%s: `%s` has zero parsed slots" % (cls, name))
                ok = False
                continue

            body = v.slots * 4
            inside = [(ln, off) for ln, off in v.refs if off and off < body]
            if inside:
                # None exist in this binary. If one ever appears it means something takes the
                # address of a slot, and that reference must follow the C++ table, not the dead
                # one - which this transformation cannot express. Refuse rather than guess.
                problems.append("%s: `%s` has %d reference(s) INTO the table body (offsets %s); "
                                "cannot hand over" %
                                (cls, name, len(inside), sorted(set(o for _, o in inside))[:5]))
                ok = False

        if not ok:
            continue

        handover.extend(live)
        externs.extend(live)

        # --- companion symbols the compiler will also emit under names the assembly already owns
        #
        # A virtual destructor does not put ~C in the vtable, it puts a compiler-generated
        # DELETING destructor there. MSVC generates `??_G<C>` (scalar); it generates `??_E<C>`
        # (vector) only where delete[] is used on the type. So `??_G<C>` will collide if the
        # assembly still defines it, and must be stripped as a PROC alongside the table.
        for s in index.values():
            if s.get("class") != cls or not s["code"]:
                continue
            if s["mangled"].startswith("??_G"):
                companion.append(s["mangled"])
            elif s["mangled"].startswith("??_E"):
                orphans.append(s["mangled"])
            elif (s.get("demangled") or "").startswith("[thunk]:"):
                # A multiple-inheritance adjustor thunk - the little `sub ecx, 180; jmp Real`
                # stub that fixes `this` when a call comes in through a secondary vtable. The
                # compiler regenerates these from the class definition under the same mangled
                # name, so they collide exactly like the table does.
                #
                # Detected from the demangled text rather than the mangled one: the mangled
                # marker `@@W` also appears in the middle of a signature whenever a parameter is
                # an enum (asCamera::GetNearClip takes a `W4kNearClip@1@`), and matching that
                # would strip six functions that are not thunks at all.
                companion.append(s["mangled"])

        # --- fidelity warnings, none of which the linker will tell you about
        prim = primary_vftable(cls)
        if prim and not vtables[prim].stripped:
            slot0 = vtables[prim].targets[0] if vtables[prim].targets else None
            if slot0 and slot0.startswith("??_E"):
                warnings.append(
                    "%s: slot 0 holds %s (VECTOR deleting destructor). MSVC will emit ??_G "
                    "(scalar) instead. Identical for delete, different for delete[] - confirm "
                    "nothing array-deletes this type." % (cls, slot0))

            pures = [i for i, t in enumerate(vtables[prim].targets) if "purecall" in t]
            if pures:
                warnings.append("%s: slots %s are __purecall - those virtuals must be declared "
                                "= 0 or the slots will not match" % (cls, pures))

            aw = [t for t in vtables[prim].targets if t == "?GetClassNameA@asNode@@UAEPADXZ"]
            if aw:
                warnings.append(
                    "%s: slot holds ?GetClassNameA@asNode@@UAEPADXZ, the windows.h macro "
                    "expansion of GetClassName. Declare ONE virtual GetClassName; declaring both "
                    "adds a slot and shifts everything after it. The two functions are "
                    "byte-identical, so collapsing them changes no behaviour." % cls)

    # Anything still assembly stays put.
    for name, v in vtables.items():
        if name not in handover and not v.stripped:
            skip.append(name)

    return Plan(handover=handover, skip=skip, problems=problems, warnings=warnings,
                companion_procs=sorted(set(companion)), orphans=sorted(set(orphans)),
                externs=sorted(set(externs)))


# ---------------------------------------------------------------------------------------------
# rewriting


def apply(lines, p, vtables=None):
    """Rewrite the assembly for every vftable in the plan.

    Returns (lines, extern_names). The line COUNT is unchanged and the emitted byte count is
    unchanged - nothing is inserted, deleted or padded, so no address in .rdata moves. That is
    the whole reason this is safe on a binary with no relocation table.
    """
    vtables = vtables if vtables is not None else scan_vtables(lines)
    out = list(lines)

    for name in p.handover:
        v = vtables[name]
        new = label_for(v.rva)

        # The PUBLIC line is commented rather than deleted so the line count holds and the change
        # is legible in a diff. Left as-is it would declare a symbol this module no longer
        # defines while game_externs.inc declares the same name EXTERN, which MASM rejects.
        out[v.public_line] = ("; PUBLIC %s   ; -> C++; see EXTERN in game_externs.inc" % name)
        out[v.label_line] = ("%s LABEL BYTE   ; was %s, %d slots, kept as dead data because "
                             "unnamed .rdata is anchored to it" % (new, name, v.slots))

        for ln, addend in v.refs:
            if not addend:
                continue                     # a real vptr reference: let it bind to the EXTERN
            d = DD_OFFSET.match(out[ln])
            if not d or d.group(2) != name:
                continue
            out[ln] = "%sdd offset %s + %d" % (d.group(1), new, addend)

    return out, list(p.externs)


Handover = namedtuple("Handover", "lines externs strip_procs report ok plan")


def hand_over(lines, marks, classes=None):
    """THE integration point. One call, from tools/asm.py, does the whole vtable side.

        h = asm_vtables.hand_over(lines, marks)

    `lines`   the assembly, already split on newlines - exactly what asm.py's read_procs returns.
    `marks`   {mangled: 'IMPORT'|'EXPORT'}, exactly what asm.py's scan_headers returns. Which
              classes to hand over is DERIVED from it: a class's vtable moves when a constructor
              or destructor of that class is ARTS_EXPORT, because those are the only functions
              that make MSVC emit `??_7C@@6B@`. There is no second list to keep in sync.
    `classes` overrides that, for driving this by hand.

    Returns a Handover:

    `.lines`        the rewritten assembly. Same number of lines and the same number of emitted
                    bytes as the input - asserted, not assumed - so every index asm.py already
                    computed against `lines` is still valid, and no address in .rdata moves.
    `.externs`      vftable symbols to declare `EXTERN <name>:BYTE`. Pass to add_externs().
    `.strip_procs`  a set of extra mangled PROC names asm.py must ALSO strip and declare
                    EXTERN:PROC. These are the symbols MSVC regenerates from the class definition
                    alone - the `??_G` scalar deleting destructor and the multiple-inheritance
                    adjustor thunks - which are declared in no header and which asm.py would
                    therefore never find on its own. Leave one behind and the link fails with a
                    duplicate symbol; that at least is loud.
    `.report`       what happened, for printing.
    `.ok`           False if anything was refused. asm.py should stop rather than write.

    WHERE THIS GOES IN asm.py

        marks = scan_headers(CODE)
        lines, procs = read_procs(ASM)

        h = asm_vtables.hand_over(lines, marks)      # <-- here, and only here
        print(h.report)
        if not h.ok:
            return 1
        lines = h.lines

        strip = [m for m in exports if m in procs]
        strip += [m for m in h.strip_procs if m in procs and m not in strip]
        ... asm.py's existing padding loop, unchanged, over `lines` ...

        # after writing the assembly:
        asm_vtables.add_externs(h.externs)

    The ordering is not a preference. asm.py's `procs` map holds line indices taken from `lines`,
    and its padding loop replaces an n-line PROC with a 2-line comment, so the moment it runs
    every index after the first stripped function is wrong. hand_over() is index-preserving by
    construction, so it is the only one of the two that can go first. Running it second means
    rescanning, and rescanning a half-rewritten file is how a tool like this starts guessing.
    """
    p = plan(marks, lines, classes=classes)
    report = [describe(p)]

    if p.problems:
        return Handover(lines=lines, externs=[], strip_procs=set(),
                        report="\n".join(report), ok=False, plan=p)

    if not p.handover:
        return Handover(lines=lines, externs=[], strip_procs=set(),
                        report="\n".join(report), ok=True, plan=p)

    out, externs = apply(lines, p)

    before, after = verify_byte_count(lines, out)
    report.append("")
    report.append("emitted bytes: %d -> %d (%s)"
                  % (before, after, "unchanged" if before == after else "CHANGED"))
    if before != after or len(out) != len(lines):
        report.append("refusing: .rdata layout would move")
        return Handover(lines=lines, externs=[], strip_procs=set(),
                        report="\n".join(report), ok=False, plan=p)

    return Handover(lines=out, externs=externs, strip_procs=set(p.companion_procs),
                    report="\n".join(report), ok=True, plan=p)


def read_asm(path=ASM):
    if not os.path.exists(path):
        sys.exit("no assembly at %s - run ExportAsm.java first" % path)
    with open(path, encoding="utf-8", errors="replace") as f:
        return f.read().split("\n"), path


def add_externs(names, path=EXTERNS):
    """Declare each handed-over vftable EXTERN so the remaining assembly still reaches it.

    `:BYTE` is the form ExportAsm reserves for external data (ExportAsm.java:172-174), though its
    `externData` list is empty today, so this is the first thing to exercise it. What IS proven in
    this build is the construct that matters: `dd offset <extern>` assembles and relocates
    correctly, 779 times over, for the `:DWORD` import externs. `:BYTE` rather than `:DWORD`
    because a vftable is an array, not a scalar - `offset` yields the same address either way, and
    the byte type stops MASM from accepting a scalar load of it by accident.
    """
    existing = set()
    if os.path.exists(path):
        with open(path, encoding="utf-8", errors="replace") as f:
            for line in f:
                m = re.match(r"EXTERN\s+(\S+):", line)
                if m:
                    existing.add(m.group(1))

    added = [n for n in names if n not in existing]
    if added:
        with open(path, "a", encoding="utf-8", newline="\n") as f:
            f.write("\n; vftables defined by the C++ side\n")
            for n in sorted(added):
                f.write("EXTERN %s:BYTE\n" % n)

    return added


# ---------------------------------------------------------------------------------------------
# the slot manifest - what the header has to say


def base_of(cls):
    """Immediate base, as data/vtable_order.json resolved it.

    That file already reconciles tools/vtables.py's byte-level inference with MM2Hook's stated
    bases and prefers a base that actually has a table, since slot indices are only meaningful
    against one. Duplicating the rule here is how the two drifted apart the first time.
    """
    e = order_of(cls)
    if e and e.get("base"):
        return e["base"]
    h = load(HIERARCHY, "run tools/vtables.py").get(cls) or {}
    return h.get("base") or (load(MM2TYPES, "run tools/mm2types.py").get("bases") or {}).get(cls)


def vtable_length(cls):
    """How many slots `cls` has, and where that number came from.

    This is the number that decides where a derived class's OWN virtuals start, so a wrong answer
    misplaces every one of them.

    Preferred: the table emitted in .rdata. Only three classes ever named as an immediate base
    have no table of their own - asCullable (the root of 121 classes), dgBangerInstance and
    lvlLevelBound - because nothing instantiates them, so the compiler never emitted one. MM2Hook
    supplies all three. There is no case left where the length is a guess.
    """
    e = order_of(cls)
    if e:
        return e["slot_count"], "emitted table"

    mm2 = (load(MM2TYPES, "run tools/mm2types.py").get("vtables") or {}).get(cls)
    if mm2:
        return max(x["slot"] for x in mm2) + 1, "MM2Hook _vtbl"

    return 0, "UNKNOWN - the base's slot count is a guess and the whole layout below it is unsafe"


# Two classes in the binary whose recovered table must not be taken at face value. Both are
# established in docs/vtable_order.md; repeated here because this is where a porter will meet them.
ANOMALIES = {
    "dgUnhitBangerInstance":
        "`??_7dgUnhitBangerInstance@@6B@` is a MISLABELLED symbol: the 28-slot table under that "
        "name is dgBangerInstance's. Every one of the nine classes deriving from "
        "dgUnhitBangerInstance holds ?Reset@dgUnhitBangerInstance@@ at slot 0 and "
        "?Init@dgUnhitBangerInstance@@ at slot 17, and this table holds lvlInstance's. Take the "
        "order from dgUnhitMtxBangerInstance's 30-slot table instead.",
    "vehSuspension":
        "slot 6 is named ?Copy@vehSuspension@@QAEXABV1@@Z, which is not virtual and cannot be in "
        "a vtable. There is no ?FileIO@vehSuspension@@ symbol at all: the two compiled to "
        "identical bytes and the linker folded them. Declare slot 6 as an override of "
        "asNode::FileIO.",
    "lvlInstance":
        "?DrawPhysics@lvlInstance@@UAEXABVVector3@@@Z is a public virtual that occupies no slot "
        "in any of the 340 tables. Declaring it `virtual` would add a 27th slot and shift every "
        "descendant. Declare it non-virtual, or leave it out.",
    "mmMultiCircuit":
        "slot 0 is the only code pointer in .rdata that midtown2.map does not name. It is the "
        "deleting destructor; declare `virtual ~mmMultiCircuit()` and it fills itself.",
}


def slot_manifest(cls, lines=None):
    """The virtual declaration order the binary requires for `cls`, slot by slot.

    Slot order is not a style question. MSVC lays a vtable out as [base's slots][this class's new
    virtuals in declaration order], with an override reusing the base's slot. The generated
    headers sort methods alphabetically, which for asNode produces a 14-slot table in the wrong
    order against the 13-slot table the binary has. Nothing about that fails to compile.

    Everything here comes from data/vtable_order.json, which read midtown2.exe directly. `lines`
    is accepted so a caller can confirm the assembly still holds the table it is about to hand
    over; it is not used to derive the order.
    """
    e = order_of(cls)
    if e is None:
        return None

    index = symbol_index()
    base = e.get("base")
    base_slots, base_src = vtable_length(base) if base else (0, "no base")

    mm2 = (load(MM2TYPES, "run tools/mm2types.py").get("vtables") or {}).get(cls) or []
    mm2_by_slot = {x["slot"]: x["name"] for x in mm2}

    rows = []
    for slot in e["slots"]:
        i = slot["index"]
        target = slot.get("symbol")
        owner = slot.get("owner")
        method = slot.get("name") or target or "<unnamed>"
        dtor = bool(target and target.startswith(("??_E", "??_G"))) or \
            "deleting destructor" in method

        if dtor:
            method = "~%s" % (owner or cls)
        elif slot["kind"] == "pure":
            # A pure virtual resolves to __purecall and the binary cannot name it. MM2Hook can,
            # and printing "__purecall" eleven times in a row as the required declaration order is
            # useless to the person who has to write the header.
            method = mm2_by_slot.get(i) or "<pure, unnamed - read the pseudocode>"

        rows.append(OrderedDict(
            slot=i,
            target=target,
            owner=owner,
            method=method,
            dtor=dtor,
            inherited=(i < base_slots),
            overrides=(slot["kind"] == "override"),
            pure=(slot["kind"] == "pure"),
            unnamed=(slot["kind"] == "unnamed"),
            mm2hook=mm2_by_slot.get(i),
            agrees=(mm2_by_slot.get(i) is None or slot["kind"] == "pure"
                    or mm2_by_slot[i] == method),
        ))

    in_asm = None
    if lines is not None:
        v = scan_vtables(lines).get(e["vftable_symbol"])
        in_asm = None if v is None else (0 if v.stripped else v.slots)

    return OrderedDict(cls=cls, vftable=e["vftable_symbol"], rva=e["vftable_rva"],
                       slots=e["slot_count"], base=base, base_slots=base_slots,
                       base_source=base_src,
                       secondary=[x["vftable_symbol"] for x in (e.get("secondary_vftables") or [])],
                       rows=rows, in_asm=in_asm, anomaly=ANOMALIES.get(cls))


# ---------------------------------------------------------------------------------------------
# reading the header back
#
# The manifest says what the header must contain. This says what it does contain. Everything
# between those two is a silent miscompile, so the comparison is worth having mechanised.


HDR_CLASS = re.compile(r"^class\s+(\w+)\b")
HDR_END = re.compile(r"^\};")


def header_virtuals(cls, root=CODE):
    """[mangled] for every virtual declared in `cls`'s own class body, IN FILE ORDER.

    The generated headers carry each declaration's mangled symbol in the comment directly above
    it, which is what makes this exact rather than a signature-matching exercise. A declaration
    whose line contains `virtual` is a virtual; anything else is not.
    """
    for dirpath, _, files in os.walk(root):
        for fname in sorted(files):
            if not fname.endswith(".h"):
                continue

            path = os.path.join(dirpath, fname)
            with open(path, encoding="utf-8", errors="replace") as f:
                text = f.read()

            for m in re.finditer(r"^class\s+(\w+)\s*(?::[^\n{]*)?$", text, re.M):
                if m.group(1) != cls:
                    continue

                body = text[m.end():]
                stop = HDR_END.search(body, re.M) if False else re.search(r"^\};", body, re.M)
                if stop:
                    body = body[:stop.start()]

                out = []
                pending = None
                for line in body.split("\n"):
                    k = MANGLED.match(line)
                    if k:
                        pending = k.group(1)
                        continue
                    if pending and line.strip():
                        if re.search(r"\bvirtual\b", line):
                            out.append((pending, line.strip()))
                        pending = None
                return out, path

    return None, None


def check_order(cls):
    """Would the header, as written, produce the vtable the binary has?

    The rule MSVC follows: the table is [base's slots][this class's new virtuals in DECLARATION
    order]. Overriding a base virtual reuses the base's slot and may be declared anywhere.
    Declaring a virtual the binary does not have appends a slot and shifts nothing - which sounds
    harmless and is not, because the C++ table is then longer than every derived table still in
    assembly, and any code that indexes past the original end reads a neighbouring vtable.

    So there are exactly three ways to fail, and all three are checked:

      1. a new virtual missing from the header      -> the table is too short, slots shift
      2. a virtual in the header that has no slot   -> the table grows a slot that must not exist
      3. the new virtuals in the wrong order        -> same length, wrong dispatch, no diagnostic

    Returns (ok, [message]).
    """
    man = slot_manifest(cls)
    if man is None:
        return True, ["%s has no vftable; nothing to check" % cls]

    decls, path = header_virtuals(cls)
    if decls is None:
        return False, ["no header declares `class %s`" % cls]

    msgs = ["%s  (%s)" % (cls, path)]

    # what the binary requires, in order: the slots at or past the end of the base's table
    want = [r for r in man["rows"] if not r["inherited"]]

    # the destructor slot is filled by declaring `virtual ~C()`, whose symbol is ??1C@@..., while
    # the slot itself holds the compiler-generated ??_E / ??_G deleting destructor
    def key(mangled):
        if mangled.startswith("??1"):
            return "~"
        return mangled

    # A slot that cannot be identified by symbol cannot be checked mechanically, and there are
    # exactly two kinds: a pure virtual, which resolves to __purecall and so carries no name at
    # all, and the one slot in the binary the map does not name. Both still occupy an index, so
    # they are reported for the porter to place by hand rather than silently skipped.
    want_keys = []
    blind = []
    for r in want:
        if r["dtor"]:
            want_keys.append("~")
        elif r["pure"] or r["unnamed"] or not r["target"]:
            want_keys.append(None)
            blind.append(r)
        else:
            want_keys.append(r["target"])

    # Every slot the class may legally declare without adding one: anything it inherits (an
    # override reuses the base's slot), and - for the five multiple-inheritance classes -
    # anything in a SECONDARY table. vehTrailer declares Reset, Save, Load, FileIO, GetClassName
    # and GetDirName once each; they occupy slots in its asNode subobject table, not its primary.
    # Checking only the primary table reported all six as extra virtuals that would grow it.
    overridable = set()

    def allow(target):
        if not target:
            return
        overridable.add(target)
        sym = symbol_index().get(target)
        if sym and sym.get("class"):
            # an override is declared under the DERIVED class's own mangled name
            overridable.add(target.replace("@%s@@" % sym["class"], "@%s@@" % cls, 1))

    for r in man["rows"]:
        if r["inherited"]:
            allow(r["target"])

    entry = order_of(cls) or {}
    for sec in entry.get("secondary_vftables") or []:
        for slot in sec["slots"]:
            allow(slot.get("symbol"))

    got_keys = []
    stray = []
    for mangled, line in decls:
        k = key(mangled)
        if k in want_keys:
            got_keys.append(k)
        elif mangled in overridable or k == "~":
            continue                          # overriding a base slot: legal, adds nothing
        else:
            stray.append((mangled, line))

    ok = True

    if got_keys != [k for k in want_keys if k is not None]:
        ok = False
        msgs.append("  DECLARATION ORDER IS WRONG. Required, in this order:")
        for r in want:
            msgs.append("    [%2d] %s" % (r["slot"], r["method"]))
        msgs.append("  header has:")
        for k in got_keys:
            s = symbol_index().get(k)
            msgs.append("         %s" % (s["name"] if s else k))

    if stray:
        ok = False
        msgs.append("  %d virtual(s) declared that occupy NO slot in the binary's table - each "
                    "one adds a slot the original code does not know about:" % len(stray))
        for mangled, line in stray:
            msgs.append("    %s" % mangled)

    declared = [m for m, _ in decls]
    missing = [r for r in want
               if r["target"] and not r["dtor"] and not r["pure"] and not r["unnamed"]
               and r["target"] not in declared]
    if missing:
        ok = False
        msgs.append("  %d required virtual(s) not declared at all:" % len(missing))
        for r in missing:
            msgs.append("    [%2d] %s   %s" % (r["slot"], r["method"], r["target"]))

    if blind:
        msgs.append("  %d slot(s) cannot be checked from the binary alone and must be placed by "
                    "hand, in these positions:" % len(blind))
        for r in blind:
            msgs.append("    [%2d] %s%s" % (
                r["slot"],
                (r["mm2hook"] or "?") if r["pure"] else "unnamed address",
                "   (pure - declare = 0)" if r["pure"] else ""))

    if man["anomaly"]:
        msgs.append("  NOTE: %s" % man["anomaly"])

    if ok:
        msgs.append("  OK - %d new virtual(s) in the order the binary requires, %d inherited slots"
                    " from %s%s" % (len(want), man["base_slots"], man["base"] or "-",
                                    "" if not blind else
                                    " (%d slot(s) unverifiable, see above)" % len(blind)))

    return ok, msgs


def describe_manifest(man):
    if man is None:
        return "no vftable for that class"

    out = []
    out.append("%s  (%s at 0x%06X, %d slots)" % (man["cls"], man["vftable"], man["rva"],
                                                 man["slots"]))
    out.append("base: %s  (%d inherited slots, from %s)"
               % (man["base"] or "-", man["base_slots"], man["base_source"]))
    if man.get("anomaly"):
        out.append("")
        out.append("!! %s" % man["anomaly"])
    if man["secondary"]:
        out.append("also carries: %s  (multiple inheritance - both tables move together)"
                   % ", ".join(man["secondary"]))
    out.append("")
    out.append("  slot  from      method                          target")
    out.append("  ----  --------  ------------------------------  " + "-" * 40)

    for r in man["rows"]:
        where = "override" if r["overrides"] else ("base" if r["inherited"] else "NEW")
        flag = ""
        if r["pure"]:
            flag = "   [pure: declare = 0]"
        elif r.get("unnamed"):
            flag = "   [midtown2.map does not name this address]"
        elif not r["agrees"]:
            flag = "   [MM2Hook says %s]" % r["mm2hook"]
        out.append("  %4d  %-8s  %-30s  %s%s" % (r["slot"], where, r["method"] or "?",
                                                 r["target"], flag))

    new = [r for r in man["rows"] if not r["inherited"]]
    out.append("")
    out.append("Declare in the header, in this order (virtuals only; everything above %d comes "
               "from %s):" % (man["base_slots"], man["base"] or "the class itself"))
    for r in man["rows"]:
        # The deleting-destructor slot is an override of the base's, so it counts as inherited -
        # but `virtual ~C()` still has to be written or the slot fills with the BASE's destructor
        # and the object is destroyed as the wrong type.
        if r["inherited"] and not r["overrides"] and not r["dtor"]:
            continue
        kind = "override" if r["overrides"] else "new"
        if r["dtor"]:
            out.append("    [%2d] %-8s virtual ~%s()   (fills the deleting-destructor slot)"
                       % (r["slot"], kind, man["cls"]))
            continue
        out.append("    [%2d] %-8s virtual %s%s" % (r["slot"], kind, r["method"],
                                                    " = 0" if r["pure"] else ""))
    if not new:
        out.append("    (adds no new virtuals; it only overrides)")

    return "\n".join(out)


# ---------------------------------------------------------------------------------------------
# reporting


def describe(p):
    out = []
    out.append("vftables to hand over : %d" % len(p.handover))
    out.append("vftables left in asm  : %d" % len(p.skip))

    if p.handover:
        out.append("")
        for n in p.handover:
            out.append("  %s" % n)

    if p.companion_procs:
        out.append("")
        out.append("%d companion PROC(s) MUST be stripped with them - the compiler regenerates "
                   "these under the same mangled name:" % len(p.companion_procs))
        for m in p.companion_procs:
            out.append("  %s" % m)

    if p.orphans:
        out.append("")
        out.append("%d symbol(s) become orphaned (still assembled, PUBLIC, referenced by "
                   "nothing). Leave them; stripping them would break the few direct calls that "
                   "exist:" % len(p.orphans))
        for m in p.orphans:
            out.append("  %s" % m)

    if p.warnings:
        out.append("")
        out.append("warnings:")
        for w in p.warnings:
            out.append("  - %s" % w)

    if p.problems:
        out.append("")
        out.append("PROBLEMS - nothing was handed over for these:")
        for w in p.problems:
            out.append("  - %s" % w)

    return "\n".join(out)


def audit(lines=None):
    """Every vftable in the binary against the gates that decide whether it can move."""
    if lines is None:
        lines = read_asm()[0]

    vtables = scan_vtables(lines)
    index = symbol_index()
    hier = load(HIERARCHY, "run tools/vtables.py")
    mm2 = load(MM2TYPES, "run tools/mm2types.py")

    classes = sorted(set(v.cls for v in vtables.values()))
    rows = []

    for cls in classes:
        names = vftables_of(cls)
        prim = primary_vftable(cls)
        v = vtables.get(prim) if prim else None

        # Every table of the class has to be strippable, not just the primary. The five
        # multiple-inheritance classes have no `??_7C@@6B@` at all: MSVC names their tables after
        # the bases, `??_7CMemStream@@6BIStream@@@` and so on.
        shape = all(vtables[n].public_line is not None and vtables[n].slots for n in names)
        inside = any(off and off < (vtables[n].slots * 4)
                     for n in names for _, off in vtables[n].refs)

        # can the declaration order be derived at all?
        anc = set()
        c = cls
        seen = set()
        while c and c not in seen:
            seen.add(c)
            c = base_of(c)
            if c:
                anc.add(c)
        strays = set()
        for n in names:
            for t in vtables[n].targets:
                s = index.get(t)
                o = s.get("class") if s else None
                if o and o != cls and o not in anc:
                    strays.add(o)

        has_ctor = any(m.startswith("??0" + cls + "@@") for m in index)
        rows.append(OrderedDict(
            cls=cls, tables=len(names), slots=sum(vtables[n].slots for n in names),
            primary=bool(prim), shape_ok=shape, refs_into_body=inside, base=base_of(cls),
            order_derivable=(not strays), strays=sorted(strays),
            mm2hook_check=(cls in (mm2.get("vtables") or {})),
            has_ctor=has_ctor,
            pure=any("purecall" in t for n in names for t in vtables[n].targets),
        ))

    return rows


def describe_audit(rows):
    n = len(rows)
    out = []
    out.append("%d classes carry a vftable (%d tables)" % (n, sum(r["tables"] for r in rows)))
    out.append("")
    out.append("  canonical block shape, strippable      : %d" % sum(1 for r in rows if r["shape_ok"]))
    out.append("  references into the table body (fatal) : %d" % sum(1 for r in rows if r["refs_into_body"]))
    out.append("  slot order derivable from the binary   : %d" % sum(1 for r in rows if r["order_derivable"]))
    out.append("  independent MM2Hook _vtbl cross-check  : %d" % sum(1 for r in rows if r["mm2hook_check"]))
    out.append("  has a constructor symbol to port       : %d" % sum(1 for r in rows if r["has_ctor"]))
    out.append("  abstract (a slot is __purecall)        : %d" % sum(1 for r in rows if r["pure"]))
    out.append("  no base resolved                       : %d" % sum(1 for r in rows if not r["base"]))
    out.append("  multiple inheritance, two tables       : %d" % sum(1 for r in rows if r["tables"] > 1))

    bad = [r for r in rows if not r["order_derivable"]]
    if bad:
        out.append("")
        out.append("slot order NOT derivable - a slot belongs to a class that is not a known "
                   "ancestor, so hierarchy.json is missing a link:")
        for r in bad:
            out.append("  %-28s slots from %s%s" % (r["cls"], ", ".join(r["strays"]),
                                                    "   (multiple inheritance: this is the OTHER "
                                                    "base, not a missing link)"
                                                    if r["tables"] > 1 else ""))
        out.append("")
        mi = sum(1 for r in bad if r["tables"] > 1)
        dgb = sum(1 for r in bad if "dgBangerInstance" in r["strays"])
        out.append("%d of these are benign: a multiple-inheritance class necessarily has slots "
                   "from a base that is not on its primary chain. %d are dgBangerInstance, a "
                   "class that emits no vftable of its own and is therefore invisible to a "
                   "byte-level hierarchy walk. See docs/vtable_order.md." % (mi, dgb))

    return "\n".join(out)


# ---------------------------------------------------------------------------------------------


def verify_byte_count(before, after):
    """Assert the rewrite emitted the same number of bytes. Cheap insurance, run in dry runs.

    game.asm is raw bytes with symbol references patched in, so only `db` lists and `dd` words
    contribute - the same rule tools/asm.py counts by.
    """
    def total(lines):
        n = 0
        for line in lines:
            s = line.strip()
            if s.startswith("db "):
                n += s[3:].count(",") + 1
            elif s.startswith("dd "):
                n += 4
        return n

    return total(before), total(after)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--write", action="store_true",
                    help="apply the changes; without it nothing is modified")
    ap.add_argument("--manifest", metavar="CLASS",
                    help="print the virtual declaration order the binary requires")
    ap.add_argument("--check", metavar="CLASS",
                    help="read the generated header back and say whether it would produce the "
                         "vtable the binary has (repeat with --check-all for every class)")
    ap.add_argument("--check-all", action="store_true",
                    help="--check for every class that has a vftable")
    ap.add_argument("--audit", action="store_true",
                    help="every vftable in the binary against the portability gates")
    ap.add_argument("--class", dest="classes", action="append", metavar="CLASS",
                    help="force a class rather than reading the headers (repeatable)")
    ap.add_argument("--verify", action="store_true",
                    help="recount emitted bytes before and after; must be identical")
    args = ap.parse_args()

    lines, path = read_asm()

    if args.manifest:
        print(describe_manifest(slot_manifest(args.manifest, lines)))
        return 0

    if args.check:
        ok, msgs = check_order(args.check)
        print("\n".join(msgs))
        return 0 if ok else 1

    if args.check_all:
        bad = 0
        for cls in sorted(load(VTABLE_ORDER, "run tools/vtable_order.py")):
            ok, msgs = check_order(cls)
            if not ok:
                bad += 1
                print("\n".join(msgs))
                print("")
        print("%d class(es) would emit the wrong vtable" % bad)
        return 1 if bad else 0

    if args.audit:
        print(describe_audit(audit(lines)))
        return 0

    marks = scan_headers()
    p = plan(marks, lines, classes=args.classes)

    print("assembly: %s" % path)
    print(describe(p))

    if not p.handover:
        if not p.problems:
            print("\nnothing to do; no class has an ARTS_EXPORT constructor or destructor")
        return 1 if p.problems else 0

    out, externs = apply(lines, p)

    if args.verify or not args.write:
        a, b = verify_byte_count(lines, out)
        print("\nemitted bytes: %d -> %d (%s)" % (a, b, "unchanged" if a == b else "CHANGED"))
        if a != b:
            print("refusing: .rdata layout would move")
            return 1

    if not args.write:
        print("\ndry run; pass --write to apply")
        seen = scan_vtables(lines)
        for n in p.handover:
            print("\nwould write, for %s:" % n)
            v = seen[n]
            print("  line %d  %s" % (v.public_line + 1, out[v.public_line]))
            print("  line %d  %s" % (v.label_line + 1, out[v.label_line]))
            plus = [ln for ln, off in v.refs if off]
            keep = [ln for ln, off in v.refs if not off]
            print("  %d `+ N` reference(s) repointed at the dead label" % len(plus))
            print("  %d plain reference(s) left to bind to the C++ table" % len(keep))
            print("  EXTERN %s:BYTE" % n)
        return 0

    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(out))

    added = add_externs(externs)
    print("\nhanded over %d vftable(s), added %d EXTERN(s)" % (len(p.handover), len(added)))

    if p.companion_procs:
        print("\nnow strip these PROCs as well (tools/asm.py will not, they are not declared in "
              "any header):")
        for m in p.companion_procs:
            print("  %s" % m)

    return 0


if __name__ == "__main__":
    sys.exit(main())
