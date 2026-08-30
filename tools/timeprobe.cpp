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

// Measure the granularity of timeGetTime, the clock MM2's frame timer is built on.
//
// datTimeManager::Update computes every frame's delta as the difference between two
// timeGetTime() calls, scaled by a seconds-per-tick constant. timeGetTime reports whole
// milliseconds, but it only ADVANCES at the system timer resolution - by default 15.6 ms. A
// process raises that to 1 ms by calling timeBeginPeriod(1), and MM2 never does: the string does
// not occur anywhere in midtown2.exe, and WINMM.dll is imported for mixer calls, timeGetTime and
// midiOutSetVolume only.
//
// If the granularity really is 15.6 ms then at any frame rate above ~64 fps most frames measure a
// delta of ZERO and every fourth or fifth measures 15 or 16 - so the simulation advances in
// lurches while the renderer runs perfectly smoothly. That is what a hitch that is not a frame
// rate drop feels like.
//
// This has to be MEASURED rather than reasoned about. Since Windows 10 2004 the timer resolution
// is per-process, but there is a documented compatibility exception for older executables, and
// whether a 1999-era binary falls under it is not something to guess at. So: sample the clock
// before and after raising the resolution, in a process built the same way the game is.
//
// Build: py tools/mktimeprobe.py
// Usage: build/timeprobe.exe

#include <windows.h>
#include <cstdio>

#pragma comment(lib, "winmm.lib")

// Watch the clock until it ticks over `samples` times, and report the steps it took.
//
// Sampling in a tight loop rather than sleeping is deliberate: Sleep has a granularity of its own
// and would measure that instead of the clock's.
static void measure(const char* label, int samples)
{
    DWORD steps[64] = {};
    int   n = 0;

    DWORD prev = timeGetTime();
    DWORD smallest = 0xFFFFFFFF, largest = 0;
    double total = 0;

    while (n < samples) {
        DWORD now = timeGetTime();
        if (now != prev) {
            DWORD step = now - prev;
            if (n < 64) steps[n] = step;
            if (step < smallest) smallest = step;
            if (step > largest)  largest = step;
            total += step;
            prev = now;
            ++n;
        }
    }

    std::printf("%s\n", label);
    std::printf("   step: min %lu ms, max %lu ms, mean %.2f ms\n",
                smallest, largest, total / n);
    std::printf("   first steps:");
    for (int i = 0; i < 12 && i < samples; ++i) std::printf(" %lu", steps[i]);
    std::printf("\n");

    // What this means for a game that reads the clock once a frame.
    //
    // A 60 fps frame is 16.7 ms. If the clock only moves in 15.6 ms jumps, a frame delta is
    // either 0 or 15/16 - never 16.7 - so the error on any single frame approaches 100%.
    const double frame_ms = 1000.0 / 60.0;
    std::printf("   at 60 fps a frame is %.1f ms, so the delta quantises to %.0f%% error\n\n",
                frame_ms, (total / n) / frame_ms * 100.0);
}

int main()
{
    // The resolution actually in force, which is a system-wide property this process may or may
    // not be subject to depending on how Windows classifies it.
    std::printf("timeGetTime granularity, as MM2's frame timer sees it\n");
    std::printf("=====================================================\n\n");

    LARGE_INTEGER qpf {};
    QueryPerformanceFrequency(&qpf);
    std::printf("QueryPerformanceFrequency: %lld Hz (%.3f us per count)\n\n",
                qpf.QuadPart, 1e6 / (double) qpf.QuadPart);

    measure("BEFORE timeBeginPeriod - what midtown2.exe gets today:", 40);

    timeBeginPeriod(1);
    measure("AFTER timeBeginPeriod(1) - what the fix would give it:", 40);
    timeEndPeriod(1);

    return 0;
}
