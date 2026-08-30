# Port order

What to reimplement, in what sequence, and what will bite you. Read `docs/harness.md` first — this
document assumes the assemble-and-link model (write C++, flip `ARTS_IMPORT` to `ARTS_EXPORT`, run
`tools/asm.py --write`, relink, run the game).

The method is Midtown Madness 1's, from `MM2_RE_KIT/MM1_METHOD.md`: core plumbing, then enough to
boot, then subsystem sweeps richest-vein-first, then the hard tail. Pick classes by **bytes of
machine code**, not method count. Port constructors and destructors **with** the virtuals. Assume
every unexplained bug is an uninitialised member until proven otherwise.

Everything below is derived from `data/symbols.json`, `data/layouts.json`, `data/hierarchy.json`,
the generated headers, and the pseudocode in `MM2_RE_KIT/MM2_PSEUDOCODE/`. Numbers are real; where
two sources disagree, both numbers are given and the disagreement is the point.

---

## 0. Prerequisites — nothing can be ported until these exist

The harness produces a runnable `build/OpenMM2.exe` today, but there is **no C++ side of the build
yet**. Before function #1:

| Missing | Where it has to go | Notes |
|---|---|---|
| `ARTS_IMPORT` / `ARTS_EXPORT` macros | a new `code/midtown2/arts_pch.h` | Nothing defines them. `tools/asm.py` already greps for them in headers. |
| `i32 u32 f32 f64 b32 u8 usize` | same header | The 531 generated headers use these throughout; no definition exists anywhere in `code/`. |
| `check_size(C, N)` | same header | Used 531 times, defined zero times. Make it a `static_assert(sizeof(C) == N)`. |
| A compiler invocation | `tools/link.py` | `link.py` links **only** `game.obj` (`OBJ = C:\mm2ghidra\out\game.obj`). There is no `.cpp` glob, no `cl.exe` call, no project file. |

Two constraints on that build line, both non-negotiable and both already documented in
`tools/link.py`:

- **Write as if there were no CRT.** `-NODEFAULTLIB` has been dropped, but that only means a
  symbol `game.obj` does not define can come from `libcmt` instead of failing the link outright.
  It does not open the standard library: no `printf`, no `malloc`, no `std::` anything, no modern
  `new`. Use the game's `Printf`/`Displayf` and its `operator new`. `tools/link.py` documents the
  three reasons in full.
- **`-FIXED -BASE:0x400000`.** No relocation table exists, so the image can only load at
  `0x400000`.

`operator new` / `operator delete` are already in the binary at `0x577360` / `0x577380` and route
straight into `memMemoryAllocator::Allocate(memMemoryAllocator::Current, size, false)`. Your C++
`new` must resolve to *those*, not to the CRT's. That is the single most important link-level fact
in the project.

---

## 1. Phase 1 — engine plumbing

Ordered so nothing appears before what it depends on. `map B` is the size from `data/symbols.json`
(next-symbol delta); `kit B` is the IDA function size from `_CLASSES.md` — they differ, see Trap 8.
"members?" means the generated header actually emits offsets (`Offset order is mandatory`), which
is the only real test of "can this be ported safely".

### 1a. Memory — must be first, `_main` uses it before anything else

| # | class | map B | kit B | fns | virt | vtable | members? | size | header |
|--:|---|--:|--:|--:|--:|:--|:--|--:|---|
| 1 | `memMemoryAllocator` | 2,616 | 2,443 | 18 | 0 | no | **YES** | `0xDC` | `code/midtown2/memory/memmemoryallocator.h` |
| 2 | `memSafeHeap` | 369 | 319 | 7 | 0 | no | **YES** | `0x18` | `code/midtown2/memory/memsafeheap.h` |
| 3 | global `operator new/delete/new[]/delete[]` | 176 | — | 4 | 0 | no | n/a | n/a | free functions, `0x577360`–`0x5773C0` |

`memMemoryAllocator` is a bucketed free-list allocator with 32 size buckets (`m_Buckets[32]` at
`0x18`) over a caller-supplied block. `memSafeHeap` wraps `VirtualAlloc` and hands the block to it.
Neither has a vtable. `_main` constructs an allocator **on the stack**, `Init`s it over an 8 KB
shadow block, and assigns `memMemoryAllocator::Current` — so this class is live before any global
constructor runs.

Dependencies: none. Open1560 has a direct counterpart at
`Open1560Dev/Open1560/code/midtown/memory/allocator.cpp`.

**Note that `Allocate` does not zero memory.** This is the mechanical reason MM1's #1 bug class is
uninitialised members: every object is born on reused, dirty heap.

### 1b. Containers and strings

| # | class | map B | kit B | fns | virt | vtable | members? | size |
|--:|---|--:|--:|--:|--:|:--|:--|--:|
| 4 | `HashTable` | 2,761 | 2,523 | 25 | 0 | no | **YES** | `0x18` |
| 5 | `CArrayList` | 368 | 337 | 6 | 0 | no | **YES** | `0x8` |
| 6 | `MArray` | 1,248 | 1,061 | 8 | 0 | no | **YES** | `0xC` |
| 7 | `WArray` | 1,968 | 1,897 | 9 | 0 | no | **YES** | `0xC` |
| 8 | `string` | 6,995 | 6,435 | 32 | 0 | no | **YES** | `0x8` |

`HashTable` has a `KillAll()` static that `EndPhase` calls, so it is on the boot path too. Port it
before `datArgParser`, which holds a `HashTable` as a static member.

### 1c. Math — `vector7`

| # | class | map B | kit B | fns | virt | vtable | members? | size | MM1 ref bytes |
|--:|---|--:|--:|--:|--:|:--|:--|--:|--:|
| 9 | `Vector2` | 80 | 45 | 3 | 0 | no | **YES** | `0x8` | — |
| 10 | `Vector3` | 6,256 | 3,397 | 48 | 0 | no | **YES** | `0xC` | 1,040 |
| 11 | `Vector4` | 1,248 | 925 | 14 | 0 | no | **YES** | `0x10` | — |
| 12 | `Matrix34` | 16,528 | 15,454 | 82 | 0 | no | **YES** | `0x30` | 6,096 |
| 13 | `Matrix44` | 5,584 | 4,910 | 24 | 0 | no | **YES** | `0x40` | 560 |
| 14 | `Matrix33` | 2,000 | 1,897 | 17 | 0 | no | no | — | — |
| 15 | `Quaternion` | 1,552 | 1,482 | 7 | 0 | no | no | — | — |
| 16 | `Matrix66` | 1,232 | 1,163 | 11 | 0 | no | no | — | — |

`Matrix34` is the single best early target in the whole binary: 16.5 KB, 82 functions, **no
virtuals, no vtable**, layout pinned at `0x30`, and the largest individual function is only
252 bytes (`0xFC`). Its layout is byte-identical to Open1560's `Matrix34` (`m00`…`m32`, `0x30`) and
19 of its mangled symbols already have a working implementation in
`Open1560Dev/Open1560/code/midtown/vector7/matrix34.cpp`.

`Matrix44::InvertTo` (`0x4C1510`, 1,766 B) is the largest function in vector7 and is **not**
Gauss-Jordan with pivoting, whatever this document said before it was disassembled. The first
instruction after the prologue is `call ?Determinant@Matrix44@@QBEMXZ`, followed by
`fdivr dword ptr [1.0f]`; the remaining 1,700 bytes are sixteen straight-line cofactor blocks, six
triple-products each, all scaled by that reciprocal. There is no loop, no pivot search and no row
swap anywhere in it. That makes it mechanical rather than delicate - but it is 288 individual member
reads with no structure to check them against, which is the exact shape of the wrong-but-plausible
value `docs/harness.md` says no byte comparison can see. Budget a pass for it on its own.

`Matrix33` / `Quaternion` / `Matrix66` have no IDB layout, but their sizes are not actually unknown
— they are 9, 4 and 36 contiguous floats respectively (`0x24`, `0x10`, `0x90`). This is the one
place where deriving a layout is legitimate, because the pseudocode reads them as flat float
arrays. Confirm against the code before writing `check_size`.

Fidelity rule that matters most here: **mirror the exact sub-expression grouping**. `Matrix34::Dot`
in x87 is not associative; MM1 lost days to physics diverging from regrouped float math.

### 1d. Streams and file IO

| # | class | map B | kit B | fns | virt | vtable | members? | size |
|--:|---|--:|--:|--:|--:|:--|:--|--:|
| 17 | `Stream` | 1,744 | 1,609 | 16 | 0 | no | **YES** | `0x1C` |
| 18 | `zipFile` | 2,752 | 810 | 16 | 0 | no | **YES** | `0x1C` |
| 19 | `CFileStream` | 1,120 | 888 | 24 | 15 | **yes** | no | — |
| 20 | `CMemStream` | 1,008 | 640 | 22 | 15 | **yes** | no | — |

`Stream` is the base of all file IO and is completely vtable-free — dispatch goes through a
`coreFileMethods*` function table at offset `0x000`, whose layout **is** known (`0x24`, 9 members).
Port `Stream` before anything that loads a file.

`CFileStream` and `CMemStream` are 15-virtual classes with no known layout — defer them (see the
hard tail).

`zipFile` needs zlib. The binary carries its own inflate (~10.7 KB of free functions:
`inflate_blocks` 3,152 B at `0x5746F0`, `inflate_codes` 1,824 B, `inflate_trees_bits` 1,232 B,
`inflate_fast` 896 B). Do **not** transcribe these — link a modern zlib behind the same entry
points, exactly as MM1 did for its backends. Also note `zipFile::Init` (1,536 B at `0x573480`) has
**no pseudocode at all** (Trap 3).

### 1e. Reflection and parsing — `data7`

MM2 has **no `MetaClass`**. Searching every symbol for `MetaClass`, `TypeInfo`, `AddField` or
`DeclareFields` returns nothing. MM1's metaclass reflection is replaced in MM2 by two mechanisms:

- **`asNode::FileIO(datParser&)`** — a virtual on every node; the object registers its own fields
  with a `datParser` via `AddValue` / `AddParser`.
- **`asFileIO`** — an 8-slot virtual interface (`code/midtown2/arts7/asfileio.h`) used by classes
  outside the node tree (e.g. `dgTrailerJoint`).

Runtime type identity is `virtual char* GetClassName()` returning a string literal, plus
`virtual int Type()` on the `ai*` hierarchy. There is no RTTI (`/GR-`).

| # | class | map B | kit B | fns | virt | vtable | members? | size |
|--:|---|--:|--:|--:|--:|:--|:--|--:|
| 21 | `datCallback` | 432 | 353 | 8 | 0 | no | **YES** | `0xC` |
| 22 | `datArgParser` | 1,824 | 1,738 | 11 | 0 | no | all-static | n/a |
| 23 | `datOutput` | 192 | 159 | 7 | 0 | no | all-static | n/a |
| 24 | `datTimeManager` | 816 | 775 | 5 | 0 | no | all-static | n/a |
| 25 | `datStack` | 864 | 807 | 7 | 0 | no | all-static | n/a |
| 26 | `datAssetManager` | 1,648 | 1,311 | 12 | 0 | no | all-static | n/a |
| 27 | `datBaseTokenizer` | 1,376 | 1,242 | 14 | 1 | **yes** | **YES** | `0x1C` |
| 28 | `datAsciiTokenizer` | 1,136 | 817 | 18 | 18 | **yes** | no | `0x1C` |
| 29 | `datBinTokenizer` | 688 | 555 | 18 | 18 | **yes** | no | `0x1C` |
| 30 | `datMultiTokenizer` | 368 | 353 | 3 | 0 | no | **YES** | `0x5C` |
| 31 | `datParserRecord` | 64 | 58 | 1 | 0 | no | **YES** | `0x54` |
| 32 | `datParser` | 2,336 | 2,345 | 15 | 0 | no | **YES** | `0x54` |

Six of these (`datArgParser`, `datOutput`, `datTimeManager`, `datStack`, `datAssetManager`,
`datReplay`) have **zero `__thiscall` functions** — they are pure static-function namespaces with
static data. They need no layout at all, which makes them the cheapest real work in the project.
`check_size(datArgParser, 0x1)` in the header is correct, not a placeholder.

`datCallback` is a member-function-pointer eraser: seven constructors taking
`void (__thiscall Base::*)(...)`. This is what the class `Base` (2 functions, 1 vtable slot,
`0x41AAF0` / `0x4C8200`) exists for — it is **not** a universal base class and **not** MM1's
metaclass `Base`. MM1's worst-ever bug (two `MetaClass` objects named `Base`) has no direct MM2
analogue; the equivalent risk here is a `GetClassName` string mismatch.

Dependency order inside 1e: `datCallback` → `datArgParser` (needs `HashTable`) → tokenizers →
`datParserRecord` → `datParser` (needs `Stream` + tokenizers).

### 1f. The node tree — `arts7`

| # | class | map B | kit B | fns | virt | slots | vtable | members? | size | base |
|--:|---|--:|--:|--:|--:|--:|:--|:--|--:|---|
| 33 | `asCullable` | 32 | 227 | 2 | 2 | — | no | no | 4 (implied) | — |
| 34 | `asNode` | 1,600 | 1,433 | 29 | 14 | 13 | **yes** | **YES** | `0x18` | `asCullable` |
| 35 | `asRoot` | 400 | 265 | 12 | 5 | 13 | **yes** | **YES** | `0x4C` | `asNode` |
| 36 | `eqEventMonitor` | 368 | 316 | 5 | 4 | 3 | **yes** | **YES** | `0xC` | — |
| 37 | `eqEventQ` | 755 | 542 | 9 | 4 | 3 | **yes** | **YES** | `0x24` | — |
| 38 | `eqEventHandler` | 1,186 | 1,797 | 13 | 2 | 1 | **yes** | partial | `0x148` **(wrong — see below)** |
| 39 | `asCullManager` | 1,120 | 973 | 13 | 8 | 17 | **yes** | **YES** | `0xA8` | `asNode` |

`asCullable` has no layout entry, but `asNode`'s first member is at `0x004`, so `asCullable` is
exactly a vptr — 4 bytes. That is a safe derivation.

**These all have vtables, and that changes the rules.** See Trap 1: for any class with a vtable you
must port the plain member functions first and leave the constructor/destructor and virtuals for
last, as one atomic change, because the moment the compiler emits `??_7asNode@@6B@` it collides
with the copy already sitting in `.rdata` inside `game.asm`.

`eqEventHandler` is a worked example of a layout you must not trust: `layouts.json` claims `0x148`
(328) but the last recovered member is at `0x48`, and `BeginPhase` allocates it with
`operator new(0x160)` (352). Both the size and the tail are wrong.

---

## 2. Phase 2 — the shortest path to a booting game

This is traced from the actual pseudocode. **Three of the five entry-point functions are filed in
the kit under wrong names, inside unrelated class folders.** The real chain is:

| # | real symbol | RVA | size | where the pseudocode actually is |
|--:|---|---|--:|---|
| 1 | `_main` | `0x402360` | 624 | `_FREE_FUNCTIONS.c` as `dynamic_initializer_for__currentPlayerVehicle__` |
| 2 | `ArchInit(int, char**)` | `0x402340` | 16 | `_FREE_FUNCTIONS.c` as `dynamic_initializer_for__cfgMm1StyleRefl__` |
| 3 | `ExceptMain()` | `0x4022B0` | 144 | `_FREE_FUNCTIONS.c`, correct name |
| 4 | `Main()` | `0x4011E0` | 1,200 | `_FREE_FUNCTIONS.c`, correct name — **truncated by `JUMPOUT(0x4012BC)`** |
| 5 | `CreateGameMutex(const char*)` | `0x402180` | 80 | `_FREE_FUNCTIONS.c` as `dynamic_initializer_for__cfgAirborneSpeedThresh__` |
| 6 | `FirstRunEula()` | `0x4026A0` | 224 | `_FREE_FUNCTIONS.c` as `dynamic_initializer_for__sm_RainParticles__` |
| 7 | `MainPhase(bool, int)` | `0x4016D0` | 816 | `_FREE_FUNCTIONS.c`, correct name |
| 8 | **`BeginPhase(bool)`** | `0x401AA0` | 816 | **`aiMap/aiMap.c` line 7**, as `dynamic_initializer_for__MM2::aiMap::_fOppUpdate__` |
| 9 | `InitAudioManager(bool)` | `0x401E70` | 336 | `_FREE_FUNCTIONS.c`, correct name |
| 10 | **`GameLoop(bool)`** | `0x401A00` | 160 | **`aiVehicleManager/aiVehicleManager.c` line 7**, as `dynamic_initializer_for__MM2::aiVehicleManager::SignalClock__` |
| 11 | **`EndPhase()`** | `0x401FC0` | 256 | **`ioMouse/ioMouse.c` line 7**, as `dynamic_initializer_for__MM2::ioMouse::m_Y__` |
| 12 | `KillAudioManager()` | `0x4020C0` | 48 | `pedRagdollMgr/pedRagdollMgr.c`, as `dynamic_initializer_for__MM2::pedRagdollMgr::Instance__` |

### What actually runs

**`_main`** (this is the `WinMain`; the map name is `_main`):

```
pShadowMem  = align8(ShadowMem_8192_ + 7)
memMemoryAllocator ctor (stack object)
memMemoryAllocator::Init(&allocator, pShadowMem, 0x2000, true, false)
memMemoryAllocator::Current = &allocator
datArgParser::Init(_argc, _argv)
ArchInit(_argc, _argv)                  // empty (16 bytes)
ExceptMain() -> Main()
datArgParser::Kill()
~memMemoryAllocator
```

**`Main`** — `ioInput::bUseJoystick = 0`, `ComputeCpuSpeed()`, `datArgParser::Get("nolog")`,
`EnumDisplaySettingsA`, `CreateGameMutex("MidtownMadness2Mutex")`, `CheckGlobalMemory()`,
`CheckDiskSpace()`, `FirstRunEula()`, then the pseudocode gives up (`JUMPOUT(0x4012BC)`) with
~460 bytes of the function unaccounted for. `Main` also has an SEH funclet
(`?Main@@YAHXZ_SEH` at `0x5A8DF3`). **Read this one out of `game.asm` or Ghidra, not the kit.**

**`MainPhase(parsedStateArgs, firstLoad)`** — the mode switch:
`BeginPhase(MMSTATE.GameState == 0)` → `datTimeManager::RealTime(0)` →
`asNode::ResChange(&ROOT, ...)` → `asRoot::SetPause(&ROOT, false)` → load the "reloading"
`gfxImage` → `gfxBitmap::Create`. Then one of two branches:

- `GameState == 0` (menus): `new mmReplayManager` (`0x840`), `new mmInterface` (**`0x7750`**),
  `AddChild(&ROOT, interface)`, `mmInterface::ShowMain(firstLoad)`.
- `GameState == 1` (in game): `new mmReplayManager` (`0x840`),
  `mmReplayManager::LoadReplay`, `new mmGameManager` (`0x1B8`),
  `asNode::AddChild(replayMgr, gameMgr)`, `asNode::AddChild(&ROOT, replayMgr)`.

Then `GameLoop(...)`, then teardown, then `EndPhase()`.

**`BeginPhase(splashScreen)`** — everything graphical:
`gfxPipeline::SetWindow` → `gfxResetAdapter` → `InitDirectDraw` (→ `gfxPipeline::BeginGfx2D`) →
`gfxPipeline::SetRes` → `gfxPipeline::BeginGfx3D` → `vglSetFormat` → `GetLoadScreenName` →
`gfxLoadImage` → `gfxImage::Scale` → `gfxBitmap::Create` → `gfxTexture::EnableCache(false)` →
`gfxCreateFont()` → `new eqEventHandler` (`0x160`) → `eqEventHandler::SuperQ = it` →
`InitAudioManager` → `AddChild(&ROOT, MMAUDMGRPTR)` → `new asCullManager(512, 64)` (`0xA8`) →
`new mmInput` (`0x248`) → `mmInput::AttachToPipe` → `mmInput::Init`.

**`GameLoop(draw)`** — 160 bytes, and the whole game:

```c
memMemoryAllocator::GetStats(memMemoryAllocator::Current, &stats, true);
while (MMSTATE.GameState == -1) {
    datTimeManager::Update();
    ioInput::Poll();
    eqEventHandler::Update(eqEventHandler::SuperQ);
    MMAUDMGRPTR->vtbl[2]();                       // AudManager::Update
    asRoot::Update(&ROOT);
    if (draw) asCullManager::Instance->Update();
    if (gfxPipeline::m_EvtFlags & 1) { MMSTATE.Shutdown = 1; break; }
}
```

**`EndPhase()`** — `lvlInstance::ResetAll` → destroy `phMaterialMgr` → destroy `GameInputPtr`
(`mmInput`) → destroy `asCullManager::Instance` → destroy `MUSICMANAGERPTR` →
`KillAudioManager()` → `vglSetFormat(0,0,0,0)` → destroy `eqEventHandler::SuperQ` →
`modShader::KillAll()` → `gfxPipeline::EndGfx3D` → `gfxTexture::EnableCache(false)` →
`gfxResetAdapter` → `gfxPipeline::EndGfx2D` → `HashTable::KillAll()`.

### The Phase 2 class list, in dependency order

All of Phase 1 first. Then:

| order | class | map B | portable today? | note |
|--:|---|--:|:--|---|
| 1 | `Timer` | 608 | yes (`0x4`) | `ComputeCpuSpeed` in `Main` |
| 2 | `gfxImage` | 3,104 | yes (`0x1C`) | no vtable; ref-counted (`RefCount` decrement is explicit in the callers) |
| 3 | `gfxBitmap` | 1,091 | yes (`0x14`) | |
| 4 | `gfxTexture` | 6,096 | yes (`0x34`) | no vtable, 32 fns — a good vein as well as a dependency |
| 5 | `gfxRenderState` | 5,664 | yes (`0x50`) | no vtable |
| 6 | `gfxViewport` | 2,464 | yes | |
| 7 | `gfxPacket` | 5,552 | yes (`0x2C`) | no vtable |
| 8 | `lvlInstance` | 7,250 | yes (`0x14`) | **22 virtuals + its own pooled `operator new`** — see Trap 6 |
| 9 | `modShader` | 3,136 | yes (`0x8`) | `EndPhase` calls `KillAll` |
| 10 | `phMaterialMgr` | 1,130 | yes (`0x1C`) | |
| 11 | `asCamera` | 4,112 | yes (`0x170`) | |
| 12 | `mmReplayManager` | 5,405 | yes (`0x840`) | layout size **matches** `operator new(0x840)` — rare and reassuring |
| 13 | `mmInput` | 17,603 | **layout wrong** | `0x1DC` claimed, `operator new(0x248)`; 10 of its functions have no pseudocode |
| 14 | `mmGameManager` | 2,332 | **no layout** | `operator new(0x1B8)` is the only size evidence |
| 15 | `mmInterface` | 43,587 | **layout suspect** | `0x78C0` claimed, `operator new(0x7750)`; **its 5,040-byte constructor has no pseudocode at all** |
| 16 | `mmPlayer` | 13,616 | **no members** | 20+ of its functions are mislabeled (Trap 2) |

**Deliberately deferred out of Phase 2:** `gfxPipeline` (21,616 B, no members, raw DirectDraw) and
`AudManager` / `audManager` (no layout, DirectSound + DirectMusic). Both are device layers. MM1's
rule applies: replace them with a modern backend behind the same interface rather than re-typing
DirectDraw 5. Until then, leave them as original assembly — the harness makes that free.

**The milestone to chase**, in order: (a) the game still boots with Phase 1 ported — proves the
build integration; (b) `GameLoop` ported — 160 bytes, and once it is yours you can instrument the
frame; (c) `MainPhase` + `BeginPhase` + `EndPhase` — proves the node tree and the whole
construct/destruct sequence.

---

## 3. Phase 3 — the good veins

Criteria, in priority order: (1) the generated header emits real member offsets, (2) **no vtable**,
(3) large, (4) an Open1560 counterpart exists.

**Only 158 of 536 code-bearing classes have real member offsets in their headers**, not the 337
that `check_size` suggests (Trap 5). Of those 158, **66 also have no vtable at all** — 232,792
bytes of the safest possible work. Ranked:

| rank | class | map B | kit B | fns | size | MM1 ref | why |
|--:|---|--:|--:|--:|--:|--:|---|
| 1 | `Matrix34` | 16,528 | 15,454 | 82 | `0x30` | 6,096 | biggest function 252 B; layout identical to MM1's |
| 2 | `aiPath` | 23,488 | 22,419 | 75 | `0x164` | 5,168 | 86 recovered members, zero virtuals, self-contained road graph |
| 3 | `modModel` | 11,831 | 9,458 | 17 | `0xC` | 0 | mesh format, already decoded by the MM2 scene; `LoadAscii` is 3,584 B |
| 4 | `phInertialCS` | 14,448 | 14,110 | 52 | disputed | 0 | see caution below |
| 5 | `gfxTexture` | 6,096 | 4,749 | 32 | `0x34` | 0 | on the boot path anyway |
| 6 | `Vector3` | 6,256 | 3,397 | 48 | `0xC` | 1,040 | but see Trap 8 — 2.2 KB of that is not Vector3 |
| 7 | `gfxRenderState` | 5,664 | 5,287 | 21 | `0x50` | 0 | pure state block |
| 8 | `Matrix44` | 5,584 | 4,910 | 24 | `0x40` | 560 | |
| 9 | `gfxPacket` | 5,552 | 4,455 | 30 | `0x2C` | 0 | |
| 10 | `phPolygon` | 4,896 | 4,824 | 12 | `0x60` | 0 | |
| 11 | `ltLight` | 4,672 | 3,245 | 23 | `0x4C` | 0 | |
| 12 | `ltProjection` | 3,648 | 1,879 | 11 | `0x3C` | 0 | 2 functions have no pseudocode |
| 13 | `aiIntersection` | 3,219 | 2,730 | 32 | `0x2C` | 0 | pairs with `aiPath` |
| 14 | `string` | 6,995 | 6,435 | 32 | `0x8` | 3,584 | |
| 15 | `HashTable` | 2,761 | 2,523 | 25 | `0x18` | — | already Phase 1 |
| 16 | `fxTexelDamage` | 3,088 | 1,782 | 6 | `0x24` | 0 | |
| 17 | `zipFile` | 2,752 | 810 | 16 | `0x1C` | 0 | see zlib note |
| 18 | `crAnimFrame` | 1,968 | 1,120 | 22 | | 0 | |
| 19 | `vehBreakableMgr` | 1,661 | 1,441 | 14 | | 0 | |
| 20 | `datParser` | 2,336 | 2,345 | 15 | `0x54` | — | already Phase 1 |

**Caution on `phInertialCS`:** it is the ideal shape on paper (52 functions, zero virtuals,
14.4 KB) but the two type sources disagree — `layouts.json` bare `phInertialCS` says `0x118`,
`MM2::phInertialCS` says `0x1B4`, and `dgRagdoll` allocates it with `operator new(436)` = `0x1B4`.
The header currently pins the wrong one (`check_size(phInertialCS, 0x118)`). Fix that first, and
be aware it is dense rigid-body integration — inspection-verify it, do not transcribe it.

**Classes with vtables that are still excellent veins**, once you have the vtable story settled
(Trap 1): `aiMap` (30,027 B, `0x194`/`0x1CC` disputed, 8,672 B of MM1 reference),
`MenuManager` (13,272 B, `0x150`, 12,544 B of MM1 reference — nearly a full reference
implementation), `RaceMenuBase` (9,360 B, 8,112 B MM1 ref), `NetArena`, `NetSelectMenu`,
`UIMenu`, `MainMenu`. The UI layer is where the MM1 head start is largest.

### The Open1560 dividend

**2,084 MM2 mangled symbols — 433,392 bytes, 24.6% of `.text` — appear verbatim in
`Open1560Dev/Open1560/code/midtown/`, and every one of them is implemented there** (not left as
`ARTS_IMPORT`). That is a free reference implementation for a quarter of the binary. Highest-yield
classes:

| class | MM2 bytes with an MM1 implementation |
|---|--:|
| `mmInterface` | 34,448 |
| `mmInput` | 16,208 |
| free functions | 14,336 |
| `MenuManager` | 12,544 |
| `aiPedestrian` | 11,616 |
| `mmMultiCR` | 10,560 |
| `asNetwork` | 9,056 |
| `mmGameMulti` | 8,816 |
| `aiMap` | 8,672 |
| `aiRailSet` | 8,672 |
| `VehicleSelectBase` | 8,368 |
| `aiGoalRandomDrive` | 8,288 |
| `RaceMenuBase` | 8,112 |
| `mmPlayer` | 7,584 |
| `Matrix34` | 6,096 |

Matching mangled names does **not** guarantee identical bodies — MM2 is a later build of the same
engine and functions did change. Treat the MM1 source as a very strong first draft, then diff it
against the MM2 pseudocode. Check for a counterpart **before** writing any `as*`, `dat*`, `mem*`,
`Matrix*`, `Vector*`, `ph*` or `mm*` class from scratch.

---

## 4. The hard tail — defer these, and know why

### 4a. Enormous single functions

Two `sdlPage16` functions are 39,856 of that class's 43,410 bytes:

| function | RVA | bytes |
|---|---|--:|
| `sdlPage16::GetDrawnSDLPrims(int, int*, int) const` | `0x450930` | **20,224** |
| `sdlPage16::Collect(const Vector4*, sdlPoly*, int, int&) const` | `0x455830` | **19,632** |
| `aiVehiclePhysics::CalcRoadTarget(int, Vector3&)` | `0x563B60` | 14,032 |
| `psdl_draw_tunnel_junction` (free) | `0x44C99C` | 13,214 |
| `psdl_draw_median_road` (free) | `0x44A156` | 10,310 |
| `gfxPipeline::RenderIdx(...)` | `0x4B54B0` | 8,064 |
| `aiVehiclePhysics::CalcDestinationTarget(int, Vector3&)` | `0x561AD0` | 7,680 |
| `mmInterface::Update()` | `0x40A6C0` | 6,560 |
| `phBoundBox::BoxToBoxFaceImpactsOffset(...)` | `0x47CE40` | 5,728 |
| `phBoundBox::BoxToBoxFaceImpacts(...)` | `0x47E4A0` | 5,392 |
| `phBoundHotdog::FindImpactsHotdogToPoly(...)` | `0x482440` | 5,184 |
| `mmInterface::mmInterface()` | `0x407C80` | 5,040 |

`sdlPage16` is the **software rasteriser** and PSDL city geometry pipeline. `sdlPage16::Draw`
itself is only 143 bytes — a jump-table dispatcher into 22 `psdl_draw_*` free functions at
`0x4483BF`–`0x45087F` (another ~33 KB), so the real cost of "porting `sdlPage16`" is ~77 KB across
two subsystems. MM1 replaced its software rasteriser with a modern backend rather than porting it
1:1. Do the same. Do not put 40 KB of hand-unrolled span code on the critical path.

### 4b. No known layout — 194 classes, 288,273 bytes

Porting one of these means guessing member offsets. **Don't.** The biggest offenders:

| class | bytes | why it hurts |
|---|--:|---|
| `aiVehiclePhysics` | 60,064 | largest class in the binary; header pins `0x9770` from MM2Hook with **zero members** |
| `aiPedestrian` | 24,286 | header pins `check_size(aiPedestrian, 0x2)` — obviously nonsense |
| `gfxPipeline` | 21,616 | header pins `0x1`; pure DirectDraw anyway |
| `mmSingleStunt` | 20,686 | 14 own virtuals, 14 SEH funclets |
| `phContactMgr` | 15,952 | dense contact solver, 7 decompiler "local variable allocation has failed" warnings |
| `phBoundTerrain` | 16,835 | |
| `mmMultiCR` | 16,226 | |
| `phBoundHotdog` | 14,672 | |
| `mmPlayer` | 13,616 | on the boot path, which makes this worse, not better |
| `aiGoalRandomDrive` | 13,488 | `SolveRailType` is 4,000 B |
| `phCollision` | 7,200 | 7 functions, one of them 1,718 B of x87 |
| `phContact` | 4,800 | |
| `crLegData` | 6,096 | `SolveIK` is 4,032 B and Hex-Rays emitted `__userpurge ...@<st0>` with a local-allocation failure |
| `audManager` / `audObject` / `audSound` / `audControl` | ~15,800 | whole audio layer |

Layout has to be recovered before any of these can be touched — from the constructor's
`operator new(N)`, from the largest offset the code touches, or from a sibling Angel binary
(`MM1_METHOD.md` §2: MM1 recovered many layouts from the Asset Manager tool, not the game).

### 4c. Dense x87 numerical math — inspection-verify, never transcribe

`phCollision`, `phContactMgr`, `phContact`, `phBoundBox` (the box-to-box impact solvers),
`phBoundHotdog`, `phBoundPolygonal::FindImpacts`, `phBoundTerrain*`, `crLegData::SolveIK`,
`RealQuartic` (`0x495520`, 1,312 B, a free function), `FindImpactPolygonToSphere` (`0x4C23F0`),
`SegmentToBoxIntersections` (`0x4C4380`).

219 pseudocode functions carry `__userpurge` or `@<st0>` in their signature, meaning Hex-Rays could
not model the x87 stack; 113 carry "local variable allocation has failed, the output may be wrong!"
— concentrated exactly here (`phContactMgr` 7, `phBoundBox` 4, `phBoundPolygonal` 3,
`phBoundHotdog` 3, `phBoundTerrain` 3, `phInertialCS` 3, `sdlPage16` 3). MM1's rule: *never ship
unverified dense numerical math.*

### 4d. Heavy device/backend coupling — replace, don't port

`gfxPipeline` (DirectDraw), `sdlPage16`/`sdlPoly`/`sdlCommon`/`lvlSDL` (software rasteriser),
`ioMouse`/`ioKeyboard`/`ioJoystick`/`ioPad` (DirectInput 7), `asNetwork`/`NetArena`/`mmNetObject`
(DirectPlay), `AudManager`/`DMusicManager`/`DirSnd`/`MixerCTL`/`EchoEffect` (DirectSound +
DirectMusic), `audCD`/`mmCDPlayer` (MCI). `tools/implibs.py` already builds import libs for
DPLAYX and DINPUT out of the binary's own import table, so these keep working as original assembly
indefinitely — which is exactly what you want.

### 4e. Third-party code embedded in the binary — link, don't transcribe

- **libjpeg**: ~29,984 bytes (`jpeg_idct_islow` 1,456 B, `jpeg_idct_ifast` 1,136 B,
  `jpeg_idct_float` 992 B, `decode_mcu`, `get_dht`, `get_sof`, `get_sos`, `read_markers`, …)
- **zlib inflate**: ~10,752 bytes
- **1999 MSVC CRT**: ~6,254 bytes of `_str*`/`_mem*`/`_pow`/`_x_ismbbtype` etc., and it must stay in
  `game.obj` because of `-NODEFAULTLIB`

That is ~47 KB — 2.7% of `.text` — that should never be hand-written.

---

## 5. Traps specific to this binary

### Trap 1 — porting a constructor emits a vtable that already exists in `game.asm`

**335 of 536 classes have a `` `vftable' `` symbol in `.rdata`.** `tools/ghidra/ExportAsm.java`
emits `PUBLIC <sym>` for every data symbol starting with `?` (line 592), so
`??_7asNode@@6B@` is a public symbol in the assembly. The moment you define a constructor or
destructor for a class with virtuals, MSVC emits its own `??_7asNode@@6B@` — **duplicate symbol at
link**.

Worse, `tools/asm.py` can only strip `PROC` blocks (`PROC = re.compile(r"^(\S+)\s+PROC\b")`). The
vtable is not a `PROC`; it is a label inside the one contiguous `.rdata` block that
`harness.md` says must keep its exact intra-section layout.

> **This trap is now handled. See `docs/vtables.md`.** `tools/asm_vtables.py` hands a vftable over
> to the C++ side without removing a byte: the table's `PUBLIC` is commented out, its label is
> renamed to a private `__vft_orig_<rva>`, the 5,818 `dd offset <table> + N` anchors that point at
> unrelated `.rdata` constants are repointed at that private label, and the symbol is declared
> `EXTERN <table>:BYTE`. Line count and emitted byte count are unchanged, so nothing in `.rdata`
> moves and consequence 3 below does not arise. `asm.py` calls `asm_vtables.hand_over()`.

Practical consequences, in order:

1. **Start with vtable-free classes.** 66 of the 158 portable-today classes have no vtable at all —
   232,792 bytes. That is more than enough runway, and it is still the easiest work.
2. For a class that has a vtable, porting its **non-virtual member functions** is free: no vtable is
   emitted, so nothing collides. Porting an ordinary **virtual** is also free, for the same reason —
   MSVC emits `??_7C@@6B@` only from a constructor or destructor. That is a much larger safe set
   than this trap originally allowed for.
3. The constructor and destructor are what trigger the handover, and they require the class's
   virtuals to be declared in the **binary's** order, not alphabetically. Run
   `py tools/asm_vtables.py --check <Class>` first; 50 of the 335 generated headers currently fail
   it.
4. When you do cross that line, MM1's rule binds: **constructor + destructor + every virtual, in one
   commit**, or you get a wall of link errors.
5. **A declaration order that is wrong also poisons every ordinary virtual CALL, not just the
   vtable definition** — and that is a much wider blast radius than point 3 suggests. A C++ virtual
   call compiles to `call [[this] + 4*slot]`, and the compiler works the slot out from the
   declaration order in the header. `py tools/asm_vtables.py --check asNode` reports asNode's
   eleven virtuals declared alphabetically instead of in the binary's order, plus a twelfth
   (`?GetClassNameA@asNode@@UAEPADXZ`) that occupies no slot at all. So **any** virtual call written
   against `asNode` or any of its ~200 descendants lands on the wrong entry, in a function with no
   constructor, no destructor and no duplicate-symbol error anywhere. Round 4 held back
   `aiIntersection::Reset`, `~aiIntersection` and `StopSignOkayToGo` for exactly this. Run the
   checker against the class whose virtual you are calling **and against its bases**, and note that
   `--check` needs `MM2_ASM=C:/mm2ghidra/out/game.asm.pristine` when `code/midtown2/game.asm` is
   absent, which it normally is.

Also: **never define a static data member.** `data/symbols.json` holds 1,301 non-code symbols, 549
of them class-scoped statics — `datArgParser::ArgHash` (a whole `HashTable`),
`memMemoryAllocator::Current`, `Stream::sm_Streams`, `eqEventHandler::SuperQ` and so on. Every one
lives in the assembly's `.data` and must stay an `ARTS_IMPORT` declaration forever; defining one in
C++ is the same duplicate-symbol failure as a vtable.

### Trap 2 — 350 functions in the kit carry a name from a foreign symbol set, and the quarantine does not contain them

`README.md` says ~349 functions carry ImGui/ImPlot/LuaIntf names and are "quarantined in
`_MISLABELED_FUNCTIONS.c` and excluded from anything generated here". That is true but it is only
half the problem, and the two halves are **disjoint sets**.

Cross-checking every row of `_INDEX.csv` against `data/symbols.json` by address:

- 9,429 kit functions; 7,684 names match the map exactly.
- **380 disagree; 350 of them are genuinely unrelated names, covering 59,087 bytes.**
- **Zero of those 350 appear in `_MISLABELED_INDEX.csv`.** The quarantine caught a different 349.

So there are **350 more mislabeled functions still live inside `MM2_PSEUDOCODE/<Class>/<Class>.c`**,
filed under the wrong class folder. The ones that will actually cost you a day:

| real symbol | bytes | filed in the kit as |
|---|--:|---|
| `mmNetObject::PositionUpdate` | 2,544 | `ImGuiDockNode/?IsRootNode@ImGuiDockNode@@QBE_NXZ` |
| `mmPlayer::Update` | 2,409 | `/_dynamic_initializer_for__cfgUseRichPresence___0` |
| `mmSingleStunt::UpdateCorner` | 2,121 | `/?ImTextStrToUtf8@@YAHPADHPBG1@Z` |
| `modModel::SaveToBinary` | 2,067 | an `auto_ptr` constructor |
| `mmSingleStunt::UpdateStop` | 1,665 | `/ImMin_float_` |
| `mmSingleStunt::UpdateJump` | 1,573 | `ImGuiTextRange/split` |
| `mmPlayer::Init` | 1,424 | `UIMenu/MM2::$::UIMenu::_dynamic_initializer_for__AssignBackground__` |
| `aiGoalRandomDrive::ChangeLanes` | 1,368 | `GameEventDispatcher/onGameInit` |
| `dgUnhitBangerInstance::Impact` | 1,366 | `?$ImVector/resize` |
| `mmPlayer::UpdateFF` | 1,054 | `MM2/MM2::_dynamic_initializer_for__vglVertex__` |
| `dgBangerData::Save` / `::Load` | 1,745 | `?$ImVector/push_back` / `pop_back` |
| `phBoundBox::MakeTransformedCorners` | 779 | `/ImAbs` |
| `pedAnimation::Load` | 737 | `/CreateHandler_aiRouteRacerHandler_` |
| `dgPhysManager::TrivialCollideInstances` | 512 | `/stbtt__track_vertex` |
| `BeginPhase` / `GameLoop` / `EndPhase` | 1,232 | `aiMap` / `aiVehicleManager` / `ioMouse` folders |

**Rule: `data/symbols.json` is the only authority on what a function is.** Look a function up by
address, never by the kit's name. In particular, `mmPlayer/mmPlayer.c` and `aiMap/aiMap.c` are
partly somebody else's code, and `aiMap/aiMap.c` opens with `BeginPhase`.

### Trap 3 — 286 real MM2 functions (53,680 bytes) have no pseudocode at all

`_THIRD_PARTY_SKIPPED.txt` lists 349 functions "not decompiled" because their labels looked like
ImGui / LuaIntf / `std::`. Those 349 addresses are the same set as `_MISLABELED_INDEX.csv`. **286
of them are real MM2 map symbols** — the kit simply has nothing for them:

| class | fns with no pseudocode | bytes |
|---|--:|--:|
| `mmInterface` | 6 | 7,264 (**including the 5,040-byte constructor**) |
| free functions | 21 | 7,072 |
| `vehCarModel` | 3 | 3,200 (`vehCarModel::Init` is 2,624) |
| `aiMap` | 9 | 2,688 |
| `zipFile` | 3 | 1,824 (`zipFile::Init` is 1,536) |
| `phBoundBox` | 1 | 1,264 (`ProbeVsBox`) |
| `mmReplayManager` | 6 | 1,152 |
| `vehCarSim` | 2 | 1,040 (including the constructor) |
| `mmInput` | 10 | 752 |

If you go to port `mmInterface` or `vehCarSim`, the constructor — the one function MM1_METHOD says
you must port together with the virtuals — is not in the kit. Ghidra or `game.asm` is the only
source.

### Trap 4 — the contamination reaches inside correctly-named functions

**11,900 foreign identifiers (`LuaIntf::`, `ImGui*`, `ImPlot*`, `std::`, `hook::`) appear across
445 of the 682 pseudocode files.** They are not only function names — they replace *global variable*
names and *parameter types* inside otherwise-correct functions. `lvlInstance::operator new` reads:

```c
v1 = ~*(_DWORD *)createstrobj & (*(_DWORD *)createstrobj + size);
...
v3 = LuaIntf::CppArgTupleInput<MM2::vehBreakable *>::get<LuaIntf::CppArgHolder<MM2::vehBreakable *>>;
```

Both of those are static data members of `lvlInstance`, not Lua. `aiMap::Police` takes a
`const ImPlot::GetterXsYs<int>*` that is really an `int` index. `BeginPhase` contains a call to
`ImPlotColormapMod::ImPlotColormapMod()`. Worst offenders by count: `_FREE_FUNCTIONS.c` (1,350),
`aiVehiclePhysics.c` (537), `aiMap.c` (534), `mmInterface.c` (313), `mmInput.c` (300),
`aiPath.c` (258), `sdlPage16.c` (242).

Every identifier in the pseudocode is a hypothesis. Resolve globals by address against
`data/symbols.json`, not by the name printed.

### Trap 5 — `check_size` being non-zero does **not** mean the layout is known

`tools/genheaders.py:167` takes the size from `layouts.json` first and falls back to MM2Hook's
`sizeof` (`data/mm2types.json`) — but it only emits **members** when `layouts.json` has them. So:

- 337 headers have a non-zero `check_size`.
- **Only 158 emit real member offsets.**
- **179 have a size and zero members.** Those classes are *not* portable.

Egregious examples: `check_size(aiVehiclePhysics, 0x9770)` with no members;
`check_size(aiPedestrian, 0x2)`; `check_size(gfxPipeline, 0x1)`; `check_size(mmPlayer, 0x23CC)`.

**The real test is whether the header contains the line `// Offset order is mandatory`.** Grep for
it. If it is absent, the class cannot be ported yet regardless of what `check_size` says.

### Trap 6 — two competing type sets, and neither one always wins

`_ALL_TYPES.h` contains both the game IDB's own recovered structs (bare names) and MM2Hook's types
(`MM2::` prefixed). Of 536 classes, 187 have a bare layout, 264 have an `MM2::` layout, 338 have at
least one — **113 have both, and 26 of those disagree on size**:

| class | bare | `MM2::` | which is right |
|---|--:|--:|---|
| `aiMap` | `0x194` | `0x1CC` | unresolved |
| `phBoundPolygonal` | `0x4C` | `0x64` | unresolved |
| `phInertialCS` | `0x118` | `0x1B4` | **`MM2::`** — `dgRagdoll` does `operator new(436)` |
| `vehCarModel` | `0xCC` | `0x154` | **bare** — `vehCar` does `lvlInstance::operator new(0xCC)` |
| `lvlSDL` | `0x60` | `0x4C` | unresolved |
| `gfxRenderState` | `0x50` | `0x98` | unresolved |
| `mmPopup` | `0x1` | `0x7C` | `MM2::`; the bare entry is an empty stub |
| `dgPhysEntity` | `0x258` | `0xB4` | unresolved |

**The tie-breaker is the allocation site.** Find the `operator new(N)` immediately before the
constructor call in the pseudocode; that N is the true `sizeof`. Running that check mechanically
over the whole tree: 92 classes have both a layout and an identifiable allocation size, and **29 of
them disagree** — `mmInput` (`0x1DC` vs `0x248`), `eqEventHandler` (`0x148` vs `0x160`),
`phInertialCS`, `UICompositeScroll`, `NetSelectMenu`, `MainMenu`, `RaceMenu`, and the whole
`Dialog_*` family (all pinned at `0xBC`, all allocating more).

Three further wrinkles:

- **Layouts can be truncated even when the size is right.** `mmInput`'s recovered members stop at
  `field_1D8` and everything past `0x1C4` is unnamed `field_NNN`. `eqEventHandler`'s last member is
  at `0x48` against a claimed `0x148`. A tail of consecutive `field_NNN` is a warning sign.
- **Three layouts are empty stubs** (`size <= 1`, zero members): `mmPopup`, `vehCarAudioContainer`,
  `mmVehList`.
- **`lvlInstance` and everything derived from it use a private pool allocator.**
  `lvlInstance::operator new` (`0x463110`) bump-allocates out of a single 0x280000-byte block, and
  `lvlInstance::operator delete` (`0x463170`) is **one byte — a bare `ret`, a no-op**. Any
  `lvlInstance` subclass you port must keep those semantics or you will free into the wrong heap.

### Trap 7 — the map's `size` is "distance to the next symbol", so a `PROC` can hide several functions

`tools/symbols.py` computes size as next-symbol-minus-this. Static helpers and compiler-generated
initialisers that the map does not name get **absorbed into the preceding symbol**. Comparing map
sizes against the kit's true IDA function sizes: **279 map symbols are ≥32 bytes larger than the
function at that address, hiding 81,766 bytes.**

| symbol | map size | real function | hidden |
|---|--:|--:|--:|
| `nullsub_21` | 9,248 | 1 | 9,247 |
| `gfxPipeline::RenderIdx` | 8,064 | 2,899 | 5,165 |
| `ImmSetCompositionWindow` | 4,116 | 6 | 4,110 |
| `aiVehicleSpline::UpdateObstacleMap` | 4,768 | 716 | 4,052 |
| `lvlLevelBound::CollidePolyToLevel` | 3,632 | 931 | 2,701 |
| **`Vector3::Dot3x3Transpose`** | **2,320** | **86** | **2,234** |
| `fxTexelDamage::ApplyDamage` | 1,744 | 475 | 1,269 |
| `gfxTexture::Load` | 1,568 | 835 | 733 |

`Vector3::Dot3x3Transpose` is the one that will bite a Phase-1 port. Its `PROC` at `0x4795C0` runs
2,320 bytes and contains, besides the 86-byte function itself, `sub_479620` (83 B) **and four
static constructors** (`static_ctor_479680`, `_479820`, `_479960`, `_479BB0`). Stripping that PROC
deletes all of them — including startup initialisers that nothing visibly calls. The symptom would
be a float global left at zero, discovered days later.

**Before stripping any PROC, check the kit's `_INDEX.csv` for other function starts inside
`[rva, rva+size)`.** If there are any, either port them all or leave the PROC alone.

**Neither size column is trustworthy on its own, and they err in opposite directions.**

- The map over-counts where unnamed statics get absorbed (279 symbols, 81,766 bytes) — `Vector3`
  is 6,256 map vs 3,397 kit, `zipFile` 2,752 vs 810.
- The kit over-counts where IDA merged a dispatcher with its jump-table targets: **27 kit functions
  are ≥32 bytes larger than the map symbol, by 44,901 bytes in total.** The extreme case is
  `sdlPage16::Draw` — **143 bytes in the map, 33,959 in the kit**. It is a small dispatcher; the
  bulk is the 22 `psdl_draw_*` free functions (`psdl_draw_tunnel_junction` 13,214 B,
  `psdl_draw_median_road` 10,310 B, `psdl_draw_intersection_sidewalks` 3,148 B, …) that it jumps
  into. Same pattern for `gfxLoadTexImage` (181 vs 2,989), `cityLevel::DrawRooms` (297 vs 2,891),
  `eqEventHandler::Update` (50 vs 1,094).

So `_CLASSES.md`'s "sdlPage16 = 76,722 bytes" and `inventory.md`'s "43,410" are both wrong for
different reasons; the truth is 43,410 for the class plus ~33 KB of `psdl_*` free functions it
dispatches to. Estimate effort from the address range, not from either table alone, and always use
`symbols.json` for addresses.

### Trap 8 — the kit splits 325 map functions into fragments, and truncates 761 more

- **982 kit functions are fragments** of a larger map symbol — they start at an interior address.
  `aiVehicleSpline::UpdateObstacleMap` is one IDA function of 716 bytes plus a 4,031-byte
  `sub_5686E0`. `gfxPipeline::RenderIdx` is split 22 ways. `aiMap::CalcRoute` (3,520 B) shows only
  2,948 in the kit. Read the whole address range, not the one file.
- **761 of 9,352 pseudocode functions (8.1%) contain a `JUMPOUT(...)`** — the decompiler gave up
  partway. `Main` is one of them. 447 of those are in `_FREE_FUNCTIONS.c`.
- **111 functions the decompiler refused outright** (`_FAILED.txt`), including
  `AudManagerBase::Update`, `AudManagerBase::UpdatePaused`, `AboutMenu::Update`,
  `Aud3DObject::CalculatePan`.
- **151 of the kit's 685 "class" folders are not MM2 classes at all** (41,670 bytes): `?$ImVector`,
  `ImGuiDockNode`, `?$CppBindClassMethodBase`, `MM2`, `GameEventDispatcher`, `$$CBUvariant_info`,
  and ~80 folders named `XPAV<Something>` which are mangled-name fragments (`PAV` = "pointer to
  class") from MM2Hook's `hook::Thunk<>` templates, not classes.
- Conversely, three real classes have **no** kit folder: `dgLinkData`,
  `phInertialCS::TerrainContact`, `ptxGlass::ptxShard`.

### Trap 9 — the generated headers do not compile

They are declaration dumps, not valid C++. Before the first build:

- **655 `_SEH` funclet symbols are declared as members across 286 headers**, 159 of them as
  `ARTS_IMPORT static void ~ClassName;` — a destructor name used as a variable. Also
  `ARTS_IMPORT static void Load;` in `asnode.h`. These are compiler-generated exception funclets
  (681 of them in the binary, 25,466 bytes); they are real symbols in `game.asm` but they are not
  class members and must not be declared. Filter `mangled.endswith("_SEH")` in
  `tools/genheaders.py`.
- **`stream/stream.h` line 87 is syntactically broken**: `ARTS_IMPORT static char (* sm_Buffers;`
  (the real type is `char (*)[256][...]`).
- **21 distinct nested types are referenced but never declared**, e.g.
  `memMemoryAllocator::node` and `::freenode` (used in `memmemoryallocator.h` for `Link`, `Unlink`
  and `m_Buckets[32]`), `audManager::AUDTYPE` (27 uses), `gfxImage::gfxImageFormat`,
  `phBoundPolygonal::Segment`, `mmHudMap::IconType`. Each needs a hand-written declaration.
- `check_size` is called 531 times and defined zero times; `ARTS_IMPORT`, `i32`, `f32`, `b32` the
  same.

### Trap 10 — the SEH funclets are separate symbols with no C++ equivalent

681 `_SEH` symbols (25,466 bytes) exist as their own map entries and their own `PROC`s:
`??1asNode@@UAE@XZ_SEH`, `?Load@asNode@@UAE_NXZ_SEH`, `?Save@asNode@@UAE_NXZ_SEH`,
`?Main@@YAHXZ_SEH`, `?BeginPhase@@YAX_N@Z_SEH`. Concentrated in `UIMenu` (22), free functions (20),
`mmInterface` (17), `mmSingleStunt` (14), `mmPlayerData` (10).

When you port the parent function, MSVC generates its own exception tables and the assembly funclet
becomes orphaned — still assembled, still `PUBLIC`, referenced by nothing. `tools/asm.py` will not
notice, because it only tracks symbols that appear above an `ARTS_*` declaration
(`MANGLED = re.compile(r"^\s*//\s*(\?[^\s|]+)")`). Decide the policy before the first port of a
function with a funclet: strip the funclet's PROC at the same time, and make sure the try/finally
semantics of the original are reproduced (they are usually a destructor call on unwind).

### Trap 11 — `.data` is currently at the wrong virtual address

`docs/harness.md` says "`.rdata` and `.data` land back at their original virtual addresses".
`tools/link.py` says the opposite in a comment, and the binaries agree with `link.py`:

| section | `midtown2.exe` | `build/OpenMM2.exe` |
|---|---|---|
| `.text` | VA `0x001000`, VS `0x1AE1BD` | VA `0x001000`, VS `0x1AF000` |
| `.rdata` | VA `0x1B0000`, VS `0x011FDB` | VA `0x1B0000`, VS `0x0144EE` |
| `.data` | **VA `0x1C2000`** | **VA `0x1C5000`** |

The linker builds its own import table into `.rdata`, adding ~0x2513 bytes, which pushes `.data`
0x3000 high. `/MERGE:.idata=` is rejected (LNK1272), so it cannot be fixed on the link line.

Two consequences you will hit:

1. **Every `.data` address in `midtown2.map`, the pseudocode and Ghidra is 0x3000 low relative to
   the running harness build.** The `byte_6B0468` that `BeginPhase` touches is at `0x6B3468` in
   `OpenMM2.exe`. Any memory-viewer debugging must add the offset.
2. Code references to `.data` are symbolised (32,033 absolute relocations) and follow their labels,
   so they are fine. The exposure is the **3 unresolved references** noted in `harness.md`, plus any
   data word that is a `.data` pointer but did not exactly match a known symbol address and so was
   left as a raw `dd`. Those now point 0x3000 too low, silently.

Fix the documentation, and treat this as the first thing to check when a data-driven subsystem
misbehaves for no reason.

### Trap 12 — uninitialised members, the MM1 #1 bug class, with an MM2-specific mechanism

`memMemoryAllocator::Allocate` (`0x5767A0`, 630 B) does **not** zero memory. It returns a block off
a free list, whatever was in it last. `operator new` is a two-line wrapper over it. So:

- Any member the original constructor sets and yours does not is **garbage**, not zero.
- Any member the *original* also leaves uninitialised must be left uninitialised — reproducing the
  bug is the requirement (MM1_METHOD §3: "1:1 with the binary, including the bugs").
- `lvlInstance`'s pool allocator makes this worse: `operator delete` is a no-op, so memory is
  recycled without ever being touched.

Symptoms to recognise: works sometimes, NaNs in physics, a pointer that passes `if (p)` and then
crashes. Check for an unset member before anything else.

### Trap 13 — `datArgParser::Exists` does not exist

A small illustration of Trap 2 that you will hit within the first hour. `Main`'s pseudocode calls
`datArgParser::Exists("nolog")`. There is no such symbol. The real one is
`?Get@datArgParser@@SA_NPBD@Z` — `static b32 Get(const char*)`. Hex-Rays invented the friendly
name. **Always confirm against the mangled name**, which is exactly why every generated declaration
carries its mangled symbol in the comment above it.

### Trap 14 — six `midtown2.map` names describe an ABI their code does not have

Trap 2 is about the kit's names being wrong. This is worse, because it is the **map** — the thing
Trap 13 tells you to trust — and it is not a naming problem, it is a calling-convention problem.

A mangled name encodes the parameter list, and MSVC turns that parameter list into a fixed
`ret imm16` in the callee. If the map's name says one argument and the machine code pops two, then
the symbol cannot be reimplemented **at all**: writing the declaration the name demands produces a
function that pops four bytes where every caller in `game.asm` pushed eight, and the stack is off
by four from then on. There is no diagnostic and the crash is nowhere near vector7.

Six symbols are in this state. Each was read off the disassembly and byte-verified against
`midtown2.exe` at the map's own RVA, so this is not a mis-extraction:

| symbol | name says | code does |
|---|---|---|
| `?Add@Vector3@@QAEXABV1@@Z` `0x45CE00` | `void Add(const Vector3&)`, `ret 4` | `ret 8`; hidden return slot + one ref; a by-value `operator+` |
| `?Scale@Vector3@@QAEXABV1@M@Z` `0x45CE40` | `void Scale(const Vector3&, f32)` | `ret 8`; hidden return slot + a float; a by-value `operator*(f32)` |
| `?Subtract@Vector3@@QAEXABV1@@Z` `0x45CD30` | one argument, `ret 4` | `ret 8`; `this = a - b` |
| `?SubtractScaled@Vector3@@QAEXABV1@M@Z` `0x474000` | subtracts | **adds**: `this += a * s` |
| `?AddScaled@Vector3@@QAEXABV1@0M@Z` `0x484420` | three arguments, `ret 12` | `ret 8`, and **subtracts**: `this -= a * s` |
| `??XVector3@@QAEXABV0@@Z` `0x43DD50` | `operator*=(const Vector3&)` | second argument is a float; the body is `Scale(const Vector3&, f32)` |

The call sites confirm the reading rather than the name: at `0x44B481`, inside `psdl_draw`, two
dwords are pushed and left for `0x45CE00` to clean while `mov ecx, eax` supplies `this` from the
previous call's return — the by-value convention exactly, and nothing like `void Add(const
Vector3&)`.

**What to do.** These six stay `ARTS_IMPORT` permanently. Do not "fix" the header to match the
code either — that changes the mangled name and the assembly's callers stop resolving.

**How to avoid walking into the next one.** `py tools/verify_arity.py` does the comparison
mechanically over every `__thiscall` code symbol - 6,315 agree, 417 are skipped as undecidable
(a by-value class parameter, or a function that ends in a tail call), and **12 disagree**:

```
0x410E90  name= 4 code= 0  ?ChangePlayerData@mmInterface@@AAEXH@Z
0x42BE10  name=16 code=20  ?Init@vehCar@@QAEXPBDHH_N@Z
0x435DE0  name= 4 code=72  ?AnyWPHits@mmWaypoints@@QAEHH@Z
0x43C820  name= 4 code= 8  ?SetPositionData@mmNetObject@@QAEHK@Z
0x43DD50  name= 4 code= 8  ??XVector3@@QAEXABV0@@Z
0x45CD30  name= 4 code= 8  ?Subtract@Vector3@@QAEXABV1@@Z
0x45CE00  name= 4 code= 8  ?Add@Vector3@@QAEXABV1@@Z
0x464430  name= 4 code= 0  ?DrawPhysics@lvlInstance@@UAEXABVVector3@@@Z
0x484420  name=12 code= 8  ?AddScaled@Vector3@@QAEXABV1@0M@Z
0x572B50  name= 8 code= 0  ?InitResults@netZoneScore@@QAEXH_N@Z
0x573480  name= 4 code= 0  ?Init@zipFile@@QAE_NPBD@Z
0x57ADE0  name= 4 code= 0  ?Load@pedAnimationInstance@@AAEPAVpedAnimation@@PBD@Z
```

It uses the RE kit's function extents rather than the map's size, because the map's size is
distance-to-the-next-symbol and would otherwise put Trap 7's absorbed static constructors' `ret` at
the end of the function - that alone accounts for the difference between 12 real hits and the 78
the naive version reports.

**Arity is necessary, not sufficient.** Two of the six Vector3 symbols - `?Scale@Vector3@@QAEXABV1@M@Z`
and `?SubtractScaled@Vector3@@QAEXABV1@M@Z` - pop exactly what their names say and are still wrong,
one because the argument the name calls a reference is loaded as a float and one because the body
adds where the name subtracts. The tool cannot see either. Read the body.

**A seventh, found by doing exactly that.** `?InvScale@Vector3@@QAEXM@Z` at `0x595B70` is a clean
`ret 4` and `verify_arity.py` passes it, because the stack does balance. It is still unportable, and
for the worst of the three reasons: the *type* of the argument is wrong, not its size.

```asm
00595B70  55              push ebp
00595B71  8bec            mov  ebp, esp
00595B73  8b4508          mov  eax, dword ptr [ebp+8]   ; the "float" argument, into a register
00595B76  d94008          fld  dword ptr [eax+8]        ; and dereferenced. it is a pointer.
00595B79  d84908          fmul dword ptr [ecx+8]
00595B7C  d94004          fld  dword ptr [eax+4]
00595B7F  d84904          fmul dword ptr [ecx+4]
00595B82  dec1            faddp st(1)
00595B84  d900            fld  dword ptr [eax]
00595B86  d809            fmul dword ptr [ecx]
00595B88  dec1            faddp st(1)
00595B8A  5d              pop  ebp
00595B8B  c20400          ret  4                        ; result left in st(0): it RETURNS a float
```

That is `a.z*z + a.y*y + a.x*x` - instruction for instruction the body of
`?Dot@Vector3@@QBEMABV1@@Z` at `0x466470`. The symbol names an `f32 Dot(const Vector3&) const`, so
declaring the `void InvScale(f32)` its name demands hands the function a float where it wants an
address, and the dereference reads whatever the float's bit pattern points at. There is no crash
near vector7 and no stack imbalance for anything to notice.

**So the rule is:** `verify_arity.py` narrows the search, it does not clear a symbol. Every function
still has to be read before it is written, and what to read for is (a) the size popped, (b) whether
each argument is used as a value or as an address, and (c) whether the body does what the verb says.

---

---

## Quick reference

```sh
# what is really at an address
py -c "import json;s=json.load(open('data/symbols.json'));print([r for r in s if r['rva']==0x401AA0])"

# is this class portable today?
grep -l "Offset order is mandatory" code/midtown2/**/*.h

# does this class have a vtable in .rdata?
py -c "import json;s=json.load(open('data/symbols.json'));print([r['demangled'] for r in s if \"\`vftable'\" in (r['demangled'] or '') and r.get('class')=='asNode'])"

# does MM1 already implement this symbol?
grep -rn '?Dot@Matrix34@@QAEXABV1@0@Z' ../Open1560Dev/Open1560/code/midtown/

# is anything else hiding inside this PROC?
grep '"0x0047' MM2_RE_KIT/MM2_PSEUDOCODE/_INDEX.csv

# does the code pop what the mangled name says it pops?  (Trap 14)
py tools/verify_arity.py
```

**Regenerate the data**: `py tools/symbols.py && py tools/vtables.py && py tools/genheaders.py && py tools/inventory.py`

---

## Picking the next function: screen before you write

Three tests reject a port, and all three can be applied **before** any C++ exists. Applying them
up front is the difference between choosing a target and discovering a wasted build.

1. **Arity.** The mangled name's stack bytes must equal the code's `ret imm16`.
   `py tools/verify_arity.py` lists every symbol where they disagree.
2. **Extent.** No dword in `.data`/`.rdata` may point strictly *inside* the function. Build gate 6
   enforces this; a pointer into the middle of a function is usually a switch jump table, and it
   cannot survive the symbol being repointed at C++.
3. **Purity.** No calls and no absolute addresses, so the body needs nothing added to
   `LINKABLE_GLOBALS` and no unrecovered class layout.

Against the tree as of this writing: **544** unported functions pass the first two tests, and
**146** pass all three. That 146 is the honest ready-to-port list.

### Known-unportable, with the reason

Do not re-derive these; each cost a build or worse.

| Symbol | Why |
|---|---|
| `Vector3::Add`, `Vector3::Subtract`, `Vector3::operator*=` | name encodes 4 bytes of arguments, code pops 8 |
| `Vector3::AddScaled` | name encodes 12, code pops 8 |
| `Vector3::InvScale` | passes the arity check and is still wrong: takes a pointer where the name says `float`, and returns a dot product in `st0` where the name says `void` |
| `Matrix34::Transform` | four dwords in `.data` are a switch jump table pointing inside it, and `game.asm` spells one as `?Transform@... + 96`, i.e. relative to the symbol. Porting repoints that at the C++ body. The note is also in `matrix34.cpp` itself. |
| `gfxTexture::GetColor` | body really is four bytes, but the recorded extent is 128 and `.data` points at `0x0045D190` inside it. Needs the extent narrowed first. |

`aiPath` supplies 35 of the 146 pure candidates and **none are takeable yet**: its `SharpTurn*`
accessors read `[this+0x24]` as a dword and index `[this+0x28]`, but `data/layouts.json` types 0x24
as `u8` and has no member at 0x28 at all. Correcting that layout unlocks the richest remaining vein
in the binary.
