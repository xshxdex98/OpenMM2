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

#include "data7/datcallback.h"
#include "stream/string.h"

#include "uimenu.h"

struct LocString;
struct NETCOMMPACK;
class UITextScroll;
class uiWidget;

class NetSelectMenu : public UIMenu
{
public:
    // ??0NetSelectMenu@@QAE@H@Z
    ARTS_IMPORT NetSelectMenu(i32 arg1);

    // ??1NetSelectMenu@@UAE@XZ
    ARTS_IMPORT virtual ~NetSelectMenu();

    // ?PreSetup@NetSelectMenu@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?PostSetup@NetSelectMenu@@UAEXXZ
    ARTS_IMPORT virtual void PostSetup();

    // ?AddModem@NetSelectMenu@@QAEXPADH@Z
    ARTS_IMPORT void AddModem(char* arg1, i32 arg2);

    // ?AddWidgetToList@NetSelectMenu@@QAEXHPAVuiWidget@@@Z
    ARTS_IMPORT void AddWidgetToList(i32 arg1, uiWidget* arg2);

    // ?BuildComs@NetSelectMenu@@QAEXXZ
    ARTS_IMPORT void BuildComs();

    // ?ClearProtocol@NetSelectMenu@@QAEXXZ
    ARTS_IMPORT void ClearProtocol();

    // ?DisableSessions@NetSelectMenu@@QAEXXZ
    ARTS_IMPORT void DisableSessions();

    // ?EnableSearchLabel@NetSelectMenu@@QAEX_N@Z
    ARTS_IMPORT void EnableSearchLabel(bool arg1);

    // ?FocusDescription@NetSelectMenu@@QAEXHH@Z
    ARTS_IMPORT void FocusDescription(i32 arg1, i32 arg2);

    // ?GetCommPack@NetSelectMenu@@QAEPAUNETCOMMPACK@@XZ
    ARTS_IMPORT NETCOMMPACK* GetCommPack();

    // ?GetHostJoin@NetSelectMenu@@QAEHXZ
    ARTS_IMPORT i32 GetHostJoin();

    // ?GetSessionID@NetSelectMenu@@QAEHXZ
    ARTS_IMPORT i32 GetSessionID();

    // ?GetSessions@NetSelectMenu@@QAEPAVUITextScroll@@XZ
    ARTS_IMPORT UITextScroll* GetSessions();

    // ?HostCB@NetSelectMenu@@QAEXXZ
    ARTS_IMPORT void HostCB();

    // ?IPAddressCallback@NetSelectMenu@@QAEXXZ
    ARTS_IMPORT void IPAddressCallback();

    // ?JoinCallback@NetSelectMenu@@QAEXXZ
    ARTS_IMPORT void JoinCallback();

    // ?JoinCB@NetSelectMenu@@QAEXXZ
    ARTS_IMPORT void JoinCB();

    // ?NetNameCB@NetSelectMenu@@QAEXXZ
    ARTS_IMPORT void NetNameCB();

    // ?ProtocolBack@NetSelectMenu@@QAEXXZ
    ARTS_IMPORT void ProtocolBack();

    // ?ReparentWidgets@NetSelectMenu@@QAEXH@Z
    ARTS_IMPORT void ReparentWidgets(i32 arg1);

    // ?SetComs@NetSelectMenu@@QAEXXZ
    ARTS_IMPORT void SetComs();

    // ?SetDescription@NetSelectMenu@@QAEXPAULocString@@@Z
    ARTS_IMPORT void SetDescription(LocString* arg1);

    // ?SetIPAddress@NetSelectMenu@@QAEXPAD@Z
    ARTS_IMPORT void SetIPAddress(char* arg1);

    // ?SetIPXButton@NetSelectMenu@@QAEXH@Z
    ARTS_IMPORT void SetIPXButton(i32 arg1);

    // ?SetModemButton@NetSelectMenu@@QAEXH@Z
    ARTS_IMPORT void SetModemButton(i32 arg1);

    // ?SetNetname@NetSelectMenu@@QAEXPAD@Z
    ARTS_IMPORT void SetNetname(char* arg1);

    // ?SetPhoneNumber@NetSelectMenu@@QAEXPAD@Z
    ARTS_IMPORT void SetPhoneNumber(char* arg1);

    // ?SetSerialButton@NetSelectMenu@@QAEXH@Z
    ARTS_IMPORT void SetSerialButton(i32 arg1);

    // ?SetSession@NetSelectMenu@@QAEXPADH@Z
    ARTS_IMPORT void SetSession(char* arg1, i32 arg2);

    // ?SetTCPLocalButton@NetSelectMenu@@QAEXH@Z
    ARTS_IMPORT void SetTCPLocalButton(i32 arg1);

    // ?SetTCPNetButton@NetSelectMenu@@QAEXH@Z
    ARTS_IMPORT void SetTCPNetButton(i32 arg1);

    // ?ShowTCPIPNetSessions@NetSelectMenu@@QAEXXZ
    ARTS_IMPORT void ShowTCPIPNetSessions();

    // ?WidgetSwitch@NetSelectMenu@@QAEXHH@Z
    ARTS_IMPORT void WidgetSwitch(i32 arg1, i32 arg2);

public:
    // Members from 0x08C; everything below that belongs to UIMenu.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8 PlayerNetName[40]; // 0x08C
    i8 field_B4[40]; // 0x0B4
    datCallback field_DC; // 0x0DC
    datCallback field_E8; // 0x0E8
    datCallback field_F4; // 0x0F4
    i32 field_100; // 0x100
    string field_104; // 0x104
    string field_10C; // 0x10C
    i32 field_114; // 0x114
    i32 field_118; // 0x118
    i32 field_11C; // 0x11C
    i32 field_120; // 0x120
    i32 field_124; // 0x124
    i32 field_128; // 0x128
    i32 field_12C; // 0x12C
    i32 field_130; // 0x130
    i8 field_134[56]; // 0x134
    i32 field_16C; // 0x16C
    i32 field_170; // 0x170
    i32 field_174; // 0x174
    i32 field_178; // 0x178
    i32 field_17C; // 0x17C
    i32 field_180; // 0x180
    i32 field_184; // 0x184
    i32 field_188; // 0x188
    i32 field_18C; // 0x18C
    i32 field_190; // 0x190
    i32 field_194; // 0x194
    i32 field_198; // 0x198
    i32 field_19C; // 0x19C
    i32 field_1A0; // 0x1A0
    i32 field_1A4; // 0x1A4
    i8 field_1A8[80]; // 0x1A8
    i32 field_1F8; // 0x1F8
    i32 field_1FC; // 0x1FC
    i32 field_200; // 0x200
    i32 field_204; // 0x204
    i32 field_208; // 0x208
    i32 field_20C; // 0x20C
    i8 field_210[40]; // 0x210
    i8 field_238[40]; // 0x238
    i32 field_260; // 0x260
    i32 field_264; // 0x264
};

// ??_7NetSelectMenu@@6B@
// vtable at 0x005B445C
check_size(NetSelectMenu, 0x268);
