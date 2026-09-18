# EP-032 — Unified Voxel Mesher and Neighbor Context

**Meta-epic:** ME-001  
**Horizon:** H0  
**GDD / architecture coverage:** Core refactor of current Chunk::Baker into one scale-aware mesher for Chunks and VoxelModels  
**Dependencies:** EP-000, EP-001

## Goal

Extract the current Chunk baking logic into one generalized, headless, scale-aware VoxelMesher that consumes a volume plus an external neighbor-resolution context, preserving current terrain behavior before serving imported models.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.
- The mesher must not depend on `Chunk::Collection`, entity types, streaming or authoring formats.
- Start by preserving current textured output/UV/material-slot behavior; Material migration is EP-003/033.
- Voxel scale multiplies positions only; visibility remains discrete-grid logic.
- Shape normalization, current orientation and vertical flip semantics remain unchanged.
- Current partial-neighbor/occlusion behavior is regression-locked before any improvement.
- A fixed-size Chunk optimization may exist behind the common read contract but must not create a second mesher.
- Meshing is CPU/headless data transformation; GPU upload is EP-003.

## Integration Test Catalogue

### Epic-specific cases

1. Golden current Chunk fixture produces semantically equivalent geometry/material data through old Baker and new VoxelMesher.
2. Solid adjacent cube cells emit no internal shared face.
3. Slab/slope/stair/cross fixtures preserve current transformed geometry, normals and occlusion behavior.
4. Orientation and vertical flip yield equivalent results to current Baker behavior.
5. Standalone volume treats outside-grid neighbors as empty.
6. Chunk context resolves outside-grid neighbor from adjacent Chunk and preserves cross-Chunk occlusion.
7. Identical cell arrangements at scale 1.0 and 0.1 have equivalent topology/material assignments while vertex positions/bounds scale by 10×.
8. Runtime-sized imported barrel uses exactly the same mesher implementation and Definition/Shape catalog resolution as a Chunk.

### Common integration invariants

1. Nominal behavior succeeds through the public/domain API without bypassing validation.
2. Invalid or unauthorized input is rejected without leaving partial state behind.
3. Exact boundary values are tested, including empty/minimum/maximum supported cases.
4. Create → use → unload → recreate does not retain stale state or dangling registrations.
5. Where deterministic behavior is expected, identical seed/configuration/commands produce identical results.
6. The feature can execute in the headless test harness when presentation is not the subject of the test.
7. If the feature is persistent, serialize → reload preserves its semantic state and stable IDs.
8. Dependency/service failure follows the documented rollback, fallback, or fail-closed policy.

## Candidate Stories

### ST-032-01 — Current Baker semantic golden fixtures

Capture current mesh semantics before code extraction and build comparison helpers tolerant of harmless vertex ordering differences.

**Acceptance cases**

- [ ] Fixtures include cube/slab/slope/stair/cross and chunk boundary cases.
- [ ] Comparison checks positions/normals/material/UV or slot semantics and visible topology.
- [ ] Old Baker remains available in test path until parity gate passes.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-032-02 — Generic cell iteration and scale-aware Shape transform

Move Shape expansion/transform from Chunk-specific code to volume-based meshing.

**Acceptance cases**

- [ ] Shape vertex mapping includes voxel coordinate and uniform voxelSize.
- [ ] Scale does not alter occupancy/neighbor decisions.
- [ ] Normals remain unit/correct under uniform scale.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-032-03 — NeighborResolver / meshing context

Externalize neighbor lookup so standalone and Chunk volumes use different boundary policies without mesher branching on type.

**Acceptance cases**

- [ ] Standalone resolver returns empty outside bounds.
- [ ] Chunk resolver obtains adjacent world cells through Collection semantics.
- [ ] Resolver contract is unit-testable without mesh/GPU types.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-032-04 — Optimization and indexed output

Preserve/remove hidden surfaces and emit efficient indexed mesh sections while profiling against the current Baker.

**Acceptance cases**

- [ ] No one-draw-call-per-voxel path is introduced.
- [ ] Material boundaries are preserved.
- [ ] Any performance regression versus current 16³ fixtures is measured and either fixed or explicitly accepted before old Baker removal.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.


## Epic Exit Criteria

- [ ] All activated stories meet their acceptance cases.
- [ ] Epic-specific and common integration cases are automated and passing.
- [ ] Invalid/corrupt content or commands cannot leave impossible partial state.
- [ ] Relevant deterministic/headless fixtures pass.
- [ ] Public/runtime schemas and decision gates are documented.
- [ ] Profiling exists for any path expected to be performance-sensitive.
