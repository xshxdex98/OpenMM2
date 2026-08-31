# Midtown Madness 2 — City / PSDL format notes

Everything here was read out of the retail `midtown2.exe` by decompiling its own loaders, as part
of the OpenMM2 reimplementation. Every claim names the function and image-relative address it came
from, so you can check any of it yourself.

**Where I could not establish what a field *means*, it says so.** A field marked "purpose not
established" has a known size and offset and nothing more. I have not filled those in with
plausible guesses, because a wrong guess in a format spec costs more time than a blank does.

---

## 1. The load path

| Address | Function | Role |
|---|---|---|
| `0x00443F30` | `cityLevel::Load` | Opens every city asset, in sequence |
| `0x0045C040` | `lvlSDL::LoadBinary` | Parses the `.psdl` file |
| `0x0045BF90` | `sdlPage16::LoadBinary` | One geometry block |
| `0x0045D910` | `lvlAiMap::LoadBinary` | AI road network — **same stream** |
| `0x0045D870` | `lvlAiRoad::LoadBinary` | One AI road record |
| `0x0045D220` | `cityPropulator::Load` | Scatters props along road segments |
| `0x00445340` | `cityLevel::DecompressPvs` | Visibility sets |
| `0x0045C660` | `lvlSDL::Propulate` | Prop placement into the SDL |
| `0x00448330` | `sdlPage16::Draw` | Per-block draw — the other half of the format |

---

## 2. What a city is made of

A city is not one file. `cityLevel::Load` takes a city name and opens a fixed set of assets, each
through `datAssetManager::Open(dir, name, ext)`. The name you pass is the stem for nearly all of
them.

| Asset | Required | Contents |
|---|---|---|
| `city/<name>.psdl` | **Yes** | Geometry, rooms, textures **and the AI road network** |
| `city/materials.csv` | **Yes** | Two columns: texture name, physics material name |
| `city/<name>.mtl` | **Yes** | Opened as a tokenizer source alongside materials |
| `city/<name>.cpvs` | — | Compressed potentially-visible set; drives room culling |
| `city/<name>.lmap` | — | Light map |
| `city/<name>.water` | — | Per-room water. Engine logs `Room %d has Water of Death(tm)` |
| `city/<name>.inst` | — | Placed object instances — buildings, props, bangers |
| `city/<name>.propdefs` / `.proprules` | — | Propulator rules for scattering props along roads |
| `city/<name>.decals` | — | Road decals, loaded via `dgPathSet::Load` |
| `city/<name>.sf` | — | Purpose not established |
| `city/<name>.shadmap_day` / `.shadmap_nite` | — | Shadow maps, day and night |
| `city/<name>.refl_dc` | — | Reflection data |
| `city/<name>_ai.*` | — | AI companions, by name suffix |
| `tune/<name>.cinfo` | **Yes** | City registration — how the game knows the city exists |

Failure to load the PSDL is fatal and produces:

```
cityLevel::Load(%s) failed.
```

### A hardcoded special case

`cityLevel::Load` contains a string comparison against `"london"`. At least one behaviour is
branched on the city's *name* rather than on anything in its data. I did not chase down which
behaviour — but a city named `london` will not behave like an arbitrary one.

---

## 3. PSDL file layout

Little-endian throughout. The file is read strictly front to back with **no seeks and no offset
table**, so every section's position depends on the length of everything before it. You cannot
patch a section without rewriting what follows.

### Header

| Offset | Type | Field | Notes |
|---|---|---|---|
| `0x00` | `u32` | magic | `0x30445350` = `"PSD0"` |
| `0x04` | `u32` | targetSize | Must be exactly `2` |
| `0x08` | `u32` | vertexCount | |
| `0x0C` | `f32[3] × n` | vertices | `vertexCount × 12` bytes. One shared pool for the whole city |

Wrong magic:

```
Wrong version of PSDL file -- please re-save city from newer version of block editor.
```

Wrong target size:

```
Wrong target size -- use a different ...
```

Note the first message names the original authoring tool: a **block editor**.

### Then, in order

```
u32          attributeCount
u32[]        attributes          // attributeCount * 4 bytes, shared by all blocks

u32          textureCount
             // entries 1 .. textureCount-1   (index 0 is reserved/empty)
             for each:
               u8    nameLen     // 0 = empty slot: no texture, material 0
               char  name[nameLen]
               // a name ending "-0" is truncated by 6 chars before lookup
               // texture  -> gfxGetTextureMovie(name, true)
               // material -> looked up by name through materials.csv

u32          blockCount          // stored at lvlSDL+0x4C
u32          unknown             // stored at lvlSDL+0x50, purpose not established
             // blocks 1 .. blockCount-1   (index 0 is null)
             for each: sdlPage16::LoadBinary(stream)

u8[]         perBlockA           // blockCount bytes  -> lvlSDL+0x58
u8[]         perBlockB           // blockCount bytes  -> lvlSDL+0x5C

f32[3]       boundsA             // -> lvlSDL+0x0C
f32[3]       boundsB             // -> lvlSDL+0x18
f32[3]       unknownVec          // -> lvlSDL+0x28
f32          unknownScalar       // -> lvlSDL+0x34

             lvlAiMap::LoadBinary(stream)   // AI road network, SAME FILE
```

### The most important structural fact

**The AI road network lives inside the `.psdl` file.** There is no separate AI file to produce or
omit — `lvlSDL::LoadBinary` calls `lvlAiMap::LoadBinary` on the same open stream immediately before
closing it.

A PSDL writer that stops after the geometry produces a **truncated file**, not a city without
traffic.

---

## 4. Geometry block — `sdlPage16`

Each block's on-disk record is two counts and two arrays. Everything else a block needs is a
pointer into a city-wide shared pool, patched in by the caller after construction.

```
u32   perimeterCount
u32   indexCount
      // construct sdlPage16(perimeterCount, indexCount)
u32[] perimeter[perimeterCount]   // perimeterCount * 4 bytes
u16[] indices[indexCount]         // indexCount * 2 bytes; omitted entirely when indexCount == 0
```

After the block is read, `lvlSDL::LoadBinary` patches four pointers into it. **Blocks own no
geometry memory** — a block is a set of indices into city-wide tables.

Runtime struct, 0x20 bytes:

| Offset | Type | Field | Source |
|---|---|---|---|
| `0x00` | `u8` | flag | Read as unsigned (`movzx`) |
| `0x01` | `u8` | PerimeterCount | Constructor argument |
| `0x02` | `u8 × 2` | flags | Purpose not established |
| `0x04` | `Vector3*` | CodedVertices | **Patched** — shared vertex pool |
| `0x08` | `f32*` | Floats | **Patched** — the attribute array |
| `0x0C` | `u32` | — | Purpose not established |
| `0x10` | — | PerimeterVertices | From file, 4 bytes per entry |
| `0x14` | `u16*` | CodedVertexIndices | From file, 2 bytes per entry |
| `0x18` | `gfxTexture**` | Textures | **Patched** — city texture table |
| `0x1C` | `u8*` | Materials | **Patched** — per-texture material index |

### Not yet decoded — this is the real blocker

**How a perimeter entry's 4 bytes divide into fields, and the exact rule that turns perimeter +
index data into triangles.**

That is the remaining work for anyone writing a converter. `sdlPage16::Draw` at `0x00448330` is
where the answer is — it consumes exactly these arrays and emits geometry, so the decode rule is
readable there. I have not done that yet.

One useful detail on the vertex encoding: the vertex pool is allocated as
`(vertexCount * 3 + 0x600) * 4` bytes but only `vertexCount * 12` bytes are read into it, so the
engine reserves scratch space past the file data. The name "CodedVertices" and the `u16`
"CodedVertexIndices" suggest an index-compression scheme rather than raw positions per block.

---

## 5. AI road network — `lvlAiMap` / `lvlAiRoad`

Read from the PSDL stream, immediately after the geometry.

```
u32   roadCount
      // roads allocated as roadCount * 0x24 bytes
      for each road:  lvlAiRoad::LoadBinary(stream)
```

One road record, in file order:

| Bytes | Goes to | Reading |
|---|---|---|
| 4 | `+0x00` | Identifier or flags — purpose not established |
| 2 | `+0x08` | Two bytes read together, used separately as counts: byte `+0x08` and byte `+0x09` |
| `(b8+b9) × 4` | `+0x04` → heap | Sized from the sum of those two counts. Consistent with two lane-direction vertex lists sharing one array |
| 2 | `+0x0A` | Purpose not established |
| 16 | `+0x0C` | Four 32-bit values — plausibly a bounding box or plane, **not verified** |
| 1 | `+0x1C` | Count for the array that follows |
| `count × 2` | `+0x20` → heap | `u16` array — indices of some kind |

`sizeof(lvlAiRoad)` is `0x24`.

---

## 6. The materials CSV

Read *before* the geometry, by `lvlSDL::LoadBinary`, from `city/materials.csv`.

- The **first line is skipped** (a header row).
- Each line is parsed by `ParseCSVLine(line, 2, 0x20, field0, field1)` — **2 fields**, each capped
  at 32 characters.
- Field 0 is the texture name, field 1 is the physics material name.
- A material named **`none`** is skipped explicitly.
- The material name is resolved through `lvlMaterialMgr::Find`, then its index is found by scanning
  `lvlMaterialMgr::Lookup`, and `index + 1` is stored.
- The resulting name→index map is a `HashTable` sized `0x65` (101 buckets).

`lvlSDL+0x04` receives `maxMaterialIndex + 1` — the count of distinct materials the city uses.

---

## 7. Porting toward MM1

MM2 and MM1 share an engine lineage, so the *concepts* map — but the file formats do not, and MM1
imposes constraints MM2 has no equivalent of. These four are the ones that actually stop a city
loading, in the order they bite. (These were established separately, against the MM1 toolchain.)

1. **Cell IDs must be ≥ 200.** MM1's `prepare_portals()` skips every cell below
   `CELL_TYPE_SWITCH` (200) as a "landmark" cell. A city built only from those ships zero portals
   and black-screens with `asPortalWeb::GetStartCell failed` every frame.

2. **Cell IDs must stay ≤ 999.** The `.CELLS` row is read into a 255-character buffer that budgets
   four characters per entry — comma plus *three* digits. Four-digit IDs overflow it and loading
   stops at `Buffer too small for full line`. **That caps a city at 799 usable cells**, which is the
   hard ceiling on how much MM2 city can come across in one piece.

3. **Ship a compiled `.BAI`, or ship no AI at all.** Text `.road`/`.map` files are not enough — MM1
   needs the binary form, which only the game itself compiles. Text without BAI is *worse* than no
   AI: the map ends up with zero intersections and `mmGame::RespawnXYZ` null-dereferences through
   `aiMap::Intersection`.

4. **With no AI map, the car spawns at the world origin.** No spawn record means the camera can end
   up in no cell at all, which looks exactly like failure 1 even when the portals are correct.
   Translate the map so drivable road sits at `(0, -1, 0)`.

### The structural consequence

MM2's AI network is **inside** the PSDL. MM1 wants a **separately compiled** BAI. Those do not
convert into one another directly.

Getting geometry across without traffic is a much smaller problem than getting traffic across.
Treat them as two milestones, not one.

---

## 8. What is still open

Do not guess at these — each names where the answer lives.

- **The perimeter entry's internal structure and the triangulation rule.** Read
  `sdlPage16::Draw` @ `0x00448330`. This is the main blocker for a geometry converter.
- `lvlSDL+0x50`, and the vector and scalar at `+0x28` / `+0x34`.
- The two per-block byte arrays at `+0x58` and `+0x5C` — one byte per block each, so almost
  certainly per-room attributes.
- Most of the `lvlAiRoad` record beyond its shape.
- What the `"london"` comparison in `cityLevel::Load` actually changes.
- The `.cpvs`, `.lmap` and `.inst` formats. I established that they are opened and when, not how
  they are laid out. `cityLevel::DecompressPvs` @ `0x00445340` is the entry point for the first.

---

## Appendix — decompiled `lvlSDL::LoadBinary`

Lightly cleaned from the Ghidra output. Variable names are the decompiler's except where the
meaning was established; error handling and SEH scaffolding removed for readability.

```c
bool lvlSDL::LoadBinary(lvlSDL *this, const char *cityName)
{
    Stream *s = datAssetManager::Open("city", cityName, "psdl", false, true);
    if (s == NULL)
        return false;

    u32 magic, targetSize;
    Stream::Read(s, &magic, 4);
    Stream::Read(s, &targetSize, 4);

    if (magic != 0x30445350)            // 'PSD0'
        Quitf("Wrong version of PSDL file -- please re-save city from "
              "newer version of block editor.");
    if (targetSize != 2)
        Quitf("Wrong target size -- use a different ...");

    // ---- shared vertex pool
    u32 vertexCount;
    Stream::Read(s, &vertexCount, 4);
    g_VertexCount = vertexCount;
    g_Vertices    = operator new((vertexCount * 3 + 0x600) * 4);
    Stream::Read(s, g_Vertices, vertexCount * 12);

    // ---- shared attribute array
    u32 attributeCount;
    Stream::Read(s, &attributeCount, 4);
    void *attributes = operator new(attributeCount * 4);
    Stream::Read(s, attributes, attributeCount * 4);

    // ---- texture / material tables
    u32 textureCount;
    Stream::Read(s, &textureCount, 4);
    u32 *textures  = operator new(textureCount * 4);
    u8  *materials = operator new(textureCount);
    textures[0] = 0;
    materials[0] = 0;

    // materials.csv -> HashTable(name -> materialIndex + 1)
    Stream *csv = datAssetManager::Open("city", "materials", "csv", false, true);
    HashTable table(0x65, false, 0x20);
    int maxMaterial = 0;
    if (csv) {
        char line[64], texName[32], matName[32];
        fgets(line, 0x40, csv);                     // skip the header row
        while (fgets(line, 0x40, csv)) {
            if (ParseCSVLine(line, 2, 0x20, texName, matName) == 2) {
                if (strcmp(matName, "none") != 0) {
                    lvlMaterial *m = lvlMaterialMgr::Find(Instance, matName);
                    if (m) {
                        int i = 0;
                        while (i < Instance->count &&
                               lvlMaterialMgr::Lookup(Instance, i) != m)
                            i++;
                        HashTable::Insert(&table, texName, (void *)(i + 1));
                        if (maxMaterial < i + 1) maxMaterial = i + 1;
                    }
                }
            }
        }
        Stream::Close(csv);
    }

    for (u32 i = 1; i < textureCount; i++) {
        u8 len = Stream::GetCh(s);
        if (len == 0) {
            textures[i]  = 0;
            materials[i] = 0;
        } else {
            char name[32];
            Stream::Read(s, name, len);
            if (name[len - 6] == '-' && name[len - 5] == '0')   // "...-0" suffix
                name[len - 6] = '\0';
            gfxGetTextureMovie(&textures[i], name, true);
            materials[i] = (u8)HashTable::Access(&table, name);
        }
    }
    this->materialCount = maxMaterial + 1;          // lvlSDL+0x04

    // ---- geometry blocks
    Stream::Read(s, &this->blockCount, 4);          // lvlSDL+0x4C
    Stream::Read(s, &this->unknown50,  4);          // lvlSDL+0x50
    this->blocks = operator new(this->blockCount * 4);
    this->blocks[0] = NULL;
    for (u32 i = 1; i < this->blockCount; i++) {
        sdlPage16 *page = sdlPage16::LoadBinary(s);
        this->blocks[i] = page;
        page->CodedVertices = g_Vertices;           // +0x04
        page->Floats        = attributes;           // +0x08
        page->Textures      = textures;             // +0x18
        page->Materials     = materials;            // +0x1C
    }

    // ---- per-block byte arrays
    this->perBlockA = operator new(this->blockCount);   // +0x58
    Stream::Read(s, this->perBlockA, this->blockCount);
    this->perBlockB = operator new(this->blockCount);   // +0x5C
    Stream::Read(s, this->perBlockB, this->blockCount);

    // ---- bounds and trailing values
    Stream::Read(s, (char *)this + 0x0C, 12);
    Stream::Read(s, (char *)this + 0x18, 12);
    Stream::Read(s, (char *)this + 0x28, 12);
    Stream::Read(s, (char *)this + 0x34, 4);

    // ---- the AI road network, from THIS SAME STREAM
    lvlAiMap::LoadBinary(s);

    Stream::Close(s);
    return true;
}
```

## Appendix — decompiled `sdlPage16::LoadBinary`

```c
sdlPage16 *sdlPage16::LoadBinary(Stream *s)
{
    u32 perimeterCount, indexCount;
    Stream::Read(s, &perimeterCount, 4);
    Stream::Read(s, &indexCount, 4);

    sdlPage16 *page = new sdlPage16(perimeterCount, indexCount);

    Stream::Read(s, page->PerimeterVertices, perimeterCount * 4);   // +0x10
    if (indexCount != 0)
        Stream::Read(s, page->CodedVertexIndices, indexCount * 2);  // +0x14

    return page;
}
```

## Appendix — decompiled `lvlAiMap` / `lvlAiRoad`

```c
void lvlAiMap::LoadBinary(Stream *s)
{
    Stream::Read(s, &m_RoadCount, 4);
    m_AiRoads = operator new(m_RoadCount * 0x24);
    for (u32 i = 0; i < m_RoadCount; i++)
        lvlAiRoad::LoadBinary(&m_AiRoads[i], s);
}

void lvlAiRoad::LoadBinary(lvlAiRoad *this, Stream *s)
{
    Stream::Read(s, this, 4);                       // +0x00
    Stream::Read(s, (char *)this + 8, 2);           // +0x08, +0x09 : two counts

    u32 n = (this->b8 + this->b9) * 4;
    void *verts = operator new(n);
    this->vertices = verts;                         // +0x04
    Stream::Read(s, verts, n);

    Stream::Read(s, (char *)this + 0x0A, 2);        // +0x0A
    Stream::Read(s, (char *)this + 0x0C, 0x10);     // +0x0C : 16 bytes

    Stream::Read(s, (char *)this + 0x1C, 1);        // +0x1C : count
    void *idx = operator new(this->count * 2);
    this->indices = idx;                            // +0x20
    Stream::Read(s, idx, this->count * 2);
}
```

---

*Derived from `midtown2.exe` (retail) by decompilation, as part of the OpenMM2 reimplementation.
Addresses are image-relative and valid for the retail build.*
