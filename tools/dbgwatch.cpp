/*
    OpenMM2 - a reimplementation of Midtown Madness 2

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

// Watch the game's debug output WITHOUT being a debugger.
//
// tools/faultwatch.cpp has to own the process it inspects, and that turned out to matter: it was
// terminating the game on first-chance access violations that MM2's own exception handlers deal
// with routinely, so a play session that was going fine ended in a reported "crash" that had not
// happened. Worse, it ends whatever the person at the keyboard was doing.
//
// This attaches to nothing. OutputDebugStringA writes into a shared memory block called
// DBWIN_BUFFER and signals an event; any process may read it, which is how DebugView works. So the
// game can be played normally while its log is captured, and closing this tool does nothing to the
// game at all.
//
// The one limitation worth knowing: this is a single-reader protocol. If a real debugger is
// attached to the game, the output goes there instead and nothing appears here.
//
// Build: py tools/mkdbgwatch.py
// Usage: dbgwatch [seconds] [> log.txt]

#include <windows.h>
#include <cstdio>
#include <cstdlib>

struct DbWinBuffer
{
    DWORD process_id;
    char  data[4096 - sizeof(DWORD)];
};

int main(int argc, char** argv)
{
    const DWORD seconds = (argc > 1) ? (DWORD) std::strtoul(argv[1], nullptr, 10) : 0;

    // Both events must exist before the buffer is published, or a writer can signal into nothing.
    HANDLE ready = CreateEventA(nullptr, FALSE, FALSE, "DBWIN_BUFFER_READY");
    HANDLE data  = CreateEventA(nullptr, FALSE, FALSE, "DBWIN_DATA_READY");

    if (!ready || !data) {
        std::printf("could not create the DBWIN events (%lu)\n", GetLastError());
        return 2;
    }

    HANDLE mapping = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE,
                                        0, sizeof(DbWinBuffer), "DBWIN_BUFFER");
    if (!mapping) {
        std::printf("could not create DBWIN_BUFFER (%lu) - is DebugView already running?\n",
                    GetLastError());
        return 2;
    }

    DbWinBuffer* buffer =
        (DbWinBuffer*) MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, sizeof(DbWinBuffer));

    if (!buffer) {
        std::printf("could not map DBWIN_BUFFER (%lu)\n", GetLastError());
        return 2;
    }

    std::printf("watching OutputDebugString%s\n",
                seconds ? " (timed)" : " - Ctrl+C to stop");
    std::fflush(stdout);

    const ULONGLONG deadline = seconds ? GetTickCount64() + seconds * 1000ULL : 0;
    DWORD last_pid = 0;
    unsigned long long lines = 0;

    for (;;) {
        // Telling the writer the buffer is free is what lets the next message through, so this has
        // to happen before every wait, not once at startup.
        SetEvent(ready);

        DWORD timeout = 500;
        if (deadline) {
            ULONGLONG now = GetTickCount64();
            if (now >= deadline) {
                break;
            }
            ULONGLONG left = deadline - now;
            if (left < timeout) {
                timeout = (DWORD) left;
            }
        }

        if (WaitForSingleObject(data, timeout) != WAIT_OBJECT_0) {
            continue;
        }

        if (buffer->process_id != last_pid) {
            std::printf("\n--- pid %lu ---\n", buffer->process_id);
            last_pid = buffer->process_id;
        }

        // The game emits partial lines constantly, so this deliberately does not add newlines of
        // its own - the output should read exactly as the game wrote it.
        std::fputs(buffer->data, stdout);
        std::fflush(stdout);
        lines++;
    }

    std::printf("\n\n%llu messages captured\n", lines);

    UnmapViewOfFile(buffer);
    CloseHandle(mapping);
    CloseHandle(data);
    CloseHandle(ready);
    return 0;
}
