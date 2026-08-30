# Porting a class that has a vtable

`tools/asm.py` hands a *function* over to C++. This is the other half: handing a *vtable* over.
Without it, 335 of the 536 code-bearing classes — **58.5% of the bytes in named code symbols** —
cannot be started at all, because the first constructor you write makes the compiler emit a symbol
the assembly already defines.

Two separate things have to be right, and only one of them fails loudly.

| | failure | how it shows up |
|---|---|---|
| the symbol | `??_7Class@@6B@` defined twice | `LNK2005`, at link. Loud. |
| the slot order | the C++ table's slots are in a different order to the binary's | nothing. The game runs and misbehaves. |

`tools/asm_vtables.py` handles the first and *checks* the second.

---

## 1. The mechanism: nothing is removed

`asm.py` can pad a stripped `PROC` with `0xCC` because a function is a self-contained run of bytes
with nothing else living inside it. A vftable is not, and the difference is not academic. Every
reference to a vftable symbol in `game.asm` takes one of four forms, and there are no others:

```
   340  PUBLIC ??_7Class@@6B@
   340  ??_7Class@@6B@ LABEL BYTE
   693  dd offset ??_7Class@@6B@
 5,818  dd offset ??_7Class@@6B@ + N
```

**All 5,818 of the `+ N` references have N past the end of the table** — up to `+33,996`. They are
not slots. They are Ghidra naming an unsymbolised `.rdata` constant by the nearest preceding
symbol, and the nearest preceding symbol is very often a vftable label, because vftables are the
densest named thing in `.rdata`. Blank a vftable's bytes and you do not lose a 52-byte table, you
lose up to 34 KB of anchored float constants and string tables, silently. `.rdata` has no
relocation table, so there is nothing to catch it.

So nothing is removed and nothing is padded:

```asm
PUBLIC ??_7asNode@@6B@                  ->  ; PUBLIC ??_7asNode@@6B@
??_7asNode@@6B@ LABEL BYTE              ->  __vft_orig_005B28E8 LABEL BYTE
    dd offset ??_7asNode@@6B@ + 52      ->      dd offset __vft_orig_005B28E8 + 52
    dd offset ??_7asNode@@6B@           ->      (untouched)
                                        +   EXTERN ??_7asNode@@6B@:BYTE
```

The old table's bytes stay exactly where they were, under a private label, as dead data. The `+ N`
anchors keep resolving to the same addresses. The plain references — the vptr stores in
constructors that are still assembly, and any type-identity compare — become external, and the
linker points them at the C++ table.

**The line count does not change and the emitted byte count does not change.** Both are asserted
on every run, not assumed. Nothing in `.rdata` moves by one byte, which is the only reason this is
safe on a binary with `IMAGE_FILE_RELOCS_STRIPPED` set, and it means a vtable handover cannot make
the `.data` shift in `docs/harness.md` any worse.

### Verified against the real assembly

Against `game.asm` as `ExportAsm.java` emits it:

| | |
|---|--:|
| vftable symbols in `midtown2.map` | 340 |
| tables located in the assembly, in the canonical `PUBLIC` + `LABEL BYTE` + `dd offset` shape | **340** |
| slot counts agreeing with a direct read of `midtown2.exe` | **340 / 340** |
| references *into* a table body (which this transformation could not express, and would refuse) | **0** |
| emitted bytes, before → after a four-table handover | 2,842,552 → 2,842,552 |

The map's own `size` for a vftable symbol disagrees with the parsed slot count on 141 of the 340
tables, and the map is wrong every time: a data symbol's size is the distance to the *next* symbol,
so it swallows whatever unnamed `.rdata` follows the table. Counting the `dd offset <code symbol>`
run is what agrees with the executable.

---

## 2. The integration contract

One function. `tools/asm_vtables.py`:

```python
Handover = namedtuple("Handover", "lines externs strip_procs report ok plan")

def hand_over(lines, marks, classes=None) -> Handover
```

| | |
|---|---|
| `lines` | the assembly, split on newlines — exactly what `asm.py`'s `read_procs()` returns |
| `marks` | `{mangled: 'IMPORT'\|'EXPORT'}` — exactly what `asm.py`'s `scan_headers()` returns |
| `classes` | optional override, for driving it by hand |
| → `.lines` | the rewritten assembly. **Same line count, same emitted byte count.** |
| → `.externs` | vftable symbols to declare `EXTERN <name>:BYTE` |
| → `.strip_procs` | extra mangled `PROC` names `asm.py` must **also** strip |
| → `.report` | what happened, for printing |
| → `.ok` | `False` if anything was refused — stop, do not write |

Which classes move is **derived, not declared**. MSVC emits `??_7C@@6B@` in a translation unit that
defines a constructor or a destructor of `C`, because those are the functions that store the vptr —
and in no other. So "the C++ owns this vtable" is exactly "a ctor or dtor of `C` is `ARTS_EXPORT`".
There is no second list to keep in sync with `data/ported.json`.

That also means **defining an ordinary virtual does not emit the vtable**. Virtuals can be ported
freely on a class whose constructor is still assembly, which is a much larger body of work than
`docs/port_order.md`'s Trap 1 currently allows for.

### Where it goes in `asm.py`

```python
    marks = scan_headers(CODE)
    lines, procs = read_procs(ASM)

    h = asm_vtables.hand_over(lines, marks)        # <-- here, and only here
    print(h.report)
    if not h.ok:
        return 1
    lines = h.lines

    strip = [m for m in exports if m in procs]
    strip += [m for m in h.strip_procs if m in procs and m not in strip]

    ...                                           # asm.py's padding loop, unchanged

    with open(ASM, "w") as f: ...                 # asm.py writes the file
    asm_vtables.add_externs(h.externs)            # EXTERN <vftable>:BYTE
```

**The ordering is forced, not preferred.** `asm.py`'s `procs` map holds line indices taken from
`lines`, and its padding loop replaces an n-line `PROC` with a 2-line comment — so the moment it
runs, every index after the first stripped function is stale. `hand_over()` is index-preserving by
construction, so it is the only one of the two that can go first. Running it second would mean
rescanning a half-rewritten file, which is where a tool like this starts guessing.

`add_externs()` is called after the write because it appends to `game_externs.inc`, a different
file, and is idempotent.

### `strip_procs` — the part that is easy to miss

MSVC regenerates two kinds of symbol from the class definition alone, under names the assembly
already owns, and neither is declared in any header — so `asm.py` scanning headers will never find
them:

- **`??_G<C>`**, the scalar deleting destructor. A virtual destructor does not put `~C` in the
  vtable; it puts a compiler-generated *deleting* destructor there, which calls `~C` and then
  `operator delete`.
- **`[thunk]: ... adjustor{N}` stubs**, for the five multiple-inheritance classes. The
  `sub ecx, 180; jmp Real` stub that fixes `this` when a call arrives through a secondary vtable is
  regenerated from the class definition, under the same mangled name.

Leave one behind and you get a duplicate symbol at link. That at least is loud.

The thunks are detected from the *demangled* text, not the mangled name: the marker `@@W` also
appears mid-signature whenever a parameter is an enum (`asCamera::GetNearClip` takes a
`W4kNearClip@1@`), and matching the mangled form strips six functions that are not thunks at all.

`??_E<C>`, the *vector* deleting destructor, is reported as an orphan rather than stripped. MSVC
emits `??_G` unless `delete[]` is used on the type, so `??_E` will not collide; it stays in the
assembly, referenced only by the dead table.

---

## 3. Slot order — the half that fails silently

A C++ compiler assigns vtable slots in **declaration order**: `[base's slots][this class's new
virtuals, in the order they are written]`. An override reuses the base's slot and may be declared
anywhere.

`tools/genheaders.py` sorts methods alphabetically after the constructor and destructor. That is
Open1560 house style and it is right for everything that is not virtual. For a polymorphic class it
produces a table that compiles, links, and dispatches every original call to the wrong method of
the right signature.

`data/vtable_order.json` holds the true order for all 335 classes, read out of `.rdata`.
`docs/vtable_order.md` is how it was recovered and what was checked. In summary:

- **prefix property**: 277 of 279 base/derived pairs hold **slot by slot** — not the length-only
  check, which passes 279/279 and proves nothing. Both exceptions are understood and neither is an
  ordering error.
- **MM2Hook cross-check**: 2,147 of 2,158 slots agree on order (99.5%) against MM2Hook's
  independently maintained `_vtbl` structs. Of the 11 that do not, the binary is right in 10.

### Most classes do not have this problem

| | |
|---|--:|
| classes that add **no** new virtuals — they only override, so order is irrelevant | **248** |
| classes that add exactly one | 26 |
| classes that add two or more — **these are the order-sensitive ones** | **61** |

Overriding is order-free because an override lands in the slot the base already assigned. So
three quarters of the polymorphic classes can be ported with the headers exactly as generated. Do
not take that on trust for any individual class — run the check.

### The check

```sh
py tools/asm_vtables.py --manifest asNode   # the order the binary requires
py tools/asm_vtables.py --check asNode      # would this header produce it?
py tools/asm_vtables.py --check-all         # every class
```

`--check` reads the header back, pulls out the mangled symbol of every declaration on a line
containing `virtual`, in file order, and compares. There are exactly three ways to fail and it
tests all three:

1. a new virtual **missing** from the header — the table is too short and every slot after it shifts
2. a virtual in the header that **has no slot** — the table grows one the original code cannot see
3. the new virtuals **in the wrong order** — same length, wrong dispatch, no diagnostic anywhere

Today, `--check-all` reports **50 classes that would not produce the binary's table**:

| | |
|---|--:|
| declaration order wrong | 44 |
| ...of which also declare a virtual with no slot (`GetClassNameA` and friends) | 3 |
| no generated header at all — `AudManager`, `AudMidi`, `AudStream`, `BMLabel`, `phPhysicsManager`, `IDirectMusicGetLoader` | 6 |

The other 285 pass as generated. The six with no header are a separate gap: they carry a vftable
but `tools/genheaders.py` never emitted a class for them, so they cannot be ported at all until it
does.

`asNode` is the worked example:

```
asNode  (code/midtown2/arts7/asnode.h)
  DECLARATION ORDER IS WRONG. Required, in this order:
    [ 2] Update      [ 3] Reset       [ 4] ResChange   [ 5] UpdatePaused
    [ 6] FileIO      [ 7] AfterLoad   [ 8] BeforeSave  [ 9] Save
    [10] Load        [11] GetClassName            [12] GetDirName
  header has:
    AfterLoad, BeforeSave, FileIO, GetClassName, GetDirName, Load,
    ResChange, Reset, Save, Update, UpdatePaused
  1 virtual(s) declared that occupy NO slot in the binary's table:
    ?GetClassNameA@asNode@@UAEPADXZ
```

`GetClassNameA` is not a second method. `<windows.h>` `#define`s `GetClassName` to `GetClassNameA`,
so the 1999 headers declared one virtual and the compiler mangled it under whichever spelling that
translation unit happened to see. The two functions are byte-identical. **Declare one.** Declaring
both adds a fourteenth slot to a thirteen-slot class.

### Three classes not to take at face value

Established in `docs/vtable_order.md`, repeated here because this is where a porter meets them.
`--manifest` and `--check` print the relevant one automatically.

- **`dgUnhitBangerInstance`** — `??_7dgUnhitBangerInstance@@6B@` is a **mislabelled symbol**. The
  28-slot table under that name is `dgBangerInstance`'s. Every one of the nine classes deriving
  from `dgUnhitBangerInstance` holds `?Reset@dgUnhitBangerInstance@@` at slot 0 and
  `?Init@dgUnhitBangerInstance@@` at slot 17; this table holds `lvlInstance`'s. Take the order from
  `dgUnhitMtxBangerInstance`'s 30-slot table.
- **`vehSuspension`** — slot 6 is named `?Copy@vehSuspension@@QAEXABV1@@Z`, which is not virtual and
  cannot be in a vtable. There is no `?FileIO@vehSuspension@@` symbol at all: the two compiled to
  identical bytes and the linker folded them. The slot is `asNode::FileIO` overridden.
- **`lvlInstance`** — `?DrawPhysics@lvlInstance@@UAEXABVVector3@@@Z` is a public virtual with 576
  bytes of real code that occupies **no slot in any of the 340 tables**. Declaring it `virtual`
  would add a 27th slot to `lvlInstance` and shift every one of its descendants. Declare it
  non-virtual, or leave it out.

---

## 4. Partial porting, and the rule for when a class may be started

A vtable is a list of external symbol references and the linker does not care where they come from.
A virtual that is still `ARTS_IMPORT` is declared in the header and defined nowhere in C++, so MSVC
emits an undefined external for it and the linker resolves it against the `PROC` still sitting in
`game.asm` under the same mangled name.

**So a class can have its constructor, destructor and vtable in C++ while every one of its virtuals
is still 1999 machine code.** That is what makes a 30-slot class portable at all — you do not have
to write thirty functions before you can write one.

The rule this buys, and the reason it is a rule rather than a preference:

> A polymorphic class may be started as soon as its **complete list of virtuals** is known and its
> header passes `--check`. It does **not** need any of them implemented. What it cannot survive is a
> header that declares the wrong *set* or the wrong *order*, because the vtable is emitted from the
> declaration, not from what is defined.

Concretely, before flipping the first `ARTS_EXPORT` on a polymorphic class:

1. `py tools/asm_vtables.py --manifest <Class>` — read the required order.
2. Reorder the virtuals in the header to match. Leave the non-virtuals alphabetical; house style is
   unaffected, because it was only ever right about non-virtuals.
3. `py tools/asm_vtables.py --check <Class>` — must pass.
4. Now port whatever you like, in any order, one function at a time as usual.

---

## 5. Residual risk

What can still go wrong after this lands, in descending order of how quietly it fails.

**1. Identical COMDAT folding can put a wrong name on a right address.** The linker merged
functions that compiled to identical bytes and `midtown2.map` kept one name for the survivor. Where
the base has the same slot, the prefix check catches it — that is exactly how `vehSuspension` slot 6
was found. Where the slot is *new*, nothing can: the address is right, the name may not be. There
is one known instance and it is unlikely to be the only one. Mitigation: for a `new` slot, read
`MM2_RE_KIT/MM2_PSEUDOCODE/` before trusting the name. Suspect it when a slot's name is a method
that also appears elsewhere in the same table, or is a `Q`-mangled (non-virtual) symbol.

**2. A virtual declared in the header that the binary does not have.** It appends a slot. The C++
table is then longer than every still-assembly derived table, and any original code that walks past
the original end reads a neighbouring vtable. `--check` catches this and 52 classes currently trip
it, `GetClassNameA` being the common cause. It is only silent if the check is skipped.

**3. Partial porting is safe for virtuals but not for the vtable itself.** The mechanism handles a
class whose virtuals are a mix of C++ and assembly — that is its main design point. What it does not
handle is a class whose *constructor* is C++ while a **derived** class's constructor is still
assembly: the derived constructor stores the derived vptr, which is still the assembly table, whose
slots still point at the base's original functions. That is correct — but it means base and derived
run different implementations of the same virtual until the derived class moves too. Behavioural,
not structural; worth knowing when a fix "does not take" on one subclass.

**4. `??_E` vs `??_G`.** The binary's slot 0 often holds `??_E<C>`, the *vector* deleting
destructor. MSVC will emit `??_G<C>`, the scalar one, unless `delete[]` is used on the type. They
are identical for `delete` and differ for `delete[]`. `plan()` warns per class; confirm nothing
array-deletes the type. The `??_E` symbol stays in the assembly as an orphan.

**5. `EXTERN <vftable>:BYTE` — now exercised, and it works.** This used to read "not yet exercised
in this build". `mmCityInfo` was the first real handover: one `:BYTE` extern through `ml.exe`,
`.rdata` byte-identical to retail afterwards, and gates 11 and 12 both reporting
`differing ANYWHERE ELSE : 0`.

That handover also corrected a wrong assumption in `asm_vtables.py` worth repeating here, because
it will bite again on the next class. **The deleting destructor MSVC emits is not the one the 1999
compiler emitted.** 1999 MSVC chose per class — `??_G` (scalar) for `mmCityInfo`, `??_E` (vector)
for `mmCityList`. MSVC 14.51 emits `??_E` for both. The tool had it the other way round, so it
stripped `??_G` as a "companion the compiler regenerates", nothing regenerated it, and the link
failed with a single unresolved external.

The rule is now: **strip `??_E`, leave `??_G`.** Leaving it is required rather than merely
harmless — the original `??_G` is called directly by assembly that deletes the object, and it keeps
working because `asm.py` rewrites its call to the destructor like any other call site. Re-check
after a toolchain upgrade with `dumpbin -SYMBOLS build/obj/<class>.obj`; that one line is the whole
test.

**6. A plain `dd offset ??_7C@@6B@` that is not a vptr.** All 693 plain references are left to bind
to the C++ table, which is right for a vptr initialiser and wrong for a data word that merely holds
that address as a constant. The two are indistinguishable in a binary with no relocation table. A
dword whose value is exactly a vftable address is a vptr in every case examined, but "in every case
examined" is the honest strength of that claim.

**7. Ten classes have slots from `dgBangerInstance`, which emits no vftable, so `hierarchy.json` has no
link to it.** `--audit` lists them under "slot order NOT derivable". Their own tables are complete
and correct; what is missing is the *split* between which virtuals belong in the base header and
which in the derived one. MM2Hook supplies `dgBangerInstance`'s length (28). Get the split wrong
and the total order per class is still right, so this is a structuring risk rather than a dispatch
one — but nine of the ten derive from `dgBangerInstance` independently of each other, so the split
has to be got right once and then holds for all of them.

**8. The one unnamed slot.** `mmMultiCircuit` slot 0 is the only code pointer in `.rdata` that
`midtown2.map` does not name. It is the deleting destructor. `--check` reports it as unverifiable
rather than guessing.
