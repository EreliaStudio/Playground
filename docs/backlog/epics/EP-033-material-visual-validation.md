# EP-033 — Multi-Scale Material Prototype and Visual Validation

**Meta-epic:** ME-001  
**Horizon:** H0  
**GDD / architecture coverage:** Early proof of the selected multi-scale voxel visual identity and Material sampling behavior  
**Dependencies:** EP-003, EP-004, EP-030, EP-031, EP-032

## Goal

Validate the unified visual architecture early with one representative scene using generated Chunks, current Shapes, palette Materials, an imported prop, an articulated Hero and attached weapon—all flowing through the same VoxelVolume → VoxelMesher → render path.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.
- This is a validation milestone, not a material-graph/editor project.
- First implementation can use simple color palettes and deterministic variation.
- Compare world/object sampling approaches visually before freezing Material schema details.
- Scene must keep current cube/slope/slab/stair behavior visible.
- Imported model/character content must not require custom authoring software.
- If the visual result is rejected, revisit art/material choices before gameplay backlog accumulates content debt.

## Integration Test Catalogue

### Epic-specific cases

1. Scene renders multiple generated 16³ terrain Chunks using generalized mesher with cube/slope/slab/stair cells.
2. Palette Material terrain remains stable while camera moves and has no unintended Chunk-boundary seam.
3. Imported small-scale barrel meshes through the same mesher and uses object-local Material sampling.
4. Articulated Hero renders from multiple small-scale VoxelModels and animates part transforms without remesh.
5. Attached voxel weapon follows hand transform and reuses the same Material/mesh infrastructure.
6. Changing Hero/weapon transforms and Material palette leaves mesher invocation count unchanged.
7. A deliberate voxel edit to barrel or test Chunk invalidates/remeshes only required volume(s)/neighbors.
8. Profiling captures mesh counts, triangles, draw calls, build times and cache reuse for the complete scene.

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

### ST-033-01 — Palette Material terrain prototype

Render terrain with a minimal palette-based Material while retaining semantic Shape slots.

**Acceptance cases**

- [ ] Grass/stone/wood-like fixtures are distinguishable without relying on camera-dependent randomness.
- [ ] World-continuous variation has no Chunk seam in the fixture.
- [ ] Current atlas fallback can be toggled/compared during migration.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-033-02 — Object-local Material prototype

Render imported barrel/weapon with stable local palette pattern/variation.

**Acceptance cases**

- [ ] Moving/rotating object does not make its pattern slide through geometry.
- [ ] Camera movement does not change palette assignment.
- [ ] Material parameter change does not remesh.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-033-03 — Unified scene assembly

Build the required terrain + barrel + articulated Hero + weapon scene using the common volume/mesher path.

**Acceptance cases**

- [ ] Runtime telemetry proves all voxel geometry was produced by EP-032.
- [ ] Terrain resolver and standalone resolver are both exercised.
- [ ] Hero transform animation runs with zero geometry rebuild after warm-up.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-033-04 — Visual/performance decision record

Capture images/profiling and decide whether the multi-scale voxel/Material direction is acceptable before broad content production.

**Acceptance cases**

- [ ] Decision record includes visual strengths/problems and measured costs.
- [ ] Rejected Material sampling options and reasons are recorded.
- [ ] Any follow-up optimization is driven by measured bottleneck rather than speculative architecture.
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
