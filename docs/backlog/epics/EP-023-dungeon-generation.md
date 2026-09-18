# EP-023 — Dungeon Rooms, Entrances, Generation, and Rotation

**Meta-epic:** ME-005  
**Horizon:** H1  
**GDD / architecture coverage:** GDD dungeon generation/rotation  
**Dependencies:** EP-004, EP-029, EP-032

## Goal

Create per-group dungeon instances from handcrafted voxel room modules, support multiple entrances/start locations, and generate deterministic layouts for a server rotation window.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Room modules load as validated voxel-room/content assets whose authoritative layout does not depend on GPU meshes.
2. Generation uses ServerSeed, RotationEpoch, RotationDuration and DungeonID deterministically.
3. Same rotation re-entry produces same structural layout.
4. Runtime encounter/loot state resets on re-entry as specified.
5. Next rotation can produce a different layout.
6. Multiple entrances map to distinct start positions in same generated instance.
7. Generation rejects incompatible room connectors rather than overlap/corrupt space.
8. Dungeon generation is headless and does not require GPU meshes/material resources.

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

### ST-023-01 — Room module contract

Define room geometry/metadata/connectors consumed by generation.

**Acceptance cases**

- [ ] The room module contract nominal fixture succeeds through the public/domain contract.
- [ ] The room module contract boundary/error fixture follows documented validation without partial mutation.
- [ ] The room module contract result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-023-02 — Deterministic assembly

Assemble valid room graph/layout using WFC/similar algorithm.

**Acceptance cases**

- [ ] The deterministic assembly nominal fixture succeeds through the public/domain contract.
- [ ] The deterministic assembly boundary/error fixture follows documented validation without partial mutation.
- [ ] The deterministic assembly result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-023-03 — Rotation key

Derive stable layout key for current server rotation.

**Acceptance cases**

- [ ] The rotation key nominal fixture succeeds through the public/domain contract.
- [ ] The rotation key boundary/error fixture follows documented validation without partial mutation.
- [ ] The rotation key result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-023-04 — Entrances and reset

Instantiate per-group runtime from selected entrance and reset ephemeral state.

**Acceptance cases**

- [ ] The entrances and reset nominal fixture succeeds through the public/domain contract.
- [ ] The entrances and reset boundary/error fixture follows documented validation without partial mutation.
- [ ] The entrances and reset result remains compatible with deterministic/headless execution where applicable.
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
