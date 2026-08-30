# -*- coding: utf-8 -*-
"""Recover instance layout from Hex-Rays pseudocode, for classes the IDB has no struct for.

tools/layouts.py takes the IDB type dump and gets 187 of our 537 classes. This takes the other
350 from the decompiled bodies instead: every `*(float *)((char *)this + 0x24)` in a class's own
methods is one observed member. The catch is in the name - these are OBSERVATIONS, not a layout.
A member nothing reads is invisible, so the offsets are a subset and max(offset + width) is only
a FLOOR on the instance size. Output goes to data/inferred_layouts.json, nested under a "layouts"
key so it cannot be dropped into a layouts.json consumer by mistake, and every size field is
null. See docs/inferred_layouts.md for why check_size must never be filled from this.
"""
import json, os, re, sys, statistics
from collections import OrderedDict, defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
PSEU = os.environ.get("MM2_PSEUDOCODE",
                      os.path.join(ROOT, "MM2_RE_KIT", "MM2_PSEUDOCODE"))

SCALARS = {
    "unsigned __int8": ("u8", 1), "_BYTE": ("u8", 1), "uint8_t": ("u8", 1),
    "unsigned char": ("u8", 1), "BYTE": ("u8", 1), "UCHAR": ("u8", 1),
    "char": ("i8", 1), "__int8": ("i8", 1), "int8_t": ("i8", 1), "signed char": ("i8", 1),
    "bool": ("u8", 1), "_BOOL1": ("u8", 1),
    "unsigned __int16": ("u16", 2), "WORD": ("u16", 2), "uint16_t": ("u16", 2),
    "unsigned short": ("u16", 2), "USHORT": ("u16", 2), "wchar_t": ("u16", 2),
    "__int16": ("i16", 2), "short": ("i16", 2), "int16_t": ("i16", 2), "_WORD": ("i16", 2),
    "unsigned __int32": ("u32", 4), "DWORD": ("u32", 4), "uint32_t": ("u32", 4),
    "unsigned int": ("u32", 4), "UINT": ("u32", 4), "ULONG": ("u32", 4),
    "unsigned long": ("u32", 4), "_DWORD": ("u32", 4), "_BOOL4": ("b32", 4),
    "int": ("i32", 4), "__int32": ("i32", 4), "int32_t": ("i32", 4), "long": ("i32", 4),
    "LONG": ("i32", 4), "HRESULT": ("i32", 4),
    "float": ("f32", 4), "double": ("f64", 8),
    "unsigned __int64": ("u64", 8), "uint64_t": ("u64", 8), "_QWORD": ("u64", 8),
    "__int64": ("i64", 8), "int64_t": ("i64", 8),
    "_OWORD": ("u128", 16), "__m128": ("u128", 16),
}

with open(os.path.join(ROOT, "data", "layouts.json"), encoding="utf-8") as f:
    LAYOUTS = json.load(f)
with open(os.path.join(ROOT, "data", "symbols.json"), encoding="utf-8") as f:
    SYMS = json.load(f)
with open(os.path.join(ROOT, "data", "hierarchy.json"), encoding="utf-8") as f:
    HIER = json.load(f)
with open(os.path.join(ROOT, "data", "mm2types.json"), encoding="utf-8") as f:
    MM2T = json.load(f)

# MM2Hook's sizeof table. It is INDEPENDENT of the pseudocode, so it grades the inference on 133
# classes the IDB type dump has nothing for. It is not clean ground truth either: a class MM2Hook
# only stubbed out reports 1 or 2 bytes (an empty C++ class), and a partially declared one reports
# the size of the part it declared.
MM2SIZE = MM2T.get("sizes", {})

DERIVED = defaultdict(list)
for _d, _i in HIER.items():
    if _i.get("base"):
        DERIVED[_i["base"]].append(_d)


def ancestor_floor(cls):
    """Nearest ancestor whose size we know: a hard floor, since the base sits at offset 0."""
    seen = set()
    cur = HIER.get(cls, {}).get("base")
    while cur and cur not in seen:
        seen.add(cur)
        if cur in LAYOUTS and LAYOUTS[cur]["size"]:
            return cur, LAYOUTS[cur]["size"]
        cur = HIER.get(cur, {}).get("base")
    return None, 0


def derived_ceiling(cls):
    """Smallest known derived-class size: an advisory ceiling, since C is embedded in D at 0."""
    best = None
    for d in DERIVED.get(cls, ()):
        if d in LAYOUTS and LAYOUTS[d]["size"]:
            if best is None or LAYOUTS[d]["size"] < best[1]:
                best = (d, LAYOUTS[d]["size"])
    return best if best else (None, 0)

OURS = sorted(set(s["class"].split("::")[0] for s in SYMS if s.get("class")))

NMETH = defaultdict(int)
for s in SYMS:
    if s.get("class"):
        NMETH[s["class"].split("::")[0]] += 1


def strip_quals(t):
    t = re.sub(r"\b(const|volatile|struct|union|enum|class)\b", " ", t)
    return re.sub(r"\s+", " ", t).strip()


def sizeof(base, stars):
    """Bytes for base with `stars` levels of pointer. None if unknown."""
    if stars > 0:
        return 4  # 32-bit target
    base = strip_quals(base)
    if base in SCALARS:
        return SCALARS[base][1]
    if base in ("void", ""):
        return None
    if base in LAYOUTS:
        return LAYOUTS[base]["size"] or None
    return None


def split_type(text):
    """MM2::asNode * -> (MM2::asNode, 1).  _DWORD ** -> (_DWORD, 2)."""
    text = text.strip()
    stars = 0
    while text.endswith("*"):
        stars += 1
        text = text[:-1].strip()
    return strip_quals(text), stars


def prim(width, floaty):
    if width == 1:
        return "u8"
    if width == 2:
        return "u16"
    if width == 4:
        return "f32" if floaty else "u32"
    if width == 8:
        return "f64" if floaty else "u64"
    if width == 16:
        return "u128"
    return None


# ---------------------------------------------------------------- file parsing
SPLIT = re.compile(r"^// -{20,}\s*$", re.M)


def first_param(sig):
    """(name, base_type, stars) of the first parameter, or None."""
    i = sig.find("(")
    if i < 0:
        return None
    depth = 0
    j = i
    plist = None
    while j < len(sig):
        c = sig[j]
        if c == "(":
            depth += 1
        elif c == ")":
            depth -= 1
            if depth == 0:
                plist = sig[i + 1:j]
                break
        j += 1
    if plist is None:
        return None
    plist = plist.strip()
    if not plist or plist == "void":
        return None
    d = a = 0
    cut = len(plist)
    for k, c in enumerate(plist):
        if c in "([":
            d += 1
        elif c in ")]":
            d -= 1
        elif c == "<":
            a += 1
        elif c == ">":
            a -= 1
        elif c == "," and d == 0 and a == 0:
            cut = k
            break
    p = plist[:cut].strip()
    p = re.sub(r"\[[^\]]*\]$", "", p).strip()
    m = re.search(r"([A-Za-z_]\w*)$", p)
    if not m:
        return None
    name = m.group(1)
    rest = p[:m.start()].strip()
    if not rest:
        return None
    base, stars = split_type(rest)
    return name, base, stars


def blocks(path):
    with open(path, encoding="utf-8", errors="replace") as f:
        text = f.read()
    for blk in SPLIT.split(text):
        blk = blk.strip("\n")
        if not blk.strip():
            continue
        lines = blk.split("\n")
        hdr = [l for l in lines if l.startswith("//")]
        demangled = None
        for l in hdr:
            s = l[2:].strip()
            if s.startswith("ea=") or s.startswith("?"):
                continue
            if "::" in s or "(" in s:
                demangled = s
                break
        bi = None
        for k, l in enumerate(lines):
            if l.strip() and not l.lstrip().startswith("//"):
                bi = k
                break
        if bi is None:
            continue
        rest = "\n".join(lines[bi:])
        bo = rest.find("{")
        if bo < 0:
            continue
        yield demangled, rest[:bo], rest[bo:]


# ---------------------------------------------------------------- access forms
CAST = r"\(\s*([A-Za-z_][A-Za-z0-9_ :]*?(?:\s*\*)*)\s*\*\s*\)"
NUM = r"(0x[0-9A-Fa-f]+|\d+)"


def num(s):
    return int(s, 16) if s.lower().startswith("0x") else int(s)


def elem_step(base, stars):
    """Stride for pointer arithmetic on a `base` + `stars` pointer - PRIMITIVE pointees only.

    A struct-typed stride is refused on purpose. `this[3].float10` with `lvlSky *this` scales to
    3*sizeof(lvlSky)+0x10, which is nonsense for a class only 0x28 bytes long: IDA mistyped the
    variable, and the code is really doing byte arithmetic on some larger object. Hex-Rays reaches
    for `->` or `(char *)this + N` when it genuinely believes the type, and those forms are read
    elsewhere, so refusing struct strides costs almost no coverage and removes the single largest
    source of fabricated offsets found in validation.
    """
    if stars > 1:
        return 4  # pointer-to-pointer: elements are 32-bit pointers
    if stars == 1 and strip_quals(base) in SCALARS:
        return SCALARS[strip_quals(base)][1]
    return None


def scan(body, var, this_base, this_stars, member_index):
    """[(offset, width, floaty, name_or_None, form)] for accesses off `var`."""
    V = re.escape(var)
    out = []

    if this_stars > 0:
        scale = elem_step(this_base, this_stars)
    elif strip_quals(this_base) in SCALARS and SCALARS[strip_quals(this_base)][1] in (4, 8):
        scale = 1  # `int this` -> plain byte arithmetic
    else:
        scale = None

    def cast_wh(c):
        b, st = split_type(c)
        return sizeof(b, st), (st == 0 and strip_quals(b) in ("float", "double"))

    BYTEPTR = r"\(\s*(?:const\s+)?(?:unsigned\s+)?(?:char|_BYTE|__int8)\s*\*\s*\)"

    # 1) *(T *)((char *)v + N)
    for m in re.finditer(r"\*\s*" + CAST + r"\(\s*" + BYTEPTR + r"\s*" + V + r"\s*\+\s*"
                         + NUM + r"\s*\)", body):
        w, fl = cast_wh(m.group(1))
        if w:
            out.append((num(m.group(2)), w, fl, None, "deref"))

    # 2) *(T *)(v + N)
    for m in re.finditer(r"\*\s*" + CAST + r"\(\s*" + V + r"\s*\+\s*" + NUM + r"\s*\)", body):
        w, fl = cast_wh(m.group(1))
        if w and scale:
            out.append((num(m.group(2)) * scale, w, fl, None, "deref"))

    # 3) *(T *)v  -> offset 0
    for m in re.finditer(r"\*\s*" + CAST + r"\s*" + V + r"\b(?!\s*[\[\+\-])", body):
        w, fl = cast_wh(m.group(1))
        if w:
            out.append((0, w, fl, None, "deref"))

    # 4) *((T *)v + N)
    for m in re.finditer(r"\*\s*\(\s*" + CAST + r"\s*" + V + r"\s*\+\s*" + NUM + r"\s*\)", body):
        b, st = split_type(m.group(1))
        step = elem_step(b, st + 1)
        w, fl = cast_wh(m.group(1))
        if w and step:
            out.append((num(m.group(2)) * step, w, fl, None, "deref"))

    # 5) ((T *)v)[N]
    for m in re.finditer(r"\(\s*" + CAST + r"\s*" + V + r"\s*\)\s*\[\s*" + NUM + r"\s*\]", body):
        b, st = split_type(m.group(1))
        step = elem_step(b, st + 1)
        w, fl = cast_wh(m.group(1))
        if w and step:
            out.append((num(m.group(2)) * step, w, fl, None, "deref"))

    # 6) v[N] -- index scaled by the POINTEE width, not by 1
    if this_stars > 0:
        step = elem_step(this_base, this_stars)
        if step:
            fl = (this_stars == 1 and strip_quals(this_base) in ("float", "double"))
            for m in re.finditer(r"(?<![\w\)\]])" + V + r"\s*\[\s*" + NUM + r"\s*\]", body):
                out.append((num(m.group(1)) * step, step, fl, None, "index"))

    # 7) v->name / v->name[N] -- resolved through the DECLARED type of v
    if member_index:
        for m in re.finditer(r"(?<![\w])" + V + r"\s*->\s*([A-Za-z_]\w*)\s*(?:\[\s*" + NUM
                             + r"\s*\])?", body):
            fld = member_index.get(m.group(1))
            if not fld:
                continue
            off, w, ty = fld
            if not w:
                continue
            if m.group(2) is not None:
                off += num(m.group(2)) * w
            out.append((off, w, ty == "f32", m.group(1), "field"))

    return out


def index_members(tname):
    lay = LAYOUTS.get(tname)
    if not lay:
        return None
    idx = {}
    for mem in lay["members"]:
        n = mem.get("name")
        if not n:
            continue
        idx[n] = (mem["offset"], mem.get("width"), mem.get("type"))
    return idx


AUTO = re.compile(r"^(gap|char|byte|word|dword|qword|float|double|pvoid|unk|off|stru|asc|"
                  r"xmmword|field)_?([0-9A-Fa-f]+)$")


def normalise(name, offset):
    m = AUTO.match(name or "")
    if not m:
        return name
    try:
        if int(m.group(2), 16) != offset:
            return name
    except ValueError:
        return name
    return "field_%X" % offset


# ---------------------------------------------------------------- per class
def analyse(cls, allow_field=True, restrict_forms=None):
    path = os.path.join(PSEU, cls, cls + ".c")
    if not os.path.exists(path):
        return None

    obs = defaultdict(lambda: defaultdict(lambda: {"hits": 0, "sites": set(), "floaty": 0,
                                                   "names": defaultdict(int), "forms": set()}))
    this_types = defaultdict(int)
    scanned = used = 0

    for demangled, sig, body in blocks(path):
        scanned += 1
        if demangled and "::" in demangled:
            owner = demangled.split("(")[0].rsplit("::", 1)[0].strip()
            if owner.split("::")[0] != cls:
                continue
        fp = first_param(sig)
        if not fp:
            continue
        name, base, stars = fp
        if "__thiscall" not in sig and name != "this":
            continue
        this_types["%s%s" % (base, "*" * stars)] += 1
        midx = index_members(base) if (stars == 1 and allow_field) else None
        acc = scan(body, name, base, stars, midx)
        if restrict_forms:
            acc = [a for a in acc if a[4] in restrict_forms]
        if acc:
            used += 1
        fid = demangled or ("fn%d" % scanned)
        for off, w, fl, nm, form in acc:
            if off < 0 or off > 0x8000:
                continue
            e = obs[off][w]
            e["hits"] += 1
            e["sites"].add(fid)
            e["forms"].add(form)
            if fl:
                e["floaty"] += 1
            if nm:
                e["names"][nm] += 1

    members = []
    conflicts = []
    lower = 0
    for off in sorted(obs):
        widths = obs[off]
        if len(widths) > 1:
            conflicts.append(OrderedDict(
                offset=off, offset_hex="0x%X" % off, widths=sorted(widths),
                note="read at %s byte widths; both recorded, neither chosen"
                     % "/".join(str(w) for w in sorted(widths))))
        for w in sorted(widths):
            e = widths[w]
            nm = max(e["names"], key=lambda k: e["names"][k]) if e["names"] else None
            nm = normalise(nm, off) if nm else "field_%X" % off
            members.append(OrderedDict(
                offset=off, offset_hex="0x%X" % off,
                type=prim(w, e["floaty"] > 0 and e["floaty"] * 2 >= e["hits"]),
                name=nm, count=0, width=w,
                sites=len(e["sites"]), hits=e["hits"],
                forms=sorted(e["forms"]), conflict=len(widths) > 1))
            lower = max(lower, off + w)

    offs = sorted(obs)

    # A Hex-Rays "Class::Method" is often compiled against a DERIVED instance, so an access can
    # land far past the end of this class. That shows up as a big hole in the offset histogram.
    # Everything past the first hole wider than GAP is quarantined as probably-not-ours.
    GAP = 0x400
    cut = None
    for i in range(len(offs) - 1):
        if offs[i + 1] - (offs[i] + max(obs[offs[i]])) > GAP:
            cut = i + 1
            break
    outliers = ["0x%X" % o for o in offs[cut:]] if cut is not None else []
    dense = 0
    for o in (offs[:cut] if cut is not None else offs):
        dense = max(dense, o + max(obs[o]))
    for mem in members:
        mem["suspect_outlier"] = ("0x%X" % mem["offset"]) in outliers

    # Validation against the 74 classes we can grade showed that offsets seen at only ONE call
    # site carry nearly all the errors: at >=2 independent sites, offsets landing past the real
    # end of the struct drop from 4.1% to 2.2%. So publish a third, stingier floor.
    conf = 0
    for mem in members:
        if mem["sites"] >= 2 and not mem["suspect_outlier"]:
            conf = max(conf, mem["offset"] + mem["width"])

    overlaps = []
    for i, off in enumerate(offs[:-1]):
        end = off + max(obs[off])
        if end > offs[i + 1]:
            overlaps.append(OrderedDict(
                offset=off, offset_hex="0x%X" % off, next_offset_hex="0x%X" % offs[i + 1],
                note="extent runs past the next observed offset (union, sub-field read, "
                     "or a bad inference)"))

    anc, anc_size = ancestor_floor(cls)
    der, der_size = derived_ceiling(cls)

    return OrderedDict(
        INFERRED=True,
        source="hex-rays pseudocode, NOT the IDB type dump",
        kind="struct",
        size=None,
        size_note="UNKNOWN. size_lower_bound is a floor, never a size. Do not use for check_size.",
        size_lower_bound=lower,
        size_lower_bound_hex="0x%X" % lower,
        size_lower_bound_dense=dense,
        size_lower_bound_dense_hex="0x%X" % dense,
        size_lower_bound_confident=conf,
        size_lower_bound_confident_hex="0x%X" % conf,
        size_lower_bound_note="three floors, weakest to strongest evidence: _lower_bound uses "
                              "every offset seen; _dense drops offsets past a >0x400 hole; "
                              "_confident additionally keeps only offsets seen at >=2 call "
                              "sites. NONE of them is the struct size.",
        outlier_offsets=outliers,
        outlier_note="offsets past a hole wider than 0x400; most likely reads into a DERIVED "
                     "object through a base-class method, not members of this class",
        size_known_elsewhere=(OrderedDict(source="data/mm2types.json (MM2Hook sizeof)",
                                          size=MM2SIZE[cls], size_hex="0x%X" % MM2SIZE[cls],
                                          note="a REAL size exists for this class - use it, not "
                                               "the lower bound below")
                              if cls in MM2SIZE else None),
        floor_from_base=(OrderedDict(base=anc, size=anc_size, size_hex="0x%X" % anc_size)
                         if anc else None),
        ceiling_from_derived=(OrderedDict(derived=der, size=der_size, size_hex="0x%X" % der_size)
                              if der else None),
        this_types=OrderedDict(sorted(this_types.items(), key=lambda kv: -kv[1])),
        methods_in_map=NMETH.get(cls, 0),
        methods_in_file=scanned,
        methods_with_this_access=used,
        members=members,
        conflicts=conflicts,
        overlaps=overlaps,
    )


def sanity(names):
    """Re-run inference on classes we DO have, and compare."""
    rows = []
    for cls in names:
        known = LAYOUTS.get(cls)
        if not known:
            continue
        # 'field' form resolves names through layouts.json itself -> circular.
        # The honest check uses only raw byte-offset arithmetic.
        r = analyse(cls, allow_field=False, restrict_forms=("deref", "index"))
        if not r:
            continue
        kn_off = set(m["offset"] for m in known["members"])
        extents = []
        for m in known["members"]:
            w = m.get("width") or 0
            n = m.get("count") or 1
            extents.append((m["offset"], m["offset"] + w * max(n, 1)))
        inf = sorted(set(m["offset"] for m in r["members"]))
        exact = [o for o in inf if o in kn_off]
        inside = [o for o in inf if o not in kn_off
                  and any(a <= o < b for a, b in extents)]
        oob = [o for o in inf if o >= known["size"]]
        other = [o for o in inf if o not in exact and o not in inside and o not in oob]
        oob_dense = [o for o in inf if o >= known["size"]
                     and ("0x%X" % o) not in r["outlier_offsets"]]
        rows.append(OrderedDict(
            cls=cls, known_size=known["size"], known_size_hex="0x%X" % known["size"],
            known_members=len(known["members"]),
            inferred_offsets=len(inf), exact=len(exact), inside_member=len(inside),
            unmatched_in_range=len(other), unmatched_list=["0x%X" % o for o in other[:20]],
            beyond_known_size=len(oob), beyond_list=["0x%X" % o for o in oob[:20]],
            beyond_after_outlier_quarantine=len(oob_dense),
            beyond_after_quarantine_list=["0x%X" % o for o in oob_dense[:20]],
            lower_bound=r["size_lower_bound"], lower_bound_hex=r["size_lower_bound_hex"],
            lower_bound_ok=r["size_lower_bound"] <= known["size"],
            dense_lower_bound=r["size_lower_bound_dense"],
            dense_lower_bound_hex=r["size_lower_bound_dense_hex"],
            dense_lower_bound_ok=r["size_lower_bound_dense"] <= known["size"],
            confident_lower_bound=r["size_lower_bound_confident"],
            confident_lower_bound_hex=r["size_lower_bound_confident_hex"],
            confident_lower_bound_ok=r["size_lower_bound_confident"] <= known["size"]))
    return rows


def grade_all():
    """Score the method against EVERY class we already have a real layout for."""
    t = defaultdict(int)
    for cls in OURS:
        if cls not in LAYOUTS:
            continue
        known = LAYOUTS[cls]
        if not known["size"] or len(known["members"]) < 4:
            continue
        r = analyse(cls, allow_field=False, restrict_forms=("deref", "index"))
        if not r or not r["members"]:
            continue
        ext = [(m["offset"], m["offset"] + (m.get("width") or 0) * max(m.get("count") or 1, 1))
               for m in known["members"]]
        kn = set(m["offset"] for m in known["members"])
        inf = sorted(set(m["offset"] for m in r["members"]))
        t["classes"] += 1
        t["offsets"] += len(inf)
        t["exact"] += sum(1 for o in inf if o in kn)
        t["inside"] += sum(1 for o in inf if o not in kn and any(a <= o < b for a, b in ext))
        t["in_range_no_member"] += sum(1 for o in inf if o < known["size"] and o not in kn
                                       and not any(a <= o < b for a, b in ext))
        t["past_known_size"] += sum(1 for o in inf if o >= known["size"])
        if r["size_lower_bound"] > known["size"]:
            t["lb_bad"] += 1
        if r["size_lower_bound_confident"] > known["size"]:
            t["conf_bad"] += 1
    n = max(t["offsets"], 1)
    return OrderedDict(
        note="the same inference run against the classes that DO have an IDB layout, using only "
             "raw byte-offset forms (the ->field form is excluded because it resolves through "
             "layouts.json and would be grading itself)",
        classes_graded=t["classes"], offsets_inferred=t["offsets"],
        landed_on_a_known_member_offset=t["exact"],
        landed_inside_a_known_member=t["inside"],
        in_range_but_no_known_member=t["in_range_no_member"],
        past_the_known_struct_size=t["past_known_size"],
        pct_on_or_inside_a_known_member=round(100.0 * (t["exact"] + t["inside"]) / n, 1),
        pct_past_the_known_size=round(100.0 * t["past_known_size"] / n, 1),
        classes_where_lower_bound_exceeds_real_size=t["lb_bad"],
        classes_where_confident_bound_exceeds_real_size=t["conf_bad"],
    )


def cross_check_mm2types(results):
    """Grade the lower bounds against MM2Hook's sizeof table, which inference never reads."""
    rows = [(c, MM2SIZE[c], v["size_lower_bound"], v["size_lower_bound_confident"])
            for c, v in results.items() if c in MM2SIZE]
    real = [r for r in rows if r[1] > 2]  # 1 and 2 are empty-class placeholders, not sizes
    over = [OrderedDict(cls=c, mm2types_size_hex="0x%X" % s_, lower_bound_hex="0x%X" % lb,
                        confident_hex="0x%X" % cf)
            for c, s_, lb, cf in sorted(real, key=lambda r: -(r[2] - r[1])) if lb > s_]
    return OrderedDict(
        note="data/mm2types.json carries MM2Hook's sizeof for 296 types and is not consulted "
             "anywhere in this inference, so it is an independent grader - including for 133 "
             "classes the IDB type dump has no layout for at all",
        classes_cross_checked=len(rows),
        excluded_placeholder_sizes=len(rows) - len(real),
        placeholder_note="a size of 1 or 2 is MM2Hook declaring an empty class, not a measurement",
        graded=len(real),
        lower_bound_within_real_size=sum(1 for r in real if r[2] <= r[1]),
        lower_bound_equals_real_size=sum(1 for r in real if r[2] == r[1]),
        lower_bound_exceeds_real_size=len(over),
        median_lower_bound_over_real_size=round(
            statistics.median([r[2] / float(r[1]) for r in real]), 2) if real else 0,
        exceeded=over,
        caveat="several of the 'exceeded' rows are MM2Hook stubs rather than inference errors. "
               "camTrackCS is the clearest: MM2Hook says 0x110, this inference says 0x298, and "
               "mmPlayer holds three camTrackCS instances at 0xE60/0x10F8/0x1390 - a stride of "
               "exactly 0x298. The inference is right and the grader is short.",
    )


def main():
    have = set(k for k in OURS if k in LAYOUTS)
    missing = [k for k in OURS if k not in LAYOUTS]

    results = OrderedDict()
    empty = []
    nofile = []
    for cls in missing:
        r = analyse(cls)
        if r is None:
            nofile.append(cls)
        elif r["members"]:
            results[cls] = r
        else:
            empty.append(cls)

    counts = sorted(len(v["members"]) for v in results.values())

    check = sanity(["aiPath", "vehWheel", "asCamera", "MenuManager", "gizBridge",
                "vehTransmission", "asCullManager", "phInertialCS", "UIMenu",
                "mmSlider", "asNode", "mmGame"])

    meta = OrderedDict(
        generated_from="MM2_RE_KIT/MM2_PSEUDOCODE/<Class>/<Class>.c",
        schema="close to data/layouts.json, but deliberately nested under 'layouts' so it "
               "cannot be dropped into a layouts.json consumer by accident",
        WARNING="EVERY entry here is INFERRED from observed member accesses. 'size' is always "
                "null. 'size_lower_bound' is a floor from max(offset+width) seen; the real "
                "struct is at least that big and is usually bigger. NEVER feed these numbers "
                "to check_size() - pinning a struct at a too-small size makes every allocation "
                "of it undersized and corrupts memory.",
        classes_known=len(OURS),
        classes_with_idb_layout=len(have),
        classes_missing_layout=len(missing),
        classes_with_inferred_members=len(results),
        classes_yielding_nothing=len(empty),
        classes_with_no_pseudocode_file=len(nofile),
        median_members=(statistics.median(counts) if counts else 0),
        mean_members=round(sum(counts) / max(len(counts), 1), 1),
        total_members=sum(counts),
        member_forms={
            "deref": "*(T *)((char *)this + N) and friends - byte offset read straight off",
            "index": "this[N] - N scaled by the pointee width of this",
            "field": "this->Name - offset resolved through the DECLARED type of this in "
                     "layouts.json; only as right as that declared type is",
        },
        sanity_check=check,
        global_validation=grade_all(),
        mm2types_cross_check=cross_check_mm2types(results),
        classes_yielding_nothing_list=empty,
        classes_with_no_pseudocode_file_list=nofile,
    )

    out = OrderedDict(_meta=meta, layouts=results)
    dest = os.path.join(ROOT, "data", "inferred_layouts.json")
    with open(dest, "w", encoding="utf-8") as f:
        json.dump(out, f, indent=1)

    print("classes known         : %d" % len(OURS))
    print("with IDB layout       : %d" % len(have))
    print("missing layout        : %d" % len(missing))
    print("  gained members      : %d" % len(results))
    print("  yielded nothing     : %d" % len(empty))
    print("  no pseudocode file  : %d" % len(nofile))
    print("median members        : %s" % (statistics.median(counts) if counts else 0))
    print("mean members          : %.1f" % (sum(counts) / max(len(counts), 1)))
    print("total members         : %d" % sum(counts))
    print("classes w/ conflicts  : %d" % sum(1 for v in results.values() if v["conflicts"]))
    print("classes w/ overlaps   : %d" % sum(1 for v in results.values() if v["overlaps"]))
    print("wrote %s" % dest)

    print("\n--- sanity check (raw byte-offset forms only, no ->field) ---")
    for r in check:
        print("  %-14s known 0x%-5X %3dm | inf %3d: exact %3d inside %3d unmatch %2d BEYOND %2d"
              " (%2d after quarantine) | lb 0x%-5X %-3s dense 0x%-5X %s"
              % (r["cls"], r["known_size"], r["known_members"], r["inferred_offsets"],
                 r["exact"], r["inside_member"], r["unmatched_in_range"],
                 r["beyond_known_size"], r["beyond_after_outlier_quarantine"],
                 r["lower_bound"], "OK" if r["lower_bound_ok"] else "BAD",
                 r["dense_lower_bound"], "OK" if r["dense_lower_bound_ok"] else "BAD")
              + "  conf 0x%-5X %s" % (r["confident_lower_bound"],
                                      "OK" if r["confident_lower_bound_ok"] else "BAD"))
        if r["beyond_list"]:
            print("       beyond: %s" % ", ".join(r["beyond_list"]))
        if r["unmatched_list"]:
            print("       unmatched: %s" % ", ".join(r["unmatched_list"]))

    top = sorted(results.items(), key=lambda kv: -len(kv[1]["members"]))
    print("\ntop 20 by member count:")
    for k, v in top[:20]:
        print("  %-30s %3d members  lower bound %s" % (k, len(v["members"]),
                                                       v["size_lower_bound_hex"]))
    print("\nthin (<=2 members):")
    thin = [k for k, v in results.items() if len(v["members"]) <= 2]
    print("  %d: %s" % (len(thin), ", ".join(thin[:40])))


if __name__ == "__main__":
    main()
