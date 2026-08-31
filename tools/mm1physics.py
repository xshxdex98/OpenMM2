"""
Build MM1-derived car physics for MM2, as a drop-in replacement tune directory.

WHY A DIRECTORY AND NOT A PATCH. `vehCarSim::GetDirName` (0x4CBAF0) is a six-byte virtual that
returns the string "tune/vehicle" - the folder every car's physics is loaded from. Overriding that
one function to return "tune/vehicle_mm1" swaps the handling of the entire roster at once, and
returning the original string swaps it straight back. Nothing is patched, nothing is destroyed, and
"unchecked reverts to vanilla MM2" is true by construction rather than by careful undoing.

WHAT "MM1 PHYSICS" CAN AND CANNOT MEAN HERE, STATED HONESTLY.

The two games do not share a physics engine. MM1's `mmCarSim` and MM2's `vehCarSim` are different
simulations with different parameter sets, and the transmission models in particular have no
translation between them at all:

    MM1   GearRatios[8], UpshiftRPM[8], DownshiftRPM[8]      - explicit ratios and shift points
    MM2   Reverse, Low, High, GearBias, UpshiftBias, ...     - speed bands and biases

So a bit-exact MM1 simulation is not achievable without porting MM1's entire physics engine into
MM2. What IS achievable, and what this does, is to carry across every parameter that genuinely
means the same thing in both simulations - which is most of what a car FEELS like: its weight and
how that weight is distributed, how much power it makes and where in the rev range, how much grip
the tyres have, how the suspension is sprung and damped, and how it behaves aerodynamically.

The transmission block is deliberately left at MM2's values. MM1's numbers there describe a
different mechanism, and writing them into MM2's fields would not approximate MM1 - it would
produce a gearbox that is neither game's, which is how a car ends up undriveable.

TWO MORE BLOCKS ARE WITHHELD, AND THEY WERE LEARNED THE HARD WAY. An earlier version translated
both, and the result was a roster that drove like DRIFT CARS - loose, rotation-happy, sliding out of
everything. Kept deliberately as the "Drift Physics" profile, but it is not MM1 handling:

  * `OptimumSlipPercent` (0.14 in MM2, 0.03 in MM1). Both engines share ONE tyre curve -
    `force = StaticFric * mult * slip * (2*Opt - slip) / Opt^2` - which peaks at `slip == Opt` with
    magnitude `StaticFric`. So Opt is not a strength, it is the WIDTH of the grip window, and peak
    grip and window width are independent. Narrowing it to 0.03 inside MM2's solver makes the tyre
    razor-peaky: grip collapses the moment slip passes 3%, and the car snaps loose and stays loose.
    MM2's window is kept; MM1's StaticFric/SlidingFric still come across, so the car keeps MM1's
    GRIP LEVEL with a breakaway MM2's solver can actually integrate.

  * `Aero.AngCDamp` / `AngVelDamp` / `AngVel2Damp`. These do not line up: MM2 carries its yaw
    damping in AngCDamp, MM1 carries its damping in AngVelDamp/AngVel2Damp. Copying MM1's block
    across writes 0/0/0 into AngCDamp and ZEROES MM2's yaw damping, so nothing resists rotation.

Peaky tyres plus no yaw damping is a drift car almost by definition. Both blocks now keep MM2's
values, which is the same rule already applied to the transmission: a parameter that merely shares a
NAME between the two engines is not thereby translatable.

SOURCES, AND WHY THERE ARE TWO.

  * MM1's own roster comes from its `core.ar` - `TUNE/<CAR>.MMCARSIM`. That covers the twelve cars
    the two games share.
  * MM2-exclusive cars have no MM1 tune, so they come from `vamm2rv6mm2cars.ar`, a mod that
    converts the MM2 cars TO MM1. Its tunes are MM1-format handling authored for those exact cars,
    which is precisely the missing half.

Three MM2 cars (vpcentury, vpeagle, vpmoonrover) exist in neither source and are copied through
unchanged, so they keep vanilla MM2 handling whether the option is on or off. That is reported
rather than hidden.
"""
import os
import re
import sys

# ── where things live ─────────────────────────────────────────────────────────
MM2_TUNE = sys.argv[1] if len(sys.argv) > 1 else r"C:\Users\xshxd\AppData\Local\Temp\claude\C--Users-xshxd-OneDrive-Desktop-Dev-Workspace\bfa02fd7-bd74-4a5f-9ae6-2123261dc7e0\tmp\mm2tune\tune\vehicle"
MM1_TUNE = sys.argv[2] if len(sys.argv) > 2 else r"C:\Users\xshxd\.claude\jobs\bfa02fd7\tmp\mm1core\TUNE"
CONV_TUNE = sys.argv[3] if len(sys.argv) > 3 else r"C:\Users\xshxd\.claude\jobs\bfa02fd7\tmp\convmod\TUNE"
OUT_DIR = sys.argv[4] if len(sys.argv) > 4 else r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\tune\vehicle_mm1"


def read_text(path):
    """Decode for PARSING only, as a byte-for-byte round trip.

    An earlier version substituted a space for every byte outside printable ASCII. That silently
    flattened two things that matter: the CR of every CRLF, and the TABS that separate the
    components of a vector (`InertiaBox 3.500000\t2.000000\t5.000000`). The output has to be
    byte-identical to vanilla except for the numbers actually being changed, or a "translated" file
    differs from the original in ways nobody asked for and a pass-through copy is not a copy.
    """
    return open(path, "rb").read().decode("latin-1")


def parse(text):
    """Parse the shared `KEY { key value... sub { ... } }` tune format into nested dicts.

    Both games use it; MM1 decorates its class line with an address (`mmCarSim :0891a1b0 {`) which
    is ignored. Values stay as STRINGS - they are written back out verbatim where untouched, so a
    float never picks up a rounding difference just by passing through.
    """
    root = {}
    stack = [root]

    for line in text.split("\n"):
        line = line.strip()

        if not line or line.startswith("type:"):
            continue

        if line == "}":
            if len(stack) > 1:
                stack.pop()
            continue

        if line.endswith("{"):
            name = line[:-1].strip().split()[0]
            node = {}
            stack[-1][name] = node
            stack.append(node)
            continue

        parts = line.split()

        if len(parts) >= 2:
            stack[-1][parts[0]] = parts[1:]

    return root


def top(d):
    """The single class node at the root (mmCarSim / vehCarSim)."""
    for k, v in d.items():
        if isinstance(v, dict):
            return v
    return {}


# ── the translation table ─────────────────────────────────────────────────────
#
# Left is the path in MM2's file, right is the path in MM1's. Only pairs that mean the same thing
# in both simulations appear here; anything absent keeps MM2's value.
SCALARS = [
    (("Mass",),                     ("Mass",)),
    (("InertiaBox",),               ("InertiaBox",)),
    (("CenterOfGravity",),          ("BodyCG",)),
    (("BoundFriction",),            ("BoundFriction",)),
    (("BoundElasticity",),          ("BoundElasticity",)),
    (("DrivetrainType",),           ("DrivetrainType",)),
    (("CarFrictionHandling",),      ("CarFrictionHandling",)),
    (("Aero", "Drag"),              ("Drag",)),
    (("Aero", "Down"),              ("Downforce",)),
    (("Engine", "MaxHorsePower"),   ("Engine", "MaxHorsePower")),
    (("Engine", "OptRPM"),          ("Engine", "OptRPM")),
    (("Engine", "MaxRPM"),          ("Engine", "MaxRPM")),
    (("Engine", "GCL"),             ("Engine", "GCL")),
]

# Per-wheel. MM1 names its wheels FrontLeft/BackLeft; MM2 has one block per axle.
WHEELS = [("WheelFront", "FrontLeft"), ("WheelBack", "BackLeft")]
WHEEL_SCALARS = [
    ("SuspensionLimit",    "SuspensionLimit"),
    ("BrakeCoef",          "BrakeRatio"),
    ("StaticFric",         "StaticFric"),
    ("SlidingFric",        "SlidingFric"),
    ("SteeringLimit",      "SteeringRatio"),
]


def get(node, path):
    cur = node
    for k in path:
        if not isinstance(cur, dict) or k not in cur:
            return None
        cur = cur[k]
    return cur if not isinstance(cur, dict) else None


def substitute(text, path, values):
    """Replace one key's VALUES only, leaving indentation, separators and the line ending intact.

    The value run stops before any CR: matching `[^\n]*` would swallow the CR of a CRLF and quietly
    rewrite the file's line endings. Indentation and the gap after the key are captured and put
    back rather than normalised, so the only bytes that move are the numbers.
    """
    key = path[-1]
    depth_hint = r"\n([ \t]*)" + re.escape(key) + r"([ \t]+)([^\r\n]*)"

    def repl(m):
        # A vector's components are TAB-separated in MM2's files (`InertiaBox 2.5\t1.1\t3.0`).
        # Reuse whichever separator the line already had so the only bytes that change are digits.
        sep = "\t" if "\t" in m.group(3) else " "
        return "\n%s%s%s%s " % (m.group(1), key, m.group(2), sep.join(values))

    # only substitute inside the right block: find the block, then the key within it
    if len(path) > 1:
        block = path[-2]
        bm = re.search(r"\n(\s*)" + re.escape(block) + r"\s*\{", text)

        if not bm:
            return text, False

        start = bm.end()
        depth = 1
        i = start

        while i < len(text) and depth:
            if text[i] == "{":
                depth += 1
            elif text[i] == "}":
                depth -= 1
            i += 1

        seg = text[start:i]
        new_seg, n = re.subn(depth_hint, repl, seg, count=1)
        return text[:start] + new_seg + text[i:], bool(n)

    # top level: must not match a key of the same name inside a sub-block, so stop at the first
    # nested '{'
    first_block = text.find("{", text.find("{") + 1)
    head = text if first_block < 0 else text[:first_block]
    tail = "" if first_block < 0 else text[first_block:]
    new_head, n = re.subn(depth_hint, repl, head, count=1)
    return new_head + tail, bool(n)


def mm1_source(name):
    """Find the MM1-format tune for an MM2 car, from MM1 itself or from the conversion mod."""
    for d in (MM1_TUNE, CONV_TUNE):
        for cand in (name.upper() + ".MMCARSIM", name + ".MMCARSIM"):
            p = os.path.join(d, cand)
            if os.path.exists(p):
                return p, ("MM1" if d == MM1_TUNE else "conv")

    # MM2's fire truck is the conversion mod's LaFrance
    if name.lower().startswith("vpftruck"):
        alt = name.upper().replace("VPFTRUCK", "VPLAFRANCE")
        p = os.path.join(CONV_TUNE, alt + ".MMCARSIM")
        if os.path.exists(p):
            return p, "conv"

    return None, None


def main():
    os.makedirs(OUT_DIR, exist_ok=True)
    done = skipped = 0
    report = []

    for fn in sorted(os.listdir(MM2_TUNE)):
        if not fn.lower().endswith(".vehcarsim"):
            continue

        name = fn[:-len(".vehcarsim")]
        mm2_text = read_text(os.path.join(MM2_TUNE, fn))
        src, origin = mm1_source(name)

        if not src:
            # written as BYTES, so a pass-through copy really is a copy
            open(os.path.join(OUT_DIR, fn), "wb").write(mm2_text.encode("latin-1"))
            report.append((name, "-", 0, "no MM1 counterpart - vanilla MM2 physics kept"))
            skipped += 1
            continue

        mm1 = top(parse(read_text(src)))
        out = mm2_text
        applied = 0

        for dst, srcpath in SCALARS:
            v = get(mm1, srcpath)
            if v:
                out, ok = substitute(out, dst, v)
                applied += ok

        for mm2_block, mm1_block in WHEELS:
            wheel = mm1.get(mm1_block)
            if not isinstance(wheel, dict):
                continue
            for dkey, skey in WHEEL_SCALARS:
                v = wheel.get(skey)
                if v:
                    out, ok = substitute(out, (mm2_block, dkey), v)
                    applied += ok

        open(os.path.join(OUT_DIR, fn), "wb").write(out.encode("latin-1"))
        report.append((name, origin, applied, os.path.basename(src)))
        done += 1

    print("MM1 physics written to %s" % OUT_DIR)
    print("  translated %d car file(s), %d kept at MM2 defaults" % (done, skipped))
    print()
    print("  %-22s %-6s %-7s %s" % ("CAR", "SOURCE", "PARAMS", "FROM"))
    for name, origin, applied, note in report:
        print("  %-22s %-6s %-7s %s" % (name, origin, applied or "", note))


if __name__ == "__main__":
    main()
