# ARCH-007 — Voxel Meshing, Palette Binding, and Assembly Contract

## Fixed pipeline

```text
VoxelVolume + OcclusionResolver + MaterialResolver
                         ↓
                    VoxelMesher
                         ↓
                     VoxelMesh
                         ↓
             voxel rendering + bound Palette
```

- `VoxelVolume` exposes dimensions, uniform voxel scale, bounds, and cells inside one volume. It owns no world lookup, palette, animation, or rendering behavior.
- `OcclusionResolver` tells `VoxelMesher` whether a candidate polygon is hidden. Standalone volumes treat cells outside their bounds as empty.
- `ChunkOcclusionResolver` is the EP-004 world-aware implementation. It resolves an out-of-volume coordinate through Chunk world/collection state so a voxel in Chunk B can occlude a boundary polygon in Chunk A.
- `MaterialResolver` independently selects the `paletteElementIndex` for each emitted polygon.
- `VoxelMesher` depends on these contracts, never directly on `Chunk::Collection`.

## Epic ownership

| Epic | Owns | Does not own |
|---|---|---|
| EP-001 | Minimal volume contract; dimensions/cell access/uniform scale/bounds; fixed 16³ Chunk adaptation; owning runtime-sized VoxelModel storage | World lookup, streaming, external occlusion, meshing, rendering |
| EP-032 | OcclusionResolver; standalone outside-is-empty policy; MaterialResolver contract; Shape expansion; visibility; deterministic VoxelMesh output | Chunk::Collection, streaming, GPU upload |
| EP-004 | ChunkOcclusionResolver; world/local lookup; Chunk integration; bake scheduling; dirty/version and neighbor invalidation; streaming/generation integration | A second volume adapter or mesher |

The structural order is EP-001 → EP-032 → EP-004 at the required-contract level. EP-001 plus EP-029 feeds EP-030. EP-001, EP-030, and EP-032 feed EP-002; EP-002 then feeds EP-031.

## Pure VoxelModel and separate VoxelAssembly

A VoxelModel asset is reusable pure volume data: dimensions, voxel scale, occupancy/Shape-or-cell data, and an authored palette element index. It contains no anchors, pivots, parents, skeleton, animation, equipment slots, or world transform.

A VoxelAssembly separately defines named anchors, optional parent anchors, local transforms, and parts that reference reusable VoxelModels. Animation clips target anchor transforms. Hierarchy evaluation produces per-part model matrices. It never edits voxel geometry or invokes `VoxelMesher`; cached part meshes remain valid.

Equipment uses the same named-anchor mechanism and reusable model parts. The exact anchor vocabulary and serialized model/assembly/animation syntax require user-approved sample assets before they are frozen. EP-002 owns runtime representation and execution. EP-031 owns external authoring/import and production fixtures that consume EP-002 contracts.

## Palette and mesh contract

A CPU-side `PaletteCollection` may manage Palette resources, but it is not uploaded as one global GPU buffer. Each `Palette` is independently associated with its own SSBO. A render command binds the program, mesh vertex/index buffers, the Palette required by that renderable, transform data, and then draws.

`VoxelMesh` vertices semantically carry at least `position`, `normal`, and integer `paletteElementIndex`. The index is flat/non-interpolated and addresses the currently bound Palette SSBO. It is not a Voxel ID, and no GPU `paletteId` exists in the initial design.

Palette selection belongs to the renderable/part instance, not the pure model asset. Consequently one cached arm mesh can render with Human, Orc, or Undead palettes without remeshing. All referenced indices must exist in the bound Palette; an out-of-range reference is detected and rejected.

Chunks normally share one relatively large `WorldPalette`. Chunk material resolution may use both the Shape polygon's semantic `materialSlot` and spatial `outerSide`; these concepts remain distinct. A dedicated content-schema ticket defines precedence among default, slot, side, and exact slot+side overrides after a user-approved example. Top, side, bottom, and individual sides can resolve to different elements of the same WorldPalette.

Chunks and VoxelModels use the same VoxelMesh format and voxel shader. Changing the bound Palette changes material output but neither mesh geometry nor mesher invocation count.

## Visual-regression sequence

1. Capture and manually approve current textured Chunk images.
2. Introduce VoxelVolume, OcclusionResolver, ChunkOcclusionResolver, and VoxelMesher.
3. Prove semantic mesh parity and unchanged textured images.
4. Introduce palette-based rendering as a separate intentional migration.
5. Review expected, produced, and difference images manually.
6. Only after approval, add a new versioned expected baseline.

Required semantic fixtures cover authored model indices, Chunk slot/side mapping, top/side/bottom and per-side variation, invalid indices, identical geometry across Palette swaps, no remesh on Palette swap, consistent polygon vertex indices, internal-face occlusion, multiple Shapes, and deterministic output. Golden fixtures cover the corresponding visible cases and render one model mesh with two Palettes plus a Chunk through WorldPalette.
