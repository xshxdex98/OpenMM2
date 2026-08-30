# Free functions, and the CRT symbols that do not exist

`midtown2.exe` has about 1,368 free functions - roughly 224 KB - and they are the cheapest code in
the binary to reimplement. With no `this` there is no instance layout to recover first, so a free
function is portable the moment its body is legible. 33 are done, in
[`code/midtown2/misc/freefuncs.h`](../code/midtown2/misc/freefuncs.h).

They live in `misc/` because that is the subsystem `tools/symbols.py` assigns to an unprefixed
global. They are not otherwise related to one another.

## A free function needs a declaration, not just a registration

`tools/asm.py` works from `ARTS_EXPORT` markers **in headers**, and free functions have no
generated header. Registering the symbol in `data/ported_*.json` alone leaves the original `PROC`
in place, and the link then fails with "already defined in game.obj". Add the declaration to
`misc/freefuncs.h`.

## The trap: three CRT functions are not linkable

**`_memset`, `_strlen` and `_strcpy` do not exist as symbols anywhere in `midtown2.exe`.** The 1999
compiler inlined all three - as `rep stos`, `repne scas` and so on. So *any* function whose C++ form
lowers to one of them cannot be linked, however readable its body is. That is why several otherwise
trivial free functions are still `ARTS_IMPORT`.

The failure is not subtle at link time, but it is easy to walk into, because **the C++ need not
mention the function at all**. MSVC recognises idioms and lowers them:

```cpp
// Recognised as memset and lowered to a call to _memset. Does not link.
for (u32 i = 0; i < dwords; ++i)
    fill[i] = 0;
```

`mmTextNode::Init` hits exactly this, zero-filling its entry array where the original contains
`rep stosd`. The fix is to defeat the idiom, not to avoid the loop:

```cpp
volatile u32* fill = static_cast<volatile u32*>(entries);
for (u32 i = 0; i < dwords; ++i)
    fill[i] = 0;
```

A `volatile` store may not be elided or merged, so the pattern matcher cannot fire. **This will bite
every future port that touches a buffer.**

## The related trap: `<cmath>` drags in the modern CRT

The link runs `-NODEFAULTLIB` against the game's own statically linked 1999 CRT, so the only names
that resolve are the undecorated ones already in `game.asm`.

MSVC's `<cmath>` includes `<cstdlib>` and `<cstring>`, which declare the CRT entry points with
`_ACRTIMP`. Depending on the CRT model that becomes `__declspec(dllimport)`, and a call to `toupper`
is emitted as `__imp__toupper`, which resolves to nothing here.

This is why `misc/mathfuncs.cpp` and `misc/miscfuncs.cpp` are separate translation units even
though their contents are related: the integer and string functions must not include `<cmath>`.
Where a CRT function is genuinely needed, declare it by hand:

```cpp
// midtown2.exe's own toupper, at 0x5A8CF3, published in the map as `_toupper`.
extern "C" i32 __cdecl toupper(i32 c);
```

Note that `__toupper` at `0x5A8CEB` is a *different* function - the C `_toupper` macro helper - and
is not what `UpperCase` calls. Resolving these by address against the map, rather than by name, is
the only reliable method.

## See also

- [`port_order.md`](port_order.md) - the traps that apply to every port, and the candidate screen.
- [`ARCHITECTURE.md`](ARCHITECTURE.md) - why `-NODEFAULTLIB` is forced in the first place.
