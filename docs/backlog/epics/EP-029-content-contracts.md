# EP-029 — Unified Content Schemas and Voxel Asset Contract

**Meta-epic:** ME-007  
**Horizon:** H0→H3  
**GDD / architecture coverage:** Versioned official content, voxel definitions/materials/imported models and future editor compatibility  
**Dependencies:** EP-000

## Goal

Treat official content as versioned validated data, including the unified VoxelVolume model assets, Definition/Shape catalogs, Materials, articulation metadata and import manifests, so runtime and external conversion tools share supported contracts without privileged shortcuts.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.
- Chunk generated cells and imported VoxelModel cells use the same stable Voxel::Cell/Definition identity semantics.
- Do not standardize one external authoring file as the runtime data model; importers are adapters.
- Derived mesh cache data is reproducible and may be invalidated by source/definition/mesher version.
- Current JSON Shape/Definition content remains readable during migration.
- Schema evolution is explicit and deterministic; no silent reinterpretation of old cells.
- Gameplay content cannot smuggle executable arbitrary code through Material/formula schemas.

## Integration Test Catalogue

### Epic-specific cases

1. Existing Shape/Definition fixtures validate under the revised content pipeline.
2. A runtime-sized model volume serializes/reloads dimensions, voxelSize and packed cells without semantic change.
3. Stable Definition IDs referenced by imported model cells resolve identically after content reload.
4. Unknown schema version or missing Definition fails before partial asset publication.
5. Derived mesh cache is rejected/rebuilt when its source/Definition/mesher version is incompatible.
6. Articulation/attachment metadata detects cycles/missing part IDs before runtime use.
7. Material sampling-space/configuration fields round-trip deterministically.
8. Headless server can load authoritative gameplay content without loading presentation-only model/material GPU data.

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

### ST-029-01 — Versioned voxel volume/model schema

Define versioned runtime content for arbitrary dimensions, uniform voxelSize and packed Voxel::Cell data.

**Acceptance cases**

- [ ] Dimensions and cell count are validated against overflow/limits.
- [ ] voxelSize rejects zero/negative/non-finite values.
- [ ] Round-trip preserves exact packed Cell values.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-029-02 — Definition/Shape and Material schema migration

Evolve current atlas-bound definitions toward Material bindings without breaking current content fixtures.

**Acceptance cases**

- [ ] Existing texture-slot content has an explicit compatibility path.
- [ ] New Material-bound definition validates required Shape slots.
- [ ] Content never depends on renderer-internal GPU handles.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-029-03 — Articulation/import manifests

Version imported asset mapping, part hierarchy, pivots/attachments and source provenance.

**Acceptance cases**

- [ ] Source tool/file metadata is diagnostic/provenance, not stable runtime identity.
- [ ] Missing/cyclic part references block publication.
- [ ] Import is reproducible for fixed source + mapping + converter version.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-029-04 — Derived mesh-cache contract

Allow optional serialized/cacheable mesher output without making it the authoritative model representation.

**Acceptance cases**

- [ ] Cache key/version includes geometry-affecting source inputs.
- [ ] Cache deletion never destroys source VoxelVolume asset.
- [ ] Runtime can regenerate cache through the same VoxelMesher when policy permits.
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
