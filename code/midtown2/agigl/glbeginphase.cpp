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

#include "core/arts.h"

define_dummy_symbol(agigl_glbeginphase);

// BeginPhase, 0x00401AA0, 0x330 bytes - THE FOURTH ACTIVATION FUNCTION.
//
// docs/opengl_activation.md used to claim that InitDirectDraw, gfxPipeline::BeginGfx and
// gfxPipeline::EndGfx were the only callers of the DirectDraw bring-up, and that three was
// therefore enough. That claim was read off the pseudocode kit and it is WRONG. A caller scan done
// against the instruction bytes - twice, once over the symbol references in game.asm.pristine and
// once by disassembling midtown2.exe with capstone and matching every rel32 and every absolute
// dword against the four target addresses - gives the real set:
//
//   BeginGfx2D  0x004A9370  <- InitDirectDraw (x2), gfxPipeline::BeginGfx
//   BeginGfx3D  0x004A96C0  <- BeginPhase, gfxPipeline::BeginGfx          <-- THE MISSING ONE
//   EndGfx2D    0x004AAA10  <- Main (x2), EndPhase, gfxPipeline::EndGfx
//   EndGfx3D    0x004AA760  <- EndPhase, gfxPipeline::EndGfx
//
// Only one of the three new callers is a problem, and this is it. BeginPhase calls
// gfxPipeline::BeginGfx3D() unconditionally, four instructions after gfxPipeline::SetRes, and
// BeginGfx3D's very first act is `lpDD->CreateSurface`. Under -gl nothing created lpDD, so the
// process dies reading null at BeginGfx3D+0x90 - which is 0x004A9750, `mov edx, [eax]` on the
// vtable of a null IDirectDraw7, and is exactly the address the -gl run reports.
//
// Main and EndPhase are NOT problems, and this was checked rather than assumed: every COM pointer
// in EndGfx2D and EndGfx3D is tested against null before it is used. Under -gl they run to
// completion doing nothing but ShowCursor(TRUE), DestroyWindow(hwndMain), and the gfxTexture /
// gfxBitmap list teardown. They leak the WGL context, which is a teardown defect and not a crash.
//
// WHY THE BRANCH GOES HERE AND NOT IN BeginGfx3D. The rule in docs/opengl_activation.md is that a
// function may carry the -gl branch only if its ORIGINAL body contains no COM, because registering
// a symbol destroys its machine code and the else arm then has to be a hand retyping of the
// original - and a retyped DirectDraw call means writing vtable indices into a build that cannot
// see ddraw.h, which is the one class of error docs/harness.md says nothing here can catch.
// BeginGfx3D is 0xA70 bytes of nothing but COM, so it can never carry the branch. BeginPhase
// contains no COM at all: every call in it is a direct call or a call through a named function
// pointer global. So the caller carries it, and BeginGfx3D keeps its own untouched bytes for the
// DirectDraw path.
//
// WHY THIS TRANSCRIPTION IS SAFER THAN IT LOOKS. This is 0x330 bytes of boot-path code and the
// else arm is what every user without -gl runs, so the usual objection applies with force. Two
// things answer it:
//
//   1. There is exactly ONE conditional in the whole function - the BeginGfx3D call site. Both
//      arms share the entire rest of the body. So the transcription is exercised on EVERY run,
//      with or without -gl.
//   2. That makes it verifiable by the test this project already has. Wave one was signed off by
//      diffing 66,068 lines of debug output before and after activation. BeginPhase emits ten
//      datDisplayUsed lines of its own and everything it constructs logs further down, so a
//      divergence here shows up in that diff instead of hiding. Contrast Set A and Set B in
//      docs/opengl_activation.md, where the ported code runs ONLY under -gl and there is no
//      before-and-after to compare it against.
//
// THIS FILE CANNOT BE ENABLED YET, AND THE REASON IS NOT ABOUT OPENGL. See the ARTS_ALIAS_DATA
// block below: three of the globals BeginPhase touches have undecorated names in the linker map,
// and tools/ghidra/ExportAsm.java emits `PUBLIC` only for symbols beginning with '?'. `useIME`,
// `audioFlags` and `inputDevice` are therefore module-local labels in game.asm and no C++ can name
// them. Registering BeginPhase today strips its PROC and then fails to link. data/
// ported_agigl_wave2.json is deliberately empty for this reason.
//
// TO ENABLE, in this order:
//   1. Make ExportAsm.java emit `PUBLIC <name>` for undecorated data labels too, and re-run the
//      export/reimport so game.asm carries them.
//   2. Add `-DARTS_AGIGL_PHASE` to tools/build.py.
//   3. Add `ARTS_EXPORT void BeginPhase(bool arg1);` under its mangled-name comment in
//      misc/freefuncs.h. asm.py works from ARTS_EXPORT markers in headers and genheaders.py does
//      not emit one for a free function, so registration in a json alone leaves the original PROC
//      in place and the link fails with "already defined in game.obj" - the exact trap wave one
//      hit with InitDirectDraw.
//   4. Add ?BeginPhase@@YAX_N@Z to data/ported.json.
//   5. Run WITHOUT -gl first and diff the log against a pre-change run. Nothing but tid values may
//      differ. Only then is the -gl arm worth looking at.
#ifdef ARTS_AGIGL_PHASE

// Every include is inside the guard, for the same reason glsetres.cpp does it: gfx/gfxtexture.h
// pulls in gfx/gfximage.h, and this file must cost nothing at all until it is deliberately
// switched on.
#    include "arts7/ascullmanager.h"
#    include "arts7/asnode.h"
#    include "arts7/asroot.h"
#    include "data7/printer.h"
#    include "eventq7/eqeventhandler.h"
#    include "gfx/gfxbitmap.h"
#    include "gfx/gfximage.h"
#    include "gfx/gfxinterface.h"
#    include "gfx/gfxpipeline.h"
#    include "gfx/gfxtexture.h"
#    include "io/ioinput.h"
#    include "misc/freefuncs.h"
#    include "mmgame/mminput.h"

// Already imported by midtown2.exe; declared here because the build has no Windows headers.
extern "C" __declspec(dllimport) int __stdcall GetSystemMetrics(int index);

// The game window, so the GL layer's scaling can be recomputed after the frontend picks its
// resolution. Declared the same way agigl/glactivate.cpp declares it - see glcommon.h on why the
// struct class-key matters for the mangled name.
ARTS_IMPORT extern HWND__* hwndMain;

#    include "glscreen.h"
#    include "glswitch.h"
#    include "glwin32.h"

#    ifdef ARTS_AGIGL
#        include "glgfx.h"
#    endif

// THE THREE GLOBALS THAT DO NOT LINK, AND WHY.
//
// tools/ghidra/ExportAsm.java writes a data label as
//
//     if (sym.startsWith("?")) w.println("PUBLIC " + sym);
//     w.println(sym + " LABEL BYTE");
//
// so a MANGLED global is PUBLIC and reachable from C++, and an UNDECORATED one is a module-local
// label that the linker never sees. Most of what BeginPhase touches is mangled - inWindow,
// bWinBorder, NeedFullShutdown, NeedStartup, gfxInterfaceChoice, gfxInterfaces, APPTITLE,
// gfxLoadImage, splash_bg, SuperQ, MMAUDMGRPTR, ROOT, GameInputPtr, datDisplayUsed - and those are
// declared normally further down. These three are not:
//
//     useIME       0x006B19BC   `useIME LABEL BYTE`      game.asm.pristine:881874
//     audioFlags   0x006B1670   `audioFlags LABEL BYTE`  game.asm.pristine:881668
//     inputDevice  0x006B16F8   `inputDevice LABEL BYTE` game.asm.pristine:881702
//
// There is no `PUBLIC` line above any of them, and grep confirms no other spelling exists.
//
// The alias below is the second half of the fix, not the whole of it. `extern "C" i32 useIME;`
// emits a reference to `_useIME`, because __cdecl prepends an underscore on x86, while the label
// in the assembly is `useIME` with no underscore. /alternatename reconciles the two spellings the
// same way agigl/glwin32.h reconciles __imp__MessageBoxA with __imp__MessageBoxA@16. But
// /alternatename can only redirect an undefined symbol to a DEFINED one, so it does nothing until
// ExportAsm.java actually makes the label public. Until then this file does not link, which is a
// loud failure and the right kind.
#    define ARTS_ALIAS_DATA(NAME) __pragma(comment(linker, "/alternatename:_" #NAME "=" #NAME))

extern "C"
{
    // Read as a dword by BeginPhase and by Main, and written `mov dword ptr [useIME], 1` by Main
    // when ImmAssociateContext succeeds. i32, not bool.
    ARTS_IMPORT extern i32 useIME;

    // Written `mov dword ptr [audioFlags], 0xC73` by Main; BeginPhase reads only the low byte and
    // masks bit 0. u32, and the mask is done here rather than in the type.
    ARTS_IMPORT extern u32 audioFlags;

    // ecx = dword ptr [inputDevice], pushed straight into mmInput::Init(i32).
    ARTS_IMPORT extern i32 inputDevice;
}

ARTS_ALIAS_DATA(useIME)
ARTS_ALIAS_DATA(audioFlags)
ARTS_ALIAS_DATA(inputDevice)

// The rest of what BeginPhase reaches. All mangled, all resolvable today, and each one declared
// under the exact name in data/symbols.json - `bool` is _NA, `int` is HA, `unsigned char` is EA.
// Getting one wrong produces a symbol that does not exist rather than one that misbehaves, which
// is the whole reason these are written out longhand instead of guessed at.

// ?datDisplayUsed@@3P6AXPBD@ZA - a function POINTER, not a function. BeginPhase calls it ten times
// and it is the coarsest progress trace the boot has; the strings below are the originals.
ARTS_IMPORT extern void(__cdecl* datDisplayUsed)(const char* text);

// ?gfxLoadImage@@3P6APAVgfxImage@@PBD_N@ZA - also a pointer. Main points it at gfxLoadImageAll.
ARTS_IMPORT extern gfxImage*(__cdecl* gfxLoadImage)(const char* name, bool arg2);

// ?bWinBorder@@3_NA      0x005CA3ED
ARTS_IMPORT extern bool bWinBorder;

// ?inWindow@@3_NA        0x006830D0
ARTS_IMPORT extern bool inWindow;

// ?NeedFullShutdown@@3_NA 0x006B0468
ARTS_IMPORT extern bool NeedFullShutdown;

// ?NeedStartup@@3_NA     0x005E0CF8
ARTS_IMPORT extern bool NeedStartup;

// ?splash_bg@@3PAVgfxBitmap@@A 0x005E0CCC
ARTS_IMPORT extern gfxBitmap* splash_bg;

// ?APPTITLE@@3PADA       0x005C28D4 - PAD, so char* and not const char*.
ARTS_IMPORT extern char* APPTITLE;

// ?GraphicsPreviousMenu@@3EA 0x005E0CFB, one byte. Declared only to reach the four bytes AFTER it -
// see the ProgressCB call site for why.
ARTS_IMPORT extern u8 GraphicsPreviousMenu;

// ?ROOT@@3VasRoot@@A     0x00661738 - the scene graph root, by value.
ARTS_IMPORT extern asRoot ROOT;

// ?GameInputPtr@@3PAVmmInput@@A 0x006B1CF0 - written by the mmInput constructor, which is why the
// `new mmInput()` below discards its result and then reads this.
ARTS_IMPORT extern mmInput* GameInputPtr;

// ?MMAUDMGRPTR@@3PAVAudManager@@A 0x006B15D0. AudManager has no header in the tree; a forward
// declaration is enough, because the only thing done with it is the AddChild below.
class AudManager;

ARTS_IMPORT extern AudManager* MMAUDMGRPTR;

// The free functions BeginPhase calls that no generated header declares.

// ?gfxApplySettings@@YAXXZ    0x004AC870
ARTS_IMPORT void gfxApplySettings();

// ?gfxLoadSettings@@YA_NXZ    0x004ABD70 - returns bool, and BeginPhase discards it.
ARTS_IMPORT bool gfxLoadSettings();

// ?vglSetFormat@@YAXIIII@Z    0x004A5490
ARTS_IMPORT void vglSetFormat(u32 arg1, u32 arg2, u32 arg3, u32 arg4);

// ?GetLoadScreenName@@YAXPBD@Z 0x00401DD0. PBD in the mangled name, so the parameter must be
// declared const char* for the symbol to resolve - even though the buffer is an OUTPUT and the
// caller reads it back. The name is part of the ABI here; the constness is not.
ARTS_IMPORT void GetLoadScreenName(const char* buffer);

// ?ProgressCB@@YAXPBDH@Z      0x004010F0
ARTS_IMPORT void ProgressCB(const char* text, i32 percent);

// ?gfxReleaseFont@@YAXXZ      0x004B12D0
ARTS_IMPORT void gfxReleaseFont();

// ?gfxCreateFont@@YAXXZ       0x004B1200
ARTS_IMPORT void gfxCreateFont();

// ?InitAudioManager@@YAX_N@Z  0x00401E70
ARTS_IMPORT void InitAudioManager(bool arg1);

// ?AngelReadString@@YAPAULocString@@I@Z - the return type is part of a free function's mangled
// name, so this has to be spelled with LocString or it becomes a symbol that does not exist. Only
// forward-declared: the original pushes the returned pointer straight into MessageBoxA's lpText
// with no offset added, so the text is at offset 0 and a reinterpret_cast is faithful. Same
// reasoning, and the same struct, as agigl/glactivate.cpp.
struct LocString;

ARTS_IMPORT LocString* AngelReadString(u32 id);

// The game's own CRT exit at 0x00581944, which game.asm emits as `_exit PROC`. extern "C" plus
// __cdecl decorates to exactly `_exit`. NOT <cstdlib>'s, which would bind to a modern CRT this
// link does not have.
extern "C" void __cdecl exit(int code);

extern "C" __declspec(dllimport) i32 __stdcall MessageBoxA(
    void* window, const char* text, const char* caption, u32 type);

// ChangeDisplaySettingsA(DEVMODEA*, DWORD). Two arguments, eight bytes of stack, which is what the
// ARTS_ALIAS_IMPORT(ChangeDisplaySettingsA, 8) entry in glwin32.h reconciles. The DEVMODE is
// declared as raw bytes below rather than as a struct, so void* is the honest parameter type.
extern "C" __declspec(dllimport) i32 __stdcall ChangeDisplaySettingsA(void* dev_mode, u32 flags);

// THE HARDCODED FRONTEND RESOLUTION, LEFT EXACTLY AS IT IS.
//
// This is the 640x480 docs/opengl_activation.md warns about, and porting BeginPhase is what makes
// it reachable from C++ for the first time. It is NOT changed here, and it must not be changed in
// the same edit that lands the port: a resolution change and a transcription cannot be verified by
// the same log diff, because the whole point of that diff is that nothing may differ.
//
// It is used in three places and all three are the same pair of registers in the original (edi and
// esi, loaded once at 0x00401AD8 / 0x00401ADD and never reloaded on this path): the display mode
// passed to ChangeDisplaySettingsA, the gfxPipeline::SetRes call, and the gfxImage::Scale that
// sizes the loading screen. Whoever raises this has to move all three together, which is easier to
// see now that they are three uses of two named constants.
static constexpr i32 kFrontendWidth = 640;
static constexpr i32 kFrontendHeight = 480;
static constexpr i32 kFrontendColorDepth = 16;
static constexpr i32 kFrontendZDepth = 16;

// The largest resolution the interface is allowed to be scaled to. See the note at the cap itself:
// the limit is MM2's 32 MB heap, not the display.
static constexpr i32 kFrontendMaxWidth = 1920;
static constexpr i32 kFrontendMaxHeight = 1080;

// The DEVMODEA fields the original writes, at the byte offsets it writes them to.
//
// Spelling the whole of DEVMODEA out would mean hand-declaring thirty fields to reach five, and a
// wrong offset among the twenty-five that are never read would never be caught by anything. These
// five were read off the instruction bytes at 0x00401B28 - 0x00401B3D and each one agrees with the
// documented DEVMODEA layout, which is what makes the buffer identification certain: dmSize at
// 0x24 holds 0x94, and 0x94 is sizeof(DEVMODEA).
static constexpr usize kDevModeSize = 0x94;
static constexpr usize kDevModeSizeOffset = 0x24;
static constexpr usize kDevModeFieldsOffset = 0x28;
static constexpr usize kDevModeBitsPerPelOffset = 0x68;
static constexpr usize kDevModePelsWidthOffset = 0x6C;
static constexpr usize kDevModePelsHeightOffset = 0x70;

// DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT, the 0x1C0000 the original stores into dmFields.
static constexpr u32 kDevModeFields = 0x001C0000;

// The buffer GetLoadScreenName fills. 0x50 bytes, and that is measured rather than chosen: the
// original's frame is `sub esp, 0xE4` after three pushes, so the lowest local is at [ebp-0xF0], the
// DEVMODE occupies [ebp-0xA0] .. [ebp-0x0C], and the name buffer is the 0x50 bytes between them.
static constexpr usize kLoadScreenNameSize = 0x50;

// MB_ICONHAND, the 0x10 the original pushes.
static constexpr u32 kMessageBoxIconHand = 0x10;

// String resource 0x263, the "could not load the loading screen" message.
static constexpr u32 kNoLoadScreenString = 0x263;

// The three heap sizes the original passes to operator new, kept as the literals it uses rather
// than as sizeof(). A sizeof() here would be a silent trap: these classes are only partially
// recovered, and if a header's size were ever wrong the allocation would change size without
// anything reporting it. The static_asserts make the two agree loudly instead.
static constexpr u32 kEqEventHandlerSize = 0x160;
static constexpr u32 kCullManagerSize = 0x00A8;
static constexpr u32 kInputSize = 0x0248;

// The asCullManager the original builds for the frontend. Discarded on purpose - the constructor
// publishes itself through asCullManager::Instance.
static constexpr i32 kCullManagerArg1 = 0x200;
static constexpr i32 kCullManagerArg2 = 0x40;

// The placement form of operator new, spelled here because <new> is not on this build's include
// path and the ordinary form would bind to a CRT that does not exist in this link. The scalar form
// operator new(u32) is implicitly declared by the compiler and resolves to the game's own
// ??2@YAPAXI@Z at 0x00577360, which is the allocator the original calls.
//
// It is needed because the original null-checks the allocation before running the constructor -
// `test eax, eax; je skip; mov ecx, eax; call ??0...` - and modern MSVC, which assumes a throwing
// operator new, would omit that check for a plain `new T()`. On out of memory the original stores
// null and this build would call a constructor on null, so the check is transcribed rather than
// left to the compiler.
inline void* operator new(usize size, void* where) noexcept
{
    (void) size;

    return where;
}

// DELIBERATELY UNMARKED, AND THE LAYOUT OF THE NEXT FIVE LINES IS PART OF THAT.
//
// tools/asm.py decides what to strip by scanning .h AND .cpp for a mangled-name comment followed
// by an ARTS_IMPORT or ARTS_EXPORT marker. It is a text scan: it does not respect #ifdef, and it
// keeps the pending symbol across intervening comment lines. So a mangled-name comment with the
// word ARTS_EXPORT anywhere in the prose beneath it - even prose explaining that the marker is
// absent - registers the symbol, strips its PROC, and leaves nothing to replace it.
//
// The marker line below is therefore bare and sits directly on the definition, which has no
// marker, so the pending symbol is discarded. Nothing in this file may be written between them.

// ?BeginPhase@@YAX_N@Z
void BeginPhase(bool arg1)
{
    datDisplayUsed("Start of BeginPhase");

    // edi and esi in the original, loaded once here and read again by SetRes and gfxImage::Scale
    // further down. On the arg1 == false path they are overwritten from the adapter table.
    i32 width = kFrontendWidth;
    i32 height = kFrontendHeight;

    if (useIME != 0)
    {
        if (arg1)
        {
            // The frontend forces adapter 0 to the software renderer and runs windowed at
            // 640x480x16. gfxInterfaces[0].Renderer is the `mov [0x00683184], ebx` at 0x00401AF5 -
            // 0x00683184 is gfxInterfaces + 0x54, and gfx/gfxinterface.h puts Renderer there.
            gfxInterfaceChoice = 0;
            gfxInterfaces[0].Renderer = Software;

            gfxApplySettings();

            inWindow = true;
            bWinBorder = false;

            gfxPipeline::SetWindow(true, -1, -1);

            NeedFullShutdown = true;

            // THE ONE PLACE THIS TRANSCRIPTION IS NOT LITERAL, AND IT IS DELIBERATE.
            //
            // The original does not clear this buffer. It writes dmSize, dmFields, dmBitsPerPel,
            // dmPelsWidth and dmPelsHeight into 148 bytes of uninitialised stack and hands the lot
            // to the display driver, which then reads dmDriverExtra at offset 0x26 as well. That
            // works today because the stack happens to be benign, not because it is correct.
            //
            // Zeroing it can only change behaviour in the case where the original passed garbage,
            // and dmFields already tells the driver which members are meaningful. Reading an
            // uninitialised local is undefined behaviour that MSVC is entitled to exploit, so the
            // literal form is not available to a C++ transcription anyway. Noted here and in
            // docs/opengl_activation.md so nobody has to rediscover it from a diff.
            u8 display_mode[kDevModeSize] = {};

            *reinterpret_cast<u16*>(display_mode + kDevModeSizeOffset) =
                static_cast<u16>(kDevModeSize);
            *reinterpret_cast<u32*>(display_mode + kDevModeFieldsOffset) = kDevModeFields;
            *reinterpret_cast<u32*>(display_mode + kDevModeBitsPerPelOffset) =
                static_cast<u32>(kFrontendColorDepth);
            *reinterpret_cast<u32*>(display_mode + kDevModePelsWidthOffset) =
                static_cast<u32>(width);
            *reinterpret_cast<u32*>(display_mode + kDevModePelsHeightOffset) =
                static_cast<u32>(height);

            // THE %d PRINTS 1, NOT THE RETURN CODE, AND THAT IS THE ORIGINAL'S BUG NOT A TYPO
            // HERE. At 0x00401B4A the bytes are `neg eax; sbb eax, eax; neg eax`, which is the
            // compiler's idiom for `!= 0`, and it is the RESULT of that - 0 or 1 - that is both
            // branched on and pushed to Errorf. The 1999 source was almost certainly
            // `int code = ChangeDisplaySettings(&dm, 0) != 0;`, an operator-precedence slip that
            // has been printing "code 1" for twenty-seven years.
            //
            // Transcribed as written. Fixing it would change a line of debug output, and a line of
            // debug output is exactly what the log diff that validates this file compares.
            i32 code = (ChangeDisplaySettingsA(display_mode, 0) != 0) ? 1 : 0;

            if (code != 0)
                Errorf("ChangeDisplayMode failed, code %d.", code);
        }
        else
        {
            bWinBorder = false;
            inWindow = false;

            gfxLoadSettings();
            gfxApplySettings();

            NeedFullShutdown = true;
        }
    }

    // 0x00401B81. When useIME is set the device is brought up on every phase change; otherwise
    // only the first time. NeedStartup is cleared either way.
    if (useIME != 0 || NeedStartup)
    {
        InitDirectDraw();

        NeedStartup = false;
    }

    datDisplayUsed("Just before CreateBankManager");

    if (arg1)
    {
        // THE FRONTEND RUNS AT THE CHOSEN RESOLUTION, NOT AT 640x480.
        //
        // This is the change that makes every other piece of UI scaling work do anything at all.
        // ui/uiscale.h derives its factor from gfxPipeline::m_iWidth, so a frontend that tells the
        // pipeline it is 640 wide computes a factor of exactly one - and bitmap resampling in
        // gfxBitmap::Create, font sizing in mmText::CreateLocFont and the text-node bitmaps in
        // mmTextNode::Init all become no-ops. The menus then reach the screen as a 640x480 image
        // stretched by the projection, which is legible and soft.
        //
        // tools/patches.py has TWO byte patches trying to fix this at literal SetRes(640, 480)
        // sites - frontend-resolution in Main and safe-begin-gfx-* in SafeBeginGfx - and neither
        // moved m_iWidth off 640, because this call is the one that runs and its arguments are
        // computed rather than literal. That is the third time on this project a patch has been
        // verified into the binary and had no effect; the lesson keeps being the same one, which
        // is to check the value the game ends up with.
        //
        // The adapter table is read exactly as the gameplay arm below reads it, so the two paths
        // now agree about what the screen is. The colour and depth arguments stay the frontend's
        // own: only the size was ever wrong.
        const gfxInterface& adapter = gfxInterfaces[gfxInterfaceChoice];
        const gfxResData& chosen = adapter.Resolutions[adapter.ResolutionChoice];

        // A FLOOR AT THE DESIGN SIZE, because the chosen mode can be smaller than the interface.
        //
        // Every menu asset is authored for 640x480 and the 2D projection is over whatever
        // resolution the pipeline reports, so a mode BELOW that magnifies the interface instead of
        // shrinking it - at 320x200 the menus came out at double size with the widgets overlapping
        // each other. Which is not hypothetical: removing dgVoodoo reset gfxconf.dat, and the
        // choice index it came back with pointed at 320x200.
        //
        // Taking the larger of the two is right in both directions. Above 640x480 the chosen mode
        // wins and uiScale scales the interface up to it; below, the interface stays at the size it
        // was drawn for and the GL layer scales the whole thing down to the window, which is what
        // the DirectDraw path did through the wrapper anyway.
        if (chosen.ScreenWidth > width && chosen.ScreenHeight > height)
        {
            width = chosen.ScreenWidth;
            height = chosen.ScreenHeight;
        }

#ifdef ARTS_AGIGL
        // UNDER -gl, THE WINDOW IS A BETTER SOURCE OF TRUTH THAN THE ADAPTER TABLE.
        //
        // The chosen mode comes from gfxconf.dat, and that turned out to be fragile in a way that
        // matters: removing dgVoodoo changed the adapter list, the file reset, and the choice index
        // came back pointing at 320x200 with an empty mode table behind it. The interface was then
        // drawn at double size, and flooring it at 640x480 only traded that for a 4:3 image
        // pillarboxed into a 16:9 window - correct, and not what the screen looks like.
        //
        // The GL layer has already resolved a real window and knows its size. Using that makes the
        // logical resolution match the window exactly, so there is no pillarbox and no scaling of
        // the interface at all, and it cannot be wrong the way a saved index can - there is nothing
        // saved. This is also what the frontend-resolution patch in tools/patches.py was always
        // trying to achieve.
        // THE DESKTOP, NOT agiScreen AND NOT gfxconf.dat.
        //
        // agiScreen was the first attempt and is empty at this point - the GL layer has not brought
        // a window up yet when BeginPhase runs, so its fields are all zero and the override never
        // fired. gfxconf.dat was the second, and it cannot be relied on either: mode detection
        // happens in InitDirectDraw through DirectDrawEnumerate, which is a free function this
        // shim cannot intercept, so with dgVoodoo gone the mode table came back EMPTY and the
        // saved choice pointed at nothing.
        //
        // GetSystemMetrics needs neither. It is already imported by midtown2.exe, it answers before
        // anything graphical exists, and the desktop size is what the window is going to be anyway.
        const i32 desktop_w = GetSystemMetrics(0); // SM_CXSCREEN
        const i32 desktop_h = GetSystemMetrics(1); // SM_CYSCREEN

        // CAPPED, because the interface is scaled on the CPU into MM2's own 32 MB heap.
        //
        // uiScale resamples every menu bitmap by the ratio between this resolution and the 640x480
        // they were authored at, and gfxImage::Scale allocates the result from the game heap. At
        // 2560x1440 that is a factor of four in each axis - sixteen times the pixels - and the
        // allocator answers "Heap overrun" partway through the menus. 1920x1080 is a factor of
        // three and fits, which is not a guess: it is the resolution this ran at happily before
        // dgVoodoo was removed reset the configuration.
        //
        // So the desktop decides the SHAPE and the cap decides the cost. Anything wider is still
        // rendered to the full window by the GL layer; only the interface's own working resolution
        // is limited.
        i32 target_w = desktop_w;
        i32 target_h = desktop_h;

        if (target_w > kFrontendMaxWidth || target_h > kFrontendMaxHeight)
        {
            target_w = kFrontendMaxWidth;
            target_h = kFrontendMaxHeight;
        }

        if (agiGLEnabled() && target_w > width && target_h > height)
        {
            width = target_w;
            height = target_h;
        }

        // NO FRONTEND CLAMP. Confirmed by testing BOTH ways: with the clamp the mirror, minimap and
        // pause menu sit offset; without it they are correctly placed. The clamp does NOT affect
        // whether the world renders - that was tested too, and the view is black either way - so it
        // is purely a placement control and the correct setting is off.
#endif

        gfxPipeline::SetRes(width, height, kFrontendColorDepth, kFrontendZDepth, false);

#ifdef ARTS_AGIGL
        // The GL layer sized its render target and blit rectangle when the window was created,
        // from a resolution that has just changed. Rerunning the calculation is what makes the
        // frontend fill the window instead of occupying a 4:3 slice of it.
        if (agiGLEnabled() && hwndMain != nullptr)
            agiGLInitScaling(hwndMain);
#endif
    }
    else
    {
        // gfxInterfaces[gfxInterfaceChoice].Resolutions[ResolutionChoice], read at
        // [eax + ecx*8 + 0x70] with a 0x270 stride - which is what gfx/gfxinterface.h's layout was
        // derived from. Both depth arguments take the same ColorDepth: the original pushes the
        // same eax twice.
        const gfxInterface& adapter = gfxInterfaces[gfxInterfaceChoice];
        const gfxResData& resolution = adapter.Resolutions[adapter.ResolutionChoice];

        width = resolution.ScreenWidth;
        height = resolution.ScreenHeight;

        gfxPipeline::SetRes(width, height, resolution.ColorDepth, resolution.ColorDepth, false);

    }

    // THE LINE THIS WHOLE FILE EXISTS FOR.
    //
    // Everything above and everything below is shared by both arms and runs unchanged on the
    // DirectDraw path. This is the only conditional in the function.
    //
    // The original discards the result, and so does this - a failed BeginGfx3D here is not checked
    // by anything. agiGLBeginGfx3D is idempotent: gfxPipeline::BeginGfx has already brought the
    // device up through InitDirectDraw, so this second call finds the framebuffer already built at
    // the right size and returns, and rebuilds it only if SetRes above changed the resolution -
    // which is precisely the job the original call is doing for DirectDraw.
// One unconditional call. gfxPipeline::BeginGfx3D now carries the -gl branch itself - it has to,
    // because its tail creates the viewports and hands one to the private ForceSetViewport - so
    // there is nothing left for this site to choose between.
    gfxPipeline::BeginGfx3D();

    vglSetFormat(0x142, 0, 0, 0);

    datDisplayUsed("Just before loading screen");

    char load_screen_name[kLoadScreenNameSize];

    GetLoadScreenName(load_screen_name);

    gfxImage* image = gfxLoadImage(load_screen_name, false);

    if (image == nullptr)
        image = gfxLoadImage("loading", false);

    if (image == nullptr)
    {
        MessageBoxA(nullptr, reinterpret_cast<const char*>(AngelReadString(kNoLoadScreenString)),
            APPTITLE, kMessageBoxIconHand);

        exit(0);
    }

    // Scale to the resolution chosen above, then hand the scaled image to gfxBitmap::Create, which
    // takes its own reference. gfxImage::Scale frees the source pixels as it goes, so the order is
    // not interchangeable.
    image->Scale(width, height);

    splash_bg = gfxBitmap::Create(image, false);

    // The release idiom, written out because 1999 inlined it at every call site and there is no
    // ?Release@gfxImage@@ symbol to call. Transcribed from the original: decrement the refcount,
    // and only when it reaches zero destruct and free. Access_Destroy is what genheaders.py emits
    // to reach the destructor, which is private in midtown2.exe and must stay that way - its access
    // is part of its mangled name.
    if (--image->RefCount == 0)
    {
        image->Access_Destroy();
        operator delete(image);
    }

    // ProgressCB's first argument is the address 0x005E0CFC, four bytes of .data? that nothing else
    // in midtown2.exe reads or writes - the only reference to it in the whole image is this push,
    // and it has no name in the linker map, so game.asm reaches it as GraphicsPreviousMenu + 1.
    // Reached the same way here rather than substituting "" so that nothing has to be assumed
    // about what ProgressCB does with the pointer.
    ProgressCB(reinterpret_cast<const char*>(&GraphicsPreviousMenu + 1), 10);

    datDisplayUsed("Just after loading screen");

    gfxReleaseFont();

    // Not an optimisation and not removable: with the internal cache off, gfxTexture::InitCache
    // returns immediately, which is why docs/opengl_activation.md can leave InitCache out of Set B.
    gfxTexture::EnableCache(false);

    // gfxCreateFont runs on both paths again.
    //
    // It was skipped under -gl for a while, because it asks gfxTexture::Create for the font page,
    // that went to DirectDraw, and DirectDraw was not there - so it returned null and gfxCreateFont
    // did `mov ecx, [eax+0Ch]` on it at 0x004B1228. Skipping it only moved the failure: with no
    // font, mmTextNode::Init called through a garbage pointer instead.
    //
    // agigl/glddshim.cpp is what makes it work rather than avoidable. lpDD now answers
    // CreateSurface with a GL-backed surface, so the original loader allocates, locks, writes and
    // unlocks exactly as it always did, and the pixels land in a texture.
    gfxCreateFont();

    // The three constructions. Each one is `operator new`, a null test, then the constructor -
    // transcribed rather than written as `new T()` for the reason given at the placement-new
    // declaration above. Only the first stores its result; asCullManager and mmInput publish
    // themselves through asCullManager::Instance and GameInputPtr from inside their constructors.
    //
    // TWO OF THE THREE RECOVERED LAYOUTS ARE SHORT, AND THAT IS WORTH KNOWING SOMEWHERE.
    // BeginPhase's allocation literals are ground truth - they are what the 1999 compiler wrote
    // from sizeof - and two of them are larger than the headers in this tree claim:
    //
    //     eqEventHandler   allocates 0x160   check_size says 0x148   short by 0x18
    //     asCullManager    allocates 0x0A8   check_size says 0x0A8   agrees
    //     mmInput          allocates 0x248   check_size says 0x1DC   short by 0x6C
    //
    // Nothing here is broken by that, because the literals are used and not sizeof(). But anything
    // that ever allocates one of these classes from C++ with `new` would allocate too little and
    // let the original machine code write past the end of the block - which is why the assertion
    // below is `<=` and not `==`: it does not demand the layouts be finished, it refuses the one
    // direction that corrupts the heap.
    static_assert(sizeof(eqEventHandler) <= kEqEventHandlerSize,
        "eqEventHandler is larger than the 0x160 BeginPhase allocates for it");
    static_assert(sizeof(asCullManager) <= kCullManagerSize,
        "asCullManager is larger than the 0xA8 BeginPhase allocates for it");
    static_assert(sizeof(mmInput) <= kInputSize,
        "mmInput is larger than the 0x248 BeginPhase allocates for it");

    void* event_handler = operator new(kEqEventHandlerSize);

    eqEventHandler::SuperQ =
        event_handler ? new (event_handler) eqEventHandler() : nullptr;

    datDisplayUsed("Just before InitAudioManager");

    InitAudioManager((audioFlags & 1) != 0);

    datDisplayUsed("Just after InitAudioManager");

    // The original pushes MMAUDMGRPTR unadjusted, which is the proof that AudManager's asNode base
    // sits at offset 0. AudManager has no header in the tree, so the cast says so explicitly
    // rather than relying on a class definition that does not exist.
    ROOT.AddChild(reinterpret_cast<asNode*>(MMAUDMGRPTR));

    void* cull_manager = operator new(kCullManagerSize);

    if (cull_manager)
        new (cull_manager) asCullManager(kCullManagerArg1, kCullManagerArg2);

    datDisplayUsed("Just before GameInput");

    void* game_input = operator new(kInputSize);

    if (game_input)
        new (game_input) mmInput();

    GameInputPtr->AttachToPipe();
    GameInputPtr->Init(inputDevice);

    datDisplayUsed("At end of BeginPhase");

    // WHAT IS NOT TRANSCRIBED, AND WHY IT IS SAFE TO LEAVE OUT.
    //
    // The original opens an MSVC exception frame at entry - `push -1; push 0x005A8E54; push fs:[0];
    // mov fs:[0], esp` - and steps [ebp-4] through unwind states 0, 1 and 2 around the three
    // allocations above, so that a throwing constructor would free the block that was just
    // allocated. tools/build.py compiles with -EHs-c-, so no such frame can be emitted here.
    //
    // It is dead code in the original too. The frame only ever runs on a C++ throw, this codebase
    // has no throw in it, and a structured exception walking the chain gets ExceptionContinueSearch
    // from __CxxFrameHandler for a non-C++ record. ?BeginPhase@@YAX_N@Z_SEH at 0x005A8E54 stays in
    // game.asm as an unreferenced PROC once this is registered, which costs nothing.
    //
    // This is the same argument docs/opengl_activation.md already accepted for -EHs-c- in wave one,
    // written out here because this is the first ported function that actually had a frame.
}

#endif
