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

#pragma once

#include "core/arts.h"

// Reconciles the decorated __stdcall import names the compiler emits with the undecorated ones
// tools/implibs.py puts in the import libraries. Without it nothing under agigl/ that calls a Win32
// function links. See the header itself for why.
#include "glwin32.h"

// THE GL VOCABULARY, DECLARED RATHER THAN INCLUDED.
//
// Nothing under agigl/ includes <windows.h>, <GL/gl.h> or a loader library, and nothing may start.
// The reason is the build, not taste: tools/build.py passes only -I code/midtown2, the MSVC include
// directory, and the Windows SDK's ucrt directory. The SDK's um/ and shared/ directories - where
// windows.h and GL/gl.h live - are not on the include path, and adding them would pull the modern
// Win32 headers into translation units compiled /arch:IA32 /GR- /GS- against the game's own 1999
// CRT.
//
// The same argument covers linking. tools/link.py links against import libraries generated from
// midtown2.exe's own import table, and midtown2.exe imports neither opengl32.dll nor the GDI
// pixel-format entry points. So every GL and WGL function is resolved at runtime through
// LoadLibraryA/GetProcAddress, which is exactly how the original resolves DirectDrawCreateEx out of
// DDRAW.DLL in gfxPipeline::BeginGfx2D. Nothing in the build has to change for agigl to link.
//
// The cost is that these declarations are ours rather than the SDK's, and a wrong one fails
// silently. Every enumerator below is a value from the OpenGL registry and must be checked against
// it, never guessed.

// __stdcall, spelled the way the GL headers spell it. Every GL and WGL entry point on Win32 x86
// uses it, and getting it wrong corrupts the stack on the first call rather than failing to link.
#define ARTS_GLAPI __stdcall

using GLenum = u32;
using GLbitfield = u32;
using GLuint = u32;
using GLint = i32;
using GLsizei = i32;
using GLboolean = u8;
using GLubyte = u8;
using GLfloat = f32;
using GLclampf = f32;
using GLdouble = f64;
using GLclampd = f64;
using GLchar = char;

// The Win32 handles agigl touches, as opaque pointers.
//
// `struct`, not `class`, and the difference is load-bearing. MSVC encodes a class as V and a struct
// as U in a mangled name, so `class HWND__` makes hwndMain mangle to ?hwndMain@@3PAVHWND__@@A while
// the retail linker map has ?hwndMain@@3PAUHWND__@@A. The reference would not resolve, and the
// error names a symbol that looks right.
//
// tools/genheaders.py currently emits `class HWND__;` into gfx/gfxpipeline.h, which is wrong for
// the same reason - its own comment above gfxWindowProc reads PAUHWND__ while the declaration
// beneath it would produce PAV. See docs/opengl_plan.md.
struct HWND__;
struct HDC__;
struct HGLRC__;
struct HINSTANCE__;

using HWND = HWND__*;
using HDC = HDC__*;
using HGLRC = HGLRC__*;
using HMODULE = HINSTANCE__*;

// ---------------------------------------------------------------------------------------------
// Enumerators, from the OpenGL registry.

constexpr GLenum GL_NO_ERROR = 0x0000;
constexpr GLenum GL_INVALID_ENUM = 0x0500;
constexpr GLenum GL_INVALID_VALUE = 0x0501;
constexpr GLenum GL_INVALID_OPERATION = 0x0502;
constexpr GLenum GL_STACK_OVERFLOW = 0x0503;
constexpr GLenum GL_STACK_UNDERFLOW = 0x0504;
constexpr GLenum GL_OUT_OF_MEMORY = 0x0505;
constexpr GLenum GL_INVALID_FRAMEBUFFER_OPERATION = 0x0506;

constexpr GLenum GL_POINTS = 0x0000;
constexpr GLenum GL_LINES = 0x0001;
constexpr GLenum GL_LINE_STRIP = 0x0003;
constexpr GLenum GL_TRIANGLES = 0x0004;
constexpr GLenum GL_TRIANGLE_STRIP = 0x0005;
constexpr GLenum GL_TRIANGLE_FAN = 0x0006;

constexpr GLenum GL_NEVER = 0x0200;
constexpr GLenum GL_LESS = 0x0201;
constexpr GLenum GL_EQUAL = 0x0202;
constexpr GLenum GL_LEQUAL = 0x0203;
constexpr GLenum GL_GREATER = 0x0204;
constexpr GLenum GL_NOTEQUAL = 0x0205;
constexpr GLenum GL_GEQUAL = 0x0206;
constexpr GLenum GL_ALWAYS = 0x0207;

constexpr GLenum GL_ZERO = 0;
constexpr GLenum GL_ONE = 1;
constexpr GLenum GL_SRC_COLOR = 0x0300;
constexpr GLenum GL_ONE_MINUS_SRC_COLOR = 0x0301;
constexpr GLenum GL_SRC_ALPHA = 0x0302;
constexpr GLenum GL_ONE_MINUS_SRC_ALPHA = 0x0303;
constexpr GLenum GL_DST_ALPHA = 0x0304;
constexpr GLenum GL_ONE_MINUS_DST_ALPHA = 0x0305;
constexpr GLenum GL_DST_COLOR = 0x0306;
constexpr GLenum GL_ONE_MINUS_DST_COLOR = 0x0307;

constexpr GLenum GL_CW = 0x0900;
constexpr GLenum GL_CCW = 0x0901;
constexpr GLenum GL_FRONT = 0x0404;
constexpr GLenum GL_BACK = 0x0405;
constexpr GLenum GL_FRONT_AND_BACK = 0x0408;

constexpr GLenum GL_POINT = 0x1B00;
constexpr GLenum GL_LINE = 0x1B01;
constexpr GLenum GL_FILL = 0x1B02;
constexpr GLenum GL_SMOOTH = 0x1D01;

constexpr GLenum GL_BLEND = 0x0BE2;
constexpr GLenum GL_CULL_FACE = 0x0B44;
constexpr GLenum GL_DEPTH_TEST = 0x0B71;
constexpr GLenum GL_SCISSOR_TEST = 0x0C11;
constexpr GLenum GL_ALPHA_TEST = 0x0BC0;
constexpr GLenum GL_FOG = 0x0B60;
constexpr GLenum GL_TEXTURE_2D = 0x0DE1;
constexpr GLenum GL_LIGHTING = 0x0B50;

// Fixed-function lighting, all GL 1.1 core. D3D7 addresses lights by index and GL_LIGHT0..GL_LIGHT7
// are consecutive, so the index adds straight onto GL_LIGHT0.
constexpr GLenum GL_VIEWPORT = 0x0BA2;
constexpr GLenum GL_FOG_MODE = 0x0B65;
constexpr GLenum GL_FOG_DENSITY = 0x0B62;
constexpr GLenum GL_FOG_START = 0x0B63;
constexpr GLenum GL_FOG_END = 0x0B64;
constexpr GLenum GL_FOG_COLOR = 0x0B66;
constexpr GLenum GL_EXP = 0x0800;
constexpr GLenum GL_EXP2 = 0x0801;

constexpr GLenum GL_LIGHT0 = 0x4000;

// Automatic texture coordinate generation, for D3DTSS_TCI_CAMERASPACENORMAL. GL_NORMAL_MAP hands
// the eye-space normal to the texture stage, which is exactly what that D3D flag asks for.
// The COORDINATE names glTexGeni takes as its first argument. These are not the same enums as the
// GL_TEXTURE_GEN_* capabilities below, which is what glEnable takes - passing a capability to
// glTexGeni is a GL_INVALID_ENUM.
constexpr GLenum GL_S = 0x2000;
constexpr GLenum GL_T = 0x2001;
constexpr GLenum GL_R = 0x2002;

constexpr GLenum GL_TEXTURE_GEN_S = 0x0C60;
constexpr GLenum GL_TEXTURE_GEN_T = 0x0C61;
constexpr GLenum GL_TEXTURE_GEN_R = 0x0C62;
constexpr GLenum GL_TEXTURE_GEN_MODE = 0x2500;
constexpr GLenum GL_NORMAL_MAP = 0x8511;
constexpr GLenum GL_TEXTURE = 0x1702;
constexpr GLenum GL_NORMALIZE = 0x0BA1;
constexpr GLenum GL_COLOR_MATERIAL = 0x0B57;
constexpr GLenum GL_AMBIENT = 0x1200;
constexpr GLenum GL_DIFFUSE = 0x1201;
constexpr GLenum GL_SPECULAR = 0x1202;
constexpr GLenum GL_POSITION = 0x1203;
constexpr GLenum GL_SPOT_DIRECTION = 0x1204;
constexpr GLenum GL_SPOT_EXPONENT = 0x1205;
constexpr GLenum GL_SPOT_CUTOFF = 0x1206;
constexpr GLenum GL_CONSTANT_ATTENUATION = 0x1207;
constexpr GLenum GL_LINEAR_ATTENUATION = 0x1208;
constexpr GLenum GL_QUADRATIC_ATTENUATION = 0x1209;
constexpr GLenum GL_LIGHT_MODEL_AMBIENT = 0x0B53;
constexpr GLenum GL_EMISSION = 0x1600;
constexpr GLenum GL_SHININESS = 0x1601;
constexpr GLenum GL_AMBIENT_AND_DIFFUSE = 0x1602;
constexpr GLenum GL_DITHER = 0x0BD0;

constexpr GLbitfield GL_DEPTH_BUFFER_BIT = 0x00000100;
constexpr GLbitfield GL_STENCIL_BUFFER_BIT = 0x00000400;
constexpr GLbitfield GL_COLOR_BUFFER_BIT = 0x00004000;

constexpr GLenum GL_UNSIGNED_BYTE = 0x1401;
constexpr GLenum GL_UNSIGNED_SHORT = 0x1403;
constexpr GLenum GL_UNSIGNED_INT = 0x1405;
constexpr GLenum GL_FLOAT = 0x1406;

constexpr GLenum GL_UNSIGNED_SHORT_5_6_5 = 0x8363;
constexpr GLenum GL_UNSIGNED_SHORT_4_4_4_4_REV = 0x8365;
constexpr GLenum GL_UNSIGNED_SHORT_1_5_5_5_REV = 0x8366;
constexpr GLenum GL_UNSIGNED_INT_8_8_8_8_REV = 0x8367;

constexpr GLenum GL_RGB = 0x1907;
constexpr GLenum GL_RGBA = 0x1908;
constexpr GLenum GL_BGR = 0x80E0;
constexpr GLenum GL_BGRA = 0x80E1;
constexpr GLenum GL_RGB8 = 0x8051;
constexpr GLenum GL_RGB5_A1 = 0x8057;
constexpr GLenum GL_RGBA4 = 0x8056;
constexpr GLenum GL_RGBA8 = 0x8058;
constexpr GLenum GL_DEPTH_COMPONENT = 0x1902;
constexpr GLenum GL_DEPTH_COMPONENT24 = 0x81A6;

constexpr GLenum GL_TEXTURE_MAG_FILTER = 0x2800;
constexpr GLenum GL_TEXTURE_MIN_FILTER = 0x2801;
constexpr GLenum GL_TEXTURE_WRAP_S = 0x2802;
constexpr GLenum GL_TEXTURE_WRAP_T = 0x2803;
constexpr GLenum GL_TEXTURE_BASE_LEVEL = 0x813C;
constexpr GLenum GL_TEXTURE_MAX_LEVEL = 0x813D;
constexpr GLenum GL_NEAREST = 0x2600;
constexpr GLenum GL_LINEAR = 0x2601;
constexpr GLenum GL_NEAREST_MIPMAP_NEAREST = 0x2700;
constexpr GLenum GL_LINEAR_MIPMAP_NEAREST = 0x2701;
constexpr GLenum GL_NEAREST_MIPMAP_LINEAR = 0x2702;
constexpr GLenum GL_LINEAR_MIPMAP_LINEAR = 0x2703;
constexpr GLenum GL_CLAMP_TO_EDGE = 0x812F;
constexpr GLenum GL_REPEAT = 0x2901;
constexpr GLenum GL_MIRRORED_REPEAT = 0x8370;
constexpr GLenum GL_TEXTURE_MAX_ANISOTROPY_EXT = 0x84FE;
constexpr GLenum GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT = 0x84FF;

constexpr GLenum GL_UNPACK_ALIGNMENT = 0x0CF5;
constexpr GLenum GL_UNPACK_ROW_LENGTH = 0x0CF2;
constexpr GLenum GL_PACK_ALIGNMENT = 0x0D05;
constexpr GLenum GL_TEXTURE_BINDING_2D = 0x8069;

constexpr GLenum GL_MODELVIEW = 0x1700;
constexpr GLenum GL_PROJECTION = 0x1701;

constexpr GLenum GL_VERTEX_ARRAY = 0x8074;
constexpr GLenum GL_NORMAL_ARRAY = 0x8075;
constexpr GLenum GL_COLOR_ARRAY = 0x8076;
constexpr GLenum GL_TEXTURE_COORD_ARRAY = 0x8078;

// EXT_secondary_color, core since 1.4. D3D7's D3DFVF_SPECULAR is a second per-vertex colour added
// after texturing, which is exactly what GL_COLOR_SUM plus a secondary colour does; there is no
// other fixed-function way to express it. Null on a driver without it - see agiGLDrawIndexedStrided
// for what happens then.
constexpr GLenum GL_COLOR_SUM = 0x8458;
constexpr GLenum GL_SECONDARY_COLOR_ARRAY = 0x845E;

constexpr GLenum GL_VENDOR = 0x1F00;
constexpr GLenum GL_RENDERER = 0x1F01;
constexpr GLenum GL_VERSION = 0x1F02;
constexpr GLenum GL_EXTENSIONS = 0x1F03;
constexpr GLenum GL_MAJOR_VERSION = 0x821B;
constexpr GLenum GL_MINOR_VERSION = 0x821C;
constexpr GLenum GL_NUM_EXTENSIONS = 0x821D;
constexpr GLenum GL_CONTEXT_PROFILE_MASK = 0x9126;
constexpr GLenum GL_CONTEXT_COMPATIBILITY_PROFILE_BIT = 0x00000002;
constexpr GLenum GL_MAX_SAMPLES = 0x8D57;
constexpr GLenum GL_MAX_TEXTURE_SIZE = 0x0D33;
constexpr GLenum GL_TEXTURE0 = 0x84C0;

constexpr GLenum GL_FRAMEBUFFER = 0x8D40;
constexpr GLenum GL_READ_FRAMEBUFFER = 0x8CA8;
constexpr GLenum GL_DRAW_FRAMEBUFFER = 0x8CA9;
constexpr GLenum GL_READ_FRAMEBUFFER_BINDING = 0x8CAA;
constexpr GLenum GL_DRAW_FRAMEBUFFER_BINDING = 0x8CA6;
constexpr GLenum GL_RENDERBUFFER = 0x8D41;
constexpr GLenum GL_COLOR_ATTACHMENT0 = 0x8CE0;
constexpr GLenum GL_DEPTH_ATTACHMENT = 0x8D00;
constexpr GLenum GL_FRAMEBUFFER_COMPLETE = 0x8CD5;

// ---------------------------------------------------------------------------------------------
// The entry points, as function pointers under their real names, so call sites read like ordinary
// GL. agiGLLoadLibrary fills the 1.1 core out of opengl32.dll; agiGLLoadExtensions fills the rest
// through wglGetProcAddress once a context is current. Anything the driver does not export stays
// null, which is what agiGLContext::HasExtension is for.

extern void(ARTS_GLAPI* glGetIntegerv)(GLenum pname, GLint* params);
extern void(ARTS_GLAPI* glFogi)(GLenum pname, GLint param);
extern void(ARTS_GLAPI* glFogf)(GLenum pname, GLfloat param);
extern void(ARTS_GLAPI* glFogfv)(GLenum pname, const GLfloat* params);
extern void(ARTS_GLAPI* glTexGeni)(GLenum coord, GLenum pname, GLint param);

// Fixed-function lighting. All GL 1.1 core, so no extension check is needed.
extern void(ARTS_GLAPI* glColorMaterial)(GLenum face, GLenum mode);
extern void(ARTS_GLAPI* glLightfv)(GLenum light, GLenum pname, const GLfloat* params);
extern void(ARTS_GLAPI* glLightf)(GLenum light, GLenum pname, GLfloat param);
extern void(ARTS_GLAPI* glLightModelfv)(GLenum pname, const GLfloat* params);
extern void(ARTS_GLAPI* glMaterialfv)(GLenum face, GLenum pname, const GLfloat* params);
extern void(ARTS_GLAPI* glMaterialf)(GLenum face, GLenum pname, GLfloat param);
extern void(ARTS_GLAPI* glGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
extern const GLubyte*(ARTS_GLAPI* glGetString)(GLenum name);
extern GLenum(ARTS_GLAPI* glGetError)();
extern void(ARTS_GLAPI* glEnable)(GLenum cap);
extern void(ARTS_GLAPI* glDisable)(GLenum cap);
extern void(ARTS_GLAPI* glClear)(GLbitfield mask);
extern void(ARTS_GLAPI* glClearColor)(GLclampf r, GLclampf g, GLclampf b, GLclampf a);
extern void(ARTS_GLAPI* glClearDepth)(GLclampd depth);
extern void(ARTS_GLAPI* glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
extern void(ARTS_GLAPI* glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
extern void(ARTS_GLAPI* glDepthMask)(GLboolean flag);
extern void(ARTS_GLAPI* glDepthFunc)(GLenum func);
extern void(ARTS_GLAPI* glBlendFunc)(GLenum sfactor, GLenum dfactor);
extern void(ARTS_GLAPI* glAlphaFunc)(GLenum func, GLclampf ref);
extern void(ARTS_GLAPI* glFrontFace)(GLenum mode);
extern void(ARTS_GLAPI* glCullFace)(GLenum mode);
extern void(ARTS_GLAPI* glPolygonMode)(GLenum face, GLenum mode);
extern void(ARTS_GLAPI* glShadeModel)(GLenum mode);
extern void(ARTS_GLAPI* glFinish)();
extern void(ARTS_GLAPI* glFlush)();
extern void(ARTS_GLAPI* glPixelStorei)(GLenum pname, GLint param);
extern void(ARTS_GLAPI* glReadPixels)(
    GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);

extern void(ARTS_GLAPI* glGenTextures)(GLsizei n, GLuint* textures);
extern void(ARTS_GLAPI* glDeleteTextures)(GLsizei n, const GLuint* textures);
extern void(ARTS_GLAPI* glBindTexture)(GLenum target, GLuint texture);
extern void(ARTS_GLAPI* glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height,
    GLint border, GLenum format, GLenum type, const void* pixels);
extern void(ARTS_GLAPI* glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width,
    GLsizei height, GLenum format, GLenum type, const void* pixels);
extern void(ARTS_GLAPI* glTexParameteri)(GLenum target, GLenum pname, GLint param);
extern void(ARTS_GLAPI* glTexParameterf)(GLenum target, GLenum pname, GLfloat param);

extern void(ARTS_GLAPI* glMatrixMode)(GLenum mode);
extern void(ARTS_GLAPI* glLoadIdentity)();
extern void(ARTS_GLAPI* glLoadMatrixf)(const GLfloat* m);
extern void(ARTS_GLAPI* glOrtho)(
    GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);

extern void(ARTS_GLAPI* glEnableClientState)(GLenum array);
extern void(ARTS_GLAPI* glDisableClientState)(GLenum array);
extern void(ARTS_GLAPI* glVertexPointer)(GLint size, GLenum type, GLsizei stride, const void* pointer);
extern void(ARTS_GLAPI* glNormalPointer)(GLenum type, GLsizei stride, const void* pointer);
extern void(ARTS_GLAPI* glColorPointer)(GLint size, GLenum type, GLsizei stride, const void* pointer);
extern void(ARTS_GLAPI* glTexCoordPointer)(GLint size, GLenum type, GLsizei stride, const void* pointer);
extern void(ARTS_GLAPI* glDrawArrays)(GLenum mode, GLint first, GLsizei count);
extern void(ARTS_GLAPI* glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices);

// The constant-colour half of a vertex stream. D3D expresses "one colour for every vertex" as a
// strided pointer with dwStride 0; GL expresses it by turning the array off and setting the current
// colour, because in GL a stride of 0 means "tightly packed" instead. glColor4ubv takes the four
// bytes in R,G,B,A order, which is NOT the order a D3DCOLOR sits in memory - see agiGLUnpackD3DColor.
extern void(ARTS_GLAPI* glColor4ubv)(const GLubyte* v);

// Post-1.1. Null when unsupported.
extern const GLubyte*(ARTS_GLAPI* glGetStringi)(GLenum name, GLuint index);
extern void(ARTS_GLAPI* glActiveTexture)(GLenum texture);

// EXT_secondary_color / GL 1.4, for D3DFVF_SPECULAR. Null on a driver that has neither.
extern void(ARTS_GLAPI* glSecondaryColorPointer)(GLint size, GLenum type, GLsizei stride, const void* pointer);
extern void(ARTS_GLAPI* glSecondaryColor3ubv)(const GLubyte* v);
// glGenerateMipmap, from GL 3.0 / ARB_framebuffer_object, with the EXT_framebuffer_object
// spelling as the fallback. It is the only way this shim can fill a mip chain: see
// agiGLSurfaceUpload for why the game cannot fill one itself.
extern void(ARTS_GLAPI* glGenerateMipmap)(GLenum target);


extern void(ARTS_GLAPI* glGenFramebuffers)(GLsizei n, GLuint* framebuffers);
extern void(ARTS_GLAPI* glDeleteFramebuffers)(GLsizei n, const GLuint* framebuffers);
extern void(ARTS_GLAPI* glBindFramebuffer)(GLenum target, GLuint framebuffer);
extern GLenum(ARTS_GLAPI* glCheckFramebufferStatus)(GLenum target);
extern void(ARTS_GLAPI* glFramebufferRenderbuffer)(
    GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern void(ARTS_GLAPI* glFramebufferTexture2D)(
    GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void(ARTS_GLAPI* glGenRenderbuffers)(GLsizei n, GLuint* renderbuffers);
extern void(ARTS_GLAPI* glDeleteRenderbuffers)(GLsizei n, const GLuint* renderbuffers);
extern void(ARTS_GLAPI* glBindRenderbuffer)(GLenum target, GLuint renderbuffer);
extern void(ARTS_GLAPI* glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern void(ARTS_GLAPI* glRenderbufferStorageMultisample)(
    GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern void(ARTS_GLAPI* glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0,
    GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

// ---------------------------------------------------------------------------------------------
// The WGL and GDI side. The three GDI entry points carry an agi prefix rather than their real
// names, so that a translation unit which does end up seeing wingdi.h cannot silently bind to the
// wrong declaration.

extern HGLRC(ARTS_GLAPI* wglCreateContext)(HDC hdc);
extern i32(ARTS_GLAPI* wglDeleteContext)(HGLRC hglrc);
extern i32(ARTS_GLAPI* wglMakeCurrent)(HDC hdc, HGLRC hglrc);
extern HGLRC(ARTS_GLAPI* wglGetCurrentContext)();
extern void*(ARTS_GLAPI* wglGetProcAddress)(const char* name);

// WGL_EXT_swap_control. Null when the driver does not advertise it, in which case vsync cannot be
// controlled from here and the driver's own setting stands.
extern i32(ARTS_GLAPI* wglSwapIntervalEXT)(i32 interval);

extern i32(ARTS_GLAPI* agiChoosePixelFormat)(HDC hdc, const void* pfd);
extern i32(ARTS_GLAPI* agiSetPixelFormat)(HDC hdc, i32 format, const void* pfd);
extern i32(ARTS_GLAPI* agiSwapBuffers)(HDC hdc);

// PIXELFORMATDESCRIPTOR, laid out as wingdi.h declares it. ChoosePixelFormat reads nSize and
// nVersion before anything else, so a wrong layout fails with a plausible error rather than a
// crash - which is the harder failure to find.
struct agiPixelFormatDescriptor
{
    u16 nSize;
    u16 nVersion;
    u32 dwFlags;
    u8 iPixelType;
    u8 cColorBits;
    u8 cRedBits;
    u8 cRedShift;
    u8 cGreenBits;
    u8 cGreenShift;
    u8 cBlueBits;
    u8 cBlueShift;
    u8 cAlphaBits;
    u8 cAlphaShift;
    u8 cAccumBits;
    u8 cAccumRedBits;
    u8 cAccumGreenBits;
    u8 cAccumBlueBits;
    u8 cAccumAlphaBits;
    u8 cDepthBits;
    u8 cStencilBits;
    u8 cAuxBuffers;
    u8 iLayerType;
    u8 bReserved;
    u32 dwLayerMask;
    u32 dwVisibleMask;
    u32 dwDamageMask;
};

check_size(agiPixelFormatDescriptor, 0x28);

constexpr u32 PFD_DOUBLEBUFFER = 0x00000001;
constexpr u32 PFD_DRAW_TO_WINDOW = 0x00000004;
constexpr u32 PFD_SUPPORT_OPENGL = 0x00000020;
constexpr u8 PFD_TYPE_RGBA = 0;
constexpr u8 PFD_MAIN_PLANE = 0;

// Resolves opengl32.dll and gdi32.dll and everything that does not need a current context.
// Returns false when opengl32.dll is missing or does not export the 1.1 core - the one failure the
// caller has to survive, because it means falling back to the original DirectDraw path rather than
// quitting outright.
bool agiGLLoadLibrary();

// Fills in everything that does need a current context: the post-1.1 core and the extensions. Must
// run after wglMakeCurrent, never before. wglGetProcAddress returns null with no current context,
// and it does so without an error.
void agiGLLoadExtensions();
