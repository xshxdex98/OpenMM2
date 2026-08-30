// Apply the midtown2.map symbols to the program.
//
// Ghidra names a stripped 1999 binary FUN_004c8af0. We already know all 10,608 real names and
// their addresses, so applying them is what makes the decompiler output reviewable: every call
// site shows a real class and method. Ghidra's Microsoft demangler then runs over the mangled
// labels and recovers signatures on top.
//
// Addresses come from midtown2.map's section:offset column, not its Rva+Base column, which is
// 0x308 too high for .rdata. See tools/pe.py in the OpenMM2 tree.
//
// Paths are written with forward slashes on purpose: Java accepts them on Windows, and it keeps
// the source free of escape sequences.
//
//@category OpenMM2

import java.io.BufferedReader;
import java.io.FileReader;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.SourceType;

public class ApplySymbols extends GhidraScript {

    @Override
    public void run() throws Exception {
        String path = "C:/mm2ghidra/ghidra_symbols.tsv";

        String[] args = getScriptArgs();
        if (args.length > 0) {
            path = args[0];
        }

        int labels = 0;
        int funcs = 0;
        int skipped = 0;
        int lineNo = 0;

        BufferedReader in = new BufferedReader(new FileReader(path));
        String line;

        while ((line = in.readLine()) != null) {
            lineNo++;

            if (lineNo == 1) {
                continue; // header
            }

            String[] f = line.split("\t", -1);
            if (f.length < 4) {
                continue;
            }

            long va;
            try {
                va = Long.parseLong(f[0], 16);
            } catch (NumberFormatException e) {
                continue;
            }

            String mangled = f[1];
            boolean isCode = "1".equals(f[3]);

            Address addr = toAddr(va);
            MemoryBlock block = currentProgram.getMemory().getBlock(addr);

            if (block == null) {
                skipped++;
                continue;
            }

            // The mangled name goes on as-is. The demangler analyzer consumes it and attaches the
            // readable signature itself, which is strictly better than anything reconstructed here.
            try {
                createLabel(addr, mangled, true, SourceType.IMPORTED);
                labels++;
            } catch (Exception e) {
                skipped++;
                continue;
            }

            if (isCode) {
                Function fn = getFunctionAt(addr);

                if (fn == null) {
                    disassemble(addr);
                    fn = createFunction(addr, null);

                    if (fn != null) {
                        funcs++;
                    }
                }
            }

            if (monitor.isCancelled()) {
                break;
            }

            if (labels % 2000 == 0) {
                println("  ... " + labels + " labels, " + funcs + " functions");
            }
        }

        in.close();

        println("ApplySymbols: " + labels + " labels, " + funcs + " functions created, "
                + skipped + " skipped");
    }
}
