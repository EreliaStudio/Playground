# ST-001-04 — Runtime-sized VoxelModel storage

## Intent

Introduce the semantic `VoxelModel` volume type for imported props and rigid assembly parts, reusing the owning vector storage, checked read contract, and batched editor/version behavior supplied by ST-001-02.

## Execution decision policy

- Do not invent unspecified asset, serialization, editing, or ownership behavior.
- Pause and record any missing observable policy as an Open Decision.
- Preserve `Voxel::Cell` byte-for-byte under resolved OD-023.

## Starting state / prerequisites

- ST-001-02 owning VoxelVolume/read contract is complete.
- The model dimensions and voxel size are runtime data, not template parameters.

## Owned behavior

- A `VoxelModel` is a semantic volume type with arbitrary valid runtime dimensions and uniform scale.
- Its cells use the same `Voxel::Cell` representation and storage/access rules as VoxelVolume and Chunk.
- It inherits the generic controlled editor/version behavior without adding a model-specific mutation API.

## Explicitly not owned

- File format, serialization, external authoring tool, importer, stable asset identity, or Definition mapping.
- Anchors, pivots, hierarchy, animation, equipment slots, world transform, rendering, Palette ownership, GPU resources, or meshing.
- Chunk world/streaming/generation behavior.

## Behavioral acceptance

- [x] Models with dimensions `10×10×14`, `12×7×14`, and `32×32×22` use the same C++ type.
- [x] An `8×8×16` model at scale `0.1` reports local bounds `0.8×0.8×1.6` within floating tolerance.
- [x] Inherited model editing stores and retrieves the same packed `Voxel::Cell` values through the public read contract and publishes one invalidation per changed session.
- [x] Invalid dimensions, scale, and coordinates follow the validation policy inherited from VoxelVolume.
- [x] Model access and bounds run headlessly without a Definition catalog, Window, OpenGL context, or GPU ownership.
- [x] No model-specific concept enters Chunk, VoxelVolume, or world lookup APIs.

## Rendering impact

None. This ticket creates headless model data only and does not require a golden image.

## Open decisions

- [OD-011](../../../open-decisions/OD-011-c-ownership-and-view-design-for-voxelvolume.md) — Resolved: VoxelModel uses the concrete vector-owning VoxelVolume base.
- [OD-023](../../../open-decisions/OD-023-whether-cell-orientation-and-flip-must-be-expanded.md) — Resolved: preserve current packing.
- Any importer/serialization/editing policy beyond the smallest required in-memory behavior remains a separate decision for its owning ticket.

## Completion evidence

- Four focused `VoxelModelTest` headless tests cover representative runtime dimensions, scale/bounds, packed cell values with one invalidation, and inherited rejection cases.
- Implementation head: [`d9f25c6`](https://github.com/EreliaStudio/Playground/commit/d9f25c6a91c32f8ac05ab71419bff30397c40d39), delivered by [PR #5](https://github.com/EreliaStudio/Playground/pull/5).
- [CI run 35469132300](https://github.com/EreliaStudio/Playground/actions/runs/35469132300) passed the CPU/headless regression suite and the unchanged Windows/OpenGL golden suite.
