/* Decompile every function in midtown2.exe to C, in the shape MM2_RE_KIT already uses.
 *
 * WHY. MM2_RE_KIT/MM2_PSEUDOCODE is Hex-Rays output and covers 71.3% of the binary's functions.
 * The other 1,792 real methods - 231 KB, including most of mmInterface, mmPlayer, aiMap and
 * vehCarModel - have no decompilation at all, so porting one of those starts from raw
 * disassembly. Ghidra has its own decompiler and this project is already set up with symbols
 * applied, so that coverage is free.
 *
 * A SECOND DECOMPILER IS WORTH HAVING EVEN WHERE THE FIRST ONE COVERS THE FUNCTION. They are
 * independent implementations and they disagree in useful places: Hex-Rays rendered
 * PlayFeedbackSampleID's argument table as if the addresses were immediates, which the
 * disassembly contradicted. Two readings that agree are evidence; two that differ are a prompt to
 * go and look. So this decompiles everything rather than only the gap.
 *
 * NEITHER IS TRUTH. Both are reconstructions, and the binary is the authority - see the note in
 * docs/harness.md about checking operand encodings against the disassembly before trusting a
 * decompiled signature.
 *
 * Output mirrors the kit exactly - <out>/<Class>/<Class>.c, one `// <mangled>` line above each
 * body - so every tool that already reads the kit reads this without changes.
 *
 * WHERE IT WRITES. Into MM2_RE_KIT/GHIDRA_DECOMP by default, which is inside the directory
 * .gitignore already excludes wholesale. This is derived from a copyrighted binary and must never
 * be committed; every contributor generates their own, exactly as with game.asm.
 *
 * Run:
 *   analyzeHeadless.bat C:/mm2ghidra/proj MM2 -process midtown2.exe -noanalysis \
 *       -scriptPath C:/mm2ghidra/scripts -postScript ExportDecomp.java <outdir>
 */

import java.io.File;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;

public class ExportDecomp extends GhidraScript {

    /** The class a mangled name belongs to, or "_free" for a free function. */
    static String classOf(String mangled) {
        if (mangled == null || !mangled.startsWith("?")) {
            return "_other";
        }
        int at = mangled.indexOf("@@");
        if (at < 0) {
            return "_other";
        }
        // ?Method@Class@@... - the segment after the first @ and before @@ is the class.
        String head = mangled.substring(1, at);
        int sep = head.indexOf('@');
        if (sep < 0) {
            return "_free";           // ?frand@@YAMXZ has no class segment
        }
        String cls = head.substring(sep + 1);
        int nested = cls.indexOf('@');
        if (nested >= 0) {
            cls = cls.substring(0, nested);
        }
        return cls.isEmpty() ? "_free" : cls;
    }

    static String safe(String name) {
        StringBuilder sb = new StringBuilder();
        for (char c : name.toCharArray()) {
            sb.append(Character.isLetterOrDigit(c) || c == '_' ? c : '_');
        }
        String s = sb.toString();
        return s.isEmpty() ? "_other" : s;
    }

    @Override
    public void run() throws Exception {
        String outDir = "MM2_RE_KIT/GHIDRA_DECOMP";
        String[] args = getScriptArgs();
        if (args.length > 0) {
            outDir = args[0];
        }
        new File(outDir).mkdirs();

        DecompInterface decomp = new DecompInterface();
        decomp.setOptions(new DecompileOptions());
        if (!decomp.openProgram(currentProgram)) {
            println("could not open the program for decompilation: " + decomp.getLastMessage());
            return;
        }

        List<Function> funcs = new ArrayList<>();
        FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
        while (it.hasNext()) {
            funcs.add(it.next());
        }
        funcs.sort(Comparator.comparingLong(f -> f.getEntryPoint().getOffset()));
        println("functions to decompile: " + funcs.size());

        // Grouped in memory and written once per class, so a class's functions land in one file in
        // address order rather than the file being reopened per function.
        Map<String, StringBuilder> byClass = new HashMap<>();
        Map<String, Integer> counts = new HashMap<>();

        int done = 0;
        int failed = 0;

        for (Function f : funcs) {
            if (monitor.isCancelled()) {
                break;
            }

            String mangled = f.getName();
            String cls = safe(classOf(mangled));

            String body;
            try {
                // 30 seconds is generous; the largest function in the binary is 13 KB.
                DecompileResults res = decomp.decompileFunction(f, 30, monitor);
                if (res != null && res.decompileCompleted()
                        && res.getDecompiledFunction() != null) {
                    body = res.getDecompiledFunction().getC();
                } else {
                    body = "// DECOMPILATION FAILED: "
                            + (res == null ? "no result" : res.getErrorMessage());
                    failed++;
                }
            } catch (Exception e) {
                body = "// DECOMPILATION FAILED: " + e;
                failed++;
            }

            StringBuilder sb = byClass.computeIfAbsent(cls, k -> new StringBuilder());
            sb.append("\n// ------------------------------------------------------------------\n");
            sb.append("// ").append(mangled).append("\n");
            sb.append("// ea=0x").append(String.format("%08X", f.getEntryPoint().getOffset()));
            sb.append("  size=0x").append(Long.toHexString(f.getBody().getNumAddresses()).toUpperCase());
            sb.append("\n\n").append(body).append("\n");
            counts.merge(cls, 1, Integer::sum);

            if (++done % 1000 == 0) {
                println("  decompiled " + done + " / " + funcs.size());
            }
        }

        for (Map.Entry<String, StringBuilder> e : byClass.entrySet()) {
            File dir = new File(outDir, e.getKey());
            dir.mkdirs();
            PrintWriter w = new PrintWriter(new File(dir, e.getKey() + ".c"), "UTF-8");
            w.println("// ===== " + e.getKey() + " - " + counts.get(e.getKey())
                    + " functions, decompiled by Ghidra =====");
            w.print(e.getValue());
            w.close();
        }

        decomp.dispose();
        println("ExportDecomp: " + done + " functions into " + byClass.size() + " files, "
                + failed + " failed");
    }
}
