# -*- coding: utf-8 -*-
"""Recover the qualified name straight from an MSVC mangled symbol.

Reading the class and method name out of the DEMANGLED text is unreliable, because the readable
form puts the type first and the type can contain anything:

    char * * lvlProgress::ProgressNames      <- "class" looks like `char * * lvlProgress`
    char (* Stream::Callback)(int)           <- the name is INSIDE the parentheses

The mangled form has no such ambiguity: `?Name@Scope@@...`, scope innermost-first, terminated by
an empty component. Parse that instead and the readable text is only ever used for types.
"""
import re

# ??0 ctor, ??1 dtor, ??_7 vftable, ??_8 vbtable, ??2/??3 operator new/delete, ??_9 vcall thunk.
SPECIAL = re.compile(r"^\?\?(_[0-9A-Z]|[0-9A-Z])")

# Templates (`?$Name@...`) nest `@` inside the name itself, so the flat split does not apply.
TEMPLATE = "?$"


def qualified(mangled):
    """(class, name, is_template) or None when the symbol is not `?`-mangled at all.

    `class` is None for a free function or global.
    """
    if not mangled.startswith("?"):
        return None  # a C symbol: _main, __setargv, nullsub_267

    body = mangled
    special = None

    m = SPECIAL.match(mangled)
    if m:
        special = m.group(1)
        body = mangled[m.end():]
    else:
        body = mangled[1:]

    # A template can appear in the TYPE, which lives after the `@@` terminator and does not
    # concern us. Only a template in the name or scope run defeats the flat split.
    head = body.split("@@", 1)[0]

    if TEMPLATE in head:
        return None  # caller falls back to the demangled text

    parts = body.split("@")

    # Components run until the empty one produced by the `@@` terminator.
    scope = []
    name = None

    for i, p in enumerate(parts):
        if p == "":
            break
        if i == 0 and special is None:
            name = p
        else:
            scope.append(p)

    cls = "::".join(reversed(scope)) if scope else None

    if special is not None:
        # For a special name the whole scope run is the class; the name comes from what it is.
        base = scope[-1] if scope else None
        cls = "::".join(reversed(scope)) if scope else None

        if special == "0":
            name = base
        elif special == "1":
            name = "~" + base if base else None
        elif special == "_7":
            name = "`vftable'"
        elif special == "_8":
            name = "`vbtable'"
        else:
            name = None  # operators and thunks: let the demangled text name them

    return cls, name, False
