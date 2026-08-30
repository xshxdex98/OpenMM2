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

#include "arts7/asnode.h"
#include "data7/datcallback.h"
#include "stream/string.h"
#include "vector7/vector2.h"

struct LocString;
class UIBMButton;
class UIBMLabel;
class UIButton;
class UICWArray;
class UICompositeScroll;
class UIControlWidget;
class UIIcon;
class UIIconW;
class UILabel;
class UIMexButton;
class UISlider;
class UITextDropdown;
class UITextField;
class UITextRoller;
class UITextRoller2;
class UITextScroll;
class UIToggleButton;
class UIToggleButton2;
class UIVScrollBar;
union eqEvent;
class mmIO;
class uiWidget;

class UIMenu : public asNode
{
public:
    enum eSource : i32;

    // ??0UIMenu@@QAE@H@Z
    ARTS_IMPORT UIMenu(i32 menuID);

    // ??1UIMenu@@UAE@XZ
    ARTS_IMPORT virtual ~UIMenu();

    // ?Update@UIMenu@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?PreSetup@UIMenu@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?PostSetup@UIMenu@@UAEXXZ
    ARTS_IMPORT virtual void PostSetup();

    // ?BackUp@UIMenu@@UAEXXZ
    ARTS_IMPORT virtual void BackUp();

    // ?CheckInput@UIMenu@@UAEXXZ
    ARTS_IMPORT virtual void CheckInput();

    // ?IsAnOptionMenu@UIMenu@@UAEHXZ
    ARTS_IMPORT virtual i32 IsAnOptionMenu();

    // ?AddBMButton@UIMenu@@QAEPAVUIBMButton@@HPADMMHVdatCallback@@PAHHH1@Z
    ARTS_IMPORT UIBMButton* AddBMButton(i32 arg1, char* arg2, f32 arg3, f32 arg4, i32 arg5, datCallback arg6, i32* arg7, i32 arg8, i32 arg9, datCallback arg10);

    // ?AddBMLabel@UIMenu@@QAEPAVUIBMLabel@@HPADPAVstring@@MMPAH@Z
    ARTS_IMPORT UIBMLabel* AddBMLabel(i32 arg1, char* arg2, string* arg3, f32 arg4, f32 arg5, i32* arg6);

    // ?AddButton@UIMenu@@QAEPAVUIButton@@HPAULocString@@MMMMHHVdatCallback@@H@Z
    ARTS_IMPORT UIButton* AddButton(i32 arg1, LocString* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, i32 arg7, i32 arg8, datCallback arg9, i32 arg10);

    // ?AddCompScroll@UIMenu@@QAEPAVUICompositeScroll@@HMMMMMHPAHHHVdatCallback@@@Z
    ARTS_IMPORT UICompositeScroll* AddCompScroll(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, i32 arg7, i32* arg8, i32 arg9, i32 arg10, datCallback arg11);

    // ?AddCWArray@UIMenu@@QAEPAVUICWArray@@HMMMMHVdatCallback@@@Z
    ARTS_IMPORT UICWArray* AddCWArray(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, i32 arg6, datCallback arg7);

    // ?AddHotSpot@UIMenu@@QAEPAVuiWidget@@HPADMMMMVdatCallback@@@Z
    ARTS_IMPORT uiWidget* AddHotSpot(i32 arg1, char* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, datCallback arg7);

    // ?AddIcon@UIMenu@@QAEPAVUIIcon@@HPADMM@Z
    ARTS_IMPORT UIIcon* AddIcon(i32 arg1, char* tooltip, f32 x, f32 y);

    // ?AddIconW@UIMenu@@QAEPAVUIIconW@@HPAULocString@@PADMMMMVdatCallback@@@Z
    ARTS_IMPORT UIIconW* AddIconW(i32 arg1, LocString* arg2, char* arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, datCallback arg8);

    // ?AddLabel@UIMenu@@QAEPAVUILabel@@HPAULocString@@MMMMHH@Z
    ARTS_IMPORT UILabel* AddLabel(i32 arg1, LocString* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, i32 arg7, i32 arg8);

    // ?AddMex@UIMenu@@QAEPAVUIMexButton@@HPAULocString@@PAHHMMMMHHVdatCallback@@@Z
    ARTS_IMPORT UIMexButton* AddMex(i32 arg1, LocString* arg2, i32* arg3, i32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, i32 arg9, i32 arg10, datCallback arg11);

    // ?AddSlider@UIMenu@@QAEPAVUISlider@@HPAULocString@@PAMMMMMMMHHHHVdatCallback@@2@Z
    ARTS_IMPORT UISlider* AddSlider(i32 arg1, LocString* arg2, f32* arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9, i32 arg10, i32 arg11, i32 arg12, i32 arg13, datCallback arg14, datCallback arg15);

    // ?AddTextDropdown@UIMenu@@QAEPAVUITextDropdown@@HPAULocString@@PAHMMMMVstring@@HHHVdatCallback@@PAD3@Z
    ARTS_IMPORT UITextDropdown* AddTextDropdown(i32 arg1, LocString* arg2, i32* arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, string arg8, i32 arg9, i32 arg10, i32 arg11, datCallback arg12, char* arg13, datCallback arg14);

    // ?AddTextField@UIMenu@@QAEPAVUITextField@@HPAULocString@@PADMMMMHHHHHVdatCallback@@@Z
    ARTS_IMPORT UITextField* AddTextField(i32 arg1, LocString* arg2, char* arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, i32 arg8, i32 arg9, i32 arg10, i32 arg11, i32 arg12, datCallback arg13);

    // ?AddTextRoller@UIMenu@@QAEPAVUITextRoller@@HPAULocString@@PAHMMMMVstring@@HHHHVdatCallback@@@Z
    ARTS_IMPORT UITextRoller* AddTextRoller(i32 arg1, LocString* arg2, i32* arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, string arg8, i32 arg9, i32 arg10, i32 arg11, i32 arg12, datCallback arg13);

    // ?AddTextRoller2@UIMenu@@QAEPAVUITextRoller2@@HPAULocString@@PAHMMMMVstring@@HHHHVdatCallback@@@Z
    ARTS_IMPORT UITextRoller2* AddTextRoller2(i32 arg1, LocString* arg2, i32* arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, string arg8, i32 arg9, i32 arg10, i32 arg11, i32 arg12, datCallback arg13);

    // ?AddTextScroll@UIMenu@@QAEPAVUITextScroll@@HPAXMMMMVstring@@HPAHVdatCallback@@@Z
    ARTS_IMPORT UITextScroll* AddTextScroll(i32 arg1, void* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, string arg7, i32 arg8, i32* arg9, datCallback arg10);

    // ?AddToggle@UIMenu@@QAEPAVUIToggleButton@@HPAULocString@@PAHMMMMHHVdatCallback@@@Z
    ARTS_IMPORT UIToggleButton* AddToggle(i32 arg1, LocString* arg2, i32* arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, i32 arg8, i32 arg9, datCallback arg10);

    // ?AddToggle2@UIMenu@@QAEPAVUIToggleButton2@@HPAULocString@@PAHMMMMHHVdatCallback@@@Z
    ARTS_IMPORT UIToggleButton2* AddToggle2(i32 arg1, LocString* arg2, i32* arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, i32 arg8, i32 arg9, datCallback arg10);

    // ?AddUIControl@UIMenu@@QAEPAVUIControlWidget@@HMMMMMPAVmmIO@@VdatCallback@@@Z
    ARTS_IMPORT UIControlWidget* AddUIControl(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, mmIO* arg7, datCallback arg8);

    // ?AddVScrollBar@UIMenu@@QAEPAVUIVScrollBar@@HPAHMMMMMMHHVdatCallback@@@Z
    ARTS_IMPORT UIVScrollBar* AddVScrollBar(i32 arg1, i32* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, i32 arg9, i32 arg10, datCallback arg11);

    // ?AddWidget@UIMenu@@QAEXPAVuiWidget@@PADMMMMH1@Z
    ARTS_IMPORT void AddWidget(uiWidget* arg1, char* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, i32 arg7, char* arg8);

    // ?AssignBackground@UIMenu@@QAEXPAD@Z
    ARTS_IMPORT void AssignBackground(char* arg1);

    // ?AssignName@UIMenu@@QAEXPAULocString@@@Z
    ARTS_IMPORT void AssignName(LocString* arg1);

    // ?CheckMouseHits@UIMenu@@QAEXXZ
    ARTS_IMPORT void CheckMouseHits();

    // ?ClearAction@UIMenu@@QAEXXZ
    ARTS_EXPORT void ClearAction();

    // ?ClearSelected@UIMenu@@QAEXXZ
    ARTS_IMPORT void ClearSelected();

    // ?ClearToolTip@UIMenu@@QAEXXZ
    ARTS_IMPORT void ClearToolTip();

    // ?ClearWidgets@UIMenu@@QAEXXZ
    ARTS_IMPORT void ClearWidgets();

    // ?Decrement@UIMenu@@QAEHXZ
    ARTS_IMPORT i32 Decrement();

    // ?Disable@UIMenu@@QAEXXZ
    ARTS_IMPORT void Disable();

    // ?DisableIME@UIMenu@@QAEXXZ
    ARTS_IMPORT void DisableIME();

    // ?Enable@UIMenu@@QAEXXZ
    ARTS_IMPORT void Enable();

    // ?FindTheFirstFocusWidget@UIMenu@@QAEHXZ
    ARTS_IMPORT i32 FindTheFirstFocusWidget();

    // ?ForceWidgetAction@UIMenu@@QAEXH@Z
    ARTS_IMPORT void ForceWidgetAction(i32 arg1);

    // ?GetBstate@UIMenu@@QAEHXZ
    ARTS_EXPORT i32 GetBstate();

    // ?GetDimensions@UIMenu@@QAEXAAM000@Z
    ARTS_IMPORT void GetDimensions(f32& arg1, f32& arg2, f32& arg3, f32& arg4);

    // ?GetWidgetID@UIMenu@@QAEHXZ
    ARTS_EXPORT i32 GetWidgetID();

    // ?Increment@UIMenu@@QAEHXZ
    ARTS_IMPORT i32 Increment();

    // ?KeyboardAction@UIMenu@@QAEXTeqEvent@@@Z
    ARTS_IMPORT void KeyboardAction(eqEvent arg1);

    // ?MouseAction@UIMenu@@QAEXTeqEvent@@@Z
    ARTS_IMPORT void MouseAction(eqEvent arg1);

    // ?MouseHitCheck@UIMenu@@QAEPAVuiWidget@@HMM@Z
    ARTS_IMPORT uiWidget* MouseHitCheck(i32 arg1, f32 arg2, f32 arg3);

    // ?ScaleWidget@UIMenu@@QAEXAAM000@Z
    ARTS_IMPORT void ScaleWidget(f32& x, f32& y, f32& w, f32& h);

    // ?ScanInput@UIMenu@@QAEHPATeqEvent@@@Z
    ARTS_IMPORT i32 ScanInput(eqEvent* arg1);

    // ?SetAction@UIMenu@@QAEXW4eSource@1@@Z
    ARTS_IMPORT void SetAction(UIMenu::eSource arg1);

    // ?SetBstate@UIMenu@@QAEXH@Z
    ARTS_IMPORT void SetBstate(i32 arg1);

    // ?SetFocusWidget@UIMenu@@QAEXH@Z
    ARTS_IMPORT void SetFocusWidget(i32 arg1);

    // ?SetSelected@UIMenu@@QAEXXZ
    ARTS_IMPORT void SetSelected();

    // ?UI_LEFT_MARGIN@UIMenu@@2MA
    ARTS_IMPORT static f32 UI_LEFT_MARGIN;

    // ?UI_LEFT_MARGIN2@UIMenu@@2MA
    ARTS_IMPORT static f32 UI_LEFT_MARGIN2;

    // ?UI_TOP_MARGIN@UIMenu@@2MA
    ARTS_IMPORT static f32 UI_TOP_MARGIN;

    // ?UI_BOTTOM_MARGIN@UIMenu@@2MA
    ARTS_IMPORT static f32 UI_BOTTOM_MARGIN;

    // ?WIDGET_WIDTH@UIMenu@@2MA
    ARTS_IMPORT static f32 WIDGET_WIDTH;

    // ?WIDGET_HEIGHT@UIMenu@@2MA
    ARTS_IMPORT static f32 WIDGET_HEIGHT;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 ActionSource; // 0x018
    u32 ActionState; // 0x01C
    u32 field_20; // 0x020
    u32 MenuID; // 0x024
    u32 field_28; // 0x028
    u32 WidgetCount; // 0x02C
    u32 field_30; // 0x030
    uiWidget** ppWidgets; // 0x034
    i8* Name; // 0x038
    u32 field_3C; // 0x03C
    u32 field_40; // 0x040
    u32 field_44; // 0x044
    Vector2 Position; // 0x048
    Vector2 Scale; // 0x050
    u32 field_58; // 0x058
    f32 ScaleX; // 0x05C
    f32 ScaleY; // 0x060
    u32 field_64; // 0x064
    u32* pCurrentWidgetID; // 0x068
    u32 ActiveWidgetID; // 0x06C
    u32 field_70; // 0x070
    u32 WidgetID; // 0x074
    u32 field_78; // 0x078
    u32 field_7C; // 0x07C
    f32 field_80; // 0x080
    u32 field_84; // 0x084
    i8* Background; // 0x088
};

// ??_7UIMenu@@6B@
// vtable at 0x005B3234
check_size(UIMenu, 0x8C);
