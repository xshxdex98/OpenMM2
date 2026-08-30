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

// Change MM2's draw distance and LOD transitions in the running game, to find values worth
// keeping before any of them are baked into a patch.
//
// What the game ships with:
//
//   lvlInstance::sm_ObjNoDrawThresh   300   beyond this an object is not drawn at all
//   lvlInstance::sm_ObjVLowThresh     200   very low detail past here
//   lvlInstance::sm_ObjLowThresh      100   low detail past here
//   lvlInstance::sm_ObjMedThresh       40   medium detail past here
//   gfxFarClip                        400   the camera's far plane
//
// The interesting part is that the first and the last disagree. The camera draws the world out to
// 400 but objects stop existing at 300, so there is a 100-unit band that is plainly in view and
// permanently empty - which is where things appear from. cityLevel::SetObjectDetail chooses among
// four presets and even its highest, detail 3, leaves the draw distance at 300; the graphics
// slider moves the LOD transitions outward but never the horizon. So no in-game setting can close
// that gap, and pop-in is not something the player is able to turn off.
//
// The right value to close it with is a judgement about how the city looks in motion, which is not
// a thing to guess at from a disassembly and then bake into a binary patch. Hence this: set them
// live, drive around, keep what looks right. Once a set of numbers earns its place it belongs in
// tools/patches.py as a permanent change, verified against retail like every other patch.
//
// Raising the far plane is not free. MM2 is a 16-bit-depth-buffer game from 1999 and pushing the
// far plane out costs depth precision, which shows up as z-fighting on coplanar surfaces long
// before it shows up as a frame rate cost. If distant road markings start to shimmer, that is the
// far clip and not the draw distance.
//
// The presets are re-applied on every level load and whenever the graphics options are touched, so
// this holds its values rather than writing them once.
//
// Build: py tools/mklodtune.py
// Usage: build/lodtune.exe                                  show the current values
//        build/lodtune.exe <draw> [vlow low med] [farclip]  set and hold them

#include <windows.h>
#include <tlhelp32.h>
#include <cstdio>
#include <cstdlib>

// Absolute addresses: midtown2.exe has IMAGE_FILE_RELOCS_STRIPPED, so it can only load at
// 0x400000 and these never move.
struct Knob
{
    const char* name;
    DWORD addr;
    float shipped;
};

static Knob KNOBS[] = {
    {"draw distance", 0x005C571C, 300.0f}, // lvlInstance::sm_ObjNoDrawThresh
    {"very low LOD ", 0x005C6658, 200.0f}, // lvlInstance::sm_ObjVLowThresh
    {"low LOD      ", 0x005C665C, 100.0f}, // lvlInstance::sm_ObjLowThresh
    {"medium LOD   ", 0x005C6660, 40.0f},  // lvlInstance::sm_ObjMedThresh
    {"far clip     ", 0x006B1990, 400.0f}, // gfxFarClip
};

static const int NKNOB = sizeof(KNOBS) / sizeof(KNOBS[0]);

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

static bool read_all(HANDLE proc, float* out)
{
    for (int i = 0; i < NKNOB; ++i) {
        SIZE_T got = 0;

        if (!ReadProcessMemory(proc, (LPCVOID) KNOBS[i].addr, &out[i], 4, &got))
            return false;
    }

    return true;
}

int main(int argc, char** argv)
{
    std::printf("waiting for the game...\n");
    std::fflush(stdout);

    DWORD pid = 0;

    while (!(pid = find_game()))
        Sleep(500);

    HANDLE proc = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION,
        FALSE, pid);

    if (!proc) {
        std::printf("could not open pid %lu (%lu)\n", pid, GetLastError());
        return 2;
    }

    float now[NKNOB] = {};

    if (!read_all(proc, now)) {
        std::printf("could not read the game's memory (%lu)\n", GetLastError());
        CloseHandle(proc);
        return 2;
    }

    std::printf("pid %lu\n\n", pid);
    std::printf("            knob     now   shipped\n");

    for (int i = 0; i < NKNOB; ++i)
        std::printf("   %s  %6.0f    %6.0f\n", KNOBS[i].name, now[i], KNOBS[i].shipped);

    if (argc < 2) {
        std::printf("\npass a draw distance to change them, e.g.\n");
        std::printf("   lodtune 900               scale everything to a 900-unit horizon\n");
        std::printf("   lodtune 900 600 300 120   set each threshold, far clip follows\n");
        std::printf("   lodtune 900 600 300 120 1000\n");
        CloseHandle(proc);
        return 0;
    }

    float want[NKNOB];
    const float draw = (float) std::atof(argv[1]);

    if (draw <= 0) {
        std::printf("draw distance must be positive\n");
        CloseHandle(proc);
        return 2;
    }

    if (argc >= 5) {
        want[0] = draw;
        want[1] = (float) std::atof(argv[2]);
        want[2] = (float) std::atof(argv[3]);
        want[3] = (float) std::atof(argv[4]);
        want[4] = (argc >= 6) ? (float) std::atof(argv[5]) : draw * 1.15f;
    } else {
        // Scale the shipped ratios rather than inventing a new curve. The shipped spacing
        // (300/200/100/40) is what the artists built the models against, so keeping its shape
        // means only the horizon moves, not the character of the transitions.
        const float k = draw / KNOBS[0].shipped;

        for (int i = 0; i < 4; ++i)
            want[i] = KNOBS[i].shipped * k;

        // The far plane has to sit past the draw distance or objects are clipped by the camera
        // before their own threshold ever applies, and nothing visible changes.
        want[4] = draw * 1.15f;
    }

    std::printf("\nholding:\n");

    for (int i = 0; i < NKNOB; ++i)
        std::printf("   %s  %6.0f\n", KNOBS[i].name, want[i]);

    std::printf("\nCtrl+C to stop holding (the game keeps whatever was last written)\n\n");
    std::fflush(stdout);

    long long writes = 0;

    for (;;) {
        float cur[NKNOB] = {};

        if (!read_all(proc, cur)) {
            std::printf("game exited after %lld corrections\n", writes);
            break;
        }

        // Only write what drifted. A level load or a visit to the graphics options calls
        // cityLevel::SetObjectDetail again and puts the presets back, and re-writing every
        // knob every tick would hide how often that happens.
        for (int i = 0; i < NKNOB; ++i) {
            if (cur[i] == want[i])
                continue;

            SIZE_T put = 0;

            if (WriteProcessMemory(proc, (LPVOID) KNOBS[i].addr, &want[i], 4, &put) && put == 4) {
                if (writes < 8 || cur[i] != 0.0f)
                    std::printf("   %s %6.0f -> %6.0f\n", KNOBS[i].name, cur[i], want[i]);

                std::fflush(stdout);
                writes++;
            }
        }

        Sleep(250);
    }

    CloseHandle(proc);
    return 0;
}
