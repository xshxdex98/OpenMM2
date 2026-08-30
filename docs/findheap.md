# `memMemoryAllocator::FindHeap failed on ptr 1112d2a0`

**Conclusion first: this is not an allocator bug, not a relocation bug, and not a bad pointer. It is
the last line of a crash.** The game died of an access violation in
`mmInterface::PlayerFillStats`, `ExceptMain` unwound `Main`, and unwinding `Main` runs
`~memSafeHeap` and `~memMemoryAllocator` — which *deregister the 32 MB heap*. Everything freed after
that point is a valid heap pointer that no longer has a heap to be found in, and the first such
`delete` calls `Abortf`.

The `play_capture.txt` in the repo root contains **two independent runs** that show the whole chain,
and they agree to the byte.

---

## 1. What `FindHeap` actually does

`?FindHeap@memMemoryAllocator@@SAPAV1@PAX@Z`, `0x005766B0`, 57 bytes of code (0x40 with padding).
Transcribed from `game.asm.pristine` and re-disassembled out of both `midtown2.exe` and
`build/OpenMM2.exe` with capstone; the two images agree except for the relocated `.data` operands.

```asm
005766b0  push  ebp
005766b1  mov   ebp, esp
005766b3  mov   eax, [memMemoryAllocator::First]   ; retail 0x6B46AC / ours 0x6C36AC
005766b8  mov   edx, [ebp+8]                       ; ptr
005766bb  test  eax, eax
005766bd  push  esi
005766be  je    5766d6                             ; empty list -> Abortf
005766c0  mov   ecx, [eax+0x04]                    ; heap->m_pHeap
005766c3  cmp   edx, ecx
005766c5  jb    5766d0                             ; below the base -> next heap
005766c7  mov   esi, [eax+0x08]                    ; heap->m_HeapSize
005766ca  add   esi, ecx                           ; base + size
005766cc  cmp   edx, esi
005766ce  jb    5766e6                             ; FOUND: return eax
005766d0  mov   eax, [eax]                         ; heap = heap->Prev   (link at +0x00)
005766d2  test  eax, eax
005766d4  jne   5766c0
005766d6  push  edx
005766d7  push  offset "memMemoryAllocator::FindHeap failed on ptr %x"   ; 0x5DBE14 / 0x5EAE14
005766dc  call  Abortf
005766e1  add   esp, 8
005766e4  xor   eax, eax
005766e6  pop   esi
005766e7  pop   ebp
005766e8  ret
```

So it is a **singly-linked list walk with one unsigned range test per node**:
`m_pHeap <= ptr < m_pHeap + m_HeapSize`. It reads nothing but three fields and the `First` head.

Its only two callers are `operator delete` (`??3@YAXPAX@Z`, `0x577380`) and
`operator delete[]` (`??_V@YAXPAX@Z`, `0x5773C0`), both of which do
`FindHeap(p)` → `mov ecx, eax` → `Free(p, false)`. **`FindHeap` is on the path of every `delete` in
the game**, and a failure kills the process (`Abortf` prints at level 5 — the `Fatal Error: ` prefix
— then writes to address 0).

### Where the set of heaps lives, and what is in it

`First` is written in exactly two places, both in this class:

- `??0memMemoryAllocator@@QAE@XZ` (`0x5766F0`): `this->Prev = First; First = this` — push front.
- `??1memMemoryAllocator@@QAE@XZ` (`0x576720`): `First = this->Prev; this->m_pHeap = 0` — pop,
  **assuming `this` is the head**. Allocators must therefore die in LIFO order.

There are exactly **two** `memMemoryAllocator` objects in the whole binary, and both are stack
locals:

| # | where | heap memory | size | registered |
|---|---|---|---|---|
| 1 | `_main` `[ebp-0xDC]`, `0x4023A2` | `pShadowMem` — a static buffer in `.data`, retail `[0x5DECB8, 0x5E0CB8)`, ours `[0x5EDCB8, 0x5EFCB8)` | `0x2000` | first, so it is the list *tail* |
| 2 | `Main` `[ebp-0x1A4]`, `0x4015A4` | `VirtualAlloc(NULL, 0x2000000, MEM_COMMIT, PAGE_READWRITE)` via `memSafeHeap::Init` | 32 MB | second, so it is `First` |

`memMemoryAllocator::Current` (retail `0x6B46A8` / ours `0x6C36A8`) is set to #1 in `_main`, then to
#2 at `0x4015EB`. `operator new` allocates from `Current`; `operator delete` finds the owner by
address. So the walk is: **32 MB game heap, then the 8 KB `.data` shadow heap, then abort.**

`memSafeHeap::Init` unconditionally clears its "safe" flag (`mov byte ptr [esi+0x14], 0` at
`0x57724F`), so the four-quarter rotate-and-unmap mode is dead code in the shipped build:
`Restart` always re-`Init`s the allocator on the *same* base and the same size.

### One non-obvious property worth recording

`m_HeapSize` is **not constant**. `memMemoryAllocator::Allocate` has a second path, taken when the
gate byte at `zipFile+5792` is set (it is `0x01` in retail and in our build) **and**
`datCurrentMemoryAlign != 0`:

```asm
mov  esi, [edi+8]          ; m_HeapSize
sub  esi, ebx              ; minus the request
and  esi, ~(align-1)       ; rounded down
...
mov  [edi+8], esi          ; m_HeapSize = esi     <-- the heap's TOP MOVES DOWN
mov  eax, [edi+4]
add  eax, esi              ; return m_pHeap + esi
```

Aligned allocations are carved off the **top** of the heap and shrink `m_HeapSize`, so the returned
pointer sits exactly on `FindHeap`'s exclusive upper bound. **Any aligned block is un-`delete`-able
by construction** — `FindHeap` will always reject it. `datCurrentMemoryAlign` is set (to 16) and
restored around specific loads in `cityLevel::Load`, `lvlMultiRoomInstance::Create`,
`modShader::AddStaticMaterial`, `modShader::LoadShaderSet`, `modStatic::Clone`,
`phBoundGeometry::Init` and `phBoundTerrain::Load`; those pools are released only by a heap
`Restart`. **Anyone porting one of those functions must not add a `delete` to it.** That is a real
trap, but it is not what happened here — see §3.

---

## 2. The evidence: two runs, identical to the byte

`play_capture.txt` interleaves several processes. Filtering to the two MM2 runs that produced a
fatal error:

| | run A (pid 10120) | run B (pid 21060) | difference |
|---|---|---|---|
| `EAX` | `1112f638` | `1101f638` | `0x110000` |
| `EBX` (= `this`, `mmInterface`) | `1111e2c8` | `1100e2c8` | `0x110000` |
| `ECX` | `0019fe7d` | `0019fe7d` | 0 |
| `ESI` | `00000384` | `00000384` | 0 |
| `EDI` | `006c0144` | `006c0144` | 0 |
| `EBP` | `001a0031` | `001a0031` | 0 |
| `ESP` | `001af8b0` | `001af8b0` | 0 |
| exception | `c0000005 @ 0x40f5f9` | `c0000005 @ 0x40f5f9` | — |
| **FindHeap ptr** | **`1112d2a0`** | **`1101d2a0`** | **`0x110000`** |

Both runs, in order, in the same pid's stream:

```
City = San Francisco
Error: nodeGetBitmap() - 'ama_rank_desc' not found
Couldn't open '...\OpenMM2.MAP'EAX=1112f638 EBX=1111e2c8 ...
Exception c0000005 at EIP=0x40f5f9(Unknown)
ExceptMain: Abnormal exit.
Max files open at once: 2
...
Fatal Error: memMemoryAllocator::FindHeap failed on ptr 1112d2a0
```

Read the table. The three heap-shaped values move by **exactly** `0x110000` between runs and the
stack-shaped values do not move at all. `0x110000` is the run-to-run jitter of the base
`VirtualAlloc` returns for the 32 MB heap. The offsets *inside* the heap are identical:

```
ptr - mmInterface  =  0xEFD8   in both runs
```

So `1112d2a0` is a fixed offset into the game heap — an ordinary allocation made at a deterministic
point during `MainPhase`, roughly 30 KB past the `mmInterface` object. **The pointer is fine.** The
`Fatal Error` line is also the *last* line the process printed, after `ExceptMain: Abnormal exit.`
and after the file-handle high-water report that `Stream` prints during teardown.

---

## 3. The mechanism: unwinding `Main` deregisters the heap

`ExceptMain` (`0x4022B0`) is a `__try`/`__except` around `Main`:

```asm
004022ea  mov   [ebp-4], 0
004022f1  call  Main
...                                    ; __except filter:
004022ff  call  datStack::ExceptionFilter      ; prints the register dump + "Exception c0000005 at EIP="
...                                    ; __except handler:
00402308  mov   esp, [ebp-0x18]
0040230b  push  "ExceptMain: Abnormal exit."
00402310  call  Displayf
00402318  mov   [ebp-4], -1
0040231f  call  datArgParser::Kill
```

`Main` has two objects under SEH scope, and `game.asm.pristine` carries the funclets the unwinder
calls, verbatim:

```asm
Unwind@005a8deb:  lea ecx, [ebp-0x34]  ; the memSafeHeap
                  jmp ??1memSafeHeap@@QAE@XZ
Unwind@005a8de0:  lea ecx, [ebp-0x1a4] ; the memMemoryAllocator
                  jmp ??1memMemoryAllocator@@QAE@XZ
```

Follow them:

1. `~memSafeHeap` → `memSafeHeap::Kill` → `Deactivate` → **`memMemoryAllocator::Kill`**, which is
   `m_pHeap = 0; m_Initialized = 0`. The 32 MB heap is still `First`, but it now claims to cover
   `[0, 0)`, so it matches nothing.
2. `~memMemoryAllocator` → `First = Prev`, i.e. the head becomes the 8 KB `.data` shadow heap, and
   its own `m_pHeap` is zeroed too.

From that instant, `FindHeap` can only ever succeed for a pointer inside
`[0x5EDCB8, 0x5EFCB8)`. `1112d2a0` is not in it — checked. Every subsequent `delete` of a real game
object (static destructors via `exit`, `_main`'s own teardown, anything after the unwind) is
guaranteed to abort, and the first one wins.

That ordering is **original 1999 code, reached by an original 1999 path**. Nothing in the harness or
in any port is involved. It is a latent shutdown-order defect in MM2 that is only reachable when
`Main` exits through the `__except` arm — which retail almost never does.

---

## 4. What was ruled out, and how

The brief's leading hypothesis — a stale or wrongly relocated heap registry — was tested first and
is **false**. All of these were checked against `build/OpenMM2.exe` and `midtown2.exe`:

| check | result |
|---|---|
| `?First@memMemoryAllocator@@0PAV1@A` placement | `0x6B46AC` → `0x6C36AC`, exactly `.data +0xF000`, the same displacement `?SAILBOAT@@3PAVgizSailboatMgr@@A` and every other `.data` symbol takes (`build/OpenMM2.exe.map`) |
| `?Current@memMemoryAllocator@@0PAV1@A` | `0x6B46A8` → `0x6C36A8`, same |
| the operand *inside* `FindHeap` | disassembled from both images: retail `a1 ac466b00`, ours `a1 ac366c00`. Correctly rewritten, not merely different |
| the operands inside the ctor (2×) and dtor (1×) | all three rewritten to `0x6C36AC`; ctor's `datDisplayUsed` operand also correct |
| the `Abortf` format-string operand | `0x5DBE14` → `0x5EAE14`, and the bytes at both read `memMemoryAllocator::FindHeap failed on ptr %x` — byte-identical |
| `operator delete` / `operator delete[]` | byte-identical in both images; both resolve to `game.obj` in the map, so no CRT `operator new`/`delete` is linked in and no allocation can come from `malloc` |
| the aligned-path gate byte `zipFile+5792` | `0x01` in **both** images — the aligned path is vanilla, not something we switched on |
| `tools/verify_operands.py` | 0 unrelocated `.text` operands, so nothing writes to a stale `.data` address (this is the class that scribbled into `Stream::sm_Buffers` last session) |
| `tools/verify_strings.py`, `verify_dataformats.py`, `verify_extents.py` | pass |

Also ruled out:

- **The pointer came from the CRT / another heap.** `Allocate` has no fallback allocator; it calls
  `Quitf("Heap overrun")` when it runs out. And the run-to-run delta proves the pointer tracks the
  game heap's base.
- **The pointer came from the aligned top-of-heap path** (§1, the `m_HeapSize` shrink). It cannot
  have: `1112d2a0` is `0xEFD8` above `mmInterface`, i.e. ~120 KB into a 32 MB heap that was 6.5 MB
  used at the time. Aligned blocks live at the far end.
- **A second allocator unlinked out of order.** There are only the two constructions listed in §1,
  and their lifetimes nest correctly.

### One unrelated defect the sweep did surface

`tools/verify_relocated.py` **FAILS with one finding**, which is worth fixing but is not this bug:

```
0x005DCD74  (cbMultiByte+0x18)  holds 0x005B6514 ('.bat'), should hold 0x005BF824
```

It is the last survivor of the CRT `.com/.exe/.bat/.cmd` extension table noted in `docs/STATUS.md`
session three. It is a read-only string pointer used by `_spawn`/path search, it points `0x9310`
low into `.rdata`, and nothing in gameplay reads it.

---

## 5. The actual bug to fix

**`?PlayerFillStats@mmInterface@@AAEXXZ + 0x199` (`0x0040F5F9`)**, which is:

```asm
0040f5ee  call  ?DisplayDriverInfo@MainMenu@@QAEXPAD0000H@Z
0040f5f3  lea   ecx, [ebp-0x1b4]              ; the mmPlayerRecord local
0040f5f9  mov   dword ptr [ebp-4], -1         ; <-- FAULTS. SEH try-level store.
0040f600  call  ??1mmPlayerRecord@@UAE@XZ
```

This is the same crash `wip/README.md` recorded for the quarantined Matrix33/Matrix66 round —
"`mmInterface::PlayerFillStats` with a misaligned frame pointer" — and Matrix33/Matrix66 are **not**
in `data/ported.json` today, so that round was not its cause. The more likely reading is that the
clean 65,795-line run simply never reached this function; it is the driver-record / stats panel, and
both crashing runs print `Error: nodeGetBitmap() - 'ama_rank_desc' not found` on the line before.

What the register dump establishes:

- `EBP = 0x001A0031`. It is **not 4-byte aligned**, so it cannot be any value `mov ebp, esp`
  produced, and it sits ~63 KB *below* `ESP = 0x001AF8B0` — far past the stack's guard page, which
  is why the store faults.
- The dump really is the faulting context, not the handler's: `ECX = 0x0019FE7D` is exactly
  `EBP - 0x1B4`, the `lea` on the instruction immediately before the fault.
- Its low 16 bits are `0x0031` = `'1'`, `'\0'`. A saved `EBP` of `0x001AXXXX` with its low two bytes
  replaced by a NUL-terminated `"1"` is the signature of a **two-byte string overrun onto a saved
  frame pointer**, i.e. the caller returned via `pop ebp` off a smashed slot.
- The immediately preceding call is `MainMenu::DisplayDriverInfo` (`0x506990`), whose only stack
  buffer is `[ebp-0xC]`, 12 bytes, filled by `sprintf(buf, "%5d", score)`. **That one is innocent**:
  `ESI = 0x384` = 900 is the score it was handed, which formats to six bytes. The smash is deeper —
  `DisplayDriverInfo` calls `mmTextNode::SetString` six times, and `SetString` does
  `strncpy(pTextEntries + index*272 + 0x10, str, 0xFF)` **with no bound on `index`** against
  `mmTextNode::MaxEntries`; the indices used here run to 9, so the node must have been `Init`ed with
  at least ten entries or that `strncpy` writes 255 bytes outside the block.

`?Init@mmTextNode@@QAEXHHHHHH@Z` **is ported** and is the function that sizes that block
(`operator new(272 * MaxEntries)`). Its allocation arithmetic matches the original's
`((n << 4) + n) << 4`, so the size is right — but it is the first thing to look at, together with
whichever `Init` overload creates `MainMenu`'s node at `[MainMenu + 0xC8]`. Note also that
`MainMenu` is one of the classes `tools/verify_sizes.py` flags as wrong in `data/layouts.json`
(declared `0xB8`, really `0x100`).

**Fixing the access violation removes the `FindHeap` abort.** There is nothing to fix in the
allocator.

---

## 6. The map errors are not related

`Error: sf: room count mismatch, please re-import.` comes from `cityLevel::Load`
(`MM2_RE_KIT/MM2_PSEUDOCODE/cityLevel/cityLevel.c:533`) and is a **lightmap** check:

```c
Stream::Read(v22, &Buffer, 4);
if ( LODWORD(Buffer) == cityLevel::SDL.PageCount )
    io = operator new(4 * cityLevel::SDL.PageCount), Stream::Read(v22, io, ...);
else
    Errorf("%s: room count mismatch, please re-import.", label);
```

The `.lmap`'s page count disagrees with the loaded SDL's, so `io` stays null. The consumer loop is
null-guarded — the disassembly at `0x444453` substitutes `-1` for the lightmap index — so the rooms
simply come up without lightmap indices. Nothing is allocated, nothing is freed, nothing is written
out of bounds. It is content damage from the installed addon maps (the same run loads `MM2HOOK.AR`
and `COPLIVERYSWAPPER.AR`), and re-importing the city would clear it.

`Room NNN has Water of Death(tm)` is a plain `Displayf` listing rooms flagged as instant-drown
water. It is informational and unconditional.

Neither can produce a bad free, and neither is on any path to `mmInterface::PlayerFillStats`.

---

## 7. Two things to carry forward

1. **`memMemoryAllocator` is not `delete`-safe against the aligned path.** `datCurrentMemoryAlign`
   turns `operator new` into a top-of-heap bump allocator that *lowers* `m_HeapSize`, so its blocks
   are permanently outside `FindHeap`'s range. A port of `cityLevel::Load`,
   `lvlMultiRoomInstance::Create`, `modShader::AddStaticMaterial`, `modShader::LoadShaderSet`,
   `modStatic::Clone`, `phBoundGeometry::Init` or `phBoundTerrain::Load` must reproduce the
   save/set/restore of `datCurrentMemoryAlign` exactly and must never free what it allocates there.

2. **A `Fatal Error` from the allocator during shutdown is a symptom, not a cause.** Once
   `ExceptMain` prints `Abnormal exit.`, the heap is gone and *any* later `delete` aborts. When this
   line appears, scroll up: the real defect is the exception above it. Grouping
   `play_capture.txt` by pid before reading it is what made this visible — the fatal line and the
   exception that caused it were 90 lines apart with three other processes interleaved between them.
