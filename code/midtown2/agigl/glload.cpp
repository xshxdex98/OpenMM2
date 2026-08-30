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

#include "glcommon.h"

#include "data7/printer.h"

define_dummy_symbol(agigl_glload);

// EVERYTHING UNDER agigl/ IS COMPILED OUT UNTIL -DARTS_AGIGL IS ADDED TO tools/build.py.
//
// This is deliberate, and it is the one switch that turns the OpenGL device layer on. Until it is
// defined these translation units contribute nothing but their dummy symbol, so adding the files to
// the tree cannot break a build that is currently green. See docs/opengl_plan.md, "Wiring it in".
#ifdef ARTS_AGIGL

// The three KERNEL32 entry points needed to resolve everything else. midtown2.exe already imports
// all three, so they come out of build/implibs/KeRNeL32.lib with no change to tools/link.py.
extern "C" __declspec(dllimport) HMODULE ARTS_GLAPI LoadLibraryA(const char* file_name);
extern "C" __declspec(dllimport) void* ARTS_GLAPI GetProcAddress(HMODULE module, const char* proc_name);
extern "C" __declspec(dllimport) i32 ARTS_GLAPI FreeLibrary(HMODULE module);

void(ARTS_GLAPI* glGetIntegerv)(GLenum, GLint*);
void(ARTS_GLAPI* glGetTexImage)(GLenum, GLint, GLenum, GLenum, void*);
void(ARTS_GLAPI* glFogi)(GLenum, GLint);
void(ARTS_GLAPI* glFogf)(GLenum, GLfloat);
void(ARTS_GLAPI* glFogfv)(GLenum, const GLfloat*);
void(ARTS_GLAPI* glTexGeni)(GLenum, GLenum, GLint);
void(ARTS_GLAPI* glColorMaterial)(GLenum, GLenum);
void(ARTS_GLAPI* glLightfv)(GLenum, GLenum, const GLfloat*);
void(ARTS_GLAPI* glLightf)(GLenum, GLenum, GLfloat);
void(ARTS_GLAPI* glLightModelfv)(GLenum, const GLfloat*);
void(ARTS_GLAPI* glMaterialfv)(GLenum, GLenum, const GLfloat*);
void(ARTS_GLAPI* glMaterialf)(GLenum, GLenum, GLfloat);
const GLubyte*(ARTS_GLAPI* glGetString)(GLenum);
GLenum(ARTS_GLAPI* glGetError)();
void(ARTS_GLAPI* glEnable)(GLenum);
void(ARTS_GLAPI* glDisable)(GLenum);
void(ARTS_GLAPI* glClear)(GLbitfield);
void(ARTS_GLAPI* glClearColor)(GLclampf, GLclampf, GLclampf, GLclampf);
void(ARTS_GLAPI* glClearDepth)(GLclampd);
void(ARTS_GLAPI* glViewport)(GLint, GLint, GLsizei, GLsizei);
void(ARTS_GLAPI* glScissor)(GLint, GLint, GLsizei, GLsizei);
void(ARTS_GLAPI* glDepthMask)(GLboolean);
void(ARTS_GLAPI* glDepthFunc)(GLenum);
void(ARTS_GLAPI* glBlendFunc)(GLenum, GLenum);
void(ARTS_GLAPI* glAlphaFunc)(GLenum, GLclampf);
void(ARTS_GLAPI* glFrontFace)(GLenum);
void(ARTS_GLAPI* glCullFace)(GLenum);
void(ARTS_GLAPI* glPolygonMode)(GLenum, GLenum);
void(ARTS_GLAPI* glShadeModel)(GLenum);
void(ARTS_GLAPI* glFinish)();
void(ARTS_GLAPI* glFlush)();
void(ARTS_GLAPI* glPixelStorei)(GLenum, GLint);
void(ARTS_GLAPI* glReadPixels)(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void*);

void(ARTS_GLAPI* glGenTextures)(GLsizei, GLuint*);
void(ARTS_GLAPI* glDeleteTextures)(GLsizei, const GLuint*);
void(ARTS_GLAPI* glBindTexture)(GLenum, GLuint);
void(ARTS_GLAPI* glTexImage2D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void*);
void(ARTS_GLAPI* glTexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void*);
void(ARTS_GLAPI* glTexParameteri)(GLenum, GLenum, GLint);
void(ARTS_GLAPI* glTexParameterf)(GLenum, GLenum, GLfloat);

void(ARTS_GLAPI* glMatrixMode)(GLenum);
void(ARTS_GLAPI* glLoadIdentity)();
void(ARTS_GLAPI* glLoadMatrixf)(const GLfloat*);
void(ARTS_GLAPI* glOrtho)(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);

void(ARTS_GLAPI* glEnableClientState)(GLenum);
void(ARTS_GLAPI* glDisableClientState)(GLenum);
void(ARTS_GLAPI* glVertexPointer)(GLint, GLenum, GLsizei, const void*);
void(ARTS_GLAPI* glNormalPointer)(GLenum, GLsizei, const void*);
void(ARTS_GLAPI* glColorPointer)(GLint, GLenum, GLsizei, const void*);
void(ARTS_GLAPI* glTexCoordPointer)(GLint, GLenum, GLsizei, const void*);
void(ARTS_GLAPI* glDrawArrays)(GLenum, GLint, GLsizei);
void(ARTS_GLAPI* glDrawElements)(GLenum, GLsizei, GLenum, const void*);
void(ARTS_GLAPI* glColor4ubv)(const GLubyte*);

const GLubyte*(ARTS_GLAPI* glGetStringi)(GLenum, GLuint);
void(ARTS_GLAPI* glActiveTexture)(GLenum);
void(ARTS_GLAPI* glGenerateMipmap)(GLenum);

void(ARTS_GLAPI* glSecondaryColorPointer)(GLint, GLenum, GLsizei, const void*);
void(ARTS_GLAPI* glSecondaryColor3ubv)(const GLubyte*);

void(ARTS_GLAPI* glGenFramebuffers)(GLsizei, GLuint*);
void(ARTS_GLAPI* glDeleteFramebuffers)(GLsizei, const GLuint*);
void(ARTS_GLAPI* glBindFramebuffer)(GLenum, GLuint);
GLenum(ARTS_GLAPI* glCheckFramebufferStatus)(GLenum);
void(ARTS_GLAPI* glFramebufferRenderbuffer)(GLenum, GLenum, GLenum, GLuint);
void(ARTS_GLAPI* glFramebufferTexture2D)(GLenum, GLenum, GLenum, GLuint, GLint);
void(ARTS_GLAPI* glGenRenderbuffers)(GLsizei, GLuint*);
void(ARTS_GLAPI* glDeleteRenderbuffers)(GLsizei, const GLuint*);
void(ARTS_GLAPI* glBindRenderbuffer)(GLenum, GLuint);
void(ARTS_GLAPI* glRenderbufferStorage)(GLenum, GLenum, GLsizei, GLsizei);
void(ARTS_GLAPI* glRenderbufferStorageMultisample)(GLenum, GLsizei, GLenum, GLsizei, GLsizei);
void(ARTS_GLAPI* glBlitFramebuffer)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);

HGLRC(ARTS_GLAPI* wglCreateContext)(HDC);
i32(ARTS_GLAPI* wglDeleteContext)(HGLRC);
i32(ARTS_GLAPI* wglMakeCurrent)(HDC, HGLRC);
HGLRC(ARTS_GLAPI* wglGetCurrentContext)();
void*(ARTS_GLAPI* wglGetProcAddress)(const char*);
i32(ARTS_GLAPI* wglSwapIntervalEXT)(i32);

i32(ARTS_GLAPI* agiChoosePixelFormat)(HDC, const void*);
i32(ARTS_GLAPI* agiSetPixelFormat)(HDC, i32, const void*);
i32(ARTS_GLAPI* agiSwapBuffers)(HDC);

static HMODULE agiGLLibrary;
static HMODULE agiGdiLibrary;
static i32 agiGLCoreMissing;

// THE TWO-STAGE RULE THAT CATCHES PEOPLE OUT. On Win32 the installable client driver exports the
// OpenGL 1.1 core from opengl32.dll itself, and wglGetProcAddress returns null for every one of
// those names. Anything newer is the other way round: it exists only through wglGetProcAddress,
// and only while a context is current. Asking the wrong one gives a null pointer and no error.
static void* GetCoreProc(const char* name)
{
    return GetProcAddress(agiGLLibrary, name);
}

static void* GetExtProc(const char* name)
{
    if (void* proc = wglGetProcAddress(name))
        return proc;

    // Some drivers do also export the newer entry points from the DLL. Trying both costs nothing
    // and covers the Mesa and llvmpipe software implementations.
    return GetProcAddress(agiGLLibrary, name);
}

#    define AGI_LOAD_CORE(NAME)                                       \
        do                                                            \
        {                                                             \
            *reinterpret_cast<void**>(&NAME) = GetCoreProc(#NAME);    \
            if (NAME == nullptr)                                      \
            {                                                         \
                ++agiGLCoreMissing;                                   \
                Errorf("GL: opengl32.dll does not export %s", #NAME); \
            }                                                         \
        } while (0)

#    define AGI_LOAD_EXT(NAME) *reinterpret_cast<void**>(&NAME) = GetExtProc(#NAME)

bool agiGLLoadLibrary()
{
    if (agiGLLibrary != nullptr)
        return agiGLCoreMissing == 0;

    agiGLLibrary = LoadLibraryA("opengl32.dll");

    if (agiGLLibrary == nullptr)
    {
        Errorf("GL: opengl32.dll not found");

        return false;
    }

    agiGdiLibrary = LoadLibraryA("gdi32.dll");

    if (agiGdiLibrary == nullptr)
    {
        Errorf("GL: gdi32.dll not found");

        return false;
    }

    // The pixel-format trio lives in gdi32, not opengl32, and midtown2.exe imports none of the
    // three - which is the whole reason agigl resolves rather than links them.
    *reinterpret_cast<void**>(&agiChoosePixelFormat) = GetProcAddress(agiGdiLibrary, "ChoosePixelFormat");
    *reinterpret_cast<void**>(&agiSetPixelFormat) = GetProcAddress(agiGdiLibrary, "SetPixelFormat");
    *reinterpret_cast<void**>(&agiSwapBuffers) = GetProcAddress(agiGdiLibrary, "SwapBuffers");

    if (agiChoosePixelFormat == nullptr || agiSetPixelFormat == nullptr || agiSwapBuffers == nullptr)
    {
        Errorf("GL: gdi32.dll is missing the pixel format entry points");

        return false;
    }

    *reinterpret_cast<void**>(&wglCreateContext) = GetCoreProc("wglCreateContext");
    *reinterpret_cast<void**>(&wglDeleteContext) = GetCoreProc("wglDeleteContext");
    *reinterpret_cast<void**>(&wglMakeCurrent) = GetCoreProc("wglMakeCurrent");
    *reinterpret_cast<void**>(&wglGetCurrentContext) = GetCoreProc("wglGetCurrentContext");
    *reinterpret_cast<void**>(&wglGetProcAddress) = GetCoreProc("wglGetProcAddress");

    if (wglCreateContext == nullptr || wglMakeCurrent == nullptr || wglGetProcAddress == nullptr)
    {
        Errorf("GL: opengl32.dll is missing the WGL entry points");

        return false;
    }

    agiGLCoreMissing = 0;

    AGI_LOAD_CORE(glGetIntegerv);
    AGI_LOAD_CORE(glGetTexImage);
    AGI_LOAD_CORE(glFogi);
    AGI_LOAD_CORE(glFogf);
    AGI_LOAD_CORE(glFogfv);
    AGI_LOAD_CORE(glTexGeni);
    AGI_LOAD_CORE(glColorMaterial);
    AGI_LOAD_CORE(glLightfv);
    AGI_LOAD_CORE(glLightf);
    AGI_LOAD_CORE(glLightModelfv);
    AGI_LOAD_CORE(glMaterialfv);
    AGI_LOAD_CORE(glMaterialf);
    AGI_LOAD_CORE(glGetString);
    AGI_LOAD_CORE(glGetError);
    AGI_LOAD_CORE(glEnable);
    AGI_LOAD_CORE(glDisable);
    AGI_LOAD_CORE(glClear);
    AGI_LOAD_CORE(glClearColor);
    AGI_LOAD_CORE(glClearDepth);
    AGI_LOAD_CORE(glViewport);
    AGI_LOAD_CORE(glScissor);
    AGI_LOAD_CORE(glDepthMask);
    AGI_LOAD_CORE(glDepthFunc);
    AGI_LOAD_CORE(glBlendFunc);
    AGI_LOAD_CORE(glAlphaFunc);
    AGI_LOAD_CORE(glFrontFace);
    AGI_LOAD_CORE(glCullFace);
    AGI_LOAD_CORE(glPolygonMode);
    AGI_LOAD_CORE(glShadeModel);
    AGI_LOAD_CORE(glFinish);
    AGI_LOAD_CORE(glFlush);
    AGI_LOAD_CORE(glPixelStorei);
    AGI_LOAD_CORE(glReadPixels);
    AGI_LOAD_CORE(glGenTextures);
    AGI_LOAD_CORE(glDeleteTextures);
    AGI_LOAD_CORE(glBindTexture);
    AGI_LOAD_CORE(glTexImage2D);
    AGI_LOAD_CORE(glTexSubImage2D);
    AGI_LOAD_CORE(glTexParameteri);
    AGI_LOAD_CORE(glTexParameterf);
    AGI_LOAD_CORE(glMatrixMode);
    AGI_LOAD_CORE(glLoadIdentity);
    AGI_LOAD_CORE(glLoadMatrixf);
    AGI_LOAD_CORE(glOrtho);
    AGI_LOAD_CORE(glEnableClientState);
    AGI_LOAD_CORE(glDisableClientState);
    AGI_LOAD_CORE(glVertexPointer);
    AGI_LOAD_CORE(glNormalPointer);
    AGI_LOAD_CORE(glColorPointer);
    AGI_LOAD_CORE(glTexCoordPointer);
    AGI_LOAD_CORE(glDrawArrays);
    AGI_LOAD_CORE(glDrawElements);
    AGI_LOAD_CORE(glColor4ubv);

    return agiGLCoreMissing == 0;
}

void agiGLLoadExtensions()
{
    AGI_LOAD_EXT(glGetStringi);
    AGI_LOAD_EXT(glActiveTexture);

    AGI_LOAD_EXT(glGenerateMipmap);

    if (glGenerateMipmap == nullptr)
        *reinterpret_cast<void**>(&glGenerateMipmap) = GetExtProc("glGenerateMipmapEXT");

    // D3DFVF_SPECULAR has no other fixed-function spelling. Both are null on a driver with neither
    // GL 1.4 nor EXT_secondary_color, and agiGLDrawIndexedStrided drops the specular stream and
    // says so once rather than drawing it as the primary colour.
    AGI_LOAD_EXT(glSecondaryColorPointer);
    AGI_LOAD_EXT(glSecondaryColor3ubv);

    if (glSecondaryColorPointer == nullptr)
    {
        *reinterpret_cast<void**>(&glSecondaryColorPointer) = GetExtProc("glSecondaryColorPointerEXT");
        *reinterpret_cast<void**>(&glSecondaryColor3ubv) = GetExtProc("glSecondaryColor3ubvEXT");
    }

    AGI_LOAD_EXT(glGenFramebuffers);
    AGI_LOAD_EXT(glDeleteFramebuffers);
    AGI_LOAD_EXT(glBindFramebuffer);
    AGI_LOAD_EXT(glCheckFramebufferStatus);
    AGI_LOAD_EXT(glFramebufferRenderbuffer);
    AGI_LOAD_EXT(glFramebufferTexture2D);
    AGI_LOAD_EXT(glGenRenderbuffers);
    AGI_LOAD_EXT(glDeleteRenderbuffers);
    AGI_LOAD_EXT(glBindRenderbuffer);
    AGI_LOAD_EXT(glRenderbufferStorage);
    AGI_LOAD_EXT(glRenderbufferStorageMultisample);
    AGI_LOAD_EXT(glBlitFramebuffer);

    AGI_LOAD_EXT(wglSwapIntervalEXT);

    // The EXT_framebuffer_object spelling, for drivers that never picked up the ARB version. The
    // EXT entry points are call-compatible with the ARB ones for everything agigl uses, which is
    // why they can be dropped into the same slots.
    if (glGenFramebuffers == nullptr)
    {
        *reinterpret_cast<void**>(&glGenFramebuffers) = GetExtProc("glGenFramebuffersEXT");
        *reinterpret_cast<void**>(&glDeleteFramebuffers) = GetExtProc("glDeleteFramebuffersEXT");
        *reinterpret_cast<void**>(&glBindFramebuffer) = GetExtProc("glBindFramebufferEXT");
        *reinterpret_cast<void**>(&glCheckFramebufferStatus) = GetExtProc("glCheckFramebufferStatusEXT");
        *reinterpret_cast<void**>(&glFramebufferRenderbuffer) = GetExtProc("glFramebufferRenderbufferEXT");
        *reinterpret_cast<void**>(&glFramebufferTexture2D) = GetExtProc("glFramebufferTexture2DEXT");
        *reinterpret_cast<void**>(&glGenRenderbuffers) = GetExtProc("glGenRenderbuffersEXT");
        *reinterpret_cast<void**>(&glDeleteRenderbuffers) = GetExtProc("glDeleteRenderbuffersEXT");
        *reinterpret_cast<void**>(&glBindRenderbuffer) = GetExtProc("glBindRenderbufferEXT");
        *reinterpret_cast<void**>(&glRenderbufferStorage) = GetExtProc("glRenderbufferStorageEXT");
    }
}

#    undef AGI_LOAD_CORE
#    undef AGI_LOAD_EXT

#endif
