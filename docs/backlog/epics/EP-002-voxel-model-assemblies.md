# EP-002 — Voxel Models, Assemblies, Animation, and Mesh Reuse

**Meta-epic:** ME-001  
**Horizon:** H0→H1  
**GDD / architecture coverage:** Hero/enemy/prop/equipment visual representation on top of the unified voxel volume  
**Dependencies:** EP-001, EP-030, EP-032

## Goal

Use runtime-sized VoxelModel volumes as reusable visual assets and compose articulated characters from rigid model parts whose cached meshes are transformed rather than rebuilt during animation.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.
- A VoxelModel is cell data + scale/metadata, not a second geometry/meshing format.
- Transform hierarchy, pivot, attachment and clip metadata are separate from the cell volume.
- Animation moves rigid parts; it does not normally mutate voxel cells.
- Equipment is an independent VoxelModel attached to hierarchy points.
- Detailed model voxels are not automatically authoritative collision or tactical cells.
- Mesh caching keys must include all geometry-affecting inputs and exclude transform/material-only state.

## Integration Test Catalogue

### Epic-specific cases

1. A barrel model and one Chunk both mesh through EP-032 with the same Definition/Shape resolution path.
2. Reusing one VoxelModel asset across many entities reuses cached mesh data rather than duplicating geometry per transform.
3. Changing entity/world transform never invokes the VoxelMesher.
4. Playing a walk/attack clip over rigid-part transforms never invokes the VoxelMesher.
5. Editing one part's voxel cells invalidates/remeshes that part without rebuilding unrelated Hero parts.
6. Sword/shield/helmet model attachments follow named parent transforms and can be swapped independently.
7. Visual part hierarchy changes cannot modify authoritative HP/AP/collision/spell validity.
8. Missing part, cyclic hierarchy or missing attachment point fails predictably before publishing the assembly.

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

### ST-002-01 — VoxelModel asset contract

Define stable identity, VoxelVolume data, pivot and optional metadata for imported/runtime model assets.

**Acceptance cases**

- [ ] Model identity is independent of display name/path.
- [ ] Pivot is expressed consistently in model-local coordinates.
- [ ] Loading a VoxelModel requires no renderer or GPU.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-002-02 — Rigid articulated assembly

Represent parent/child part hierarchy for torso/head/limbs using transforms over independent VoxelModels.

**Acceptance cases**

- [ ] Hierarchy evaluation produces deterministic world/local part transforms.
- [ ] Cycles are rejected.
- [ ] Reparenting/pose changes do not edit part cell data.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-002-03 — Transform animation clips

Define clips over articulated transforms with explicit duration/loop and optional presentation-only markers.

**Acceptance cases**

- [ ] A walk cycle changes part transforms with stable timing.
- [ ] Clip interruption leaves a valid pose and attachment hierarchy.
- [ ] Reaching an animation marker cannot authoritatively apply gameplay damage/effects.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-002-04 — Equipment and mesh reuse

Attach independent equipment VoxelModels and cache meshes across entities/poses.

**Acceptance cases**

- [ ] Sword/shield swap changes attachment asset without remeshing the body.
- [ ] Same model asset at multiple transforms shares one geometry cache entry.
- [ ] A voxel content version change invalidates the relevant mesh cache key.
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
