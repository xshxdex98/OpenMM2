# -*- coding: utf-8 -*-
"""Measure how far the port has got, and record it so the trend is visible.

Two different things are worth measuring and they are easy to conflate.

**Ported** is what has actually been reimplemented: a declaration marked ARTS_EXPORT, whose PROC
has been stripped out of game.asm. This is the real number and it starts at zero.

**Ready** is what could be ported right now without guessing. A class cannot be safely ported
until its instance layout is known, because members sit at fixed byte offsets that the remaining
assembly still reads. Readiness is the useful leading indicator, and it is where all the work so
far has gone.

Progress is measured in BYTES OF MACHINE CODE, not function count. A class of eighty one-line
accessors is an afternoon; sdlPage16 is nineteen functions and 47 KB of tessellator. Counting
functions would make the first look like four times the progress of the second.

Writes docs/PROGRESS.md and appends a snapshot to data/progress_history.jsonl.
"""
import argparse
import datetime
import json
import os
import re
import sys
from collections import OrderedDict, defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

CODE = os.path.join(ROOT, "code", "midtown2")
ASM = os.environ.get("MM2_ASM", r"C:\mm2ghidra\out\game.asm")

OUT_MD = os.path.join(ROOT, "docs", "PROGRESS.md")
HISTORY = os.path.join(ROOT, "data", "progress_history.jsonl")

FREE = "(free functions)"

MANGLED = re.compile(r"^\s*//\s*(\?[^\s|]+)")
MARKER = re.compile(r"\bARTS_(IMPORT|EXPORT)\b")


def load(name):
    path = os.path.join(ROOT, "data", name)
    if not os.path.exists(path):
        return {}
    with open(path, encoding="utf-8") as f:
        return json.load(f)


def scan_markers():
    """{mangled: 'IMPORT'|'EXPORT'} across every generated header."""
    marks = {}

    for dirpath, _, files in os.walk(CODE):
        for name in sorted(files):
            if not name.endswith((".h", ".cpp")):
                continue

            pending = None
            with open(os.path.join(dirpath, name), encoding="utf-8", errors="replace") as f:
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


def asm_stats():
    """Size of the remaining assembly - the literal progress bar."""
    if not os.path.exists(ASM):
        return None

    lines = procs = 0
    with open(ASM, encoding="utf-8", errors="replace") as f:
        for line in f:
            lines += 1
            if " PROC" in line and not line.startswith(";"):
                procs += 1

    return OrderedDict(path=ASM, bytes=os.path.getsize(ASM), lines=lines, procs=procs)


def human(n):
    for unit in ("B", "KB", "MB"):
        if n < 1024 or unit == "MB":
            return "%.1f %s" % (n, unit)
        n /= 1024.0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--no-record", action="store_true",
                    help="print the report without appending a history snapshot")
    args = ap.parse_args()

    syms = load("symbols.json")
    layouts = load("layouts.json")
    hierarchy = load("hierarchy.json")
    mm2types = load("mm2types.json")

    if not syms:
        sys.exit("no data/symbols.json - run tools/symbols.py first")

    marks = scan_markers()

    code = [s for s in syms if s.get("code")]
    total_bytes = sum(s.get("size") or 0 for s in code)

    sized = {}   # class -> code bytes
    fns = defaultdict(int)
    sub_of = {}

    for s in code:
        cls = (s.get("class") or FREE).split("::")[0]
        sized[cls] = sized.get(cls, 0) + (s.get("size") or 0)
        fns[cls] += 1
        sub_of[cls] = s.get("subsystem", "misc")

    # Ported: an ARTS_EXPORT marker means the C++ has replaced the original.
    ported_fns = [s for s in code if marks.get(s["mangled"]) == "EXPORT"]
    ported_bytes = sum(s.get("size") or 0 for s in ported_fns)

    # READY means the MEMBER layout is known, not merely the size.
    #
    # Free functions belong to no class and have no instance to lay out, so they count as ready.
    # Treating them as "blocked on a layout" put 224 KB - 13% of the binary - in the blocked column
    # and at the top of the "recover this layout first" list, where there is no layout to recover.
    #
    # These are different things and conflating them overstated progress badly. MM2Hook supplies
    # sizeof for 264 classes but member offsets for almost none, and a class cannot be ported from
    # a size alone - the remaining assembly reads its fields at fixed offsets, which have to be
    # declared. Counting size-only classes as ready reported 83.5%; the honest figure is far lower,
    # and the weaker tier is tracked separately.
    have_size = set(layouts) | {FREE}
    ready_classes = [c for c in sized if c in have_size]
    ready_bytes = sum(sized[c] for c in ready_classes)

    size_only = (set(mm2types.get("sizes", {})) - set(layouts)) & set(sized)
    size_only_bytes = sum(sized[c] for c in size_only)

    have_base = set(c for c, r in hierarchy.items() if r.get("base")) \
        | set(mm2types.get("bases", {}))

    # --- per subsystem ---
    subs = defaultdict(lambda: OrderedDict(classes=0, fns=0, bytes=0, ported=0,
                                           ready_classes=0, ready_bytes=0))
    for cls, b in sized.items():
        s = subs[sub_of[cls]]
        s["classes"] += 1
        s["fns"] += fns[cls]
        s["bytes"] += b
        if cls in have_size:
            s["ready_classes"] += 1
            s["ready_bytes"] += b

    for s in ported_fns:
        subs[s.get("subsystem", "misc")]["ported"] += s.get("size") or 0

    asm = asm_stats()

    # --- interesting numbers ---
    ranked = sorted(sized.items(), key=lambda kv: -kv[1])
    top20 = sum(b for _, b in ranked[:20])
    sizes = sorted((s.get("size") or 0) for s in code)
    median_fn = sizes[len(sizes) // 2] if sizes else 0
    ported_classes = {x.get("class") for x in ported_fns}
    biggest_unported = next(((c, b) for c, b in ranked
                             if b and c != FREE and c not in ported_classes), (None, 0))
    virtuals = sum(1 for s in syms if s.get("virtual"))
    vftables = sum(1 for s in syms if s.get("kind") == "vftable")

    snapshot = OrderedDict(
        date=datetime.date.today().isoformat(),
        functions_total=len(code),
        functions_ported=len(ported_fns),
        bytes_total=total_bytes,
        bytes_ported=ported_bytes,
        classes_total=len(sized),
        classes_ready=len(ready_classes),
        bytes_ready=ready_bytes,
        classes_size_only=len(size_only),
        bytes_size_only=size_only_bytes,
        asm_bytes=(asm or {}).get("bytes"),
        asm_procs=(asm or {}).get("procs"),
    )

    write_report(dict(
        snapshot=snapshot, subs=subs, sized=sized, fns=fns, have_size=have_size,
        have_base=have_base, ranked=ranked, top20=top20, total_bytes=total_bytes,
        median_fn=median_fn, biggest_unported=biggest_unported, virtuals=virtuals,
        vftables=vftables, asm=asm, ready_bytes=ready_bytes, ported_bytes=ported_bytes,
        n_ported=len(ported_fns), n_code=len(code), layouts=layouts, mm2types=mm2types,
        size_only_bytes=size_only_bytes))

    if not args.no_record:
        os.makedirs(os.path.dirname(HISTORY), exist_ok=True)
        with open(HISTORY, "a", encoding="utf-8", newline="\n") as f:
            f.write(json.dumps(snapshot) + "\n")
        print("recorded snapshot in %s" % HISTORY)


def bar(frac, width=28):
    filled = int(round(frac * width))
    return "`" + "#" * filled + "." * (width - filled) + "`"


def write_report(c):
    s = c["snapshot"]
    total = c["total_bytes"] or 1
    L = []

    L.append("# OpenMM2 progress")
    L.append("")
    L.append("Generated by `tools/progress.py`. Do not edit by hand.")
    L.append("")
    L.append("Progress is measured in **bytes of machine code**, not function count. A class of")
    L.append("eighty one-line accessors is an afternoon; `sdlPage16` is nineteen functions and")
    L.append("47 KB of city tessellator. Counting functions would rank the first four times higher.")
    L.append("")
    L.append("Two numbers matter and they are different:")
    L.append("")
    L.append("- **Ported** - actually reimplemented. An `ARTS_EXPORT` declaration whose `PROC` has")
    L.append("  been stripped from `game.asm`. This is the real number.")
    L.append("- **Ready** - could be ported now without guessing. This needs the MEMBER layout,")
    L.append("  not just the size: the remaining assembly reads fields at fixed offsets, so they")
    L.append("  must be declared. A size on its own only pins `check_size`.")
    L.append("")
    L.append("## Overall")
    L.append("")
    L.append("| | | |")
    L.append("|---|--:|---|")
    L.append("| Ported | %.2f%% | %s |" % (100.0 * c["ported_bytes"] / total,
                                           bar(c["ported_bytes"] / float(total))))
    L.append("| Ready to port | %.1f%% | %s |" % (100.0 * c["ready_bytes"] / total,
                                                  bar(c["ready_bytes"] / float(total))))
    L.append("| Size known, members not | %.1f%% | %s |"
             % (100.0 * c["size_only_bytes"] / total,
                bar(c["size_only_bytes"] / float(total))))
    L.append("")
    L.append("| Metric | Value |")
    L.append("|---|--:|")
    L.append("| Functions | %d |" % s["functions_total"])
    L.append("| Functions ported | %d |" % s["functions_ported"])
    L.append("| Machine code | %s |" % human(c["total_bytes"]))
    L.append("| Code ported | %s |" % human(c["ported_bytes"]))
    L.append("| Classes | %d |" % s["classes_total"])
    L.append("| Classes with a member layout | %d (%.0f%%) |" %
             (s["classes_ready"], 100.0 * s["classes_ready"] / max(s["classes_total"], 1)))
    L.append("| Classes with only a size | %d |" % s["classes_size_only"])
    L.append("| Virtual methods | %d |" % c["virtuals"])
    L.append("| Vftables | %d |" % c["vftables"])

    if c["asm"]:
        L.append("| `game.asm` remaining | %s, %d PROCs |" %
                 (human(c["asm"]["bytes"]), c["asm"]["procs"]))

    L.append("")
    L.append("The size of `game.asm` *is* the progress bar: it starts holding the whole game and")
    L.append("shrinks with every function ported.")
    L.append("")
    L.append("## By subsystem")
    L.append("")
    L.append("Ranked by code size. `Ready` is the share of that subsystem's code whose class")
    L.append("layout is known.")
    L.append("")
    L.append("| Subsystem | Classes | Functions | Code | Share | Ported | Ready |")
    L.append("|---|--:|--:|--:|--:|--:|--:|")

    for name, v in sorted(c["subs"].items(), key=lambda kv: -kv[1]["bytes"]):
        L.append("| `%s` | %d | %d | %s | %.1f%% | %.1f%% | %.0f%% |" % (
            name, v["classes"], v["fns"], human(v["bytes"]),
            100.0 * v["bytes"] / total,
            100.0 * v["ported"] / max(v["bytes"], 1),
            100.0 * v["ready_bytes"] / max(v["bytes"], 1)))

    L.append("")
    L.append("## Shape of the work")
    L.append("")
    top20_pct = 100.0 * c["top20"] / total
    L.append("- The **20 largest classes are %.0f%% of all code**. The port is front-loaded: a"
             % top20_pct)
    L.append("  small number of large classes dominate, and the long tail is cheap.")
    L.append("- **Median function is %d bytes** - most of the binary is small functions."
             % c["median_fn"])
    L.append("- **%d virtual methods across %d vftables.** Constructors must be ported together"
             % (c["virtuals"], c["vftables"]))
    L.append("  with every virtual of their class, or the vtable the compiler emits will not link.")

    if c["biggest_unported"][0]:
        L.append("- Largest class not yet ported: **`%s`** at %s."
                 % (c["biggest_unported"][0], human(c["biggest_unported"][1])))

    blocked = [(cl, b) for cl, b in c["ranked"] if cl not in c["have_size"] and b]
    blocked_bytes = sum(b for _, b in blocked)
    L.append("- **%d classes (%s, %.0f%% of code) have no known layout** and are blocked: their"
             % (len(blocked), human(blocked_bytes), 100.0 * blocked_bytes / total))
    L.append("  member offsets would have to be guessed, which is how you get silent corruption.")

    L.append("")
    L.append("### Biggest blocked classes")
    L.append("")
    L.append("Recovering these layouts unblocks the most code per unit of effort.")
    L.append("")
    L.append("| Class | Functions | Code |")
    L.append("|---|--:|--:|")
    for cl, b in blocked[:12]:
        L.append("| `%s` | %d | %s |" % (cl, c["fns"][cl], human(b)))

    L.append("")
    L.append("### Biggest ready classes")
    L.append("")
    L.append("Layout known, so these can be started today.")
    L.append("")
    L.append("| Class | Functions | Code |")
    L.append("|---|--:|--:|")
    ready = [(cl, b) for cl, b in c["ranked"]
             if cl in c["have_size"] and b and cl != FREE]
    for cl, b in ready[:12]:
        L.append("| `%s` | %d | %s |" % (cl, c["fns"][cl], human(b)))

    L.append("")
    L.append("## History")
    L.append("")

    if os.path.exists(HISTORY):
        rows = []
        with open(HISTORY, encoding="utf-8") as f:
            for line in f:
                line = line.strip()
                if line:
                    rows.append(json.loads(line))

        if rows:
            L.append("| Date | Ported | Code ported | game.asm |")
            L.append("|---|--:|--:|--:|")
            for r in rows[-15:]:
                L.append("| %s | %.2f%% | %s | %s |" % (
                    r["date"],
                    100.0 * r["bytes_ported"] / max(r["bytes_total"], 1),
                    human(r["bytes_ported"]),
                    human(r["asm_bytes"]) if r.get("asm_bytes") else "-"))
            L.append("")

    L.append("Each run of `tools/progress.py` appends a snapshot to")
    L.append("`data/progress_history.jsonl`; pass `--no-record` to print without recording.")
    L.append("")

    os.makedirs(os.path.dirname(OUT_MD), exist_ok=True)
    with open(OUT_MD, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(L))

    print("wrote %s" % OUT_MD)
    print("  ported %.2f%% of code (%d/%d functions)"
          % (100.0 * c["ported_bytes"] / total, s["functions_ported"], s["functions_total"]))
    print("  ready  %.1f%% of code (%d/%d classes have a layout)"
          % (100.0 * c["ready_bytes"] / total, s["classes_ready"], s["classes_total"]))


if __name__ == "__main__":
    main()
