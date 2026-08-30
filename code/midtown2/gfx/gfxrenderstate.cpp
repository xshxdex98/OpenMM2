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

#include "gfxrenderstatedata.h"
#include "gfxrenderstate.h"

#include "origfallback.h"

#include "vector7/matrix44.h"

#ifdef ARTS_AGIGL
#    include "agigl/glframe.h"
#    include "agigl/glswitch.h"
#endif

define_dummy_symbol(gfx_gfxrenderstate);

// ?SetCamera@gfxRenderState@@SAXABVMatrix44@@@Z
//
// Reached from gfxPipeline::ForceSetViewport, which is why porting BeginGfx3D alone was not
// enough: ForceSetViewport flushes the viewport and then sets the camera, and this function ends
// in lpD3DDev->SetTransform(D3DTRANSFORMSTATE_VIEW, &sm_View) at 0x004B2A61. Under -gl that
// faulted at +0x3F reading a null device, three calls below a function that had just been ported.
//
// Everything except that one line is renderer-agnostic bookkeeping the rest of the engine reads,
// so it is transcribed rather than skipped: sm_Camera is what the caller passed, sm_View is its
// fast inverse composed with sm_FullComposite, and m_Touched's bit 7 is what tells the state
// flusher the view changed. Leaving any of it out would leave the globals disagreeing with what
// was actually loaded.
void gfxRenderState::SetCamera(const Matrix44& cameraMatrix)
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        sm_Camera = cameraMatrix;

        sm_View.FastInverse(sm_Camera);
        sm_View.Dot(sm_FullComposite);

        agiGLSetViewMatrix(reinterpret_cast<const f32*>(&sm_View));

        m_Touched |= 0x80;

        return;
    }
#endif

    ArtsOrigSetCamera44(cameraMatrix);
}
