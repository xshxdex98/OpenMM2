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

define_dummy_symbol(agigl_gllight);

#ifdef ARTS_AGIGL

#    include "data7/printer.h"

#    include "glcommon.h"
#    include "glcontext.h"
#    include "glstub.h"

// D3D7 LIGHTING, TRANSLATED INTO GL'S FIXED-FUNCTION LIGHTS.
//
// Until this existed, agiGLApplyRenderState forced GL_LIGHTING off no matter what the game asked
// for. That was a deliberate stand-in and it is why every car rendered solid black: MM2's car
// bodies arrive with FVF 0x112 - position, normal, one texture coordinate, no diffuse colour - so
// all of their shading is meant to come from lighting. Enabling GL_LIGHTING with no lights
// translated shaded them by nothing at all, and switching it off instead made them flat white
// modulated by their paint texture, which looked right in the showcase and wrong everywhere else.
//
// The city does not depend on this the way vehicles do - its geometry carries baked vertex colour -
// which is why the world looked correct in a race while every car in it was a silhouette.

namespace
{
    // D3DLIGHT7, by byte offset. Written as offsets rather than a struct because it arrives from
    // 1999 machine code and the layout is the contract, not the field names.
    constexpr usize kLightType = 0x00 / 4;      // 1 POINT, 2 SPOT, 3 DIRECTIONAL
    constexpr usize kLightDiffuse = 0x04 / 4;   // four floats, r g b a
    constexpr usize kLightSpecular = 0x14 / 4;
    constexpr usize kLightAmbient = 0x24 / 4;
    constexpr usize kLightPosition = 0x34 / 4;  // three floats
    constexpr usize kLightDirection = 0x40 / 4;
    constexpr usize kLightAtten0 = 0x54 / 4;
    constexpr usize kLightAtten1 = 0x58 / 4;
    constexpr usize kLightAtten2 = 0x5C / 4;
    constexpr usize kLightPhi = 0x64 / 4;

    // D3DMATERIAL7.
    constexpr usize kMaterialDiffuse = 0x00 / 4;
    constexpr usize kMaterialAmbient = 0x10 / 4;
    constexpr usize kMaterialSpecular = 0x20 / 4;
    constexpr usize kMaterialEmissive = 0x30 / 4;
    constexpr usize kMaterialPower = 0x40 / 4;

    constexpr i32 kMaxLights = 8;

    struct GLLight
    {
        f32 Data[0x68 / 4];
        bool Valid;
        bool Enabled;
    };

    GLLight g_lights[kMaxLights];

    // GL TRANSFORMS A LIGHT'S POSITION BY THE MODELVIEW AT THE MOMENT glLightfv IS CALLED.
    //
    // D3D does not: its lights are in world space and stay there. So a light can only be issued
    // while the modelview holds the VIEW matrix alone - with the world matrix still in it the light
    // would be placed relative to whatever object happened to be drawn last, and would swim as the
    // scene moved. agiGLApplyLights is therefore called again whenever the view changes, not only
    // when the game sets a light.
    void IssueLight(i32 index)
    {
        const GLLight& light = g_lights[index];

        if (!light.Valid)
            return;


        const GLenum id = GL_LIGHT0 + static_cast<GLenum>(index);
        const f32* d = light.Data;

        // THE MODELVIEW MUST HOLD THE VIEW ALONE WHILE A LIGHT'S POSITION IS ISSUED.
        //
        // glLightfv(GL_POSITION) bakes the CURRENT modelview into the light; D3D keeps lights in
        // world space and does not. agiGLSetViewTransform already honours this - it loads the view,
        // issues the lights, then restores the composed matrix - but IssueLight is also reached
        // from SetLight and LightEnable, and those fire at arbitrary points in the frame with
        // world * view loaded.
        //
        // modShader::BeginEnvMap and EndEnvMap call LightEnable for lights 0, 1 and 2 around every
        // environment-mapped car, and cityLevel::Draw restores two lights through SetLight. Each of
        // those re-baked the sun through whatever object transform happened to be current, so its
        // direction was wrong for everything drawn afterwards until the next view transform put it
        // back - lighting that changes from frame to frame with no change in the scene.
        const bool pushed = agiGLPushViewOnly();

        // DIAGNOSTIC: a timeline of what each light actually becomes.
        //
        // The scene now stays correct for a long stretch and then goes dark and STAYS dark, which is
        // a persistent change rather than a per-frame swing - so the last entry logged for a light
        // is the state it went dark in, and the entry before it is what it should have been.
        // Reported only when a light's issued values change, so a steady scene is silent.
        {
            static f32 last[8][7] = {};
            static bool seen[8] = {};

            const f32 now[7] = {d[kLightDiffuse + 0], d[kLightDiffuse + 1], d[kLightDiffuse + 2],
                d[kLightDirection + 0], d[kLightDirection + 1], d[kLightDirection + 2],
                static_cast<f32>(reinterpret_cast<const u32*>(d)[kLightType])};

            bool changed = !seen[index];

            for (i32 i = 0; i < 7; ++i)
            {
                if (last[index][i] != now[i])
                    changed = true;
            }

            if (changed)
            {
                seen[index] = true;

                for (i32 i = 0; i < 7; ++i)
                    last[index][i] = now[i];

                if (agiGLProbeVerbose())
                    Displayf("LIGHTSET %d type=%d diffuse=%.2f,%.2f,%.2f dir=%.3f,%.3f,%.3f viewonly=%d", index,
                        static_cast<i32>(now[6]), now[0], now[1], now[2], now[3], now[4], now[5], pushed ? 1 : 0);
            }
        }

        glLightfv(id, GL_DIFFUSE, &d[kLightDiffuse]);
        glLightfv(id, GL_SPECULAR, &d[kLightSpecular]);
        glLightfv(id, GL_AMBIENT, &d[kLightAmbient]);

        // dltType IS A DWORD, NOT A FLOAT, and reading it through the f32 view is why every light
        // came out as type 0.
        //
        // D3DLIGHT7 is mostly floats so it arrives here as f32*, but its first member is the
        // D3DLIGHTTYPE enum. Reading the integer 3 as a float gives about 4e-45, which casts back
        // to 0 - an invalid type that fell through to the point-light branch and used dvPosition.
        // For a directional light that is (0,0,0), so every light in the game sat at the world
        // origin and the cars it was meant to shade rendered black. The probe printed "type=0" for
        // all eight, which is not a value D3D can produce, and that is what gave it away.
        const i32 type = static_cast<i32>(reinterpret_cast<const u32*>(d)[kLightType]);

        if (type == 3)
        {
            // Directional. GL has no direction for a light - it has a position with w = 0, meaning
            // "infinitely far away in this direction", and the vector points AT the light rather
            // than away from it. Hence the negation.
            const f32 position[4] = {-d[kLightDirection + 0], -d[kLightDirection + 1], -d[kLightDirection + 2], 0.0f};

            glLightfv(id, GL_POSITION, position);

            // A directional light does not attenuate. GL would still apply the quadratic terms if
            // a previous point light had left them set.
            glLightf(id, GL_CONSTANT_ATTENUATION, 1.0f);
            glLightf(id, GL_LINEAR_ATTENUATION, 0.0f);
            glLightf(id, GL_QUADRATIC_ATTENUATION, 0.0f);
            glLightf(id, GL_SPOT_CUTOFF, 180.0f);
        }
        else
        {
            const f32 position[4] = {d[kLightPosition + 0], d[kLightPosition + 1], d[kLightPosition + 2], 1.0f};

            glLightfv(id, GL_POSITION, position);

            glLightf(id, GL_CONSTANT_ATTENUATION, d[kLightAtten0]);
            glLightf(id, GL_LINEAR_ATTENUATION, d[kLightAtten1]);
            glLightf(id, GL_QUADRATIC_ATTENUATION, d[kLightAtten2]);

            if (type == 2)
            {
                // Spot. D3D describes the cone with two full angles in radians - theta inside, phi
                // outside - and a falloff between them. GL has one HALF angle in degrees plus an
                // exponent, so phi/2 converted to degrees is the closest honest mapping and the
                // soft edge between theta and phi is lost. MM2 uses spots for headlights, where
                // that shows as a harder edge rather than as a wrong shape.
                const f32 cutoff = d[kLightPhi] * 0.5f * 57.2957795f;

                glLightf(id, GL_SPOT_CUTOFF, (cutoff > 90.0f) ? 90.0f : cutoff);
                glLightfv(id, GL_SPOT_DIRECTION, &d[kLightDirection]);
            }
            else
            {
                glLightf(id, GL_SPOT_CUTOFF, 180.0f);
            }
        }

        if (pushed)
            agiGLPopViewOnly();
    }
} // namespace

// Re-issues every enabled light. Called after the view matrix changes, because that is when GL's
// idea of where the lights are stops matching D3D's.
void agiGLApplyLights()
{
    for (i32 i = 0; i < kMaxLights; ++i)
    {
        if (g_lights[i].Valid && g_lights[i].Enabled)
            IssueLight(i);
    }
}

// IDirect3DDevice7::SetLight
ilong __stdcall agiGLStubSetLight(void* self, u32 index, f32* light)
{
    (void) self;

    if (light == nullptr)
        return 0x80004005;

    if (index >= static_cast<u32>(kMaxLights))
    {
        // GL fixed-function guarantees eight, and agigl/glcaps.cpp reports eight, so the game
        // should never ask for a ninth. Saying so once is better than silently dropping it.
        static bool said = false;

        if (!said)
        {
            said = true;
            Errorf("agiGL: light %u is past the eight GL fixed-function provides", index);
        }

        return 0x80004005;
    }

    for (usize i = 0; i < sizeof(g_lights[0].Data) / sizeof(f32); ++i)
        g_lights[index].Data[i] = light[i];

    g_lights[index].Valid = true;

    if (g_lights[index].Enabled)
        IssueLight(static_cast<i32>(index));

    return 0;
}

// IDirect3DDevice7::LightEnable
ilong __stdcall agiGLStubLightEnable(void* self, u32 index, u32 enable)
{
    (void) self;

    if (index >= static_cast<u32>(kMaxLights))
        return 0x80004005;

    // DIAGNOSTIC: every LightEnable, in order.
    //
    // The vehicle preview draws with lighting ON and ZERO lights enabled, so it is lit by nothing.
    // gfxRenderState::Default does SetLight(0, &gfxLight::Sun) followed by LightEnable(0, 1), and
    // gfxRenderState::DisableAllLights turns them all off again - so the question is simply which
    // of the two ran last before the menu drew, and whether the enable reached this layer at all.
    {
        static i32 shown = 0;

        if (shown < 30 && agiGLProbeVerbose())
        {
            ++shown;

            Displayf("LIGHTENABLE %d index=%u enable=%u", shown, index, enable);
        }
    }

    g_lights[index].Enabled = (enable != 0);

    agiGL->EnableDisable(GL_LIGHT0 + index, g_lights[index].Enabled);

    if (g_lights[index].Enabled)
        IssueLight(static_cast<i32>(index));

    return 0;
}

// IDirect3DDevice7::GetLightEnable, which was a stub that wrote nothing.
//
// This is what left every car a black silhouette. modShader::BeginEnvMap saves the enable state of
// lights 0, 1 and 2 through this call, disables them for the reflection pass, and EndEnvMap turns
// them back on from what it saved. With the stub writing nothing, the game saved whatever was on
// its stack - zero - and dutifully restored "off". Every light in the scene went out the first time
// a car was drawn, and everything rendered after that was lit by nothing.
//
// Same shape of bug as GetRenderState: a getter that reports success while leaving the caller's
// variable untouched is worse than one that fails, because the caller acts on the rubbish.
// DIAGNOSTIC: how many lights are actually contributing right now.
//
// GL_LIGHTING stays on through the whole scene (measured: zero frames with it off), so a flat,
// evenly-bright scene is not the master switch - it is every LIGHT being disabled while lighting
// stays enabled, which renders ambient-only and reads exactly as "fullbright".
i32 agiGLEnabledLightCount()
{
    i32 count = 0;

    for (i32 i = 0; i < kMaxLights; ++i)
    {
        if (g_lights[i].Enabled)
            ++count;
    }

    return count;
}

// IDirect3DDevice7::GetLight - AND THE GAME RESTORES WHAT THIS HANDS BACK.
//
// cityLevel::Draw saves two lights before it draws the sky-lit pass and puts them back afterwards:
//
//     gfxRenderState::GetLight(0, &SomeGfxLight1);
//     gfxRenderState::GetLight(1, &...);
//
// which reaches this slot through gfxRenderState::GetLight. As a do-nothing stub it left those
// globals holding whatever they held before, and the restore then pushed that back through
// SetLight - so the city was lit by stale or uninitialised light parameters every time that path
// ran, with the colours and directions changing as the globals did.
//
// This is the third instance of one pattern, and it is the pattern to check first for any "state
// goes wrong on its own" symptom: MM2 SAVES DEVICE STATE AND RESTORES IT, so a getter that does
// not answer honestly does not lose a read - it makes the game overwrite its own state with a
// fiction. GetLightEnable was already implemented for exactly this reason; GetRenderState needed
// D3DRENDERSTATE_AMBIENT for it; this is the same bug in the light slot.
// LIGHTING ON WITH NO LIGHTS ENABLED IS ALWAYS BLACK, AND MM2 NEVER MEANS IT.
//
// The vehicle preview draws with GL_LIGHTING enabled and ZERO lights on. Measured, not inferred:
// every lit mesh in the showcase reports lights=0 while its material is 1.000 white and its paint
// texture is bound and real, so the car is black purely because nothing illuminates it.
//
// The kit shows the intended arrangement. gfxRenderState::Default does
//
//     gfxRenderState::SetLight(0, &gfxLight::Sun);
//     gfxRenderState::LightEnable(0, 1);
//
// and MenuManager positions that same Sun for the showcase but only calls SetLight - it relies on
// Default having enabled light 0 already. gfxRenderState::DisableAllLights turns every light off
// and zeroes sm_MaxLights, so whichever of the two ran last decides whether the menu is lit.
//
// Restoring the invariant rather than the call: if the game asks for lighting while no light is
// enabled, light 0 is switched on provided SetLight has actually given it one. In gameplay this is
// dead code - lights are always enabled there, measured - so it can only affect the case that is
// otherwise guaranteed black.
void agiGLEnsureSomeLightEnabled()
{
    extern bool agiGLLightingEnabled();

    if (!agiGLLightingEnabled())
        return;

    for (i32 i = 0; i < kMaxLights; ++i)
    {
        if (g_lights[i].Enabled)
            return;
    }

    if (!g_lights[0].Valid)
        return;

    static bool said = false;

    if (!said)
    {
        said = true;

        Displayf("GL: lighting is on with no light enabled - enabling light 0 (gfxLight::Sun)");
    }

    g_lights[0].Enabled = true;

    agiGL->EnableDisable(GL_LIGHT0, true);

    IssueLight(0);
}

ilong __stdcall agiGLStubGetLight(void* self, u32 index, f32* out)
{
    (void) self;

    if (out == nullptr)
        return 0x80004005;

    if (index >= static_cast<u32>(kMaxLights) || !g_lights[index].Valid)
    {
        // LEAVE THE CALLER'S BUFFER ALONE, AND THE REASON IS SPECIFIC TO WHAT MM2 READS INTO.
        //
        // This used to zero the buffer, on the general principle that a getter should never leave
        // an out-parameter untouched. That principle is right for a getter whose caller will USE
        // the value, and wrong here, because cityLevel reads a light back into the very global it
        // BUILDS its lights in - SomeGfxLight1 - and then refills only dvDirection and dcvDiffuse
        // from TIMEWEATHER before calling SetLight with it.
        //
        // So zeroing did not produce a harmless empty light. It wiped the fields the game does not
        // rewrite, dltType among them, and a type of 0 is exactly the invalid value IssueLight
        // documents above: it falls through to the point-light branch and puts the sun at the world
        // origin. The scene stayed correctly lit until the first such read and then went dark and
        // stayed dark.
        //
        // Answering "no" without touching the buffer leaves the game's own template intact, which
        // is what a device that has never been given this light would leave behind anyway.
        return 0x80004005;
    }

    for (usize i = 0; i < sizeof(g_lights[0].Data) / sizeof(f32); ++i)
        out[i] = g_lights[index].Data[i];

    return 0;
}

ilong __stdcall agiGLStubGetLightEnable(void* self, u32 index, u32* out)
{
    (void) self;

    if (out == nullptr)
        return 0x80004005;

    if (index >= static_cast<u32>(kMaxLights))
    {
        *out = 0;

        return 0x80004005;
    }

    *out = g_lights[index].Enabled ? 1u : 0u;

    return 0;
}

// THE MATERIAL GL KEEPS IS NOT ALWAYS THE ONE SetMaterial WAS GIVEN.
//
// While GL_COLOR_MATERIAL is enabled the tracked property FOLLOWS the current colour, and
// glMaterialfv against a tracked property has no lasting effect - the GL spec is explicit that
// tracking overwrites it. Disabling tracking later does not restore what glMaterialfv was given;
// the material simply keeps the last value it tracked, which for MM2 is the white constant the
// packet builder supplies for a format with no diffuse.
//
// That is why the tyres stayed white after the colour-material rule went in. Their draw correctly
// stopped tracking, but by then the 0.10 material gfxRenderState had set was long gone, replaced by
// white while tracking was still on. Body panels hid it again: their material is 1.00, so tracked
// white and set white are the same number.
//
// So the values are cached on the way through and re-issued whenever a draw turns tracking off.
f32 agiGLMaterialDiffuse[4] = {1.0f, 1.0f, 1.0f, 1.0f};
f32 agiGLMaterialAmbient[4] = {1.0f, 1.0f, 1.0f, 1.0f};
bool agiGLMaterialValid = false;

static f32 g_applied_diffuse[4] = {-1.0f, -1.0f, -1.0f, -1.0f};
static f32 g_applied_ambient[4] = {-1.0f, -1.0f, -1.0f, -1.0f};
static bool g_material_clobbered = false;

// Called when a draw turns GL_COLOR_MATERIAL ON. Tracking is about to replace the material with the
// current colour, so whatever agiGLApplyMaterialColors last pushed is no longer what GL holds.
void agiGLInvalidateMaterialColors()
{
    g_material_clobbered = true;
}

void agiGLApplyMaterialColors()
{
    if (!agiGLMaterialValid)
        return;

    // THE GUARD MUST BE INVALIDATED BY TRACKING, NOT ONLY BY A NEW MATERIAL.
    //
    // Skipping the re-issue when the values match is only sound while nothing else writes the
    // material, and enabling GL_COLOR_MATERIAL does exactly that: the tracked property takes the
    // CURRENT COLOUR, so after any tracked draw GL holds white no matter what was last pushed here.
    // Remembering "already applied" across that makes the next untracked draw skip its re-issue and
    // render white.
    //
    // That is the reported "traffic light heads and nearby traffic cars turn white when hit": those
    // are material-coloured geometry, and a collision reorders the draws enough to interleave a
    // tracked draw between the material being set and the untracked draw that needs it.
    //
    // agiGLInvalidateMaterialColors is called whenever a draw turns tracking ON, which is the only
    // thing that can overwrite these behind our back.
    if (g_material_clobbered)
    {
        g_material_clobbered = false;

        for (i32 i = 0; i < 4; ++i)
        {
            g_applied_diffuse[i] = -1.0f;
            g_applied_ambient[i] = -1.0f;
        }
    }

    bool same = true;

    for (i32 i = 0; i < 4; ++i)
    {
        if (g_applied_diffuse[i] != agiGLMaterialDiffuse[i] || g_applied_ambient[i] != agiGLMaterialAmbient[i])
            same = false;
    }

    if (same)
        return;

    for (i32 i = 0; i < 4; ++i)
    {
        g_applied_diffuse[i] = agiGLMaterialDiffuse[i];
        g_applied_ambient[i] = agiGLMaterialAmbient[i];
    }

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, agiGLMaterialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, agiGLMaterialAmbient);
}

// IDirect3DDevice7::SetMaterial
ilong __stdcall agiGLStubSetMaterial(void* self, f32* material)
{
    (void) self;

    if (material == nullptr)
        return 0x80004005;

    // FRONT AND BACK together: MM2 does not use two-sided materials, and setting only the front
    // would leave any back face the game chooses not to cull lit by GL's default grey.
    // TEMPORARY PROBE: what the game actually asks for. modStatic::Draw sets the body's paint
    // through a DIRECT SetMaterial call (not via DoFlush), so whatever arrives here is the paint -
    // and if the diffuse is black, the cars are black for an honest reason rather than a lost one.
    {
        static i32 shown = 0;

        if (shown < 14)
        {
            ++shown;

            if (agiGLProbeVerbose())
                Displayf("MATERIAL %d diffuse=%.3f,%.3f,%.3f,%.3f ambient=%.3f,%.3f,%.3f emissive=%.3f,%.3f,%.3f pow=%.2f",
                    shown, material[kMaterialDiffuse + 0], material[kMaterialDiffuse + 1],
                    material[kMaterialDiffuse + 2], material[kMaterialDiffuse + 3],
                    material[kMaterialAmbient + 0], material[kMaterialAmbient + 1], material[kMaterialAmbient + 2],
                    material[kMaterialEmissive + 0], material[kMaterialEmissive + 1], material[kMaterialEmissive + 2],
                    material[kMaterialPower]);
        }
    }

    // KEEP A COPY, BECAUSE GL MAY BE ABOUT TO THROW THIS AWAY. See agiGLApplyMaterialColors below.
    {
        extern f32 agiGLProbeMaterial[4];

        for (i32 i = 0; i < 4; ++i)
        {
            agiGLProbeMaterial[i] = material[kMaterialDiffuse + i];
            agiGLMaterialDiffuse[i] = material[kMaterialDiffuse + i];
            agiGLMaterialAmbient[i] = material[kMaterialAmbient + i];
        }

        agiGLMaterialValid = true;
    }

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &material[kMaterialDiffuse]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &material[kMaterialAmbient]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &material[kMaterialSpecular]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &material[kMaterialEmissive]);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material[kMaterialPower]);

    return 0;
}

// D3DRENDERSTATE_AMBIENT, which is a colour rather than a light and so has nowhere else to go.
// D3D AND GL DISAGREE ABOUT THE DEFAULT GLOBAL AMBIENT, AND MM2 NEVER SETS IT.
//
//     D3DRENDERSTATE_AMBIENT default   0x00000000  - black
//     GL_LIGHT_MODEL_AMBIENT default   (0.2, 0.2, 0.2, 1.0)
//
// Measured: agiGLSetAmbient is called ZERO times in a full session, so the game leaves this at
// whatever the API starts with. Retail therefore renders with no global ambient at all while we add
// a fifth of grey to every lit surface - brighter than retail everywhere, and brightest where a
// material is dark, because ambient is the only term that does not fall off with the light.
//
// Called once from agiGLBeginGfx3D so the starting state matches D3D's.
void agiGLResetAmbientToD3DDefault()
{
    const f32 black[4] = {0.0f, 0.0f, 0.0f, 1.0f};

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);
}

void agiGLSetAmbient(u32 argb)
{
    // DIAGNOSTIC: every distinct global ambient the game asks for.
    //
    // Lighting is never off during world draws, so fullbright is not a disabled-lighting bug. The
    // remaining way a lit surface reads white is too much ambient - and we currently track ambient
    // from the VERTEX COLOUR (white) via GL_AMBIENT_AND_DIFFUSE, where D3D takes it from the
    // material. If MM2 raises this per area, ours blows out where retail does not.
    {
        static u32 last = 0xDEADBEEF;

        if (argb != last)
        {
            last = argb;

            if (agiGLProbeVerbose())
                Displayf("AMBIENT %08X", argb);
        }
    }

    const f32 ambient[4] = {
        static_cast<f32>((argb >> 16) & 0xFF) * (1.0f / 255.0f),
        static_cast<f32>((argb >> 8) & 0xFF) * (1.0f / 255.0f),
        static_cast<f32>(argb & 0xFF) * (1.0f / 255.0f),
        static_cast<f32>((argb >> 24) & 0xFF) * (1.0f / 255.0f),
    };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
}

#endif
