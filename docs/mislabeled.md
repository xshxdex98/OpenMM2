# The 349 mislabeled functions, re-identified

`MM2_RE_KIT/MM2_PSEUDOCODE/_MISLABELED_FUNCTIONS.c` holds 349 functions whose IDB name is
junk. This document explains where the junk came from, how the real names were recovered, and
lists every one of the 349 with the evidence behind its new name.

Machine-readable results: [`data/recovered_names.json`](../data/recovered_names.json).

| | |
|---|---:|
| Mislabeled functions | 349 |
| **CONFIRMED** | **346** |
| PROBABLE | 1 |
| GUESS | 1 |
| Still unidentified | **1** |

Of the 349: 286 are real MM2 functions now named from the shipped linker map, and 63 are
compiler-generated artifacts (dynamic initializers, atexit destructors, EH funclets, and IDA
chunk-splits) that were never member functions at all. The 286 real functions span 150 classes
plus 21 free functions, and include 61 virtual methods.

## 1. Where the wrong names came from

The community IDB was annotated over many years, and at some point somebody loaded a symbol
file for **MM2Hook** - the ImGui-based mod loader - over part of the database. MM2Hook is a
separate DLL that injects into `midtown2.exe`; its symbols describe *its* code, not the game's.
Applied to game addresses they are pure noise.

The smoking gun sits in the game folder next to the binary this kit describes:

```
MM2Hook.pdb      56 MB   <- the source of the bad names
mm2hook.ini
mm2hook.log
imgui.ini        <- ImGui's own layout file, written by MM2Hook, not by the game
midtown2.exe
midtown2.map     <- the shipped linker map. This is what fixes everything.
```

`midtown2.exe` contains no ImGui code and not one ImGui string. Every name of the form
`ImGui::*`, `ImPlot::*`, `ImVector<T>::*`, `LuaIntf::*` or `std::*` in the IDB is a
misattribution. The **code** under those names is real Midtown Madness 2; only the label is junk.

Two consequences worth knowing:

* The contamination is **wider than the 349**. `_INDEX.csv`, the supposedly clean index, also
  carries `ImVector`, `ImRect`, `hook::`, `MM2::`, `UIMenu`-namespaced and `HookConfig` names on
  real game code. The quarantine list caught the obvious ImGui/ImPlot cases, not all of them.
* The 349 are not a random sample. They cluster in `mmInterface`, `mmInput`, `aiMap`, the audio
  classes and the CRT initializer chain - wherever MM2Hook happened to have symbols at nearby
  addresses.

## 2. The recovery method

### The shipped linker map beats every heuristic

`midtown2.map` shipped with the retail game. It is the MSVC linker's own output: 10,608 public
symbols in address order, each with its `Rva+Base`, each still mangled - and a mangled MSVC name
encodes access, virtualness, calling convention, return type and every parameter. This project
already parses it into `data/symbols.json` (see `tools/symbols.py`).

So the recovery is not inference at all for most of the 349: it is a **lookup**. If a mislabeled
function starts at address A and the linker map has a symbol at exactly A, that symbol is the
function's real name, straight from the tool that produced the binary.

### Establishing that the map matches this binary

A map from the wrong build would be worse than no map, so this was checked four ways before any
name was accepted:

1. **Binary identity.** `midtown2.exe` in the game folder is 1,966,080 bytes, md5
   `e8e0da2bc2695968ecb0a8ef65ac3c67` - the exact md5 `MM2_RE_KIT/README.md` section 3 names as
   byte-identical to the IDB at 489 of 489 sampled entry points. The map's header agrees:
   load address `00400000`, `.text` `0001:00001000` length `001AF000`.
2. **Bulk name agreement.** For the 6,526 IDB functions that have a genuine (non-foreign) name
   *and* an exact address in the map, the map agrees on the class **96.8%** of the time and on
   the method name **92.8%** of the time. Nearly every remaining disagreement is another piece of
   MM2Hook contamination in the IDB - i.e. the map is right and the IDB is wrong.
3. **Function sizes.** For 283 of the mislabeled functions IDA knows a size. **273** of them are
   identical to, or within 16 bytes of (alignment padding), the map-implied size. The other 10
   are all cases where IDA split one function into chunks.
4. **Vtables, read from the binary itself.** 50 of the mislabeled addresses appear inside a
   vftable. The map independently marks the symbol at each of those 50 addresses `virtual`.
   And the converse holds with zero exceptions: **no** address the map calls non-virtual turns
   up in any vftable. Two unrelated sources, no contradictions.

### One caveat about the map file itself

The copy of `midtown2.map` in the game folder has been reformatted at some point - the
`Lib:Object` column reads `<unknown>` on every line - and about 71 of its 10,608 entries carry
IDA-style placeholder names (`nullsub_245`, `j_nullsub_228`, `unknown_libname_*`) where somebody
filled a gap. That does **not** make it IDB-derived and it does not make it circular with the
thing it is being used to fix: the map contains **zero** `ImGui`, `ImPlot`, `ImVector`, `LuaIntf`
or `hook::` names, and **zero** `sub_XXXXXX` names, while carrying 7,264 genuine MSVC-mangled
C++ symbols. The four checks above are what license its use, not its filename.

It does mean one thing for this exercise: where the map's entry is itself a placeholder, no name
has been recovered. That happens exactly once, at `0x005418C0`, whose body is empty (`{ ; }`).
That entry is marked `UNIDENTIFIED`.

### The rest: things that were never functions

A linker map lists *publics only*. Static functions and compiler-generated helpers are absent.
63 of the 349 have no symbol at their exact address, and every one of them turned out to be an
artifact rather than a game function:

| what it is | how many | how it was proved |
|---|--:|---|
| C++ dynamic initializer | 45 | address is reached from the CRT initializer table at `.data:0x005C2004` (548 entries, walked by `__initterm` at startup) |
| atexit destructor for a file-scope object | 3 | same CRT table, plus the MSVC guard-byte pattern `if ((guard & 1) == 0) { guard \|= 1; ~T(&global); }` |
| IDA chunk-split of a larger routine | 9 | address falls strictly inside another map symbol's extent; body continues that function |
| C++ EH funclet (`__ehhandler` / `__unwindfunclet`) | 4 | address is in the `_SEH` funclet region at the end of `.text`, body is a bare `__CxxFrameHandler` call |
| genuine file-local `static` | 2 | real call target, but no public symbol |

The dynamic initializers are the ones that look most like real methods and are the easiest to
mis-name. `0x00479830` for example is decompiled as `ImGui::ListBoxHeader` and its body is:

```c
phBoundBox::EdgeNormals[12].x = 1.0;
phBoundBox::EdgeNormals[12].y = 0.0;
...
JUMPOUT(0x4798C3);
```

That is the static initializer for `phBoundBox::EdgeNormals`, which IDA split into five separate
"functions". It is not a member of anything.

## 3. Why the nine earlier recoveries need revising

`MM2_RE_KIT/README.md` section 5 lists nine names recovered by reading each function's own debug
strings. The map confirms **three** of them exactly and overturns **six**. All nine are in
`recovered_names.json`, flagged `previously_known`.

| address | earlier recovery | shipped map says | verdict |
|---|---|---|---|
| `0x0040FF80` | `mmInterface::InitNetwork` | `mmInterface::InitNetwork` | confirmed |
| `0x004A47B0` | `modPackage::OpenFile` | `modPackage::OpenFile` | confirmed |
| `0x00533660` | `asMeshSetForm::SetShape` | `asMeshSetForm::SetShape` | confirmed |
| `0x00517A00` | `IDirectMusicPerformance::IsPlaying` | `SegmentWrapper::IsPlaying` | **wrong** |
| `0x00531940` | `mmRoadFF::CreateEffect` | `mmRoadFF::Init` | **wrong** |
| `0x00531EC0` | `mmSpringFF::CreateEffect` | `mmSpringFF::Init` | **wrong** |
| `0x00571700` | `asNetwork::UnSealing` | `asNetwork::UnSealSession` | **wrong** |
| `0x00572B00` | `IZoneScore::SendFinalScore` | `netZoneScore::SendGameEnd` | **wrong** |
| `0x005A7590` | `SoundObj::IsPlaying` | `EffectBase::OriginalBufferPlaying` | **wrong** |

None of `SoundObj`, `mmRoadFF::CreateEffect`, `mmSpringFF::CreateEffect`, `asNetwork::UnSealing`
or `IZoneScore::SendFinalScore` exists anywhere in the 10,608-symbol map. They were never real
names. The failure mode is instructive, because it is the failure mode of the whole
read-the-debug-string technique:

* **The string names the callee, not the caller.** `0x00517A00` is
  `ErrorDisplay("SegmentWrapper::IsPlaying", "IDirectMusicPerformance::IsPlaying", hr)` - argument
  one is the function reporting the error, argument two is the COM method that failed. The
  earlier pass took argument two.
* **The string names something the function called and that failed.** `0x00531940` prints
  `"mmRoadFF::CreateEffect failed - %08Xh"`. It is `mmRoadFF::Init` *reporting* that
  `CreateEffect` failed, and the map's signature
  `virtual int __thiscall mmRoadFF::Init(struct IDirectInputDevice2A *)` matches the body, which
  enumerates DirectInput effect types. Same for `mmSpringFF`.
* **The string names a later step in the same function.** `0x00572B00` calls
  `SendFinalScore` and then `SendGameState(..., 2)`, and warns `"IZoneScore::SendGameEnd failed."`.
  It *is* `SendGameEnd`; `SendFinalScore` is just its first COM call.
* **The string was copy-pasted.** `0x005A7590` prints `"SoundObj::IsPlaying buffer lost"` but the
  map calls it `EffectBase::OriginalBufferPlaying(short)`. `SoundObj` is not a class in this
  binary at all. This is the one case where the string genuinely self-names and is still wrong,
  which is exactly why a self-naming string is evidence and not proof.

A debug string is strong evidence when it names the function's *own* class and method and nothing
else in the body contradicts it. It is not authority. The linker map is.

## 4. Confidence levels used here

**CONFIRMED** - a symbol in the shipped linker map at exactly this address, or, for the
artifacts, membership of a structure read out of the binary that admits no other reading (the
CRT initializer table, a vftable slot, the `_SEH` region). 346 entries.

**PROBABLE** - no symbol, but converging structural evidence. 1 entry: `0x00575C00`, zlib's
`huft_build`. It has exactly three call sites, which is exactly how many functions call
`huft_build` upstream (`inflate_trees_bits`, `inflate_trees_dynamic`, `inflate_trees_fixed`), it
sits immediately after `inflate_trees_bits`, and its locals `int[15]`, `int[15]`, `int[14]` match
`u[BMAX]`, `x[BMAX+1]`, `c[BMAX+1]` for `BMAX=15`. The name comes from upstream zlib, not from
this binary.

**GUESS** - the function's *purpose* is certain but its *name* is invented. 1 entry:
`0x004F4F30`, a static in the `GraphicsOptions` translation unit that fills the renderer dropdown
from `gfxInterfaces[].DeviceCaps` and then calls `SetupResChoices()`. It is called
`SetupRendererChoices` here purely by analogy. **Do not trust that name.** Nothing in the binary
or the map attests it.

**UNIDENTIFIED** - 1 entry: `0x005418C0`, where the map itself holds only a placeholder.

Note that "identified" for the 63 artifacts means *proved not to be a game function* and given
a descriptive label - not given a linker name, because none exists.

## 5. What was recovered, by class

Classes with three or more recovered methods:

| class | methods recovered |
|---|--:|
| `(free functions)` | 21 |
| `mmInput` | 10 |
| `aiMap` | 9 |
| `mmReplayManager` | 6 |
| `mmInterface` | 6 |
| `UIMenu` | 6 |
| `Aud3DObject` | 5 |
| `asNetwork` | 5 |
| `netZoneScore` | 5 |
| `Matrix44` | 4 |
| `vehBreakableMgr` | 4 |
| `NetArena` | 4 |
| `mmSingleBlitz` | 3 |
| `mmMultiBlitz` | 3 |
| `gfxTextureCachePool` | 3 |
| `datAssetManager` | 3 |
| `datAsciiTokenizer` | 3 |
| `vehCarModel` | 3 |
| `vehCarAudio` | 3 |
| `NetSelectMenu` | 3 |
| `mmCityList` | 3 |
| `mmTextNode` | 3 |
| `aiSubway` | 3 |
| `aiPoliceOfficer` | 3 |
| `aiCableCar` | 3 |
| `zipFile` | 3 |

The largest single recoveries by code size:

| address | recovered | bytes |
|---|---|--:|
| `0x00407C80` | `mmInterface::mmInterface` | 5040 |
| `0x004CCFE0` | `vehCarModel::Init` | 2624 |
| `0x00575410` | `inflate_codes` | 1824 |
| `0x00573480` | `zipFile::Init` | 1536 |
| `0x004805B0` | `phBoundBox::ProbeVsBox` | 1264 |
| `0x00575B50` | `inflate_trees_bits` | 1232 |
| `0x004C23F0` | `FindImpactPolygonToSphere` | 1232 |
| `0x00409030` | `mmInterface::InitPlayerInfo` | 1168 |
| `0x004D9360` | `vehAero::Update` | 896 |
| `0x00538530` | `aiMap::PredictAmbFreewayIntersectionPath` | 784 |
| `0x00410E90` | `mmInterface::ChangePlayerData` | 720 |
| `0x0049C6E0` | `jpeg_make_d_derived_tbl` | 704 |
| `0x00420220` | `mmMultiBlitz::InitGameObjects` | 656 |
| `0x004CB660` | `vehCarSim::vehCarSim` | 640 |
| `0x005407B0` | `aiCableCar::CheckForObstacles` | 624 |

## 6. Full table

`wrong name` is the ImGui/ImPlot/STL label as the decompiler printed it in
`_MISLABELED_FUNCTIONS.c`. Evidence keys:

* `map` - exact-address symbol in the shipped `midtown2.map`
* `vtable slot N` - the address occupies slot N of the class's vftable in the binary
* `self-naming string` - the body prints its own recovered name
* `virtual` - the mangled map symbol is marked virtual
* `CRT init table slot` - reached from the CRT dynamic-initializer table
* `_SEH region` - C++ exception funclet
* `IDA chunk` - not a function; inside another function's extent

| address | wrong name | recovered | conf. | evidence |
|---|---|---|---|---|
| `0x00406F30` | `std::_Wrap_alloc<std::allocator<std::_Contain…` | `mmReplayManager::LoadReplayDesc` | CONF | map |
| `0x004072D0` | `std::allocator<std::_Container_proxy>::constr…` | `mmReplayManager::GetBrakes` | CONF | map |
| `0x00407360` | `std::allocator<char>::destroy<char *>` | `mmReplayManager::GetThrottle` | CONF | map |
| `0x00407390` | `std::_Wrap_alloc<std::allocator<std::_Contain…` | `mmReplayManager::GetData` | CONF | map |
| `0x00407410` | `std::forward<std::_Wrap_alloc<std::allocator<…` | `mmReplayManager::GetReplayInfo` | CONF | map |
| `0x004075A0` | `std::_Wrap_alloc<std::allocator<char>>::_Wrap…` | `mmReplayManager::SetReplayInfo` | CONF | map |
| `0x00407B40` | `std::_String_val<std::_Simple_types<char>>::~…` | `asNode::GetClassName` | CONF | map + vtable slot 11 + virtual |
| `0x00407C80` | `std::_String_val<std::_Simple_types<char>>::_…` | `mmInterface::mmInterface` | CONF | map |
| `0x00409030` | `std::allocator_traits<std::allocator<char>>::…` | `mmInterface::InitPlayerInfo` | CONF | map |
| `0x0040FE80` | `ImGui::SetWindowPos` | `mmInterface::PlayerReadState` | CONF | map |
| `0x0040FF80` | `ImGui::GetContentRegionMax` | `mmInterface::InitNetwork` | CONF | map + self-naming string |
| `0x00410E90` | `ImGui::BeginGroup` | `mmInterface::ChangePlayerData` | CONF | map |
| `0x00411F70` | `ImGui::IsPopupOpen` | `mmInterface::SendReadyReq` | CONF | map |
| `0x00413D30` | `ImGui::IsAnyItemActive` | `mmGame::IsPopupEnabled` | CONF | map |
| `0x00414E50` | `<eax>` | `mmGame::SendChatMessage` | CONF | map + vtable (inherited) + virtual |
| `0x004150B0` | `ImGui::IsMouseDragging` | `_dynamic_initializer (CRT slot 15, mmGame TU) - writes stru_6…` | CONF | CRT init table slot |
| `0x00415110` | `ImGui::GetMouseDragDelta` | `_dynamic_initializer (CRT slot 16, mmGame TU) - writes stru_6…` | CONF | CRT init table slot |
| `0x0041AEC0` | `<eax>` | `mmSingleBlitz::InitHUD` | CONF | map + vtable slot 18 + virtual |
| `0x0041B590` | `ImGui::FocusTopMostWindowUnderOne` | `mmSingleBlitz::UpdateGameInput` | CONF | map + vtable slot 19 + virtual |
| `0x0041C390` | `ImGui::StartMouseMovingWindowOrNode` | `_dynamic_initializer (CRT slot 38, mmSingleBlitz TU)` | CONF | CRT init table slot |
| `0x0041C490` | `ImGui::UpdateMouseMovingWindowNewFrame` | `mmSingleBlitz::GetWaypoints` | CONF | map + vtable slot 28 + virtual |
| `0x0041CA50` | `ImGui::ScaleWindowsInViewport` | `mmSingleCircuit::Reset` | CONF | map + vtable slot 3 + virtual |
| `0x0041D840` | `ImGui::KeepAliveID` | `mmSingleCircuit::RegisterLap` | CONF | map |
| `0x0041DEC0` | `ImGui::SetLastItemData` | `_dynamic_initializer (CRT slot 46, mmSingleCircuit TU) - writ…` | CONF | CRT init table slot |
| `0x0041F8E0` | `ImGui::NavMoveRequestButNoResultYet` | `_dynamic_initializer (CRT slot 52, mmSingleRace TU) - writes …` | CONF | CRT init table slot |
| `0x0041FDA0` | `ImGui::CalcTypematicRepeatAmount` | `mmSingleRoam::NextRace` | CONF | map + vtable slot 22 + virtual |
| `0x0041FE80` | `ImGui::ActivateItem` | `_dynamic_initializer (CRT slot 58, mmSingleRoam TU) - writes …` | CONF | CRT init table slot |
| `0x0041FF30` | `ImGui::SetNavIDWithRectRel` | `mmMultiBlitz::mmMultiBlitz` | CONF | map |
| `0x00420000` | `?PopFocusScope@ImGui@@YAXXZ` | `mmMultiBlitz::Init` | CONF | map + vtable slot 13 + virtual |
| `0x00420220` | `ImGui::DockContextClearNodes` | `mmMultiBlitz::InitGameObjects` | CONF | map + vtable slot 17 + virtual |
| `0x00421B70` | `ImGui::DockBuilderSplitNode` | `mmGameMulti::NextRace` | CONF | map + vtable (inherited) + virtual |
| `0x0042B6C0` | `ImGui::AddUpdateViewport` | `_dynamic_initializer (CRT slot 95, mmPopup TU)` | CONF | CRT init table slot |
| `0x0042CC00` | `ImGui::FindPlatformMonitorForPos` | `dgPhysEntity::DetachMe` | CONF | map + vtable (inherited) + virtual |
| `0x0042EE90` | `ImGui::DockContextAddNode` | `mmHudMap::Reset` | CONF | map + vtable slot 3 + virtual |
| `0x0042FA40` | `ImGui::DockContextPruneUnusedSettingsNodes` | `mmHudMap::SetOrient` | CONF | map |
| `0x0042FE10` | `ImGui::DockContextFindNodeByID` | `mmCarRoadFF::AssignProperties` | CONF | map |
| `0x00432140` | `ImGui::DockNodeUpdateTabBar` | `mmIcons::mmIcons` | CONF | map |
| `0x00432D10` | `ImGui::DockNodeRemoveTabBar` | `mmAmbientAudio::Update` | CONF | map |
| `0x00432F40` | `ImGui::DockNodeUpdateVisibleFlag` | `_dynamic_initializer (CRT slot 116, mmAmbientAudio TU)` | CONF | CRT init table slot |
| `0x00433100` | `ImGui::DockNodeIsDropAllowed` | `mmCDPlayer::Update` | CONF | map + vtable slot 2 + virtual |
| `0x00433FB0` | `ImGui::DockNodeCalcTabBarLayout` | `mmGameMusicData::GetNumDMusicChoiceGroups` | CONF | map |
| `0x0043C080` | `ImGui::DockNodeGetRootNode` | `mmNetObject::mmNetObject` | CONF | map |
| `0x00442580` | `ImGui::ShowDemoWindow` | `dgBangerInstance::GetBound` | CONF | map + vtable (inherited) + virtual |
| `0x00463090` | `ImGui::ShadeVertsLinearColorGradientKeepAlpha` | `_dynamic_atexit_destructor_for__lvlInstance::sm_XrefHash_` | CONF | CRT table + atexit guard pattern |
| `0x0046EE60` | `ImBitVector::Create` | `_dynamic_initializer (Matrix34/phContact TU) - flt_660F5C = s…` | CONF | CRT init table slot |
| `0x0046F110` | `ImBitVector::TestBit` | `phContact::IsEqual` | CONF | map |
| `0x00478820` | `ImGui::TreeNodeV` | `phInertialCS::CalcNetTurn` | CONF | map |
| `0x00479830` | `ImGui::ListBoxHeader` | `_dynamic_initializer_for__phBoundBox::EdgeNormals_ (chunk: en…` | CONF | CRT init table slot |
| `0x00479950` | `ImGui::PlotLines` | `_dynamic_initializer_for__phBoundBox::EdgeNormals_ (chunk)` | CONF | inside a mapped function (IDA chunk) |
| `0x00479A50` | `ImGui::PlotHistogram` | `_dynamic_initializer_for__phBoundBox::EdgeNormals_ (chunk: en…` | CONF | inside a mapped function (IDA chunk) |
| `0x00479A90` | `ImGui::Value` | `_dynamic_initializer_for__phBoundBox::EdgeNormals_ (chunk: en…` | CONF | inside a mapped function (IDA chunk) |
| `0x00479AD0` | `ImGui::Value` | `_dynamic_initializer_for__phBoundBox::EdgeNormals_ (chunk: en…` | CONF | inside a mapped function (IDA chunk) |
| `0x00479D50` | `ImGui::EndMenuBar` | `_dynamic_initializer (CRT slot 173, phBoundBox TU) - fills th…` | CONF | CRT init table slot |
| `0x004805B0` | `ImGui::Scrollbar` | `phBoundBox::ProbeVsBox` | CONF | map |
| `0x004813A0` | `<al>` | `phBoundHotdog::TestProbe` | CONF | map + vtable slot 10 + virtual |
| `0x0048B230` | `ImStb::stb_text_makeundo_insert` | `phBoundPolygonal::ResetVertNeedsH` | CONF | map |
| `0x004917F0` | `ImGui::ScaleRatioFromValueT<double,double,dou…` | `phBoundCollision::testNoOverlap` | CONF | map |
| `0x00499470` | `ImGui_ImplAGE_SetupRenderState` | `examine_app14` | CONF | map |
| `0x0049ACA0` | `ImGui_ImplWin32_CreateWindow` | `alloc_sarray` | CONF | map |
| `0x0049B480` | `ImGui_ImplWin32_SetWindowAlpha` | `free_pool` | CONF | map |
| `0x0049B5D0` | `ImGui_ImplWin32_WndProcHandler_PlatformWindow` | `jpeg_abort` | CONF | map |
| `0x0049BE50` | `ImGui::InputTextWithHint` | `start_pass_main` | CONF | map |
| `0x0049C6D0` | `ImPlot::SetCurrentContext` | `dummy_consume_data` | CONF | map |
| `0x0049C6E0` | `ImPlot::BeginPlot` | `jpeg_make_d_derived_tbl` | CONF | map |
| `0x004A32B0` | `ImPlot::GetPlotQuery` | `_dynamic_initializer_for__asLinearCS::RootFrame_` | CONF | CRT init table slot |
| `0x004A3440` | `ImPlot::Annotate` | `asCullable::Cull` | CONF | map + vtable (inherited) + virtual |
| `0x004A34D0` | `ImPlot::AnnotateV` | `eqEventQ::~eqEventQ` | CONF | map + virtual |
| `0x004A47B0` | `<eax>` | `modPackage::OpenFile` | CONF | map + self-naming string |
| `0x004A6A50` | `ImPlot::GetStyleColorName` | `rglCameraMatrix` | CONF | map |
| `0x004AD5D0` | `ImPlot::LerpColormap` | `gfxTexture::Load (chunk, +1232) - RGB888 -> RGB555 conversion…` | CONF | inside a mapped function (IDA chunk) |
| `0x004AD750` | `ImPlot::AddTextVertical` | `gfxTexture::SetMIPMapEnv` | CONF | map |
| `0x004ADBB0` | `ImPlot::GetGmtTime` | `gfxTextureCachePool::~gfxTextureCachePool` | CONF | map |
| `0x004ADBE0` | `ImPlot::MkLocTime` | `gfxTextureCachePool::IsCompatibleWith` | CONF | map |
| `0x004ADD30` | `ImPlot::GetYear` | `gfxTextureCachePool::AddSlot` | CONF | map |
| `0x004AE920` | `ImPlot::FormatDateTime` | `gfxImage::Create` | CONF | map |
| `0x004B30A0` | `ImPlot::IsColorAuto` | `gfxAnnotateImage` | CONF | map |
| `0x004B3310` | `ImPlot::Precision` | `gfxPacket::ForceCPV` | CONF | map |
| `0x004C0770` | `ImPlot::ScrollingBuffer::ScrollingBuffer` | `Matrix44::Zero` | CONF | map |
| `0x004C0920` | `ImPlot::BenchRecord::~BenchRecord` | `Matrix44::Transpose` | CONF | map |
| `0x004C0970` | `ImPlot::HugeTimeData::~HugeTimeData` | `Matrix44::FromMatrix34` | CONF | map |
| `0x004C0AD0` | `ImPlot::RollingBuffer::AddPoint` | `Matrix44::Add` | CONF | map |
| `0x004C1170` | `ImPlot::PlotShadedG` | `Vector4::Dot` | CONF | map |
| `0x004C1200` | `ImPlot::PlotBarsHG` | `Vector4::Dot3x3` | CONF | map |
| `0x004C23F0` | `ImPlot::PlotRects` | `FindImpactPolygonToSphere` | CONF | map |
| `0x004C5870` | `ImPlot::PlotBarsH<unsigned short>` | `datAssetManager::Open` | CONF | map |
| `0x004C58C0` | `ImPlot::PlotBarsH<int>` | `datAssetManager::Open` | CONF | map |
| `0x004C59E0` | `ImPlot::PlotBarsH<float>` | `datAssetManager::Exists` | CONF | map |
| `0x004C5B90` | `ImPlot::PlotBarsH<unsigned __int64>` | `_dynamic_atexit_destructor_for__datArgParser::ArgHash_` | CONF | CRT table + atexit guard pattern |
| `0x004C7A20` | `ImPlot::PlotBarsHEx<ImPlot::GetterXsYs<unsign…` | `_dynamic_initializer_for__NullCallback_ (datCallback)` | CONF | CRT init table slot |
| `0x004C8860` | `ImPlot::PlotDigital<float>` | `datAsciiTokenizer::GetVector` | CONF | map + vtable slot 3 + virtual |
| `0x004C8920` | `ImPlot::PlotDigital<__int64>` | `datAsciiTokenizer::MatchVector` | CONF | map + vtable slot 10 + virtual |
| `0x004C8980` | `ImPlot::PlotDigital<unsigned __int64>` | `datAsciiTokenizer::Put` | CONF | map + vtable slot 17 + virtual |
| `0x004C8DE0` | `ImPlot::PlotDigitalEx<ImPlot::GetterXsYs<unsi…` | `datBinTokenizer::Put` | CONF | map + vtable slot 15 + virtual |
| `0x004C91E0` | `ImPlot::PlotDigitalEx<ImPlot::GetterXsYs<shor…` | `datReplay::BeginFrame` | CONF | map |
| `0x004CB5E0` | `ImPlot::PlotErrorBars<signed char>` | `_dynamic_initializer (CRT slot 212, vehCarDamage TU)` | CONF | CRT init table slot |
| `0x004CB660` | `ImPlot::PlotErrorBars<unsigned char>` | `vehCarSim::vehCarSim` | CONF | map |
| `0x004CB8E0` | `ImPlot::PlotErrorBars<float>` | `vehCarSim::~vehCarSim` | CONF | map + virtual |
| `0x004CCFE0` | `ImPlot::PlotErrorBarsEx<ImPlot::GetterError<_…` | `vehCarModel::Init` | CONF | map |
| `0x004CDA20` | `ImPlot::PlotErrorBarsHEx<ImPlot::GetterError<…` | `vehCarModel::InitBound` | CONF | map |
| `0x004CDF60` | `ImPlot::PlotErrorBarsHEx<ImPlot::GetterError<…` | `vehCarModel::InitSirenLight` | CONF | map |
| `0x004D00A0` | `ImPlot::PlotHeatmap<double>` | `aiAmbientVehicleAudio::PlayImpactReaction (chunk, +16)` | CONF | inside a mapped function (IDA chunk) |
| `0x004D0A10` | `ImPlot::PlotLine<double>` | `_dynamic_initializer (CRT slot 215, aiAmbientVehicleAudio TU)` | CONF | CRT init table slot |
| `0x004D0AA0` | `ImPlot::PlotLine<__int64>` | `_dynamic_initializer (CRT slot 218, aiAmbientVehicleAudio TU)` | CONF | CRT init table slot |
| `0x004D12E0` | `ImPlot::PlotLineEx<ImPlot::GetterXsYs<short>>` | `vehCarAudioContainer::InitPolice` | CONF | map |
| `0x004D4800` | `ImPlot::PlotLineEx<ImPlot::GetterYs<double>>` | `vehPoliceCarAudio::ReadSirenData` | CONF | map |
| `0x004D5220` | `ImPlot::PlotLineEx<ImPlot::GetterFuncPtr>` | `vehPoliceCarAudio::UnAssignSounds` | CONF | map + vtable slot 1 + virtual |
| `0x004D8580` | `ImPlot::PlotScatter<signed char>` | `vehBreakableMgr::vehBreakableMgr` | CONF | map |
| `0x004D86A0` | `ImPlot::PlotScatter<short>` | `vehBreakableMgr::Add` | CONF | map |
| `0x004D8770` | `ImPlot::PlotScatter<unsigned short>` | `vehBreakableMgr::Update` | CONF | map |
| `0x004D8890` | `ImPlot::PlotScatter<unsigned int>` | `vehBreakableMgr::DrawCityLit` | CONF | map |
| `0x004D9360` | `ImPlot::PlotScatterEx<ImPlot::GetterXsYs<unsi…` | `vehAero::Update` | CONF | map + vtable slot 2 + virtual |
| `0x004DB7B0` | `ImPlot::PlotScatterEx<ImPlot::GetterYs<double…` | `_dynamic_initializer (CRT slot 245, vehHornAudio TU)` | CONF | CRT init table slot |
| `0x004DC210` | `ImPlot::PlotShaded<signed char>` | `vehCarAudio::StopHorn` | CONF | map |
| `0x004DC340` | `ImPlot::PlotShaded<unsigned char>` | `vehCarAudio::IsAirBorne` | CONF | map |
| `0x004DC730` | `ImPlot::PlotShaded<int>` | `vehCarAudio::GetAudImpactPtr` | CONF | map |
| `0x004DC8D0` | `ImPlot::PlotShaded<float>` | `_dynamic_initializer (CRT slot 252, vehCarAudio TU) - flt_6B0…` | CONF | CRT init table slot |
| `0x004DC930` | `ImPlot::PlotShaded<float>` | `vehSemiCarAudio::vehSemiCarAudio` | CONF | map |
| `0x004DD150` | `ImPlot::PlotShadedEx<ImPlot::GetterXsYs<unsig…` | `_dynamic_initializer (vehSemiCarAudio TU) - flt_6B007C` | CONF | CRT init table slot |
| `0x004DD8D0` | `ImPlot::PlotShadedEx<ImPlot::GetterXsYs<unsig…` | `vehEngineAudio::~vehEngineAudio (chunk, +16) - deletes the ve…` | CONF | inside a mapped function (IDA chunk) |
| `0x004DFC70` | `ImPlot::PlotShadedEx<ImPlot::GetterYs<unsigne…` | `_dynamic_initializer (vehEngineSampleWrapper TU) - flt_6B00DC` | CONF | CRT init table slot |
| `0x004E0210` | `ImPlot::PlotShadedEx<ImPlot::GetterYs<__int64…` | `vehSurfaceAudioData::ParseCSVBuffer (chunk, +16) - the fgets/…` | CONF | inside a mapped function (IDA chunk) |
| `0x004E0A80` | `ImPlot::PlotStairs<unsigned int>` | `UIMenu::Update` | CONF | map + vtable slot 2 + virtual |
| `0x004E0B10` | `ImPlot::PlotStairs<float>` | `UIMenu::BackUp` | CONF | map + vtable slot 15 + virtual |
| `0x004E0B50` | `ImPlot::PlotStairs<float>` | `UIMenu::SetFocusWidget` | CONF | map |
| `0x004E0BA0` | `ImPlot::PlotStairs<double>` | `UIMenu::GetBstate` | CONF | map |
| `0x004E0BE0` | `ImPlot::PlotStairs<double>` | `UIMenu::DisableIME` | CONF | map |
| `0x004E1860` | `ImPlot::PlotStairsEx<ImPlot::GetterXsYs<unsig…` | `UIMenu::AddUIControl` | CONF | map |
| `0x004E5AD0` | `ImPlot::PlotStems<int>` | `MenuManager::SetPreviousMenu` | CONF | map |
| `0x004EA2F0` | `ImPlot::PlotStemsEx<ImPlot::GetterYs<__int64>…` | `UITextScroll::~UITextScroll` | CONF | map + virtual |
| `0x004EADD0` | `ImPlot::RenderHeatmap<signed char,ImPlot::Tra…` | `UITextScroll::InitTextScroll` | CONF | map |
| `0x004ECB50` | `ImPlot::RenderHeatmap<short,ImPlot::Transform…` | `UISlider::SetReadWrite` | CONF | map |
| `0x004EDA10` | `ImPlot::RenderHeatmap<unsigned short,ImPlot::…` | `UIIcon::Cull` | CONF | map + vtable slot 1 + virtual |
| `0x004F0D40` | `ImPlot::RenderHeatmap<float,ImPlot::Transform…` | `MArray::Init` | CONF | map |
| `0x004F10E0` | `ImPlot::RenderHeatmap<double,ImPlot::Transfor…` | `MArray::AddMenuData` | CONF | map |
| `0x004F1BC0` | `ImPlot::RenderHeatmap<double,ImPlot::Transfor…` | `TextDropWidget::SetHighlight` | CONF | map |
| `0x004F2DE0` | `ImPlot::RenderHeatmap<unsigned __int64,ImPlot…` | `VSWidget::Action` | CONF | map |
| `0x004F4F30` | `ImPlot::RenderLineSegments<ImPlot::GetterXsYs…` | `SetupRendererChoices` | GUES | purpose clear, name invented |
| `0x004F5470` | `ImPlot::RenderLineSegments<ImPlot::GetterXsYs…` | `VehicleSelectBase::VehicleSelectBase` | CONF | map |
| `0x004F7930` | `ImPlot::RenderLineSegments<ImPlot::GetterYs<u…` | `_dynamic_initializer (CRT slot 308, VehicleSelectBase TU)` | CONF | CRT init table slot |
| `0x004F7A80` | `ImPlot::RenderLineSegments<ImPlot::GetterYs<u…` | `VehicleSelectBase::SetShowcaseFlag` | CONF | map |
| `0x004F87A0` | `ImPlot::RenderLineSegments<ImPlot::GetterYs<i…` | `AudioOptions::SetSFXVolume` | CONF | map |
| `0x004F90D0` | `ImPlot::RenderLineSegments<ImPlot::GetterYs<u…` | `AudioOptions::LoadUIMusicCSV` | CONF | map |
| `0x004F9220` | `ImPlot::RenderLineSegments<ImPlot::GetterYs<f…` | `_dynamic_initializer (CRT slot 310, AudioOptions TU)` | CONF | CRT init table slot |
| `0x004F9610` | `ImPlot::RenderLineSegments<ImPlot::GetterYs<f…` | `Dialog_Eject::AddName` | CONF | map |
| `0x004FA1E0` | `ImPlot::RenderLineSegments<ImPlot::GetterYs<u…` | `Dialog_ReplayEdit::~Dialog_ReplayEdit` | CONF | map + virtual |
| `0x004FA9A0` | `ImPlot::RenderLineStrip<ImPlot::GetterXsYs<si…` | `Dialog_Replay::DeleteCB` | CONF | map |
| `0x004FAFE0` | `ImPlot::RenderLineStrip<ImPlot::GetterXsYs<un…` | `Dialog_Host::`scalar deleting destructor'` | CONF | map + vtable slot 0 + virtual |
| `0x004FB9E0` | `ImPlot::RenderLineStrip<ImPlot::GetterXsYs<un…` | `Dialog_RaceEnvironment::DoneCallback` | CONF | map |
| `0x004FD560` | `ImPlot::RenderLineStrip<ImPlot::GetterXsYs<un…` | `Dialog_Message::~Dialog_Message` | CONF | map + virtual |
| `0x004FD920` | `ImPlot::RenderLineStrip<ImPlot::GetterYs<sign…` | `Dialog_NewPlayer::EnterNewPlayer` | CONF | map |
| `0x004FF9A0` | `ImPlot::RenderLineStrip<ImPlot::GetterYs<floa…` | `NetArena::RemoveRosterName` | CONF | map |
| `0x005003A0` | `ImPlot::RenderLineStrip<ImPlot::GetterYs<__in…` | `NetArena::SetHost` | CONF | map |
| `0x005004E0` | `ImPlot::RenderLineStrip<ImPlot::GetterYs<__in…` | `NetArena::LoadRaceMap` | CONF | map |
| `0x00500620` | `ImPlot::RenderLineStrip<ImPlot::GetterYs<unsi…` | `NetArena::SetMyStatus` | CONF | map |
| `0x00501700` | `ImPlot::RenderMarkers<ImPlot::TransformerLinL…` | `HostRaceMenu::GetGoldMassVal` | CONF | map |
| `0x00502380` | `ImPlot::RenderMarkers<ImPlot::TransformerLinL…` | `ControlSetup::POVCB` | CONF | map |
| `0x00502880` | `ImPlot::RenderMarkers<ImPlot::TransformerLinL…` | `ControlCustom::ClearBadAssignment` | CONF | map |
| `0x00502920` | `ImPlot::RenderMarkers<ImPlot::TransformerLinL…` | `ControlCustom::`scalar deleting destructor'` | CONF | map + vtable slot 0 + virtual |
| `0x00502CE0` | `ImPlot::RenderMarkers<ImPlot::TransformerLogL…` | `OptionsMenu::FocusDescription` | CONF | map |
| `0x00502EC0` | `ImPlot::RenderMarkers<ImPlot::TransformerLogL…` | `CrashCourseIntro::`scalar deleting destructor'` | CONF | map + vtable slot 0 + virtual |
| `0x00503960` | `ImPlot::RenderMarkers<ImPlot::TransformerLogL…` | `CrashCourse::GameCallback` | CONF | map |
| `0x00503F00` | `ImPlot::RenderMarkers<ImPlot::TransformerLogL…` | `CrashCourse::FocusDescription` | CONF | map |
| `0x00504B80` | `ImPlot::RenderPrimitives<ImPlot::LineSegments…` | `NetSelectMenu::GetSessions` | CONF | map |
| `0x00504CE0` | `ImPlot::RenderPrimitives<ImPlot::LineSegments…` | `NetSelectMenu::SetComs` | CONF | map |
| `0x00504FA0` | `ImPlot::RenderPrimitives<ImPlot::LineSegments…` | `NetSelectMenu::SetModemButton` | CONF | map |
| `0x005057E0` | `ImPlot::RenderPrimitives<ImPlot::LineSegments…` | `Vehicle::SetSubMenuButtons` | CONF | map |
| `0x00505C00` | `ImPlot::RenderPrimitives<ImPlot::LineSegments…` | `AboutMenu::`scalar deleting destructor'` | CONF | map + vtable slot 0 + virtual |
| `0x00506020` | `ImPlot::RenderPrimitives<ImPlot::LineSegments…` | `MainMenu::~MainMenu` | CONF | map + virtual |
| `0x00508540` | `ImPlot::RenderPrimitives<ImPlot::LineSegments…` | `RaceMenuBase::GameCallback` | CONF | map |
| `0x00509CA0` | `ImPlot::RenderPrimitives<ImPlot::LineSegments…` | `PUResults::AddName` | CONF | map |
| `0x0050AA60` | `ImPlot::RenderPrimitives<ImPlot::LineSegments…` | `PURoster::~PURoster` | CONF | map + virtual |
| `0x0050AFE0` | `ImPlot::RenderPrimitives<ImPlot::LineSegments…` | `PURoster::FindRosterName` | CONF | map |
| `0x0050B560` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `PUReplay::GetSaveRO` | CONF | map |
| `0x0050B980` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `PUReplaySave::PUReplaySave` | CONF | map |
| `0x0050BAE0` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `PUReplaySave::~PUReplaySave` | CONF | map + virtual |
| `0x0050C1C0` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `PUControl::ControlSelect` | CONF | map |
| `0x0050C5E0` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `PUOptions::~PUOptions` | CONF | map + virtual |
| `0x0050CA00` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `_dynamic_initializer (CRT slot 320, PUAudioOptions TU)` | CONF | CRT init table slot |
| `0x0050DBE0` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `AudSoundBase::Update` | CONF | map + vtable slot 2 + virtual |
| `0x0050E2C0` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `AudSoundBase::SetSoundHandleIndex` | CONF | map |
| `0x0050EC60` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `AudStreamingMusic::StreamingMusicUpdate` | CONF | map |
| `0x0050EDC0` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `Aud_Stream_Size` | CONF | map |
| `0x0050F1E0` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `AudManagerBase::StopAllSounds` | CONF | map |
| `0x0050F600` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `_dynamic_initializer (Aud3DObjectManager TU) - flt_6B1488` | CONF | CRT init table slot |
| `0x0050F760` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `Aud3DObjectManager::Remove` | CONF | map |
| `0x0050FA20` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `Aud3DObjectManager::SetNumScreenSplits` | CONF | map |
| `0x00511020` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `_dynamic_initializer (AudCreatureContainer TU) - flt_6B1498` | CONF | CRT init table slot |
| `0x005112E0` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `AudImpact::GetSecondsElapsed` | CONF | map |
| `0x00511860` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `AudImpact::Play` | CONF | map |
| `0x005119C0` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `_dynamic_initializer (CRT slot 337, AudImpact TU)` | CONF | CRT init table slot |
| `0x00511B20` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `Aud3DObject::~Aud3DObject` | CONF | map + virtual |
| `0x00511DE0` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `_dynamic_initializer (CRT slot 348, Aud3DObject TU)` | CONF | CRT init table slot |
| `0x00511F40` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `Aud3DObject::CalcSinglePlayerPan` | CONF | map |
| `0x00512200` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `Aud3DObject::GetDistToClosestHead2` | CONF | map |
| `0x00512360` | `ImPlot::RenderPrimitives<ImPlot::LineStripRen…` | `Aud3DObject::SetNon3DParams` | CONF | map + vtable slot 4 + virtual |
| `0x00512620` | `ImPlot::RenderPrimitives<ImPlot::RectRenderer…` | `Aud3DObject::CalcPseudoDistToClosestHead` | CONF | map |
| `0x00512A40` | `ImPlot::RenderPrimitives<ImPlot::RectRenderer…` | `_dynamic_initializer (AudCreature TU) - registers exit_sub_51…` | CONF | CRT init table slot |
| `0x00513C20` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `Aud3DSampleWrapper::SetSampleName` | CONF | map |
| `0x005141A0` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `AudImpactData::ReadCSV` | CONF | map |
| `0x00514720` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `_dynamic_initializer (CRT slot 357, AudImpactData TU)` | CONF | CRT init table slot |
| `0x00514CA0` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `AudCreatureAvoid::SetAud3DObjectPtr` | CONF | map |
| `0x00515A60` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `Aud3DAmbientObject::SetSoundData` | CONF | map |
| `0x00515D20` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `_dynamic_initializer (Aud3DAmbientObject TU) - flt_6B1584` | CONF | CRT init table slot |
| `0x00517060` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `ThreadProc` | CONF | map |
| `0x00517A00` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `SegmentWrapper::IsPlaying` | CONF | map + self-naming string |
| `0x00517CC0` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `CLoader::Init` | CONF | map |
| `0x00518660` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `_dynamic_initializer (CLoader/DMusic TU) - flt_6B15A4` | CONF | CRT init table slot |
| `0x00518A80` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `CFileStream::~CFileStream` | CONF | map |
| `0x00518EA0` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `CMemStream::CMemStream` | CONF | map |
| `0x00519160` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `CMemStream::Seek` | CONF | map + vtable slot 5 + virtual |
| `0x00519580` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `AudManager::GetRaceSpeechPtr` | CONF | map |
| `0x00519C60` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `AudManager::SetEAXReverbVolume` | CONF | map |
| `0x00519DC0` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `_dynamic_initializer (CRT slot 382, AudManager TU)` | CONF | CRT init table slot |
| `0x0051AB80` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `mmRaceSpeech::LoadVehicleUnlock` | CONF | map |
| `0x0051B3C0` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `_dynamic_initializer (CRT slot 395, mmRaceSpeech TU)` | CONF | CRT init table slot |
| `0x0051B7E0` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `mmSpeechContainer::GetRaceSpeechPtr` | CONF | map |
| `0x0051C2E0` | `ImPlot::RenderPrimitives<ImPlot::ShadedRender…` | `MixerCTL::RefreshAll` | CONF | map |
| `0x005200C0` | `ImPlot::RenderPrimitives<ImPlot::StairsRender…` | `camViewCS::OneShot` | CONF | map |
| `0x00522060` | `ImPlot::RenderPrimitives<ImPlot::StairsRender…` | `camAppCS::ApproachIt` | CONF | map |
| `0x005230E0` | `ImPlot::RenderPrimitives<ImPlot::StairsRender…` | `Spline::Print` | CONF | map |
| `0x00524160` | `ImPlot::RenderPrimitives<ImPlot::StairsRender…` | `mmCityList::mmCityList` | CONF | map |
| `0x005242C0` | `ImPlot::RenderPrimitives<ImPlot::StairsRender…` | `mmCityList::SetCurrentCity` | CONF | map |
| `0x00524420` | `ImPlot::RenderPrimitives<ImPlot::StairsRender…` | `mmCityList::Print` | CONF | map |
| `0x00524B00` | `ImPlot::RenderPrimitives<ImPlot::StairsRender…` | `mmMiscData::Reset` | CONF | map |
| `0x00526020` | `ImPlot::RenderStairs<ImPlot::GetterXsYs<short…` | `mmGfxCFG::Set` | CONF | map |
| `0x00526470` | `ImPlot::RenderStairs<ImPlot::GetterXsYs<short…` | `_dynamic_initializer (CRT slot 410, mmPlayerConfig TU)` | CONF | CRT init table slot |
| `0x00526A30` | `ImPlot::RenderStairs<ImPlot::GetterXsYs<unsig…` | `mmPlayerDirectory::GetPlayerName` | CONF | map |
| `0x00527440` | `ImPlot::RenderStairs<ImPlot::GetterXsYs<unsig…` | `mmPlayerData::GetTotalPassed` | CONF | map |
| `0x00527890` | `ImPlot::RenderStairs<ImPlot::GetterXsYs<float…` | `mmPlayerData::ResolveCheckpointProgress` | CONF | map |
| `0x00529F60` | `ImPlot::RenderStairs<ImPlot::GetterYs<unsigne…` | `mmRaceData::GetNumOpponents` | CONF | map |
| `0x0052C7A0` | `ImPlot::RenderStairs<ImPlot::GetterFuncPtr,Im…` | `mmInput::ForceAssignment` | CONF | map |
| `0x0052CA80` | `ImPlot::RenderStairs<ImPlot::GetterFuncPtr,Im…` | `mmInput::PollStates` | CONF | map |
| `0x0052CBF0` | `ImPlot::RenderStairs<ImPlot::GetterFuncPtr,Im…` | `mmInput::ClearEventHitFlags` | CONF | map |
| `0x0052CFE0` | `ImPlot::GetterBarV<signed char>::GetterBarV<s…` | `mmInput::EventToButton` | CONF | map |
| `0x0052D5E0` | `ImPlot::GetterError<double>::GetterError<doub…` | `mmInput::PutEventInQueue` | CONF | map |
| `0x0052DA00` | `ImPlot::GetterXsYRef<double>::GetterXsYRef<do…` | `mmInput::GetBrakesVal` | CONF | map |
| `0x0052E0A0` | `ImPlot::GetterYs<int>::GetterYs<int>` | `mmInput::DeviceConnected` | CONF | map |
| `0x0052E340` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterXs…` | `mmInput::FFSetValues` | CONF | map |
| `0x0052E580` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterXs…` | `mmInput::BinarySaveConfig` | CONF | map |
| `0x0052EDC0` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterXs…` | `mmInput::`scalar deleting destructor'` | CONF | map + vtable slot 0 + virtual |
| `0x0052EEE0` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterXs…` | `mmIO::Clear` | CONF | map |
| `0x0052F0C0` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterXs…` | `mmIO::GetDescription` | CONF | map |
| `0x0052F3C0` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterYs…` | `mmIODev::mmIODev` | CONF | map |
| `0x0052FB40` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterYs…` | `mmIODev::Read` | CONF | map |
| `0x0052FEA0` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterYs…` | `mmJoyMan::GetJoyButton` | CONF | map |
| `0x005305C0` | `ImPlot::LineStripRenderer<ImPlot::GetterXsYs<…` | `mmJoystick::inputPrepareDevice` | CONF | map |
| `0x00531540` | `ImPlot::LineStripRenderer<ImPlot::GetterYs<si…` | `mmEffectFF::mmEffectFF` | CONF | map |
| `0x00531940` | `ImPlot::LineStripRenderer<ImPlot::GetterYs<sh…` | `mmRoadFF::Init` | CONF | map + vtable slot 0 + virtual |
| `0x00531A40` | `ImPlot::LineStripRenderer<ImPlot::GetterYs<sh…` | `mmRoadFF::SetValues` | CONF | map + vtable slot 3 + virtual |
| `0x00531EC0` | `ImPlot::LineStripRenderer<ImPlot::GetterYs<in…` | `mmSpringFF::Init` | CONF | map + vtable slot 0 + virtual |
| `0x00531FC0` | `ImPlot::LineStripRenderer<ImPlot::GetterYs<un…` | `mmSpringFF::SetValues` | CONF | map + vtable slot 3 + virtual |
| `0x005321C0` | `ImPlot::LineStripRenderer<ImPlot::GetterYs<fl…` | `mmText::CreateLocFont` | CONF | map |
| `0x005322C0` | `ImPlot::LineStripRenderer<ImPlot::GetterYs<fl…` | `mmText::GetDC` | CONF | map |
| `0x005326C0` | `ImPlot::LineStripRenderer<ImPlot::GetterYs<__…` | `mmTextNode::~mmTextNode` | CONF | map + virtual |
| `0x00532840` | `ImPlot::LineStripRenderer<ImPlot::GetterYs<un…` | `mmTextNode::Init` | CONF | map |
| `0x00532A40` | `ImPlot::LineStripRenderer<ImPlot::GetterFuncP…` | `mmTextNode::SetHlColor` | CONF | map |
| `0x005334C0` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<uns…` | `mmNumber::Update` | CONF | map + vtable slot 2 + virtual |
| `0x00533660` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<uns…` | `asMeshSetForm::SetShape` | CONF | map + self-naming string |
| `0x005339A0` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<uns…` | `asMeshSetForm::`vector deleting destructor'` | CONF | map + vtable slot 0 + virtual |
| `0x00534500` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<uns…` | `mmMouseSteerBar::Cull` | CONF | map + vtable slot 1 + virtual |
| `0x005345D0` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<uns…` | `mmDamage::Init` | CONF | map |
| `0x00534840` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<int…` | `_dynamic_atexit_destructor_for__AIMAP_ (aiMap)` | CONF | CRT table + atexit guard pattern |
| `0x005349E0` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<int…` | `aiMap::Hookman` | CONF | map |
| `0x00534AB0` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<int…` | `aiMap::Pedestrian` | CONF | map |
| `0x005374F0` | `ImPlot::ShadedRenderer<ImPlot::GetterYs<unsig…` | `aiMap::Cull` | CONF | map + vtable slot 1 + virtual |
| `0x005375C0` | `ImPlot::ShadedRenderer<ImPlot::GetterYs<short…` | `aiMap::DrawMap` | CONF | map |
| `0x00537AA0` | `ImPlot::ShadedRenderer<ImPlot::GetterYs<unsig…` | `aiMap::MapComponent` | CONF | map |
| `0x005382C0` | `ImPlot::ShadedRenderer<ImPlot::GetterYs<float…` | `aiMap::PredictAmbIntersectionPath` | CONF | map |
| `0x00538530` | `ImPlot::ShadedRenderer<ImPlot::GetterYs<float…` | `aiMap::PredictAmbFreewayIntersectionPath` | CONF | map |
| `0x00538A10` | `ImPlot::ShadedRenderer<ImPlot::GetterYs<__int…` | `aiMap::ChooseNextRandomLink` | CONF | map |
| `0x0053B700` | `ImPlot::StairsRenderer<ImPlot::GetterYs<float…` | `aiMap::RemovePlayer` | CONF | map |
| `0x0053BB80` | `ImPlot::StairsRenderer<ImPlot::GetterYs<__int…` | `_dynamic_initializer (CRT slot 414, aiMap TU)` | CONF | CRT init table slot |
| `0x0053BC10` | `ImPlot::StairsRenderer<ImPlot::GetterYs<__int…` | `_dynamic_initializer (CRT slot 417, aiMap TU)` | CONF | CRT init table slot |
| `0x0053BCA0` | `ImPlot::StairsRenderer<ImPlot::GetterYs<__int…` | `aiVehicleAmbient::`vector deleting destructor'` | CONF | map |
| `0x0053BDC0` | `ImPlot::StairsRenderer<ImPlot::GetterYs<unsig…` | `aiPedestrian::`vector deleting destructor'` | CONF | map |
| `0x0053BEE0` | `ImPlot::StairsRenderer<ImPlot::GetterYs<unsig…` | `aiSubway::`vector deleting destructor'` | CONF | map |
| `0x0053C000` | `ImPlot::StairsRenderer<ImPlot::GetterFuncPtr,…` | `aiIntersection::Road` | CONF | map |
| `0x0053D010` | `ImPlot::GetterError<int>::operator` | `_dynamic_initializer (CRT slot 425, aiPedAudio TU)` | CONF | CRT init table slot |
| `0x0053D3B0` | `ImPlot::GetterError<__int64>::operator` | `aiRouteRacer::Update` | CONF | map |
| `0x0053D4B0` | `ImPlot::GetterError<unsigned __int64>::operat…` | `aiRouteRacer::DriveRoute` | CONF | map |
| `0x0053D910` | `ImPlot::GetterXsYRef<__int64>::operator` | `aiPoliceOfficer::aiPoliceOfficer` | CONF | map |
| `0x0053E360` | `ImPlot::GetterYs<unsigned __int64>::operator` | `aiPoliceOfficer::Speeding` | CONF | map |
| `0x0053E3F0` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterXs…` | `aiPoliceOfficer::OffRoad` | CONF | map |
| `0x0053F290` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterXs…` | `_dynamic_initializer (CRT slot 429, aiPoliceOfficer TU)` | CONF | CRT init table slot |
| `0x0053F360` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterXs…` | `aiVehiclePlayer::Reset` | CONF | map + vtable slot 10 + virtual |
| `0x0053F5D0` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterXs…` | `aiObstacle::InAccident` | CONF | map + vtable slot 0 + virtual |
| `0x0053F6A0` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterXs…` | `aiVehiclePlayer::LSideDistance` | CONF | map + vtable slot 15 + virtual |
| `0x005407B0` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterYs…` | `aiCableCar::CheckForObstacles` | CONF | map |
| `0x00540A20` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterYs…` | `aiCableCar::OkayToEnterIntersection` | CONF | map |
| `0x00541650` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterYs…` | `aiCableCarInstance::GetPosition` | CONF | map + vtable slot 1 + virtual |
| `0x005417F0` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterYs…` | `aiCableCar::CurrentLane` | CONF | map + vtable slot 17 + virtual |
| `0x005418C0` | `ImPlot::LineSegmentsRenderer<ImPlot::GetterYs…` | `(unnamed empty stub)` | UNID | map entry is a placeholder; body is empty |
| `0x00543AD0` | `ImPlot::LineStripRenderer<ImPlot::GetterXsYs<…` | `aiSubway::DrawId` | CONF | map + vtable slot 19 + virtual |
| `0x00543C90` | `ImPlot::LineStripRenderer<ImPlot::GetterXsYs<…` | `aiSubway::Type` | CONF | map + vtable slot 11 + virtual |
| `0x005482F0` | `ImPlot::RectRenderer<ImPlot::GetterFuncPtr,Im…` | `aiPath::SharpTurnEndDir` | CONF | map |
| `0x0054A170` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<sig…` | `aiIntersection::AddBangersToObsMap` | CONF | map |
| `0x00550350` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<int…` | `_dynamic_initializer (aiPedestrian TU) - flt_6B316C` | CONF | CRT init table slot |
| `0x00551AF0` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<uns…` | `_dynamic_initializer (CRT slot 453, aiVehicleAmbient TU)` | CONF | CRT init table slot |
| `0x00552870` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<uns…` | `aiVehicleInstance::DrawPart` | CONF | map |
| `0x00552F30` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<flo…` | `aiVehicleInstance::DrawShadowMap` | CONF | map + vtable slot 13 + virtual |
| `0x00553290` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<flo…` | `aiVehicleActive::Reset` | CONF | map |
| `0x00554370` | `ImPlot::ShadedRenderer<ImPlot::GetterXsYs<flo…` | `aiVehicleManager::`scalar deleting destructor'` | CONF | map + vtable slot 0 + virtual |
| `0x0055F9B0` | `ImPlot::ShadedRenderer<ImPlot::GetterYs<doubl…` | `aiVehiclePhysics::InitRoadTurns` | CONF | map |
| `0x005671B0` | `ImPlot::StairsRenderer<ImPlot::GetterYs<short…` | `_dynamic_initializer (aiVehiclePhysics TU) - flt_6B3274` | CONF | CRT init table slot |
| `0x0056A680` | `ImPlot::TransformerLogLog::operator` | `_dynamic_initializer (aiVehicleSpline TU) - flt_6B3290` | CONF | CRT init table slot |
| `0x0056B6B0` | `ImPlot::RenderMarkerRight` | `_dynamic_initializer (aiGoalAvoidPlayer TU) - flt_6B32A0` | CONF | CRT init table slot |
| `0x005715F0` | `std::_Wrap_alloc<std::allocator<std::_Contain…` | `asNetwork::GetNumSessions` | CONF | map |
| `0x00571700` | `std::_Compressed_pair<std::_Wrap_alloc<std::a…` | `asNetwork::UnSealSession` | CONF | map |
| `0x00571AB0` | `??$?MDU?$char_traits@D@std@@V?$allocator@D@1@…` | `asNetwork::GetSessionsAsynch` | CONF | map |
| `0x00571B90` | `std::_Tree_comp_alloc<std::_Tmap_traits<std::…` | `asNetwork::PollLobby` | CONF | map |
| `0x00571D50` | `std::_Tree<std::_Tmap_traits<std::string,std:…` | `asNetwork::HandleAppMessage` | CONF | map |
| `0x00572640` | `std::_Pocma<std::_Wrap_alloc<std::allocator<c…` | `EnumConnectionsCallback` | CONF | map |
| `0x005726D0` | `std::addressof<std::pair<std::string const,st…` | `EnumPlayersCallback` | CONF | map |
| `0x005729C0` | `std::allocator<std::_Tree_node<std::pair<std:…` | `netZoneScore::~netZoneScore` | CONF | map |
| `0x00572AA0` | `std::forward<std::pair<std::string const,std:…` | `netZoneScore::SendGameStart` | CONF | map |
| `0x00572AD0` | `std::forward<std::_Wrap_alloc<std::allocator<…` | `netZoneScore::SendGameStartStaging` | CONF | map |
| `0x00572B00` | `std::forward<std::pair<std::string,std::strin…` | `netZoneScore::SendGameEnd` | CONF | map |
| `0x00572B50` | `std::map<std::string,std::string>::insert<std…` | `netZoneScore::InitResults` | CONF | map |
| `0x00572BC0` | `std::make_pair<std::string &,std::string &>` | `netZoneScore::InitResults (chunk, +112) - releases pZoneScore` | CONF | inside a mapped function (IDA chunk) |
| `0x00572D20` | `std::_Iterator012<std::bidirectional_iterator…` | `zipMultiAutoInit` | CONF | map |
| `0x00572F80` | `std::_String_iterator<std::_String_val<std::_…` | `zipHandle::Read` | CONF | map |
| `0x00573210` | `std::_Tree_iterator<std::_Tree_val<std::_Tree…` | `zipFile::zipRead` | CONF | map |
| `0x00573480` | `std::string::string` | `zipFile::Init` | CONF | map |
| `0x00573B60` | `std::pair<std::string,std::string>::~pair<std…` | `zipFile::EnumFiles` | CONF | map |
| `0x005745A0` | `??G?$_String_iterator@V?$_String_val@U?$_Simp…` | `zcfree` | CONF | map |
| `0x005753D0` | `std::_Tree<std::_Tmap_traits<std::string,std:…` | `inflate_codes_new` | CONF | map |
| `0x00575410` | `std::_Tree_val<std::_Tree_simple_types<std::p…` | `inflate_codes` | CONF | map |
| `0x00575B50` | `std::string::_Traits_compare` | `inflate_trees_bits` | CONF | map |
| `0x00575C00` | `std::allocator<std::_Tree_node<std::pair<std:…` | `huft_build` | PROB | 3 call sites + zlib structure |
| `0x00577410` | `std::string::replace` | `gizBridge::gizBridge` | CONF | map |
| `0x005776D0` | `std::string::substr` | `gizBridge::Update` | CONF | map |
| `0x0057C4A0` | `std::initializer_list<unsigned char>::initial…` | `_dynamic_initializer (CRT slot 525, pedActive TU)` | CONF | CRT init table slot |
| `0x0059A1E0` | `std::_Vector_alloc<std::_Vec_base_types<std::…` | `UnderscoreSpaces` | CONF | map |
| `0x0059BF80` | `std::_Unchecked<std::string *>` | `ltLensFlare::~ltLensFlare` | CONF | map |
| `0x0059BFA0` | `std::_Unfancy<std::string>` | `ltLensFlare::DrawBegin` | CONF | map |
| `0x0059C5B0` | `std::allocator_traits<std::allocator<std::str…` | `ltProjection::MakeOrthoMatrix` | CONF | map |
| `0x0059D230` | `std::allocator<std::string>::allocator<std::s…` | `ltProjection::DrawDebug` | CONF | map |
| `0x0059D6D0` | `std::_Compressed_pair<std::_Wrap_alloc<std::a…` | `aiSubwayAudio::Deactivate` | CONF | map |
| `0x0059E1D0` | `??_G?$basic_string@DU?$char_traits@D@std@@V?$…` | `_dynamic_initializer (vehWheelCheap TU) - flt_6B4C04` | CONF | CRT init table slot |
| `0x005A7060` | `std::_Vector_alloc<std::_Vec_base_types<std::…` | `audCDObject::GetTrackTime` | CONF | map |
| `0x005A74D0` | `std::vector<std::string>::_Grow_to` | `audSoundBankHdr::GetSoundIndex` | CONF | map |
| `0x005A7590` | `std::vector<std::string>::_Inside` | `EffectBase::OriginalBufferPlaying` | CONF | map |
| `0x005A7F70` | `std::vector<std::string>::_Xlen` | `dgRagdoll::Reset` | CONF | map |
| `0x005A7FB0` | `std::_Wrap_alloc<std::allocator<std::string>>…` | `dgRagdoll::Update` | CONF | map |
| `0x005A8220` | `std::string::find` | `dgLink::Reset` | CONF | map |
| `0x005A9B60` | `std::_Wrap_alloc<std::allocator<std::string>>…` | `__unwindfunclet$?InitGameObjects@mmMultiBlitz@@UAEXXZ` | CONF | _SEH region + __CxxFrameHandler |
| `0x005A9BB0` | `<eax>` | `__ehhandler$?InitMyPlayer@mmMultiCircuit@@UAEXXZ` | CONF | _SEH region + __CxxFrameHandler |
| `0x005A9BF0` | `<eax>` | `__ehhandler$?InitNetworkPlayers@mmMultiCircuit@@UAEXXZ` | CONF | _SEH region + __CxxFrameHandler |
| `0x005A9DD0` | `std::vector<std::string>::push_back` | `__ehhandler$?InitMyPlayer@mmMultiRace@@UAEXXZ` | CONF | _SEH region + __CxxFrameHandler |

## 7. Reproducing this

Nothing here needs IDA. The inputs are `data/symbols.json` (already in the repo, built from
`midtown2.map` by `tools/symbols.py`), `MM2_RE_KIT/MM2_PSEUDOCODE/_MISLABELED_INDEX.csv`, and
`midtown2.exe` itself for the vftable and CRT-table reads. The whole job is an address join;
the work was in proving the join is sound and in classifying the 63 addresses the map does not
cover.

The same join should be run over `_INDEX.csv` as a whole. The 349 were quarantined by name
pattern, and that pattern misses contaminated entries such as `ImVector<T>::*`, `ImRect::*`,
`hook::Type<...>`, `HookConfig::*`, `GameEventDispatcher::*` and the `MM2::`-namespaced
`Matrix34`/`Matrix44`/`UIMenu` names, all of which sit on real game code.
