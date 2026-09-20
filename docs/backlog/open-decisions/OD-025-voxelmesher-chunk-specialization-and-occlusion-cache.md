# OD-025 — VoxelMesher Chunk specialization and occlusion cache ownership

## Status

Resolved

## Problem

Chunk and runtime-sized VoxelModel data require the same cell iteration, Shape expansion, transform, visibility, and mesh-output algorithm, but only Chunk meshing may resolve an out-of-volume neighbor through adjacent world Chunks. The architecture must preserve that additional behavior without duplicating the meshing algorithm or losing the effective occlusion-result cache.

## Affected epics/tickets

[EP-032](../epics/EP-032-unified-voxel-mesher/EP-032-unified-voxel-mesher.md), [ST-032-02](../epics/EP-032-unified-voxel-mesher/tickets/ST-032-02-generic-cell-iteration-and-scale-aware-shape-transform.md), [ST-032-03](../epics/EP-032-unified-voxel-mesher/tickets/ST-032-03-occlusionresolver-meshing-context.md), and [ST-004-02](../epics/EP-004-chunk-world-generation/tickets/ST-004-02-chunk-occlusion-resolver.md).

## Known constraints

- Chunk and VoxelModel must share one generic meshing implementation.
- VoxelMesher must remain usable for a standalone VoxelVolume without Chunk/world dependencies.
- A missing or unavailable external neighbor is treated as the existing empty `Voxel::Cell{}` representation (runtime ID `0`).
- The cache for transformed polygon/occluder results is performance-relevant and must remain shared generic behavior.
- OD-020 still owns later merging, final indexing, and hard-normal policy.

## Options considered

- Inject independent `OcclusionResolver` and `ChunkOcclusionResolver` strategy objects.
- Put Chunk lookup directly into one mesher and branch on the concrete volume type.
- Keep one inheritable `VoxelMesher` algorithm with a narrow overridable outside-volume neighbor hook, then derive `ChunkMesher` only for Chunk lookup.

## Decision

- `VoxelMesher` owns deterministic volume iteration, in-bounds cell lookup, Shape expansion/transform, visibility/occlusion evaluation, mesh emission, and the reusable occlusion-result cache.
- Neighbor coordinates inside the volume are resolved directly by `VoxelMesher` through the common read contract.
- Only an out-of-volume coordinate reaches a protected virtual neighbor hook. The base implementation returns `Voxel::Cell{}`, so standalone VoxelVolume and VoxelModel boundaries are empty.
- `ChunkMesher` derives from `VoxelMesher` and overrides only that hook. It maps the out-of-volume coordinate through the source Chunk coordinate and `Chunk::Collection` to query an adjacent available Chunk; missing Chunk data returns `Voxel::Cell{}`.
- `ChunkMesher` must not duplicate the iteration, Shape, occlusion, cache, or mesh-emission algorithm.
- The exact protected method name is an implementation detail; its call boundary and semantics are the contract.

## Rationale

Inheritance expresses the one intentional behavioral specialization while keeping the expensive and correctness-sensitive algorithm in one place. Models and standalone volumes get the simplest safe boundary rule. Chunk meshing adds world lookup without teaching the generic mesher about Chunk collection, streaming, or concrete volume type tests. Keeping the cache in the base ensures every meshing path benefits and preserves the current optimization.

## Consequences

- EP-032 introduces the common VoxelMesher and its default outside-is-empty hook.
- EP-004 supplies the concrete ChunkMesher override and Chunk/world coordinate conversion.
- The earlier standalone `OcclusionResolver`/`ChunkOcclusionResolver` strategy-object plan is superseded.
- Future specializations may override only external neighbor resolution unless another user-approved decision expands the inheritance contract.

## Validation / evidence

- ST-032-01 canonical fixtures remain the parity oracle for positions, normals, atlas UVs, winding, visible topology, and Chunk-boundary outcomes.
- ST-032-03 must prove standalone outside-is-empty behavior and that the shared occlusion cache is used by the base algorithm.
- ST-004-02 must prove adjacent Chunk lookup, missing-neighbor fallback, and unchanged cross-Chunk occlusion through `ChunkMesher`.

## Resolution provenance

Resolved by the project owner during ST-032-01 execution on 20 September 2026: one generic VoxelMesher base, a ChunkMesher subclass that overrides external-neighbor lookup, and retention of the shared occlusion-result cache.
