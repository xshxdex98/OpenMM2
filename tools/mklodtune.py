import sys, os, glob, subprocess
sys.path.insert(0, "tools")
from build import tool

cl = tool("cl.exe")
vc = cl.split(os.sep + "bin" + os.sep)[0]
sdk = sorted(glob.glob(r"C:\Program Files (x86)\Windows Kits\10\Include\*"))[-1]
lib = sdk.replace("Include", "Lib")

args = [cl, "-nologo", "-EHsc", "-O2",
        "-I", os.path.join(vc, "include"),
        "-I", os.path.join(sdk, "um"),
        "-I", os.path.join(sdk, "shared"),
        "-I", os.path.join(sdk, "ucrt"),
        "-Fe:" + os.path.join("build", "lodtune.exe"), "-Fo:" + os.path.join("build", "lodtune.obj"),
        os.path.join("tools", "lodtune.cpp"), "-link",
        "-LIBPATH:" + os.path.join(vc, "lib", "x86"),
        "-LIBPATH:" + os.path.join(lib, "um", "x86"),
        "-LIBPATH:" + os.path.join(lib, "ucrt", "x86")]

r = subprocess.run(args, capture_output=True, text=True)
print(r.stdout[-2000:])
print(r.stderr[-800:])
sys.exit(r.returncode)
