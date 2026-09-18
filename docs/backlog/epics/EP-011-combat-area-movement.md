# EP-011 — Combat Cells, Elastic Area, and Tactical Movement

**Meta-epic:** ME-003  
**Horizon:** H1  
**GDD / architecture coverage:** GDD combat cells/elastic radius  
**Dependencies:** EP-005, EP-010

## Goal

Build cell-based tactical movement directly from current voxel traversal surfaces and maintain the elastic combat area around participants.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Combat cell positions correspond to valid traversable voxel surfaces.
2. Cells retain 1x1 horizontal gameplay footprint regardless of visual VoxelModel resolution/voxel scale.
3. Elastic area equals union of configured radius around combatants.
4. Adding/removing/moving combatants updates area deterministically.
5. Movement cannot target invalid/unreachable cells.
6. Cell graph respects slope/slab traversal metadata.
7. World base terrain remains immutable during combat.
8. Rendering mesh detail does not create extra tactical cells.

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

### ST-011-01 — Combat-cell extraction

Convert traversal surfaces into encounter cells.

**Acceptance cases**

- [ ] The combat-cell extraction nominal fixture succeeds through the public/domain contract.
- [ ] The combat-cell extraction boundary/error fixture follows documented validation without partial mutation.
- [ ] The combat-cell extraction result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-011-02 — Elastic area membership

Maintain combat area around participants.

**Acceptance cases**

- [ ] The elastic area membership nominal fixture succeeds through the public/domain contract.
- [ ] The elastic area membership boundary/error fixture follows documented validation without partial mutation.
- [ ] The elastic area membership result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-011-03 — Tactical pathing

Resolve MP-cost movement through valid combat cells.

**Acceptance cases**

- [ ] The tactical pathing nominal fixture succeeds through the public/domain contract.
- [ ] The tactical pathing boundary/error fixture follows documented validation without partial mutation.
- [ ] The tactical pathing result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-011-04 — Movement execution

Commit movement and events through Encounter Clock rules.

**Acceptance cases**

- [ ] The movement execution nominal fixture succeeds through the public/domain contract.
- [ ] The movement execution boundary/error fixture follows documented validation without partial mutation.
- [ ] The movement execution result remains compatible with deterministic/headless execution where applicable.
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
