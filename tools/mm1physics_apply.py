"""
Mount or unmount an alternate car-handling profile, with no change to the executable.

    py tools/mm1physics_apply.py on [mm1|drift]
    py tools/mm1physics_apply.py off
    py tools/mm1physics_apply.py status

TWO PROFILES. `mm1` carries MM1's weight, power, brakes, steering, suspension and grip LEVEL into
MM2's handling model. `drift` is the earlier, more literal translation, which ALSO transplanted
MM1's tyre slip window and angular damping and turned the whole roster into drift cars - kept
deliberately because it is fun, not because it is MM1. tools/mm1physics.py documents exactly which
parameters differ between the two and why. Only one profile is mounted at a time; turning one on
removes the other, because both claim the same entry names and the alphabetically first archive
would otherwise silently win.

DELIVERY IS AN ARCHIVE, AND THE NAME IS LOAD-BEARING. Two earlier mechanisms failed, and both
failures looked identical from the driver's seat - the car simply handled like stock - so they are
written down here rather than rediscovered.

  1. Loose files in `tune/vehicle/`. INERT. The stock engine reads its archives and never consults
     that folder. Proven, not assumed: a deliberately absurd tune - 60 kg and 3000 hp - installed
     there drove exactly like vanilla.

  2. Loose files in `mods/tune/vehicle/`. ALSO INERT HERE, though for a different reason. The mods
     folder is an MM2Hook feature (`UseModsFolder=1`), and MM2Hook version-checks its host and
     refuses to attach to this one - mm2hook.log says "Unknown module detected! Terminating..."
     because OpenMM2.exe is not a binary it recognises. The folder works for retail midtown2.exe
     and does nothing at all for us.

What does work is the engine's own archive layer. `zipFile::Init` links each archive in with

        this->PrevFile = zipFile::sm_First;   zipFile::sm_First = this;

- a push onto the HEAD of the list - and `zipFile::zipOpen` walks from `sm_First` and returns the
first entry that matches. So the archive mounted LAST is searched FIRST. The game mounts archives in
DESCENDING alphabetical order, which inverts that into a simple rule:

        THE ALPHABETICALLY FIRST ARCHIVE NAME WINS.

Hence the leading '!'. Named `zmm1physics.ar` this same archive mounted first, was therefore
searched last, and lost every lookup to mm2core.ar - valid, loaded, and completely ineffective.
CopLiverySwapper.ar, an existing mod that does work, is named the way it is for this reason.

WHY NOT THE GetDirName HOOK. The tidier design overrides `vehCarSim::GetDirName` (0x4CBAF0), a
six-byte virtual returning "tune/vehicle", to name a different folder. Porting it crashes the game:
`asm.py` hands a class's vftable to C++ only when that class's CONSTRUCTOR OR DESTRUCTOR is ported,
and only GetDirName was, so the vtable slot still pointed at the address asm.py had filled with
0xCC. That route needs the ctor/dtor ported first; the archive needs no code at all.

ONLY FILES THAT ACTUALLY DIFFER ARE PACKED. Eight of the 48 cars have no MM1 counterpart and their
generated file is a byte copy of the vanilla one; packing those would add nothing and would widen
the override for no reason. They are skipped, so the archive contains exactly the cars whose
handling really changes.
"""
import os
import shutil
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import arpack

GAME = r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2"
VANILLA = r"C:\Users\xshxd\AppData\Local\Temp\claude\C--Users-xshxd-OneDrive-Desktop-Dev-Workspace\bfa02fd7-bd74-4a5f-9ae6-2123261dc7e0\tmp\mm2tune\tune\vehicle"

PROFILES = {
    "mm1":   ("vehicle_mm1",   "!mm1physics.ar"),
    "drift": ("vehicle_drift", "!driftphysics.ar"),
}


def paths(profile):
    folder, archive = PROFILES[profile]
    return os.path.join(GAME, "tune", folder), os.path.join(GAME, archive)


def changed_files(profile):
    """The generated files that genuinely differ from vanilla."""
    src, _ = paths(profile)
    out = []

    for fn in sorted(os.listdir(src)):
        if not fn.lower().endswith(".vehcarsim"):
            continue

        van = os.path.join(VANILLA, fn)

        if os.path.exists(van):
            with open(van, "rb") as a, open(os.path.join(src, fn), "rb") as b:
                if a.read() == b.read():
                    continue        # no counterpart - identical, nothing to override

        out.append(fn)

    return out


def turn_on(profile):
    turn_off(quiet=True)            # only one profile may be mounted at a time

    src, archive = paths(profile)
    files = changed_files(profile)
    stage = tempfile.mkdtemp(prefix="mm1phys_")
    veh = os.path.join(stage, "tune", "vehicle")
    os.makedirs(veh)

    try:
        for fn in files:
            shutil.copy2(os.path.join(src, fn), os.path.join(veh, fn))

        arpack.build(arpack.collect(stage), archive)
    finally:
        shutil.rmtree(stage, ignore_errors=True)

    print("%s physics ON - %d car(s) packed into %s"
          % (profile.upper(), len(files), os.path.basename(archive)))
    print("  mounts last, so it is searched first; mm2core.ar is untouched")


def turn_off(quiet=False):
    gone = []

    for profile in PROFILES:
        _, archive = paths(profile)

        if os.path.exists(archive):
            os.remove(archive)
            gone.append(os.path.basename(archive))

    if quiet:
        return

    if gone:
        print("physics OFF - removed %s; every car is back on the mm2core.ar copy" % ", ".join(gone))
    else:
        print("physics was already OFF")


def status():
    active = [p for p in sorted(PROFILES) if os.path.exists(paths(p)[1])]

    if active:
        for p in active:
            print("%s physics is ON  (%s, %d bytes)"
                  % (p.upper(), os.path.basename(paths(p)[1]), os.path.getsize(paths(p)[1])))
    else:
        print("physics is OFF (vanilla MM2 - no override archive present)")

    for p in sorted(PROFILES):
        print("  %-6s %d file(s) differ from vanilla" % (p, len(changed_files(p))))


if __name__ == "__main__":
    cmd = (sys.argv[1] if len(sys.argv) > 1 else "status").lower()
    prof = (sys.argv[2] if len(sys.argv) > 2 else "mm1").lower()

    if cmd == "on":
        if prof not in PROFILES:
            raise SystemExit("unknown profile %r - choose from %s"
                             % (prof, ", ".join(sorted(PROFILES))))
        turn_on(prof)
    elif cmd == "off":
        turn_off()
    else:
        status()
