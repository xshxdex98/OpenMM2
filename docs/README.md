# Documentation index

Thirty-odd files accumulated in the order the work happened. This is what each one is for.

## Start here

| | |
|---|---|
| [ARCHITECTURE.md](ARCHITECTURE.md) | What the project is, how the harness works, what every build gate catches, and where it is weak. Read this first. |
| [running.md](running.md) | How to launch the game. `-gl` is mandatory; the working directory must be the install. |
| [harness.md](harness.md) | The assemble-and-link model in detail - the piece everything else depends on. |

## Where the project stands

| | |
|---|---|
| [STATUS.md](STATUS.md) | What runs, and the five defects that stood between "links" and "runs". |
| [PROGRESS.md](PROGRESS.md) | Generated each build: ported count and percentage. |
| [gameplay_defects.md](gameplay_defects.md) | Open and fixed gameplay bugs, **including theories that were investigated and disproven**. |

## Reimplementing functions

| | |
|---|---|
| [port_order.md](port_order.md) | What to port and in what sequence, the traps, and the three-test screen for choosing a candidate. |
| [vtables.md](vtables.md) | Porting a class that has a vtable - the 335-class blocker and how `asm_vtables.py` gets round it. |
| [vtable_order.md](vtable_order.md) | Why declaration order matters: a compiler assigns slots in declaration order, and the generated headers sort alphabetically. |
| [inventory.md](inventory.md) | What is in `midtown2.exe`, counted. |
| [mm1_counterparts.md](mm1_counterparts.md) | MM2 class → the Open1560 class that already solved it. |
| [mislabeled.md](mislabeled.md) | 349 functions the kit names wrongly, re-identified against the map. |
| [harness_audit.md](harness_audit.md) | Deliberately hunting for a sixth class of silent corruption the gates could not see. |

## Graphics and the OpenGL layer

| | |
|---|---|
| [opengl_plan.md](opengl_plan.md) | The plan for replacing DirectDraw / D3D7. |
| [opengl_activation.md](opengl_activation.md) | What actually happened, including where the plan turned out to be wrong. |
| [gfx_layouts.md](gfx_layouts.md) | The graphics classes' member layouts. |
| [resolution.md](resolution.md) | Where the 1600x1200 cap lives in the binary. |
| [resolution_blueprint.md](resolution_blueprint.md) | What Open1560 did for arbitrary resolution and how much transfers. |
| [ui_scaling.md](ui_scaling.md) | UI and HUD coordinates, and what 1440p actually requires. |
| [ui_scaling_progress.md](ui_scaling_progress.md) | What has landed and what is left. |

## Class-layout recovery

Recovering member offsets is the gate on everything else - a class cannot be ported without them.
These are the working notes from the several independent methods used, kept because they disagree
with each other in informative ways.

| | |
|---|---|
| [layout_sources.md](layout_sources.md) | Where layout information comes from, and what may be merged with what. **Read before the rest.** |
| [layouts_from_ctors.md](layouts_from_ctors.md) | Offsets recovered from constructor stores. |
| [layouts_from_datparser.md](layouts_from_datparser.md) | Offsets recovered from `datParser` field tables. |
| [inferred_layouts.md](inferred_layouts.md) | Offsets inferred from access patterns. |
| [short_layouts.md](short_layouts.md) | Class sizes from `operator new` call sites, and 28 layouts that disagreed. |
| [layout_blocker.md](layout_blocker.md) | The section-layout problem: why `.rdata`/`.data` are displaced. |
| [layout_hunt.md](layout_hunt.md) | Hunting pointers that are wrong in every build. |
| [findheap.md](findheap.md) | One `FindHeap failed` crash, traced to a false relocation. Worth reading as a case study. |

## Reference

| | |
|---|---|
| [command_line.md](command_line.md) | Every option the binary parses, extracted from the `datArgParser` call sites. |
| [ported_matrix34.md](ported_matrix34.md) | Notes from porting `Matrix34`, as a worked example. |
