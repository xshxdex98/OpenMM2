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
#include "stream/string.h"
#include "vector7/vector4.h"

class AudSoundBase;
class Card2D;
class MArray;
class UIIcon;
class UIMenu;
class UITextField;
class WArray;
class asCamera;
class asViewCS;
class eqEventQ;
class sfPointer;
class uiNavBar;
class uiWidget;

class MenuManager : public asNode
{
public:
    // ??0MenuManager@@QAE@XZ
    ARTS_IMPORT MenuManager();

    // ??1MenuManager@@UAE@XZ
    ARTS_IMPORT virtual ~MenuManager();

    // ?Update@MenuManager@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?ResChange@MenuManager@@UAEXHH@Z
    ARTS_IMPORT virtual void ResChange(i32 arg1, i32 arg2);

    // ?ActionID@MenuManager@@QAEHH@Z
    ARTS_IMPORT i32 ActionID(i32 arg1);

    // ?AddBrackets@MenuManager@@QAEXPAVUIIcon@@0PAVuiWidget@@MM@Z
    ARTS_IMPORT void AddBrackets(UIIcon* arg1, UIIcon* arg2, uiWidget* arg3, f32 arg4, f32 arg5);

    // ?AddMenu2@MenuManager@@QAEHPAVUIMenu@@@Z
    ARTS_IMPORT i32 AddMenu2(UIMenu* label_id);

    // ?AddPointer@MenuManager@@QAEXXZ
    ARTS_IMPORT void AddPointer();

    // ?AdjustPopupCard@MenuManager@@QAEXPAVUIMenu@@@Z
    ARTS_IMPORT void AdjustPopupCard(UIMenu* label_id);

    // ?AllocateMenuSwitchAudio@MenuManager@@QAEXXZ
    ARTS_IMPORT void AllocateMenuSwitchAudio();

    // ?CheckBG@MenuManager@@QAEXPAVUIMenu@@@Z
    ARTS_IMPORT void CheckBG(UIMenu* arg1);

    // ?CheckInput@MenuManager@@QAEXXZ
    ARTS_IMPORT void CheckInput();

    // ?ClearAllWidgets@MenuManager@@QAEXXZ
    ARTS_IMPORT void ClearAllWidgets();

    // ?CloseDialog@MenuManager@@QAEXXZ
    ARTS_IMPORT void CloseDialog();

    // ?CurrentMenuSelected@MenuManager@@QAEHXZ
    ARTS_IMPORT i32 CurrentMenuSelected();

    // ?DeclareLastDrawn@MenuManager@@QAEXPAVasNode@@@Z
    ARTS_IMPORT void DeclareLastDrawn(asNode* arg1);

    // ?DeleteMenu@MenuManager@@QAEXPAVUIMenu@@@Z
    ARTS_IMPORT void DeleteMenu(UIMenu* label_id);

    // ?Disable@MenuManager@@QAEXH@Z
    ARTS_IMPORT void Disable(i32 arg1);

    // ?DisableNavBar@MenuManager@@QAEXXZ
    ARTS_IMPORT void DisableNavBar();

    // ?DisablePU@MenuManager@@QAEXXZ
    ARTS_IMPORT void DisablePU();

    // ?Enable@MenuManager@@QAEXH@Z
    ARTS_IMPORT void Enable(i32 arg1);

    // ?EnableNavBar@MenuManager@@QAEXXZ
    ARTS_IMPORT void EnableNavBar();

    // ?EnablePU@MenuManager@@QAEXXZ
    ARTS_IMPORT void EnablePU();

    // ?FindMenu@MenuManager@@QAEHH@Z
    ARTS_IMPORT i32 FindMenu(i32 arg1);

    // ?Flush@MenuManager@@QAEXXZ
    ARTS_IMPORT void Flush();

    // ?ForceCurrentFocus@MenuManager@@QAEXXZ
    ARTS_IMPORT void ForceCurrentFocus();

    // ?GetControllerName@MenuManager@@QAEPADH@Z
    ARTS_IMPORT char* GetControllerName(i32 arg1);

    // ?GetCurrentMenu@MenuManager@@QAEPAVUIMenu@@XZ
    ARTS_IMPORT UIMenu* GetCurrentMenu();

    // ?GetFGColor@MenuManager@@QAEAAVVector4@@H@Z
    ARTS_IMPORT Vector4& GetFGColor(i32 arg1);

    // ?GetFont@MenuManager@@QAEPAXH@Z
    ARTS_IMPORT void* GetFont(i32 label_id);

    // ?GetPreviousMenu@MenuManager@@QAEHXZ
    ARTS_IMPORT i32 GetPreviousMenu();

    // ?GetScale@MenuManager@@QAEXAAM000@Z
    ARTS_IMPORT void GetScale(f32& arg1, f32& arg2, f32& arg3, f32& arg4);

    // ?Help@MenuManager@@QAEXXZ
    ARTS_IMPORT void Help();

    // ?Init@MenuManager@@QAEXHHPAD@Z
    ARTS_IMPORT void Init(i32 arg1, i32 arg2, char* arg3);

    // ?Init@MenuManager@@QAEXPAVasCamera@@HHMMMM@Z
    ARTS_IMPORT void Init(asCamera* arg1, i32 arg2, i32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7);

    // ?InitCommonStuff@MenuManager@@QAEXHH@Z
    ARTS_IMPORT void InitCommonStuff(i32 arg1, i32 arg2);

    // ?InitGlobalStrings@MenuManager@@QAEXXZ
    ARTS_IMPORT void InitGlobalStrings();

    // ?Kill@MenuManager@@QAEXXZ
    ARTS_IMPORT void Kill();

    // ?LoadRaceNames@MenuManager@@QAEXXZ
    ARTS_IMPORT void LoadRaceNames();

    // ?MenuState@MenuManager@@QAEHH@Z
    ARTS_IMPORT i32 MenuState(i32 arg1);

    // ?MouseAction@MenuManager@@QAEPAVuiWidget@@HMM@Z
    ARTS_IMPORT uiWidget* MouseAction(i32 type, f32 x, f32 y);

    // ?NotifyMouseSelect@MenuManager@@QAEXPAVUIMenu@@@Z
    ARTS_IMPORT void NotifyMouseSelect(UIMenu* label_id);

    // ?OpenDialog@MenuManager@@QAEXH@Z
    ARTS_IMPORT void OpenDialog(i32 arg1);

    // ?PlayRecordsSound@MenuManager@@QAEXXZ
    ARTS_IMPORT void PlayRecordsSound();

    // ?PlayReplaySound@MenuManager@@QAEXXZ
    ARTS_IMPORT void PlayReplaySound();

    // ?PlaySound@MenuManager@@QAEXH@Z
    ARTS_IMPORT void PlaySound(i32 arg1);

    // ?RegisterWidgetFocus@MenuManager@@QAEXHMMMMPAVuiWidget@@@Z
    ARTS_IMPORT void RegisterWidgetFocus(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, uiWidget* arg6);

    // ?ScanGlobalKeys@MenuManager@@QAEHH@Z
    ARTS_IMPORT i32 ScanGlobalKeys(i32 arg1);

    // ?SetBackgroundImage@MenuManager@@QAEXPAD@Z
    ARTS_IMPORT void SetBackgroundImage(char* arg1);

    // ?SetDefaultBackgroundImage@MenuManager@@QAEXPAD@Z
    ARTS_IMPORT void SetDefaultBackgroundImage(char* arg1);

    // ?SetFocus@MenuManager@@QAEXPAVUIMenu@@@Z
    ARTS_IMPORT void SetFocus(UIMenu* arg1);

    // ?SetPreviousMenu@MenuManager@@QAEXH@Z
    ARTS_IMPORT void SetPreviousMenu(i32 arg1);

    // ?Switch@MenuManager@@QAEHH@Z
    ARTS_IMPORT i32 Switch(i32 label_id);

    // ?SwitchFocus@MenuManager@@QAEXPAVUIMenu@@@Z
    ARTS_IMPORT void SwitchFocus(UIMenu* arg1);

    // ?ToggleFocus@MenuManager@@QAEXH@Z
    ARTS_IMPORT void ToggleFocus(i32 arg1);

    // ?TogglePU@MenuManager@@QAEXXZ
    ARTS_IMPORT void TogglePU();

    // ?Instance@MenuManager@@2PAV1@A
    ARTS_IMPORT static MenuManager* Instance;

private:
    // ?PlayMenuSwitchSound@MenuManager@@AAEXXZ
    ARTS_IMPORT void PlayMenuSwitchSound();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    eqEventQ* pEventQ; // 0x018
    asCamera* pCamera; // 0x01C
    asViewCS* pViewCS; // 0x020
    uiNavBar* pNavBar; // 0x024
    u32 field_28; // 0x028
    u32 field_2C; // 0x02C
    u32 FGColor; // 0x030
    u32 PUEnabled; // 0x034
    uiWidget* pFocusedWidget; // 0x038
    uiWidget* pCurrentWidget; // 0x03C
    UITextField* CurrentIMEText; // 0x040
    WArray* pWidgets; // 0x044
    MArray* pMenus; // 0x048
    string Controller; // 0x04C
    string Transmission; // 0x054
    string ObjectDetail; // 0x05C
    string SoundQuality; // 0x064
    string CloudShadows; // 0x06C
    string CheckpointRaces; // 0x074
    string RaceType; // 0x07C
    string string84; // 0x084
    string string8C; // 0x08C
    string CircuitRaces; // 0x094
    string BlitzRaces; // 0x09C
    string SomeDetailLevel; // 0x0A4
    string CurrentCrashCourses; // 0x0AC
    string CabbieRaces; // 0x0B4
    string StuntRaces; // 0x0BC
    Vector4 Scale; // 0x0C4
    sfPointer* pPointer; // 0x0D4
    u32 DefaultFont; // 0x0D8
    u32 field_DC; // 0x0DC
    u32 field_E0; // 0x0E0
    f32 ScaleX; // 0x0E4
    f32 ScaleY; // 0x0E8
    UIMenu* CurrentDialog; // 0x0EC
    UIMenu* PrevMenu; // 0x0F0
    u32 field_F4; // 0x0F4
    UIMenu** ppMenus; // 0x0F8
    UIMenu* FocusedMenu; // 0x0FC
    Card2D* pCard; // 0x100
    u32 field_104; // 0x104
    u32 nMaxMenus; // 0x108
    u32 nMenuCount; // 0x10C
    u32 field_110; // 0x110
    u32 field_114; // 0x114
    void* hfont118; // 0x118
    void* hfont11C; // 0x11C
    void* hfont120; // 0x120
    void* hfont124; // 0x124
    void* hfont128; // 0x128
    void* hfont12C; // 0x12C
    void* hfont130; // 0x130
    void* hfont134; // 0x134
    u32 CurrentMenu; // 0x138
    AudSoundBase* MoveSound; // 0x13C
    AudSoundBase* SelectionSound; // 0x140
    AudSoundBase* SwitchSound; // 0x144
    AudSoundBase* ReplaySound; // 0x148
    i8* DefaultBackground; // 0x14C
};

// ??_7MenuManager@@6B@
// vtable at 0x005B3288
check_size(MenuManager, 0x150);
