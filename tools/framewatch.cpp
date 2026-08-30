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

// Measure MM2's frame pacing from outside the process, to find out what a "hitch" actually is.
//
// A hitch is not the same defect as a low frame rate and needs different evidence: the
// DISTRIBUTION of frame times, not their average. A game averaging 200 fps with one 80 ms frame
// every few seconds feels far worse than a steady 60, and no average will ever show it.
//
// datTimeManager keeps both numbers this needs. ActualSeconds is how long the frame really took;
// Seconds is how far the simulation was actually advanced. Update derives the second from the
// first:
//
//     if (SampleStep == 0)        Seconds = ActualSeconds;   // real time
//     else if (raw >= SampleStep) Seconds = SampleStep;      // capped
//     else                        Seconds = ActualSeconds;
//     Seconds = clamp(Seconds, ClampMin, ClampMax);          // 0.1 ms .. 100 ms
//
// SampleStep's initialiser in .data is 1/60, which looks like the game caps its timestep and runs
// in slow motion below 60 fps. It does not: MainPhase calls datTimeManager::RealTime(0.0f), which
// sets SampleStep to zero, and datTimeManager::FixedFrame is never called at all. A .data
// initialiser is not a runtime value, and mistaking one for the other invents a bug that is not
// there.
//
// So rather than assume which branch runs, this compares the two globals directly. If Seconds is
// below ActualSeconds the world advanced by less than really elapsed, whatever the reason - today
// that can only be the 100 ms ClampMax, but the comparison stays honest if the mode ever changes.
//
// This reads the globals out of the running game with ReadProcessMemory and never attaches as a
// debugger. That distinction is not academic: tools/faultwatch.cpp owns the process it inspects and
// was terminating the game on first-chance exceptions MM2 handles routinely, ending play sessions
// that were going fine. Nothing here can affect the game - if this program is killed the game does
// not notice.
//
// FrameCount is polled rather than sampled on a timer, so each frame is counted once no matter how
// fast the game runs; frames missed because the poll was too slow are reported rather than hidden.
//
// Build: py tools/mkframewatch.py
// Usage: build/framewatch.exe [seconds]

#include <windows.h>
#include <tlhelp32.h>
#include <cstdio>
#include <cstdlib>

// Absolute addresses, not RVAs: midtown2.exe has IMAGE_FILE_RELOCS_STRIPPED, so the image can only
// ever load at 0x400000 and these are fixed for the life of the binary.
static const DWORD ADDR_RAW_DELTA = 0x005CE824;  // seconds of real time the last frame took
static const DWORD ADDR_TIMESTEP = 0x005CE820;   // seconds the simulation was actually advanced
static const DWORD ADDR_FRAMECOUNT = 0x006A3C50; // datTimeManager::FrameCount
static const DWORD ADDR_ELAPSED = 0x006A3C40;    // datTimeManager::ElapsedTime

// The spike threshold. Not a frame rate target: a frame is worth calling a hitch when it is far
// out of line with its neighbours, and 16.7 ms is simply the conventional place to start counting.
static const double SPIKE_MS = 16.7;

static DWORD find_game()
{
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE)
        return 0;

    PROCESSENTRY32 pe {};
    pe.dwSize = sizeof(pe);
    DWORD found = 0;

    for (BOOL ok = Process32First(snap, &pe); ok; ok = Process32Next(snap, &pe)) {
        if (_stricmp(pe.szExeFile, "OpenMM2.exe") == 0 ||
            _stricmp(pe.szExeFile, "midtown2.exe") == 0) {
            found = pe.th32ProcessID;
            break;
        }
    }

    CloseHandle(snap);
    return found;
}

int main(int argc, char** argv)
{
    const DWORD seconds = (argc > 1) ? (DWORD) std::strtoul(argv[1], nullptr, 10) : 0;

    std::printf("waiting for the game...\n");
    std::fflush(stdout);

    DWORD pid = 0;
    while (!(pid = find_game()))
        Sleep(500);

    HANDLE proc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

    if (!proc) {
        std::printf("could not open pid %lu (%lu)\n", pid, GetLastError());
        return 2;
    }

    std::printf("attached to pid %lu (read-only)\n\n", pid);
    std::fflush(stdout);

    // Buckets in milliseconds, placed around 16.7 ms rather than spread evenly. A hitch is a
    // frame far out of line with its neighbours, so what matters is how heavy the tail is, and an
    // even spread would put almost every frame in one bucket and tell us nothing.
    static const double edges[] = {4, 8, 12, 16.67, 20, 25, 33.3, 50, 100, 1e9};

    static const char* names[] = {"      <4ms  (>250fps)", "     4-8ms  (125-250)",
                                  "     8-12ms  (83-125)", "   12-16.7ms  (60-83)",
                                  "   16.7-20ms  (50-60)", "     20-25ms  (40-50)",
                                  "     25-33ms  (30-40)", "     33-50ms  (20-30)",
                                  "    50-100ms  (10-20)", "     >100ms  (<10fps)"};

    const int NB = 10;
    long long bucket[NB] = {};

    DWORD last_frame = 0;
    long long frames = 0, dilated = 0, spikes = 0, missed = 0;
    double real_total = 0, sim_total = 0, worst = 0, worst_at = 0;

    const ULONGLONG deadline = seconds ? GetTickCount64() + seconds * 1000ULL : 0;
    ULONGLONG last_report = GetTickCount64();

    for (;;) {
        if (deadline && GetTickCount64() >= deadline)
            break;

        DWORD frame = 0;
        SIZE_T got = 0;

        if (!ReadProcessMemory(proc, (LPCVOID) ADDR_FRAMECOUNT, &frame, 4, &got)) {
            std::printf("\ngame exited\n");
            break;
        }

        if (frame != last_frame) {
            if (last_frame && frame > last_frame + 1)
                missed += frame - last_frame - 1;

            float raw = 0, step = 0, elapsed = 0;
            ReadProcessMemory(proc, (LPCVOID) ADDR_RAW_DELTA, &raw, 4, &got);
            ReadProcessMemory(proc, (LPCVOID) ADDR_TIMESTEP, &step, 4, &got);
            ReadProcessMemory(proc, (LPCVOID) ADDR_ELAPSED, &elapsed, 4, &got);

            // A load screen produces one enormous delta that is not a hitch; anything past a
            // second is the game restarting its clock, not a frame that took that long.
            if (raw > 0.0f && raw < 1.0f) {
                double ms = raw * 1000.0;

                for (int i = 0; i < NB; ++i) {
                    if (ms < edges[i]) {
                        bucket[i]++;
                        break;
                    }
                }

                frames++;
                real_total += raw;
                sim_total += step;

                // The world advanced by less than really elapsed - the game gave up real time
                // rather than take one large simulation step. Read from the game's own two
                // numbers instead of assuming which branch of Update ran.
                if (step < raw * 0.999f)
                    dilated++;

                if (ms > SPIKE_MS)
                    spikes++;

                if (ms > worst) {
                    worst = ms;
                    worst_at = elapsed;
                }
            }

            last_frame = frame;
        }

        // A tight spin would read the same frame thousands of times and burn a core the game
        // wants. One millisecond is well under any plausible frame time.
        Sleep(1);

        ULONGLONG now = GetTickCount64();

        if (now - last_report >= 15000 && frames) {
            std::printf("  ... %lld frames, %.1f%% in slow motion, worst %.1f ms\n", frames,
                100.0 * dilated / frames, worst);
            std::fflush(stdout);
            last_report = now;
        }
    }

    if (!frames) {
        std::printf("no frames seen - was the game in a menu the whole time?\n");
        CloseHandle(proc);
        return 1;
    }

    std::printf("\n=== frame pacing over %lld frames ===\n\n", frames);

    for (int i = 0; i < NB; ++i) {
        if (!bucket[i])
            continue;

        double pct = 100.0 * bucket[i] / frames;
        std::printf("%s %8lld  %5.2f%%  ", names[i], bucket[i], pct);

        for (int k = 0; k < (int) (pct / 2 + 0.5); ++k)
            std::putchar('#');

        std::putchar('\n');
    }

    // The headline number. Every frame slower than 1/60 s advances the simulation by exactly 1/60,
    // so the difference between real elapsed time and simulated time is time the world lost.
    const double lost = real_total - sim_total;

    std::printf("\nframes above 16.7 ms: %lld (%.2f%%) - each runs the world in slow motion\n",
        dilated, 100.0 * dilated / frames);
    std::printf("real %.1f s, simulated %.1f s -> the world lost %.2f s (%.2f%%)\n", real_total,
        sim_total, lost, 100.0 * lost / (real_total ? real_total : 1));
    std::printf("worst frame: %.1f ms (at %.0f s into the level)\n", worst, worst_at);

    if (missed)
        std::printf("(%lld frames went by faster than this could poll)\n", missed);

    CloseHandle(proc);
    return 0;
}
