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

class datBinTokenizer : public datBaseTokenizer
{
public:
    // ?GetInt@datBinTokenizer@@UAEHXZ
    ARTS_IMPORT virtual i32 GetInt();

    // ?GetFloat@datBinTokenizer@@UAEMXZ
    ARTS_IMPORT virtual f32 GetFloat();

    // ?GetVector@datBinTokenizer@@UAEXAAVVector4@@@Z
    ARTS_IMPORT virtual void GetVector(Vector4& stream);

    // ?GetVector@datBinTokenizer@@UAEXAAVVector3@@@Z
    ARTS_IMPORT virtual void GetVector(Vector3& arg1);

    // ?GetVector@datBinTokenizer@@UAEXAAVVector2@@@Z
    ARTS_IMPORT virtual void GetVector(Vector2& arg1);

    // ?GetDelimiter@datBinTokenizer@@UAEXPBD@Z
    ARTS_IMPORT virtual void GetDelimiter(const char* arg1);

    // ?MatchInt@datBinTokenizer@@UAEHPBD@Z
    ARTS_IMPORT virtual i32 MatchInt(const char* arg1);

    // ?MatchFloat@datBinTokenizer@@UAEMPBD@Z
    ARTS_IMPORT virtual f32 MatchFloat(const char* arg1);

    // ?MatchVector@datBinTokenizer@@UAEXPBDAAVVector4@@@Z
    ARTS_IMPORT virtual void MatchVector(const char* arg1, Vector4& arg2);

    // ?MatchVector@datBinTokenizer@@UAEXPBDAAVVector3@@@Z
    ARTS_IMPORT virtual void MatchVector(const char* arg1, Vector3& arg2);

    // ?MatchVector@datBinTokenizer@@UAEXPBDAAVVector2@@@Z
    ARTS_IMPORT virtual void MatchVector(const char* arg1, Vector2& arg2);

    // ?Put@datBinTokenizer@@UAE_NABVVector4@@@Z
    ARTS_IMPORT virtual bool Put(const Vector4& lpData);

    // ?Put@datBinTokenizer@@UAE_NABVVector3@@@Z
    ARTS_IMPORT virtual bool Put(const Vector3& arg1);

    // ?Put@datBinTokenizer@@UAE_NABVVector2@@@Z
    ARTS_IMPORT virtual bool Put(const Vector2& arg1);

    // ?Put@datBinTokenizer@@UAE_NM@Z
    ARTS_IMPORT virtual bool Put(f32 Data);

    // ?Put@datBinTokenizer@@UAE_NH@Z
    ARTS_IMPORT virtual bool Put(i32 arg1);

    // ?Put@datBinTokenizer@@UAE_NPBDI@Z
    ARTS_IMPORT virtual bool Put(const char* lpData, u32 arg2);

    // ?PutDelimiter@datBinTokenizer@@UAE_NPBD@Z
    ARTS_IMPORT virtual bool PutDelimiter(const char* arg1);
};

// ??_7datBinTokenizer@@6B@
// vtable at 0x005B2B74
// check_size(datBinTokenizer, 0x1C); // size known, members are not - cannot verify
