/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2

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

define_dummy_symbol(agigl_glddshim);

#ifdef ARTS_AGIGL

#    include "data7/printer.h"
#    include "gfx/ddrawshim.h"

#    include "glstub.h"
#    include "glsurface.h"

// The three KERNEL32 entry points midtown2.exe already imports, declared the way
// agigl/glload.cpp declares GetProcAddress for the same reason - there are no Windows headers on
// the include path.
extern "C" __declspec(dllimport) void* __stdcall LoadLibraryA(const char* name);
extern "C" __declspec(dllimport) void* __stdcall GetProcAddress(void* module, const char* name);

// A DirectDraw THAT WORKS, rather than one that merely does not crash.
//
// agigl/glstubdevice.cpp gives lpD3DDev methods that do nothing, which is the right answer for
// render state: the game keeps its own copy of all of it and the drawing is agigl/'s job. Surfaces
// are different. gfxTexture::Create and gfxBitmap::Load do not just SET state, they ask DirectDraw
// for memory, write pixels into it and hand it back - and a stub that returns success without
// memory turns that into a write through an uninitialised pointer.
//
// So this half is implemented rather than stubbed. CreateSurface makes a real GL texture with a
// CPU staging buffer beside it, Lock hands back that buffer, and Unlock uploads it. The original
// 1999 texture loader then works unmodified, which is worth far more than porting it would be: it
// is several hundred bytes of format conversion whose behaviour is the thing being preserved.
//
// WHY 32-BIT ARGB IS REPORTED. Lock has to describe the memory it returns, and the game converts
// into whatever format it is told. 8888 is the format agigl/glbitmap.cpp's converter already
// understands, it is what a GL texture wants anyway, and it avoids the 565-versus-555 ambiguity
// that the 16-bit paths carry. Reporting a 16-bit format here would work too and would cost a
// conversion on every upload for nothing.

namespace
{
    // A DirectDraw surface backed by a GL texture.
    //
    // lpVtbl FIRST, for the same reason agiGLSurface has one: everything the game holds this
    // through is typed IDirectDrawSurface7*, so offset 0 is read as a vtable pointer.
    struct StubSurface
    {
        const void* lpVtbl;

        // 'SUBS', at offset 4 - the same place agiGLSurface keeps its own magic, and for the same
        // reason. Both types are reached only through agiGLStubSurfaceVtbl, so a method that is
        // handed one of them can read this word safely and tell which it has.
        u32 Tag;

        agiGLSurface* gl;
        u8* pixels;
        i32 width;
        i32 height;
        bool locked;

        // True when `pixels` is a DIB section owned by GDI rather than a staging buffer of ours.
        // The two are freed differently, and only ours may be released after an upload.
        bool pixels_are_dib;

        // COM reference count, starting at 1 for the CreateSurface that hands the surface out.
        // Without this the pool was append-only: IDirectDrawSurface7::Release was a do-nothing
        // stub, so every texture the game finished with kept both its slot and its GL texture, and
        // after 512 of them CreateSurface began failing outright.
        u32 refs;

        // True for the one surface installed into lpdsRend. It is not drawn to, but it IS what the
        // game asks for the display's pixel format, and that answer is not the same as a texture's.
        bool is_primary;

        // The GDI side, made on first use. mmText draws with ordinary GDI calls into a DC it gets
        // from the surface, so a surface that cannot produce one cannot carry text.
        void* dc;
        void* dib;
    };

    constexpr u32 kStubSurfaceTag = 0x53425553; // 'SUBS'

    // THE STAGING BUFFERS DO NOT COME OUT OF MM2'S HEAP.
    //
    // Lock hands the game a CPU buffer the size of the whole surface - width * height * 4 - and
    // during a city load that is hundreds of textures churning through it. Taking that from the
    // game's 32 MB allocator is what exhausted it: the race load reached 9.5 MB of city and then
    // died with "Fatal Error: Heap overrun", followed by a FindHeap failure as the allocator came
    // apart. Two earlier attempts blamed the surface teardown for those crashes; the log order says
    // otherwise, the overrun comes first and the FindHeap failure is the cascade.
    //
    // This memory is the shim's own infrastructure, not game data, so it belongs on the process
    // heap where its size is bounded by the machine rather than by a 1999 budget. Allocating and
    // freeing through the SAME allocator also removes any question of handing MM2's allocator a
    // pointer it did not issue.
    using PFNGetProcessHeap = void*(__stdcall*)();
    using PFNHeapAlloc = void*(__stdcall*)(void*, u32, usize);
    using PFNHeapFree = i32(__stdcall*)(void*, u32, void*);

    PFNGetProcessHeap pGetProcessHeap = nullptr;
    PFNHeapAlloc pHeapAlloc = nullptr;
    PFNHeapFree pHeapFree = nullptr;
    void* g_process_heap = nullptr;

    bool LoadHeapApi()
    {
        if (g_process_heap != nullptr)
            return true;

        void* k = LoadLibraryA("KERNEL32.dll");

        if (k == nullptr)
            return false;

        pGetProcessHeap = reinterpret_cast<PFNGetProcessHeap>(GetProcAddress(k, "GetProcessHeap"));
        pHeapAlloc = reinterpret_cast<PFNHeapAlloc>(GetProcAddress(k, "HeapAlloc"));
        pHeapFree = reinterpret_cast<PFNHeapFree>(GetProcAddress(k, "HeapFree"));

        if (pGetProcessHeap == nullptr || pHeapAlloc == nullptr || pHeapFree == nullptr)
            return false;

        g_process_heap = pGetProcessHeap();

        return g_process_heap != nullptr;
    }

    u8* ShimAlloc(usize bytes)
    {
        if (!LoadHeapApi())
            return nullptr;

        return static_cast<u8*>(pHeapAlloc(g_process_heap, 0, bytes));
    }

    void ShimFree(u8* p)
    {
        if (p != nullptr && g_process_heap != nullptr)
            pHeapFree(g_process_heap, 0, p);
    }

    // How many surfaces are alive, for diagnostics only. There is no fixed pool any more.
    //
    // There used to be a 512-entry array, and membership in it was how Lock told one of our
    // surfaces from something else. Two problems. The scan was linear on every single call, and
    // far worse, 512 is not a real limit - DirectDraw's limit was video memory. MM2 holds hundreds
    // of textures for a city, so the array filled, CreateSurface started returning E_FAIL, and the
    // game carried on drawing with whatever was still bound. The identity check is the Tag field
    // now, which is O(1) and has no ceiling.
    i32 g_surface_count = 0;

    StubSurface* NewSurface(i32 width, i32 height)
    {
        if (width <= 0 || height <= 0)
            return nullptr;

        StubSurface* s = new StubSurface();

        s->lpVtbl = agiGLStubSurfaceVtbl();
        s->Tag = kStubSurfaceTag;
        // GL_BGRA, BECAUSE THE PIXELS THE GAME WRITES THROUGH Lock ARE B, G, R, A.
        //
        // FillPixelFormat tells the game these surfaces are D3D ARGB8888 - R 0x00FF0000,
        // G 0x0000FF00, B 0x000000FF - which on x86 is the bytes B, G, R, A, and the game fills the
        // staging buffer accordingly. Uploading that as GL_RGBA read red where blue was.
        //
        // Measured, not reasoned about, because reasoning got it wrong once already: the Mustang's
        // red paint texture read back as (12, 12, 215) per texel. Swap R and B and it is
        // (215, 12, 12) - a red car. The car rendered blue.
        //
        // The earlier attempt at this changed the format here AND deleted the hand swap in
        // ReleaseDC, which made the menu text teal and looked like proof the whole idea was wrong.
        // It was not. agigl creates surfaces in TWO places - here, and glbitmap.cpp for gfxBitmap -
        // and only this one changed format, so deleting that swap broke the surfaces that still
        // needed it. ReleaseDC now keys the swap off the surface's own format instead.
        s->gl = agiGLSurfaceCreate(width, height, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE, 1);

        // THE STAGING BUFFER IS ALLOCATED ON FIRST USE, NOT HERE.
        //
        // It comes out of MM2's own 32 MB heap, and the primary surface alone is 2560x1440x4 -
        // 14.7 MB, which the allocator answered with "Fatal Error: Heap overrun" and a
        // DebugBreak. Most surfaces are small and every one of them is locked, so the saving is
        // not the point; the primary is large and is NEVER locked, because under -gl nothing
        // blits to it - the GL layer draws instead. It exists only to be asked what pixel format
        // the display is in.
        s->pixels = nullptr;
        s->pixels_are_dib = false;
        s->is_primary = false;
        s->width = width;
        s->height = height;
        s->locked = false;
        s->refs = 1;

        ++g_surface_count;

        return s;
    }

    // The tag is cleared before a surface is freed, so a stale pointer fails this rather than
    // being treated as live memory.
    bool IsOurs(const void* p)
    {
        return p != nullptr && static_cast<const StubSurface*>(p)->Tag == kStubSurfaceTag;
    }

    // GDI, RESOLVED AT RUNTIME RATHER THAN IMPORTED.
    //
    // midtown2.exe imports exactly two GDI functions, SelectObject and DeleteObject, because it
    // never creates a device context - it asks DirectDraw for one. Adding imports would mean
    // teaching tools/implibs.py to invent entries the retail binary does not have, and GDI32 is
    // already in the process either way, so GetProcAddress is both smaller and less invasive. It
    // is the same route agigl/glload.cpp takes for the OpenGL entry points.
    using PFNCreateCompatibleDC = void*(__stdcall*)(void*);
    using PFNCreateDIBSection = void*(__stdcall*)(void*, const void*, u32, void**, void*, u32);
    using PFNSelectObject = void*(__stdcall*)(void*, void*);
    using PFNDeleteDC = i32(__stdcall*)(void*);
    using PFNDeleteObject = i32(__stdcall*)(void*);

    PFNCreateCompatibleDC pCreateCompatibleDC = nullptr;
    PFNCreateDIBSection pCreateDIBSection = nullptr;
    PFNSelectObject pSelectObject = nullptr;
    PFNDeleteDC pDeleteDC = nullptr;
    PFNDeleteObject pDeleteObject = nullptr;

    bool LoadGdi()
    {
        if (pCreateDIBSection != nullptr)
            return true;

        // LoadLibraryA rather than GetModuleHandleA: GDI32 is already in the process - the game
        // imports SelectObject and DeleteObject from it - so this only takes a reference and hands
        // back the handle it already has. GetModuleHandleA is not in tools/implibs.py's generated
        // libraries, and adding it would be a change to the import table for no gain.
        void* gdi = LoadLibraryA("GDI32.dll");

        if (gdi == nullptr)
            return false;

        pCreateCompatibleDC = reinterpret_cast<PFNCreateCompatibleDC>(GetProcAddress(gdi, "CreateCompatibleDC"));
        pCreateDIBSection = reinterpret_cast<PFNCreateDIBSection>(GetProcAddress(gdi, "CreateDIBSection"));
        pSelectObject = reinterpret_cast<PFNSelectObject>(GetProcAddress(gdi, "SelectObject"));
        pDeleteDC = reinterpret_cast<PFNDeleteDC>(GetProcAddress(gdi, "DeleteDC"));
        pDeleteObject = reinterpret_cast<PFNDeleteObject>(GetProcAddress(gdi, "DeleteObject"));

        return pCreateDIBSection != nullptr && pCreateCompatibleDC != nullptr && pSelectObject != nullptr;
    }

    // BITMAPINFOHEADER, declared by hand like everything else Win32 in this tree.
    struct GdiBitmapInfoHeader
    {
        u32 biSize;
        i32 biWidth;
        i32 biHeight;
        u16 biPlanes;
        u16 biBitCount;
        u32 biCompression;
        u32 biSizeImage;
        i32 biXPelsPerMeter;
        i32 biYPelsPerMeter;
        u32 biClrUsed;
        u32 biClrImportant;
    };

    // THE PRIMARY AND A TEXTURE DO NOT ANSWER THIS THE SAME WAY.
    //
    // A texture surface is memory this shim hands out and then uploads, so 8888 is simply true of
    // it - and convenient, being what a GL texture wants and what agigl/glbitmap.cpp's converter
    // already speaks.
    //
    // The primary is a different question. Nothing is ever drawn to it under -gl, but it is what
    // the game asks "what is the display?", and the answer decides which texture variants the
    // loaders look for. BeginPhase brings the mode up as SetRes(w, h, 16, 16, 0) - a SIXTEEN bit
    // display - so claiming 32 sent gfxLoadTargaImage and friends down a path whose files are not
    // there. It reported every car texture as "not found", and vehCar::Init then dereferenced the
    // null it got back at 0x0042C0C6.
    //
    // 565 rather than 555 because that is what a modern desktop is, and because
    // agigl/glbitmap.cpp's RGB_0555 case already documents the payload behind that name as 5-6-5.
    // TWO KINDS OF SURFACE ANSWER TO THE SAME INTERFACE.
    //
    // gfxBitmap::Surface holds an agiGLSurface, put there by the -gl branch in gfxBitmap::Create;
    // anything the original code allocated through lpDD->CreateSurface holds a StubSurface. Both
    // are reached as IDirectDrawSurface7*, and GetDC has to work on either - it did not, which is
    // why every mmText string was blank.
    agiGLSurface* ResolveGL(void* self)
    {
        if (self == nullptr)
            return nullptr;

        if (IsOurs(self))
            return static_cast<StubSurface*>(self)->gl;

        agiGLSurface* gl = static_cast<agiGLSurface*>(self);

        return (gl->Magic == kAgiGLSurfaceMagic) ? gl : nullptr;
    }

    void FillPixelFormat(gfxDDPixelFormat* pf, bool primary)
    {
        if (pf == nullptr)
            return;

        pf->dwSize = sizeof(gfxDDPixelFormat);
        pf->dwFourCC = 0;

        // THE PRIMARY REPORTS 32-BIT TOO, and the 16-bit answer that used to be here was wrong in
        // a way that only showed on screen.
        //
        // BeginPhase brings the mode up as SetRes(w, h, 16, 16, 0), so 16 looked like the honest
        // answer. But this is the format the IMAGE LOADERS ask about, and the whole 2D scaling
        // chain keys off what they produce: ScaledImageSize refuses anything that is not
        // ARGB_8888 or RGB_0888, so a 16-bit answer meant menu bitmaps were never resampled to the
        // screen size and every one of them drew at its authored 640x480 into a 1920x1080
        // projection - a third of the width, in the top-left corner.
        //
        // Under GL there is no 16-bit anything: the framebuffer is 32-bit, textures are RGBA8, and
        // the staging buffer Lock hands out is 8888. Reporting 16 described a device that does not
        // exist. It also did not fix the texture loading it was tried for - EnumTextureFormats
        // did.
        (void) primary;

        pf->dwFlags = 0x00000040 | 0x00000001; // DDPF_RGB | DDPF_ALPHAPIXELS
        pf->dwRGBBitCount = 32;
        pf->dwRBitMask = 0x00FF0000;
        pf->dwGBitMask = 0x0000FF00;
        pf->dwBBitMask = 0x000000FF;
        pf->dwRGBAlphaBitMask = 0xFF000000;
    }
    // Tears one surface down for good: GDI objects first, because only this file holds the
    // dynamically resolved DeleteDC and DeleteObject, then the staging buffer, then the GL texture.
    void DestroySurface(StubSurface* s)
    {
        if (s->gl != nullptr)
        {
            if (s->gl->Dc != nullptr && pDeleteDC != nullptr)
                pDeleteDC(s->gl->Dc);

            if (s->gl->Dib != nullptr && pDeleteObject != nullptr)
                pDeleteObject(s->gl->Dib);

            s->gl->Dc = nullptr;
            s->gl->Dib = nullptr;
            s->gl->DibPixels = nullptr;

            agiGLSurfaceDestroy(s->gl);
            s->gl = nullptr;
        }

        // A DIB's pixels belong to GDI and went with the DeleteObject above; a staging buffer is
        // ours. Unlock frees the staging buffer already, so this only catches a surface released
        // while still locked.
        if (!s->pixels_are_dib)
            ShimFree(s->pixels);

        s->pixels = nullptr;

        delete s;
    }

} // namespace

// IDirectDrawSurface7::AddRef and ::Release, which were do-nothing stubs.
//
// This is what stops the "out of stub surfaces at 512" failure. MM2 creates and destroys textures
// constantly - every vehicle preview loads a fresh set - and with Release doing nothing the pool
// filled, CreateSurface started returning E_FAIL, and the game carried on drawing with whatever
// texture happened to be bound. That is the single stale texture seen on every part of the car.
//
// Ordinary COM semantics: both return the new count, and the last Release destroys.
ilong __stdcall agiGLStubSurfaceAddRef(void* self)
{
    if (!IsOurs(self))
        return 1;

    StubSurface* s = static_cast<StubSurface*>(self);

    return static_cast<ilong>(++s->refs);
}

ilong __stdcall agiGLStubSurfaceRelease(void* self)
{
    if (!IsOurs(self))
        return 0;

    StubSurface* s = static_cast<StubSurface*>(self);

    if (s->refs > 1)
        return static_cast<ilong>(--s->refs);

    // THE LAST RELEASE DELIBERATELY DOES NOT FREE. THIS LEAKS, AND THAT IS THE LESSER BUG.
    //
    // Destroying here killed the game outright: MM2's own allocator reported "Heap overrun" and
    // "find heap failed" before a single line of log came out. It had never actually run before -
    // the identity check used to be a scan of a fixed array, and while a bug left every surface out
    // of that array IsOurs returned false for all of them, so Release was silently a no-op for
    // hours of testing. Fixing identity turned the teardown on for the first time and it took the
    // process down immediately.
    //
    // Something about the lifetime is wrong - either the game releases a surface it still uses (real
    // DirectDraw would have held references this shim never takes, QueryInterface among them), or
    // freeing through operator delete hands MM2's allocator memory it will not accept. Neither is
    // settled, and guessing at it is what this session has already paid for twice.
    //
    // Leaking is survivable now in a way it was not before: the 512-entry pool is gone, so nothing
    // fails when surfaces accumulate. It costs memory over a long session and nothing else. The
    // refcount is still tracked, so when the lifetime is understood the free goes back here.
    s->refs = 0;

    return 0;
}

// The functional surface methods. Declared normally rather than naked, so the compiler emits the
// right `ret` for each - the argument lists here are the whole reason these are safe.

ilong __stdcall agiGLStubSurfaceLock(void* self, void* rect, gfxDDSurfaceDesc2* desc, u32 flags, void* event)
{
    (void) rect;
    (void) flags;
    (void) event;

    if (!IsOurs(self) || desc == nullptr)
        return 0x80004005;

    StubSurface* s = static_cast<StubSurface*>(self);

    // ALLOCATED PER LOCK AND RELEASED AT UNLOCK, because this memory comes out of MM2's own
    // 32 MB heap and DirectDraw's did not.
    //
    // Holding one buffer per surface for the life of the surface took the city load from 9.6 MB to
    // 24.5 MB, and the allocator answered by failing - which surfaces as textures that cannot be
    // loaded, reported as "not found" several layers up. The GL texture is the copy that lasts;
    // this is scratch between a Lock and its Unlock, and the game never holds a lock across
    // anything.
    if (s->pixels == nullptr)
    {
        s->pixels = ShimAlloc(static_cast<usize>(s->width) * static_cast<usize>(s->height) * 4);

        if (s->pixels == nullptr)
        {
            Errorf("agiGL: could not allocate a %dx%d staging buffer", s->width, s->height);

            return 0x80004005;
        }
        s->pixels_are_dib = false;
    }

    desc->dwSize = sizeof(gfxDDSurfaceDesc2);
    desc->dwFlags = MM_DDSD_WIDTH | MM_DDSD_HEIGHT | MM_DDSD_PIXELFORMAT;
    desc->dwWidth = static_cast<u32>(s->width);
    desc->dwHeight = static_cast<u32>(s->height);
    desc->lPitch = s->width * 4;
    desc->lpSurface = s->pixels;

    FillPixelFormat(&desc->ddpfPixelFormat, s->is_primary);

    s->locked = true;

    return 0;
}

ilong __stdcall agiGLStubSurfaceUnlock(void* self, void* rect)
{
    (void) rect;

    if (!IsOurs(self))
        return 0x80004005;

    StubSurface* s = static_cast<StubSurface*>(self);

    if (s->locked && s->gl != nullptr && s->pixels != nullptr)
        agiGLSurfaceUpload(s->gl, 0, s->pixels, s->width * 4);

    // Give the scratch back. A DIB is not ours to free and is kept, because text is redrawn
    // constantly and rebuilding it per string would cost more than it saves.
    if (!s->pixels_are_dib)
    {
        ShimFree(s->pixels);
        s->pixels = nullptr;
    }

    s->locked = false;

    return 0;
}

// IDirectDrawSurface7::GetDC, and it has to work rather than fail.
//
// mmText draws every string with ordinary GDI into a DC it asks the surface for, and mmTextNode
// measures the result to decide how big a bitmap to make. With GetDC failing, that measurement
// came back zero, mmTextNode::Init asked gfxBitmap::Create for a 328x0 bitmap, got null - which
// none of its forty-odd callers check - and the next instruction was a method call through it.
//
// A DIB section IS the answer rather than a workaround: it is a GDI bitmap whose pixels are plain
// memory the caller chooses, which is exactly what a DirectDraw surface lock hands out. Pointing
// the surface's staging buffer at the DIB's bits makes GDI drawing and Lock/Unlock the same
// memory, so text drawn through the DC is uploaded by the next Unlock with nothing to copy.
// IDirect3DDevice7::Load - THE COPY THAT MAKES A LOADED TEXTURE VISIBLE.
//
// MM2 loads a texture in two halves. gfxTexture::Load fills a SYSTEM-MEMORY surface through
// Lock/Unlock, and gfxTextureCacheEntry then moves it into the VIDEO-MEMORY surface that is
// actually bound at draw time:
//
//     lpD3DDev->lpVtbl->Load(lpD3DDev, this->Surface, 0, lpDDSurface, 0, 0);
//
// This slot was a stub that reported success and copied nothing, so the surface the game drew from
// kept the zeroes it was created with. That is the whole of the black-car bug: the paint textures
// loaded perfectly into a surface nothing ever sampled.
//
// The shape of it is visible in the shim's own log. Surfaces are created in pairs of equal size -
// an even id created AND unlocked with real pixels, an odd id created immediately after and never
// locked at all. The odd one is the destination of this call.
//
// Two earlier explanations were wrong and are recorded so they are not tried again. The paletted
// converter in glbitmap.cpp is not involved; it serves gfxBitmap, and textures never reach it. And
// IDirectDrawSurface7::Blt is not involved either - a probe on that vtable logged zero calls
// across a full race, because the copy is a method on the DEVICE, not on the surface.
//
// Read back rather than cached: the source's staging buffer is freed at Unlock, and a readback is
// correct no matter how the source came by its contents.
ilong __stdcall agiGLStubDeviceLoadTexture(
    void* self, void* dest, void* dest_point, void* source, void* source_rect, u32 flags)
{
    (void) self;
    (void) dest_point;
    (void) source_rect;
    (void) flags;

    agiGLSurface* dst = ResolveGL(dest);
    agiGLSurface* src = ResolveGL(source);

    if (dst == nullptr || src == nullptr || dst->Texture == 0 || src->Texture == 0)
        return 0x80004005;

    // gfxTextureCacheEntry passes null for both, meaning the whole surface. gfxTexture::Blit passes
    // a POINT and a RECT, and honouring them costs little next to the cost of finding out later
    // that a decal landed in the wrong corner.
    i32 src_x = 0;
    i32 src_y = 0;
    i32 width = src->Width;
    i32 height = src->Height;

    if (source_rect != nullptr)
    {
        const i32* r = static_cast<const i32*>(source_rect); // left, top, right, bottom

        src_x = r[0];
        src_y = r[1];
        width = r[2] - r[0];
        height = r[3] - r[1];
    }

    i32 dst_x = 0;
    i32 dst_y = 0;

    if (dest_point != nullptr)
    {
        const i32* p = static_cast<const i32*>(dest_point); // x, y

        dst_x = p[0];
        dst_y = p[1];
    }

    // Clamp into both surfaces rather than trusting the caller: a copy that runs off the end of
    // either one is a heap overrun on the readback or undefined texels on the upload.
    if (width > dst->Width - dst_x)
        width = dst->Width - dst_x;

    if (height > dst->Height - dst_y)
        height = dst->Height - dst_y;

    if (width > src->Width - src_x)
        width = src->Width - src_x;

    if (height > src->Height - src_y)
        height = src->Height - src_y;

    if (width <= 0 || height <= 0 || src_x < 0 || src_y < 0 || dst_x < 0 || dst_y < 0)
        return 0x80004005;

    // COPY ON THE GPU. THE READBACK VERSION OF THIS CAUSED VISIBLE HITCHING.
    //
    // The first working version of this function did glGetTexImage into a staging buffer and
    // uploaded that. It fixed the black cars and introduced a stutter, for a reason that is obvious
    // in hindsight: glGetTexImage is a GPU-to-CPU readback, which drains the pipeline, and the
    // texture cache calls this DURING GAMEPLAY as it pages textures in. Every paged texture became
    // a synchronisation point, plus a heap allocation out of MM2's 32 MB.
    //
    // A framebuffer blit stays entirely on the GPU. Both framebuffer bindings are saved and put
    // back because this runs mid-frame, inside the game's own rendering, and the scissor test is
    // turned off across the blit - glBlitFramebuffer is scissored, and the game leaves scissoring
    // on for its viewport, which would clip the copy to whatever rectangle happened to be set.
    bool copied = false;

    if (glGenFramebuffers != nullptr && glBindFramebuffer != nullptr && glFramebufferTexture2D != nullptr &&
        glBlitFramebuffer != nullptr && glCheckFramebufferStatus != nullptr)
    {
        static u32 read_fbo = 0;
        static u32 draw_fbo = 0;

        if (read_fbo == 0)
            glGenFramebuffers(1, &read_fbo);

        if (draw_fbo == 0)
            glGenFramebuffers(1, &draw_fbo);

        GLint prev_read = 0;
        GLint prev_draw = 0;
        glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &prev_read);
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &prev_draw);

        // Raw glEnable/glDisable rather than agiGLContext::EnableDisable, so that the toggle cache
        // is neither consulted nor updated: the state is put back exactly as it was found, and the
        // cache's view of it stays true throughout.
        GLint scissor = 0;
        glGetIntegerv(GL_SCISSOR_TEST, &scissor);

        if (scissor)
            glDisable(GL_SCISSOR_TEST);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, read_fbo);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, src->Texture, 0);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw_fbo);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst->Texture, 0);

        if (glCheckFramebufferStatus(GL_READ_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE &&
            glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
        {
            glBlitFramebuffer(src_x, src_y, src_x + width, src_y + height, dst_x, dst_y, dst_x + width,
                dst_y + height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

            copied = true;
        }

        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, static_cast<GLuint>(prev_read));
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, static_cast<GLuint>(prev_draw));

        if (scissor)
            glEnable(GL_SCISSOR_TEST);
    }

    if (copied)
    {
        dst->Uploaded = true;

        return 0;
    }

    // FALLBACK, for a driver without framebuffer objects. Correct but slow, and kept only so that
    // the absence of an extension degrades to a stutter rather than to black cars.
    //
    // glGetTexImage has no source rectangle - it returns the whole level - so the readback is
    // sized to the source and the sub-rectangle is picked out on the way back in.
    u8* buffer = ShimAlloc(static_cast<usize>(src->Width) * static_cast<usize>(src->Height) * 4);

    if (buffer == nullptr)
    {
        Errorf("agiGL: could not allocate a %dx%d buffer for a texture copy", src->Width, src->Height);

        return 0x80004005;
    }

    GLint bound = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound);

    glBindTexture(GL_TEXTURE_2D, src->Texture);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, buffer);

    const u8* first = buffer + (static_cast<usize>(src_y) * static_cast<usize>(src->Width) + src_x) * 4;

    glBindTexture(GL_TEXTURE_2D, dst->Texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, src->Width);
    glTexSubImage2D(GL_TEXTURE_2D, 0, dst_x, dst_y, width, height, GL_BGRA, GL_UNSIGNED_BYTE, first);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(bound));

    // A destination that is never uploaded through the normal path still counts as having content
    // now, which is what keeps the "created but never written" diagnostic honest.
    dst->Uploaded = true;

    ShimFree(buffer);

    return 0;
}

ilong __stdcall agiGLStubSurfaceGetDC(void* self, void** out_dc)
{
    agiGLSurface* gl = ResolveGL(self);

    if (gl == nullptr || out_dc == nullptr)
        return 0x80004005;

    if (gl->Dc == nullptr)
    {
        if (!LoadGdi())
            return 0x80004005;

        GdiBitmapInfoHeader bi {};

        bi.biSize = sizeof(bi);
        bi.biWidth = gl->Width;

        // NEGATIVE, so the DIB is top-down and its rows are in the same order as the texture's.
        // A bottom-up DIB uploads every glyph upside down.
        bi.biHeight = -static_cast<i32>(gl->Height);
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = 0; // BI_RGB

        void* bits = nullptr;
        void* dc = pCreateCompatibleDC(nullptr);

        if (dc == nullptr)
            return 0x80004005;

        void* dib = pCreateDIBSection(dc, &bi, 0 /* DIB_RGB_COLORS */, &bits, nullptr, 0);

        if (dib == nullptr || bits == nullptr)
        {
            if (pDeleteDC != nullptr)
                pDeleteDC(dc);

            return 0x80004005;
        }

        pSelectObject(dc, dib);

        gl->Dc = dc;
        gl->Dib = dib;
        gl->DibPixels = static_cast<u8*>(bits);
    }

    *out_dc = gl->Dc;

    return 0;
}

ilong __stdcall agiGLStubSurfaceReleaseDC(void* self, void* dc)
{
    (void) dc;

    agiGLSurface* gl = ResolveGL(self);

    if (gl == nullptr || gl->DibPixels == nullptr)
        return 0x80004005;

    // GDI LEAVES THE ALPHA BYTE AT ZERO, AND THE COLOUR KEY IS THE ALPHA CHANNEL.
    //
    // agigl/glpipe.cpp implements the source colour key as an alpha test and agigl/glbitmap.cpp
    // folds black into alpha 0 when converting an image, so alpha is what decides whether a texel
    // survives. GDI knows none of that - it writes RGB into a 32-bit DIB and never touches the
    // fourth byte - so without this every glyph would arrive fully transparent.
    //
    // The rule restored is the one DirectDraw was using: these surfaces carry a black source
    // colour key, and mmTextNode::Init clears its bitmap to black before drawing into it, so black
    // is background and anything else is ink.
    const usize count = static_cast<usize>(gl->Width) * static_cast<usize>(gl->Height);

    // AND THE RED AND BLUE CHANNELS ARE SWAPPED ON THE WAY, BUT ONLY FOR AN RGBA SURFACE.
    //
    // A 32-bit GDI DIB is always B, G, R, A in memory. Whether that needs swapping depends on what
    // the texture was created as, and agigl creates surfaces two different ways: glbitmap.cpp makes
    // gfxBitmap surfaces GL_RGBA, which do need the swap, while the DirectDraw surfaces above are
    // GL_BGRA and already match the DIB byte for byte. Swapping those would undo the format.
    //
    // Keying off the surface's own format rather than assuming one is what went wrong before: the
    // swap was deleted outright when only one of the two creation sites had changed, and MM2's
    // yellow menu text came out teal.
    const bool swap_rb = (gl->Format == GL_RGBA);

    for (usize i = 0; i < count; ++i)
    {
        u8* px = gl->DibPixels + (i * 4);

        const u8 b = px[0];
        const u8 r = px[2];

        if (swap_rb)
        {
            px[0] = r;
            px[2] = b;
        }

        // The fourth byte is the one GDI leaves alone.
        px[3] = (r == 0 && px[1] == 0 && b == 0) ? 0 : 0xFF;
    }

    agiGLSurfaceUpload(gl, 0, gl->DibPixels, gl->Width * 4);

    return 0;
}

ilong __stdcall agiGLStubSurfaceGetPixelFormat(void* self, gfxDDPixelFormat* pf)
{
    if (!IsOurs(self))
        return 0x80004005;

    FillPixelFormat(pf, static_cast<StubSurface*>(self)->is_primary);

    return 0;
}

ilong __stdcall agiGLStubSurfaceGetSurfaceDesc(void* self, gfxDDSurfaceDesc2* desc)
{
    if (!IsOurs(self) || desc == nullptr)
        return 0x80004005;

    StubSurface* s = static_cast<StubSurface*>(self);

    desc->dwSize = sizeof(gfxDDSurfaceDesc2);
    desc->dwFlags = MM_DDSD_WIDTH | MM_DDSD_HEIGHT | MM_DDSD_PIXELFORMAT;
    desc->dwWidth = static_cast<u32>(s->width);
    desc->dwHeight = static_cast<u32>(s->height);
    desc->lPitch = s->width * 4;

    FillPixelFormat(&desc->ddpfPixelFormat, s->is_primary);

    return 0;
}

// IDirectDraw7::EnumDisplayModes, and leaving it a stub is what emptied the mode list.
//
// Returning S_OK without calling the callback describes a display that supports NO resolution at
// all. The game writes that into gfxconf.dat as a zero-length mode table, falls back to 640x480,
// and the saved choice index points at nothing - which is how the interface ended up at 320x200
// after dgVoodoo was removed, drawn at double size with the widgets overlapping. Precisely the
// mistake EnumTextureFormats had, with a more confusing symptom.
//
// The list is fixed rather than queried. Enumerating the real driver would mean EnumDisplaySettings
// and a mode-to-DDSURFACEDESC2 translation for modes the GL layer does not care about - it renders
// into a window of its own size whatever is chosen here. What the list has to do is offer sensible
// choices INCLUDING the one the window is already at, so the interface runs at native resolution
// rather than being scaled.
ilong __stdcall agiGLStubEnumDisplayModes(void* self, u32 flags, void* desc, void* context, void* callback)
{
    (void) self;
    (void) flags;
    (void) desc;

    if (callback == nullptr)
        return 0x80004005;

    using PFNEnumModes = ilong(__stdcall*)(gfxDDSurfaceDesc2*, void*);

    PFNEnumModes cb = reinterpret_cast<PFNEnumModes>(callback);

    struct Mode
    {
        u32 w;
        u32 h;
    };

    static const Mode kModes[] = {{640, 480}, {800, 600}, {1024, 768}, {1152, 864}, {1280, 720},
        {1280, 960}, {1280, 1024}, {1600, 900}, {1600, 1200}, {1920, 1080}, {1920, 1440}, {2560, 1440}};

    // Both depths, because MM2 filters on the one it wants and a list with only one can leave it
    // with nothing.
    static const u32 kDepths[] = {16, 32};

    for (const Mode& m : kModes)
    {
        for (u32 depth : kDepths)
        {
            gfxDDSurfaceDesc2 sd {};

            sd.dwSize = sizeof(sd);
            sd.dwFlags = MM_DDSD_WIDTH | MM_DDSD_HEIGHT | MM_DDSD_PIXELFORMAT;
            sd.dwWidth = m.w;
            sd.dwHeight = m.h;
            sd.lPitch = static_cast<i32>(m.w * (depth / 8));

            sd.ddpfPixelFormat.dwSize = sizeof(sd.ddpfPixelFormat);
            sd.ddpfPixelFormat.dwFlags = 0x00000040; // DDPF_RGB
            sd.ddpfPixelFormat.dwRGBBitCount = depth;

            if (depth == 16)
            {
                sd.ddpfPixelFormat.dwRBitMask = 0xF800;
                sd.ddpfPixelFormat.dwGBitMask = 0x07E0;
                sd.ddpfPixelFormat.dwBBitMask = 0x001F;
            }
            else
            {
                sd.ddpfPixelFormat.dwRBitMask = 0x00FF0000;
                sd.ddpfPixelFormat.dwGBitMask = 0x0000FF00;
                sd.ddpfPixelFormat.dwBBitMask = 0x000000FF;
            }

            // DDENUMRET_OK is 1 and CANCEL is 0.
            if (cb(&sd, context) == 0)
                return 0;
        }
    }

    return 0;
}

// IDirect3DDevice7::EnumTextureFormats, which is how the game learns what it may load.
//
// A stub that returns S_OK without calling the callback is not a harmless no-op here: it reports a
// device that supports NO texture format at all. gfxTexture then refuses anything whose format is
// not on the list, which is every .tga in the game - and the failure surfaces a long way off, as
// "Texture ptx_wheel not found" and as gfxGetTexture handing vehCar::Init a null it does not
// check.
//
// The four formats below are the ones MM2 actually asks for. 565 is the 16-bit opaque case, 1555
// and 4444 the 16-bit alpha cases, 8888 the 32-bit one. Enumerating more would not help; these are
// what the loaders match against.
ilong __stdcall agiGLStubEnumTextureFormats(void* self, void* callback, void* context)
{
    (void) self;

    if (callback == nullptr)
        return 0x80004005;

    // D3DENUMRET_OK is 1 and CANCEL is 0, so the loop stops the moment the game says it has seen
    // enough - which it does as soon as it finds one it likes.
    using PFNEnum = ilong(__stdcall*)(gfxDDPixelFormat*, void*);

    PFNEnum cb = reinterpret_cast<PFNEnum>(callback);

    struct Fmt
    {
        u32 bits;
        u32 r;
        u32 g;
        u32 b;
        u32 a;
    };

    static const Fmt kFormats[] = {
        {16, 0xF800, 0x07E0, 0x001F, 0x0000},         // 565
        {16, 0x7C00, 0x03E0, 0x001F, 0x8000},         // 1555
        {16, 0x0F00, 0x00F0, 0x000F, 0xF000},         // 4444
        {32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000}, // 8888
    };

    for (const Fmt& f : kFormats)
    {
        gfxDDPixelFormat pf {};

        pf.dwSize = sizeof(pf);
        pf.dwFlags = 0x00000040 | (f.a ? 0x00000001u : 0u); // DDPF_RGB | DDPF_ALPHAPIXELS
        pf.dwRGBBitCount = f.bits;
        pf.dwRBitMask = f.r;
        pf.dwGBitMask = f.g;
        pf.dwBBitMask = f.b;
        pf.dwRGBAlphaBitMask = f.a;

        if (cb(&pf, context) == 0)
            break;
    }

    return 0;
}

// IDirectDraw7::CreateSurface. The one method on the DirectDraw object that has to do real work,
// because everything else the game asks it for is either a capability query it ignores the answer
// to or a mode change the GL layer has already made.
ilong __stdcall agiGLStubCreateSurface(void* self, gfxDDSurfaceDesc2* desc, void** out, void* outer)
{
    (void) self;
    (void) outer;

    if (desc == nullptr || out == nullptr)
        return 0x80004005;

    StubSurface* s = NewSurface(static_cast<i32>(desc->dwWidth), static_cast<i32>(desc->dwHeight));

    if (s == nullptr)
        return 0x80004005;

    *out = s;

    return 0;
}

// The GL surface behind a slot the game is holding as an IDirectDrawSurface7*.
//
// Two different things can be in that field under -gl: an agiGLSurface, put there by the -gl
// branch in gfxBitmap::Create, or one of these, handed out by CreateSurface to original code. Both
// answer the same question, so this is where the two are reconciled rather than at every use.
// THE PRIMARY SURFACE, WHICH IS NOT ABOUT DRAWING.
//
// lpdsRend is where every 2D blit lands in the original, and under -gl nothing lands there at all -
// the GL layer draws instead. It still has to EXIST, because it is also the thing the game asks
// what pixel format the display is in, and that answer decides how images are loaded.
//
// gfxLoadImageAll tries four loaders in turn and returns null if all four decline, gfxGetTexture
// returns null in turn, and vehCar::Init then does `mov ecx, [eax+0Ch]` on it at 0x0042C0C6. The
// visible symptom was every car texture failing to load - "No damage textures found", "Texture
// ptx_wheel not found" - none of which appear on the DirectDraw path.
void agiGLInstallStubPrimary(i32 width, i32 height)
{
    if (lpdsRend != nullptr)
        return;

    StubSurface* s = NewSurface(width, height);

    if (s == nullptr)
        return;

    s->is_primary = true;

    lpdsRend = reinterpret_cast<IDirectDrawSurface7*>(s);

    Displayf("GL: installed a %dx%d primary surface so image loading can read a pixel format", width, height);
}

// The public form of ResolveGL, for callers outside this file. agigl/gldraw.cpp needs it to turn
// the surface D3D calls a texture into the GL texture behind it.
agiGLSurface* agiGLResolveSurface(void* slot)
{
    return ResolveGL(slot);
}

agiGLSurface* agiGLSurfaceFromStub(void* slot)
{
    if (slot == nullptr)
        return nullptr;

    if (IsOurs(slot))
        return static_cast<StubSurface*>(slot)->gl;

    return nullptr;
}

#endif
