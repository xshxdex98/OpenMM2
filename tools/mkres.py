# -*- coding: utf-8 -*-
"""Compile tools/openmm2.rc into build/openmm2.res for the linker.

Separate from the icon build (tools/mkicon.py) because they answer to different inputs: the icon
only changes if the retail binary does, while the version block changes whenever the project's own
metadata does. Running this every build is cheap; regenerating the icon every build is not.

rc.exe lives in the Windows SDK rather than in the MSVC toolchain, so tools/build.py's tool()
helper - which searches the MSVC bin - cannot find it and this does its own lookup.
"""
import glob
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

RC = os.path.join(HERE, "openmm2.rc")
RES = os.path.join(ROOT, "build", "openmm2.res")


def rc_exe():
    """The newest rc.exe in the Windows SDK, or None if the SDK is not installed.

    Returning None rather than exiting is deliberate: an executable without an icon is a cosmetic
    loss, and failing an otherwise good build over it would be out of proportion.
    """
    hits = sorted(glob.glob(
        r"C:\Program Files (x86)\Windows Kits\10\bin\*\x86\rc.exe"))
    return hits[-1] if hits else None


def main():
    rc = rc_exe()

    if not rc:
        print("  rc.exe not found - building without an icon or version block")
        return 0

    if not os.path.exists(RC):
        print("  no %s - building without an icon or version block" % RC)
        return 0

    os.makedirs(os.path.dirname(RES), exist_ok=True)

    # Two different include needs. The .rc references openmm2.ico by a bare filename, so tools/
    # has to be on the path; and it includes <windows.h> for the VS_VERSION_INFO constants, which
    # lives in the SDK. rc.exe inherits neither from the environment here.
    cmd = [rc, "/nologo", "/i", HERE]

    sdk = sorted(glob.glob(r"C:\Program Files (x86)\Windows Kits\10\Include\*"))

    if sdk:
        for part in ("um", "shared"):
            cmd += ["/i", os.path.join(sdk[-1], part)]

    cmd += ["/fo", RES, RC]
    result = subprocess.run(cmd, capture_output=True, text=True)

    if result.returncode != 0:
        print("  rc.exe failed:")

        for line in (result.stdout + result.stderr).splitlines():
            if line.strip():
                print("    %s" % line.strip())

        # Same reasoning as a missing SDK: report it clearly and let the build continue.
        if os.path.exists(RES):
            os.remove(RES)

        return 0

    print("  wrote %s (%d bytes)" % (RES, os.path.getsize(RES)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
