# -*- coding: utf-8 -*-
"""Recover member layouts from constructors, by disassembling midtown2.exe.

347 classes have no member offsets and cannot be ported without them: the assembly still in the
build reads their fields at fixed byte offsets, so those offsets have to be declared before a
single line of the class can be reimplemented.

A constructor is the best place to find them. It writes the object's members, usually in offset
order, and every store reveals three things at once - the offset, the access width (hence the
type) and the initial value. It also BOUNDS the object: a constructor that initialises up to 0x40
and stops is real evidence about where the class's own fields end, which member accesses
scattered through the rest of the class are not.

Crucially, a constructor cannot suffer the failure that sank the previous attempt
(`docs/inferred_layouts.md`). A base-class METHOD is compiled once and runs on derived instances,
so `mmGame::Init` reading offset 0x7670 may be reading an `mmGameMulti` field through an
`mmGame*`. A base-class CONSTRUCTOR only ever runs on the base sub-object; the derived
constructor calls it and then writes its own fields itself. Every offset here belongs to the
class that wrote it, and the base boundary is not inferred - it is the `call Base::Base` at
`this+0` that opens the function.

The input is `midtown2.exe` itself rather than the RE kit's pseudocode, because:

  - all 665 constructors are in the binary, not the 444 the kit decompiled;
  - the access width is the instruction's operand size, not Hex-Rays' guess at a cast;
  - `this` is tracked explicitly through registers and frame slots, so the pointer-scaling trap
    that produced fabricated offsets last time (`this + 24` is offset 24 or offset 96 depending
    on the declared type of `this`) cannot arise - machine code has no declared types.

Every symbol is resolved BY ADDRESS against `data/symbols.json`, which comes from the retail
linker map. The RE kit mislabels 19 of these constructors and the map is authoritative. Symbols
ending `_SEH` are skipped: they are unwind funclets sharing the constructor's name, and `this` is
not in ecx there. Scanning them was worth about a hundred fabricated offsets.

Output: `data/layouts_from_ctors.json`, graded CONFIRMED / PROBABLE / WEAK.
See `docs/layouts_from_ctors.md` for the method and its measured accuracy.

Regenerate:

    py tools/ctor_layouts.py
"""
import json, os, struct, sys, collections
import capstone
from capstone import x86

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
EXE = os.environ.get("MM2_EXE", r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\midtown2.exe")
IMAGE_BASE = 0x400000


def load():
    with open(EXE, "rb") as f:
        data = f.read()
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    ns = struct.unpack_from("<H", data, pe + 6)[0]
    opt = struct.unpack_from("<H", data, pe + 20)[0]
    tbl = pe + 24 + opt
    secs = []
    for i in range(ns):
        b = tbl + i * 40
        vsize, vaddr, rawsize, rawptr = struct.unpack_from("<IIII", data, b + 8)
        secs.append((vaddr + IMAGE_BASE, vaddr + IMAGE_BASE + max(vsize, rawsize), rawptr))
    return data, secs


DATA, SECS = load()


def read(va, n):
    for lo, hi, raw in SECS:
        if lo <= va < hi:
            off = raw + (va - lo)
            return DATA[off:off + n]
    return None


MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
MD.detail = True

THIS = "THIS"
GPR = {x86.X86_REG_EAX: "eax", x86.X86_REG_ECX: "ecx", x86.X86_REG_EDX: "edx",
       x86.X86_REG_EBX: "ebx", x86.X86_REG_ESP: "esp", x86.X86_REG_EBP: "ebp",
       x86.X86_REG_ESI: "esi", x86.X86_REG_EDI: "edi"}
SUB = {
    x86.X86_REG_AX: "eax", x86.X86_REG_AL: "eax", x86.X86_REG_AH: "eax",
    x86.X86_REG_CX: "ecx", x86.X86_REG_CL: "ecx", x86.X86_REG_CH: "ecx",
    x86.X86_REG_DX: "edx", x86.X86_REG_DL: "edx", x86.X86_REG_DH: "edx",
    x86.X86_REG_BX: "ebx", x86.X86_REG_BL: "ebx", x86.X86_REG_BH: "ebx",
    x86.X86_REG_SI: "esi", x86.X86_REG_DI: "edi",
    x86.X86_REG_SP: "esp", x86.X86_REG_BP: "ebp",
}


def full(r):
    return GPR.get(r) or SUB.get(r)


WRITE_OPS = {"mov", "and", "or", "xor", "add", "sub", "inc", "dec", "not", "neg",
             "shl", "shr", "sar", "adc", "sbb",
             "setne", "sete", "setl", "setg", "setle", "setge", "setb", "seta",
             "setbe", "setae", "sets", "setns"}
FPU_STORE = {"fstp", "fst", "fistp", "fist", "fbstp"}


def analyse(va, size, max_insn=20000):
    code = read(va, size)
    if not code:
        return None

    regs = {n: None for n in ("eax", "ecx", "edx", "ebx", "esi", "edi", "ebp")}
    regs["ecx"] = THIS
    stack = {}
    writes = []
    reads = []
    calls = []
    lea_this = {}
    last_load = {}
    rep_stos = []
    ret_seen = False
    n = 0
    decoded = 0

    for ins in MD.disasm(code, va):
        n += 1
        decoded = ins.address + ins.size - va
        if n > max_insn:
            break
        m = ins.mnemonic
        ops = ins.operands

        if m in ("ret", "retn"):
            ret_seen = True
            continue

        if m == "push":
            continue
        if m == "pop":
            if ops and ops[0].type == x86.X86_OP_REG:
                r = full(ops[0].reg)
                if r:
                    regs[r] = None
                    lea_this.pop(r, None)
            continue

        if m == "call":
            ecx_from = last_load.get("ecx")
            tgt = None
            kind = "indirect"
            if ops and ops[0].type == x86.X86_OP_IMM:
                tgt = ops[0].imm
                kind = "direct"
            elif ops and ops[0].type == x86.X86_OP_MEM:
                kind = "mem"
            off = None
            if regs.get("ecx") == THIS:
                off = 0
            elif "ecx" in lea_this:
                off = lea_this["ecx"]
            calls.append(dict(offset=off, target=tgt, kind=kind, va=ins.address,
                              ecx_from=ecx_from))
            for r in ("eax", "ecx", "edx"):
                regs[r] = None
                lea_this.pop(r, None)
                last_load.pop(r, None)
            continue

        if m.startswith("stos") or m.startswith("rep stos"):
            if regs.get("edi") == THIS or "edi" in lea_this:
                base = 0 if regs.get("edi") == THIS else lea_this.get("edi", 0)
                rep_stos.append(base)
            regs["edi"] = None
            lea_this.pop("edi", None)
            continue

        if ops and ops[0].type == x86.X86_OP_MEM and (m in WRITE_OPS or m in FPU_STORE):
            mem = ops[0].mem
            b = full(mem.base) if mem.base else None
            if mem.index == 0 and b:
                hit = None
                if regs.get(b) == THIS:
                    hit = mem.disp
                elif b in lea_this:
                    hit = lea_this[b] + mem.disp
                if hit is not None and hit >= 0:
                    width = ops[0].size
                    val = None
                    if len(ops) > 1 and ops[1].type == x86.X86_OP_IMM:
                        val = ops[1].imm
                    writes.append(dict(offset=hit, width=width, mnem=m, value=val,
                                       va=ins.address))

        if m == "lea" and ops and ops[0].type == x86.X86_OP_REG and ops[1].type == x86.X86_OP_MEM:
            d = full(ops[0].reg)
            mem = ops[1].mem
            b = full(mem.base) if mem.base else None
            if d:
                if mem.index == 0 and b and regs.get(b) == THIS:
                    if mem.disp == 0:
                        regs[d] = THIS
                        lea_this.pop(d, None)
                    else:
                        regs[d] = None
                        lea_this[d] = mem.disp
                elif mem.index == 0 and b and b in lea_this:
                    regs[d] = None
                    lea_this[d] = lea_this[b] + mem.disp
                else:
                    regs[d] = None
                    lea_this.pop(d, None)
            continue

        if m == "mov" and len(ops) == 2:
            dst, src = ops[0], ops[1]
            if dst.type == x86.X86_OP_REG:
                d = full(dst.reg)
                if d:
                    if src.type == x86.X86_OP_REG and dst.size == 4 and src.size == 4 \
                            and regs.get(full(src.reg)) == THIS:
                        regs[d] = THIS
                        lea_this.pop(d, None)
                    elif src.type == x86.X86_OP_REG and dst.size == 4 \
                            and full(src.reg) in lea_this:
                        regs[d] = None
                        lea_this[d] = lea_this[full(src.reg)]
                    elif src.type == x86.X86_OP_MEM and src.mem.index == 0 \
                            and src.mem.base in (x86.X86_REG_EBP, x86.X86_REG_ESP) \
                            and stack.get((full(src.mem.base), src.mem.disp)) == THIS:
                        regs[d] = THIS
                        lea_this.pop(d, None)
                    else:
                        regs[d] = None
                        lea_this.pop(d, None)
                    last_load.pop(d, None)
                    if src.type == x86.X86_OP_MEM and src.mem.index == 0 and src.mem.base:
                        sb = full(src.mem.base)
                        h = None
                        if regs.get(sb) == THIS:
                            h = src.mem.disp
                        elif sb in lea_this:
                            h = lea_this[sb] + src.mem.disp
                        if h is not None and h >= 0:
                            reads.append(dict(offset=h, width=src.size, va=ins.address))
                            last_load[d] = h
                continue
            if dst.type == x86.X86_OP_MEM and dst.mem.index == 0 \
                    and dst.mem.base in (x86.X86_REG_EBP, x86.X86_REG_ESP):
                key = (full(dst.mem.base), dst.mem.disp)
                if src.type == x86.X86_OP_REG and regs.get(full(src.reg)) == THIS:
                    stack[key] = THIS
                else:
                    stack.pop(key, None)
                continue

        for r in ins.regs_access()[1]:
            f = full(r)
            if f and f in regs:
                regs[f] = None
                lea_this.pop(f, None)

    return dict(writes=writes, reads=reads, calls=calls, rep_stos=rep_stos, insns=n,
                complete=ret_seen, decoded=decoded, size=size)




D = os.path.join(ROOT, "data")

SYMS = json.load(open(os.path.join(D, "symbols.json"), encoding="utf-8"))
LAYOUTS = json.load(open(os.path.join(D, "layouts.json"), encoding="utf-8"))
MM2 = json.load(open(os.path.join(D, "mm2types.json"), encoding="utf-8"))
HIER = json.load(open(os.path.join(D, "hierarchy.json"), encoding="utf-8"))

BYRVA = {s["rva"]: s for s in SYMS}

CTORS = collections.defaultdict(list)
DTORS = collections.defaultdict(list)
CODE = collections.defaultdict(int)
for s in SYMS:
    c = (s.get("class") or "").split("::")[0]
    if not c:
        continue
    if s.get("code"):
        CODE[c] += s.get("size") or 0
    if s["mangled"].endswith("_SEH"):
        continue                      # an SEH funclet, not the function - `this` is not in ecx
    if s["mangled"].startswith("??0"):
        CTORS[c].append(s)
    elif s["mangled"].startswith("??1"):
        DTORS[c].append(s)


def base_of(cls):
    b = MM2["bases"].get(cls)
    if b and b != cls:
        return b
    h = HIER.get(cls) or {}
    b = h.get("base") or h.get("first_base")
    return b if b != cls else None


VFT_LO, VFT_HI = 0x5A0000, 0x700000


def collect(cls, funcs=None):
    """Merge every constructor of cls."""
    funcs = funcs if funcs is not None else CTORS[cls]
    offs = {}
    subobjects = {}
    basecalls = []
    calls = []
    vptr_vals = set()
    stos = []
    incomplete = []
    for s in funcs:
        r = analyse(s["rva"], s["size"])
        if r is None:
            continue
        if not r["complete"]:
            incomplete.append(s["mangled"])
        for w in r["writes"]:
            e = offs.setdefault(w["offset"], dict(widths=collections.Counter(), values=set(),
                                                  sites=set(), fp=0, n=0))
            e["widths"][w["width"]] += 1
            e["n"] += 1
            if w["value"] is not None:
                e["values"].add(w["value"] & 0xFFFFFFFF)
            if w["mnem"] in ("fstp", "fst"):
                e["fp"] += 1
            e["sites"].add(s["mangled"])
        for c in r["calls"]:
            t = BYRVA.get(c["target"]) if c["target"] else None
            calls.append((c["offset"], t["mangled"] if t else None, s["mangled"]))
            if not t:
                continue
            tc = (t.get("class") or "").split("::")[0]
            if t["mangled"].startswith("??0"):
                if c["offset"] == 0 and tc != cls:
                    basecalls.append(tc)
                elif c["offset"]:
                    subobjects.setdefault(c["offset"], tc)
            elif c["offset"] and tc and tc != cls:
                subobjects.setdefault(c["offset"], tc)
        stos += r["rep_stos"]
    if 0 in offs:
        vptr_vals = set(v for v in offs[0]["values"] if VFT_LO <= v <= VFT_HI)
    return dict(offs=offs, subobjects=subobjects, basecalls=basecalls, calls=calls,
                vptr=bool(vptr_vals), incomplete=incomplete, stos=stos,
                nfuncs=len(funcs))


L, M = LAYOUTS, MM2
OPERATOR_DELETE = {"??3@YAXPAX@Z", "??_V@YAXPAX@Z"}

_ctor = {}
_dtor = {}


def ctor(cls):
    if cls not in _ctor:
        _ctor[cls] = collect(cls) if cls in CTORS else None
    return _ctor[cls]


def dtor(cls):
    """-> dict(reads=Counter, freed=set, subobjects={off:type}, complete=bool)"""
    if cls in _dtor:
        return _dtor[cls]
    reads = collections.Counter()
    freed = set()
    subd = {}
    seen = False
    for s in DTORS.get(cls, []):
        r = analyse(s["rva"], s["size"])
        if not r:
            continue
        seen = True
        for x in r["reads"]:
            reads[x["offset"]] += 1
        for c in r["calls"]:
            t = BYRVA.get(c["target"]) if c["target"] else None
            nm = t["mangled"] if t else None
            if c.get("ecx_from") is not None and (
                    nm in OPERATOR_DELETE or (nm or "").startswith("??1") or c["kind"] == "mem"):
                freed.add(c["ecx_from"])
            if nm and nm.startswith("??1") and c["offset"]:
                tc = (t.get("class") or "").split("::")[0]
                if tc != cls:
                    subd.setdefault(c["offset"], tc)
    _dtor[cls] = dict(reads=reads, freed=freed, subobjects=subd, seen=seen)
    return _dtor[cls]


def align(n, a=4):
    return (n + a - 1) // a * a


# --------------------------------------------------------------------------- references
def stub_layout(cls):
    if cls not in L:
        return False
    ms = [m for m in L[cls]["members"] if m.get("name")]
    return len(ms) <= 1


def ref_size(cls):
    """(size, source) from the two independent references, or (None, note)."""
    a = L[cls]["size"] if (cls in L and not stub_layout(cls)) else None
    b = M["sizes"].get(cls)
    if b is not None and b <= 4:
        b = None                       # MM2Hook declares some classes empty; 1/2/4 is a placeholder
    if a is not None and b is not None:
        return (a, "IDB + MM2Hook agree") if a == b else (None, "IDB 0x%X vs MM2Hook 0x%X" % (a, b))
    if a is not None:
        return a, "IDB type library"
    if b is not None:
        return b, "MM2Hook sizeof"
    return None, None


def base_only_ref(cls, size):
    """True if a reference size is really just sizeof(base) - a body that was never declared."""
    b = base_of(cls)
    if not b:
        return False
    for s in (L[b]["size"] if b in L else None, M["sizes"].get(b)):
        if s is not None and s == size:
            return True
    return False


# --------------------------------------------------------------------------- extents
_extent = {}


def extent(cls, depth=0):
    """max(offset + width) over everything the constructor touches, sub-objects included."""
    if cls in _extent:
        return _extent[cls]
    _extent[cls] = None                # break cycles
    r = ctor(cls)
    if not r:
        return None
    best = 0
    for o, e in r["offs"].items():
        best = max(best, o + max(e["widths"]))
    for o, tc in r["subobjects"].items():
        ts = size_hint(tc, depth + 1)
        best = max(best, o + (ts or 4))
    d = dtor(cls)
    for o in d["reads"]:
        best = max(best, o + 4)
    for o, tc in d["subobjects"].items():
        ts = size_hint(tc, depth + 1)
        best = max(best, o + (ts or 4))
    _extent[cls] = best or None
    return _extent[cls]


def size_hint(cls, depth=0):
    """Best available size for an embedded type, for folding its interior away."""
    s, _ = ref_size(cls)
    if s is not None and not base_only_ref(cls, s):
        return s
    st = STRIDE.get(cls)
    if st:
        return st
    if depth > 3:
        return None
    e = extent(cls, depth)
    return align(e) if e else None


# --------------------------------------------------------------------------- strides
def build_strides():
    """T constructed at 3+ offsets with one constant stride inside one host => sizeof(T)."""
    cands = collections.defaultdict(list)
    for cls in CTORS:
        r = ctor(cls)
        if not r:
            continue
        bytype = collections.defaultdict(list)
        for off, tc in r["subobjects"].items():
            bytype[tc].append(off)
        for tc, offs in bytype.items():
            offs.sort()
            run = [offs[0]]
            for a, b in zip(offs, offs[1:]):
                if len(run) >= 2 and (b - a) != (run[1] - run[0]):
                    if len(run) >= 3:
                        cands[tc].append((cls, run[1] - run[0], len(run)))
                    run = [a]
                run.append(b)
            if len(run) >= 3:
                cands[tc].append((cls, run[1] - run[0], len(run)))
    out = {}
    src = {}
    for tc, lst in cands.items():
        vals = set(s for _, s, _ in lst)
        if len(vals) == 1:
            out[tc] = lst[0][1]
            src[tc] = lst
    return out, src


STRIDE, STRIDE_SRC = build_strides()


# --------------------------------------------------------------------------- ceilings
def build_ceilings():
    derived = collections.defaultdict(list)
    for cls in CTORS:
        r = ctor(cls)
        if not r:
            continue
        for b in set(r["basecalls"]):
            derived[b].append(cls)
    out = {}
    for b, ds in derived.items():
        f = extent(b)
        if not f:
            continue
        best = None
        who = None
        for d in ds:
            rd = ctor(d)
            own = [o for o in rd["offs"] if o >= f] + [o for o in rd["subobjects"] if o >= f]
            if own and (best is None or min(own) < best):
                best, who = min(own), d
        if best is not None:
            out[b] = (best, who, sorted(ds))
    return out


CEIL = build_ceilings()


# --------------------------------------------------------------------------- members
def float_like(v):
    if v is None:
        return False
    v &= 0xFFFFFFFF
    if v in (0, 0xFFFFFFFF):
        return False
    return 0x66 <= ((v >> 23) & 0xFF) <= 0x9C


TYPE_BY_WIDTH = {1: "u8", 2: "u16", 4: "i32", 8: "u64"}


def members_of(cls, base_size, size, floor=None):
    r = ctor(cls)
    d = dtor(cls)
    sub = dict(r["subobjects"])
    for o, tc in d["subobjects"].items():
        sub.setdefault(o, tc)

    occupied = {}
    for o, tc in sub.items():
        occupied[o] = (tc, size_hint(tc))

    entries = {}
    for o, e in r["offs"].items():
        entries[o] = dict(offset=o, widths=e["widths"], values=e["values"], fp=e["fp"],
                          sites=len(e["sites"]), src=["ctor"])
    for o, n in d["reads"].items():
        if o in entries:
            entries[o]["src"].append("dtor")
        else:
            entries[o] = dict(offset=o, widths=collections.Counter({4: n}), values=set(), fp=0,
                              sites=0, src=["dtor"])
    for o in sub:
        entries.setdefault(o, dict(offset=o, widths=collections.Counter({4: 1}), values=set(),
                                   fp=0, sites=1, src=["ctor"]))

    # anything strictly inside an embedded object is that object's business, not ours
    drop = set()
    for o, (tc, ts) in occupied.items():
        if ts:
            drop |= set(x for x in entries if o < x < o + ts)
    for o in drop:
        entries.pop(o, None)

    def gap(a, b):
        """An unobserved run [a, b).  Split at the base boundary so it can be declared."""
        cuts = [a, b]
        if base_size and a < base_size < b:
            cuts = [a, base_size, b]
        for x, y in zip(cuts, cuts[1:]):
            out.append(collections.OrderedDict(
                offset=x, offset_hex="0x%X" % x, type="u8", name="field_%X" % x,
                count=y - x, width=1, unknown=True,
                from_base=bool(base_size and x < base_size)))

    out = []
    overlaps = []
    prev_end = 0
    for o in sorted(entries):
        e = entries[o]
        if size is not None and o >= size:
            continue
        if o < prev_end:
            # a write that lands inside a member already emitted - a union, or a byte-wise store
            # into a wider field.  Recording it as a member would break the tiling, so it is kept
            # as a note instead and the offset is still on the record.
            overlaps.append("0x%X w%d inside the member at 0x%X"
                            % (o, max(e["widths"]), out[-1]["offset"] if out else 0))
            continue
        if o > prev_end:
            gap(prev_end, o)
        if o in occupied:
            tc, ts = occupied[o]
            m = collections.OrderedDict(offset=o, offset_hex="0x%X" % o, type=tc,
                                        name="field_%X" % o, count=0, width=ts)
            m["src"] = "constructed in place"
        else:
            w = max(e["widths"])
            t = TYPE_BY_WIDTH.get(w, "u8")
            if w == 4:
                if e["fp"] or any(float_like(v) for v in e["values"]):
                    t = "f32"
                elif o in d["freed"]:
                    t = "void*"
            elif w == 8 and e["fp"]:
                t = "f64"
            nm = "vtable" if (o == 0 and r["vptr"]) else "field_%X" % o
            if o == 0 and r["vptr"]:
                t = "void*"
            m = collections.OrderedDict(offset=o, offset_hex="0x%X" % o, type=t,
                                        name=nm, count=0, width=w)
            vals = sorted(e["values"])
            if len(vals) == 1:
                m["init"] = "0x%X" % vals[0]
            if len(e["widths"]) > 1:
                m["width_conflict"] = sorted(e["widths"])
            m["src"] = "+".join(sorted(set(e["src"])))
            m["ctor_sites"] = e["sites"]
        if base_size and o < base_size:
            m["from_base"] = True
        if o in d["freed"]:
            m["freed_by_destructor"] = True
        out.append(m)
        prev_end = max(prev_end, o + (m["width"] or 4))
    fill = size if size is not None else floor
    if fill is not None and prev_end < fill:
        gap(prev_end, fill)
        prev_end = fill
    return out, prev_end, overlaps


# --------------------------------------------------------------------------- main
def build():
    out = collections.OrderedDict()
    for cls in sorted(CTORS):
        r = ctor(cls)
        if not r or (not r["offs"] and not r["subobjects"]):
            continue
        floor = extent(cls)
        if not floor:
            continue
        afloor = align(floor)

        ref, ref_src = ref_size(cls)
        notes = []
        stub = False
        if ref is not None and floor > ref and base_only_ref(cls, ref):
            notes.append("the %s size 0x%X is only sizeof(%s), a body that was never declared; "
                         "the constructor writes past it" % (ref_src, ref, base_of(cls)))
            ref, ref_src, stub = None, None, True

        st = STRIDE.get(cls)
        ce = CEIL.get(cls)

        # ---- size, and where it came from
        size = None
        size_src = None
        contradiction = False
        if ref is not None:
            if floor > ref:
                contradiction = True
                notes.append("CONTRADICTION: the constructor reaches 0x%X but %s says 0x%X"
                             % (floor, ref_src, ref))
            elif afloor == ref:
                size, size_src = ref, ref_src
                notes.append("constructor extent 0x%X aligns exactly to the %s size 0x%X"
                             % (floor, ref_src, ref))
            else:
                notes.append("constructor extent 0x%X lies inside the %s size 0x%X - the layout "
                             "is real but incomplete" % (floor, ref_src, ref))
        if st is not None:
            hosts = ", ".join("%s x%d" % (h, n) for h, _, n in STRIDE_SRC[cls])
            if afloor == st:
                notes.append("%s constructs %s in a row with a constant stride of 0x%X, which "
                             "equals the constructor extent" % (hosts, cls, st))
                if size is None:
                    size, size_src = st, "array stride in %s" % STRIDE_SRC[cls][0][0]
            elif floor > st:
                contradiction = True
                notes.append("CONTRADICTION: extent 0x%X exceeds the array stride 0x%X in %s"
                             % (floor, st, hosts))
            else:
                notes.append("array stride of 0x%X in %s bounds this class from above" % (st, hosts))
        if ce is not None:
            c, who, ds = ce
            if afloor == c:
                notes.append("derived class %s starts its own fields at 0x%X, exactly the "
                             "constructor extent" % (who, c))
            elif floor <= c:
                notes.append("derived class %s starts its own fields at 0x%X" % (who, c))

        # ---- confidence
        n = len(r["offs"]) + len(r["subobjects"])
        complete = not r["incomplete"]
        if contradiction or not complete:
            conf = "WEAK"
        elif size is not None and n >= 3:
            conf = "CONFIRMED"
        elif n >= 4 and (ref is not None or ce is not None or st is not None or r["basecalls"]):
            conf = "PROBABLE"
        elif n >= 8:
            conf = "PROBABLE"
        else:
            conf = "WEAK"

        base = r["basecalls"][0] if r["basecalls"] else None
        base_size = None
        if base:
            bs, _ = ref_size(base)
            if bs is not None and not base_only_ref(base, bs):
                base_size = bs
            else:
                be = extent(base)
                base_size = align(be) if be else None

        members, _, overlaps = members_of(cls, base_size, size, floor)

        ev = list(notes)
        ev.append("%d offset%s written by %d constructor%s (%s), 0x%X bytes of code"
                  % (len(r["offs"]), "" if len(r["offs"]) == 1 else "s",
                     len(CTORS[cls]), "" if len(CTORS[cls]) == 1 else "s",
                     ", ".join("0x%X" % s["rva"] for s in CTORS[cls]),
                     sum(s["size"] for s in CTORS[cls])))
        if base:
            ev.append("constructor calls %s::%s at this+0, so [0x0, 0x%s) belongs to the base"
                      % (base, base, ("%X" % base_size) if base_size else "?"))
        if r["subobjects"]:
            ev.append("sub-objects constructed in place: " + ", ".join(
                "%s at 0x%X" % (t, o) for o, t in sorted(r["subobjects"].items())))
        d = dtor(cls)
        if d["freed"]:
            ev.append("destructor frees the pointer at " + ", ".join(
                "0x%X" % o for o in sorted(d["freed"])))
        if d["reads"]:
            ev.append("destructor also reads " + ", ".join(
                "0x%X" % o for o in sorted(d["reads"])))
        if r["incomplete"]:
            ev.append("did not disassemble to a ret: " + ", ".join(r["incomplete"]))

        e = collections.OrderedDict()
        e["kind"] = "struct"
        e["size"] = size
        e["size_source"] = size_src
        e["size_lower_bound"] = floor
        e["size_lower_bound_hex"] = "0x%X" % floor
        e["confidence"] = conf
        e["constructors"] = [collections.OrderedDict(
            mangled=s["mangled"], rva="0x%X" % s["rva"], code_size=s["size"])
            for s in CTORS[cls]]
        e["destructors"] = [collections.OrderedDict(
            mangled=s["mangled"], rva="0x%X" % s["rva"], code_size=s["size"])
            for s in DTORS.get(cls, [])]
        e["base"] = base
        e["base_size"] = base_size
        e["polymorphic"] = r["vptr"]
        e["reference"] = collections.OrderedDict(
            idb=L[cls]["size"] if cls in L else None,
            idb_is_stub=stub_layout(cls),
            mm2hook=M["sizes"].get(cls),
            reference_is_base_only=stub)
        e["contradicts_reference"] = contradiction
        e["class_code_bytes"] = CODE[cls]
        e["evidence"] = ev
        if overlaps:
            e["overlapping_writes"] = overlaps
            ev.append("writes that land inside an already-emitted member (union, or a byte-wise "
                      "store into a wider field): " + "; ".join(overlaps))
        e["members"] = members
        out[cls] = e
    return out




def main():
    out = build()
    path = os.path.join(ROOT, "data", "layouts_from_ctors.json")

    meta = collections.OrderedDict()
    meta["_note"] = (
        "Recovered from constructor and destructor machine code. NOT a drop-in replacement for "
        "data/layouts.json: `size` is null unless an independent source corroborates the "
        "constructor's extent, and `size_lower_bound` is a floor, never a size. Never fill "
        "check_size() from size_lower_bound - see docs/layouts_from_ctors.md.")
    meta["generator"] = "tools/ctor_layouts.py"
    meta["classes"] = len(out)
    meta["grades"] = dict(collections.Counter(v["confidence"] for v in out.values()))
    meta["grade_scope"] = (
        "The grade describes the MEMBER evidence, not the size. `size` is set independently, "
        "whenever the constructor's extent was corroborated - so a WEAK entry can still carry a "
        "trustworthy size (17 do: tiny classes where the constructor found only two members but "
        "the extent matched two agreeing references exactly). A contradicted size is never "
        "emitted; those entries have size null and contradicts_reference true.")
    meta["grade_meaning"] = collections.OrderedDict([
        ("CONFIRMED",
         "the constructor disassembled cleanly to a ret, wrote at least three offsets, and its "
         "extent aligns EXACTLY to a size from a source independent of the disassembly - "
         "MM2Hook's sizeof, the IDB type library, or an array stride observed in another class's "
         "constructor. The layout is complete at the top end. Interior gaps are still unknown "
         "and are declared as field_X[n] blocks."),
        ("PROBABLE",
         "a complete constructor with four or more recovered offsets and no contradiction, but "
         "nothing pins the total size. `size` is null and `size_lower_bound` is a floor."),
        ("WEAK",
         "thin evidence, a constructor that did not disassemble to a ret, or a direct "
         "contradiction with a reference size. Read the constructor yourself before using one."),
    ])
    meta["member_fields"] = collections.OrderedDict([
        ("unknown", "true means this entry is a GAP - bytes no constructor touched. Its extent "
                    "is bounded by the two observed offsets around it, but its contents are not "
                    "known. Declared as u8 field_X[n] so the following offsets stay put."),
        ("from_base", "the offset lies below sizeof(base), so the member belongs to the base "
                      "class, not to this one."),
        ("src", "which function saw it: ctor, dtor, both, or 'constructed in place' for a "
                "sub-object identified by a `lea ecx,[this+N]; call Foo::Foo`."),
        ("init", "the immediate the constructor stored, when every constructor stored the same "
                 "one."),
        ("freed_by_destructor", "the destructor passes this field to operator delete or to a "
                                "destructor, so it holds a pointer."),
        ("width_conflict", "the same offset was written at two different widths - a union, a "
                           "sub-field write, or a byte-wise store into a wider member."),
    ])

    body = collections.OrderedDict()
    body["_meta"] = meta
    body.update(out)
    with open(path, "w", encoding="utf-8") as f:
        json.dump(body, f, indent=1)

    c = collections.Counter(v["confidence"] for v in out.values())
    novel = {k: v for k, v in out.items() if k not in L or stub_layout(k)}
    cn = collections.Counter(v["confidence"] for v in novel.values())
    print("wrote %s" % path)
    print("  classes with a usable constructor : %d  %s" % (len(out), dict(c)))
    print("  of which have no real IDB struct  : %d" % len(novel))
    for g in ("CONFIRMED", "PROBABLE", "WEAK"):
        b = sum(v["class_code_bytes"] for v in novel.values() if v["confidence"] == g)
        print("    %-10s %3d classes, %8d bytes of machine code" % (g, cn.get(g, 0), b))


if __name__ == "__main__":
    main()
