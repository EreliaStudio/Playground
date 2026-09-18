# Open Decision Gates

Open decisions block only stories that require the exact choice. Interfaces/configuration can proceed first.

| ID | Decision |
|---|---|
| OD-001 | Exact Hero/combat stat sheet and defaults. |
| OD-002 | Equipment stat-budget and roll distribution model. |
| OD-003 | Variable properties stored by spell-item instances. |
| OD-004 | Durability loss, repair and maximum-durability rules. |
| OD-005 | Exact diminishing-return defense and critical formulas. |
| OD-006 | Combat elastic-area / flee radius and related tuning. |
| OD-007 | Town building catalog, upgrade costs and service mapping. |
| OD-008 | Number/cadence/content progression of Worlds. |
| OD-009 | Marketplace procurement fair-price/outlier policy. |
| OD-010 | PvP design; intentionally separate from baseline implementation. |
| OD-011 | Exact C++ ownership/view design of the generic VoxelVolume contract and whether model storage is dense initially. Fixed Chunk storage remains optimized internally. |
| OD-012 | First external voxel/3D authoring tool and interchange format used by EP-030. |
| OD-013 | Exact authoritative collision primitives for articulated entities; visual VoxelModel cells do not implicitly define them. |
| OD-014 | Simulation fixed tick rate(s). |
| OD-015 | Persistence backend/cloud topology. |
| OD-016 | Network transport/library. |
| OD-017 | Whether richer non-cube Shape assignments for imported models use sidecar metadata, Blender data or remain hand-authored content initially. |
| OD-018 | First non-color Material properties/effects after palette prototype; emissive is a likely early candidate but not required for VS-000. |
| OD-019 | Material palette variation algorithm and sampling-space declaration after visual comparison (world-continuous vs object/local). |
| OD-020 | VoxelMesher surface merge/indexing aggressiveness and hard-edge normal policy, driven by parity fixtures and profiling. |
| OD-021 | Whether static imported VoxelModel meshes are serialized as derivative build cache or generated/cached on load; source VoxelVolume remains authoritative either way. |
| OD-022 | Articulation/pivot/attachment/animation authoring source: Blender, selected voxel tool metadata, or lightweight sidecar. No custom animation editor in H0/H1. |
| OD-023 | Whether existing horizontal Orientation + vertical Flip is sufficient for model art. Expanding Cell rotation is explicitly outside the unification refactor until content proves it necessary. |
