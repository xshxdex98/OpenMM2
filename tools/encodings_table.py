# -*- coding: utf-8 -*-
"""The verified instruction-text table, shared by everything that has to measure game.asm.

ExportAsm.java used to emit .text as nothing but `db` runs, so any tool that needed the byte
length of a line could count commas. It now emits a real mnemonic wherever one assembles back to
retail's exact bytes (see tools/verify_encodings.py), and a mnemonic line is opaque to that count.

Getting the length wrong is not a visible failure, which is why this lives in one place instead of
being re-derived per tool:

  - tools/asm.py pads a ported function with exactly the bytes it removed. Undercount, and .text
    shrinks and every later address moves - breaking -FIXED -BASE:0x400000 silently.
  - tools/patches.py locates a patch by walking .text and accumulating lengths. Undercount, and
    the patch lands on the wrong bytes, or is refused as "not inside the emitted .text".

The table is the same file that drove the emission, so the lengths agree with what was emitted by
construction, rather than by a second disassembler having to independently be right.
"""
import os
import re

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
TABLE = os.path.join(ROOT, "data", "encodings.tsv")

# `jmp eax` is two bytes, not five, and a bare register name matches the symbol pattern used
# for the rel32 rule below. Register-operand branches are in the encoding table; this list
# only stops one from reaching the rel32 rule if the table is ever missing an entry.
REGISTERS = frozenset("""eax ebx ecx edx esi edi ebp esp
                         ax bx cx dx si di bp sp
                         al bl cl dl ah bh ch dh""".split())

_cache = None


def load(path=None):
    """MASM text -> the bytes it assembles to. Empty when no table has been generated yet."""
    global _cache
    if path is None and _cache is not None:
        return _cache

    table = {}
    src = path or TABLE
    if os.path.exists(src):
        collisions = set()
        with open(src, encoding="utf-8") as f:
            for line in f:
                if not line or line.startswith("#"):
                    continue
                hexbytes, _, text = line.rstrip("\n").partition("\t")
                if not text:
                    continue
                raw = bytes.fromhex(hexbytes)
                if text in table and table[text] != raw:
                    # Two encodings rendering to one mnemonic would make the text ambiguous and its
                    # length unknowable. It should not happen - only byte-identical round-trips are
                    # in this table - but guessing would silently misplace everything after it.
                    collisions.add(text)
                table[text] = raw
        for text in collisions:
            del table[text]

    if path is None:
        _cache = table
    return table


def line_length(stripped, table):
    """Byte length of one emitted .text line, or None if it does not contribute bytes.

    Returns 0 for the directives that occupy no space (labels, PROC/ENDP, PUBLIC, comments) and
    None for a line that looks like an instruction but cannot be measured - the caller must treat
    that as a hard error rather than as zero.
    """
    s = stripped
    if not s or s.startswith(";"):
        return 0
    # A trailing comment contributes no bytes. tools/asm_vtables.py annotates the lines it rewrites
    # ("__vft_orig_005B524C LABEL BYTE   ; was ??_7mmCityInfo@@6B@..."), and everything below
    # matches on the end of the line. Safe to cut at the first semicolon because ExportAsm.java
    # emits data only as `db 0XXh` runs - there are no quoted strings for one to hide in.
    cut = s.find(";")
    if cut > 0:
        s = s[:cut].strip()
        if not s:
            return 0
    if s.startswith("db "):
        # tools/asm.py pads a stripped function with `db <n> dup (0CCh)`, which is n bytes, not the
        # one byte a comma count would give.
        m = re.match(r"db\s+(\d+)\s+dup\s*\(", s)
        if m:
            return int(m.group(1))
        return s[3:].count(",") + 1
    if s.startswith("dd "):
        return 4
    if s.startswith("dw "):
        return 2
    if s.startswith("dq "):
        return 8
    # `D_5B0000 LABEL BYTE` names an address without occupying one. These only appear in the data
    # sections, which is why they surfaced the first time tools/asm_vtables.py - the only consumer
    # that walks .rdata - started measuring through here.
    if re.match(r"^\S+\s+LABEL\s+\w+\s*$", s):
        return 0
    if s in table:
        return len(table[s])
    # tools/asm.py rewrites `db 0E8h` + `dd SYM - ($ + 4)` into `call SYM` once SYM is EXTERN,
    # because MASM cannot evaluate the subtraction across modules. An external target's distance is
    # unknown at assembly time, so MASM has to emit the rel32 form: five bytes for both call and
    # jmp. The indirect forms never reach here - they are in the table above.
    # A symbol operand only. An indirect `call dword ptr [ecx + 54h]` also starts with a
    # letter, and answering 5 for its 3 bytes would silently shift everything after it -
    # so anything with a memory operand falls through to the hard error below instead.
    m = re.match(r"^(?:call|jmp)\s+([?_@$A-Za-z][\w?@$]*)\s*$", s)
    if m and m.group(1).lower() not in REGISTERS:
        return 5
    if (s.endswith(":") or s.startswith("PUBLIC") or " PROC" in s or " ENDP" in s
            or s.startswith("ALIGN") or s.startswith("INCLUDE") or s.startswith("OPTION")
            or s.startswith("ASSUME") or s.startswith("EXTERN") or s.startswith("EXTRN")
            or s.startswith("END") or s.startswith(".")):
        return 0
    return None
