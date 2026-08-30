// Export midtown2.exe as a MASM source that assembles and links.
//
// This is the harness the whole project depends on. It is what Open1560's game.asm is for Midtown
// Madness 1: the original binary, assembled and linked into the build, so there is a runnable
// executable on day one at 0% reimplemented. A function is ported by writing the C++, flipping
// ARTS_IMPORT to ARTS_EXPORT, and stripping its PROC out of here - so every function is verified
// against the real game the moment it is written, instead of debugging thousands of unknowns at
// the end.
//
// WHY BYTES AND NOT MNEMONICS.
//
// The obvious approach is to re-emit disassembly as MASM text. Ghidra's syntax is not MASM's -
// operand order, size prefixes, hex literal spelling and segment overrides all differ - and every
// one of those is a chance to silently change an instruction. Emitting the original bytes cannot
// change them. The only places that need real symbols are the ones holding an address, and those
// are patched in as `dd`:
//
//     db 0E8h                       ; call rel32
//     dd ?Update@mmGameManager@@UAEXXZ - ($ + 4)
//
// The linker then resolves that to the C++ version once the function is ported, which is the
// entire point of the harness. Everything else stays byte-identical to the retail binary.
//
// DATA IS EMITTED AS ONE CONTIGUOUS BLOCK PER SECTION, with a label at every known symbol. That
// preserves the original intra-section layout exactly, which matters because midtown2.exe has
// IMAGE_FILE_RELOCS_STRIPPED set and an empty base-relocation directory - nothing in the file says
// which data words are pointers. A data word is only turned into a symbol reference when Ghidra
// recorded a reference there or the value is an exact match for a known symbol address. Guessing
// more aggressively would silently corrupt data that merely looks like a pointer.
//
//@category OpenMM2

import java.io.File;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.data.DataType;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolTable;

public class ExportAsm extends GhidraScript {

    static class Patch {
        int pos;
        String symbol;
        long addend;
        boolean relative;
        boolean indirect; // an import slot: the value is __imp__Name, not a normal symbol

        Patch(int pos, String symbol, long addend, boolean relative) {
            this.pos = pos;
            this.symbol = symbol;
            this.addend = addend;
            this.relative = relative;
        }
    }

    Map<Long, String> symbolAt = new HashMap<>();
    TreeMap<Long, String> sortedSymbols = new TreeMap<>();

    // Code addresses that are referenced but carry no symbol of their own - switch-case targets
    // and jump-table entries, mostly. Each one needs a label emitted at exactly its address, or
    // the reference has to fall back to nearest-symbol-plus-offset, which is wrong: `caseD_4 + 123`
    // is not a stable way to name a jump target and MASM cannot resolve it as a relative branch.
    Set<Long> neededLabels = new HashSet<>();

    // Referenced DATA addresses that carry no symbol of their own. Refusing to anchor these
    // across a section boundary is necessary but not sufficient: left as raw bytes they keep
    // their retail value, and .data is displaced in the linked image, so they still point at the
    // wrong place. Emitting a label at the exact address is the complete fix - the same treatment
    // code targets already get.
    Set<Long> neededDataLabels = new HashSet<>();

    // Addresses inside .text that hold a POINTER rather than an instruction.
    //
    // Const data lives in the code section: DirectInput's c_dfDIMouse points at a rgodf array at
    // 0x590B70 in .text, and each entry there holds a GUID pointer into .rdata. Emitted as raw
    // bytes those pointers keep their retail values, .rdata moves, and DirectInput rejects the
    // format with E_INVALIDARG - which is exactly the mouse failure on the splash screen.
    Set<Long> textPointers = new HashSet<>();
    Set<Long> dataFormatPointers = new HashSet<>();
    Set<Long> pointersEmitted = new HashSet<>();
    int stringsProtected = 0;

    // The sections emitDataBlock actually writes. A label is only useful inside one of them.
    static final Set<String> EMITTED_DATA =
            new HashSet<>(java.util.Arrays.asList(".rdata", ".data", ".rsrc"));

    // MASM keywords that Ghidra happily uses as label names. `Name` is a real one in this binary.
    static final Set<String> RESERVED = new HashSet<>(java.util.Arrays.asList(
            "NAME", "SIZE", "LENGTH", "TYPE", "MASK", "WIDTH", "OFFSET", "PTR", "SHORT", "NEAR",
            "FAR", "BYTE", "WORD", "DWORD", "QWORD", "FWORD", "TBYTE", "REAL4", "REAL8", "REAL10",
            "LOW", "HIGH", "THIS", "SEG", "LENGTHOF", "SIZEOF", "OPATTR", "END", "PROC", "ENDP",
            "SEGMENT", "GROUP", "ASSUME", "PUBLIC", "EXTERN", "ALIGN", "ORG", "EVEN", "LABEL",
            "RECORD", "STRUCT", "UNION", "MACRO", "PAGE", "TITLE", "COMMENT", "INCLUDE", "IF",
            "ELSE", "ENDIF", "REPEAT", "WHILE", "FOR", "GOTO", "DUP",
            // Instruction mnemonics are reserved too. A rebuilt database surfaced a CRT function
            // genuinely named "add", and `add PROC` is a syntax error, not a procedure - seven
            // assembler errors from one symbol. Any mnemonic can appear as a name, so list them
            // rather than wait to be surprised by the next one.
            "ADD", "SUB", "MUL", "IMUL", "DIV", "IDIV", "INC", "DEC", "NEG", "NOT",
            "AND", "OR", "XOR", "TEST", "CMP", "SHL", "SHR", "SAL", "SAR", "ROL", "ROR",
            "RCL", "RCR", "MOV", "MOVZX", "MOVSX", "LEA", "PUSH", "POP", "XCHG", "NOP",
            "CALL", "RET", "RETN", "JMP", "JE", "JNE", "JZ", "JNZ", "JA", "JB", "JG", "JL",
            "LOOP", "INT", "IN", "OUT", "REP", "REPE", "REPNE", "STR", "SET", "CLC", "STC",
            "CLD", "STD", "CLI", "STI", "HLT", "WAIT", "LOCK", "ENTER", "LEAVE", "CDQ", "CWD",
            "SCAS", "LODS", "STOS", "MOVS", "CMPS", "BT", "BTS", "BTR", "BTC", "BSF", "BSR",
            "FLD", "FST", "FSTP", "FADD", "FSUB", "FMUL", "FDIV", "FCOM", "FCHS", "FABS"));
    Map<Long, String> importAt = new HashMap<>(); // IAT slot address -> import name
    Set<String> emitted = new HashSet<>();
    Set<String> externProcs = new TreeSet<>();
    Set<String> externData = new TreeSet<>();
    Set<String> imports = new TreeSet<>();

    long patchedAbs = 0;
    long patchedRel = 0;
    long patchedImport = 0;
    long refNoSymbol = 0;
    long refNotLocated = 0;
    long dataPointers = 0;
    long crossSectionRefused = 0;
    long missedOperands = 0;
    long dataLabelsEmitted = 0;
    long recoveredPointers = 0;

    @Override
    public void run() throws Exception {
        String outDir = "C:/mm2ghidra/out";

        String[] args = getScriptArgs();
        if (args.length > 0) {
            outDir = args[0];
        }

        new File(outDir).mkdirs();

        buildSymbolIndex();
        buildImportIndex();

        println("symbol index: " + symbolAt.size() + " addresses, "
                + importAt.size() + " import slots");

        markCoverage();
        assignUniqueNames();
        findTextPointers();
        println("  [set] after findTextPointers: " + textPointers.size());
        findDataFormatPointers();
        println("  [set] after findDataFormatPointers: " + textPointers.size());
        disassembleGaps();
        println("  [set] after disassembleGaps: " + textPointers.size());
        protectTextPointers();
        println("  [set] after protectTextPointers: " + textPointers.size());
        {
            PrintWriter pw = new PrintWriter(new File(outDir, "textpointers.txt"), "UTF-8");
            List<Long> sorted = new ArrayList<>(textPointers);
            Collections.sort(sorted);
            for (long p : sorted) {
                pw.println(String.format("%08X %08X", p, readDword(p)));
            }
            pw.close();
        }
        markCoverage();   // coverage changed; recompute before collecting labels
        collectLabels();
        println("internal labels needed: " + neededLabels.size());

        List<Function> funcs = new ArrayList<>();
        FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
        while (it.hasNext()) {
            funcs.add(it.next());
        }
        funcs.sort(Comparator.comparingLong(f -> f.getEntryPoint().getOffset()));

        println("functions: " + funcs.size());

        PrintWriter w = new PrintWriter(new File(outDir, "game.asm"), "UTF-8");
        writeHeader(w);

        w.println(".CODE");
        w.println();

        int n = emitText(w);

        // Data. .rdata is read-only, .data holds both initialised bytes and the uninitialised
        // tail (virtual size far exceeds raw size), which has to go in .DATA? or the object file
        // would carry a megabyte of zeroes.
        emitDataBlock(w, ".rdata", ".CONST", outDir);
        emitDataBlock(w, ".data", ".DATA", outDir);

        // .rsrc is tiny but .rdata points into it, so it has to exist as addressable bytes or the
        // link fails. NOTE: this makes the addresses resolve, it does NOT rebuild a real PE
        // resource directory - LoadIcon and friends will need a proper .res at some point.
        emitDataBlock(w, ".rsrc", ".CONST", outDir);

        w.println();
        // MASM sets the entry point from END <label>. Passing it to the linker as /ENTRY instead
        // fails: link decorates the name and looks for _start, which does not exist in flat model.
        w.println(symbolAt.containsValue("start") ? "END start" : "END");
        w.close();

        // The export IS the pristine copy, so write it here rather than letting the build guess.
        //
        // build.py restores game.asm from game.asm.pristine before every build, because stripping
        // ported functions is not idempotent - re-stripping an already-stripped file would pad the
        // padding. That is correct. What was missing is that nothing ever refreshed the snapshot:
        // it was written once, the first time a build ran, and every export afterwards was silently
        // overwritten by it at build time. Hours of ExportAsm fixes verified perfectly in game.asm
        // and never reached the binary.
        java.nio.file.Files.copy(
                new File(outDir, "game.asm").toPath(),
                new File(outDir, "game.asm.pristine").toPath(),
                java.nio.file.StandardCopyOption.REPLACE_EXISTING);
        println("refreshed game.asm.pristine from this export");

        PrintWriter e = new PrintWriter(new File(outDir, "game_externs.inc"), "UTF-8");
        e.println("; Symbols the assembly needs that the C++ side or the CRT provides.");
        e.println("; Regenerated as functions move from ARTS_IMPORT to ARTS_EXPORT.");
        e.println();
        for (String s : imports) {
            e.println("EXTERN " + s + ":DWORD");
        }
        for (String s : externProcs) {
            e.println("EXTERN " + s + ":PROC");
        }
        for (String s : externData) {
            e.println("EXTERN " + s + ":BYTE");
        }
        e.close();

        println("ExportAsm: " + n + " functions");
        println("  absolute relocations : " + patchedAbs);
        println("  relative relocations : " + patchedRel);
        println("  import relocations   : " + patchedImport);
        println("  data pointers        : " + dataPointers);
        println("  data labels emitted  : " + dataLabelsEmitted);
        println("  operands Ghidra missed: " + missedOperands);
        println("  recovered via refs   : " + recoveredPointers);
        println("  refs with no symbol  : " + refNoSymbol);
        println("  refs not located in bytes: " + refNotLocated);
        println("  imports              : " + imports.size());
    }

    void buildSymbolIndex() {
        SymbolTable st = currentProgram.getSymbolTable();

        for (Symbol s : st.getAllSymbols(false)) {
            Address a = s.getAddress();
            if (!a.isMemoryAddress()) {
                continue;
            }

            long off = a.getOffset();
            String name = s.getName();
            String have = symbolAt.get(off);

            // Prefer the mangled name: it is what the linker matches against the C++ side.
            if (have == null || (!have.startsWith("?") && name.startsWith("?"))) {
                symbolAt.put(off, sanitize(name, off));
            }
        }

        sortedSymbols.putAll(symbolAt);
    }

    void buildImportIndex() {
        SymbolTable st = currentProgram.getSymbolTable();

        for (Symbol s : st.getExternalSymbols()) {
            for (Reference r : s.getReferences()) {
                Address from = r.getFromAddress();
                if (from != null && from.isMemoryAddress()) {
                    importAt.put(from.getOffset(), mangleImport(s.getName()));
                }
            }
        }

        // Ghidra also puts a thunk/pointer symbol on the IAT slot itself.
        for (Symbol s : st.getAllSymbols(false)) {
            Address a = s.getAddress();
            if (a.isExternalAddress()) {
                continue;
            }
        }
    }

    // Ghidra numbers switch-case labels PER FUNCTION, so caseD_0 exists hundreds of times over.
    // In one flat assembly file those all collide. Anything matching this gets its address
    // appended, which is unique by construction and still readable.
    static final java.util.regex.Pattern LOCAL_LABEL =
            java.util.regex.Pattern.compile("^(caseD_|switchD|default|LAB_|JMP_|joined_|code_).*");

    String sanitize(String name, long addr) {
        // A mangled name is never a keyword and must never be altered - it is what the linker
        // matches against the C++ side.
        if (name.startsWith("?")) {
            return name;
        }

        if (LOCAL_LABEL.matcher(name).matches()) {
            return String.format("%s_%06X", name, addr);
        }

        if (RESERVED.contains(name.toUpperCase())) {
            return "g_" + name;
        }

        return name;
    }

    String dataLabelFor(long addr) {
        return String.format("D_%06X", addr);
    }

    String labelFor(long addr) {
        return String.format("L_%06X", addr);
    }

    /** Find every referenced code address that has no symbol, so a label can be placed there. */
    void collectLabels() throws Exception {
        InstructionIterator ii = currentProgram.getListing().getInstructions(true);

        while (ii.hasNext()) {
            Instruction insn = ii.next();

            for (Reference ref : insn.getReferencesFrom()) {
                if (ref.getReferenceType().isFallthrough()) {
                    continue;
                }

                Address to = ref.getToAddress();
                if (!to.isMemoryAddress()) {
                    continue;
                }

                long target = to.getOffset();
                if (target < 0x400000L || symbolAt.containsKey(target)) {
                    continue;
                }

                // Only code targets need a label. A reference into the middle of a data object is
                // legitimately expressed as `offset SYM + delta`.
                MemoryBlock b = currentProgram.getMemory().getBlock(to);
                if (b != null && b.isExecute()) {
                    neededLabels.add(target);
                } else if (b != null && EMITTED_DATA.contains(b.getName())) {
                    // A data target with no symbol at it, in a section we actually emit. Give it
                    // a label. Anything outside those sections - notably the PE header block,
                    // which Ghidra maps but we do not write - must stay raw. That is correct: the
                    // header always sits at the image base, and the link pins it to 0x400000.
                    neededDataLabels.add(target);
                }
            }

            if (monitor.isCancelled()) {
                return;
            }
        }

        // Jump tables live in .text but outside any function body, so no instruction references
        // their entries. Scan them here: a label placed later than the function that jumps to it
        // would be too late, because the function is already written out.
        for (long a = textStart; a + 4 <= textEnd; a += 4) {
            if (covered[(int) (a - textStart)]) {
                continue;
            }

            long value = readDword(a);
            if (isCodeStart(value) && !symbolAt.containsKey(value)) {
                neededLabels.add(value);
            }
        }
    }

    String mangleImport(String name) {
        // MASM references an import through its __imp__ slot. Ghidra reports the plain API name.
        String clean = name.replaceAll("[^A-Za-z0-9_@$?]", "_");
        return "__imp__" + clean;
    }

    void writeHeader(PrintWriter w) {
        w.println("; Generated by ExportAsm.java from midtown2.exe (MM3393). Do not edit by hand.");
        w.println("; Instruction bytes are verbatim from the retail binary; only address operands");
        w.println("; are replaced by symbol references so the linker can swap in ported C++.");
        w.println();
        w.println(".686P");
        w.println(".XMM");
        w.println(".MODEL FLAT");
        w.println("ASSUME FS:NOTHING");
        w.println("OPTION CASEMAP:NONE");
        // MASM scopes a label defined inside a PROC to that PROC. This binary is full of
        // cross-procedure jumps - Ghidra even carves switch cases out as separate functions - so
        // those labels have to be visible module-wide. Every generated label carries its address,
        // so making them global cannot collide.
        w.println("OPTION NOSCOPED");
        w.println();
        w.println("INCLUDE game_externs.inc");
        w.println();
    }

    /**
     * Names decided once, up front, so a call site and the procedure it calls always agree.
     *
     * emitText used to de-duplicate as it went: on meeting a second function called "add" it
     * emitted "add_583E56", because two PROCs of the same name will not assemble. But nothing told
     * the rest of the exporter about that rename, so a call targeting the second "add" still
     * emitted "g_add" - and linked to the first one, at a completely different address. The
     * assembler is perfectly happy with that; it is a silent call to the wrong function.
     *
     * It stayed hidden only because the previous database happened to have no duplicates. A
     * rebuilt one did, and the byte comparison caught exactly four bytes: one call operand.
     */
    Map<Long, String> finalName = new HashMap<>();

    void assignUniqueNames() {
        Set<Long> entries = new HashSet<>();
        FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
        while (it.hasNext()) {
            entries.add(it.next().getEntryPoint().getOffset());
        }

        List<Long> addrs = new ArrayList<>(symbolAt.keySet());
        Collections.sort(addrs);

        Set<String> taken = new HashSet<>();
        int renamed = 0;

        // Uniqueness has to cover every emitted symbol, not just function entry points.
        //
        // Doing only functions left four bytes wrong. The call at 0x00583DD2 targets 0x00583E56,
        // which carries a plain label named "add"; a different address, 0x00582B13, holds a
        // function of the same name. Both emitted "g_add", so the call linked to the function and
        // the label it actually meant was never referenced. The assembler cannot object - both
        // names exist and one of them resolves.
        //
        // Functions are assigned first so they keep the plain name and labels take the suffix,
        // which keeps the output stable and readable. Mangled C++ names are never renamed: they
        // are unique by construction, and they are the interface the ported C++ links against.
        for (int pass = 0; pass < 2; pass++) {
            for (long a : addrs) {
                if (entries.contains(a) != (pass == 0)) {
                    continue;
                }

                String n = symbolAt.get(a);
                if (n == null || n.startsWith("?")) {
                    continue;
                }

                if (!taken.add(n)) {
                    n = String.format("%s_%06X", n, a);
                    taken.add(n);
                    renamed++;
                }
                finalName.put(a, n);
            }
        }

        // Functions with no symbol at all still need a name, and it still has to be unique.
        for (long a : entries) {
            if (!finalName.containsKey(a) && symbolAt.get(a) == null) {
                String n = String.format("sub_%06X", a);
                taken.add(n);
                finalName.put(a, n);
            }
        }

        println("unique names assigned: " + finalName.size()
                + " (" + renamed + " disambiguated)");
    }

    String nameFor(long addr) {
        String unique = finalName.get(addr);
        return unique != null ? unique : symbolAt.get(addr);
    }

    /**
     * Nearest preceding symbol, ANCHORED WITHIN THE TARGET'S OWN SECTION.
     *
     * Crossing a section boundary here produces a wrong address at runtime, silently. `.rdata` and
     * `.data` are displaced by different amounts in the linked image (+0x1310 and +0x4000, both
     * measured), so `offset <rdata symbol> + delta` naming a `.data` address relocates by the
     * WRONG delta and lands 0x2CF0 short.
     *
     * That is not hypothetical: the CRT initialiser table bounds were anchored to
     * `__IMPORT_DESCRIPTOR_DSOUND` because `.data` has no symbol before offset 0x8AC. `_initterm`
     * then walked the wrong memory, read 0x00030000 as a function pointer and called it - the
     * startup crash.
     *
     * If the target's section has no preceding symbol, refuse. Leaving the raw bytes is correct
     * while layout is preserved; inventing a cross-section anchor never is.
     */
    String[] nearest(long addr) {
        MemoryBlock block = currentProgram.getMemory().getBlock(toAddr(addr));
        if (block == null) {
            return null;
        }

        long blockStart = block.getStart().getOffset();

        Map.Entry<Long, String> e = sortedSymbols.floorEntry(addr);

        if (e == null || e.getKey() < blockStart) {
            // No symbol precedes this inside its own section. A label is emitted at exactly this
            // address in the data block, so name that rather than reaching across the boundary.
            if (neededDataLabels.contains(addr)) {
                crossSectionRefused++;
                return new String[] { dataLabelFor(addr), "0" };
            }
            crossSectionRefused++;
            return null;
        }

        return new String[] { e.getValue(), Long.toString(addr - e.getKey()) };
    }

    void emitFunction(PrintWriter w, Function fn) throws Exception {
        Address entry = fn.getEntryPoint();
        long start = entry.getOffset();

        String name = nameFor(start);
        // Anything with a real name is made public: mangled C++ symbols so the linker can swap
        // them for ported code, and plain ones like  (the entry point) so /ENTRY can find
        // it. Only address-derived labels stay private.
        boolean isPublic = name != null
                && !name.startsWith("sub_") && !name.startsWith("caseD_")
                && !name.startsWith("switchD") && !name.startsWith("default_")
                && !name.startsWith("LAB_") && !name.startsWith("L_")
                && !name.startsWith("nullsub_") && !name.startsWith("j_");

        if (name == null) {
            name = String.format("sub_%06X", start);
        }

        if (!emitted.add(name)) {
            w.println("; duplicate symbol skipped: " + name);
            return;
        }

        w.println("ALIGN 16");
        if (isPublic) {
            w.println("PUBLIC " + name);
            w.println(name + " PROC");
        } else {
            w.println(name + " PROC PRIVATE");
        }

        AddressSetView body = fn.getBody();
        InstructionIterator ii = currentProgram.getListing().getInstructions(body, true);

        while (ii.hasNext()) {
            Instruction insn = ii.next();
            long at = insn.getAddress().getOffset();

            // A jump-table or switch-case target inside this function needs a label of its own.
            // Both spellings occur: Ghidra names some of them (caseD_2e) and leaves others bare.
            if (at != start) {
                String inner = symbolAt.get(at);
                if (inner != null) {
                    w.println(inner + ":");
                }
            }

            if (neededLabels.contains(at)) {
                w.println(labelFor(at) + ":");
            }

            emitInstruction(w, insn);
        }

        w.println(name + " ENDP");
        w.println();
    }

    void emitInstruction(PrintWriter w, Instruction insn) throws Exception {
        byte[] bytes = insn.getBytes();
        long addr = insn.getAddress().getOffset();
        long end = addr + bytes.length;

        List<Patch> patches = new ArrayList<>();

        for (Reference ref : insn.getReferencesFrom()) {
            if (ref.getReferenceType().isFallthrough()) {
                continue;
            }

            Address to = ref.getToAddress();
            String sym;
            long addend = 0;
            boolean indirect = false;

            // Ghidra reports stack slots and registers as references too. They are not addresses
            // and have nothing encoded in the instruction, so they must not be counted as
            // unresolved - doing so buried the real failures under 85,000 false ones.
            if (!to.isMemoryAddress() || to.getOffset() < 0x400000L) {
                continue;
            }

            if (to.isExternalAddress()) {
                // A call into a DLL. The instruction encodes the IAT slot, not the target, so the
                // slot address is what has to be found in the bytes.
                continue; // handled below via importAt on the memory operand
            }

            long target = to.getOffset();

            // Only consider the import substitution when the reference could plausibly be the
            // absolute IAT form; the relative case is corrected after the encoding is located.
            String imp = importAt.get(target);
            if (imp != null) {
                sym = imp;
                indirect = true;
                imports.add(imp);
            } else {
                sym = nameFor(target);

                if (sym == null && neededLabels.contains(target)) {
                    sym = labelFor(target);
                }

                if (sym == null) {
                    // Nearest-symbol-plus-offset is only meaningful for a reference landing inside
                    // a data object (an array element, a struct field). For code it produces
                    // nonsense like `caseD_4 + 123`, so leave those bytes alone - an intra-function
                    // branch stays correct anyway, because the function is emitted contiguously.
                    MemoryBlock blk = currentProgram.getMemory().getBlock(to);
                    if (blk != null && blk.isExecute()) {
                        refNoSymbol++;
                        continue;
                    }

                    String[] near = nearest(target);
                    if (near == null) {
                        refNoSymbol++;
                        continue;
                    }
                    sym = near[0];
                    addend = Long.parseLong(near[1]);
                }
            }

            int pos = findDword(bytes, (int) (target - end));
            boolean rel = true;

            if (pos < 0) {
                pos = findDword(bytes, (int) target);
                rel = false;
            }

            if (pos < 0) {
                refNotLocated++;
                continue;
            }

            // AN IMPORT SUBSTITUTION IS ONLY VALID FOR AN ABSOLUTE ENCODING.
            //
            // `call dword ptr [__imp__Foo]` encodes the IAT SLOT, so replacing it with
            // `dd offset __imp__Foo` is right. A plain `e8 rel32` call does not: it targets a
            // THUNK, and Ghidra resolves the reference straight through that thunk to the import -
            // so importAt matches, and the absolute slot address gets written into a relative
            // field. `call +0x113E` came out as `e8 64 10 5b 00`, a branch to nowhere.
            //
            // Caught by tools/verify_text.py comparing all of .text against retail; the two-anchor
            // check in the build could never have seen it.
            if (rel && indirect) {
                indirect = false;

                sym = nameFor(target);
                if (sym == null && neededLabels.contains(target)) {
                    sym = labelFor(target);
                }
                if (sym == null) {
                    refNoSymbol++;
                    continue;
                }
                addend = 0;
            }

            Patch p = new Patch(pos, sym, addend, rel);
            p.indirect = indirect;
            patches.add(p);

            if (indirect) {
                patchedImport++;
            } else if (rel) {
                patchedRel++;
            } else {
                patchedAbs++;
            }
        }

        // Ghidra's reference analysis misses immediates that point at UNDEFINED data. It records a
        // reference when it knows what is at the target; where nothing is defined it leaves the
        // value as a plain constant, and the operand keeps its retail address forever.
        //
        // FirstRunEula was exactly this: one `push 0x0068C35D` naming a string, no reference, so it
        // stayed raw, .data moved 0x8000 underneath it, the EULA check read the wrong bytes and
        // returned false - and Main called exit(0) before opening a single file.
        //
        // So sweep the operand bytes for values that land inside .rdata or .data and patch them
        // too. This does NOT re-classify the instruction - the opcode bytes are untouched and only
        // a 4-byte operand window is rewritten, which is the difference between this and the
        // earlier attempt that corrupted the DirectDraw detect path.
        for (int pos = 1; pos + 4 <= bytes.length; pos++) {
            // Only where an operand can actually be.
            //
            // Scanning every position reads four bytes that straddle the opcode, ModRM and
            // displacement and calls them an address whenever the number happens to match. In
            // skip_variable that turned "C7 42 14 5B 00 00 00" - mov dword ptr [edx+0x14], 0x5B -
            // into a relocated pointer, because bytes 1..4 are 0x005B1442 and there is a vftable
            // near that address. The result was an illegal instruction the moment the JPEG decoder
            // reached it, and it verified clean, because a rewritten instruction differs from
            // retail by exactly the section delta - the same as a correct relocation.
            //
            // x86 puts these operands last: a trailing imm32 or disp32 ends the instruction, and
            // the one case with both - mov dword ptr [addr], imm32 - puts the address eight bytes
            // from the end. Nothing legitimate hides in the middle.
            // A disp32 may be followed by an imm8, and that case was being skipped.
            //
            // Constraining this to trailing operands was right - scanning every position rewrote a
            // live instruction in skip_variable and broke the JPEG decoder. But `or byte ptr
            // [eax + 0x6B4E81], 0x20` is `80 88 <disp32> <imm8>`: seven bytes, address at offset 2,
            // so neither pos+4 nor pos+8 reaches it. Five such writes in the CRT's MBCS setup kept
            // their retail addresses while 21 of 25 other references to the same table relocated -
            // so reads landed correctly and only the writes went astray, which is why nothing saw
            // it. They scribble into Stream::sm_Buffers, and `or ..., 0x20` turns a NUL into a
            // space: an unterminated string, which is exactly how a stack frame gets smashed.
            //
            // imm16 is deliberately NOT allowed. skip_variable's `C7 42 14 5B 00 00 00` is seven
            // bytes with a false candidate at offset 1, and pos+4+2 == 7 would match it and bring
            // that bug straight back.
            if (pos + 4 != bytes.length
                    && pos + 5 != bytes.length
                    && pos + 8 != bytes.length) {
                continue;
            }

            boolean taken = false;
            for (Patch p : patches) {
                if (pos < p.pos + 4 && p.pos < pos + 4) {
                    taken = true;
                    break;
                }
            }
            if (taken) {
                continue;
            }

            long v = (bytes[pos] & 0xFFL) | ((bytes[pos + 1] & 0xFFL) << 8)
                    | ((bytes[pos + 2] & 0xFFL) << 16) | ((bytes[pos + 3] & 0xFFL) << 24);

            if (v < 0x400000L) {
                continue;
            }

            MemoryBlock vb = currentProgram.getMemory().getBlock(toAddr(v));
            if (vb == null || !EMITTED_DATA.contains(vb.getName())) {
                continue;
            }

            // THE WINDOW MUST BE A NUMBER THE INSTRUCTION ACTUALLY ENCODES.
            //
            // "trailing" is not the same as "is an operand", and that gap shipped a crash.
            // `mov eax, [esi + 6398h]` is 8B 86 98 63 00 00 - six bytes, so pos == 1 satisfies
            // pos + 5 == length, and bytes 1..4 are 00639886h: the ModRM byte followed by three
            // bytes of the displacement. That lands in .data, so it was relocated - and relocating
            // it moved a STRUCTURE OFFSET. Every [reg + 6398h] in dgBangerActiveManager became
            // [reg + 64D8h], 140h bytes past the end of a 639Ch object.
            //
            // What that cost: the constructor's m_ActiveCount = 0 wrote outside its own heap block
            // (the memMemoryAllocator::FindHeap failure), and Attach() then indexed the embedded
            // actives array with a garbage count and returned a float out of the middle of a
            // Matrix34 as a dgPhysEntity*. dgPhysManager::CollideInstances dereferenced it the
            // first time the player hit a trashcan. Seventeen instructions, the whole class.
            //
            // It verified clean through every gate, because a FALSE relocation differs from retail
            // by exactly the section delta - the same as a correct one. verify_text.py cannot tell
            // those apart even in principle, which is why the constraint has to live here.
            //
            // The pos + 4 clause is unsound the same way: `cmp word ptr [edx + 60h], 0` is
            // 66 83 7A 60 00, whose bytes 1..4 are 00607A83h. Two more sites in jpeg_idct_islow
            // came in through it.
            //
            // An operand is a number the disassembler has already agreed is there. Requiring the
            // window to EQUAL one makes a straddling window impossible by construction, and costs
            // nothing for the cases this sweep exists for - FirstRunEula's push 0068C35Dh and the
            // CRT's `or byte ptr [eax + 6B4E81h], 20h` both encode their address as an operand,
            // which is precisely why they needed relocating.
            boolean encoded = false;

            for (int oi = 0; oi < insn.getNumOperands() && !encoded; oi++) {
                for (Object o : insn.getOpObjects(oi)) {
                    long ov;

                    if (o instanceof Scalar) {
                        ov = ((Scalar) o).getUnsignedValue() & 0xFFFFFFFFL;
                    } else if (o instanceof Address) {
                        ov = ((Address) o).getOffset();
                    } else {
                        continue;
                    }

                    if (ov == v) {
                        encoded = true;
                        break;
                    }
                }
            }

            if (!encoded) {
                continue;
            }

            // Name it directly with a generated label when no symbol precedes it in-section.
            // Registering the label and skipping the patch - which is what this did first - leaves
            // the operand raw forever: emitDataBlock runs after this and duly emits the label, but
            // nothing ever comes back to use it. FirstRunEula's string reference sat unrelocated
            // for exactly that reason.
            String[] near = nearest(v);
            String sym;
            long addend;

            if (near == null) {
                neededDataLabels.add(v);
                sym = dataLabelFor(v);
                addend = 0;
            } else {
                sym = near[0];
                addend = Long.parseLong(near[1]);
            }

            Patch p = new Patch(pos, sym, addend, false);
            patches.add(p);
            missedOperands++;
        }

        if (patches.isEmpty()) {
            emitBytes(w, bytes, 0, bytes.length, addr);
            return;
        }

        patches.sort(Comparator.comparingInt(p -> p.pos));

        int at = 0;
        for (Patch p : patches) {
            if (p.pos < at) {
                continue;
            }

            emitBytes(w, bytes, at, p.pos - at, addr);
            w.println("    " + patchExpr(p));
            at = p.pos + 4;
        }

        emitBytes(w, bytes, at, bytes.length - at, addr);
    }

    String patchExpr(Patch p) {
        String expr = p.symbol;

        if (p.addend > 0) {
            expr += " + " + p.addend;
        } else if (p.addend < 0) {
            expr += " - " + (-p.addend);
        }

        if (p.indirect) {
            return "dd offset " + expr;
        }
        if (p.relative) {
            return "dd " + expr + " - ($ + 4)";
        }
        return "dd offset " + expr;
    }

    /**
     * Emit .text in strict address order, functions and gaps interleaved, with no padding.
     *
     * This is what keeps the output byte-identical to the retail .text, and that matters more than
     * it first appears. Emitting functions in one pass and jump tables in another - with ALIGN 16
     * before each PROC - grew .text by ~90KB and moved every function: sdlPage16::Draw went from
     * 0x448330 to 0x445950. Since midtown2.exe has no relocation table, any absolute address that
     * was not symbolised is then pointing at the wrong place, silently.
     *
     * Walking the section once, in order, reproduces the original layout exactly. Every address
     * stays valid whether or not we managed to symbolise it, and .rdata and .data then land at
     * their original virtual addresses too, because section alignment puts them there.
     */
    int emitText(PrintWriter w) throws Exception {
        Function cur = null;
        String curName = null;
        int count = 0;

        long a = textStart;

        while (a < textEnd) {
            if (monitor.isCancelled()) {
                break;
            }

            Address addr = toAddr(a);
            Function f = currentProgram.getFunctionManager().getFunctionContaining(addr);

            if (f != cur) {
                if (cur != null) {
                    w.println(curName + " ENDP");
                    w.println();
                }

                cur = f;
                curName = null;

                if (f != null) {
                    String name = nameFor(f.getEntryPoint().getOffset());
                    if (name == null) {
                        name = String.format("sub_%06X", f.getEntryPoint().getOffset());
                    }

                    // Two different jobs, and removing this one broke the build.
                    //
                    // assignUniqueNames handles distinct functions that share a name, so that call
                    // sites and procedures agree. This handles the same function appearing twice:
                    // a non-contiguous body makes the walk leave the function and come back, and
                    // emitText closes the PROC and opens a new one when it does. ExceptMain is
                    // exactly that - a body in two pieces - and emitting both as
                    // "?ExceptMain@@YAHXZ PROC" is a symbol redefinition.
                    //
                    // Renaming the later piece is safe because nothing calls it: a call targets the
                    // entry point, which is the first piece, and jumps into the middle of a
                    // function go through L_ labels.
                    if (!emitted.add(name)) {
                        name = String.format("%s_%06X", name, a);
                        emitted.add(name);
                    }

                    if (isPublicName(name)) {
                        w.println("PUBLIC " + name);
                    }

                    w.println(name + " PROC" + (isPublicName(name) ? "" : " PRIVATE"));
                    curName = name;
                    count++;

                    if (count % 2000 == 0) {
                        println("  code: " + count + " functions");
                    }
                }
            }

            // Labels land here whether we are inside a function or in a gap between them.
            boolean atEntry = (f != null && f.getEntryPoint().getOffset() == a);

            if (!atEntry) {
                String inner = symbolAt.get(a);
                if (inner != null) {
                    w.println(inner + ":");
                }
            }

            if (neededLabels.contains(a)) {
                w.println(labelFor(a) + ":");
            }

            Instruction insn = currentProgram.getListing().getInstructionAt(addr);

            if (insn != null) {
                emitInstruction(w, insn);
                a += insn.getLength();
                continue;
            }

            // Not code: a jump table entry, or padding. A table entry holds an absolute code
            // address and must be symbolised; padding is emitted as-is.
            if (textPointers.contains(a)) {
                long value = readDword(a);
                String s = symbolAt.get(value);
                w.println("    dd offset " + (s != null ? s : dataLabelFor(value)));
                pointersEmitted.add(a);
                a += 4;
                continue;
            }

            if ((a % 4) == 0 && a + 4 <= textEnd) {
                long value = readDword(a);
                if (isCodeStart(value)) {
                    String s = symbolAt.get(value);
                    w.println("    dd offset " + (s != null ? s : labelFor(value)));
                    a += 4;
                    continue;
                }
            }

            w.println(String.format("    db 0%02Xh",
                    currentProgram.getMemory().getByte(addr) & 0xFF));
            a++;
        }

        if (cur != null) {
            w.println(curName + " ENDP");
            w.println();
        }

        // A pointer that was found but never emitted is the worst possible outcome: it keeps its
        // retail value, so every byte-comparison passes while the pointer is silently wrong. Say
        // so loudly, and name a few, because the reason is always that the walk stepped over the
        // address inside some longer thing rather than landing on it.
        println("pointer entries emitted: " + pointersEmitted.size() + " of " + textPointers.size());

        int shown = 0;
        for (long p : textPointers) {
            if (pointersEmitted.contains(p)) {
                continue;
            }
            if (shown < 5) {
                Instruction over = currentProgram.getListing().getInstructionContaining(toAddr(p));
                println(String.format("  MISSED 0x%08X%s", p,
                        over == null ? " (no instruction over it)"
                                     : String.format(" inside instruction at 0x%08X len %d",
                                             over.getMinAddress().getOffset(), over.getLength())));
            }
            shown++;
        }
        if (shown > 5) {
            println("  ... and " + (shown - 5) + " more");
        }

        return count;
    }

    /**
     * Should this data symbol be visible to the linker?
     *
     * Only mangled names were, which quietly limited what could ever be ported: a reimplemented
     * function that touches an undecorated global - `useIME`, `audioFlags`, `inputDevice` - cannot
     * name it, so registering that function strips its PROC and then fails to link. BeginPhase, the
     * function that closes the OpenGL activation boundary, is blocked on exactly those three.
     *
     * Ghidra's invented names are excluded. They carry no information, there are tens of thousands
     * of them, and every one is a chance of a link-time collision with a real name somewhere else.
     */
    /**
     * Undecorated globals that ported C++ is allowed to name.
     *
     * Only mangled symbols are exported, which limits what can be ported: a reimplemented function
     * touching an undecorated global cannot name it, so registering that function strips its PROC
     * and then fails to link. BeginPhase - the function that closes the OpenGL activation boundary
     * - is blocked on exactly the three below.
     *
     * The obvious generalisation, exporting every recovered name, was tried and is wrong. It made
     * 12,221 symbols public and the game began jumping to a stack address inside `start`, while
     * .text still verified byte-perfect against retail - so the damage was purely in link-time
     * resolution, somewhere among twelve thousand new opportunities for one. An explicit list has
     * no blast radius and says exactly why each entry exists.
     */
    static final Set<String> LINKABLE_GLOBALS = new HashSet<>(java.util.Arrays.asList(
            "useIME",         // BeginPhase: chooses the IME path
            "audioFlags",     // BeginPhase: passed to InitAudioManager
            "inputDevice"));  // BeginPhase: passed to GameInput

    boolean isLinkableName(String name) {
        return name != null && (name.startsWith("?") || LINKABLE_GLOBALS.contains(name));
    }

    boolean isPublicName(String name) {
        return name != null
                && !name.startsWith("sub_") && !name.startsWith("caseD_")
                && !name.startsWith("switchD") && !name.startsWith("default_")
                && !name.startsWith("LAB_") && !name.startsWith("L_")
                && !name.startsWith("nullsub_") && !name.startsWith("j_");
    }

    /** Bytes of .text that no function body covers: jump tables, padding, orphaned stubs. */
    boolean[] covered;
    long textStart, textEnd;

    void markCoverage() {
        MemoryBlock text = currentProgram.getMemory().getBlock(".text");
        textStart = text.getStart().getOffset();
        textEnd = text.getEnd().getOffset() + 1;
        covered = new boolean[(int) (textEnd - textStart)];

        FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
        while (it.hasNext()) {
            Function fn = it.next();
            for (Address a : fn.getBody().getAddresses(true)) {
                long o = a.getOffset();
                if (o >= textStart && o < textEnd) {
                    covered[(int) (o - textStart)] = true;
                }
            }
        }
    }

    /**
     * Re-assert every known pointer as data, after disassembly has had its turn.
     *
     * findDataFormatPointers reported 307 recovered GUID pointers and not one of them reached the
     * output. The reason is an ordering trap: clearing a listing does not stop disassembleGaps from
     * putting an instruction back over it a moment later, and an instruction that STARTS a few
     * bytes earlier can span the entry entirely. emitText checks "is there an instruction here"
     * before it checks "is this a known pointer", so the instruction wins and the pointer is
     * emitted as part of its bytes - unrelocated, and byte-identical to retail, so no verifier
     * complains.
     *
     * Running last is what makes this reliable. Anything that overlaps a known pointer is cleared,
     * including an instruction starting before it; the leftover bytes ahead of the entry are then
     * emitted raw, which is what they are and what retail contains.
     *
     * It deliberately acts ONLY on pointers found by reading a structure's own header, never on
     * ones found heuristically. Deleting an instruction is destructive, and a heuristic that is
     * wrong about one address would have this pass erase real code and emit a relocated pointer in
     * its place - which is invisible to a byte comparison, because "differs from retail by exactly
     * the section delta" is what a correct relocation looks like too. A DIDATAFORMAT states its own
     * entry count and stride, so those addresses are certain in a way a guess never is.
     */
    void protectTextPointers() throws Exception {
        int recleared = 0;

        for (long a : new ArrayList<>(dataFormatPointers)) {
            for (long p = a; p < a + 4; p++) {
                Instruction in = currentProgram.getListing().getInstructionContaining(toAddr(p));
                if (in == null) {
                    continue;
                }
                try {
                    clearListing(in.getMinAddress(), in.getMaxAddress());
                    recleared++;
                } catch (Exception e) {
                    // leave it: emitText will emit the instruction and the verifier will show it
                }
            }
        }

        println("pointer entries re-cleared after disassembly: " + recleared);
    }

    /**
     * Pointers inside DirectInput data formats, read from the structure's own header.
     *
     * This exists because the heuristics could not recover c_dfDIMouse's GUID pointers, and that
     * failure cost the game its mouse: SetDataFormat validates every pguid it is handed, so a
     * single unrelocated entry makes it return E_INVALIDARG and input never initialises.
     *
     * The heuristics failed for a reason worth stating. A DIOBJECTDATAFORMAT is 16 bytes, so the
     * pointers are strided rather than adjacent; Ghidra disassembled some entries as instructions
     * and left others alone; and the GUIDs they point at were never defined as data, so there is no
     * symbol to match against. Every rule based on what the entries look like ran aground on one of
     * those three.
     *
     * But a DIDATAFORMAT states its own dwNumObjs and rgodf. Nothing has to be guessed: find the
     * header, believe it, and walk exactly that many entries at exactly that stride. The header is
     * identified by dwSize == 24 and dwObjSize == 16 - the sizes of the two structures, fixed by
     * the DirectInput ABI - plus the requirement that the whole array fit inside one memory block,
     * which is what keeps a coincidental pair of dwords from being read as a format.
     *
     * A NULL pguid is normal and skipped: mouse buttons and joystick POVs leave it null.
     */
    void findDataFormatPointers() throws Exception {
        int formats = 0, ptrs = 0, outside = 0, already = 0;

        for (MemoryBlock block : currentProgram.getMemory().getBlocks()) {
            if (!EMITTED_DATA.contains(block.getName())) {
                continue;
            }

            long start = block.getStart().getOffset();
            long end = block.getEnd().getOffset();

            for (long a = start; a + 24 <= end; a += 4) {
                // sizeof(DIDATAFORMAT) and sizeof(DIOBJECTDATAFORMAT). Checked first and alone,
                // because this runs over every aligned dword of .rdata and .data.
                if (readDword(a) != 24 || readDword(a + 4) != 16) {
                    continue;
                }

                long flags = readDword(a + 8);          // DIDF_ABSAXIS / DIDF_RELAXIS
                if (flags != 1 && flags != 2) {
                    continue;
                }

                long numObjs = readDword(a + 16);
                long rgodf = readDword(a + 20);
                if (numObjs == 0 || numObjs > 1024 || rgodf < 0x400000L) {
                    continue;
                }

                MemoryBlock ab = currentProgram.getMemory().getBlock(toAddr(rgodf));
                if (ab == null || rgodf + 16 * numObjs > ab.getEnd().getOffset() + 1) {
                    continue;
                }

                formats++;

                for (long k = 0; k < numObjs; k++) {
                    long e = rgodf + 16 * k;

                    // These arrays live in .text in this binary - const data among code, which is
                    // exactly why they were missed. Only the .text emitter consults textPointers,
                    // so an array anywhere else would be silently skipped; count it and say so
                    // rather than report a pointer that was never actually emitted.
                    if (e < textStart || e + 4 > textEnd) {
                        outside++;
                        continue;
                    }

                    long g = readDword(e);
                    if (g < 0x400000L) {
                        continue;               // NULL pguid: normal for buttons and POVs
                    }

                    MemoryBlock gb = currentProgram.getMemory().getBlock(toAddr(g));
                    if (gb == null || !EMITTED_DATA.contains(gb.getName())) {
                        continue;
                    }

                    try {
                        clearListing(toAddr(e), toAddr(e + 3));
                    } catch (Exception ex) {
                        continue;
                    }

                    if (!symbolAt.containsKey(g)) {
                        neededDataLabels.add(g);
                    }

                    dataFormatPointers.add(e);
                    if (textPointers.add(e)) {
                        ptrs++;
                    } else {
                        already++;
                    }
                }
            }
        }

        println("DirectInput data formats: " + formats + " (" + ptrs + " new, "
                + already + " already known)");
        if (outside > 0) {
            println("  WARNING: " + outside + " entries lie outside .text and were NOT emitted");
        }
    }

    /**
     * Find pointer arrays embedded in .text, before anything tries to disassemble them.
     *
     * The rule is deliberately strict: at least TWO consecutive 4-byte-aligned dwords that each
     * land exactly on a known symbol or on a plausible address inside an emitted data section.
     * A lone value that happens to look like an address is not enough - a float constant or a
     * length can do that, and symbolising one would change its value.
     */
    void findTextPointers() throws Exception {
        int found = 0;

        for (long a = textStart; a + 8 <= textEnd; a += 4) {
            // The test is "is this inside an actual INSTRUCTION", not "is this inside a function
            // body". Those differ, and the difference is the whole problem:
            //
            //   * Skipping whole function bodies misses data islands embedded in functions, and
            //     their pointers stay unrelocated.
            //   * Not skipping anything overrules the disassembler about what is code - it
            //     replaced a live instruction's immediate in the DirectDraw detect path and broke
            //     device enumeration.
            //
            // A real instruction is never re-classified; a gap inside a function body still is.
            // This must run BEFORE disassembleGaps, which would turn those islands into
            // instructions and hide them again.
            if (currentProgram.getListing().getInstructionContaining(toAddr(a)) != null) {
                continue;
            }

            long v = readDword(a);

            // An EXACT match against a known symbol address in an emitted data section. This is
            // the same rule emitDataBlock already relies on, and it is strong: a float or a length
            // would have to coincide precisely with a symbol's address to be mistaken for one.
            //
            // A run-of-consecutive-pointers rule was tried first and missed the real case -
            // DIOBJECTDATAFORMAT is 16 bytes, so the GUID pointers in c_dfDIMouse's rgodf array are
            // STRIDED, never adjacent. Requiring adjacency found 6 dwords and none of the ones
            // that mattered.
            if (v >= 0x400000L && symbolAt.containsKey(v)) {
                MemoryBlock b = currentProgram.getMemory().getBlock(toAddr(v));

                if (b != null && EMITTED_DATA.contains(b.getName())) {
                    // A pointer ARRAY is strided, and Ghidra may have disassembled some of its
                    // entries as instructions while leaving others alone - c_dfDIMouse's rgodf came
                    // out with entry 0 relocated and entries 1 and 2 raw, because only the first
                    // escaped the disassembler.
                    //
                    // So once one entry is confirmed, walk the stride and take the rest, clearing
                    // whatever the disassembler put over them. Requiring EVERY entry to be an exact
                    // symbol match keeps this safe: real code does not produce a run of values that
                    // each land precisely on a data symbol's address at a constant stride.
                    // Once one entry is confirmed, walk the stride and take the rest, clearing
                    // whatever the disassembler put over them. Requiring EVERY entry to be an exact
                    // symbol match keeps this safe: real code does not produce a run of values that
                    // each land precisely on a data symbol's address at a constant stride.
                    //
                    // Relaxing that to "in a data section and near the anchor" was tried and is a
                    // dead end - it fired 32,000 extra times and still did not recover the array it
                    // was written for, because those entries point at GUIDs Ghidra never defined.
                    // Structures that describe their own length are handled by
                    // findDataFormatPointers instead, which is exact rather than plausible.
                    //
                    // The walk runs both directions: the anchor is whichever entry escaped the
                    // disassembler, and there is no reason that is the first one.
                    for (int stride : new int[] {16, 8, 4, 12, 20, 24, 32}) {
                        List<Long> hits = new ArrayList<>();
                        List<Long> targets = new ArrayList<>();

                        for (int dir = -1; dir <= 1; dir += 2) {
                            long q = a + (long) dir * stride;

                            while (q >= textStart && q + 4 <= textEnd && hits.size() < 256) {
                                long qv = readDword(q);
                                if (qv < 0x400000L || !symbolAt.containsKey(qv)) {
                                    break;
                                }
                                MemoryBlock qb = currentProgram.getMemory().getBlock(toAddr(qv));
                                if (qb == null || !EMITTED_DATA.contains(qb.getName())) {
                                    break;
                                }
                                hits.add(q);
                                targets.add(qv);
                                q += (long) dir * stride;
                            }
                        }

                        if (!hits.isEmpty()) {
                            for (int k = 0; k < hits.size(); k++) {
                                long p = hits.get(k);
                                try {
                                    clearListing(toAddr(p), toAddr(p + 3));
                                } catch (Exception e) {
                                    continue;
                                }
                                if (!symbolAt.containsKey(targets.get(k))) {
                                    neededDataLabels.add(targets.get(k));
                                }
                                textPointers.add(p);
                                found++;
                            }
                            break;
                        }
                    }

                    // Scan the WHOLE section, including inside function bodies. Const data sits
                    // among code, and an earlier disassembly pass turned some of it into bogus
                    // "instructions" that persist in the Ghidra database - the rgodf array was one.
                    // Clear whatever is there so the emitter treats these four bytes as the
                    // pointer they are.
                    //
                    // An exact symbol match is what makes this safe to do even inside a function:
                    // a real instruction's bytes would have to coincide exactly with a data
                    // symbol's address across all four bytes at a 4-byte-aligned offset.
                    try {
                        clearListing(toAddr(a), toAddr(a + 3));
                    } catch (Exception e) {
                        continue;
                    }

                    textPointers.add(a);
                    found++;
                }
            }
        }

        println("pointer dwords embedded in .text: " + found);
    }

    /**
     * Disassemble every undefined byte in .text.
     *
     * A region Ghidra never disassembled is emitted as raw bytes, and raw bytes carry raw absolute
     * addresses that nothing relocates. That is invisible to a byte-for-byte comparison against
     * retail - the bytes match precisely BECAUSE they were not touched - and fatal at runtime once
     * .rdata and .data move.
     *
     * It cost a shutdown crash: an unnamed static at 0x58D723 does
     *   push dword ptr [0x006B4AC4]   ; a .data global
     *   call dword ptr [0x005B01E8]   ; an IAT slot
     * with both addresses still pointing where retail put them. The map has no symbol there, so
     * ApplySymbols never made it a function and analysis never reached it.
     */
    void disassembleGaps() throws Exception {
        int made = 0;
        long a = textStart;

        while (a < textEnd) {
            if (monitor.isCancelled()) {
                return;
            }

            Address addr = toAddr(a);

            if (textPointers.contains(a)) {
                a += 4;     // a pointer, not code
                continue;
            }

            if (currentProgram.getListing().getInstructionContaining(addr) != null
                    || currentProgram.getListing().getDefinedDataContaining(addr) != null) {
                a++;
                continue;
            }

            disassemble(addr);

            Instruction insn = currentProgram.getListing().getInstructionAt(addr);
            if (insn != null) {
                made++;
                a += insn.getLength();
            } else {
                a++;
            }
        }

        println("disassembled " + made + " previously-unanalysed instructions in .text");
    }

    boolean isCodeStart(long a) throws Exception {
        return a >= textStart && a < textEnd
                && currentProgram.getListing().getInstructionAt(toAddr(a)) != null;
    }

    /**
     * Emit the parts of .text that are not inside a function.
     *
     * These are mostly switch jump tables, and they are NOT inert bytes: each entry is an absolute
     * code address. Left raw they would still point at the retail addresses, so every entry that
     * targets a real instruction is emitted as a symbol reference instead.
     */
    void emitCodeGaps(PrintWriter w) throws Exception {
        w.println();
        w.println("; ---- .text data: jump tables and padding outside any function ----");
        w.println();

        long a = textStart;
        int tables = 0;

        while (a < textEnd) {
            if (covered[(int) (a - textStart)]) {
                a++;
                continue;
            }

            long runStart = a;
            while (a < textEnd && !covered[(int) (a - textStart)]) {
                a++;
            }

            // Emit this uncovered run, labelled wherever a symbol or a needed label sits.
            for (long p = runStart; p < a; ) {
                String sym = symbolAt.get(p);
                if (sym != null) {
                    if (isLinkableName(sym)) {
                        w.println("PUBLIC " + sym);
                    }
                    w.println(sym + ":");
                }
                if (neededLabels.contains(p)) {
                    w.println(labelFor(p) + ":");
                }

                String target = null;

                if ((p % 4) == 0 && p + 4 <= a) {
                    long value = readDword(p);
                    if (isCodeStart(value)) {
                        String s = symbolAt.get(value);
                        target = (s != null) ? s : labelFor(value);
                        neededLabels.add(value);
                    }
                }

                if (target != null) {
                    w.println("    dd offset " + target);
                    tables++;
                    p += 4;
                    continue;
                }

                int b = currentProgram.getMemory().getByte(toAddr(p)) & 0xFF;
                w.println(String.format("    db 0%02Xh", b));
                p++;
            }

            if (monitor.isCancelled()) {
                return;
            }
        }

        println("  jump-table entries symbolised: " + tables);
    }

    // ---------------------------------------------------------------------------------------
    // Data

    /**
     * Is this address inside text rather than at a pointer?
     *
     * The exact-symbol rule is strong in .text and dangerously weak in .data, because .data is full
     * of strings and a four-byte run of ASCII is numerically indistinguishable from an address. A
     * real pointer here has high byte 0x00 and second byte in 0x40..0x6B - which is to say, the
     * printable range. "tga\0" is 0x00616774, and there is a data symbol at exactly 0x00616774.
     *
     * That collision relocated the middle of the string "texture\0tga\0" by +0x8000, turning the
     * texture extension into "t\xe7a". gfxLoadTexImage then searched for files that do not exist,
     * the loading screen failed to load, and the game showed "not properly installed - insert the
     * original CD" and quit. Every byte-level verifier passed, because a corrupted string is
     * byte-for-byte what a correctly relocated pointer looks like.
     *
     * So Ghidra's data typing gets the final say: if it has decided these bytes are a string, they
     * are a string, and no numeric coincidence overrides that.
     */
    boolean insideString(long a) {
        Data d = currentProgram.getListing().getDefinedDataContaining(toAddr(a));
        if (d == null) {
            return false;
        }
        DataType dt = d.getDataType();
        if (dt == null) {
            return false;
        }
        String n = dt.getName().toLowerCase();
        return n.contains("string") || n.contains("char") || d.hasStringValue();
    }

    /**
     * Is this candidate part of a run of pointers, rather than a lone numeric coincidence?
     *
     * Ghidra's string typing was not enough on its own: it had never typed "texture\0tga\0" as a
     * string, so the guard passed and "tga" was still relocated into "t\xe7a". Something that does
     * not depend on Ghidra having done any particular analysis is needed.
     *
     * Pointer tables come in runs at a consistent stride, and a coincidence does not. Requiring two
     * further candidates at the same stride costs nothing for real tables - the CRT locale tables
     * here are twenty-five entries at stride 8 - while a stray four bytes of ASCII that happens to
     * equal a symbol address has no such company. Genuine lone pointers are still caught by the
     * reference test, because code that loads a global creates a reference to it; what has no
     * reference and no neighbours has no evidence at all, and is left as the bytes it already is.
     */
    /**
     * Do these bytes look like an entry in a string table?
     *
     * Two earlier guards failed on the same address. Ghidra's string typing failed because it had
     * never typed "texture\0tga\0" as a string. Requiring a strided run of pointers failed because
     * this dword did not come through the exact-symbol rule at all - Ghidra had recorded a
     * reference to the location, so the emitter trusted it and asked nearest() for a name, getting
     * "?PLAYER@@3PAVmmPlayer@@A + 219700". A pointer 214 KB past the nearest symbol is not a
     * pointer; it is four bytes of ASCII that happen to be numerically plausible.
     *
     * So this decides from the bytes themselves, needing no analysis to have been done. A string
     * table entry is printable characters ending in a terminator, immediately preceded by another
     * C string. For a real pointer to be rejected here it would have to have all three low bytes
     * printable, a zero high byte, and sit directly after a NUL-terminated string.
     */
    /** A printable, letter-bearing run starting at `from` and walking in `step`, past any padding. */
    boolean plausibleDataAddr(long v) {
        if (v < 0x400000L) {
            return false;
        }
        MemoryBlock b = currentProgram.getMemory().getBlock(toAddr(v));
        return b != null && EMITTED_DATA.contains(b.getName());
    }

    /**
     * How long an ascending table of data pointers this address sits in.
     *
     * The text heuristics were needed because a dword of ASCII looks like an address. This is the
     * same ambiguity seen from the other side: vehLoadNameList is an array of 128 char*, and the
     * entries whose low byte happened to be a letter - 0x005D4864 is 'd','H',']' - were classified
     * as text and left unrelocated while their neighbours relocated correctly. Half a pointer array
     * is worse than none: mmVehList::LoadAll read one, called SetDefaultVehicle with a retail
     * address that means nothing in our image, and the game died on the first vehicle it loaded.
     *
     * What tells the two apart is order. A table of string pointers ascends, in small steps,
     * because it points at consecutive strings - vehLoadNameList climbs by 4, 8 and 12 bytes at a
     * time. Four bytes of ASCII carry no such relationship to the four bytes after them, so a run
     * of four consecutive ascending entries does not happen by accident.
     *
     * This overrides the text heuristics rather than being overridden by them: ascending order is
     * positive evidence about structure, while "these bytes could be read as letters" is an
     * observation about coincidence.
     */
    /**
     * Is this dword one of several pointers inside the same named object?
     *
     * The run rules reason about a window of neighbours and cannot see an object's boundaries, so a
     * short table defeats them: `lpOutputString` is four entries, and no threshold that keeps it
     * also keeps four bytes of ASCII out. Raising and lowering that threshold traded one defect for
     * the other for several rounds.
     *
     * A recovered symbol name is the boundary the run rules were missing. `lpOutputString` is a
     * named object, small, and most of its dwords address the emitted data sections - which is what
     * an array of pointers looks like and what a string does not. The size cap is what keeps a
     * string blob out: those are named too, but they are long, and a handful of coincidental
     * addresses among hundreds of bytes is not evidence of anything.
     *
     * This matters more than its size suggests. `lpOutputString` is the message-prefix array behind
     * every Displayf, Warningf and Errorf in the game, so two stale entries in it hand the printer
     * an unbounded string - harmless while they point into zeroed BSS, and a stack smash the moment
     * the section layout moves.
     */
    /**
     * Objects that are known to be arrays of pointers, named explicitly.
     *
     * Six successive rules were tried to infer this and every one traded one defect for the other:
     * strict enough to keep four bytes of ASCII out, and `lpOutputString` stays broken; loose
     * enough to catch it, and real strings get rewritten as pointers. The distinguishing evidence
     * simply is not in the bytes for a four-entry table.
     *
     * So this says which objects they are. It does not generalise, and it is not meant to - it is a
     * short list of things that were established by reading the code that uses them, which is the
     * only source that actually settles the question.
     *
     * `lpOutputString` earns its place: it is the message-prefix array behind every Displayf,
     * Warningf and Errorf in the game. Two stale entries hand the printer an unbounded string,
     * which is harmless while they point into zeroed BSS and a stack smash the moment the section
     * layout moves - which is exactly what porting the next few hundred bytes of anything does.
     */
    static final Set<String> POINTER_ARRAYS = new HashSet<>(java.util.Arrays.asList(
            "lpOutputString"));

    boolean inNamedPointerObject(long a) throws Exception {
        Map.Entry<Long, String> e = sortedSymbols.floorEntry(a);
        if (e == null || !POINTER_ARRAYS.contains(e.getValue())) {
            return false;
        }

        Long next = sortedSymbols.higherKey(e.getKey());
        long objEnd = next != null ? next : e.getKey() + 0x40;
        return a + 4 <= objEnd && plausibleDataAddr(readDword(a));
    }

    /** A real recovered name, as opposed to one Ghidra invented. */
    boolean isLinkableOrNamed(String name) {
        if (name == null) {
            return false;
        }
        for (String prefix : new String[] {
                "DAT_", "LAB_", "FUN_", "SUB_", "UNK_", "unk_", "loc_", "sub_",
                "byte_", "word_", "dword_", "off_", "flt_", "dbl_", "asc_",
                "D_", "L_", "caseD_", "switchD", "default_", "nullsub_", "j_", "s_", "u_"}) {
            if (name.startsWith(prefix)) {
                return false;
            }
        }
        return true;
    }

    int pointerTableRun(long a, long start, long initEnd, boolean allowGap)
            throws Exception {
        if (!plausibleDataAddr(readDword(a))) {
            return 0;
        }

        // Monotone in EITHER direction, and at more strides than 4.
        //
        // Requiring strictly ascending stride-4 was too narrow and a static audit proved it: 21
        // pointers stayed unrelocated in four real tables. MSVC's `_rterrs` has stride 8; the
        // ".com/.exe/.bat/.cmd" extension list DESCENDS; and DefaultPrinter's message-prefix array
        // broke the run at three entries, one short of the threshold. Two of those tables point
        // into zeroed BSS today, so the wrong pointers behave correctly and nothing at runtime
        // would ever have shown it.
        //
        // Direction carries no meaning - a table of pointers to consecutive strings ascends if the
        // strings were emitted in order and descends if they were not - so requiring one direction
        // was an assumption, not evidence. What actually distinguishes a table from a coincidence
        // is that consecutive entries are *related*: same stride, same direction, small step.
        int best = 1;

        for (int stride : new int[] {4, 8, 12, 16}) {
            for (int dir = -1; dir <= 1; dir += 2) {
                int n = 1;
                int skipped = 0;

                for (long p = a; ; p += (long) dir * stride) {
                    long q = p + (long) dir * stride;
                    if (q < start || q + 4 > initEnd) {
                        break;
                    }

                    long cur = readDword(p), next = readDword(q);
                    if (!plausibleDataAddr(next)) {
                        break;
                    }

                    // Nearness, not order.
                    //
                    // Monotonicity was the third assumption in this rule to fail, and it failed on
                    // the first entry of four different tables: zlib's z_errmsg and MSVC's message
                    // arrays point at strings that were not emitted in address order, so the chain
                    // breaks at exactly one link and takes the entry with it.
                    //
                    // What actually makes a table a table is that its entries address the same
                    // neighbourhood at a fixed stride. A table of pointers to related strings does
                    // that whatever order the strings ended up in; four bytes of ASCII that happen
                    // to look like an address do not, because nothing constrains the four bytes
                    // beside them to point anywhere near.
                    long step = next - cur;
                    if (step > 4096 || step < -4096) {
                        // Tolerate one broken link, then stop.
                        //
                        // Four pointers were still being missed, every one the FIRST entry of a
                        // real table - zlib's z_errmsg, MSVC's message arrays - because the string
                        // they point at happens to sit far from the next one, so the chain snapped
                        // at exactly one link and took that entry with it. Allowing a single gap
                        // reaches across it; allowing two would start joining unrelated tables.
                        // The gap is only allowed where nothing suggests this is text. Allowing
                        // it everywhere let a run chain across a string table and rewrote two real
                        // strings as pointers - the same collision as always, one step further out.
                        if (!allowGap || skipped > 0) {
                            break;
                        }
                        skipped++;
                    }
                    n++;
                }

                if (n > best) {
                    best = n;
                }
            }
        }

        return best;
    }

    boolean stringRunNear(long from, int step) {
        try {
            long p = from;
            for (int skipped = 0; skipped < 12; skipped++) {
                if ((currentProgram.getMemory().getByte(toAddr(p)) & 0xFF) != 0) {
                    break;
                }
                p += step;
            }

            int run = 0, letters = 0;
            for (int i = 0; i < 24; i++) {
                int b = currentProgram.getMemory().getByte(toAddr(p)) & 0xFF;
                if (b < 0x20 || b >= 0x7F) {
                    break;
                }
                if ((b >= 'A' && b <= 'Z') || (b >= 'a' && b <= 'z')) {
                    letters++;
                }
                run++;
                p += step;
            }
            return run >= 3 && letters >= 2;
        } catch (Exception e) {
            return false;
        }
    }

    /** Does this run contain a printf conversion - a '%' followed by a conversion character? */
    boolean looksLikeFormat(long a, int len) {
        try {
            for (int i = 0; i + 1 < len; i++) {
                if ((currentProgram.getMemory().getByte(toAddr(a + i)) & 0xFF) != '%') {
                    continue;
                }
                for (int j = i + 1; j < len; j++) {
                    int c = currentProgram.getMemory().getByte(toAddr(a + j)) & 0xFF;
                    if ("diouxXeEfgGcspn%".indexOf(c) >= 0) {
                        return true;
                    }
                    // Flags, width and precision may sit between the % and the conversion.
                    if ("-+ #.0123456789".indexOf(c) < 0) {
                        break;
                    }
                }
            }
        } catch (Exception e) {
            return false;
        }
        return false;
    }

    boolean looksLikeStringData(long a) {
        try {
            int nul = -1;
            int letters = 0;

            for (int i = 0; i < 4; i++) {
                int b = currentProgram.getMemory().getByte(toAddr(a + i)) & 0xFF;
                if (b == 0) {
                    nul = i;
                    break;
                }
                if (b < 0x20 || b >= 0x7F) {
                    return false;
                }
                if ((b >= 'A' && b <= 'Z') || (b >= 'a' && b <= 'z')) {
                    letters++;
                }
            }

            if (nul <= 0) {
                return false;       // no characters, or no terminator inside the dword
            }

            // Two letters is what separates text from a pointer that merely prints. An address in
            // this image has its second byte in 0x40..0x6B, so one letter is common; two is not.
            // The same rule, applied independently in tools/verify_strings.py, keeps "tga" and
            // drops all sixty pointers like ",w\" that a length test alone flagged.
            // A printf conversion beats the letter count.
            //
            // Two letters is a decent proxy for "this is text", but format strings are short and
            // mostly punctuation: "%5d" has one letter, "%4d" one, "#%d" one, "%s_" one. All four
            // were relocated as pointers in a shipped build - "%5d" is 0x00643525, which lands in
            // .data, so it looked exactly like an address. The consequence is worse than a wrong
            // string: sprintf reads its arguments according to the format, so corrupting one makes
            // it misread the stack, and turning a %d into a %s hands it an integer to dereference.
            //
            // A '%' followed by a conversion character is not a coincidence in the way that three
            // printable bytes are.
            if (letters < 2 && !looksLikeFormat(a, nul)) {
                return false;
            }

            // A string keeps company with other strings; a pointer keeps company with pointers.
            // So look for a neighbouring string on EITHER side, skipping the NUL padding these
            // tables are aligned with.
            //
            // Both directions are needed. Looking only backwards missed a second copy of "tga" and
            // the extensions "bnd" and "pkg", because those sit immediately after a binary colour
            // table with no padding in front of them - but each is followed by the next entry in
            // its table ("tga\0text..."), which is just as good evidence.
            return stringRunNear(a + nul + 1, 1) || stringRunNear(a - 1, -1);
        } catch (Exception e) {
            return false;
        }
    }

    boolean inPointerRun(long a, Set<Long> candidates) {
        for (int s : new int[] {4, 8, 12, 16, 20, 24, 28, 32}) {
            int n = 1;
            if (candidates.contains(a - s)) n++;
            if (candidates.contains(a - 2L * s)) n++;
            if (candidates.contains(a + s)) n++;
            if (candidates.contains(a + 2L * s)) n++;
            if (n >= 3) {
                return true;
            }
        }
        return false;
    }

    void emitDataBlock(PrintWriter w, String blockName, String segment, String outDir)
            throws Exception {
        MemoryBlock block = currentProgram.getMemory().getBlock(blockName);
        if (block == null) {
            println("no block " + blockName);
            return;
        }

        long start = block.getStart().getOffset();
        long end = block.getEnd().getOffset() + 1;

        // Initialised bytes only; the rest of the block is BSS and goes to .DATA?.
        long initEnd = end;
        if (!block.isInitialized()) {
            initEnd = start;
        }

        println("emitting " + blockName + " 0x" + Long.toHexString(start)
                + "..0x" + Long.toHexString(end));

        w.println();
        w.println(segment);
        w.println();

        ReferenceManager rm = currentProgram.getReferenceManager();
        stringsProtected = 0;

        // Every 4-aligned dword whose value lands exactly on a known symbol. Collected up front so
        // a candidate can be judged by the company it keeps.
        Set<Long> candidates = new HashSet<>();
        for (long a = start; a + 4 <= initEnd; a += 4) {
            long v = readDword(a);
            if (v >= 0x400000L && symbolAt.containsKey(v)) {
                candidates.add(a);
            }
        }

        StringBuilder run = new StringBuilder();
        int runLen = 0;

        for (long a = start; a < initEnd; ) {
            String sym = symbolAt.get(a);

            if (sym != null) {
                flushRun(w, run, runLen);
                runLen = 0;

                if (isLinkableName(sym)) {
                    w.println("PUBLIC " + sym);
                }
                w.println(sym + " LABEL BYTE");
            }

            if (neededDataLabels.contains(a)) {
                flushRun(w, run, runLen);
                runLen = 0;
                w.println(dataLabelFor(a) + " LABEL BYTE");
                dataLabelsEmitted++;
            }

            // Is this a pointer?
            //
            // "Value happens to fall in the image range" is not good enough - a small float is
            // 0x004xxxxx too, and symbolising one would change its value. Two things are trusted
            // instead: an exact match against a known symbol, and Ghidra's own data analysis,
            // which decides pointer-ness from the data type it recovered rather than from the
            // numeric value.
            //
            // Anything Ghidra calls a pointer but which lands INSIDE an object rather than at its
            // start is emitted as `offset SYM + delta`. That is exact, because each section is
            // written as one contiguous block, so distances within it are preserved.
            String target = null;
            long addend = 0;

            if ((a % 4) == 0 && a + 4 <= initEnd) {
                long value = readDword(a);
                String exact = symbolAt.get(value);

                boolean referenced = hasDataReference(a);
                // A longer run is required to overrule the text heuristics than to stand alone.
                //
                // These two rules genuinely conflict, and each has been wrong in its own direction:
                // the text rule left half of vehLoadNameList - a 128-entry array of char* - as raw
                // bytes, and the table rule rewrote four real strings as pointers. Neither can
                // simply win.
                //
                // Length settles it. A real table is long: vehLoadNameList is 128 entries and the
                // CRT message arrays are ten or more. An accidental run through a string table is
                // four or five entries and then falls apart. So four entries is enough where
                // nothing suggests text, and eight are required where something does.
                // Text gets the strict rule; everything else may reach across one gap.
                boolean textish = insideString(a) || looksLikeStringData(a);
                int runLength = pointerTableRun(a, start, initEnd, !textish);
                boolean inTable = runLength >= (textish ? 8 : 4) || inNamedPointerObject(a);

                // A printf conversion vetoes everything, including the table rule.
                //
                // "%5d" survived the text check and was relocated anyway, because it sits among
                // enough plausible addresses to look like a pointer table. Nothing is worth that:
                // a format string decides how sprintf reads its arguments, so corrupting one can
                // turn a %d into a %s and hand it an integer to dereference. A real pointer whose
                // bytes happen to spell a valid conversion is possible; losing one is a stale
                // pointer, while losing a format string is a stack smash.
                if (looksLikeFormat(a, 4)) {
                    inTable = false;
                }

                if (value >= 0x400000L && inTable && !looksLikeFormat(a, 4)) {
                    // An ascending run of pointers. Structure beats coincidence.
                    if (exact != null) {
                        target = exact;
                    } else {
                        String[] near = nearest(value);
                        if (near != null) {
                            target = near[0];
                            addend = Long.parseLong(near[1]);
                            recoveredPointers++;
                        }
                    }
                } else if (value >= 0x400000L && !insideString(a) && !looksLikeStringData(a)
                        && !looksLikeFormat(a, 4)) {
                    if (exact != null && (referenced || inPointerRun(a, candidates))) {
                        target = exact;
                    } else if (referenced) {
                        String[] near = nearest(value);
                        if (near != null) {
                            target = near[0];
                            addend = Long.parseLong(near[1]);
                            recoveredPointers++;
                        }
                    } else if (exact != null) {
                        stringsProtected++;   // lone match, no reference: treated as data
                    }
                } else if (value >= 0x400000L) {
                    stringsProtected++;
                }
            }

            if (target != null) {
                flushRun(w, run, runLen);
                runLen = 0;
                w.println("    dd offset " + target + (addend != 0 ? " + " + addend : ""));
                dataPointers++;
                a += 4;
                continue;
            }

            int b = currentProgram.getMemory().getByte(toAddr(a)) & 0xFF;

            if (runLen > 0) {
                run.append(", ");
            }
            run.append(String.format("0%02Xh", b));
            runLen++;

            if (runLen == 16) {
                flushRun(w, run, runLen);
                runLen = 0;
            }

            a++;

            if (monitor.isCancelled()) {
                break;
            }
        }

        flushRun(w, run, runLen);

        println("  " + blockName + ": " + stringsProtected
                + " lone symbol-valued dwords left as data (no reference, no run)");

        if (initEnd < end) {
            w.println();
            w.println(".DATA?");
            w.println();

            for (long a = initEnd; a < end; ) {
                String sym = symbolAt.get(a);
                if (sym != null) {
                    if (isLinkableName(sym)) {
                        w.println("PUBLIC " + sym);
                    }
                    w.println(sym + " LABEL BYTE");
                }

                long next = end;
                Long k = sortedSymbols.higherKey(a);
                if (k != null && k < end) {
                    next = k;
                }

                w.println("    db " + (next - a) + " dup (?)");
                a = next;
            }
        }
    }

    /** Did Ghidra's data analysis record a pointer originating at this address? */
    boolean hasDataReference(long a) {
        Reference[] refs = currentProgram.getReferenceManager().getReferencesFrom(toAddr(a));
        return refs != null && refs.length > 0;
    }

    void flushRun(PrintWriter w, StringBuilder run, int runLen) {
        if (runLen > 0) {
            w.println("    db " + run.toString());
        }
        run.setLength(0);
    }

    long readDword(long a) throws Exception {
        return ((long) currentProgram.getMemory().getInt(toAddr(a))) & 0xFFFFFFFFL;
    }

    void emitBytes(PrintWriter w, byte[] bytes, int from, int len) {
        emitBytes(w, bytes, from, len, -1);
    }

    /**
     * Emit a byte run, breaking it wherever a label has to land.
     *
     * Some references point INTO an instruction rather than at its start - a jump table computed
     * off a base, mostly. Those addresses still need a label, and because the output is raw bytes
     * rather than mnemonics a label can simply be placed between two `db` directives. Anything
     * that assumes labels only occur at instruction boundaries leaves them undefined.
     */
    void emitBytes(PrintWriter w, byte[] bytes, int from, int len, long baseAddr) {
        StringBuilder sb = new StringBuilder();
        int run = 0;

        for (int i = 0; i < len; i++) {
            if (baseAddr >= 0) {
                long here = baseAddr + from + i;

                if (i > 0 && (neededLabels.contains(here) || symbolAt.containsKey(here))) {
                    if (run > 0) {
                        w.println("    db " + sb);
                        sb.setLength(0);
                        run = 0;
                    }

                    String s = symbolAt.get(here);
                    if (s != null) {
                        w.println(s + ":");
                    }
                    if (neededLabels.contains(here)) {
                        w.println(labelFor(here) + ":");
                    }
                }
            }

            if (run > 0) {
                sb.append(", ");
            }
            sb.append(String.format("0%02Xh", bytes[from + i] & 0xFF));
            run++;

            if (run == 16) {
                w.println("    db " + sb);
                sb.setLength(0);
                run = 0;
            }
        }

        if (run > 0) {
            w.println("    db " + sb);
        }
    }

    int findDword(byte[] bytes, int value) {
        for (int i = 0; i + 4 <= bytes.length; i++) {
            int v = (bytes[i] & 0xFF) | ((bytes[i + 1] & 0xFF) << 8)
                    | ((bytes[i + 2] & 0xFF) << 16) | ((bytes[i + 3] & 0xFF) << 24);

            if (v == value) {
                return i;
            }
        }

        return -1;
    }
}
