# MM2's resolution cap, located

Confirmed live: running the retail binary prints

```
Max display resolution restricted to 1600 x 1200
```

## Where it is

`AutoDetectCallback` (entry `0x004AC030`), at `0x004AC300`. The DirectDraw enumeration callback
picks a maximum from available video memory and stores it in a global:

```
                      ; ja +0x11   - branch on the video-memory test above
b8 00 04 00 00        mov eax, 0x400        ; 1024
b9 00 03 00 00        mov ecx, 0x300        ;  768
a3 fc 44 68 00        mov [0x006844FC], eax
eb 0f                 jmp short
b8 40 06 00 00        mov eax, 0x640        ; 1600
b9 b0 04 00 00        mov ecx, 0x4B0        ; 1200
a3 fc 44 68 00        mov [0x006844FC], eax
51 50 68 ...          push ecx / push eax / "Max display resolution restricted to %d x %d"
```

So the cap is two immediates on each branch, and the chosen width lands in the global at
`0x006844FC` - which the linker map names `gfxMaxScreenWidth`.

The height does **not** go to the adjacent slot. Its global is `gfxMaxScreenHeight` at
**`0x006844D8`**, 0x24 bytes earlier; `0x00684500` is `sm_UseInternal`, an unrelated bool. Both
names are in `data/symbols.json` and in the kit inventory, and `ResCallback` (`0x4AC6F0`) reads the
pair together when it rejects a mode "due to memory requirements". See `docs/gfx_layouts.md`.

## What to change for 1440p

Raise the immediates on the high branch to 2560 x 1440. Because OpenMM2 owns the assembly, this is
an edit to `game.asm` rather than a runtime patch - and once `AutoDetectCallback` is reimplemented
in C++ it becomes an ordinary constant.

Do not simply raise it and stop: `docs/ui_scaling.md` documents what else breaks above 640x480 -
in particular the CSV-driven widgets, which pin to the top-left 640x480 corner regardless of
resolution, and the fixed-pixel fonts.

## Note on the video-memory test

The branch reads the value dgVoodoo reports (67,108,864 bytes here). A modern GPU behind a real
OpenGL backend should report far more, so once the device layer is replaced this test picks the
high branch unconditionally - the cap becomes the only thing standing in the way.
