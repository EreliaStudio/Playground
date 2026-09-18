# EP-031 — Articulated Voxel Character Assembly and Transform Animation

**Meta-epic:** ME-008  
**Horizon:** H0→H1  
**GDD / architecture coverage:** Visual Hero/enemy assembly, equipment and animation without voxel remeshing per frame  
**Dependencies:** EP-002, EP-030

## Goal

Use externally authored rigid VoxelModel parts, hierarchy/pivots and transform animation to produce the first practical Hero/enemy workflow without skeletal vertex skinning or custom animation software.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.
- Each part remains an independent VoxelModel using the shared mesher.
- Hierarchy/animation metadata may come from Blender or a simple sidecar; exact tool is a decision gate.
- Runtime animation changes transforms, not cell geometry.
- Equipment uses attachment points and independent VoxelModels.
- Gameplay collision is explicit and not recomputed from animated visual voxels.
- Do not require every armor slot to have unique visible geometry in H0.

## Integration Test Catalogue

### Epic-specific cases

1. Torso/head/upper+lower limbs import and assemble into one Hero hierarchy.
2. Idle/walk clip transforms parts without any VoxelMesher call after meshes are ready.
3. Attack/spell pose can rotate hierarchy while weapon remains attached to expected point.
4. Swapping sword/shield assets does not rebuild Hero body meshes.
5. Two Heroes can share part/model meshes while having independent poses/transforms.
6. Animation interruption/cross-clip switch never creates a cyclic/invalid hierarchy.
7. Visual pose change does not change authoritative collision unless explicit gameplay logic updates it.
8. Missing part/pivot/attachment metadata fails at validation/load boundary with useful diagnostics.

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

### ST-031-01 — Character part hierarchy metadata

Define parent/child part references, pivots and local bind transforms independent of voxel cell storage.

**Acceptance cases**

- [ ] A canonical Hero hierarchy validates without renderer.
- [ ] Cycle/missing parent is rejected.
- [ ] Part asset reuse is supported.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-031-02 — Transform animation authoring/import

Use mature tooling or declarative clips to author rigid-part transform keyframes.

**Acceptance cases**

- [ ] Clip timing and transforms round-trip deterministically.
- [ ] No vertex skin weights are required.
- [ ] Playback does not edit VoxelVolume cells.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-031-03 — Equipment attachments

Attach weapon/shield/helmet or other visible equipment to named hierarchy points.

**Acceptance cases**

- [ ] Attachment follows parent animation.
- [ ] Equip/unequip is an asset/reference operation, not body remesh.
- [ ] Missing attachment point has explicit failure/fallback policy.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-031-04 — First Hero/enemy production fixture

Produce a representative Hero and enemy using the chosen external workflow and shared runtime.

**Acceptance cases**

- [ ] Fixture can be rebuilt from documented source files/tool steps.
- [ ] Both use EP-032 for geometry.
- [ ] Profiling records part count, triangles, draw calls and cache reuse.
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
