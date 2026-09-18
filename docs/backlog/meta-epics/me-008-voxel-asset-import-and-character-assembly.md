# ME-008 — Voxel Asset Import and Character Assembly

## Goal

Provide a practical content path for small-scale voxel objects and articulated characters **without building a custom modeling program**, converting mature external-tool output into the same VoxelVolume representation and mesher used by terrain.

## Epics

- [EP-030 — External Voxel Asset Import and Volume Conversion](../epics/EP-030-external-voxel-import.md)
- [EP-031 — Articulated Voxel Character Assembly and Transform Animation](../epics/EP-031-articulated-voxel-characters.md)

The shared mesher lives in ME-001/EP-032 rather than this tooling meta-epic because it is runtime infrastructure for both world and model volumes.

## Meta-epic invariants

- Imported models become normal VoxelVolumes; they do not create a second cell format.
- The importer is replaceable if authoring tools change.
- No custom MagicaVoxel/Blender replacement is required for H0/H1.
- Rigid-part animation updates transforms only.
- Equipment uses independent VoxelModel assets and attachment transforms.
- Import/runtime schemas remain versioned and validate atomically.
