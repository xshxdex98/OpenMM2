# Where layout information comes from, and what may be merged

Member layouts are the binding constraint on this port: a class cannot be reimplemented until its
field offsets are known, because the not-yet-ported assembly reads them at fixed byte offsets.
Several independent recoveries are in flight, and they are NOT interchangeable.

## The sources

| File | What it is | Merge into `layouts.json`? |
|---|---|---|
| `layouts.json` | IDA type library — full member lists, 188 classes | it **is** the merged truth |
| `mm2types.json` | MM2Hook `sizeof` for 264 classes, sparse `hook::Field` offsets | size only, never members |
| `layouts_from_datparser.json` | `datParser::AddValue` registrations — real 1999 names with offsets | **yes, when CONFIRMED** |
| `layouts_from_serialization.json` | `Load`/`Save`/`memset`/format-string walking | **no** — see below |
| `layouts_from_ctors.json` | constructor writes | only where CONFIRMED and reconciled |
| `inferred_layouts.json` | all member accesses across a class's methods | **no** — lower bounds only |

## Why the sparse sources must not be merged

`check_size` and the emitted member list are driven by `layouts.json`. A class merged there with a
partial member list gets a `sizeof` that is *smaller than the real object*, and that is worse than
having no entry at all: the assembly then writes past the end of everything we allocate, and
nothing reports it.

`layouts_from_serialization.json` illustrates the shape of the problem honestly. `Aud3DObjectManager`
comes back with exactly one member — `field_3C`, width 100 — because that is the extent of a
`memset`. It is a true fact about the class and it is not a layout. Its `size` is correctly `null`,
with the number kept separately as `size_lower_bound`; a lower bound is not a size and the file is
careful never to present it as one.

The same reasoning kept `check_size` off 179 classes where MM2Hook supplies only a size: with no
members emitted, `sizeof` is 1 on both sides and the assertion passes while certifying nothing.

## What the sparse sources ARE good for

**Field names.** 280 real 1999 names have been recovered so far. Where a class already has a
layout from the IDA type library, replacing `field_2C` with the name the original programmers used
is a pure improvement and carries no layout risk — the offsets are unchanged.

**Corroboration.** Two independent recoveries agreeing on an offset is much stronger evidence than
either alone, and disagreement is a signal that one method is unsound for that class.

## datParser is MM2's reflection system

Worth stating plainly because it was got wrong once here: MM2 appears to have dropped MM1's
`DeclareFields`/`MetaClass` reflection, and searching for those names returns zero symbols. It did
not. The same mechanism exists as `datParser`:

```
datParser::AddValue(const char* name, float* ptr, datCallback*)
datParser::AddValue(const char* name, int* ptr, datCallback*)
datParser::AddRecord(int, const char*, void*, unsigned short, datCallback*)
```

Each call site pairs a field NAME with a POINTER to that field. Where the pointer is `this + N`,
the original code is stating the name, the offset and — through the overload chosen — the type.
That is the strongest layout evidence available in the binary, and stronger than any inference.
82 classes have `Load`/`Save`-style methods that would call it.
