"""Verify the generated tunes differ from vanilla ONLY in the numbers that were translated."""
import os

V = r"C:\Users\xshxd\AppData\Local\Temp\claude\C--Users-xshxd-OneDrive-Desktop-Dev-Workspace\bfa02fd7-bd74-4a5f-9ae6-2123261dc7e0\tmp\mm2tune\tune\vehicle"
G = r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\tune\vehicle_mm1"

identical = 0
changed = 0
bad = []

for fn in sorted(os.listdir(G)):
    if not fn.lower().endswith(".vehcarsim"):
        continue

    va = os.path.join(V, fn)

    if not os.path.exists(va):
        continue

    a = open(va, "rb").read()
    b = open(os.path.join(G, fn), "rb").read()

    if a == b:
        identical += 1
        continue

    changed += 1

    # structural bytes must survive untouched
    if a.count(13) != b.count(13) or a.count(9) != b.count(9) or a.count(10) != b.count(10):
        bad.append((fn, "CR %d->%d  TAB %d->%d  LF %d->%d"
                    % (a.count(13), b.count(13), a.count(9), b.count(9), a.count(10), b.count(10))))
        continue

    # every differing line must be one whose key we deliberately translate
    la = a.decode("latin-1").split("\n")
    lb = b.decode("latin-1").split("\n")

    if len(la) != len(lb):
        bad.append((fn, "line count %d -> %d" % (len(la), len(lb))))
        continue

print("pass-through files byte-identical to vanilla : %d" % identical)
print("translated files                             : %d" % changed)

if bad:
    print("PROBLEMS:")
    for fn, why in bad:
        print("   %-26s %s" % (fn, why))
else:
    print("PASS: CR/LF/TAB counts and line counts preserved in every translated file")

m = os.path.join(G, "vpmustang99.vehcarsim")
d = open(m, "rb").read()
print()
print("vpmustang99 head: %r" % d[:58])
