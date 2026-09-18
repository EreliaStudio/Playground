# EP-003 — Material Rendering, Mesh Runtime, and Caching

**Meta-epic:** ME-001  
**Horizon:** H0  
**GDD / architecture coverage:** Render path shared by chunk and model meshes; material migration and cache behavior  
**Dependencies:** EP-001, EP-032

## Goal

Render meshes produced by the generalized VoxelMesher through one ordinary Sparkle mesh/material path, while introducing the Material abstraction incrementally and preserving current atlas rendering until parity is proven.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.
- Renderer consumes meshes/material references, not Chunk or Hero-specific geometry formats.
- Avoid one draw call per voxel; volumes are meshed into exterior/indexed geometry.
- Keep current `TextureMesh3D`/atlas path available during mesher extraction so terrain regression is attributable.
- Material/palette changes do not dirty geometry when slot topology is unchanged.
- Material sampling space is explicit and stable for moving objects versus world terrain.
- Dynamic visual effects remain presentation-only.

## Integration Test Catalogue

### Epic-specific cases

1. Current textured chunk fixture renders through generalized mesher with equivalent Shape transforms/material-slot/UV semantics.
2. One generalized mesh representation can be rendered for both a Chunk-derived mesh and a VoxelModel-derived mesh.
3. Same VoxelModel mesh is reused by multiple transforms/entities.
4. Palette/material parameter changes do not call the VoxelMesher.
5. World-space terrain variation remains continuous across Chunk boundaries where configured.
6. Object-local material variation moves with a barrel/sword and does not swim as the object moves through world space.
7. Moving the camera alone cannot change deterministic palette-cell assignment/pattern phase.
8. Cache unload/reload releases GPU resources without invalidating headless volume/model data.

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

### ST-003-01 — Common mesh upload/cache path

Upload/index/cache VoxelMesher output independent of whether the source was Chunk or VoxelModel.

**Acceptance cases**

- [ ] Cache key includes geometry/definition/mesher versions required for correctness.
- [ ] Transform is instance state, not mesh-cache identity.
- [ ] Cache miss/hit behavior is profileable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-003-02 — Material abstraction beside atlas path

Introduce semantic Material bindings without requiring immediate removal of current texture/UV rendering.

**Acceptance cases**

- [ ] Existing atlas-backed Definition fixture continues to render during migration.
- [ ] A base-color/palette Material can render a new definition without changing mesher logic.
- [ ] Missing required material binding fails visibly/predictably rather than sampling garbage.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-003-03 — Sampling spaces and palette variation

Prototype deterministic palette variation with explicit world/object/local sampling intent.

**Acceptance cases**

- [ ] Two adjacent terrain Chunks show no artificial pattern seam in continuous-world mode.
- [ ] A moving object retains object-local pattern phase.
- [ ] Camera motion does not cause pattern swimming.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-003-04 — Presentation material effects

Support per-instance tint/effect parameters and leave room for emissive/metallic/transparency later.

**Acceptance cases**

- [ ] Damage flash/tint changes instance rendering without editing shared Material asset.
- [ ] Base-color-only Material remains valid.
- [ ] Effect time/state is excluded from authoritative simulation hashes.
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
