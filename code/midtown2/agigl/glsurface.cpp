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

#include "glsurface.h"

#include "glstub.h"

#include "data7/printer.h"

#include "glcontext.h"

define_dummy_symbol(agigl_glsurface);

#ifdef ARTS_AGIGL

agiGLSurface* agiGLSurfaceFromSlot(void* slot)
{
    if (slot == nullptr)
        return nullptr;

    agiGLSurface* surface = static_cast<agiGLSurface*>(slot);

    if (surface->Magic == kAgiGLSurfaceMagic)
        return surface;

    // THERE ARE TWO KINDS OF SURFACE NOW, and this only knew about one.
    //
    // gfxBitmap::Create's -gl branch makes an agiGLSurface directly, but anything the original code
    // allocates through lpDD->CreateSurface gets one of the DirectDraw shim's instead - which wraps
    // an agiGLSurface rather than being one. Both arrive here as an IDirectDrawSurface7*, and until
    // now the second tripped the check below and killed the process.
    agiGLSurface* wrapped = agiGLSurfaceFromStub(slot);

    if (wrapped != nullptr)
        return wrapped;

    // Neither kind. That means a REAL IDirectDrawSurface7 is still alive and the two paths have
    // been mixed, which produces wrong pixels rather than a crash - so it is still caught here
    // rather than left to surface somewhere else.
    Quitf("agiGLSurface: slot %p is neither a GL surface nor a shim surface - a caller is unported", slot);

    return nullptr;
}

agiGLSurface* agiGLSurfaceCreate(i32 width, i32 height, GLenum internal, GLenum format, GLenum type, i32 level_count)
{
    if (width <= 0 || height <= 0)
    {
        Errorf("agiGLSurface: refusing to create a %dx%d surface", width, height);

        return nullptr;
    }

    if (level_count < 1)
        level_count = 1;

    agiGLSurface* surface = new agiGLSurface;

    surface->lpVtbl = agiGLStubSurfaceVtbl();
    surface->Magic = kAgiGLSurfaceMagic;
    surface->Dc = nullptr;
    surface->Dib = nullptr;
    surface->DibPixels = nullptr;
    surface->Texture = 0;
    surface->Framebuffer = 0;
    surface->Width = static_cast<u16>(width);
    surface->Height = static_cast<u16>(height);
    surface->LevelCount = static_cast<u16>(level_count);
    surface->Uploaded = false;
    surface->Flags = (level_count > 1) ? kAgiGLSurfaceMipMapped : 0;
    surface->Format = format;
    surface->Type = type;
    surface->Internal = internal;

    if (internal == GL_RGBA8 || internal == GL_RGBA4 || internal == GL_RGB5_A1)
        surface->Flags |= kAgiGLSurfaceAlpha;

    glGenTextures(1, &surface->Texture);
    agiGL->BindTexture2D(surface->Texture);

    // Allocate every level up front with a null pixel pointer, then fill each one with
    // glTexSubImage2D. glTexStorage2D would be tidier but needs 4.2, and MM2 must still start on
    // whatever the machine has - the format negotiation it used to do against DirectDraw exists
    // precisely because that machine was not assumed.
    for (i32 i = 0; i < level_count; ++i)
    {
        i32 level_width = width >> i;
        i32 level_height = height >> i;

        if (level_width < 1)
            level_width = 1;

        if (level_height < 1)
            level_height = 1;

        glTexImage2D(
            GL_TEXTURE_2D, i, static_cast<GLint>(internal), level_width, level_height, 0, format, type, nullptr);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, level_count - 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (level_count > 1) ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

    if (level_count > 1 && agiGL->GetMaxAnisotropy() > 0)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<GLfloat>(agiGL->GetMaxAnisotropy()));

    return surface;
}

void agiGLSurfaceDestroy(agiGLSurface* surface)
{
    if (surface == nullptr)
        return;

    if (surface->Framebuffer != 0 && glDeleteFramebuffers != nullptr)
        glDeleteFramebuffers(1, &surface->Framebuffer);

    if (surface->Texture != 0)
        glDeleteTextures(1, &surface->Texture);

    surface->Magic = 0;

    delete surface;
}

void agiGLSurfaceUpload(agiGLSurface* surface, i32 level, const void* pixels, i32 pitch)
{
    if (surface == nullptr || pixels == nullptr)
        return;

    if (level < 0 || level >= surface->LevelCount)
    {
        Errorf("agiGLSurface: level %d is outside the %d level chain", level, surface->LevelCount);

        return;
    }

    i32 width = surface->Width >> level;
    i32 height = surface->Height >> level;

    if (width < 1)
        width = 1;

    if (height < 1)
        height = 1;

    agiGL->BindTexture2D(surface->Texture);

    // gfxImage rows are packed to BytesPerRow, which is width * bpp / 8 and therefore only ever 1,
    // 2, 3 or 4 byte aligned. Telling GL the wrong alignment does not fail - it reads the rows at
    // the wrong stride, and the result is the classic diagonal-shear texture.
    GLint alignment = 1;

    if ((pitch & 7) == 0)
        alignment = 8;
    else if ((pitch & 3) == 0)
        alignment = 4;
    else if ((pitch & 1) == 0)
        alignment = 2;

    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

    glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, surface->Format, surface->Type, pixels);

    if (level == 0)
        surface->Uploaded = true;

    // The mip chain has to be built here, because the game can no longer build it itself.
    //
    // MM2 walks a DirectDraw mip chain with GetAttachedSurface, locking each sublevel and blitting
    // the smaller image into it. The shim's GetAttachedSurface deliberately FAILS - returning
    // success made gfxTexture::Load believe in a level that did not exist, Release a stale pointer
    // and fault at 0x004AD437 - so the game gets level 0 uploaded and never reaches levels 1..n.
    //
    // agiGLSurfaceCreate had already allocated every level with a null pixel pointer, which makes
    // them defined but leaves their contents undefined. Anything minified enough to sample past
    // level 0 therefore read whatever the driver left in that memory: car bodies came out wrong and
    // wheel treads came out white while the rims, drawn nearer to level 0, looked correct.
    //
    // Generating the chain from level 0 is what the game's own downsampling was for, so this
    // restores the intent rather than adding an effect. Without the entry point there is no way to
    // fill the levels at all, and a texture sampling only level 0 is far better than one sampling
    // undefined memory - so the chain is disowned instead, by capping MAX_LEVEL and dropping back
    // to plain linear filtering.
    if (level == 0 && surface->LevelCount > 1)
    {
        if (glGenerateMipmap != nullptr)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
    }
}

#endif
