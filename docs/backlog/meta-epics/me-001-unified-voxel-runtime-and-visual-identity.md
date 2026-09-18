# ME-001 — Unified Voxel Runtime and Visual Identity

## Goal

Evolve the working terrain voxel implementation into one reusable multi-scale voxel-volume/meshing/material path without regressing current Chunk behavior.

## Epics

- [EP-001 — Unified Voxel Volume and Cell Contract](../epics/EP-001-unified-voxel-volume.md)
- [EP-002 — Voxel Models, Assemblies, Animation, and Mesh Reuse](../epics/EP-002-voxel-model-assemblies.md)
- [EP-003 — Material Rendering, Mesh Runtime, and Caching](../epics/EP-003-runtime-rendering-mesh-cache.md)
- [EP-032 — Unified Voxel Mesher and Neighbor Context](../epics/EP-032-unified-voxel-mesher.md)
- [EP-033 — Multi-Scale Material Prototype and Visual Validation](../epics/EP-033-material-visual-validation.md)

## Meta-epic invariants

- A 16³ Chunk and arbitrary VoxelModel expose the same meshing-facing cell/scale contract.
- One mesher handles both; no terrain/model mesher fork exists in the first implementation.
- Existing cube/slab/slope/stair/cross shape behavior, orientation, vertical flip and cross-chunk occlusion remain covered by regression tests.
- Scale affects geometry coordinates, not logical grid neighborhood.
- Entity transform/animation does not trigger remeshing.
- Material sampling remains stable in the intended coordinate space.
- Headless world/model data and meshing tests require no GPU.
