# EP-005 — Voxel Traversal Graph and Surface Extraction

**Meta-epic:** ME-002  
**Horizon:** H0→H1  
**GDD / architecture coverage:** GDD exploration/combat traversal derived from immutable voxel world geometry  
**Dependencies:** EP-001, EP-004

## Goal

Derive navigation/traversable surfaces from authoritative world Chunk voxel geometry while ensuring the new small-scale VoxelModel representation does not accidentally become navigation or collision truth.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.
- Traversal operates on world/Chunk semantics, not every VoxelVolume instance.
- Entity model voxels, equipment and visual animation do not automatically create world traversal nodes.
- Existing Shape geometry/coverage/orientation/flip can inform surface extraction at world scale.
- Material/color/effect changes never alter traversal.
- Multiple traversable surfaces in one X/Z column remain supported where world geometry permits.

## Integration Test Catalogue

### Epic-specific cases

1. Existing cube/slab/slope/stair world fixtures yield expected traversable surfaces after unified volume refactor.
2. Rotated/flipped world cells produce correspondingly transformed traversal geometry.
3. Two surfaces at different Y positions in one X/Z column remain distinct.
4. A visually identical Chunk at a different hypothetical voxel scale changes world dimensions consistently only when explicitly used as world geometry.
5. Adding a decorative barrel VoxelModel does not create traversal cells unless an explicit world collision/obstacle system registers it.
6. Hero limb/equipment VoxelModels never alter terrain navigation during animation.
7. Material palette changes have zero traversal effect.
8. Chunk unload/reload regenerates equivalent traversal for identical authoritative cells.

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

### ST-005-01 — World surface extraction parity

Protect current/expected shape-based traversable-surface extraction through the voxel refactor.

**Acceptance cases**

- [ ] Cube, slab, slope and stair fixtures are represented.
- [ ] Orientation/flip affect surface geometry correctly.
- [ ] Cross-plane vegetation does not become a traversable floor by default.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-005-02 — Cardinal connectivity graph

Connect compatible neighboring world surfaces with height/action metadata needed by exploration/combat.

**Acceptance cases**

- [ ] Cardinal neighbor edges respect configured traversal constraints.
- [ ] Multiple heights per column remain addressable.
- [ ] Deterministic Chunk order does not change graph result.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-005-03 — Explicit dynamic obstacle boundary

Keep movable/model entities separate from static terrain graph while allowing explicit blockers later.

**Acceptance cases**

- [ ] Spawning a visual prop alone does not mutate the base graph.
- [ ] Explicit blocker registration can be layered without rewriting VoxelModel cells.
- [ ] Removing a blocker restores prior traversal semantics.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-005-04 — Combat-cell projection from traversal

Expose valid world surfaces to encounter combat-cell construction without using render meshes as authority.

**Acceptance cases**

- [ ] Combat cells retain 1×1 horizontal world footprint for baseline terrain.
- [ ] Render/material cache loss does not remove authoritative combat cells.
- [ ] Model voxel resolution is irrelevant to combat-cell dimensions.
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
