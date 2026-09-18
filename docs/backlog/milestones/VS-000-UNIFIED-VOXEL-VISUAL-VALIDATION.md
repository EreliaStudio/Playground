# VS-000 — Unified Voxel Visual Validation

## Purpose

Validate the selected multi-scale voxel direction **before** large gameplay/content investment. This milestone is successful only if the working terrain behavior survives the refactor and small-scale voxel assets are visually/technically convincing without a custom modeling application.

## Required scene

One runtime scene contains simultaneously:

- generated 16×16×16 terrain Chunks at voxelSize 1.0;
- visible cube, slab, slope and stair Shape usage;
- palette Material terrain prototype;
- one externally authored/imported small-scale barrel VoxelModel;
- one articulated Hero assembled from multiple small-scale VoxelModels;
- one attached voxel weapon;
- lighting/camera sufficient to judge the art direction.

All voxel geometry must pass through the same fundamental path:

```text
VoxelVolume → NeighborResolver → VoxelMesher → Mesh + Materials → renderer
```

## Entry gates

- Current Chunk mesh semantic regression fixtures exist.
- EP-001 volume contract can adapt current Chunk and own runtime-sized model storage.
- EP-032 has old/new Chunk parity on representative fixtures.
- EP-030 can import at least one external model source without custom authoring software.

## System acceptance cases

1. **Terrain parity:** current representative Chunk fixtures retain cell/Shape/orientation/flip/occlusion semantics after old Baker path is replaced.
2. **Cross-Chunk neighbor:** boundary occlusion uses adjacent Chunk through resolver/context rather than VoxelMesher coupling to `Chunk::Collection`.
3. **Scale:** terrain 16³@1.0 and small models @<1.0 produce correct geometric extents while using identical grid-neighbor logic.
4. **Shared mesher:** telemetry/test instrumentation demonstrates terrain, barrel, Hero parts and weapon all use EP-032.
5. **Material terrain stability:** intended world-space palette variation is stable under camera movement and seamless across Chunk boundary fixture.
6. **Material object stability:** barrel/weapon object-local variation moves with object and does not swim.
7. **Articulation:** Hero idle/walk pose updates transforms with zero remesh after initial mesh availability.
8. **Equipment:** weapon attachment follows hand/part transform and can be swapped without body remesh.
9. **Edit invalidation:** changing a test barrel cell causes barrel remesh; moving it does not.
10. **Headless parity:** volume/import/meshing tests run without GPU; presentation scene does not change authoritative state hash.
11. **Performance capture:** record meshing time, mesh triangle/vertex counts, cache hits, draw calls and frame cost for the scene.
12. **Decision record:** document whether the visual result is accepted, what Material sampling rule is chosen, and any measured blocker before H1 content production.

## Explicit non-goals

- custom voxel editor;
- custom animation editor;
- GPU meshing;
- voxel LOD/SVO;
- per-voxel physics/networking;
- full PBR/material graph;
- automatic object fracturing.
