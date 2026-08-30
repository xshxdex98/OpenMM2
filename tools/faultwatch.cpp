// Minimal debugger: run a program, report where it faults - or where it chose to quit.
//
// There is no cdb on this machine and Windows Error Reporting did not record the crash, so this
// exists to answer one question precisely - the RVA of the faulting instruction, which
// tools/symbols.json turns straight into a function name.
//
// Also reports the address the program tried to touch and whether it was a read or a write. For
// this project that distinction matters: an unsymbolised data pointer displaced by 0x4000 shows up
// as a read of an address exactly 0x4000 away from something real.
//
// Optional +RVA arguments plant INT3 breakpoints. A clean exit(0) leaves no fault to catch, so the
// only way to learn WHERE a process decided to quit is to trap the call itself: break on exit, and
// the stack dump names the caller. Nothing crashes on that path, so there is otherwise nothing to
// look at.
//
// Build: py tools/mkfaultwatch.py
// Usage: faultwatch <exe> [working-dir] [+RVA ...]

#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::printf("usage: faultwatch <exe> [working-dir] [+RVA ...]\n");
        return 2;
    }

    SIZE_T bp_rva[16];
    int bp_count = 0;

    // Addresses to print when a breakpoint is hit, given as @RVA.
    //
    // The game never logs the resolution it actually selected - only the cap. Reading
    // gfxPipeline::m_iWidth and m_iHeight out of the live process is the only way to know whether
    // 1440p is really in use or whether the process was handed a DPI-virtualised size.
    SIZE_T dump_rva[16];
    int dump_count = 0;

    // A register whose target string should be printed at a breakpoint, given as %edi etc.
    char str_reg[8] = {0};

    const char* workdir = nullptr;

    for (int i = 2; i < argc; ++i) {
        if (std::strcmp(argv[i], "--") == 0) {
            break;
        }
        if (argv[i][0] == '+') {
            if (bp_count < 16)
                bp_rva[bp_count++] = (SIZE_T) std::strtoul(argv[i] + 1, nullptr, 16);
        } else if (argv[i][0] == '%') {
            std::strncpy(str_reg, argv[i] + 1, sizeof(str_reg) - 1);
        } else if (argv[i][0] == '@') {
            if (dump_count < 16)
                dump_rva[dump_count++] = (SIZE_T) std::strtoul(argv[i] + 1, nullptr, 16);
        } else if (!workdir) {
            workdir = argv[i];
        }
    }

    STARTUPINFOA si {};
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi {};

    // Everything after a lone "--" is passed to the game rather than read as an option here, so a
    // flag like -gl can be tested under the debugger instead of only from a shell.
    char cmd[MAX_PATH * 4];
    int n = std::snprintf(cmd, sizeof(cmd), "\"%s\"", argv[1]);

    bool passthrough = false;
    for (int i = 2; i < argc; ++i) {
        if (!passthrough) {
            if (std::strcmp(argv[i], "--") == 0) passthrough = true;
            continue;
        }
        n += std::snprintf(cmd + n, sizeof(cmd) - n, " %s", argv[i]);
    }

    if (!CreateProcessA(nullptr, cmd, nullptr, nullptr, FALSE,
                        DEBUG_ONLY_THIS_PROCESS, nullptr, workdir, &si, &pi)) {
        std::printf("CreateProcess failed: %lu\n", GetLastError());
        return 2;
    }

    // The image base is only known once the loader reports it, and every address has to be
    // reported as an RVA to be useful - an absolute address means nothing without it.
    SIZE_T image_base = 0;
    int reported = 0;

    for (;;) {
        DEBUG_EVENT ev {};
        if (!WaitForDebugEvent(&ev, 60000)) {
            std::printf("timed out waiting for the process\n");
            break;
        }

        DWORD action = DBG_CONTINUE;

        switch (ev.dwDebugEventCode) {
        case CREATE_PROCESS_DEBUG_EVENT:
            image_base = reinterpret_cast<SIZE_T>(ev.u.CreateProcessInfo.lpBaseOfImage);
            std::printf("image base: 0x%08zX\n", image_base);

            for (int i = 0; i < bp_count; ++i) {
                LPVOID at = reinterpret_cast<LPVOID>(image_base + bp_rva[i]);
                BYTE int3 = 0xCC;
                SIZE_T wrote = 0;
                DWORD old = 0;
                if (VirtualProtectEx(pi.hProcess, at, 1, PAGE_EXECUTE_READWRITE, &old) &&
                    WriteProcessMemory(pi.hProcess, at, &int3, 1, &wrote) && wrote == 1) {
                    std::printf("breakpoint armed at RVA 0x%06zX\n", bp_rva[i]);
                } else {
                    std::printf("could NOT arm breakpoint at RVA 0x%06zX (%lu)\n",
                                bp_rva[i], GetLastError());
                }
            }

            if (ev.u.CreateProcessInfo.hFile) CloseHandle(ev.u.CreateProcessInfo.hFile);
            break;

        case LOAD_DLL_DEBUG_EVENT:
            if (ev.u.LoadDll.hFile) CloseHandle(ev.u.LoadDll.hFile);
            break;

        case OUTPUT_DEBUG_STRING_EVENT: {
            char buf[512] {};
            SIZE_T got = 0;
            if (ev.u.DebugString.nDebugStringLength < sizeof(buf) &&
                ReadProcessMemory(pi.hProcess, ev.u.DebugString.lpDebugStringData,
                                  buf, ev.u.DebugString.nDebugStringLength, &got)) {
                std::printf("[dbg] %s", buf);
            }
            break;
        }

        case EXCEPTION_DEBUG_EVENT: {
            const EXCEPTION_RECORD& er = ev.u.Exception.ExceptionRecord;

            // The loader raises a breakpoint on attach; that one is normal and must be continued.
            if (er.ExceptionCode == EXCEPTION_BREAKPOINT && reported == 0) {
                reported = 1;
                break;
            }

            SIZE_T at = reinterpret_cast<SIZE_T>(er.ExceptionAddress);

            std::printf("\nEXCEPTION 0x%08lX at 0x%08zX", er.ExceptionCode, at);
            if (image_base && at >= image_base)
                std::printf("   RVA 0x%06zX", at - image_base);
            std::printf("\n");

            if (er.ExceptionCode == EXCEPTION_ACCESS_VIOLATION && er.NumberParameters >= 2) {
                const char* how = er.ExceptionInformation[0] == 0 ? "read"
                                : er.ExceptionInformation[0] == 1 ? "write" : "execute";
                std::printf("  tried to %s 0x%08zX\n", how, er.ExceptionInformation[1]);
            }

            std::printf("  first chance: %s\n", ev.u.Exception.dwFirstChance ? "yes" : "no");

            // Registers and the top of the stack. When execution jumps to a bad address the CALL
            // has already pushed its return address, so [ESP] names the caller - the only thing
            // that identifies where the bad pointer came from.
            CONTEXT ctx {};
            ctx.ContextFlags = CONTEXT_FULL;

            if (GetThreadContext(pi.hThread, &ctx)) {
                std::printf("  eip=%08lX esp=%08lX ebp=%08lX\n", ctx.Eip, ctx.Esp, ctx.Ebp);
                std::printf("  eax=%08lX ecx=%08lX edx=%08lX ebx=%08lX esi=%08lX edi=%08lX\n",
                            ctx.Eax, ctx.Ecx, ctx.Edx, ctx.Ebx, ctx.Esi, ctx.Edi);

                DWORD stack[12] {};
                SIZE_T got = 0;
                if (ReadProcessMemory(pi.hProcess, reinterpret_cast<LPCVOID>(ctx.Esp),
                                      stack, sizeof(stack), &got)) {
                    for (int i = 0; i < 12; ++i) {
                        std::printf("  [esp+%02X] %08lX", i * 4, stack[i]);
                        if (image_base && stack[i] >= image_base &&
                            stack[i] < image_base + 0x400000) {
                            std::printf("   rva %06lX", stack[i] - (DWORD)image_base);
                        }
                        std::printf("\n");
                    }
                }
            }

            for (int i = 0; i < dump_count; ++i) {
                DWORD value = 0;
                SIZE_T got = 0;
                LPCVOID at_addr = reinterpret_cast<LPCVOID>(image_base + dump_rva[i]);
                if (ReadProcessMemory(pi.hProcess, at_addr, &value, sizeof(value), &got)) {
                    std::printf("  [RVA %06zX] = %lu (0x%08lX)\n", dump_rva[i], value, value);
                } else {
                    std::printf("  [RVA %06zX] unreadable (%lu)\n", dump_rva[i], GetLastError());
                }
            }

            // The string a register points at, and how long it is.
            //
            // A fixed RVA cannot answer the question that matters here: PlayerFillStats copies a
            // localised string into a 32-byte stack buffer with no length check, so what decides
            // whether the frame survives is the length of a string whose address only exists in a
            // register at that moment.
            if (str_reg[0] && GetThreadContext(pi.hThread, &ctx)) {
                DWORD addr = 0;
                if (!std::strcmp(str_reg, "edi")) addr = ctx.Edi;
                else if (!std::strcmp(str_reg, "esi")) addr = ctx.Esi;
                else if (!std::strcmp(str_reg, "eax")) addr = ctx.Eax;
                else if (!std::strcmp(str_reg, "ecx")) addr = ctx.Ecx;
                else if (!std::strcmp(str_reg, "edx")) addr = ctx.Edx;
                else if (!std::strcmp(str_reg, "ebx")) addr = ctx.Ebx;

                char text[512] {};
                SIZE_T got = 0;
                if (addr && ReadProcessMemory(pi.hProcess, reinterpret_cast<LPCVOID>(addr),
                                              text, sizeof(text) - 1, &got)) {
                    text[sizeof(text) - 1] = 0;
                    std::printf("  %s -> 0x%08lX  len=%zu  \"%.200s\"\n",
                                str_reg, addr, std::strlen(text), text);
                } else {
                    std::printf("  %s -> 0x%08lX  unreadable\n", str_reg, addr);
                }
            }

            // A planted breakpoint has told us everything it can once the stack is printed, and
            // resuming from it would just re-execute the clobbered byte.
            if (er.ExceptionCode == EXCEPTION_BREAKPOINT) {
                std::printf("  (planted breakpoint - [esp+00] is the caller)\n");
                TerminateProcess(pi.hProcess, 1);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                return 1;
            }

            // Only a SECOND-chance exception is a crash.
            //
            // This used to terminate on any access violation, first chance included, and that was
            // wrong in a way that distorted real results: MM2 uses structured exception handling
            // throughout - the same run handled 18,529 guard-page exceptions without trouble - so a
            // first-chance access violation may well be caught and dealt with by the game. Killing
            // the process at that point reports a crash that would not have happened, and ends a
            // play session that was going fine.
            //
            // Passing it back with DBG_EXCEPTION_NOT_HANDLED is what a debugger is supposed to do:
            // let the program's own handler run. If nothing handles it, the exception comes round
            // again with dwFirstChance clear, and that is a genuine crash.
            if (!ev.u.Exception.dwFirstChance) {
                std::printf("  (unhandled - this one is a real crash)\n");
                TerminateProcess(pi.hProcess, 1);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                return 1;
            }

            action = DBG_EXCEPTION_NOT_HANDLED;
            break;
        }

        case EXIT_PROCESS_DEBUG_EVENT:
            std::printf("exited, code %lu\n", ev.u.ExitProcess.dwExitCode);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            return 0;
        }

        ContinueDebugEvent(ev.dwProcessId, ev.dwThreadId, action);
    }

    TerminateProcess(pi.hProcess, 1);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 1;
}
