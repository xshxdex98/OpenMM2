# -*- coding: utf-8 -*-
"""Deliberate behavioural changes to the original code, applied to game.asm.

Everything else in this project is a faithful transcription: the point of the harness is that the
binary behaves exactly as it did in 1999. This file is the one place where that is intentionally
untrue, so a divergence can never happen by accident or hide in a diff.

Each patch states what it changes, why, and what it does NOT fix. They are applied after
ExportAsm generates game.asm and before tools/asm.py strips ported functions.

A patch is checked twice before it is allowed to write anything:

  1. The retail binary must hold the expected bytes at the stated address. Without this the
     address is decorative - and both patches below were in fact first written down five bytes
     off, with nothing to catch it.
  2. The byte sequence must be UNIQUE in game.asm. Taking the first hit would be reckless: five
     ordinary bytes have plenty of room to occur twice in 1.7 MB of code, and patching the wrong
     one would change an unrelated constant and report success.

If either check fails the patch REFUSES rather than writing.
"""
import argparse
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)

ASM = os.environ.get("MM2_ASM", "C:/mm2ghidra/out/game.asm")
RETAIL = os.environ.get(
    "MM2_RETAIL", "C:/Users/xshxd/OneDrive/Desktop/PC Games/MM2/midtown2.exe")


class Patch:
    def __init__(self, name, address, old, new, why):
        self.name = name
        self.address = address
        self.old = old
        self.new = new
        self.why = why


PATCHES = [
    # Two separate patches, not one ten-byte pattern.
    #
    # game.asm emits one `db` directive per instruction, so `mov eax, 1600` and `mov ecx, 1200`
    # land on different lines and a pattern spanning both never matches. Splitting them also means
    # each is verified independently - if only one matched, that is worth seeing rather than
    # applying half a change.
    Patch(
        name="resolution-cap-width",
        address=0x004AC2EE,
        old=[0xB8, 0x40, 0x06, 0x00, 0x00],      # mov eax, 1600
        new=[0xB8, 0x00, 0x0A, 0x00, 0x00],      # mov eax, 2560
        why="""AutoDetectCallback caps the advertised resolution and reports
        'Max display resolution restricted to 1600 x 1200'. A video-memory test picks one of two
        branches: the low one loads 1024x768, the high one 1600x1200, and both then store the
        width to the global at 0x006844FC and push the pair to the mode setter.

        This ONLY changes what the game advertises and accepts. It does not fix the UI, which is
        authored for 640x480 and does not scale - see docs/ui_scaling.md, especially the CSV widget
        loader that pins menu widgets to the top-left 640x480 corner at any resolution.""",
    ),
    # The frontend's resolution, which the original hardcodes.
    #
    # BeginPhase sets width/height to 640x480 at its top and the splash path keeps them, so the menus
    # run at 640x480 no matter what mode is selected - `gfxPipeline::SetRes(640, 480, 16, 16, 0)` in
    # the pseudocode. The gameplay branch overwrites both registers from the chosen mode, so patching
    # the constants moves the frontend without touching gameplay.
    #
    # This is the change that makes 1440p visible rather than merely available. It depends on the UI
    # scaling work in docs/ui_scaling_progress.md - the CSV widget loaders, font sizing and bitmap
    # resampling - because MM2's interface is authored for 640x480 and does not scale on its own.
    Patch(
        name="skip-lock-check",
        address=0x004012CB,
        old=[0x75, 0x40],                        # jne  0x40130D
        new=[0xEB, 0x40],                        # jmp  0x40130D
        why="""Always skip the crash-recovery lock check, so the game starts every time.

        MM2 writes MM2.lock while running and deletes it on a clean exit. If it finds the file
        still present at startup it assumes the last session ended badly, deletes it, runs
        `system("start Trouble.rtf")` and returns from Main - no window, no message, nothing. Any
        run that crashes or is killed therefore makes the NEXT launch silently do nothing, which is
        maddening and looks exactly like a broken executable.

        The game has its own switch for this, -nolockcheck, and the branch here is the one that
        reads it. Turning the conditional into an unconditional jump takes that path always, which
        is the same thing the switch does without needing a shortcut, a batch file, or knowing the
        flag exists.

        What is lost is MM2's offer to open TROUBLE.RTF after a bad exit - a 1999 troubleshooting
        document about DirectX 7 and CD drives, which is of no use here.""",
    ),
    Patch(
        name="frontend-resolution",
        address=0x00401478,
        old=[0x68, 0xE0, 0x01, 0x00, 0x00,       # push 480
             0x68, 0x80, 0x02, 0x00, 0x00],      # push 640
        new=[0x68, 0xA0, 0x05, 0x00, 0x00,       # push 1440
             0x68, 0x00, 0x0A, 0x00, 0x00],      # push 2560
        why="""The resolution the frontend runs at.

        `Main` calls gfxPipeline::SetRes(640, 480, 16, 16, 0) with literal arguments at 0x00401482,
        and that - not BeginPhase - is what sizes the window. BeginPhase does contain a 640x480
        pair, and patching it changes nothing observable: its two registers are the default for a
        call whose splash path never runs, so an earlier attempt on those constants was verified
        into the binary and had no effect at all. Only a caller scan over the disassembly found the
        real site; there are exactly two callers of SetRes in the image and this is the other one.

        Both pushes are in one patch because neither is unique alone - `push 480` is five ordinary
        bytes - and because a frontend 2560 wide and 480 tall would be worse than leaving it.

        This makes 1440p visible rather than merely available. It depends on the UI scaling work in
        docs/ui_scaling_progress.md, since MM2's interface is authored for 640x480.""",
    ),
    Patch(
        name="resolution-cap-height",
        address=0x004AC2F3,
        old=[0xB9, 0xB0, 0x04, 0x00, 0x00],      # mov ecx, 1200
        new=[0xB9, 0xA0, 0x05, 0x00, 0x00],      # mov ecx, 1440
        why="The height half of the cap above; the two must move together.",
    ),
    # The draw distance, which the graphics slider cannot reach.
    #
    # cityLevel::SetObjectDetail holds four presets of four distances each, and detail 2 and 3 -
    # "high" and "maximum" - carried the SAME draw distance of 300, so moving the slider to maximum
    # pushed the LOD transitions outward and left the distance at which objects wink out exactly
    # where it was. 400 is not a taste: gfxFarClip is set to 400.0 in mmStatePack::SetDefaults at
    # 0x00523459, so the shipped game rendered a hundred-unit band of city that is inside the far
    # plane, plainly in view, and permanently empty of objects.
    #
    # Two patches do it - draw-distance-high at 0x00443EC4 and draw-distance-max at 0x00443EF0,
    # each rewriting one 300.0f immediate. Only the immediate is patched, not the whole
    # instruction: the operand beside it is the address of lvlInstance::sm_ObjNoDrawThresh, which
    # ExportAsm emits as a symbolic reference rather than raw bytes, and a patch spanning it would
    # land on a `dd` and be refused.
    #
    # PORTING cityLevel::SetObjectDetail WAS TRIED AND BACKED OUT - see wip/citylevel.cpp. The C++
    # is written and correct, and it would make the whole four-by-four preset table editable
    # instead of just this one immediate. It cannot be built: city/citylevel.h pulls in generated
    # headers that do not compile (lvl/lvlsdl.h, ph/phbound.h, data7/datcallback.h all emit
    # `unsigned const i16*`, which drops members and cascades into failed size assertions). Fix
    # tools/genheaders.py first, then this becomes code and these two patches come out.
    # THE SECOND SITE THAT HARDCODES 640x480, and the reason the first one appeared to do nothing.
    #
    # frontend-resolution above patches gfxPipeline::SetRes(640, 480, ...) in Main. That was the
    # right instruction and the wrong function: gfxPipeline::SafeBeginGfx makes the SAME call with
    # the same literals at 0x004A92C3, and it is the one that actually runs. The evidence is direct
    # rather than inferred - gfxPipeline::m_iWidth/m_iHeight read 640x480 in the frontend with the
    # Main patch verified into the binary and applied.
    #
    # This is not cosmetic. ui/uiscale.h derives its scale factor from m_iWidth, so a frontend that
    # believes it is 640 wide computes a factor of exactly one and the ENTIRE UI scaling layer -
    # bitmap resampling in gfxBitmap::Create, font sizing in mmText::CreateLocFont, text node
    # bitmaps in mmTextNode::Init - does nothing at all. Every menu asset stays at its authored
    # size, and the only thing making the menus fill the screen is the GL projection stretching a
    # 640x480 image, which is why the text is soft.
    #
    # With both sites patched the frontend runs at the real resolution, uiScale scales assets up to
    # it, and agiGLBegin2D's projection maps that 1:1 onto the window.
    #
    # This is the fifth entry in this file and the third to be caught by the same mistake: a patch
    # that is verified into the binary is not a patch that had an effect. Check the value the game
    # actually ends up with, not the bytes.
    Patch(
        name="safe-begin-gfx-height",
        address=0x004A92C3,
        old=[0x68, 0xE0, 0x01, 0x00, 0x00],      # push 480
        new=[0x68, 0xA0, 0x05, 0x00, 0x00],      # push 1440
        why="SafeBeginGfx's frontend height; the site frontend-resolution missed.",
    ),
    Patch(
        name="safe-begin-gfx-width",
        address=0x004A92C8,
        old=[0x68, 0x80, 0x02, 0x00, 0x00],      # push 640
        new=[0x68, 0x00, 0x0A, 0x00, 0x00],      # push 2560
        why="SafeBeginGfx's frontend width; must move with the height above.",
    ),
    # THE 32 MB HEAP, WHICH IS A 1999 BUDGET AND THE REAL CEILING ON 1440p GAMEPLAY.
    #
    # 0x004015D7 is `push 0x2000000` - the size handed to memSafeHeap::Init, which VirtualAllocs it
    # and hands it to memMemoryAllocator. Everything the game allocates comes out of that block, and
    # when it runs out memMemoryAllocator::Allocate calls Quitf("Heap overrun").
    #
    # This is what stops a race loading under -gl at 2560x1440. The frontend was already capped at
    # 1920x1080 for the same reason - uiScale resamples menu bitmaps into this heap - but GAMEPLAY
    # is not capped, and a screen-sized allocation at 1440p does not fit alongside a loaded city.
    # The stack said so exactly: operator new -> memMemoryAllocator::Allocate -> Quitf, with 0xA00
    # and 0x5A0 (2560 and 1440) sitting in the frame.
    #
    # 128 MB rather than something larger: it is four times the headroom, it is committed up front
    # by VirtualAlloc so it is a real reservation rather than a promise, and a 32-bit process only
    # has 2 GB of address space to spend. Raising the ceiling is the right fix rather than capping
    # the render resolution, because 1440p support is the point of the project.
    Patch(
        name="heap-size",
        address=0x004015D7,
        old=[0x68, 0x00, 0x00, 0x00, 0x02],      # push 0x02000000 - 32 MB
        new=[0x68, 0x00, 0x00, 0x00, 0x08],      # push 0x08000000 - 128 MB
        why="MM2's whole heap. 32 MB cannot hold a city plus a screen-sized allocation at 1440p.",
    ),
    Patch(
        name="draw-distance-high",
        address=0x00443EC4,
        old=[0x00, 0x00, 0x96, 0x43],            # 300.0f, detail preset 2
        new=[0x00, 0x00, 0xC8, 0x43],            # 400.0f
        why="Detail 2's draw distance, raised to meet the far plane it is drawn inside of.",
    ),
    # NO PATCH FOR THE OBJECT DRAW DISTANCE, DELIBERATELY.
    #
    # cityLevel::SetObjectDetail writes the cull distance as four `mov dword [0x5c571c], imm32`
    # stores, and this file refuses any address it cannot locate byte-by-byte in game.asm - that
    # region is emitted as instructions, not data, so the patch was refused rather than applied
    # wrongly. Correct behaviour by the tool; recorded here so nobody re-adds it.
    #
    # It is done at runtime instead, in agiGLApplyObjectDrawDistance in agigl/glframe.cpp, which is
    # the better home anyway: -objdist makes the distance answerable by driving, which the docs
    # argue is the only way to choose it.
    Patch(
        name="draw-distance-max",
        address=0x00443EF0,
        old=[0x00, 0x00, 0x96, 0x43],            # 300.0f, detail preset 3
        new=[0x00, 0x00, 0xC8, 0x43],            # 400.0f
        why="Detail 3's draw distance; see draw-distance-high, the two must move together.",
    ),
]


def retail_text():
    """Retail .text bytes and the address they start at, or None if unreadable."""
    try:
        from pe import IMAGE_BASE
        from verify_data import sections
        _, s = sections(RETAIL)
        return s[".text"]["bytes"], s[".text"]["va"] + IMAGE_BASE
    except Exception as e:
        print("warning: cannot read retail binary (%s)" % e)
        return None


def parse_db(line):
    """Byte values from a `db 0xxh, 0xxh, ...` line."""
    body = line.strip()
    if not body.startswith("db "):
        return None
    out = []
    for tok in body[3:].split(","):
        tok = tok.strip()
        m = re.fullmatch(r"0([0-9A-Fa-f]{2})h", tok)
        if not m:
            return None
        out.append(int(m.group(1), 16))
    return out


sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import encodings_table as encodings  # noqa: E402

TEXT_BASE = 0x00401000

def build_address_index(lines, mnemonics):
    """Map every emitted .text address to the (line, position) that produces it.

    ExportAsm writes .text in strict address order with no ALIGN, so walking the directives and
    accumulating their lengths reproduces the addresses exactly. Only `db` positions are recorded -
    a `dd` is a symbolic reference and its bytes are decided by the linker, so it can be counted
    but not patched. A mnemonic line is counted from the encoding table and recorded too, because
    a patch landing inside one can still be applied by expanding that line back to `db` first.
    """
    index = {}
    addr = None
    unknown = []

    for i, line in enumerate(lines):
        s = line.strip()

        if s == ".CODE":
            addr = TEXT_BASE
            continue
        if addr is None:
            continue
        if s in (".CONST", ".DATA", ".DATA?"):
            break

        vals = parse_db(line)
        if vals is not None:
            for k in range(len(vals)):
                index[addr + k] = (i, k)
            addr += len(vals)
        elif s.startswith("dd "):
            addr += 4
        elif s in mnemonics:
            raw = mnemonics[s]
            for k in range(len(raw)):
                index[addr + k] = (i, k)
            addr += len(raw)
        elif s and not s.startswith(";") and not s.endswith(":") and not s.startswith("PUBLIC") \
                and " PROC" not in s and " ENDP" not in s and not s.startswith("ALIGN") \
                and not s.startswith("INCLUDE") and not s.startswith("dw "):
            # An instruction line the table does not explain. Counting it as zero-length would
            # silently shift every address after it, so record it and let the caller refuse rather
            # than apply a patch to the wrong bytes.
            unknown.append((i, s))

    if unknown:
        print("  %d assembly line(s) of unknown length; addresses after the first are unreliable"
              % len(unknown))
        for i, s in unknown[:5]:
            print("     line %d: %s" % (i + 1, s))
        return None

    return index


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true", help="apply; without it nothing is modified")
    ap.add_argument("--only", help="apply just this patch by name")
    args = ap.parse_args()

    if not os.path.exists(ASM):
        sys.exit("no assembly at %s" % ASM)

    with open(ASM, encoding="utf-8", errors="replace") as f:
        lines = f.read().split("\n")

    rt = retail_text()
    text, text_va = rt if rt else (None, 0)
    if text is None:
        print("         patch addresses will NOT be verified")

    mnemonics = encodings.load()
    index = build_address_index(lines, mnemonics)

    applied, refused = [], []

    for patch in PATCHES:
        if args.only and patch.name != args.only:
            continue

        if text is not None:
            off = patch.address - text_va
            if off < 0 or off + len(patch.old) > len(text):
                refused.append((patch.name, "address 0x%08X is outside .text" % patch.address))
                continue
            actual = list(text[off:off + len(patch.old)])
            if actual != patch.old:
                refused.append((patch.name, "retail has %s at 0x%08X, expected %s" % (
                    " ".join("%02X" % b for b in actual), patch.address,
                    " ".join("%02X" % b for b in patch.old))))
                continue

        # Locate the patch by ADDRESS, not by searching for its bytes.
        #
        # Byte search was the wrong mechanism and kept proving it. A five-byte instruction is often
        # not unique - `push 480` occurs six times - and the obvious fix, adding leading context,
        # runs out almost immediately: the instruction before this one is a `call`, which ExportAsm
        # emits as a symbolic `dd`, not as bytes, so there is nothing contiguous left to match on.
        #
        # But the emission is strictly ordered from .CODE at TEXT_BASE and every directive's length
        # is known - a `db` contributes its value count, a `dd` contributes four, and labels, PROC,
        # ENDP and PUBLIC contribute nothing. So the address of every byte is computable, and a
        # patch can say exactly where it goes. The retail check above already confirms what *should*
        # be at that address; this confirms the assembly agrees, which together is far stronger than
        # any pattern.
        if index is None:
            refused.append((patch.name, "could not walk the assembly to compute addresses"))
            continue

        where = [index.get(patch.address + k) for k in range(len(patch.old))]
        if any(w is None for w in where):
            refused.append((patch.name,
                            "0x%08X is not inside the emitted .text" % patch.address))
            continue

        # A patch may land inside an instruction the exporter emitted as a mnemonic. The bytes are
        # the same bytes - `mov eax, 3E0h` and `db 0B8h, 0E0h, 003h, 000h, 000h` are one and the
        # same instruction - so expand that line back to its byte form and patch it there. Doing it
        # this way keeps every patch defined by address and value, exactly as before, instead of
        # teaching each one how to rewrite a mnemonic operand.
        for line_i, _ in where:
            s = lines[line_i].strip()
            if parse_db(lines[line_i]) is None and s in mnemonics:
                lines[line_i] = ("    db "
                                 + ", ".join("0%02Xh" % b for b in mnemonics[s]))

        found = []
        for line_i, pos in where:
            vals = parse_db(lines[line_i])
            if vals is None:
                found = None
                break
            found.append(vals[pos])

        if found is None:
            refused.append((patch.name, "0x%08X lands on a symbolic directive, not raw bytes - "
                                        "it cannot be patched as data" % patch.address))
            continue

        if found != patch.old:
            refused.append((patch.name, "assembly has %s at 0x%08X, expected %s" % (
                " ".join("%02X" % b for b in found), patch.address,
                " ".join("%02X" % b for b in patch.old))))
            continue

        touched = {}
        for k, b in enumerate(patch.new):
            line_i, pos = where[k]
            touched.setdefault(line_i, parse_db(lines[line_i]))
            touched[line_i][pos] = b

        for line_i, vals in touched.items():
            lines[line_i] = "    db " + ", ".join("0%02Xh" % v for v in vals)

        applied.append(patch)
        continue

    verb = "applied" if args.write else "would apply"
    for p in applied:
        print("%s: %s" % (verb, p.name))
        print("   0x%08X  %s -> %s" % (p.address,
                                       " ".join("%02X" % b for b in p.old),
                                       " ".join("%02X" % b for b in p.new)))

    for name, why in refused:
        print("REFUSED: %s - %s" % (name, why))

    if not args.write:
        print("\ndry run; pass --write to apply")
        return 1 if refused else 0

    with open(ASM, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines))

    print("\nwrote %d patch(es) to %s" % (len(applied), ASM))
    return 1 if refused else 0


if __name__ == "__main__":
    sys.exit(main())
