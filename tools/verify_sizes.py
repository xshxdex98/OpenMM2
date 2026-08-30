# -*- coding: utf-8 -*-
"""Every class size in data/layouts.json must agree with what midtown2.exe allocates for it.

`check_size` looks like verification and is not. It relates the emitted header to the number
recorded beside it, and BOTH are generated from the same `layouts.json` entry - so it can only
catch a header that has drifted from its own source. It cannot see that the source is wrong.
`check_size(mmInput, 0x1DC)` passed all along against an object that is 0x248 bytes.

This gate closes that loop against the retail binary. The evidence is the strongest size evidence
the file contains:

    push 0x160                  ; <- the 1999 compiler wrote this number
    call ??2@YAPAXI@Z           ; operator new(size_t)
    mov  ecx, eax
    call ??0eqEventHandler@@QAE@XZ

The immediate pushed to `operator new` IS `sizeof(T)`. It was emitted by the compiler, not written
by a human and not inferred by an analyser, and the constructor called immediately afterwards with
that same pointer in `ecx` names the T. Nothing sits between it and the truth.

It says only the total size. It says nothing about what is inside, which is why this is a SIZE gate
and not a layout gate: a disagreement here means our number is wrong, not that we know the members.

### Two rejections, because both produce plausible WRONG numbers

Neither is theoretical; both turned up while calibrating this sweep, and taking either at face
value certifies a size that is confidently incorrect.

**Inlined derived constructors.** When MSVC inlines a derived class's constructor the only `call`
left is the BASE constructor, while the allocation size belongs to the DERIVED class:

    push 0x44
    call ??2lvlInstance@@SAPAXI@Z
    mov  ecx, edi
    call ??0dgUnhitBangerInstance@@QAE@XZ   ; this ctor stores vftable 0x005B56D4
    mov  dword ptr [edi], 0x005B5658        ; ...and then a DIFFERENT one. Not that class.

Read naively this "proves" dgUnhitBangerInstance is 0x44, against 0x18 in two independent sources.
The tell is that second, different vftable stored to [this+0] after the base constructor returns -
the inlined derived body finishing the job. Three sites of this shape would have made `lvlInstance`
0x3C, 0x40 and 0x58 instead of its correct 0x14.

**Array `new`.** `dgRagdollData` does `operator new(0xC0)` and then constructs four `dgLinkData` in
a loop. 0xC0 is `4 * 0x30`, and 0x30 is exactly what layouts.json already says, so the naive
reading inflates the element fourfold. The tell is a backward branch after the constructor call
that re-enters at or before it.

Anything that is not a clean, unambiguous `push <imm>` -> `new` -> ctor is simply not evidence and
is dropped. This gate would rather report 246 classes it is sure of than 400 it is not. For the
same reason, a class whose own clean sites disagree with each other is dropped and named rather
than resolved by majority.

### What a failure means

A class named here is one of:

  - **short** - the shipped game allocates MORE than our struct declares. The still-assembly code
    writes past the end of everything a ported `new` would allocate, and nothing reports it. This
    is the dangerous direction.
  - **over** - our struct declares fields that are not in the object. `gfxTextureMovie` carried
    three of them past the end of a 0x1C object, and its `check_size` passed.

Either way the fix is a deliberate edit to `data/layouts.json` (see `docs/short_layouts.md` and
`docs/layout_sources.md`), never a change to the number in the header. And never invent a member to
make the arithmetic work: a fabricated tiling produces a `check_size` that passes and certifies
nothing, which is worse than having no entry at all.

Run standalone:

    py tools/verify_sizes.py            # summary
    py tools/verify_sizes.py -v         # every disagreement, with its allocation site
    py tools/verify_sizes.py --json     # machine-readable
"""
import argparse
import json
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)

from pe import IMAGE_BASE            # noqa: E402
from verify_data import sections     # noqa: E402

RETAIL = os.environ.get(
    "MM2_EXE",
    os.environ.get("MM2_RETAIL",
                   r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\midtown2.exe"))

# The two allocators that take a real `sizeof` from their caller. `??2lvlInstance` is a bump
# allocator out of the level heap, but its argument is still the compiler's sizeof for the class.
ALLOCATORS = ("??2@YAPAXI@Z", "??2lvlInstance@@SAPAXI@Z")

# Where vftables live in this image. Used only to recognise a vftable STORE, so the bounds are
# deliberately generous - a miss here loses a rejection, which is the safe direction to be wrong in
# only if it is loud, and it is: a lost rejection shows up as a class whose sites disagree.
VFT_LO, VFT_HI = 0x005A0000, 0x00700000

# How far past `call operator new` to look for the constructor, and past the constructor for the
# two rejection tells. Both are generous; the real distances are under a dozen instructions.
WINDOW_CTOR = 48
WINDOW_TELL = 24

# Classes whose measured size this gate has been told not to judge.
#
# Deliberately empty. Every disagreement it reports today is a real one, and an exclusion list is
# how a gate like this quietly stops being a gate. An entry here needs a reason written beside it
# explaining why the binary's own allocation literal is not the last word for that class.
KNOWN_OK = {}


def load_image(path):
    with open(path, "rb") as f:
        data = f.read()
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    if data[pe:pe + 4] != b"PE\0\0":
        sys.exit("not a PE file: %s" % path)
    ns = struct.unpack_from("<H", data, pe + 6)[0]
    tbl = pe + 24 + struct.unpack_from("<H", data, pe + 20)[0]
    secs = []
    for i in range(ns):
        b = tbl + i * 40
        vsize, vaddr, rawsize, rawptr = struct.unpack_from("<IIII", data, b + 8)
        secs.append((vaddr + IMAGE_BASE, vaddr + IMAGE_BASE + max(vsize, rawsize), rawptr))
    return data, secs


def make_reader(data, secs):
    def read(va, n):
        for lo, hi, raw in secs:
            if lo <= va < hi:
                return data[raw + (va - lo):raw + (va - lo) + n]
        return None
    return read


def _slot(ins, op):
    """A stable name for an [ebp+X] / [esp+X] spill slot, or None if it is not one."""
    from capstone import x86
    if op.type != x86.X86_OP_MEM or op.mem.index != 0:
        return None
    if op.mem.base not in (x86.X86_REG_EBP, x86.X86_REG_ESP):
        return None
    return "%s%+d" % (ins.reg_name(op.mem.base), op.mem.disp)


def ctor_vftables(read, md, rva, size):
    """Which vftables does this constructor itself store to [this+0]?

    Only the opening of the function is scanned. MSVC emits the vptr store early, and a store
    further in is as likely to belong to an inlined sub-object as to `this`.
    """
    from capstone import x86
    code = read(rva, min(size or 96, 96))
    if not code:
        return set()
    out = set()
    for ins in md.disasm(code, rva):
        if ins.mnemonic == "ret":
            break
        if ins.mnemonic != "mov" or len(ins.operands) != 2:
            continue
        dst, src = ins.operands
        if (dst.type == x86.X86_OP_MEM and dst.mem.disp == 0 and dst.mem.index == 0
                and src.type == x86.X86_OP_IMM and VFT_LO <= (src.imm & 0xFFFFFFFF) < VFT_HI):
            out.add(src.imm & 0xFFFFFFFF)
    return out


def sweep(read, syms):
    """Every `push <imm>; call operator new; ...; call <ctor>` in the image.

    Returns (sites, stats). A site is a dict naming the class, the size the compiler pushed, and
    the address of the `call operator new` it was pushed to.
    """
    import capstone
    from capstone import x86

    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    by_rva = {s["rva"]: s for s in syms}
    alloc_rvas = {s["rva"]: s["mangled"] for s in syms if s["mangled"] in ALLOCATORS}
    if not alloc_rvas:
        sys.exit("neither operator new symbol is in data/symbols.json - cannot sweep")

    vft_cache = {}

    def vfts_of(sym):
        if sym["rva"] not in vft_cache:
            vft_cache[sym["rva"]] = ctor_vftables(read, md, sym["rva"], sym.get("size"))
        return vft_cache[sym["rva"]]

    sites = []
    stats = dict(allocations=0, not_literal=0, no_ctor=0, array_new=0, inlined_derived=0, clean=0)

    for s in syms:
        if not s.get("code") or not s.get("size"):
            continue
        if s["mangled"] in ALLOCATORS:
            continue

        code = read(s["rva"], s["size"])
        if not code:
            continue

        try:
            ins_list = list(md.disasm(code, s["rva"]))
        except Exception:
            continue

        for i, ins in enumerate(ins_list):
            if ins.mnemonic != "call" or not ins.operands:
                continue
            op = ins.operands[0]
            if op.type != x86.X86_OP_IMM:
                continue
            target = op.imm & 0xFFFFFFFF
            if target not in alloc_rvas:
                continue

            stats["allocations"] += 1

            # --- the size argument -------------------------------------------------------
            # The last `push <imm>` before the call, with no intervening call. Anything else - a
            # register, a computed `n * sizeof`, a memory operand - is not a literal and is not
            # evidence. Most sites in the binary are in that class.
            size_imm = None
            for j in range(i - 1, max(-1, i - 8), -1):
                p = ins_list[j]
                if p.mnemonic == "call":
                    break
                if p.mnemonic == "push":
                    q = p.operands[0]
                    if q.type == x86.X86_OP_IMM and 0 < (q.imm & 0xFFFFFFFF) < 0x100000:
                        size_imm = q.imm & 0xFFFFFFFF
                    break
            if size_imm is None:
                stats["not_literal"] += 1
                continue

            # --- follow the returned pointer to a constructor ----------------------------
            # `new` returns in eax. MSVC copies it around - into another register, into an
            # [ebp+X] spill slot - and eventually into ecx for the __thiscall. Only a ctor called
            # with THAT pointer names the class; a ctor called with anything else is constructing
            # a different object and the size does not belong to it.
            holders = {"eax"}
            ecx_holds = False
            ctor = None
            ctor_at = None

            for j in range(i + 1, min(len(ins_list), i + 1 + WINDOW_CTOR)):
                nx = ins_list[j]

                if nx.mnemonic == "call":
                    t = nx.operands[0] if nx.operands else None
                    tgt = (t.imm & 0xFFFFFFFF) if (t and t.type == x86.X86_OP_IMM) else None
                    cand = by_rva.get(tgt) if tgt else None
                    if (ecx_holds and cand and cand["mangled"].startswith("??0")
                            and not cand["mangled"].endswith("_SEH")):
                        ctor, ctor_at = cand, j
                    # A call ends the window either way: it clobbers eax and ecx, so a constructor
                    # after an unrelated call is not provably running on this pointer.
                    break

                if nx.mnemonic == "mov" and len(nx.operands) == 2:
                    dst, src = nx.operands
                    sname = nx.reg_name(src.reg) if src.type == x86.X86_OP_REG else None
                    if dst.type == x86.X86_OP_REG:
                        dname = nx.reg_name(dst.reg)
                        held = (sname in holders) if sname else (_slot(nx, src) in holders)
                        if held:
                            holders.add(dname)
                            if dname == "ecx":
                                ecx_holds = True
                        else:
                            holders.discard(dname)
                            if dname == "ecx":
                                ecx_holds = False
                    elif dst.type == x86.X86_OP_MEM and sname and sname in holders:
                        slot = _slot(nx, dst)
                        if slot:
                            holders.add(slot)
                    continue

                # Anything else that writes a register invalidates it as a holder. `test`/`cmp`
                # are exempt: MSVC's null check between the allocation and the constructor is
                # `test eax, eax` and it does not disturb the pointer.
                if nx.mnemonic in ("test", "cmp") or nx.mnemonic.startswith("j"):
                    continue
                for r in nx.regs_access()[1]:
                    rn = nx.reg_name(r)
                    if rn in holders:
                        holders.discard(rn)
                        if rn == "ecx":
                            ecx_holds = False

            if ctor is None:
                stats["no_ctor"] += 1
                continue

            cls = ctor.get("class") or ctor["demangled"].split("::")[0]

            # --- rejection 1: array new ---------------------------------------------------
            # A constructor loop: after the ctor call a backward branch re-enters at or before it,
            # so the pushed size is n * sizeof(element), not sizeof(element).
            #
            # The branch target must land BETWEEN the allocation and the constructor. That
            # qualifier is what separates an array `new` from an ordinary single-object `new`
            # sitting inside an ordinary loop - the latter's backward branch goes back past the
            # `call operator new`, because the allocation happens again on every iteration. Without
            # it this rule rejected 37 sites instead of 5, and every one of the extra 32 was a
            # perfectly good size.
            loop = False
            for j in range(ctor_at + 1, min(len(ins_list), ctor_at + 1 + WINDOW_TELL)):
                nx = ins_list[j]
                if nx.mnemonic.startswith("j") and nx.operands \
                        and nx.operands[0].type == x86.X86_OP_IMM:
                    tgt = nx.operands[0].imm & 0xFFFFFFFF
                    if ins.address < tgt <= ins_list[ctor_at].address:
                        loop = True
                        break
            if loop:
                stats["array_new"] += 1
                continue

            # --- rejection 2: an inlined derived constructor -------------------------------
            # The only `call` left is the BASE ctor; the allocation size is the DERIVED class's.
            # Visible as a second, DIFFERENT vftable stored to [this+0] once the base returns.
            own = vfts_of(ctor)
            inlined = False
            for j in range(ctor_at + 1, min(len(ins_list), ctor_at + 1 + WINDOW_TELL)):
                nx = ins_list[j]
                if nx.mnemonic == "call":
                    break
                if nx.mnemonic != "mov" or len(nx.operands) != 2:
                    continue
                dst, src = nx.operands
                if (dst.type == x86.X86_OP_MEM and dst.mem.disp == 0 and dst.mem.index == 0
                        and nx.reg_name(dst.mem.base) in holders
                        and src.type == x86.X86_OP_IMM
                        and VFT_LO <= (src.imm & 0xFFFFFFFF) < VFT_HI
                        and (src.imm & 0xFFFFFFFF) not in own):
                    inlined = True
                    break
            if inlined:
                stats["inlined_derived"] += 1
                continue

            stats["clean"] += 1
            sites.append(dict(cls=cls, size=size_imm, at=ins.address,
                              inside=s["mangled"], ctor=ctor["mangled"],
                              allocator=alloc_rvas[target]))

    return sites, stats


def measured_sizes(sites):
    """class -> (size, n_sites, example), keeping only classes whose sites all agree.

    A class whose clean sites disagree with EACH OTHER is not evidence about anything: one of them
    is a rejection this sweep failed to make. Reporting the majority would be exactly the kind of
    plausible wrong number the rejections exist to keep out, so such a class is dropped and named.
    """
    by_cls = {}
    for s in sites:
        by_cls.setdefault(s["cls"], []).append(s)

    agreed, split = {}, {}
    for cls, ss in by_cls.items():
        vals = sorted(set(x["size"] for x in ss))
        if len(vals) == 1:
            agreed[cls] = (vals[0], len(ss), ss[0])
        else:
            split[cls] = vals
    return agreed, split


def reachable_from_cpp(names):
    """Which mis-sized classes does our own compiled code actually instantiate?

    All 25 disagreements are wrong, but only a few can corrupt memory *today*. The dangerous shape
    is a ported `new X` or `sizeof(X)` where our declared size is smaller than what midtown2.exe
    allocates: the object is short, the still-original assembly writes past its end, and the heap is
    corrupted somewhere else entirely.

    That is not hypothetical - it reached the user. Re-landing 55 ported functions produced exactly
    this, the build printed the warning as advisory, and the game came up with a heap error. A wall
    of 25 advisory lines is easy to walk past; a single line naming the class you just allocated is
    not.
    """
    import re as _re

    hits = {}
    root = os.path.join(ROOT, "code")
    if not os.path.isdir(root):
        return hits

    pattern = _re.compile(
        r"(?:new|sizeof)\s*[\(\s]\s*(" + "|".join(_re.escape(n) for n in names) + r")")

    for dirpath, _, files in os.walk(root):
        for name in files:
            if not name.endswith((".cpp", ".h")):
                continue
            path = os.path.join(dirpath, name)
            try:
                text = io.open(path, encoding="utf-8", errors="replace").read()
            except Exception:
                continue
            for m in pattern.finditer(text):
                rel = path[len(ROOT) + 1:].replace("\\", "/")
                hits.setdefault(m.group(1), set()).add(rel)

    return hits


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument("--json", action="store_true", help="machine-readable, to stdout")
    ap.add_argument("--exe", default=RETAIL)
    args = ap.parse_args()

    if not os.path.exists(args.exe):
        print("  retail binary not found: %s" % args.exe)
        print("  SKIP: nothing to verify against (set MM2_EXE)")
        return 0

    try:
        import capstone  # noqa: F401
    except ImportError:
        print("  capstone is not installed - `py -m pip install capstone`")
        print("  SKIP: cannot disassemble")
        return 0

    with open(os.path.join(ROOT, "data", "symbols.json"), encoding="utf-8") as f:
        syms = json.load(f)
    with open(os.path.join(ROOT, "data", "layouts.json"), encoding="utf-8") as f:
        layouts = json.load(f)

    # `sections()` is how every other verifier opens the retail image; going through it here means
    # this gate fails the same way they do when the file is missing or is not a PE.
    sections(args.exe)
    data, secs = load_image(args.exe)
    read = make_reader(data, secs)

    sites, stats = sweep(read, syms)
    agreed, split = measured_sizes(sites)

    short, over, no_layout = [], [], 0
    for cls, (size, n, site) in sorted(agreed.items()):
        info = layouts.get(cls)
        if not info or not info.get("size"):
            no_layout += 1
            continue
        if cls in KNOWN_OK:
            continue
        declared = info["size"]
        if declared == size:
            continue
        (short if declared < size else over).append((cls, size, declared, n, site))

    if args.json:
        json.dump(dict(
            stats=stats,
            measured={c: v[0] for c, v in sorted(agreed.items())},
            split=split,
            short=[dict(cls=c, real=r, declared=d, sites=n, at=s["at"], inside=s["inside"])
                   for c, r, d, n, s in short],
            over=[dict(cls=c, real=r, declared=d, sites=n, at=s["at"], inside=s["inside"])
                  for c, r, d, n, s in over],
        ), sys.stdout, indent=1)
        sys.stdout.write("\n")
        return 1 if (short or over) else 0

    # Anything our own code instantiates is fatal; the rest is a standing debt.
    dangerous = reachable_from_cpp([c for c, *_ in short]) if short else {}

    print("  operator new sites       : %d" % stats["allocations"])
    print("    size not a literal     : %d" % stats["not_literal"])
    print("    no ctor on the pointer : %d" % stats["no_ctor"])
    print("    array new (rejected)   : %d" % stats["array_new"])
    print("    inlined derived (rej.) : %d" % stats["inlined_derived"])
    print("    clean                  : %d" % stats["clean"])
    print("  classes with a real size : %d  (%d of them are in layouts.json)"
          % (len(agreed), len(agreed) - no_layout))
    if split:
        print("  sites disagree, dropped  : %d" % len(split))
        if args.verbose:
            for cls, vals in sorted(split.items()):
                print("      %-30s %s" % (cls, ", ".join("0x%X" % v for v in vals)))

    if not short and not over:
        print("  PASS: every layouts.json size agrees with what the binary allocates")
        return 0

    if short:
        print("")
        print("  SHORT - the game allocates MORE than layouts.json declares (%d):" % len(short))
        print("    a ported new() under-allocates and the still-assembly code writes past the end")
        for cls, real, decl, n, site in sorted(short, key=lambda x: -(x[1] - x[2])):
            print("    %-28s declared 0x%-6X real 0x%-6X short by 0x%X"
                  % (cls, decl, real, real - decl))
            if args.verbose:
                print("        push 0x%X ; call %s ; call %s" % (real, site["allocator"], site["ctor"]))
                print("        at 0x%08X in %s%s" % (site["at"], site["inside"],
                                                     "" if n == 1 else "  (+%d more sites)" % (n - 1)))

    if over:
        print("")
        print("  OVER - layouts.json declares MORE than the object is (%d):" % len(over))
        print("    the header carries members the object does not have, and check_size passes")
        for cls, real, decl, n, site in sorted(over, key=lambda x: -(x[2] - x[1])):
            print("    %-28s declared 0x%-6X real 0x%-6X over by 0x%X"
                  % (cls, decl, real, decl - real))
            if args.verbose:
                print("        push 0x%X ; call %s ; call %s" % (real, site["allocator"], site["ctor"]))
                print("        at 0x%08X in %s%s" % (site["at"], site["inside"],
                                                     "" if n == 1 else "  (+%d more sites)" % (n - 1)))

    print("")
    print("  FAIL: %d class sizes in data/layouts.json disagree with midtown2.exe"
          % (len(short) + len(over)))
    print("  Fix data/layouts.json, not the number in the header - and never invent a member to")
    print("  make the arithmetic work. See docs/short_layouts.md and docs/layout_sources.md.")
    if dangerous:
        print()
        print("  REACHABLE - our own code allocates a class the binary makes BIGGER:")
        for cls in sorted(dangerous):
            print("    %-28s used in %s" % (cls, ", ".join(sorted(dangerous[cls]))))
        print("  This is the heap-corruption case. The object is short, and the original")
        print("  assembly writes past its end. Fix the layout or do not port that code.")
        return 2

    return 1


if __name__ == "__main__":
    sys.exit(main())
