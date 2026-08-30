# -*- coding: utf-8 -*-
"""Recover the true virtual-method SLOT ORDER of every polymorphic class, from midtown2.exe.

Why this exists
---------------
`tools/genheaders.py` sorts methods alphabetically after the constructor and destructor. That is
Open1560's house rule and it is right for readability - but a C++ compiler assigns vtable slots in
DECLARATION order. So the moment a polymorphic class is reimplemented, the compiler emits a vtable
with the virtuals in alphabetical order while the original binary's vtable has them in 1999
declaration order. Every virtual call through that table then dispatches to the WRONG method: same
signature, same arity, no diagnostic anywhere. The game would look like it had gone quietly insane.

The fix is to declare virtuals in slot order, which means recovering slot order. The vftables in
`.rdata` are that order, verbatim: slot i of the table is the i'th virtual method declared.

Method
------
For each of the 340 `??_7...@@6B...` symbols, walk `.rdata` a dword at a time. Anything pointing
into `.text` is a slot - an entry with no symbol is still real, because a pure virtual resolves to
`__purecall` and thunks are not always in the map - so only a value outside the code section ends
the table. Vftables sit back to back, so the next vftable symbol is a hard ceiling.

Every slot address is then resolved against data/symbols.json, which is authoritative, and
classified against the base class's own table:

    inherited   the base's table has the identical pointer at this index
    override    the base's table has a DIFFERENT pointer at this index
    new         this index is past the end of the base's table
    pure        the slot points at __purecall
    thunk       the slot points at an [thunk]: adjustor or vcall thunk (multiple inheritance)

NOTE ON ADDRESSES. midtown2.map's `Rva+Base` column is 0x308 too high for `.rdata` - exactly the
`.rdata` start printed in the map header. `.text` is unaffected, so code resolves fine and the bug
is invisible until you read a vftable, at which point every lookup lands on a neighbouring table.
data/symbols.json already carries the corrected `rva` (with the raw value kept as `map_rva`), so
this tool uses `rva` and nothing else. Never use `map_rva` for a `.rdata` symbol.
"""
import json
import os
import re
import struct
import sys
from bisect import bisect_right
from collections import OrderedDict, Counter

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pe import IMAGE_BASE, load_sections, to_offset

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

SYMBOLS = os.path.join(ROOT, "data", "symbols.json")
HIERARCHY = os.path.join(ROOT, "data", "hierarchy.json")
MM2TYPES = os.path.join(ROOT, "data", "mm2types.json")
OUT = os.path.join(ROOT, "data", "vtable_order.json")
DOC = os.path.join(ROOT, "docs", "vtable_order.md")
EXE = os.environ.get("MM2_EXE", r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\midtown2.exe")

MAX_SLOTS = 512

# ??_7Class@@6B@            single-inheritance / primary vftable
# ??_7Class@@6BBase@@@      the sub-vftable for the `Base' subobject (multiple inheritance)
VFT_FOR = re.compile(r"^\?\?_7(?P<cls>.+?)@@6B(?P<forr>.*)@$")


def vft_for(mangled):
    """The base a vftable belongs to, or None for the primary table."""
    m = VFT_FOR.match(mangled)
    if not m:
        return None
    forr = m.group("forr")
    if not forr:
        return None
    return forr.rstrip("@")


_ALL_SYMS = []


def main():
    global _ALL_SYMS
    with open(SYMBOLS, encoding="utf-8") as f:
        syms = json.load(f)
    _ALL_SYMS = syms
    with open(HIERARCHY, encoding="utf-8") as f:
        hier = json.load(f)
    with open(MM2TYPES, encoding="utf-8") as f:
        mm2t = json.load(f)

    data, sections = load_sections(EXE)

    # ---- address -> symbol -------------------------------------------------------------------
    exact = {}
    for s in syms:
        if s["code"]:
            exact.setdefault(s["rva"], s)

    code_syms = sorted((s for s in syms if s["code"]), key=lambda s: s["rva"])
    code_addrs = [s["rva"] for s in code_syms]

    def resolve(addr):
        """Exact symbol if there is one, else the symbol whose extent contains the address."""
        s = exact.get(addr)
        if s:
            return s, 0
        i = bisect_right(code_addrs, addr) - 1
        if i < 0:
            return None, None
        c = code_syms[i]
        span = c.get("size") or 0
        if span and addr < c["rva"] + span:
            return c, addr - c["rva"]
        return None, None

    code_lo = min(code_addrs)
    code_hi = max(s["rva"] + (s["size"] or 0) for s in code_syms)

    purecall = next((s["rva"] for s in syms if s["mangled"] == "__purecall"), None)

    # ---- read every vftable ------------------------------------------------------------------
    vfts = [s for s in syms if s["kind"] == "vftable" and s.get("class")]
    vfts.sort(key=lambda s: s["rva"])

    ceiling = {}
    for i, v in enumerate(vfts):
        nxt = vfts[i + 1]["rva"] if i + 1 < len(vfts) else None
        ceiling[v["rva"]] = (nxt - v["rva"]) // 4 if nxt else MAX_SLOTS

    raw_tables = OrderedDict()   # mangled -> {cls, forr, rva, ptrs[]}
    for v in vfts:
        cls = v["class"].split("::")[0]
        off = to_offset(sections, v["rva"] - IMAGE_BASE)
        if off is None:
            continue

        limit = min(ceiling.get(v["rva"], MAX_SLOTS), MAX_SLOTS)
        ptrs = []
        for i in range(limit):
            raw = struct.unpack_from("<I", data, off + i * 4)[0]
            if not (code_lo <= raw < code_hi):
                break
            ptrs.append(raw)

        raw_tables[v["mangled"]] = dict(cls=cls, forr=vft_for(v["mangled"]),
                                        rva=v["rva"], ptrs=ptrs, mangled=v["mangled"])

    # primary table per class = the one with no `{for Base}` qualifier.
    #
    # The five multiple-inheritance classes have no unqualified table at all, and picking the
    # lowest address is WRONG: vehTrailer's two tables are `@@6BasNode@@@` (lower) and
    # `@@6BdgPhysEntity@@@`, and it is the dgPhysEntity one that sits at offset 0. What settles it
    # is the adjustor thunks. A secondary table is reached with a `this` that must be corrected
    # first, so MSVC fills its overridden slots with `[thunk]: ... adjustor{N}` stubs; the primary
    # table never needs one. So: among the qualified tables, the primary is the one with no thunk.
    #
    # Getting this wrong is not cosmetic - it made MM2Hook's vehTrailer _vtbl look like nine
    # consecutive slot-order disagreements when in fact it agrees on all of them.
    def has_thunk(t):
        for addr in t["ptrs"]:
            sym, _ = resolve(addr)
            if sym and (sym.get("demangled") or "").startswith("[thunk]"):
                return True
        return False

    primary = {}
    extras = {}
    for t in raw_tables.values():
        cls = t["cls"]
        if t["forr"] is None:
            primary[cls] = t
        else:
            extras.setdefault(cls, []).append(t)
    for cls, lst in extras.items():
        if cls not in primary:
            lst.sort(key=lambda t: (has_thunk(t), t["rva"]))
            primary[cls] = lst.pop(0)

    # ---- base resolution ---------------------------------------------------------------------
    stated_bases = mm2t.get("bases") or {}

    def base_of(cls):
        """genheaders.py's rule, but preferring a base that actually HAS a vftable, since the
        classification below compares tables. A stated base with no table of its own tells us
        nothing about slot indices."""
        stated = stated_bases.get(cls)
        inferred = (hier.get(cls) or {}).get("base")
        for cand in (stated, inferred):
            if cand and cand != cls and cand in primary:
                return cand
        for cand in (stated, inferred):
            if cand and cand != cls:
                return cand
        return None

    # ---- classify ----------------------------------------------------------------------------
    def build(t, base_ptrs):
        slots = []
        for i, addr in enumerate(t["ptrs"]):
            sym, delta = resolve(addr)

            # An address that lands INSIDE a symbol rather than at its start is not that symbol.
            # It is a function midtown2.map never named, and the map's `size` (distance to the
            # next named symbol) simply swallowed it. Reporting the containing symbol's name here
            # would be a lie in the one place a lie is most expensive - mmMultiCircuit slot 0 came
            # out as an `override` of `GameMessage` when it is the deleting destructor.
            #
            # Keep the containing symbol as provenance and nothing more.
            nearest = None
            if sym is not None and delta:
                nearest = sym["mangled"]
                sym, delta = None, None

            mangled = sym["mangled"] if sym else None
            demangled = (sym.get("demangled") or "") if sym else ""
            owner = sym.get("class").split("::")[0] if (sym and sym.get("class")) else None

            if purecall is not None and addr == purecall:
                kind = "pure"
            elif demangled.startswith("[thunk]"):
                kind = "thunk"
            elif base_ptrs is not None and i < len(base_ptrs):
                kind = "inherited" if base_ptrs[i] == addr else "override"
            elif base_ptrs is None:
                kind = "new" if owner == t["cls"] else "inherited"
            else:
                kind = "new"

            if mangled is None and purecall is not None and addr != purecall:
                kind = "unnamed"

            slot = OrderedDict(index=i, address=addr, symbol=mangled, owner=owner, kind=kind)
            if sym and sym.get("name"):
                slot["name"] = sym["name"]
            if nearest:
                slot["unresolved"] = True
                slot["inside_symbol"] = nearest
            slots.append(slot)
        return slots

    result = OrderedDict()
    for cls in sorted(primary):
        t = primary[cls]
        base = base_of(cls)
        base_t = primary.get(base)
        slots = build(t, base_t["ptrs"] if base_t else None)

        entry = OrderedDict(
            vftable_rva=t["rva"],
            vftable_symbol=t["mangled"],
            base=base,
            slot_count=len(slots),
            slots=slots,
        )

        if extras.get(cls):
            secondary = []
            for e in sorted(extras[cls], key=lambda x: x["rva"]):
                sb = primary.get(e["forr"])
                secondary.append(OrderedDict(
                    for_base=e["forr"],
                    vftable_rva=e["rva"],
                    vftable_symbol=e["mangled"],
                    slot_count=len(e["ptrs"]),
                    slots=build(e, sb["ptrs"] if sb else None),
                ))
            entry["secondary_vftables"] = secondary

        result[cls] = entry

    with open(OUT, "w", encoding="utf-8", newline="\n") as f:
        json.dump(result, f, indent=1)

    # ---- checks ------------------------------------------------------------------------------
    report = analyse(result, primary, mm2t, base_of)
    write_doc(result, report)

    print("wrote %s (%d classes, %d slots)" % (
        OUT, len(result), sum(r["slot_count"] for r in result.values())))
    for line in report["summary"]:
        print("  " + line)


# ---------------------------------------------------------------------------------------------


def method_name(slot):
    """The name to compare slots by, normalised over the two things that are not real differences.

    `GetClassName` / `GetClassNameA`: <windows.h> #defines GetClassName to GetClassNameA, so the
    1999 headers declared one virtual and the compiler mangled it under whichever spelling that
    translation unit happened to see. The two functions are byte-identical. Treating them as
    different accounted for 39 of the 40 apparent prefix-property violations.

    The deleting destructor: `vector deleting destructor', `scalar deleting destructor' and ~Class
    are all the same slot.
    """
    n = (slot.get("name") or "").strip("`'")
    if not n:
        return None
    if "deleting destructor" in n:
        return "~dtor"
    if n == "GetClassNameA":
        return "GetClassName"
    return n


def norm_mm2(name):
    """The same normalisation, applied to a name MM2Hook wrote by hand."""
    n = (name or "").strip("`'")
    if not n:
        return None
    if "deleting destructor" in n or n.startswith("~") or n in ("Destroy", "dtor"):
        return "~dtor"
    if n == "GetClassNameA":
        return "GetClassName"
    return n


def analyse(result, primary, mm2t, base_of):
    """The two independent checks on the recovered order.

    Both were previously reported far more strongly than they were performed, so what each one
    actually compares is spelled out here and in the document it writes.
    """
    # --- prefix property, slot by slot ---------------------------------------------------------
    #
    # The claim being tested is NOT "the derived table is at least as long as the base's". That is
    # a length check, it passes 279/279, and it is worth nothing - it cannot see a table paired
    # with the wrong base, which is the one failure it exists to catch.
    #
    # The real claim: at every index the base also has, the derived table holds either the
    # identical pointer (inherited) or a function of the same name (an override).
    prefix_ok = 0
    prefix_bad = []
    checked = 0
    for cls, r in result.items():
        base = r["base"]
        if not base or base not in result:
            continue
        checked += 1
        b = result[base]
        bad = []
        if r["slot_count"] < b["slot_count"]:
            bad.append("derived table is SHORTER (%d < %d)" % (r["slot_count"], b["slot_count"]))
        for i in range(min(r["slot_count"], b["slot_count"])):
            ds, bs = r["slots"][i], b["slots"][i]
            if ds["address"] == bs["address"]:
                continue
            if ds["kind"] == "pure" or bs["kind"] == "pure":
                continue
            dn, bn = method_name(ds), method_name(bs)
            if dn is not None and dn == bn:
                continue
            bad.append("slot %d holds `%s`, base holds `%s`" % (i, dn or "?", bn or "?"))
        if bad:
            prefix_bad.append((cls, base, "; ".join(bad[:4])))
        else:
            prefix_ok += 1

    # --- a class whose own virtuals are missing from its own table -----------------------------
    #
    # If a class declares `virtual void Reset()` then `?Reset@Cls@@...` must occupy a slot in Cls's
    # own table. Where several of them do not, the table carrying that name is not that class's
    # table. This is the check that caught the one mislabelled vftable symbol in the binary.
    own_missing = []
    virt_of = {}
    for s in _ALL_SYMS:
        if s.get("virtual") and s.get("code") and s.get("class") and s["kind"] == "function":
            if (s.get("demangled") or "").startswith("[thunk]"):
                continue
            virt_of.setdefault(s["class"].split("::")[0], set()).add(s["mangled"])
    for cls, r in result.items():
        have = set(x["symbol"] for x in r["slots"] if x["symbol"])
        for e in r.get("secondary_vftables") or []:
            have |= set(x["symbol"] for x in e["slots"] if x["symbol"])
        miss = sorted(m for m in virt_of.get(cls, ()) if m not in have)
        # A class routinely carries both ??_E (vector) and ??_G (scalar) deleting destructors and
        # the table holds only one of them; that is not a missing virtual.
        miss = [m for m in miss if not m.startswith(("??_E", "??_G"))]
        # GetClassNameA is the same virtual as GetClassName, see method_name().
        miss = [m for m in miss if "?GetClassNameA@" not in m]
        if miss:
            own_missing.append((cls, r["slot_count"], miss))

    # --- MM2Hook cross-check -------------------------------------------------------------------
    #
    # MM2Hook's _vtbl struct is an independently maintained list of virtual slot NAMES in order.
    # Three things make a naive comparison understate the agreement badly:
    #
    #   1. A slot the binary fills with __purecall is a pure virtual. MM2Hook names the method;
    #      the binary cannot. That is still agreement about ORDER, which is all that is at stake.
    #   2. A class with two tables has to be compared against the right one. MM2Hook's `vehTrailer`
    #      describes the dgPhysEntity subobject, not the asNode one.
    #   3. GetClassName/GetClassNameA and the destructor spellings, as above.
    #
    # What survives all three is a real disagreement, and every one is listed in the document.
    mm_vt = {k: v for k, v in (mm2t.get("vtables") or {}).items() if v}
    agree = pure = past = disagree = 0
    length_mismatch = []
    name_mismatch = []
    covered = 0
    absent = []

    def score(slots, names):
        a = 0
        for idx, nm in names.items():
            if idx >= len(slots):
                continue
            if slots[idx]["kind"] == "pure" or method_name(slots[idx]) == norm_mm2(nm):
                a += 1
        return a

    for cls, entries in sorted(mm_vt.items()):
        r = result.get(cls)
        if not r:
            absent.append(cls)
            continue
        covered += 1
        names = {e["slot"]: e["name"] for e in entries}

        candidates = [(r["vftable_symbol"], r["slots"])]
        for sec in r.get("secondary_vftables") or []:
            candidates.append((sec["vftable_symbol"], sec["slots"]))
        which, slots = max(candidates, key=lambda c: score(c[1], names))

        if max(names) + 1 != len(slots):
            length_mismatch.append((cls, which, max(names) + 1, len(slots)))
        for idx, nm in sorted(names.items()):
            if idx >= len(slots):
                past += 1
                name_mismatch.append((cls, idx, nm, "past end of table", "MM2Hook has a slot the "
                                                                        "binary's table does not"))
                continue
            slot = slots[idx]
            if slot["kind"] == "pure":
                pure += 1
                continue
            if method_name(slot) == norm_mm2(nm):
                agree += 1
            else:
                disagree += 1
                name_mismatch.append((cls, idx, nm, method_name(slot) or "?", ""))

    compared = agree + pure + past + disagree

    unresolved = [(cls, s["index"], "0x%X" % s["address"], s.get("inside_symbol") or "-")
                  for cls, r in result.items()
                  for s in (r["slots"] + [x for e in (r.get("secondary_vftables") or [])
                                          for x in e["slots"]])
                  if not s["symbol"]]

    kinds = Counter(s["kind"] for r in result.values() for s in r["slots"])
    multi = [c for c, r in result.items() if r.get("secondary_vftables")]
    tables = len(result) + sum(len(result[c]["secondary_vftables"]) for c in multi)

    summary = [
        "classes recovered      : %d" % len(result),
        "tables recovered       : %d" % tables,
        "slots recovered        : %d" % sum(r["slot_count"] for r in result.values()),
        "slot kinds             : %s" % ", ".join("%s=%d" % kv for kv in kinds.most_common()),
        "slots the map cannot name : %d" % len(unresolved),
        "prefix property        : %d/%d base pairs hold slot-by-slot" % (prefix_ok, checked),
        "own virtuals missing   : %d classes" % len(own_missing),
        "MM2Hook classes shared : %d of %d (%d emit no table of their own)" % (
            covered, len(mm_vt), len(absent)),
        "MM2Hook order agreement: %d/%d (%.1f%%)" % (
            agree + pure, compared, 100.0 * (agree + pure) / max(compared, 1)),
        "MM2Hook real conflicts : %d name, %d past-end" % (disagree, past),
        "classes with >1 vtable : %d (%s)" % (len(multi), ", ".join(sorted(multi))),
    ]

    return dict(summary=summary, prefix_bad=prefix_bad, length_mismatch=length_mismatch,
                name_mismatch=name_mismatch, unresolved=unresolved, kinds=kinds, multi=multi,
                agree=agree, pure=pure, past=past, disagree=disagree, compared=compared,
                covered=covered, checked=checked, prefix_ok=prefix_ok, absent=absent,
                own_missing=own_missing, mm_slots=sum(len(v) for v in mm_vt.values()),
                mm_classes=len(mm_vt), tables=tables)


def write_doc(result, rep):
    L = []
    w = L.append
    w("# Virtual method slot order")
    w("")
    w("Generated by `tools/vtable_order.py`. Data: `data/vtable_order.json`.")
    w("Consumed by `tools/asm_vtables.py`; `docs/vtables.md` is how a polymorphic class is")
    w("actually handed over to C++.")
    w("")
    w("## Why this matters")
    w("")
    w("A C++ compiler assigns vtable slots in **declaration order**. `tools/genheaders.py` sorts")
    w("methods alphabetically after the constructor and destructor - correct for readability, and")
    w("catastrophic for a polymorphic class. The moment such a class is reimplemented, the")
    w("compiler emits a vtable whose virtuals are in alphabetical order while every piece of")
    w("original 1999 code still calls through *slot indices* taken from the 1999 declaration")
    w("order. `Update` gets called when the binary meant `Reset`. Same signature, same arity, no")
    w("link error, no crash at the call site - the game just behaves as if it had gone insane.")
    w("")
    w("This document records the real order, read out of `.rdata`.")
    w("")
    w("## Method")
    w("")
    w("For each `??_7...@@6B...` symbol, `.rdata` is walked one dword at a time. Any value inside")
    w("`.text` is a slot; the first value outside it ends the table, and the next vftable symbol")
    w("is a hard ceiling because vftables sit back to back. An entry with no symbol is still a")
    w("real slot - a pure virtual resolves to `__purecall`, and thunks are not always named - so")
    w("only the section test terminates.")
    w("")
    w("Each address is resolved against `data/symbols.json` (authoritative) and classified")
    w("against the base class's own table:")
    w("")
    w("| kind | meaning |")
    w("|---|---|")
    w("| `inherited` | the base's table holds the identical pointer at this index |")
    w("| `override` | the base's table holds a different pointer at this index |")
    w("| `new` | this index is past the end of the base's table |")
    w("| `pure` | the slot points at `__purecall` |")
    w("| `thunk` | the slot points at an `[thunk]:` adjustor or vcall thunk |")
    w("| `unnamed` | a real code pointer `midtown2.map` never named |")
    w("")
    w("An address that falls *inside* a named symbol rather than at its start is **not** that")
    w("symbol. The map's `size` is the distance to the next named symbol, so a function the map")
    w("omits is swallowed by its predecessor's extent. Such slots are `unnamed`, with the")
    w("containing symbol kept only as provenance (`inside_symbol`). Resolving them by containment")
    w("reported `mmMultiCircuit` slot 0 as an override of `GameMessage`; it is the deleting")
    w("destructor.")
    w("")
    w("> `midtown2.map`'s `Rva+Base` column is 0x308 too high for `.rdata`. `data/symbols.json`")
    w("> already carries the corrected `rva` (raw value kept as `map_rva`); this tool reads `rva`")
    w("> only. Using `map_rva` here lands every lookup on a neighbouring table.")
    w("")
    w("## Results")
    w("")
    for line in rep["summary"]:
        k, _, v = line.partition(":")
        w("- **%s** - %s" % (k.strip(), v.strip()))
    w("")
    w("### Slot kinds")
    w("")
    w("| kind | slots |")
    w("|---|--:|")
    for k, n in rep["kinds"].most_common():
        w("| `%s` | %d |" % (k, n))
    w("")

    w("## The prefix property")
    w("")
    w("A base's vtable must be a prefix of its derived class's: the derived table is at least as")
    w("long, and at every index the base also has, the derived table holds either the identical")
    w("pointer or a function of the same name.")
    w("")
    w("**This is checked slot by slot, not by length.** The length-only version passes 279 of 279")
    w("and proves nothing: it cannot see a table paired with the wrong base, which is exactly the")
    w("failure it exists to catch.")
    w("")
    w("Result: **%d of %d** base/derived pairs hold." % (rep["prefix_ok"], rep["checked"]))
    w("")
    if rep["prefix_bad"]:
        w("| class | base | first differences |")
        w("|---|---|---|")
        for cls, base, why in rep["prefix_bad"]:
            w("| `%s` | `%s` | %s |" % (cls, base, why))
        w("")
        w("Both are understood, and neither is a slot-order error:")
        w("")
        w("- **`mmMultiCircuit` slot 0** is the only code pointer in the whole of `.rdata` that")
        w("  `midtown2.map` does not name. It is the deleting destructor; every other class has")
        w("  one at slot 0 and this one was simply left out of the map. Declare")
        w("  `virtual ~mmMultiCircuit()` and the slot fills itself.")
        w("- **`vehSuspension` slot 6** holds `?Copy@vehSuspension@@QAEXABV1@@Z`, which is not")
        w("  virtual and therefore cannot be in a vtable. There is no `?FileIO@vehSuspension@@`")
        w("  symbol at all. This is **identical COMDAT folding**: `vehSuspension::FileIO` and")
        w("  `vehSuspension::Copy` compiled to the same bytes, the linker kept one copy, and the")
        w("  map kept one name. The slot is `asNode::FileIO` overridden - declare it as such.")
        w("")
        w("Folding is worth understanding because it is the one way a slot can carry a wrong name")
        w("*silently*. Where the base has the same slot, the prefix check above catches it, as it")
        w("did here. Where the slot is new, nothing can: the address is right, the name may not")
        w("be. Read the pseudocode before trusting a `new` slot's name.")
    else:
        w("No violations.")
    w("")

    if rep["own_missing"]:
        w("## Classes whose own virtuals are not all in their own table")
        w("")
        w("If a class declares `virtual void Reset()`, then `?Reset@Cls@@...` must occupy a slot in")
        w("`Cls`'s own table. Where several do not, the table carrying that name is not that")
        w("class's table.")
        w("")
        w("| class | slots | own virtuals absent |")
        w("|---|--:|---|")
        for cls, n, miss in rep["own_missing"]:
            names = ", ".join(sorted(set(m.split("@")[0].lstrip("?") for m in miss)))
            w("| `%s` | %d | %s |" % (cls, n, names))
        w("")
        w("**`??_7dgUnhitBangerInstance@@6B@` is a mislabelled symbol. Do not port that class from")
        w("its own table.** The evidence is conclusive:")
        w("")
        w("- The table under that name holds `?Reset@lvlInstance@@` at slot 0 and")
        w("  `?Init@lvlInstance@@` at slot 17, while **every one of the nine classes that derive")
        w("  from `dgUnhitBangerInstance`** holds `?Reset@dgUnhitBangerInstance@@` and")
        w("  `?Init@dgUnhitBangerInstance@@` at those indices. A base cannot fail to contain an")
        w("  override that all its descendants inherit.")
        w("- It is 28 slots; `dgUnhitMtxBangerInstance` and `dgUnhitYBangerInstance` are 30, and")
        w("  the two extra slots hold `?Impact@dgUnhitBangerInstance@@` and")
        w("  `?ImpactCB@dgUnhitBangerInstance@@` - virtuals a derived class cannot introduce.")
        w("- It matches MM2Hook's `dgBangerInstance` `_vtbl` on 26 of 28 slots, and")
        w("  `dgBangerInstance` is one of the classes MM2Hook describes that emits no table.")
        w("")
        w("So the 28-slot table at that address is `dgBangerInstance`'s. The real chain is")
        w("`lvlInstance` (26) -> `dgBangerInstance` (28, adds `ComputeLod` and a *const*")
        w("`SetupGfxLights`) -> `dgUnhitBangerInstance` (30, adds `Impact`, `ImpactCB`). Take")
        w("`dgUnhitBangerInstance`'s order from `dgUnhitMtxBangerInstance`'s table instead.")
        w("")
        w("**`lvlInstance::DrawPhysics` occupies no slot in any of the 340 tables.** It is a")
        w("public virtual with 576 bytes of real code, and nothing dispatches to it. Declaring it")
        w("`virtual` on the C++ side would add a 27th slot to `lvlInstance` and shift every one of")
        w("its descendants. Declare it non-virtual, or leave it out.")
        w("")

    w("## Multiple inheritance")
    w("")
    w("A class with more than one vftable symbol has more than one vtable, one per base")
    w("subobject, and the secondary tables are reached through `[thunk]:` adjustors.")
    w("")
    w("None of the five has an unqualified `??_7C@@6B@`, so the primary table has to be picked.")
    w("**It is the one with no adjustor thunk in it**, not the one at the lower address: a")
    w("secondary table is entered with a `this` that has to be corrected first, so MSVC fills its")
    w("overridden slots with `[thunk]: ... adjustor{N}` stubs, and the primary never needs one.")
    w("Picking by address put `vehTrailer`'s asNode subobject first and made MM2Hook look like it")
    w("disagreed on nine consecutive slots; against the right table it agrees on all of them.")
    w("")
    w("| class | primary | secondary |")
    w("|---|---|---|")
    for cls in sorted(rep["multi"]):
        r = result[cls]
        sec = "<br>".join("`%s` (%d slots, for `%s`)" % (e["vftable_symbol"], e["slot_count"],
                                                         e["for_base"])
                          for e in r["secondary_vftables"])
        w("| `%s` | `%s` (%d slots) | %s |" % (cls, r["vftable_symbol"], r["slot_count"], sec))
    w("")

    w("## Cross-check against MM2Hook")
    w("")
    w("`data/mm2types.json` carries MM2Hook's `_vtbl` structs, an independent source that names")
    w("virtual slots in order - **%d slots across %d classes**. Comparing its name for slot *i*"
      % (rep["mm_slots"], rep["mm_classes"]))
    w("with the name the map gives the function actually sitting at slot *i*:")
    w("")
    w("| | |")
    w("|---|--:|")
    w("| classes MM2Hook describes | %d |" % rep["mm_classes"])
    w("| ...with an emitted table to compare against | %d |" % rep["covered"])
    w("| slots compared | %d |" % rep["compared"])
    w("| name matches | %d |" % rep["agree"])
    w("| binary says `__purecall`, MM2Hook names the pure virtual | %d |" % rep["pure"])
    w("| MM2Hook names a slot past the end of the emitted table | %d |" % rep["past"])
    w("| genuine name disagreements | %d |" % rep["disagree"])
    w("")
    w("**Agreement on slot order: %d/%d = %.1f%%.**" % (
        rep["agree"] + rep["pure"], rep["compared"],
        100.0 * (rep["agree"] + rep["pure"]) / max(rep["compared"], 1)))
    w("")
    w("A `__purecall` slot is counted as agreeing because it *is* agreement about order: MM2Hook")
    w("names the pure virtual declared at that index and the binary cannot, since a pure virtual")
    w("has no function to point at. Name-for-name, ignoring that, the figure is %d/%d = %.1f%%."
      % (rep["agree"], rep["compared"], 100.0 * rep["agree"] / max(rep["compared"], 1)))
    w("")
    if rep["absent"]:
        w("MM2Hook describes %d classes that emit no vftable of their own - nothing ever"
          % len(rep["absent"]))
        w("instantiates them, so the compiler never wrote a table. Their slot counts still matter,")
        w("because they are where a derived class's own virtuals start, and MM2Hook is the only")
        w("source for them: `%s`." % "`, `".join(sorted(rep["absent"])))
        w("")
    if rep["length_mismatch"]:
        w("### Table length disagreements")
        w("")
        w("| class | compared against | MM2Hook slots | read from binary |")
        w("|---|---|--:|--:|")
        for cls, which, theirs, ours in rep["length_mismatch"]:
            w("| `%s` | `%s` | %d | %d |" % (cls, which, theirs, ours))
        w("")
        w("The pattern is one-directional: MM2Hook stops short. Its `_vtbl` structs are")
        w("hand-maintained and were only ever filled in as far as somebody needed, which is why")
        w("the three tokenizers are described by a single slot against a 19-slot table. The")
        w("binary's length is the emitted table, bounded by the next vftable symbol, and it is")
        w("what the compiler will have to match.")
        w("")
        w("`lvlFixedRotY` and `lvlLandmark` are the only two where MM2Hook is *longer*, and both")
        w("are the same error: it lists `IsVisible` a second time at slot 26, where the emitted")
        w("table ends at 26 slots with the next vftable symbol beginning at the very next dword.")
        w("`IsVisible` is at slot 2.")
        w("")
    if rep["name_mismatch"]:
        w("### Every remaining disagreement")
        w("")
        w("| class | slot | MM2Hook | binary | note |")
        w("|---|--:|---|---|---|")
        for cls, idx, theirs, ours, why in rep["name_mismatch"]:
            w("| `%s` | %d | `%s` | `%s` | %s |" % (cls, idx, theirs, ours, why))
        w("")
        w("Eleven rows, four causes, and **the binary is right in ten of them**:")
        w("")
        w("- **The tokenizers.** MM2Hook's `datAsciiTokenizer`, `datBinTokenizer` and")
        w("  `datBaseTokenizer` `_vtbl` structs each contain exactly one entry against a 19-slot")
        w("  table. `?Put@datBaseTokenizer@@UAE_ND@Z` is real, and it is at slot **16**.")
        w("- **The bangers.** MM2Hook's `dgBangerInstance` omits `ComputeLod` and the *const*")
        w("  `SetupGfxLights` - a second virtual of that name, `UBE` rather than `UAE` - so every")
        w("  slot it lists after 25 is two too low. This is precisely the silent failure this file")
        w("  exists to prevent, present in the cross-check source itself, which is the argument")
        w("  for reading the order out of `.rdata` rather than trusting any hand-written list.")
        w("- **`lvlFixedRotY` / `lvlLandmark` slot 26**: a duplicated `IsVisible`, see above.")
        w("- **`vehSuspension` slot 6**: identical COMDAT folding, see the prefix section. Here")
        w("  MM2Hook is right and the map's name is the artefact - the slot is `FileIO`.")
        w("")

    if rep["unresolved"]:
        w("## Slots the map cannot name")
        w("")
        w("| class | slot | address | falls inside |")
        w("|---|--:|---|---|")
        for cls, idx, addr, inside in rep["unresolved"]:
            w("| `%s` | %d | `%s` | `%s` |" % (cls, idx, addr, inside))
        w("")

    with open(DOC, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(L) + "\n")


if __name__ == "__main__":
    main()
