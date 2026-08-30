# -*- coding: utf-8 -*-
"""Build OpenMM2 end to end, refusing to continue past anything unverified.

    py tools/build.py

The whole pipeline in one command:

    merge ported lists -> regenerate headers -> compile -> VERIFY SYMBOLS
      -> strip PROCs from game.asm -> assemble -> link -> VERIFY LAYOUT

The two verify steps are the point. Both failures they catch are silent:

  SYMBOLS - a reimplementation only replaces the original if its mangled name matches exactly.
            If it does not, the object contributes an unreferenced symbol, the assembly's version
            stays in the link, and the game keeps running 1999 code. Nothing errors. The port looks
            finished and is not. (`bool` mangles `_N`, `b32` mangles `H` - that one alone would
            have silently disabled every ported predicate.)

  LAYOUT  - midtown2.exe has no relocation table, so any absolute address that was not symbolised
            is only correct while the image keeps its original layout. A build that shifts .text
            still links and still looks fine. So the entry point and two known function addresses
            are checked against the retail binary on every build.

Fail fast: a failing gate stops the build rather than producing an executable nobody should trust.
"""
import glob
import json
import os
import shutil
import struct
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

sys.path.insert(0, HERE)

ASM_DIR = os.environ.get("MM2_ASM_DIR", r"C:\mm2ghidra\out")
ASM = os.path.join(ASM_DIR, "game.asm")
ASM_PRISTINE = os.path.join(ASM_DIR, "game.asm.pristine")
GAME_OBJ = os.path.join(ASM_DIR, "game.obj")

OBJ_DIR = os.path.join(ROOT, "build", "obj")
CODE = os.path.join(ROOT, "code", "midtown2")
PORTED = os.path.join(ROOT, "data", "ported.json")

RETAIL = os.environ.get("MM2_EXE", r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\midtown2.exe")
OUT_EXE = os.path.join(ROOT, "build", "OpenMM2.exe")

# Checked on every build. If any of these move, the layout guarantee is broken.
LAYOUT_ANCHORS = [
    (0x00448330, "sdlPage16::Draw"),
    (0x00543E40, "aiPath::aiPath"),
]
ENTRY_RVA = 0x00181F5C


def tool(name):
    hits = sorted(glob.glob(
        r"C:\Program Files\Microsoft Visual Studio\*\*\VC\Tools\MSVC\*\bin\Hostx64\x86\%s" % name))
    if not hits:
        sys.exit("could not find %s" % name)
    return hits[-1]


def run(cmd, what):
    res = subprocess.run(cmd, capture_output=True, text=True)
    if res.returncode != 0:
        print((res.stdout or "") + (res.stderr or ""))
        sys.exit("FAILED: %s" % what)
    return res


def step(n, text):
    print("\n[%d] %s" % (n, text))


def merge_ported():
    """Fold every data/ported_*.json into data/ported.json.

    Each agent writes its own file so concurrent work cannot clobber a shared one; this is where
    they come back together. Duplicates are an error, not something to silently dedupe - the same
    symbol implemented twice means two definitions and a link failure, and it is better to hear
    about it here.
    """
    with open(PORTED, encoding="utf-8") as f:
        base = json.load(f)

    known = list(base.get("ported", []))
    seen = set(known)
    added = 0
    clashes = []

    for path in sorted(glob.glob(os.path.join(ROOT, "data", "ported_*.json"))):
        with open(path, encoding="utf-8") as f:
            part = json.load(f)

        for sym in part.get("ported", []):
            if sym in seen:
                clashes.append((os.path.basename(path), sym))
                continue
            seen.add(sym)
            known.append(sym)
            added += 1

    if clashes:
        print("  duplicate symbols across ported lists:")
        for src, sym in clashes[:10]:
            print("    %s: %s" % (src, sym))

    base["ported"] = sorted(known)

    with open(PORTED, "w", encoding="utf-8", newline="\n") as f:
        json.dump(base, f, indent=1)

    print("  %d symbols marked ported (%d newly merged)" % (len(known), added))
    return len(known)


def compile_sources(cl):
    """Compile every .cpp under code/midtown2 to build/obj."""
    sdk = sorted(glob.glob(r"C:\Program Files (x86)\Windows Kits\10\Include\*\ucrt"))[-1]

    # cl.exe lives at <MSVC>/bin/Hostx64/x86/cl.exe, so the toolset root is four levels up - three
    # only reaches <MSVC>/bin, and the missing level is why <cstddef> could not be found.
    msvc_root = cl
    for _ in range(4):
        msvc_root = os.path.dirname(msvc_root)
    msvc_inc = os.path.join(msvc_root, "include")

    if not os.path.isdir(msvc_inc):
        sys.exit("MSVC include directory not found at %s" % msvc_inc)

    # Work in progress, kept out of the build on purpose - see wip/README.md.
    #
    # This is a skip list rather than a directory move because agents write into code/ while the
    # build runs, and a file moved aside simply reappears. The harness is only worth having if a
    # runnable binary can be produced at any moment: the whole design is that the game plays at 0%
    # reimplemented, and every bug found tonight was found by running it. One source file that does
    # not compile takes that away.
    WIP = tuple(w for w in os.environ.get("MM2_WIP", "").split(",") if w)

    sources = []
    for dirpath, _, files in os.walk(CODE):
        for name in sorted(files):
            if name.endswith(".cpp"):
                rel = os.path.join(dirpath, name)[len(CODE) + 1:].replace("\\", "/")
                if any(rel.startswith(w) or rel == w for w in WIP):
                    continue
                sources.append(os.path.join(dirpath, name))

    if not sources:
        sys.exit("no .cpp files found under %s" % CODE)

    os.makedirs(OBJ_DIR, exist_ok=True)

    # Start from nothing. Every .cpp is recompiled each build, so a leftover object can only be
    # stale - and a stale one is actively harmful: quarantine had been restoring a held-back
    # object AFTER the compile step, clobbering the fresh build with the old one and then
    # re-quarantining it on symbols the source no longer defines.
    for stale in glob.glob(os.path.join(OBJ_DIR, "*.obj")) +                  glob.glob(os.path.join(ROOT, "build", "pending", "*.obj")):
        os.remove(stale)

    print("  %d source files" % len(sources))

    for src in sources:
        obj = os.path.join(OBJ_DIR, os.path.splitext(os.path.basename(src))[0] + ".obj")

        # /arch:IA32 keeps the compiler on x87, matching the 1999 build. With SSE2 the float
        # results differ in the low bits, and physics turns that into visibly different driving.
        #
        # /GS- and /GR- match it too. The stack-cookie check pulls in @__security_check_cookie@4,
        # which the game's own 1999 CRT does not provide - and adding a modern CRT to satisfy it
        # would put two CRTs in one image. midtown2.exe was built without RTTI as well (/GR-), so
        # enabling it here would only add vtable metadata the original never had.
        # C++14 sized deallocation is off, because midtown2.exe does not have it.
        #
        # A class with a virtual destructor gets a compiler-generated "scalar deleting destructor",
        # and by default that calls operator delete(void*, size_t) - ??3@YAXPAXI@Z. The 1999 binary
        # only exports the unsized ??3@YAXPAX@Z, so the first ported class with a virtual destructor
        # fails to link against a function the game has never had.
        cmd = [cl, "-nologo", "-c",
               # Exceptions off. midtown2.exe has no C++ exception handling, and a reimplementation
               # that unwinds where the original could not would diverge in a way nothing here can
               # check. With -EHsc the compiler emits unwind tables referencing __CxxFrameHandler3,
               # which this build has no CRT to supply - the link fails the moment any function
               # needs one.
               "-EHs-c-",
               "-O2", "-arch:IA32", "-W3", "-GS-", "-GR-",
               "-Zc:sizedDealloc-",

               # Compile the OpenGL device layer. This does NOT enable it: agiGLEnabled() still
               # returns false unless -gl is on the command line, and nothing under agigl/ is
               # registered in data/ported.json, so no original PROC is stripped. It only means the
               # code is built and linked, which is the prerequisite for testing any of it.
               "-DARTS_AGIGL",
               "-DARTS_AGIGL_PHASE",
               "-I" + CODE, "-I" + msvc_inc, "-I" + sdk, "-Fo" + obj, src]

        res = subprocess.run(cmd, capture_output=True, text=True)
        out = (res.stdout or "") + (res.stderr or "")

        if res.returncode != 0:
            print(out)
            sys.exit("FAILED: compiling %s" % os.path.basename(src))

        for line in out.splitlines():
            if "warning" in line.lower():
                print("    %s" % line.strip())

    return len(sources)


def quarantine_unregistered():
    """Hold back objects defining game symbols that data/ported.json does not list.

    Agents write the implementation and its ported list as separate files, so between those two
    writes there is a .cpp whose symbols nothing has registered. Linking it fails outright: the
    assembly still defines those symbols PUBLIC, because tools/asm.py only strips what is
    registered, so the linker sees two definitions of every one.

    The two failure directions are NOT symmetric:

      registered but not compiled -> hard failure. Something claims to be ported and is not, and
                                     asm.py will have stripped the original, leaving nothing there.
      compiled but not registered -> in-flight work. Hold the object back, say so, carry on.

    Held objects move to build/pending/ rather than being deleted, so the next build picks them up
    automatically once the agent's ported list lands.
    """
    with open(PORTED, encoding="utf-8") as f:
        registered = set(json.load(f).get("ported", []))

    with open(os.path.join(ROOT, "data", "symbols.json"), encoding="utf-8") as f:
        GAME_SYMBOLS = {s["mangled"] for s in json.load(f)}

    dumpbin = tool("dumpbin.exe")
    pending_dir = os.path.join(ROOT, "build", "pending")
    os.makedirs(pending_dir, exist_ok=True)

    held = []

    for obj in sorted(glob.glob(os.path.join(OBJ_DIR, "*.obj"))):
        res = subprocess.run([dumpbin, "-symbols", obj], capture_output=True, text=True)

        unregistered = set()
        for line in res.stdout.splitlines():
            if "External" not in line or "UNDEF" in line:
                continue

            parts = line.split("|", 1)
            if len(parts) < 2:
                continue

            sym = parts[1].split()[0]

            # Only a symbol that exists in midtown2.exe can mask an unported function.
            #
            # "Looks mangled and has @@ in it" was the wrong test. It held back every object in the
            # OpenGL layer, because that code introduces names like ?agiGLBeginDevice@@YA_NXZ which
            # appear in no version of the game - there is nothing for them to silently replace.
            # The generated Access_ helpers were the same story, handled by a prefix check that
            # this rule now covers on its own.
            if sym not in GAME_SYMBOLS:
                continue

            # An accessor generated by tools/genheaders.py, not a game symbol.
            #
            # Access is part of a mangled name, so a private static like
            # ?m_iWidth@gfxPipeline@@0HA cannot be declared public without becoming a different
            # symbol. The generator declares it correctly and adds an inline accessor beside it,
            # which MSVC emits as a COMDAT external. There is no Access_ anything in midtown2.exe,
            # so none of these can silently replace original code - which is the only thing this
            # quarantine exists to prevent.
            if sym.startswith("?Access_"):
                continue

            # A game symbol is mangled and is not a compiler-generated helper.
            if sym.startswith("?") and not sym.startswith("??_") and "@@" in sym:
                if sym not in registered:
                    unregistered.add(sym)

        if unregistered:
            shutil.move(obj, os.path.join(pending_dir, os.path.basename(obj)))
            held.append((os.path.basename(obj), len(unregistered)))

    for name, count in held:
        print("  holding back %s - %d symbols not yet in data/ported.json" % (name, count))

    if held:
        print("  (an agent is still writing these; they link once its ported list lands)")

    return held


def verify_layout():
    """The built image must keep the retail addresses."""
    def sections(path):
        with open(path, "rb") as f:
            d = f.read()
        pe = struct.unpack_from("<I", d, 0x3C)[0]
        n = struct.unpack_from("<H", d, pe + 6)[0]
        opt = pe + 24
        oep = struct.unpack_from("<I", d, opt + 16)[0]
        tbl = opt + struct.unpack_from("<H", d, pe + 20)[0]
        secs = {}
        for i in range(n):
            b = tbl + i * 40
            name = d[b:b + 8].rstrip(b"\0").decode()
            vsz, va, rsz, rp = struct.unpack_from("<IIII", d, b + 8)
            secs[name] = (va, vsz, rp, rsz)
        return d, oep, secs

    retail, _, rsec = sections(RETAIL)
    built, oep, bsec = sections(OUT_EXE)

    ok = True

    if oep != ENTRY_RVA:
        print("  ENTRY MOVED: 0x%06X, expected 0x%06X" % (oep, ENTRY_RVA))
        ok = False
    else:
        print("  entry point   0x%06X  (unchanged)" % oep)

    tva, _, trp, trsz = bsec[".text"]
    text = built[trp:trp + trsz]

    for va, label in LAYOUT_ANCHORS:
        want = retail[va - 0x400000: va - 0x400000 + 16]
        at = text.find(want)
        found = (tva + at + 0x400000) if at >= 0 else 0

        if found == va:
            print("  %-18s 0x%08X  (unchanged)" % (label, va))
        else:
            print("  %-18s MOVED to 0x%08X, expected 0x%08X" % (label, found, va))
            ok = False

    # .text must start where it always did - everything else is measured from it.
    if bsec[".text"][0] != rsec[".text"][0]:
        print("  .text  at 0x%06X, retail has 0x%06X" % (bsec[".text"][0], rsec[".text"][0]))
        ok = False

    # .rdata and .data are DISPLACED, and it is not a regression - it is architectural.
    #
    # Two causes, neither fixable from here: the linker builds its own import table into .rdata on
    # top of the original IAT we emit as data, and the reimplemented C++ has to live somewhere, so
    # it grows .text past its retail size. #pragma code_seg was tried and made it worse (MSVC link
    # groups code sections together regardless of name).
    #
    # Failing the build on this would mean never building again, which helps nobody. It is
    # reported on every build so the number stays visible, and the real fix - symbolising the
    # remaining data pointers so layout stops mattering - is tracked in docs/harness.md.
    for name in (".rdata", ".data"):
        delta = bsec[name][0] - rsec[name][0]
        if delta:
            print("  %-6s at 0x%06X, retail 0x%06X  (+0x%X displaced - known, see docs/harness.md)"
                  % (name, bsec[name][0], rsec[name][0], delta))

    return ok


def main():
    cl = tool("cl.exe")
    ml = tool("ml.exe")

    if not os.path.exists(ASM_PRISTINE):
        if not os.path.exists(ASM):
            sys.exit("no game.asm at %s - run tools/ghidra/ExportAsm.java first" % ASM)
        shutil.copy(ASM, ASM_PRISTINE)
        print("saved a pristine copy of game.asm")

    # ExportAsm.java writes game.asm.pristine itself, so the two are in sync by construction. What
    # this catches is the other half of the same mistake: editing the generator and then building
    # without re-exporting. Every fix would verify correctly in the generator and silently not be
    # in the binary, which is exactly how a whole evening's work got thrown away once already.
    generator = os.path.join(HERE, "ghidra", "ExportAsm.java")
    if os.path.exists(generator) and os.path.getmtime(generator) > os.path.getmtime(ASM_PRISTINE):
        sys.exit("FAILED: ExportAsm.java is newer than the exported assembly - "
                 "re-run ExportAsm.java, or the build will use the previous export.")

    step(1, "merging ported lists")
    merge_ported()

    # Before genheaders, because this is the one check on the INPUT to header generation rather
    # than on its output. check_size compares the emitted header against the number recorded
    # beside it, and both come from the same layouts.json entry - so it can only catch a header
    # that has drifted from its own source, never a source that is wrong. This asks midtown2.exe.
    #
    # Deliberately NOT fatal. It reports 25 classes today, every one of them a known, documented
    # disagreement with a real size and no member list to write beside it (docs/short_layouts.md).
    # Failing the build on them would block every unrelated port for as long as they stay open,
    # and the point of the gate is that they stop being invisible - not that they stop the world.
    # The list reached zero, so this is fatal now, as the note here used to ask for.
    step(2, "checking layouts.json sizes against what the binary allocates")
    res = subprocess.run([sys.executable, os.path.join(HERE, "verify_sizes.py")],
                         capture_output=True, text=True)
    for line in res.stdout.splitlines():
        if line.strip():
            print("  " + line.strip())
    if res.returncode != 0:
        sys.exit("FAILED: a layouts.json size disagrees with what midtown2.exe allocates - a ported "
                 "new() would under-allocate and the still-assembly code would write past the end")

    step(3, "regenerating headers")
    run([sys.executable, os.path.join(HERE, "genheaders.py")], "genheaders")
    print("  headers regenerated")

    step(4, "compiling")
    n = compile_sources(cl)
    print("  %d objects" % n)

    step(5, "verifying symbols")
    quarantine_unregistered()

    res = subprocess.run([sys.executable, os.path.join(HERE, "verify_symbols.py")],
                         capture_output=True, text=True)
    print("\n".join("  " + l for l in res.stdout.splitlines()[-6:]))
    if res.returncode != 0:
        sys.exit("FAILED: symbol verification - a port would silently not take effect")

    # Before anything is stripped, not after: this is the one gate whose failure mode is deleting
    # a function that is still referenced, and the byte verifiers cannot see it because the bytes
    # it destroys are inside a region that is *expected* to differ.
    step(6, "verifying ported extents do not swallow other code")
    res = subprocess.run([sys.executable, os.path.join(HERE, "verify_extents.py")],
                         capture_output=True, text=True)
    for line in res.stdout.splitlines():
        if line.strip():
            print("  " + line.strip())
    if res.returncode != 0:
        sys.exit("FAILED: a ported function's extent swallows code that data still points into")

    step(7, "stripping ported functions from the assembly")
    # Always start from the pristine assembly: stripping is not idempotent, and re-stripping an
    # already-stripped file would pad the padding.
    shutil.copy(ASM_PRISTINE, ASM)
    env = dict(os.environ, MM2_ASM=ASM)

    # Deliberate behavioural changes go on here, between the restore and the strip: after the
    # restore because it would otherwise overwrite them, before the strip because a patched
    # function may itself be one that gets stripped. Each patch verifies its own address against
    # the retail binary and refuses rather than writing if anything does not match.
    res = subprocess.run([sys.executable, os.path.join(HERE, "patches.py"), "--write"],
                         capture_output=True, text=True, env=env)
    for line in res.stdout.splitlines():
        if line.strip() and not line.startswith("dry run"):
            print("  " + line.strip())
    if res.returncode != 0:
        sys.exit("FAILED: a deliberate patch refused to apply - see the reason above")

    res = subprocess.run([sys.executable, os.path.join(HERE, "asm.py"), "--write"],
                         capture_output=True, text=True, env=env)
    print("\n".join("  " + l for l in res.stdout.splitlines()[-4:]))
    if res.returncode != 0:
        sys.exit("FAILED: asm.py")

    step(8, "assembling")
    res = subprocess.run([ml, "-c", "-coff", "-Fo" + GAME_OBJ, "game.asm"],
                         capture_output=True, text=True, cwd=ASM_DIR)
    errors = [l for l in ((res.stdout or "") + (res.stderr or "")).splitlines() if "error A" in l]
    if errors:
        for l in errors[:10]:
            print("  " + l)
        sys.exit("FAILED: %d assembler errors" % len(errors))
    print("  game.obj %.1f MB" % (os.path.getsize(GAME_OBJ) / 1048576.0))

    # Before linking, because the linker takes the .res as an input. Never fatal: it prints what
    # went wrong and returns, since a missing icon is not a reason to fail a correct build.
    subprocess.run([sys.executable, os.path.join(HERE, "mkres.py")])

    step(9, "linking")
    res = subprocess.run([sys.executable, os.path.join(HERE, "link.py")],
                         capture_output=True, text=True)
    tail = [l for l in res.stdout.splitlines() if l.strip()][-3:]
    print("\n".join("  " + l for l in tail))
    if res.returncode != 0:
        sys.exit("FAILED: link")

    step(10, "verifying layout against the retail binary")
    if not verify_layout():
        sys.exit("FAILED: layout moved - unsymbolised absolute addresses are now wrong")

    step(11, "verifying .text byte-for-byte against retail")
    res = subprocess.run([sys.executable, os.path.join(HERE, "verify_text.py")],
                         capture_output=True, text=True)
    for line in res.stdout.splitlines():
        if any(k in line for k in ("differing", "PASS", "FAIL", "displacement", "longer")):
            print("  " + line.strip())
    if res.returncode != 0:
        sys.exit("FAILED: .text differs from retail in ways the pipeline cannot account for")

    step(12, "verifying .rdata/.data byte-for-byte against retail")
    res = subprocess.run([sys.executable, os.path.join(HERE, "verify_data.py")],
                         capture_output=True, text=True)
    for line in res.stdout.splitlines():
        if any(k in line for k in ("differing", "PASS", "FAIL", "displacements", "begins")):
            print("  " + line.strip())
    if res.returncode != 0:
        sys.exit("FAILED: data differs from retail in ways the pipeline cannot account for")

    # This gate is separate from the two above because it catches the opposite kind of failure.
    # Those ask whether a byte DIFFERS from retail and whether the difference is accounted for. An
    # unrelocated pointer does not differ from retail at all - it is wrong precisely because it was
    # left untouched - so it passes both of them and then breaks the game at runtime.
    step(13, "verifying DirectInput data formats")
    res = subprocess.run([sys.executable, os.path.join(HERE, "verify_dataformats.py")],
                         capture_output=True, text=True)
    for line in res.stdout.splitlines():
        if any(k in line for k in ("formats", "pointers", "PASS", "FAIL", "UNRELOCATED", "entry")):
            print("  " + line.strip())
    if res.returncode != 0:
        sys.exit("FAILED: GUID pointers left unrelocated - SetDataFormat will reject them")

    # Separate from the string gate because it catches what that one structurally cannot: format
    # strings are short and mostly punctuation, so a letter-count rule lets them through.
    step(14, "verifying printf format strings")
    res = subprocess.run([sys.executable, os.path.join(HERE, "verify_formats.py")],
                         capture_output=True, text=True)
    for line in res.stdout.splitlines():
        if line.strip():
            print("  " + line.strip())
    if res.returncode != 0:
        sys.exit("FAILED: a printf format string was relocated as if it were a pointer")

    step(15, "verifying string data survived the export")
    res = subprocess.run([sys.executable, os.path.join(HERE, "verify_strings.py")],
                         capture_output=True, text=True)
    for line in res.stdout.splitlines():
        if any(k in line for k in ("strings", "PASS", "FAIL", "DAMAGED", "want", "got", "retail")):
            print("  " + line.strip())
    if res.returncode != 0:
        sys.exit("FAILED: a string was rewritten as if it were a pointer")

    # THE INVERSE OF verify_text, AND THE ONE QUESTION IT CANNOT ASK.
    #
    # verify_text accepts a dword that differs from retail by a measured section displacement,
    # which is the right test for "was this relocated correctly". It is structurally blind to the
    # opposite mistake: a FALSE relocation, applied to four bytes that were never an address,
    # differs by exactly the same delta. Nothing that looks only at the value can separate them.
    #
    # This decodes .text instead and asks whether every changed dword is a WHOLE operand - an imm32
    # or a disp32 that capstone agrees is there. A window that ends where an operand ends but
    # starts inside the opcode is the shape that ships a corrupted structure offset.
    #
    # Cheap enough to run always: about six seconds against 1.7 MB of code.
    step(16, "verifying relocated operands are whole operands")
    res = subprocess.run([sys.executable, os.path.join(HERE, "verify_operand_windows.py")],
                         capture_output=True, text=True)
    for line in res.stdout.splitlines():
        if any(k in line for k in ("operand", "PASS", "FAIL", "section deltas", "decoded", "skipped")):
            print("  " + line.strip())
    if res.returncode != 0:
        sys.exit("FAILED: an operand changed by something other than a section delta")

    step(15, "recording progress")
    res = subprocess.run([sys.executable, os.path.join(HERE, "progress.py")],
                         capture_output=True, text=True)
    print("\n".join("  " + l for l in res.stdout.splitlines()[:3]))

    print("\nBUILD OK  ->  %s" % OUT_EXE)
    return 0


if __name__ == "__main__":
    sys.exit(main())
