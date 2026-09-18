# EP-006 — World and Encounter Simulation Clocks

**Meta-epic:** ME-003  
**Horizon:** H0  
**GDD / architecture coverage:** GDD clocks/readiness  
**Dependencies:** EP-000

## Goal

Provide explicit deterministic time domains: a continuously advancing Region Clock plus independent Encounter Clocks that can pause for tactical decisions without stopping the world or other encounters.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Region Clock continues while one Encounter Clock is paused.
2. Two encounters advance independently.
3. Explicit fixed-step advancement is deterministic.
4. Entities belong to exactly one active time domain.
5. Transfer between time domains is atomic.
6. Clock serialization/reload preserves semantic scheduling.
7. Long-step and equivalent repeated short-step fixtures produce documented equivalent behavior where required.
8. Presentation frame rate cannot change authoritative clock advancement.

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

### ST-006-01 — Deterministic Region Clock

Implement explicit fixed-step Region Clock advancement.

**Acceptance cases**

- [ ] The deterministic region clock nominal fixture succeeds through the public/domain contract.
- [ ] The deterministic region clock boundary/error fixture follows documented validation without partial mutation.
- [ ] The deterministic region clock result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-006-02 — Independent Encounter Clock

Create encounter-owned clocks with pause/resume.

**Acceptance cases**

- [ ] The independent encounter clock nominal fixture succeeds through the public/domain contract.
- [ ] The independent encounter clock boundary/error fixture follows documented validation without partial mutation.
- [ ] The independent encounter clock result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-006-03 — Time-domain ownership

Track exactly one simulation clock owner per entity.

**Acceptance cases**

- [ ] The time-domain ownership nominal fixture succeeds through the public/domain contract.
- [ ] The time-domain ownership boundary/error fixture follows documented validation without partial mutation.
- [ ] The time-domain ownership result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-006-04 — Clock snapshot/replay

Serialize and replay clock state deterministically.

**Acceptance cases**

- [ ] The clock snapshot/replay nominal fixture succeeds through the public/domain contract.
- [ ] The clock snapshot/replay boundary/error fixture follows documented validation without partial mutation.
- [ ] The clock snapshot/replay result remains compatible with deterministic/headless execution where applicable.
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
