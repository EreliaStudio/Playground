# EP-030 — External Voxel Asset Import and Volume Conversion

**Meta-epic:** ME-008  
**Horizon:** H0  
**GDD / architecture coverage:** Asset production for props, Hero parts, equipment and other small-scale voxel volumes  
**Dependencies:** EP-001, EP-029

## Goal

Choose a practical mature voxel/3D authoring workflow and implement the smallest headless importer/converter that maps authored cells into the unified VoxelModel representation without creating a custom modeling program.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.
- Tool/format selection is evidence-driven and replaceable; no editor becomes an engine dependency.
- Import output is VoxelVolume cell data, not a special pre-baked model mesh format.
- Cube-centric external formats are acceptable initially; Erelia Shapes remain richer than the source format.
- Definition/material mapping is explicit and deterministic.
- Coordinate axes/origin, palette mapping and scale are never guessed silently.
- Importer can run headlessly in CI/build tooling.

## Integration Test Catalogue

### Epic-specific cases

1. Tool/format spike compares at least viable candidates against documented requirements and records the selected first path.
2. A small barrel authored externally imports to expected dimensions, voxelSize and Cell coordinates.
3. Imported palette/color entries map deterministically to configured Voxel::Definition IDs.
4. Axis/origin conversion fixture produces expected orientation with no mirror/rotation surprise.
5. Invalid/oversized source or unknown mapping fails without replacing last valid asset.
6. Re-import of identical source/mapping/converter version produces semantically identical VoxelModel.
7. Imported VoxelModel meshes through EP-032 with no model-specific mesher.
8. Importer executes without Sparkle Window/Surface/OpenGL and does not require a custom editor executable.

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

### ST-030-01 — Authoring tool/format evaluation

Evaluate mature existing voxel/3D tools and common formats for Erelia model production.

**Acceptance cases**

- [ ] Decision record covers licensing, format documentation, grid/palette support, hierarchy options and headless conversion.
- [ ] Rejected options include concrete reasons.
- [ ] Decision does not require modifying the terrain voxel format.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-030-02 — Basic VoxelModel importer

Import dimensions/cells/palette mapping into EP-001 runtime-sized VoxelModel.

**Acceptance cases**

- [ ] Empty and occupied coordinates match a golden fixture.
- [ ] Imported voxelSize is explicit configuration/metadata.
- [ ] Unknown palette-to-Definition mapping blocks publication.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-030-03 — Coordinate and Definition mapping

Make axis/origin/palette/Definition conversion explicit and testable.

**Acceptance cases**

- [ ] Source coordinate handedness/axes map to documented Erelia local axes.
- [ ] Mapping supports reuse of existing cube Definitions.
- [ ] Future richer Shape assignment can be added through mapping/sidecar without changing VoxelVolume.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-030-04 — Headless conversion pipeline

Integrate import/validation into build/content workflow with stable diagnostics and atomic output.

**Acceptance cases**

- [ ] CLI/build action returns nonzero on invalid source.
- [ ] Successful conversion emits version/provenance metadata.
- [ ] Failed conversion leaves previous valid converted asset intact.
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
