# ST-001-03 — Chunk adapter/specialization

## Intent

Make current Chunk derive from/use the resolved concrete `VoxelVolume` contract while preserving fixed 16³ Chunk semantics and all observable behavior.

## Execution decision policy

- Do not invent unspecified observable behavior; record a missing choice as an Open Decision.
- Follow resolved OD-011 even where it supersedes the earlier fixed-array optimization option.
- Preserve `Voxel::Cell` byte-for-byte under resolved OD-023.

## Starting state / prerequisites

- ST-001-01 current-Chunk semantic and textured golden baseline is complete.
- ST-001-02 owning VoxelVolume/read contract is complete.

## Owned behavior

- Construct the Chunk volume as exactly `16 × 16 × 16` at voxel size `1.0`.
- Remove duplicate Chunk cell ownership and use the vector storage owned by `VoxelVolume`.
- Preserve coordinate-to-index order and the existing `Chunk::Editor` mutation/version behavior.
- Preserve Chunk coordinate/world conversion, Collection, generator, scheduler, and renderer call sites.

## Explicitly not owned

- Runtime-sized VoxelModel behavior.
- World lookup or cross-Chunk access inside `VoxelVolume`.
- Mesher extraction, Palette migration, new rendering behavior, or changed golden images.

## Behavioral acceptance

- [ ] Chunk reports dimensions `16 × 16 × 16`, voxel size `1.0`, and local bounds `[0,16]³` through `VoxelVolume`.
- [ ] First, interior, and last Chunk cells match the same coordinates and span positions as before migration.
- [ ] Existing packed Cell ID/orientation/flip values round-trip byte-for-byte.
- [ ] Invalid local coordinates remain rejected without aliasing storage.
- [ ] `Chunk::Editor` still invalidates the version only when a cell actually changes.
- [ ] Chunk world/local conversion, Collection, generation, baking, and scheduling CPU tests remain green.
- [ ] All 20 approved current textured Chunk golden comparisons remain green on the supported runner.
- [ ] No approved reference, comparison tolerance, texture, or UV is changed.

## Rendering impact

No intentional visual impact. The existing 20-reference suite is mandatory regression evidence for the storage migration.

## Open decisions

- [OD-011](../../../open-decisions/OD-011-c-ownership-and-view-design-for-voxelvolume.md) — Resolved: Chunk keeps fixed dimensions but migrates from its fixed array to the vector owned by VoxelVolume.
- [OD-023](../../../open-decisions/OD-023-whether-cell-orientation-and-flip-must-be-expanded.md) — Resolved: preserve current packing.

## Completion evidence

- Focused Chunk/VoxelVolume integration tests.
- Passing complete CPU/headless suite.
- Passing supported Windows/OpenGL golden suite with unchanged reference hashes.
- Implementation commit/PR and CI links.
