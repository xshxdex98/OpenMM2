/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2
    Copyright (C) 2026 The OpenMM2 contributors

    Structure, conventions and the arts/core runtime follow Open1560 by Brick,
    an Open Source Re-Implementation of Midtown Madness 1. Copyright (C) 2020 Brick.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "mm1physics.h"

#include "data7/printer.h"

/*
    The setting, and its persistence.

    WHY WIN32 DIRECTLY AND NOT THE CRT. midtown2.exe statically linked its own 1999 CRT and the
    build deliberately does not open the standard library - no fopen, no printf, no std::. The
    established way round that in this tree is agigl/gltexdump.cpp: resolve the two or three
    KERNEL32 entry points actually needed through GetProcAddress and use them directly. This does
    the same, for a file that is at most a couple of dozen bytes.

    WHY A PLAIN TEXT FILE. It is written next to the game so it can be read, edited or deleted by
    hand, and a missing or malformed file means "off", which is retail behaviour. There is no state
    here that can break a save or a config the game itself owns.
*/

bool MM1PhysicsEnabled = false;
f32 MM1PhysicsBlend = 1.0f;

// There are no Windows headers on this include path, so the two entry points used to reach the
// rest of KERNEL32 are declared here - the same thing agigl/glddshim.cpp does, for the same reason.
extern "C" __declspec(dllimport) void* __stdcall LoadLibraryA(const char* name);
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);

namespace
{
    constexpr const char* kSettingsFile = "mm1physics.cfg";

    using PFNCreateFileA = void*(__stdcall*)(const char*, u32, u32, void*, u32, u32, void*);
    using PFNReadFile = i32(__stdcall*)(void*, void*, u32, u32*, void*);
    using PFNWriteFile = i32(__stdcall*)(void*, const void*, u32, u32*, void*);
    using PFNCloseHandle = i32(__stdcall*)(void*);

    PFNCreateFileA pCreateFileA = nullptr;
    PFNReadFile pReadFile = nullptr;
    PFNWriteFile pWriteFile = nullptr;
    PFNCloseHandle pCloseHandle = nullptr;

    bool LoadKernel()
    {
        if (pCreateFileA != nullptr)
            return true;

        void* k = LoadLibraryA("KERNEL32.dll");

        if (k == nullptr)
            return false;

        pCreateFileA = reinterpret_cast<PFNCreateFileA>(GetProcAddress(k, "CreateFileA"));
        pReadFile = reinterpret_cast<PFNReadFile>(GetProcAddress(k, "ReadFile"));
        pWriteFile = reinterpret_cast<PFNWriteFile>(GetProcAddress(k, "WriteFile"));
        pCloseHandle = reinterpret_cast<PFNCloseHandle>(GetProcAddress(k, "CloseHandle"));

        return pCreateFileA != nullptr && pReadFile != nullptr && pWriteFile != nullptr;
    }

    // A deliberately tiny float formatter/parser. The blend is one digit after the point, which is
    // all a slider needs, and hand-rolling it avoids pulling in any CRT formatting.
    i32 ParseTenths(const char* s, i32 len)
    {
        i32 whole = 0;
        i32 i = 0;

        while (i < len && s[i] >= '0' && s[i] <= '9')
            whole = whole * 10 + (s[i++] - '0');

        i32 tenths = 0;

        if (i < len && s[i] == '.' && i + 1 < len && s[i + 1] >= '0' && s[i + 1] <= '9')
            tenths = s[i + 1] - '0';

        return whole * 10 + tenths;
    }
} // namespace

void MM1PhysicsLoad()
{
    MM1PhysicsEnabled = false;
    MM1PhysicsBlend = 1.0f;

    if (!LoadKernel())
        return;

    // GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING
    void* h = pCreateFileA(kSettingsFile, 0x80000000u, 1u, nullptr, 3u, 0x80u, nullptr);

    if (h == nullptr || h == reinterpret_cast<void*>(-1))
        return;                     // absent is not an error: it means vanilla

    char buf[64] = {};
    u32 got = 0;
    pReadFile(h, buf, sizeof(buf) - 1, &got, nullptr);
    pCloseHandle(h);

    for (u32 i = 0; i + 1 < got; ++i)
    {
        if (buf[i] == 'o' && buf[i + 1] == 'n')
            MM1PhysicsEnabled = true;

        if (buf[i] == '=' && i + 1 < got)
        {
            i32 t = ParseTenths(buf + i + 1, static_cast<i32>(got - i - 1));

            if (t > 0 && t <= 10)
                MM1PhysicsBlend = static_cast<f32>(t) * 0.1f;
        }
    }

    Displayf("MM1 physics: %s (blend %.1f)", MM1PhysicsEnabled ? "ON" : "off", MM1PhysicsBlend);
}

void MM1PhysicsSave()
{
    if (!LoadKernel())
        return;

    // GENERIC_WRITE, no sharing, CREATE_ALWAYS
    void* h = pCreateFileA(kSettingsFile, 0x40000000u, 0u, nullptr, 2u, 0x80u, nullptr);

    if (h == nullptr || h == reinterpret_cast<void*>(-1))
        return;

    char out[48];
    i32 n = 0;

    const char* state = MM1PhysicsEnabled ? "on" : "off";

    while (*state)
        out[n++] = *state++;

    out[n++] = ' ';
    out[n++] = '=';

    i32 tenths = static_cast<i32>(MM1PhysicsBlend * 10.0f + 0.5f);

    if (tenths < 0)
        tenths = 0;

    if (tenths > 10)
        tenths = 10;

    out[n++] = static_cast<char>('0' + tenths / 10);
    out[n++] = '.';
    out[n++] = static_cast<char>('0' + tenths % 10);
    out[n++] = '\n';

    u32 wrote = 0;
    pWriteFile(h, out, static_cast<u32>(n), &wrote, nullptr);
    pCloseHandle(h);
}

define_dummy_symbol(veh_mm1physics);
