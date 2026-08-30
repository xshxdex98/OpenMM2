# Work in progress

Source that is **deliberately kept out of the build**, referenced from `tools/build.py`.

The harness is only worth having if a runnable binary can be produced at any moment. The whole
design is that the game plays at 0% reimplemented and every bug is found by running it, so a single
source file that does not compile takes that away. Rather than let a half-finished port block the
build, it waits here.

Exclusion is a **skip list**, not a directory move: files get written into `code/` while a build is
running, so a file moved aside simply reappears. Set `MM2_WIP` to a comma-separated list of
basenames to hold more back.

## What is here

### `citylevel.cpp` - `cityLevel::SetObjectDetail`

The whole of the "Object Detail" graphics setting: a four-entry table of LOD distances written into
four `lvlInstance` statics. It is finished and correct, and it is the port that makes the
draw-distance fix a value rather than two byte patches into an instruction's immediates.

It is held back because **the header chain does not compile**. `city/citylevel.h` pulls in
`lvl/lvlsdl.h`, `ph/phbound.h` and `data7/datcallback.h`, which `tools/genheaders.py` emits with
`unsigned const i16*` - illegal, because `unsigned` cannot qualify a typedef. The dropped members
then cascade into failed `check_size` assertions that look like layout defects and are not.

Fixing `genheaders.py`'s qualifier emission unblocks this file as it stands. See the trap of the
same name in [`../docs/port_order.md`](../docs/port_order.md).
