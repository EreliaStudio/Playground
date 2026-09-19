# ST-001-02 — Read-only VoxelVolume contract

**Status:** In implementation until focused tests and the existing regression suite pass

## Intent

Introduce the smallest common owning volume and read contract required by later meshing, following resolved [OD-011](../../../open-decisions/OD-011-c-ownership-and-view-design-for-voxelvolume.md).

## Execution decision policy

- Do not invent, infer, or silently select behavior, data, schemas, ownership, error policy, architecture, or technology not defined here or by a resolved linked decision.
- Treat a missing observable choice as an Open Decision and pause the affected work.
- Preserve the existing `Voxel::Cell` representation exactly as resolved by OD-023.

## Starting state / prerequisites

- ST-001-01 is complete: the current Chunk has headless characterization and 20 approved textured references.
- Playground consumes the installed `Sparkle::TestLibrary` from the pinned prebuilt Sparkle package.
- OD-011 and OD-023 are resolved.

## Owned behavior

- A concrete `VoxelVolume` owns a runtime-sized `std::vector<Voxel::Cell>`.
- Construction takes runtime `spk::Vector3UInt` dimensions and a uniform voxel size.
- Every dimension and the finite voxel size are strictly positive.
- Cells default to the existing empty `Voxel::Cell` value.
- Public access is read-only through checked `at(spk::Vector3Int)` and `std::span<const Voxel::Cell>`.
- Storage order preserves current Chunk indexing: X fastest, then Z, then Y.
- Local bounds are `[0, dimensions × voxelSize]`.
- Derived semantic volume types receive protected checked mutation access for their controlled editors/loaders.

## Explicitly not owned

- Adapting or modifying `Chunk`; ST-001-03 owns that migration.
- Introducing the `VoxelModel` semantic type; ST-001-04 owns it.
- World/chunk lookup, streaming, generation, coordinate conversion, meshing, rendering, Palette ownership, animation, or GPU behavior.
- Any change to `Voxel::Cell`, Definition, Shape, texture, UV, or approved golden references.

## Behavioral acceptance

- [ ] Runtime dimensions and uniform voxel size are returned exactly.
- [ ] A valid first, interior, and last coordinate returns the expected `Voxel::Cell`.
- [ ] Every negative and one-step-past coordinate is rejected with `std::out_of_range` and cannot alias valid storage.
- [ ] The read-only span contains exactly `x × y × z` cells in the documented order.
- [ ] Zero in any dimension is rejected with `std::invalid_argument`.
- [ ] Zero, negative, infinite, and NaN voxel sizes are rejected with `std::invalid_argument`.
- [ ] Local minimum is zero and local maximum equals `dimensions × voxelSize`.
- [ ] Changing voxel size changes physical bounds without changing dimensions, topology, or packed cell IDs.
- [ ] All focused tests run without creating a Window or OpenGL context.
- [ ] Existing CPU/headless tests remain green.
- [ ] Existing current-Chunk golden tests remain green on the supported runner; no reference or tolerance is changed.

## Rendering impact

None. This ticket adds headless data behavior only. The existing GPU suite is a no-change regression gate, not a source of new references.

## Open decisions

- [OD-011](../../../open-decisions/OD-011-c-ownership-and-view-design-for-voxelvolume.md) — Resolved: concrete vector-owning base, strict non-empty dimensions, finite positive scale, checked read access, protected mutation.
- [OD-023](../../../open-decisions/OD-023-whether-cell-orientation-and-flip-must-be-expanded.md) — Resolved: preserve the current packed Cell representation byte-for-byte.

## Completion evidence

- Focused GoogleTest files grouped into metadata/validation and access/topology behavior.
- Passing CPU/headless suite and supported Windows/OpenGL golden suite.
- No modified PNG references or broadened comparison tolerances.
- OD-011/OD-023 provenance and current project status updated in the implementation commit.
