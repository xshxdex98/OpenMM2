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

#include "datbasetokenizer.h"

class Vector2;
class Vector3;
class Vector4;

class datAsciiTokenizer : public datBaseTokenizer
{
public:
    // ?GetInt@datAsciiTokenizer@@UAEHXZ
    ARTS_IMPORT virtual i32 GetInt();

    // ?GetFloat@datAsciiTokenizer@@UAEMXZ
    ARTS_IMPORT virtual f32 GetFloat();

    // ?GetVector@datAsciiTokenizer@@UAEXAAVVector4@@@Z
    ARTS_IMPORT virtual void GetVector(Vector4& arg1);

    // ?GetVector@datAsciiTokenizer@@UAEXAAVVector3@@@Z
    ARTS_IMPORT virtual void GetVector(Vector3& arg1);

    // ?GetVector@datAsciiTokenizer@@UAEXAAVVector2@@@Z
    ARTS_IMPORT virtual void GetVector(Vector2& arg1);

    // ?GetDelimiter@datAsciiTokenizer@@UAEXPBD@Z
    ARTS_IMPORT virtual void GetDelimiter(const char* expected);

    // ?MatchInt@datAsciiTokenizer@@UAEHPBD@Z
    ARTS_IMPORT virtual i32 MatchInt(const char* expected);

    // ?MatchFloat@datAsciiTokenizer@@UAEMPBD@Z
    ARTS_IMPORT virtual f32 MatchFloat(const char* expected);

    // ?MatchVector@datAsciiTokenizer@@UAEXPBDAAVVector4@@@Z
    ARTS_IMPORT virtual void MatchVector(const char* expected, Vector4& arg2);

    // ?MatchVector@datAsciiTokenizer@@UAEXPBDAAVVector3@@@Z
    ARTS_IMPORT virtual void MatchVector(const char* expected, Vector3& arg2);

    // ?MatchVector@datAsciiTokenizer@@UAEXPBDAAVVector2@@@Z
    ARTS_IMPORT virtual void MatchVector(const char* arg1, Vector2& arg2);

    // ?Put@datAsciiTokenizer@@UAE_NABVVector4@@@Z
    ARTS_IMPORT virtual bool Put(const Vector4& arg1);

    // ?Put@datAsciiTokenizer@@UAE_NABVVector3@@@Z
    ARTS_IMPORT virtual bool Put(const Vector3& arg1);

    // ?Put@datAsciiTokenizer@@UAE_NABVVector2@@@Z
    ARTS_IMPORT virtual bool Put(const Vector2& arg1);

    // ?Put@datAsciiTokenizer@@UAE_NM@Z
    ARTS_IMPORT virtual bool Put(f32 arg1);

    // ?Put@datAsciiTokenizer@@UAE_NH@Z
    ARTS_IMPORT virtual bool Put(i32 arg1);

    // ?Put@datAsciiTokenizer@@UAE_NPBDI@Z
    ARTS_IMPORT virtual bool Put(const char* arg1, u32 arg2);

    // ?PutDelimiter@datAsciiTokenizer@@UAE_NPBD@Z
    ARTS_IMPORT virtual bool PutDelimiter(const char* arg1);
};

// ??_7datAsciiTokenizer@@6B@
// vtable at 0x005B1778
// check_size(datAsciiTokenizer, 0x1C); // size known, members are not - cannot verify
