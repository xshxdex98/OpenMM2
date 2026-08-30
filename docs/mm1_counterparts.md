# MM1 (Open1560) counterparts for MM2 classes

Midtown Madness 2 and Midtown Madness 1 run the same Angel Studios **ARTS** engine.
Midtown Madness 1 already has a ~90%-complete open C++ reimplementation, **Open1560**,
whose full source is on this machine at
`C:\Users\xshxd\OneDrive\Desktop\Dev Workspace\Open1560Dev\Open1560\code\midtown`.
For any MM2 class with a counterpart there, that MM1 C++ is a working reference
implementation - it is the single biggest accelerator this project has.

## How to use this table

1. Find the MM2 class you are about to port.
2. Open the MM1 source path in the last column **before** reading the MM2 pseudocode.
   The MM1 headers carry researched field names, comments and struct offsets that the
   Ghidra output does not.
3. Treat the **method overlap** column as the fraction of the MM2 class you can expect
   to lift with light edits. It is computed mechanically: the share of MM2 method names
   (from `data/symbols.json`, compiler-generated deleting destructors excluded) that also
   appear as methods on the MM1 class. It undercounts where MM1 uses free functions or
   operators instead of members (`Matrix34`, `Vector3`), and it overcounts nothing.
4. `NONE` rows are genuinely new work. They are listed again at the bottom.

### Subsystem renames between the two games

| MM1 prefix | MM2 prefix | subsystem |
|---|---|---|
| `agi*` | `gfx*`, `Age*` | graphics device / pipeline |
| `agiworld`, `DLP*` | `mod*`, `gfxPacket` | geometry / models |
| `mmcity*`, `mmInstance` | `lvl*`, `city*` | world, rooms, placed instances |
| `mmcar`, `mm*Car*` | `veh*` | vehicle dynamics and car audio |
| `mmphysics`, `mmdyna`, `asBound` | `ph*` | collision and rigid bodies |
| `mmbangers`, `mmPhysicsMGR` | `dg*` | dynamic props, physics manager |
| `mmaudio` (`AudManager`, `AudSound`) | `Aud*`, `DirSnd` | legacy audio (still shared) |
| -- | `aud*`, `DMusic*` | new Angel audio + DirectMusic (MM2 only) |
| `mmcamcs` (`*CamCS`) | `cam*CS` | camera coordinate systems |
| `mmanim` (`mmAnim*`, `mmBridge*`) | `giz*` | scripted city gizmos |
| `mmeffects` | `fx*`, `ptx*`, `as*` | effects and particles |
| `data7`, `stream` | `dat*` | parsers, streams, asset manager |
| `memory` (`asMemoryAllocator`) | `mem*` | allocators |
| `arts7`, `eventq7`, `vector7` | `as*`, `eq*`, `Matrix*`/`Vector*` | **unchanged** |

### Scope

The 160 MM2 classes below are the top 120 by `.text` bytes (from `data/symbols.json`, the same source as `docs/inventory.md`) plus every `as*`, `dat*`, `eq*`, `mem*`, `Matrix*` and `Vector*` class regardless of size.
Section 6 then adds an appendix of smaller classes that were checked the same way.
Every counterpart claimed here was verified against the MM1 header it is cited from;
where a plausible counterpart could not be confirmed the row says `PARTIAL` or `NONE`
rather than guessing.

## Summary

Sections 1-5 (the 160 in-scope classes; the section 6 appendix is not counted here):

| Match kind | Classes | MM2 code bytes covered |
|---|--:|--:|
| IDENTICAL | 69 | 497,792 |
| RENAMED | 28 | 155,472 |
| PARTIAL | 36 | 271,694 |
| NONE | 27 | 167,474 |
| **total** | **160** | **1,092,432** |

## 1. IDENTICAL -- same class name, same class

MM1 C++ can be adapted almost directly. Port these first.

| MM2 class | Code bytes | Open1560 counterpart | Match | Method overlap | MM1 source |
|---|--:|---|---|--:|---|
| `mmInterface` | 43587 | `mmInterface` | IDENTICAL | 77% (66/86) | `Open1560Dev/Open1560/code/midtown/mmgame/interface.h` |
| `aiMap` | 30027 | `aiMap` | IDENTICAL | 61% (43/71) | `Open1560Dev/Open1560/code/midtown/mmai/aiMap.h` |
| `aiPedestrian` | 24286 | `aiPedestrian` | IDENTICAL | 68% (26/38) | `Open1560Dev/Open1560/code/midtown/mmai/aiPedestrian.h` |
| `aiPath` | 23488 | `aiPath` | IDENTICAL | 41% (28/69) | `Open1560Dev/Open1560/code/midtown/mmai/aiPath.h` |
| `mmInput` | 17603 | `mmInput` | IDENTICAL | 82% (55/67) | `Open1560Dev/Open1560/code/midtown/mminput/input.h` |
| `Matrix34` | 16528 | `Matrix34` | IDENTICAL | 29% (19/65) | `Open1560Dev/Open1560/code/midtown/vector7/matrix34.h` |
| `mmMultiCR` | 16226 | `mmMultiCR` | IDENTICAL | 88% (42/48) | `Open1560Dev/Open1560/code/midtown/mmgame/multicr.h` |
| `mmPlayer` | 13616 | `mmPlayer` | IDENTICAL | 53% (18/34) | `Open1560Dev/Open1560/code/midtown/mmgame/player.h` |
| `aiGoalRandomDrive` | 13488 | `aiGoalRandomDrive` | IDENTICAL | 78% (14/18) | `Open1560Dev/Open1560/code/midtown/mmai/aiGoalRandomDrive.h` |
| `MenuManager` | 13272 | `MenuManager` | IDENTICAL | 93% (51/55) | `Open1560Dev/Open1560/code/midtown/mmwidget/manager.h` |
| `mmGameMulti` | 12230 | `mmGameMulti` | IDENTICAL | 57% (32/56) | `Open1560Dev/Open1560/code/midtown/mmgame/gamemulti.h` |
| `mmGame` | 11715 | `mmGame` | IDENTICAL | 63% (19/30) | `Open1560Dev/Open1560/code/midtown/mmgame/game.h` |
| `asNetwork` | 10944 | `asNetwork` | IDENTICAL | 73% (44/60) | `Open1560Dev/Open1560/code/midtown/mmnetwork/network.h` |
| `aiVehicleSpline` | 10839 | `aiVehicleSpline` | IDENTICAL | 51% (19/37) | `Open1560Dev/Open1560/code/midtown/mmai/aiVehicleSpline.h` |
| `UIMenu` | 10440 | `UIMenu` | IDENTICAL | 96% (52/54) | `Open1560Dev/Open1560/code/midtown/mmwidget/menu.h` |
| `VehicleSelectBase` | 9749 | `VehicleSelectBase` | IDENTICAL | 82% (23/28) | `Open1560Dev/Open1560/code/midtown/mmui/vselect.h` |
| `aiRailSet` | 9552 | `aiRailSet` | IDENTICAL | 77% (10/13) | `Open1560Dev/Open1560/code/midtown/mmai/aiRailSet.h` |
| `RaceMenuBase` | 9360 | `RaceMenuBase` | IDENTICAL | 46% (12/26) | `Open1560Dev/Open1560/code/midtown/mmui/racebase.h` |
| `mmWaypoints` | 8752 | `mmWaypoints` | IDENTICAL | 83% (24/29) | `Open1560Dev/Open1560/code/midtown/mmgame/waypoints.h` |
| `NetArena` | 7850 | `NetArena` | IDENTICAL | 54% (15/28) | `Open1560Dev/Open1560/code/midtown/mmui/netarena.h` |
| `mmNetObject` | 7432 | `mmNetObject` | IDENTICAL | 55% (11/20) | `Open1560Dev/Open1560/code/midtown/mmgame/netobject.h` |
| `mmMultiBlitz` | 7376 | `mmMultiBlitz` | IDENTICAL | 93% (13/14) | `Open1560Dev/Open1560/code/midtown/mmgame/multiblitz.h` |
| `mmMultiRace` | 7261 | `mmMultiRace` | IDENTICAL | 80% (12/15) | `Open1560Dev/Open1560/code/midtown/mmgame/multirace.h` |
| `mmMultiCircuit` | 7134 | `mmMultiCircuit` | IDENTICAL | 93% (13/14) | `Open1560Dev/Open1560/code/midtown/mmgame/multicircuit.h` |
| `string` | 6995 | `string` | IDENTICAL | 41% (9/22) | `Open1560Dev/Open1560/code/midtown/data7/str.h` |
| `mmSingleCircuit` | 6976 | `mmSingleCircuit` | IDENTICAL | 90% (19/21) | `Open1560Dev/Open1560/code/midtown/mmgame/singlecircuit.h` |
| `mmPopup` | 6754 | `mmPopup` | IDENTICAL | 80% (16/20) | `Open1560Dev/Open1560/code/midtown/mmgame/popup.h` |
| `mmSingleBlitz` | 6432 | `mmSingleBlitz` | IDENTICAL | 95% (18/19) | `Open1560Dev/Open1560/code/midtown/mmgame/singleblitz.h` |
| `AudioOptions` | 6418 | `AudioOptions` | IDENTICAL | 66% (19/29) | `Open1560Dev/Open1560/code/midtown/mmui/audio.h` |
| `Vector3` | 6256 | `Vector3` | IDENTICAL | 35% (14/40) | `Open1560Dev/Open1560/code/midtown/vector7/vector3.h` |
| `mmHUD` | 6187 | `mmHUD` | IDENTICAL | 84% (31/37) | `Open1560Dev/Open1560/code/midtown/mmgame/hud.h` |
| `Matrix44` | 5584 | `Matrix44` | IDENTICAL | 19% (4/21) | `Open1560Dev/Open1560/code/midtown/vector7/matrix44.h` |
| `mmPlayerData` | 5059 | `mmPlayerData` | IDENTICAL | 60% (18/30) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/playerdata.h` |
| `aiVehicleInstance` | 5048 | `aiVehicleInstance` | IDENTICAL | 42% (8/19) | `Open1560Dev/Open1560/code/midtown/mmai/aiVehicleMGR.h` |
| `mmDashView` | 4746 | `mmDashView` | IDENTICAL | 67% (10/15) | `Open1560Dev/Open1560/code/midtown/mmgame/dash.h` |
| `mmCRHUD` | 4595 | `mmCRHUD` | IDENTICAL | 89% (16/18) | `Open1560Dev/Open1560/code/midtown/mmgame/wphud.h` |
| `mmHudMap` | 4391 | `mmHudMap` | IDENTICAL | 52% (14/27) | `Open1560Dev/Open1560/code/midtown/mmgame/hudmap.h` |
| `PUResults` | 4304 | `PUResults` | IDENTICAL | 75% (12/16) | `Open1560Dev/Open1560/code/midtown/mmui/pu_results.h` |
| `UIBMButton` | 4166 | `UIBMButton` | IDENTICAL | 96% (22/23) | `Open1560Dev/Open1560/code/midtown/mmwidget/bm_button.h` |
| `asCamera` | 4112 | `asCamera` | IDENTICAL | 71% (15/21) | `Open1560Dev/Open1560/code/midtown/arts7/camera.h` |
| `aiRaceData` | 4029 | `aiRaceData` | IDENTICAL | 50% (1/2) | `Open1560Dev/Open1560/code/midtown/mmai/aiData.h` |
| `NetSelectMenu` | 4025 | `NetSelectMenu` | IDENTICAL | 94% (32/34) | `Open1560Dev/Open1560/code/midtown/mmui/netselect.h` |
| `mmPlayerConfig` | 3970 | `mmPlayerConfig` | IDENTICAL | 82% (18/22) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/playercfg.h` |
| `UICompositeScroll` | 3894 | `UICompositeScroll` | IDENTICAL | 82% (18/22) | `Open1560Dev/Open1560/code/midtown/mmwidget/compscroll.h` |
| `asBirthRule` | 3888 | `asBirthRule` | IDENTICAL | 22% (2/9) | `Open1560Dev/Open1560/code/midtown/mmeffects/birth.h` |
| `UITextScroll` | 3861 | `UITextScroll` | IDENTICAL | 84% (16/19) | `Open1560Dev/Open1560/code/midtown/mmwidget/tscroll.h` |
| `GraphicsOptions` | 3779 | `GraphicsOptions` | IDENTICAL | 80% (8/10) | `Open1560Dev/Open1560/code/midtown/mmui/graphics.h` |
| `ControlSetup` | 3743 | `ControlSetup` | IDENTICAL | 94% (17/18) | `Open1560Dev/Open1560/code/midtown/mmui/control.h` |
| `UITextRoller2` | 3720 | `UITextRoller2` | IDENTICAL | 92% (12/13) | `Open1560Dev/Open1560/code/midtown/mmwidget/textroller2.h` |
| `Dialog_HallOfFame` | 3707 | `Dialog_HallOfFame` | IDENTICAL | 86% (6/7) | `Open1560Dev/Open1560/code/midtown/mmui/dlg_hoff.h` |
| `mmJoystick` | 3697 | `mmJoystick` | IDENTICAL | 58% (15/26) | `Open1560Dev/Open1560/code/midtown/mminput/joystick.h` |
| `UITextField` | 3301 | `UITextField` | IDENTICAL | 87% (13/15) | `Open1560Dev/Open1560/code/midtown/mmwidget/textfield.h` |
| `mmTextNode` | 3253 | `mmTextNode` | IDENTICAL | 76% (13/17) | `Open1560Dev/Open1560/code/midtown/mmeffects/mmtext.h` |
| `asViewCS` | 3104 | `asViewCS` | IDENTICAL | 100% (10/10) | `Open1560Dev/Open1560/code/midtown/arts7/view.h` |
| `asDofCS` | 2528 | `asDofCS` | IDENTICAL | 83% (5/6) | `Open1560Dev/Open1560/code/midtown/arts7/dof.h` |
| `asLineSparks` | 2096 | `asLineSparks` | IDENTICAL | 80% (4/5) | `Open1560Dev/Open1560/code/midtown/mmeffects/linespark.h` |
| `asParticles` | 1939 | `asParticles` | IDENTICAL | 100% (7/7) | `Open1560Dev/Open1560/code/midtown/mmeffects/ptx.h` |
| `asNode` | 1600 | `asNode` | IDENTICAL | 61% (14/23) | `Open1560Dev/Open1560/code/midtown/arts7/node.h` |
| `Vector4` | 1248 | `Vector4` | IDENTICAL | 9% (1/11) | `Open1560Dev/Open1560/code/midtown/vector7/vector4.h` |
| `eqEventHandler` | 1186 | `eqEventHandler` | IDENTICAL | 55% (6/11) | `Open1560Dev/Open1560/code/midtown/eventq7/event.h` |
| `asCullManager` | 1120 | `asCullManager` | IDENTICAL | 89% (8/9) | `Open1560Dev/Open1560/code/midtown/arts7/cullmgr.h` |
| `asMeshSetForm` | 1051 | `asMeshSetForm` | IDENTICAL | 75% (6/8) | `Open1560Dev/Open1560/code/midtown/mmeffects/meshform.h` |
| `eqEventQ` | 755 | `eqEventQ` | IDENTICAL | 100% (5/5) | `Open1560Dev/Open1560/code/midtown/eventq7/eventq.h` |
| `asSparkLut` | 544 | `asSparkLut` | IDENTICAL | 50% (1/2) | `Open1560Dev/Open1560/code/midtown/mmeffects/linespark.h` |
| `asLinearCS` | 384 | `asLinearCS` | IDENTICAL | 100% (3/3) | `Open1560Dev/Open1560/code/midtown/arts7/linear.h` |
| `eqEventMonitor` | 368 | `eqEventMonitor` | IDENTICAL | 100% (3/3) | `Open1560Dev/Open1560/code/midtown/eventq7/event.h` |
| `asNetObject` | 112 | `asNetObject` | IDENTICAL | 100% (2/2) | `Open1560Dev/Open1560/code/midtown/mmnetwork/netobject.h` |
| `Vector2` | 80 | `Vector2` | IDENTICAL | 67% (2/3) | `Open1560Dev/Open1560/code/midtown/vector7/vector2.h` |
| `asCullable` | 32 | `asCullable` | IDENTICAL | 100% (2/2) | `Open1560Dev/Open1560/code/midtown/arts7/cullable.h` |

## 2. RENAMED -- different name, same role

The subsystem prefix changed but the class is recognisably the same. These are
the highest-leverage rows in the document after the IDENTICAL block, because the
rename hides the match from a plain name search.

| MM2 class | Code bytes | Open1560 counterpart | Match | Method overlap | MM1 source |
|---|--:|---|---|--:|---|
| `gfxPipeline` | 21616 | `agiPipeline` | RENAMED | 25% (13/52) | `Open1560Dev/Open1560/code/midtown/agi/pipeline.h` |
| `phInertialCS` | 14448 | `asInertialCS` | RENAMED | 11% (5/45) | `Open1560Dev/Open1560/code/midtown/mmphysics/inertia.h` |
| `dgTrailerJoint` | 11904 | `Joint3Dof` | RENAMED | 61% (17/28) | `Open1560Dev/Open1560/code/midtown/mmphysics/joint3dof.h` |
| `camTrackCS` | 9773 | `TrackCamCS` | RENAMED | 83% (15/18) | `Open1560Dev/Open1560/code/midtown/mmcamcs/trackcamcs.h` |
| `vehWheel` | 8838 | `mmWheel` | RENAMED | 47% (9/19) | `Open1560Dev/Open1560/code/midtown/mmcar/wheel.h` |
| `vehCarModel` | 8672 | `mmCarModel` | RENAMED | 37% (10/27) | `Open1560Dev/Open1560/code/midtown/mmcar/carmodel.h` |
| `dgPhysManager` | 7712 | `mmPhysicsMGR` | RENAMED | 52% (12/23) | `Open1560Dev/Open1560/code/midtown/mmphysics/phys.h` |
| `lvlInstance` | 7250 | `mmInstance` | RENAMED | 25% (11/44) | `Open1560Dev/Open1560/code/midtown/mmcity/inst.h` |
| `mmSingleRace` | 6928 | `mmGameSingle` | RENAMED | 86% (18/21) | `Open1560Dev/Open1560/code/midtown/mmgame/gamesingle.h` |
| `aiPoliceOfficer` | 6768 | `aiGoalChase` | RENAMED | 61% (14/23) | `Open1560Dev/Open1560/code/midtown/mmai/aiGoalChase.h` |
| `vehCarSim` | 6572 | `mmCarSim` | RENAMED | 55% (11/20) | `Open1560Dev/Open1560/code/midtown/mmcar/carsim.h` |
| `lvlAiMap` | 5920 | `mmRoadSect` | RENAMED | 29% (10/35) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/roadsect.h` |
| `vehSurfaceAudio` | 5165 | `mmSurfaceAudio` | RENAMED | 35% (8/23) | `Open1560Dev/Open1560/code/midtown/mmcar/surfaceaudio.h` |
| `vehPoliceCarAudio` | 4890 | `mmPoliceCarAudio` | RENAMED | 32% (10/31) | `Open1560Dev/Open1560/code/midtown/mmcar/policecaraudio.h` |
| `vehCar` | 4550 | `mmCar` | RENAMED | 35% (7/20) | `Open1560Dev/Open1560/code/midtown/mmcar/car.h` |
| `vehCarAudio` | 4490 | `mmPlayerCarAudio` | RENAMED | 24% (7/29) | `Open1560Dev/Open1560/code/midtown/mmcar/playercaraudio.h` |
| `vehTrailer` | 3993 | `mmTrailer` | RENAMED | 38% (6/16) | `Open1560Dev/Open1560/code/midtown/mmcar/trailer.h` |
| `AudSoundBase` | 3606 | `AudSound` | RENAMED | 59% (29/49) | `Open1560Dev/Open1560/code/midtown/mmaudio/sound.h` |
| `mmMultiRoam` | 3488 | `mmMultiRace` | RENAMED | 92% (12/13) | `Open1560Dev/Open1560/code/midtown/mmgame/multirace.h` |
| `memMemoryAllocator` | 2616 | `asMemoryAllocator` | RENAMED | 65% (11/17) | `Open1560Dev/Open1560/code/midtown/memory/allocator.h` |
| `datParser` | 2336 | `MiniParser` | RENAMED | 22% (2/9) | `Open1560Dev/Open1560/code/midtown/data7/miniparser.h` |
| `datBaseTokenizer` | 1376 | `Tokenizer` | RENAMED | 21% (3/14) | `Open1560Dev/Open1560/code/midtown/stream/token.h` |
| `asMeshCardInfo` | 992 | `agiMeshCardInfo` | RENAMED | 33% (1/3) | `Open1560Dev/Open1560/code/midtown/agiworld/meshrend.h` |
| `datCallback` | 432 | `Callback` | RENAMED | 100% (2/2) | `Open1560Dev/Open1560/code/midtown/data7/callback.h` |
| `asRoot` | 400 | `asSimulation` | RENAMED | 78% (7/9) | `Open1560Dev/Open1560/code/midtown/arts7/sim.h` |
| `memSafeHeap` | 369 | `asSafeHeap` | RENAMED | 100% (6/6) | `Open1560Dev/Open1560/code/midtown/memory/valloc.h` |
| `datMemStream` | 304 | `Stream` | RENAMED | 83% (5/6) | `Open1560Dev/Open1560/code/midtown/stream/stream.h` |
| `datTokenizer` | 64 | `Tokenizer` | RENAMED | 100% (1/1) | `Open1560Dev/Open1560/code/midtown/stream/token.h` |

## 3. PARTIAL -- related but materially different

Read the MM1 class for the algorithm and the field names, but expect to write the
MM2 class yourself.

| MM2 class | Code bytes | Open1560 counterpart | Match | Method overlap | MM1 source |
|---|--:|---|---|--:|---|
| `aiVehiclePhysics` | 60064 | `aiVehicleSpline` | PARTIAL | 10% (6/63) | `Open1560Dev/Open1560/code/midtown/mmai/aiVehicleSpline.h` |
| `phBoundBox` | 27584 | `mmBoundTemplate` | PARTIAL | 7% (2/30) | `Open1560Dev/Open1560/code/midtown/mmdyna/bndtmpl.h` |
| `mmSingleStunt` | 20686 | `mmSingleBlitz` | PARTIAL | 47% (16/34) | `Open1560Dev/Open1560/code/midtown/mmgame/singleblitz.h` |
| `phBoundPolygonal` | 18960 | `mmBoundTemplate` | PARTIAL | 0% (0/28) | `Open1560Dev/Open1560/code/midtown/mmdyna/bndtmpl.h` |
| `phBoundTerrain` | 16835 | `mmBoundTemplate` | PARTIAL | 10% (2/20) | `Open1560Dev/Open1560/code/midtown/mmdyna/bndtmpl.h` |
| `modModel` | 11831 | `agiMeshSet` | PARTIAL | 7% (1/14) | `Open1560Dev/Open1560/code/midtown/agiworld/meshset.h` |
| `cityLevel` | 11696 | `mmCullCity` | PARTIAL | 6% (2/35) | `Open1560Dev/Open1560/code/midtown/mmcity/cullcity.h` |
| `aiSubway` | 9075 | `aiVehicleSpline` | PARTIAL | 30% (8/27) | `Open1560Dev/Open1560/code/midtown/mmai/aiVehicleSpline.h` |
| `aiCableCar` | 8213 | `aiVehicleSpline` | PARTIAL | 31% (8/26) | `Open1560Dev/Open1560/code/midtown/mmai/aiVehicleSpline.h` |
| `phBoundGeometry` | 7261 | `mmBoundTemplate` | PARTIAL | 21% (4/19) | `Open1560Dev/Open1560/code/midtown/mmdyna/bndtmpl.h` |
| `gfxTexture` | 6096 | `agiTexDef` | PARTIAL | 3% (1/29) | `Open1560Dev/Open1560/code/midtown/agi/texdef.h` |
| `gfxRenderState` | 5664 | `agiRendState` | PARTIAL | 0% (0/19) | `Open1560Dev/Open1560/code/midtown/agi/rsys.h` |
| `gfxPacket` | 5552 | `agiMeshSet` | PARTIAL | 7% (2/27) | `Open1560Dev/Open1560/code/midtown/agiworld/meshset.h` |
| `mmReplayManager` | 5405 | `mmGameRecord` | PARTIAL | 5% (1/22) | `Open1560Dev/Open1560/code/midtown/mmcamtour/gamerecord.h` |
| `vehCarDamage` | 5208 | `mmDamage` | PARTIAL | 20% (3/15) | `Open1560Dev/Open1560/code/midtown/mmeffects/damage.h` |
| `phPolygon` | 4896 | `mmPolygon` | PARTIAL | 8% (1/12) | `Open1560Dev/Open1560/code/midtown/mmdyna/poly.h` |
| `ptxGlass` | 4875 | `mmShardManager` | PARTIAL | 40% (2/5) | `Open1560Dev/Open1560/code/midtown/mmcar/shard.h` |
| `aiAmbientVehicleAudio` | 4859 | `aiVehicleAmbient` | PARTIAL | 28% (7/25) | `Open1560Dev/Open1560/code/midtown/mmai/aiVehicleAmbient.h` |
| `HostRaceMenu` | 4679 | `HostRaceMenu` | PARTIAL | 6% (1/18) | `Open1560Dev/Open1560/code/midtown/mmui/racehost.h` |
| `ltLight` | 4672 | `agiLight` | PARTIAL | 5% (1/21) | `Open1560Dev/Open1560/code/midtown/agi/light.h` |
| `CrashCourse` | 4391 | `CRSettings` | PARTIAL | 7% (1/14) | `Open1560Dev/Open1560/code/midtown/mmui/cr_settings.h` |
| `MainMenu` | 4092 | `DriverMenu` | PARTIAL | 61% (11/18) | `Open1560Dev/Open1560/code/midtown/mmui/driver.h` |
| `mmRaceSpeech` | 3952 | `mmVoiceCommentary` | PARTIAL | 23% (5/22) | `Open1560Dev/Open1560/code/midtown/mmaudio/mmvoicecommentary.h` |
| `Aud3DObjectManager` | 3491 | `aiAudioManager` | PARTIAL | 45% (10/22) | `Open1560Dev/Open1560/code/midtown/mmai/aiaudiomanager.h` |
| `mmPlayerCityRecord` | 3424 | `mmPlayerRecord` | PARTIAL | 25% (3/12) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/playerdata.h` |
| `datArgParser` | 1824 | `ArgSet` | PARTIAL | 0% (0/7) | `Open1560Dev/Open1560/code/midtown/data7/args.h` |
| `datAssetManager` | 1648 | `FileSystem` | PARTIAL | 0% (0/8) | `Open1560Dev/Open1560/code/midtown/stream/fsystem.h` |
| `datAsciiTokenizer` | 1136 | `Tokenizer` | PARTIAL | 22% (2/9) | `Open1560Dev/Open1560/code/midtown/stream/token.h` |
| `datTimeManager` | 816 | `asSimulation` | PARTIAL | 80% (4/5) | `Open1560Dev/Open1560/code/midtown/arts7/sim.h` |
| `asFileIO` | 777 | `asNode` | PARTIAL | 44% (4/9) | `Open1560Dev/Open1560/code/midtown/arts7/node.h` |
| `datBinTokenizer` | 688 | `Tokenizer` | PARTIAL | 22% (2/9) | `Open1560Dev/Open1560/code/midtown/stream/token.h` |
| `datReplay` | 576 | `eqReplay` | PARTIAL | 0% (0/10) | `Open1560Dev/Open1560/code/midtown/eventq7/replay.h` |
| `datMultiTokenizer` | 368 | `Tokenizer` | PARTIAL | 33% (1/3) | `Open1560Dev/Open1560/code/midtown/stream/token.h` |
| `datRefCount` | 176 | `agiRefreshable` | PARTIAL | 33% (1/3) | `Open1560Dev/Open1560/code/midtown/agi/refresh.h` |
| `asUnderlay` | 160 | `asMeshSetForm` | PARTIAL | 50% (2/4) | `Open1560Dev/Open1560/code/midtown/mmeffects/meshform.h` |
| `datParserRecord` | 64 | `MiniParser` | PARTIAL | 100% (1/1) | `Open1560Dev/Open1560/code/midtown/data7/miniparser.h` |

### Notes on the PARTIAL rows

- **`aiVehiclePhysics`** (60,064 bytes) -> `aiVehicleSpline` -- MM2 name is misleading: this is road/route following, not physics. MM1 splits the same job across aiVehicleSpline + aiGoalFollowWayPts + aiPath; the route planner and shortcut logic are MM2-only.
- **`phBoundBox`** (27,584 bytes) -> `mmBoundTemplate` -- Same job (box collision) but MM2 is a separate ph bound hierarchy with SAT face-dot code; MM1 folds everything into one mmBoundTemplate. Method sets are effectively disjoint.
- **`mmSingleStunt`** (20,686 bytes) -> `mmSingleBlitz` -- MM2-only game mode, but its vtable and lifecycle are the shared mmGameSingle mode shape - the MM1 single-mode classes are the template to copy.
- **`phBoundPolygonal`** (18,960 bytes) -> `mmBoundTemplate` -- Polygon-soup bound. MM1 equivalent geometry lives in mmBoundTemplate::Collide/LineGeometry; structure is different.
- **`phBoundTerrain`** (16,835 bytes) -> `mmBoundTemplate` -- Terrain grid bound. MM1 has the same row-bucket grid idea in mmBoundTemplate (DoMakeTable/LineTable) but a different API.
- **`modModel`** (11,831 bytes) -> `agiMeshSet` -- MM2 .mod geometry container. MM1 stores geometry in agiMeshSet/DLPTemplate; file formats and APIs differ entirely.
- **`cityLevel`** (11,696 bytes) -> `mmCullCity` -- City root. MM1 splits this across mmCullCity (culling/rooms), mmLoader and mmCityInfo; MM2 folds PVS, warps, lighting and probes into one class.
- **`aiSubway`** (9,075 bytes) -> `aiVehicleSpline` -- Subway train. Derives from the same spline-vehicle base as MM1 ambient traffic; the rail-link and curve solving are MM2-only.
- **`aiCableCar`** (8,213 bytes) -> `aiVehicleSpline` -- Cable car. Same spline-vehicle base, MM2-only link/sister logic.
- **`phBoundGeometry`** (7,261 bytes) -> `mmBoundTemplate` -- Generic polygonal bound; see phBoundBox.
- **`gfxTexture`** (6,096 bytes) -> `agiTexDef` -- Texture object. Same role; MM2 owns a global texture cache and mip/LOD control, MM1 agiTexDef owns surface paging. Almost no shared method names.
- **`gfxRenderState`** (5,664 bytes) -> `agiRendState` -- Render-state cache. Same idea, zero shared method names - MM2 is a D3D state-block flusher.
- **`gfxPacket`** (5,552 bytes) -> `agiMeshSet` -- Vertex/index packet. MM1 agiMeshSet is the nearest thing but is a mesh container, not a VB wrapper.
- **`mmReplayManager`** (5,405 bytes) -> `mmGameRecord` -- Replay record/playback. MM1 mmGameRecord (mmcamtour) records the same inputs but is far smaller and shares almost no method names.
- **`vehCarDamage`** (5,208 bytes) -> `mmDamage` -- Damage accumulation and deformation. MM1 mmDamage is only the visual texel damage; MM2 also does impact bookkeeping and smoke.
- **`phPolygon`** (4,896 bytes) -> `mmPolygon` -- Collision polygon. Same concept, different segment/edge test API.
- **`ptxGlass`** (4,875 bytes) -> `mmShardManager` -- Breaking glass particles. MM1 mmShard/mmShardManager is the same effect with a different API.
- **`aiAmbientVehicleAudio`** (4,859 bytes) -> `aiVehicleAmbient` -- Ambient traffic audio. In MM1 the audio lives on aiVehicleAmbient / mmOpponentCarAudio rather than a dedicated class.
- **`HostRaceMenu`** (4,679 bytes) -> `HostRaceMenu` -- Host-a-race screen. Name matches but MM2s content is closer to MM1 CRSettings (~47% of names) than to MM1 HostRaceMenu.
- **`ltLight`** (4,672 bytes) -> `agiLight` -- Light source. MM2 lt is a whole scene-lighting subsystem (glows, highlights, projections); MM1 agiLight is a pipeline resource.
- **`CrashCourse`** (4,391 bytes) -> `CRSettings` -- Crash Course front-end screen. MM1 CRSettings/RaceMenuBase are the nearest shape.
- **`MainMenu`** (4,092 bytes) -> `DriverMenu` -- Main menu. MM1 MainMenu exists but the MM2 class body matches MM1 DriverMenu (~58%) far better.
- **`mmRaceSpeech`** (3,952 bytes) -> `mmVoiceCommentary` -- Race commentary. Same job, MM2 has a much larger event vocabulary.
- **`Aud3DObjectManager`** (3,491 bytes) -> `aiAudioManager` -- Positional-audio slot manager. MM1 aiAudioManager does the same slot/priority juggling for AI voices.
- **`mmPlayerCityRecord`** (3,424 bytes) -> `mmPlayerRecord` -- Per-city best times. MM1 splits this differently across mmPlayerRecord/mmMiscData.
- **`datArgParser`** (1,824 bytes) -> `ArgSet` -- Command-line argument store. MM1 ArgSet/asArg is the same job with a different API.
- **`datAssetManager`** (1,648 bytes) -> `FileSystem` -- Path/asset resolution. MM1 FileSystem + VirtualFileSystem cover the same ground; no shared method names.
- **`datAsciiTokenizer`** (1,136 bytes) -> `Tokenizer` -- ASCII tokenizer; MM1 Tokenizer is the closest thing.
- **`datTimeManager`** (816 bytes) -> `asSimulation` -- Frame timing. MM1 keeps the same Update/Reset/Freeze timing logic inside asSimulation.
- **`asFileIO`** (777 bytes) -> `asNode` -- MM2 reflection/serialisation mix-in (FileIO/AddWidgets/DeclareFields). MM1 spreads the same reflection over MetaClass + asNode::AddWidgets.
- **`datBinTokenizer`** (688 bytes) -> `Tokenizer` -- Binary tokenizer; no direct MM1 class.
- **`datReplay`** (576 bytes) -> `eqReplay` -- Input replay channel. MM1 eqReplay/eqReplayChannel is the same feature under the eq prefix; no shared method names.
- **`datMultiTokenizer`** (368 bytes) -> `Tokenizer` -- Multi-source tokenizer; no direct MM1 class.
- **`datRefCount`** (176 bytes) -> `agiRefreshable` -- Ref-counted base. MM1 uses agiRefreshable / Rc for the same purpose.
- **`asUnderlay`** (160 bytes) -> `asMeshSetForm` -- Full-screen underlay node. No MM1 class of this name.
- **`datParserRecord`** (64 bytes) -> `MiniParser` -- Single parsed record; part of MM1 MiniParser.

## 4. Highest-value transfers, by MM2 code size

IDENTICAL + RENAMED only, largest first -- this is the recommended port order for
anything that is not already blocked.

| MM2 class | Code bytes | Open1560 counterpart | Match | Method overlap | MM1 source |
|---|--:|---|---|--:|---|
| `mmInterface` | 43587 | `mmInterface` | IDENTICAL | 77% (66/86) | `Open1560Dev/Open1560/code/midtown/mmgame/interface.h` |
| `aiMap` | 30027 | `aiMap` | IDENTICAL | 61% (43/71) | `Open1560Dev/Open1560/code/midtown/mmai/aiMap.h` |
| `aiPedestrian` | 24286 | `aiPedestrian` | IDENTICAL | 68% (26/38) | `Open1560Dev/Open1560/code/midtown/mmai/aiPedestrian.h` |
| `aiPath` | 23488 | `aiPath` | IDENTICAL | 41% (28/69) | `Open1560Dev/Open1560/code/midtown/mmai/aiPath.h` |
| `gfxPipeline` | 21616 | `agiPipeline` | RENAMED | 25% (13/52) | `Open1560Dev/Open1560/code/midtown/agi/pipeline.h` |
| `mmInput` | 17603 | `mmInput` | IDENTICAL | 82% (55/67) | `Open1560Dev/Open1560/code/midtown/mminput/input.h` |
| `Matrix34` | 16528 | `Matrix34` | IDENTICAL | 29% (19/65) | `Open1560Dev/Open1560/code/midtown/vector7/matrix34.h` |
| `mmMultiCR` | 16226 | `mmMultiCR` | IDENTICAL | 88% (42/48) | `Open1560Dev/Open1560/code/midtown/mmgame/multicr.h` |
| `phInertialCS` | 14448 | `asInertialCS` | RENAMED | 11% (5/45) | `Open1560Dev/Open1560/code/midtown/mmphysics/inertia.h` |
| `mmPlayer` | 13616 | `mmPlayer` | IDENTICAL | 53% (18/34) | `Open1560Dev/Open1560/code/midtown/mmgame/player.h` |
| `aiGoalRandomDrive` | 13488 | `aiGoalRandomDrive` | IDENTICAL | 78% (14/18) | `Open1560Dev/Open1560/code/midtown/mmai/aiGoalRandomDrive.h` |
| `MenuManager` | 13272 | `MenuManager` | IDENTICAL | 93% (51/55) | `Open1560Dev/Open1560/code/midtown/mmwidget/manager.h` |
| `mmGameMulti` | 12230 | `mmGameMulti` | IDENTICAL | 57% (32/56) | `Open1560Dev/Open1560/code/midtown/mmgame/gamemulti.h` |
| `dgTrailerJoint` | 11904 | `Joint3Dof` | RENAMED | 61% (17/28) | `Open1560Dev/Open1560/code/midtown/mmphysics/joint3dof.h` |
| `mmGame` | 11715 | `mmGame` | IDENTICAL | 63% (19/30) | `Open1560Dev/Open1560/code/midtown/mmgame/game.h` |
| `asNetwork` | 10944 | `asNetwork` | IDENTICAL | 73% (44/60) | `Open1560Dev/Open1560/code/midtown/mmnetwork/network.h` |
| `aiVehicleSpline` | 10839 | `aiVehicleSpline` | IDENTICAL | 51% (19/37) | `Open1560Dev/Open1560/code/midtown/mmai/aiVehicleSpline.h` |
| `UIMenu` | 10440 | `UIMenu` | IDENTICAL | 96% (52/54) | `Open1560Dev/Open1560/code/midtown/mmwidget/menu.h` |
| `camTrackCS` | 9773 | `TrackCamCS` | RENAMED | 83% (15/18) | `Open1560Dev/Open1560/code/midtown/mmcamcs/trackcamcs.h` |
| `VehicleSelectBase` | 9749 | `VehicleSelectBase` | IDENTICAL | 82% (23/28) | `Open1560Dev/Open1560/code/midtown/mmui/vselect.h` |
| `aiRailSet` | 9552 | `aiRailSet` | IDENTICAL | 77% (10/13) | `Open1560Dev/Open1560/code/midtown/mmai/aiRailSet.h` |
| `RaceMenuBase` | 9360 | `RaceMenuBase` | IDENTICAL | 46% (12/26) | `Open1560Dev/Open1560/code/midtown/mmui/racebase.h` |
| `vehWheel` | 8838 | `mmWheel` | RENAMED | 47% (9/19) | `Open1560Dev/Open1560/code/midtown/mmcar/wheel.h` |
| `mmWaypoints` | 8752 | `mmWaypoints` | IDENTICAL | 83% (24/29) | `Open1560Dev/Open1560/code/midtown/mmgame/waypoints.h` |
| `vehCarModel` | 8672 | `mmCarModel` | RENAMED | 37% (10/27) | `Open1560Dev/Open1560/code/midtown/mmcar/carmodel.h` |
| `NetArena` | 7850 | `NetArena` | IDENTICAL | 54% (15/28) | `Open1560Dev/Open1560/code/midtown/mmui/netarena.h` |
| `dgPhysManager` | 7712 | `mmPhysicsMGR` | RENAMED | 52% (12/23) | `Open1560Dev/Open1560/code/midtown/mmphysics/phys.h` |
| `mmNetObject` | 7432 | `mmNetObject` | IDENTICAL | 55% (11/20) | `Open1560Dev/Open1560/code/midtown/mmgame/netobject.h` |
| `mmMultiBlitz` | 7376 | `mmMultiBlitz` | IDENTICAL | 93% (13/14) | `Open1560Dev/Open1560/code/midtown/mmgame/multiblitz.h` |
| `mmMultiRace` | 7261 | `mmMultiRace` | IDENTICAL | 80% (12/15) | `Open1560Dev/Open1560/code/midtown/mmgame/multirace.h` |

## 5. No MM1 counterpart -- this is the real work

27 of the 160 classes surveyed have nothing to copy from Open1560, together accounting for **167,474 bytes** of MM2 code. They fall into four clusters:

- **The `sdl` streaming terrain system** (`sdlPage16`, `lvlSDL`). MM2 streams city
  terrain as paged display lists; MM1 has no such system at all. Largest single
  unknown in the binary.
- **The `ph` physics engine.** MM2 ships a newer Angel physics library with a bound
  class hierarchy, a persistent contact manager and oversampled integration. MM1
  resolves impacts immediately inside `asBound::Impact`. Nothing transfers.
- **The new audio stack** (`aud*`, `DMusic*`, `Aud3D*`). MM2 layers a node-pool audio
  manager and DirectMusic scoring on top of the MM1-era `AudManager`/`AudSound` code.
  Only the older half has a counterpart.
- **Character rigs and scene lighting** (`cr*`, `lt*`) plus the `Age`/`mod` geometry
  layer. MM1 has bone playback but no IK, no projected lighting and no `.mod` format.

| MM2 class | Code bytes | Open1560 counterpart | Match | Method overlap | MM1 source |
|---|--:|---|---|--:|---|
| `sdlPage16` | 43410 | -- | NONE | - | -- |
| `phContactMgr` | 15952 | -- | NONE | - | -- |
| `phBoundHotdog` | 14672 | -- | NONE | - | -- |
| `phCollision` | 7200 | -- | NONE | - | -- |
| `lvlSDL` | 6874 | -- | NONE | - | -- |
| `crLegData` | 6096 | -- | NONE | - | -- |
| `phColliderBase` | 5248 | -- | NONE | - | -- |
| `DMusicObject` | 5072 | -- | NONE | - | -- |
| `audManager` | 5072 | -- | NONE | - | -- |
| `phContact` | 4800 | -- | NONE | - | -- |
| `phBoundSphere` | 4480 | -- | NONE | - | -- |
| `vehCarAudioContainer` | 4130 | -- | NONE | - | -- |
| `phBoundTerrainLocal` | 4048 | -- | NONE | - | -- |
| `lvlLevelBound` | 4032 | -- | NONE | - | -- |
| `AgeDevice` | 3936 | -- | NONE | - | -- |
| `dgImpact` | 3696 | -- | NONE | - | -- |
| `audObject` | 3696 | -- | NONE | - | -- |
| `ltProjection` | 3648 | -- | NONE | - | -- |
| `audControl` | 3616 | -- | NONE | - | -- |
| `crAnimation` | 3601 | -- | NONE | - | -- |
| `audSound` | 3392 | -- | NONE | - | -- |
| `Aud3DAmbientObject` | 3267 | -- | NONE | - | -- |
| `phImpactBase` | 3248 | -- | NONE | - | -- |
| `Matrix33` | 2000 | -- | NONE | - | -- |
| `Matrix66` | 1232 | -- | NONE | - | -- |
| `datStack` | 864 | -- | NONE | - | -- |
| `datOutput` | 192 | -- | NONE | - | -- |

### Notes on the NONE rows

- **`sdlPage16`** (43,410 bytes) -> none -- MM2 streaming display-list terrain page. MM1 has no SDL system at all (city geometry is agiMeshSet + mmBoundTemplate).
- **`phContactMgr`** (15,952 bytes) -> none -- MM2-only persistent contact manager (held contacts, contact hashing, oversampling). MM1 resolves impacts immediately in asBound::Impact - nothing to port.
- **`phBoundHotdog`** (14,672 bytes) -> none -- Capsule ("hotdog") bound. No MM1 counterpart.
- **`phCollision`** (7,200 bytes) -> none -- MM2-only static collision-primitive dispatcher.
- **`lvlSDL`** (6,874 bytes) -> none -- Streaming display-list terrain. No MM1 counterpart.
- **`crLegData`** (6,096 bytes) -> none -- Character-rig leg IK solver. MM1 has no character rig (bnSkeleton is bone playback only).
- **`phColliderBase`** (5,248 bytes) -> none -- MM2 collider driver (gravity, air resistance, impact callbacks). MM1 folds this into mmPhysEntity/asBound.
- **`DMusicObject`** (5,072 bytes) -> none -- DirectMusic interactive-score playback. MM1 uses CD audio only.
- **`audManager`** (5,072 bytes) -> none -- MM2-only lower-level Angel audio manager (control/head/stream node pools). Distinct from the MM1-shared AudManager.
- **`phContact`** (4,800 bytes) -> none -- MM2-only contact record.
- **`phBoundSphere`** (4,480 bytes) -> none -- Sphere bound; no MM1 counterpart class.
- **`vehCarAudioContainer`** (4,130 bytes) -> none -- MM2-only aggregate that owns the per-car audio objects.
- **`phBoundTerrainLocal`** (4,048 bytes) -> none -- Per-instance local terrain bound; MM2-only.
- **`lvlLevelBound`** (4,032 bytes) -> none -- Level-wide bound used for room-scoped collision; MM2-only.
- **`AgeDevice`** (3,936 bytes) -> none -- Thin D3D7 device wrapper (AGE = Angel Graphics Engine). MM1 has no equivalent indirection.
- **`dgImpact`** (3,696 bytes) -> none -- MM2-only impact-response solver (2 large free-standing functions).
- **`audObject`** (3,696 bytes) -> none -- MM2-only Angel audio object (see audManager).
- **`ltProjection`** (3,648 bytes) -> none -- Projected-texture lighting (shadows/headlight pools). MM2-only.
- **`audControl`** (3,616 bytes) -> none -- MM2-only Angel audio control (see audManager).
- **`crAnimation`** (3,601 bytes) -> none -- Character animation table (hash of named clips). MM1 bnAnimation is bone playback only and shares nothing.
- **`audSound`** (3,392 bytes) -> none -- MM2-only Angel audio sound buffer (see audManager).
- **`Aud3DAmbientObject`** (3,267 bytes) -> none -- MM2-only 3D ambient emitter.
- **`phImpactBase`** (3,248 bytes) -> none -- MM2-only impact record base.
- **`Matrix33`** (2,000 bytes) -> none -- Pure 3x3 matrix used by the MM2 physics solver. MM1 has no Matrix33 (it uses the 3x3 part of Matrix34).
- **`Matrix66`** (1,232 bytes) -> none -- 6x6 mass matrix used by the MM2 joint solver. MM1 has no equivalent.
- **`datStack`** (864 bytes) -> none -- MM2-only small stack allocator.
- **`datOutput`** (192 bytes) -> none -- Log/print sink. MM1 has the same feature but as free functions plus a `Printer` function pointer in `data7/printer.h`, not a class - see that header for the level/format contract.

## 6. Appendix -- verified matches below the size cutoff

These 164 classes fall outside the top-120 selection but were checked the same way. Several are near-total transfers: the whole `cam*CS` camera family, most of `veh*`, and most of the `UI*` / `PU*` / `Dialog_*` front end. Included because anyone porting bottom-up (as `docs/inventory.md` recommends) will hit these first.

| MM2 class | Code bytes | Open1560 counterpart | Match | Method overlap | MM1 source |
|---|--:|---|---|--:|---|
| `aiIntersection` | 3219 | `aiIntersection` | IDENTICAL | 29% (8/28) | `Open1560Dev/Open1560/code/midtown/mmai/aiIntersection.h` |
| `modShader` | 3136 | -- | NONE | - | -- |
| `gfxImage` | 3104 | `agiBitmap` | PARTIAL | 11% (1/9) | `Open1560Dev/Open1560/code/midtown/agi/bitmap.h` |
| `dgBangerData` | 3101 | `mmBangerData` | RENAMED | 22% (2/9) | `Open1560Dev/Open1560/code/midtown/mmbangers/data.h` |
| `camAppCS` | 3088 | `AppCamCS` | RENAMED | 83% (5/6) | `Open1560Dev/Open1560/code/midtown/mmcamcs/appcamcs.h` |
| `fxTexelDamage` | 3088 | `mmDamage` | RENAMED | 60% (3/5) | `Open1560Dev/Open1560/code/midtown/mmeffects/damage.h` |
| `UICWArray` | 3082 | `UICWArray` | IDENTICAL | 81% (17/21) | `Open1560Dev/Open1560/code/midtown/mmwidget/cwarray.h` |
| `AudManager` | 3040 | `AudManager` | IDENTICAL | 56% (32/57) | `Open1560Dev/Open1560/code/midtown/mmaudio/manager.h` |
| `aiVehicleActive` | 3030 | `aiVehicleActive` | IDENTICAL | 62% (8/13) | `Open1560Dev/Open1560/code/midtown/mmai/aiVehicleMGR.h` |
| `sdlPoly` | 3024 | -- | NONE | - | -- |
| `UITextRoller` | 2981 | `UITextRoller` | IDENTICAL | 100% (12/12) | `Open1560Dev/Open1560/code/midtown/mmwidget/textroller.h` |
| `lvlLevel` | 2899 | `mmCullCity` | PARTIAL | 14% (2/14) | `Open1560Dev/Open1560/code/midtown/mmcity/cullcity.h` |
| `vehWheelCheap` | 2896 | `mmWheelCheap` | RENAMED | 100% (4/4) | `Open1560Dev/Open1560/code/midtown/mmcar/carsimcheap.h` |
| `aiVehicle` | 2896 | `aiVehicle` | IDENTICAL | 57% (4/7) | `Open1560Dev/Open1560/code/midtown/mmai/aiVehicle.h` |
| `mmIcons` | 2851 | `mmIcons` | IDENTICAL | 100% (5/5) | `Open1560Dev/Open1560/code/midtown/mmgame/icons.h` |
| `mmVehicleForm` | 2821 | `mmVehicleForm` | IDENTICAL | 80% (4/5) | `Open1560Dev/Open1560/code/midtown/mmeffects/vehform.h` |
| `DirSnd` | 2813 | `DSGlobal` | RENAMED | 81% (13/16) | `Open1560Dev/Open1560/code/midtown/mmaudio/dsglobal.h` |
| `mmPlayerDirectory` | 2784 | `mmPlayerDirectory` | IDENTICAL | 100% (18/18) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/playerdir.h` |
| `mmMiscData` | 2782 | `mmMiscData` | PARTIAL | 12% (1/8) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/miscdata.h` |
| `HashTable` | 2761 | `HashTable` | IDENTICAL | 48% (10/21) | `Open1560Dev/Open1560/code/midtown/data7/hash.h` |
| `zipFile` | 2752 | `VirtualFileSystem` | PARTIAL | 7% (1/15) | `Open1560Dev/Open1560/code/midtown/stream/vfsystem.h` |
| `mmDropDown` | 2720 | `mmDropDown` | IDENTICAL | 71% (10/14) | `Open1560Dev/Open1560/code/midtown/mmwidget/dropdown.h` |
| `mmGameManager` | 2678 | `mmGameManager` | IDENTICAL | 71% (5/7) | `Open1560Dev/Open1560/code/midtown/mmgame/gameman.h` |
| `mmCircuitHUD` | 2649 | `mmCircuitHUD` | IDENTICAL | 88% (7/8) | `Open1560Dev/Open1560/code/midtown/mmgame/wphud.h` |
| `UIVScrollBar` | 2640 | `UIVScrollBar` | IDENTICAL | 100% (17/17) | `Open1560Dev/Open1560/code/midtown/mmwidget/vscrollbar.h` |
| `phCollisionPrim` | 2592 | -- | NONE | - | -- |
| `VSWidget` | 2592 | `VSWidget` | IDENTICAL | 100% (15/15) | `Open1560Dev/Open1560/code/midtown/mmwidget/vswidget.h` |
| `aiVehicleAmbient` | 2551 | `aiVehicleAmbient` | IDENTICAL | 90% (9/10) | `Open1560Dev/Open1560/code/midtown/mmai/aiVehicleAmbient.h` |
| `MixerCTL` | 2544 | -- | NONE | - | -- |
| `aiGoalRegainRail` | 2528 | `aiGoalRegainRail` | IDENTICAL | 83% (5/6) | `Open1560Dev/Open1560/code/midtown/mmai/aiGoalRegainRail.h` |
| `mcHookman` | 2528 | -- | NONE | - | -- |
| `aiGoalAvoidPlayer` | 2512 | `aiGoalAvoidPlayer` | IDENTICAL | 89% (8/9) | `Open1560Dev/Open1560/code/midtown/mmai/aiGoalAvoidPlayer.h` |
| `AudImpact` | 2491 | `mmImpactAudio` | PARTIAL | 21% (3/14) | `Open1560Dev/Open1560/code/midtown/mmcar/impactaudio.h` |
| `gfxViewport` | 2464 | `agiViewport` | PARTIAL | 10% (1/10) | `Open1560Dev/Open1560/code/midtown/agi/viewport.h` |
| `mmWaypointObject` | 2314 | `mmWaypointObject` | IDENTICAL | 41% (7/17) | `Open1560Dev/Open1560/code/midtown/mmgame/wpobject.h` |
| `vehDrivetrain` | 2283 | `mmDrivetrain` | RENAMED | 70% (7/10) | `Open1560Dev/Open1560/code/midtown/mmcar/drivetrain.h` |
| `UISlider` | 2272 | `UISlider` | IDENTICAL | 100% (17/17) | `Open1560Dev/Open1560/code/midtown/mmwidget/slider.h` |
| `gizBridgeMgr` | 2254 | `mmBridgeMgr` | RENAMED | 38% (3/8) | `Open1560Dev/Open1560/code/midtown/mmanim/bridge.h` |
| `Dialog_RaceEnvironment` | 2219 | `Dialog_RaceEnvironment` | IDENTICAL | 100% (6/6) | `Open1560Dev/Open1560/code/midtown/mmui/dlg_renv.h` |
| `aiVehicleManager` | 2199 | `aiVehicleManager` | IDENTICAL | 88% (7/8) | `Open1560Dev/Open1560/code/midtown/mmai/aiVehicleMGR.h` |
| `AudSpeech` | 2171 | `mmVoiceCommentary` | PARTIAL | 17% (3/18) | `Open1560Dev/Open1560/code/midtown/mmaudio/mmvoicecommentary.h` |
| `camTransitionCS` | 2128 | `TransitionCS` | RENAMED | 100% (10/10) | `Open1560Dev/Open1560/code/midtown/mmcamcs/transitioncs.h` |
| `mmIODev` | 2128 | `mmIODev` | IDENTICAL | 92% (11/12) | `Open1560Dev/Open1560/code/midtown/mminput/iodev.h` |
| `mmSlider` | 2128 | `mmSlider` | IDENTICAL | 100% (17/17) | `Open1560Dev/Open1560/code/midtown/mmwidget/sliderbar.h` |
| `RadialGauge` | 2024 | `RadialGauge` | IDENTICAL | 80% (4/5) | `Open1560Dev/Open1560/code/midtown/mmgame/gauge.h` |
| `vehTransmission` | 2014 | `mmTransmission` | RENAMED | 60% (9/15) | `Open1560Dev/Open1560/code/midtown/mmcar/transmission.h` |
| `lvlSky` | 2000 | `mmSky` | RENAMED | 33% (3/9) | `Open1560Dev/Open1560/code/midtown/mmcity/sky.h` |
| `pedAnimationInstance` | 2000 | `mmPed` | PARTIAL | 33% (3/9) | `Open1560Dev/Open1560/code/midtown/mmcity/ped.h` |
| `PURoster` | 1968 | `PURoster` | IDENTICAL | 90% (9/10) | `Open1560Dev/Open1560/code/midtown/mmui/pu_roster.h` |
| `mmCDPlayer` | 1946 | `mmCDPlayer` | IDENTICAL | 100% (9/9) | `Open1560Dev/Open1560/code/midtown/mmgame/cd.h` |
| `Spline` | 1888 | `Spline` | IDENTICAL | 80% (8/10) | `Open1560Dev/Open1560/code/midtown/mmcamcs/spline.h` |
| `vehTrailerInstance` | 1872 | `mmTrailerInstance` | RENAMED | 50% (6/12) | `Open1560Dev/Open1560/code/midtown/mmcar/trailer.h` |
| `gizPathspline` | 1872 | `mmAnimSpline` | RENAMED | 29% (4/14) | `Open1560Dev/Open1560/code/midtown/mmanim/AnimSpline.h` |
| `vehEngine` | 1840 | `mmEngine` | RENAMED | 73% (8/11) | `Open1560Dev/Open1560/code/midtown/mmcar/engine.h` |
| `aiTrafficLightSet` | 1792 | `aiTrafficLightSet` | IDENTICAL | 75% (3/4) | `Open1560Dev/Open1560/code/midtown/mmai/aiTrafficLight.h` |
| `dgBangerInstance` | 1760 | `mmBangerInstance` | RENAMED | 57% (8/14) | `Open1560Dev/Open1560/code/midtown/mmbangers/banger.h` |
| `Stream` | 1744 | `Stream` | IDENTICAL | 57% (8/14) | `Open1560Dev/Open1560/code/midtown/stream/stream.h` |
| `dgLink` | 1728 | -- | NONE | - | -- |
| `mmCompRoster` | 1680 | `mmCompRoster` | IDENTICAL | 67% (12/18) | `Open1560Dev/Open1560/code/midtown/mmwidget/comproster.h` |
| `UIControlWidget` | 1679 | `UIControlWidget` | IDENTICAL | 100% (11/11) | `Open1560Dev/Open1560/code/midtown/mmwidget/controlwidget.h` |
| `phJoint` | 1664 | `Joint3Dof` | PARTIAL | 56% (5/9) | `Open1560Dev/Open1560/code/midtown/mmphysics/joint3dof.h` |
| `vehStuck` | 1662 | `mmStuck` | RENAMED | 67% (6/9) | `Open1560Dev/Open1560/code/midtown/mmcar/stuck.h` |
| `vehBreakableMgr` | 1661 | `mmShardManager` | PARTIAL | 25% (3/12) | `Open1560Dev/Open1560/code/midtown/mmcar/shard.h` |
| `mmWPHUD` | 1622 | `mmWPHUD` | IDENTICAL | 88% (7/8) | `Open1560Dev/Open1560/code/midtown/mmgame/wphud.h` |
| `aiTrafficLightInstance` | 1600 | `aiTrafficLightInstance` | IDENTICAL | 50% (2/4) | `Open1560Dev/Open1560/code/midtown/mmai/aiTrafficLight.h` |
| `vehSplash` | 1594 | `mmSplash` | RENAMED | 100% (5/5) | `Open1560Dev/Open1560/code/midtown/mmcar/splash.h` |
| `mmRainAudio` | 1584 | `mmRainAudio` | IDENTICAL | 100% (5/5) | `Open1560Dev/Open1560/code/midtown/mmgame/rainaudio.h` |
| `dgPath` | 1570 | -- | NONE | - | -- |
| `Quaternion` | 1552 | `Quaternion` | IDENTICAL | 20% (1/5) | `Open1560Dev/Open1560/code/midtown/vector7/quat.h` |
| `modStatic` | 1552 | `agiMeshSet` | PARTIAL | 10% (1/10) | `Open1560Dev/Open1560/code/midtown/agiworld/meshset.h` |
| `mmJoyMan` | 1543 | `mmJoyMan` | IDENTICAL | 95% (19/20) | `Open1560Dev/Open1560/code/midtown/mminput/joyman.h` |
| `aiBanger` | 1520 | `mmBangerInstance` | PARTIAL | 18% (2/11) | `Open1560Dev/Open1560/code/midtown/mmbangers/banger.h` |
| `mmSingleRoam` | 1520 | `mmGameSingle` | PARTIAL | 93% (13/14) | `Open1560Dev/Open1560/code/midtown/mmgame/gamesingle.h` |
| `gizBridge` | 1508 | `mmBridgeSet` | RENAMED | 56% (5/9) | `Open1560Dev/Open1560/code/midtown/mmanim/bridge.h` |
| `PUGraphics` | 1504 | `PUGraphics` | IDENTICAL | 50% (3/6) | `Open1560Dev/Open1560/code/midtown/mmui/pu_graphics.h` |
| `PUMenuBase` | 1502 | `PUMenuBase` | IDENTICAL | 100% (10/10) | `Open1560Dev/Open1560/code/midtown/mmui/pu_menu.h` |
| `TextDropWidget` | 1498 | `TextDropWidget` | IDENTICAL | 82% (14/17) | `Open1560Dev/Open1560/code/midtown/mmwidget/tdwidget.h` |
| `mmCompDRecord` | 1456 | `mmCompDRecord` | IDENTICAL | 100% (12/12) | `Open1560Dev/Open1560/code/midtown/mmwidget/compdrec.h` |
| `camAICS` | 1424 | `AICamCS` | RENAMED | 71% (5/7) | `Open1560Dev/Open1560/code/midtown/mmcamcs/aicamcs.h` |
| `crKinematics` | 1424 | -- | NONE | - | -- |
| `CDMan` | 1424 | `CDMan` | IDENTICAL | 100% (9/9) | `Open1560Dev/Open1560/code/midtown/mmaudio/cd.h` |
| `dgBangerActive` | 1421 | `mmBangerActive` | RENAMED | 88% (7/8) | `Open1560Dev/Open1560/code/midtown/mmbangers/active.h` |
| `fxShardManager` | 1408 | `mmShardManager` | RENAMED | 78% (7/9) | `Open1560Dev/Open1560/code/midtown/mmcar/shard.h` |
| `mmIO` | 1392 | `mmIO` | IDENTICAL | 91% (10/11) | `Open1560Dev/Open1560/code/midtown/mminput/io.h` |
| `phSleep` | 1376 | `mmPhysEntity` | PARTIAL | 22% (2/9) | `Open1560Dev/Open1560/code/midtown/mmphysics/entity.h` |
| `ioJoystick` | 1376 | `mmJoystick` | PARTIAL | 20% (2/10) | `Open1560Dev/Open1560/code/midtown/mminput/joystick.h` |
| `PUControl` | 1370 | `PUControl` | IDENTICAL | 100% (6/6) | `Open1560Dev/Open1560/code/midtown/mmui/pu_control.h` |
| `gfxBitmap` | 1360 | `agiBitmap` | PARTIAL | 17% (1/6) | `Open1560Dev/Open1560/code/midtown/agi/bitmap.h` |
| `lvlMultiRoomInstance` | 1357 | `mmInstChain` | PARTIAL | 14% (2/14) | `Open1560Dev/Open1560/code/midtown/mmcity/instchn.h` |
| `Dialog_City2` | 1351 | `Dialog_City` | PARTIAL | 43% (3/7) | `Open1560Dev/Open1560/code/midtown/mmui/dlg_city.h` |
| `dgRoadDecalInstance` | 1350 | -- | NONE | - | -- |
| `mmRaceData` | 1328 | `mmRaceData` | IDENTICAL | 93% (13/14) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/racedata.h` |
| `mmGameSingle` | 1299 | `mmGameSingle` | IDENTICAL | 92% (12/13) | `Open1560Dev/Open1560/code/midtown/mmgame/gamesingle.h` |
| `phMaterialMgr` | 1284 | -- | NONE | - | -- |
| `PUAudioOptions` | 1253 | `PUAudioOptions` | IDENTICAL | 100% (6/6) | `Open1560Dev/Open1560/code/midtown/mmui/pu_audio.h` |
| `MArray` | 1248 | `MArray` | IDENTICAL | 100% (7/7) | `Open1560Dev/Open1560/code/midtown/mmwidget/mstore.h` |
| `camPolarCS` | 1232 | `PolarCamCS` | RENAMED | 67% (4/6) | `Open1560Dev/Open1560/code/midtown/mmcamcs/polarcamcs.h` |
| `mmText` | 1232 | `mmText` | IDENTICAL | 100% (9/9) | `Open1560Dev/Open1560/code/midtown/mmeffects/mmtext.h` |
| `vehFeedback` | 1216 | `mmEffectFF` | PARTIAL | 7% (1/14) | `Open1560Dev/Open1560/code/midtown/mminput/effect.h` |
| `mmCompRaceRecord` | 1216 | `mmCompRaceRecord` | IDENTICAL | 100% (11/11) | `Open1560Dev/Open1560/code/midtown/mmwidget/comprrec.h` |
| `mmPositions` | 1213 | `mmPositions` | IDENTICAL | 67% (6/9) | `Open1560Dev/Open1560/code/midtown/mmcity/position.h` |
| `uiNavBar` | 1174 | `uiNavBar` | IDENTICAL | 83% (10/12) | `Open1560Dev/Open1560/code/midtown/mmwidget/navbar.h` |
| `mmExternalView` | 1152 | `mmExternalView` | IDENTICAL | 86% (6/7) | `Open1560Dev/Open1560/code/midtown/mmgame/dash.h` |
| `aiPoliceForce` | 1152 | `aiPoliceForce` | IDENTICAL | 100% (7/7) | `Open1560Dev/Open1560/code/midtown/mmai/aiPoliceForce.h` |
| `UIButton` | 1152 | `UIButton` | IDENTICAL | 85% (11/13) | `Open1560Dev/Open1560/code/midtown/mmwidget/button.h` |
| `dgBangerActiveManager` | 1145 | `mmBangerActiveManager` | RENAMED | 100% (6/6) | `Open1560Dev/Open1560/code/midtown/mmbangers/active.h` |
| `mmArrow` | 1130 | `mmArrow` | IDENTICAL | 100% (6/6) | `Open1560Dev/Open1560/code/midtown/mmgame/hud.h` |
| `CFileStream` | 1120 | `FileStream` | PARTIAL | 15% (3/20) | `Open1560Dev/Open1560/code/midtown/stream/filestream.h` |
| `sfPointer` | 1115 | `sfPointer` | IDENTICAL | 88% (7/8) | `Open1560Dev/Open1560/code/midtown/mmwidget/pointer.h` |
| `gizTrain` | 1106 | `mmAnimTrain` | RENAMED | 67% (4/6) | `Open1560Dev/Open1560/code/midtown/mmanim/train.h` |
| `camPointCS` | 1104 | `PointCamCS` | RENAMED | 82% (9/11) | `Open1560Dev/Open1560/code/midtown/mmcamcs/pointcamcs.h` |
| `mmVehInfo` | 1088 | `mmVehInfo` | IDENTICAL | 57% (4/7) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/vehinfo.h` |
| `vehGyro` | 1072 | `VehGyro` | RENAMED | 33% (2/6) | `Open1560Dev/Open1560/code/midtown/mmcar/gyro.h` |
| `audHead` | 1072 | `AudHead` | PARTIAL | 7% (1/15) | `Open1560Dev/Open1560/code/midtown/mmaudio/head.h` |
| `phMaterial` | 1072 | -- | NONE | - | -- |
| `vehSiren` | 1042 | `mmPoliceCarAudio` | PARTIAL | 33% (2/6) | `Open1560Dev/Open1560/code/midtown/mmcar/policecaraudio.h` |
| `PUKey` | 1040 | `PUKey` | IDENTICAL | 100% (3/3) | `Open1560Dev/Open1560/code/midtown/mmui/pu_key.h` |
| `mmVehList` | 1037 | `mmVehList` | IDENTICAL | 100% (8/8) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/vehlist.h` |
| `OptionsMenu` | 1032 | `OptionsMenu` | IDENTICAL | 75% (3/4) | `Open1560Dev/Open1560/code/midtown/mmui/options.h` |
| `Dialog_NewPlayer` | 1030 | `Dialog_NewPlayer` | IDENTICAL | 100% (3/3) | `Open1560Dev/Open1560/code/midtown/mmui/dlg_newp.h` |
| `dgRagdoll` | 1029 | -- | NONE | - | -- |
| `vehAxle` | 1024 | `mmAxle` | RENAMED | 50% (3/6) | `Open1560Dev/Open1560/code/midtown/mmcar/axle.h` |
| `UIBMLabel` | 1024 | `UIBMLabel` | IDENTICAL | 100% (6/6) | `Open1560Dev/Open1560/code/midtown/mmwidget/bm_label.h` |
| `CMemStream` | 1008 | `VirtualStream` | PARTIAL | 6% (1/18) | `Open1560Dev/Open1560/code/midtown/stream/vstream.h` |
| `mmCompCity` | 1008 | `mmCompBase` | PARTIAL | 50% (6/12) | `Open1560Dev/Open1560/code/midtown/mmwidget/compbase.h` |
| `camViewCS` | 981 | `mmViewCS` | RENAMED | 80% (8/10) | `Open1560Dev/Open1560/code/midtown/mmcamcs/viewcs.h` |
| `UIIconW` | 981 | `UIIconW` | IDENTICAL | 100% (5/5) | `Open1560Dev/Open1560/code/midtown/mmwidget/iconwidget.h` |
| `mmCityInfo` | 976 | `mmCityInfo` | IDENTICAL | 100% (2/2) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/cityinfo.h` |
| `gizFerry` | 948 | `mmBoat` | RENAMED | 38% (3/8) | `Open1560Dev/Open1560/code/midtown/mmanim/boat.h` |
| `aiVehiclePlayer` | 944 | `aiVehicleActive` | PARTIAL | 18% (3/17) | `Open1560Dev/Open1560/code/midtown/mmai/aiVehicleMGR.h` |
| `AboutMenu` | 932 | `AboutMenu` | IDENTICAL | 100% (4/4) | `Open1560Dev/Open1560/code/midtown/mmui/about.h` |
| `aiStuck` | 931 | `aiStuck` | IDENTICAL | 100% (6/6) | `Open1560Dev/Open1560/code/midtown/mmai/aiStuck.h` |
| `mmRecord` | 928 | `mmRecord` | IDENTICAL | 81% (13/16) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/racerecord.h` |
| `mmMirror` | 928 | `mmGhostCar` | PARTIAL | 57% (4/7) | `Open1560Dev/Open1560/code/midtown/mmgame/gameman.h` |
| `UIToggleButton2` | 928 | `UIToggleButton2` | IDENTICAL | 91% (10/11) | `Open1560Dev/Open1560/code/midtown/mmwidget/togglebutton2.h` |
| `mmCityList` | 896 | `mmCityList` | IDENTICAL | 100% (9/9) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/citylist.h` |
| `mmStatePack` | 880 | `mmStatePack` | IDENTICAL | 67% (2/3) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/state.h` |
| `mmPlayerRecord` | 880 | `mmPlayerRecord` | IDENTICAL | 71% (5/7) | `Open1560Dev/Open1560/code/midtown/mmcityinfo/playerdata.h` |
| `phCollider` | 880 | -- | NONE | - | -- |
| `RaceMenu` | 878 | `RaceMenu` | IDENTICAL | 71% (5/7) | `Open1560Dev/Open1560/code/midtown/mmui/race.h` |
| `camPovCS` | 848 | `PovCamCS` | RENAMED | 78% (7/9) | `Open1560Dev/Open1560/code/midtown/mmcamcs/povcamcs.h` |
| `mmCollideFF` | 832 | `mmCollideFF` | IDENTICAL | 100% (5/5) | `Open1560Dev/Open1560/code/midtown/mminput/collide.h` |
| `sdlCommon` | 816 | -- | NONE | - | -- |
| `mmCarRoadFF` | 804 | `mmCarRoadFF` | IDENTICAL | 100% (9/9) | `Open1560Dev/Open1560/code/midtown/mmcar/roadff.h` |
| `camPostCS` | 800 | `PostCamCS` | RENAMED | 71% (5/7) | `Open1560Dev/Open1560/code/midtown/mmcamcs/postcamcs.h` |
| `PUMain` | 784 | `PUMain` | IDENTICAL | 20% (1/5) | `Open1560Dev/Open1560/code/midtown/mmui/pu_main.h` |
| `gizTrainCar` | 771 | `mmAnimTrainCar` | RENAMED | 43% (3/7) | `Open1560Dev/Open1560/code/midtown/mmanim/traincar.h` |
| `dgGlassInstance` | 768 | `mmShardManager` | PARTIAL | 14% (1/7) | `Open1560Dev/Open1560/code/midtown/mmcar/shard.h` |
| `UILabel` | 757 | `UILabel` | IDENTICAL | 88% (7/8) | `Open1560Dev/Open1560/code/midtown/mmwidget/label.h` |
| `mmToggle` | 752 | `mmToggle` | IDENTICAL | 100% (6/6) | `Open1560Dev/Open1560/code/midtown/mmwidget/toggle.h` |
| `UIToggleButton` | 736 | `UIToggleButton` | IDENTICAL | 100% (7/7) | `Open1560Dev/Open1560/code/midtown/mmwidget/togglebutton.h` |
| `mmToggle2` | 734 | `mmToggle2` | IDENTICAL | 100% (6/6) | `Open1560Dev/Open1560/code/midtown/mmwidget/toggle2.h` |
| `modPackage` | 720 | -- | NONE | - | -- |
| `Dialog_TCPIP` | 720 | `Dialog_TCPIP` | IDENTICAL | 75% (3/4) | `Open1560Dev/Open1560/code/midtown/mmui/dlg_tcpip.h` |
| `dgBangerManager` | 709 | `mmBangerManager` | RENAMED | 80% (4/5) | `Open1560Dev/Open1560/code/midtown/mmbangers/banger.h` |
| `UIMexButton` | 709 | `UIMexButton` | IDENTICAL | 100% (7/7) | `Open1560Dev/Open1560/code/midtown/mmwidget/mexbutton.h` |
| `OptionsBase` | 706 | `OptionsBase` | IDENTICAL | 80% (4/5) | `Open1560Dev/Open1560/code/midtown/mmui/optionsbase.h` |
| `ioMouse` | 704 | -- | NONE | - | -- |
| `ioPad` | 704 | -- | NONE | - | -- |
| `VehShowcase` | 674 | `VehShowcase` | IDENTICAL | 100% (3/3) | `Open1560Dev/Open1560/code/midtown/mmui/vshow.h` |
| `mmFrictionFF` | 672 | `mmFrictionFF` | IDENTICAL | 100% (5/5) | `Open1560Dev/Open1560/code/midtown/mminput/friction.h` |
| `ioKeyboard` | 608 | -- | NONE | - | -- |
| `gfxLight` | 112 | `agiLight` | PARTIAL | 0% (0/1) | `Open1560Dev/Open1560/code/midtown/agi/light.h` |
| `gfxMaterial` | 80 | `agiMtlDef` | PARTIAL | 0% (0/1) | `Open1560Dev/Open1560/code/midtown/agi/mtldef.h` |

---

Generated by hand-verified inspection of the Open1560 headers against
`data/symbols.json`; overlap percentages are mechanical, classifications are not.
