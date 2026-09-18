# Erelia Implementation Backlog — Unified Multi-Scale Voxel Revision

This folder is the **same progressively elaborated Erelia implementation backlog**, revised around the unified multi-scale voxel architecture. Unrelated gameplay, authority, economy, dungeon and networking epics are retained; the voxel/model/tooling assumptions have been rewritten rather than replaced with a second plan.

See [GDD alignment review](traceability/GDD-ALIGNMENT-REVIEW.md) for the repository integration and design changes.

## Verified baseline

- GDD: [Erelia GDD](../gdd/Erelia_GDD_Current.md)
- GDD revision: **17 September 2026**
- Playground baseline commit: `ed75feb5e9e9a748c8bb4df95e341a19648d0091`
- Sparkle baseline: `Version0.1.1`
- Sparkle baseline commit: `9784377d41509234d43e4adec16505382eef178f`

The current Playground implementation already provides normalized data-driven `Voxel::Shape` polygons, material-slot names, UVs, `Voxel::Definition`, a compact 32-bit `Voxel::Cell` with orientation/vertical flip, headless 16³ `Chunk` storage, `Chunk::Collection::worldCell`, and `Chunk::Baker` coupled to chunk-neighbor lookup. Those are the migration baseline, not disposable prototypes.


Current package: **34 epics, 137 candidate stories, 546 integration cases, 828 story acceptance cases, 9 capability/root meta-epics and 64 Markdown files.**

## Central architecture principle

> **Erelia has one multi-scale voxel representation. A Chunk is a 16×16×16 dynamically generated voxel volume with a one-world-unit voxel scale. A model is the same kind of voxel volume with arbitrary runtime dimensions and a smaller configurable voxel scale. Both use the same voxel Definitions, Shapes, meshing principles and material system; they differ in ownership and gameplay semantics, not in graphical data representation.**

Conceptually:

```text
Voxel::Cell
    ↓
Voxel::Definition
    ↓
normalized Voxel::Shape + material slots

              shared cell representation
                       ↓
                  VoxelVolume
          dimensions + cells + voxelSize
              ↙                    ↘
        Chunk semantics          VoxelModel semantics
      16³ / streaming /         arbitrary dimensions /
       generation / world        imported asset / entity
              ↘                    ↙
               NeighborResolver
                       ↓
                  VoxelMesher
                       ↓
              Mesh + material refs
                       ↓
               ordinary rendering
```

The exact C++ names, ownership model and template signatures remain implementation decisions. In particular, a Chunk may keep fixed-size optimized storage internally while exposing the same read contract the mesher uses for runtime-sized model volumes.

## Safe migration rule

Do **not** rewrite the working world renderer and material system in one step.

1. Characterize current chunk output with semantic fixtures and manually approved textured PNG references using SparkleTestLibrary image comparison.
2. Extract generic volume access from `Chunk` without changing current output.
3. Extract neighbor resolution from `Chunk::Baker` while preserving cross-chunk behavior.
4. Generalize the baker into one `VoxelMesher` while continuing to emit the current textured mesh representation.
5. Prove terrain output/behavior parity, including unchanged textured PNG comparisons on the supported GPU runner.
6. Add voxel scale and runtime-sized/imported `VoxelModel` volumes.
7. Mesh a small prop through the exact same mesher.
8. Introduce the new Material abstraction alongside the current atlas path.
9. Compare non-textured/palette output against the old textured references, review the resulting failures, and explicitly version approved new visual baselines before migrating content incrementally.
10. Assemble the first articulated Hero from cached rigid voxel-volume parts.

## Character direction

Characters are **assemblies of rigid voxel volumes**, not deforming monolithic volumes and not a separate voxel format:

```text
Hero
├── torso VoxelModel
├── head VoxelModel
├── upper arm VoxelModel
│   └── lower arm VoxelModel
├── upper leg VoxelModel
│   └── lower leg VoxelModel
└── attached equipment VoxelModels
```

Animation changes part transforms. Transform animation must not trigger voxel remeshing. Actual voxel edits invalidate only the affected volume mesh.

## Asset workflow

The plan explicitly rejects a custom voxel-modeling application. H0 investigates mature external voxel/3D authoring tools and common formats, then implements the smallest importer/converter needed to produce the shared `VoxelVolume` representation. Blender or sidecar metadata may be used for hierarchy, pivots, attachments and animation if useful.

## Material direction

The current atlas/UV system remains supported during the refactor. The target rendering abstraction is:

```text
Shape::materialSlot
        ↓
Voxel::Definition
        ↓
Material
```

A Material begins with palette/color behavior and can later own emissive, metallic/roughness, transparency or time/procedural parameters. Sampling space is explicit: terrain may need continuous world-space variation while movable models need object/local-space variation.

## Hierarchy

```text
ME-000 Erelia Implementation
└── Capability Meta-Epics
    └── Epics
        └── Candidate Stories
            └── Acceptance cases
```

## Testing convention

| Level | Backlog source | Intended implementation |
|---|---|---|
| Meta-epic / milestone | End-to-end invariants | system / vertical-slice tests |
| Epic | `Integration Test Catalogue` | integration tests |
| Story | Acceptance cases | unit / component tests |
| Architecture | cross-cutting properties | property, fuzz, soak, chaos, visual-regression and performance tests |

## Planning horizons

- **H0 — Foundations + visual validation:** characterize the current renderer, extract the unified volume/neighbor/mesher contracts, retain chunk parity, import one small model, prototype Materials and prove the multi-scale scene in `VS-000`.
- **H1 — First playable vertical slice:** one World proving the Erelia loop using the validated unified voxel path and an articulated Hero.
- **H2 — Onlineization and scale:** dedicated authority, remote replication, reconciliation, stronger persistence/operations.
- **H3 — Economy/content depth:** robust marketplace procurement, larger content catalog, material effects and scale/performance based on profiling.

See [`milestones/HORIZONS.md`](milestones/HORIZONS.md), [`milestones/VS-000-UNIFIED-VOXEL-VISUAL-VALIDATION.md`](milestones/VS-000-UNIFIED-VOXEL-VISUAL-VALIDATION.md) and [`milestones/VS-001-FIRST-PLAYABLE.md`](milestones/VS-001-FIRST-PLAYABLE.md).

## Rules for refinement

1. Never invent a balance/design value merely to make a story look complete.
2. Turn unresolved GDD points into a `Decision Gate`.
3. Prefer a vertical behavior slice over a large subsystem rewrite.
4. Domain state must remain testable without a Sparkle window or GPU.
5. Rendering is a view of authoritative/simulation state.
6. Visual voxel cells do not automatically become collision, navigation, networking or simulation entities.
7. One generic meshing implementation must serve Chunks and VoxelModels unless measured evidence proves otherwise.
8. Current working Chunk behavior must have regression fixtures before structural refactoring.
9. Material migration follows mesher parity; it does not gate extraction of the unified volume contract.
10. Every epic must pass its integration catalogue before it is Done.
11. Every story must have executable acceptance tests before it is Done.
12. Performance targets are measured with fixed fixtures and distributions, not one timing sample.
