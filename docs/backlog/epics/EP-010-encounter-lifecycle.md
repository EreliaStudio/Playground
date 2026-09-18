# EP-010 — Encounter Creation, Handoff, Surprise, and Reinforcements

**Meta-epic:** ME-003  
**Horizon:** H1  
**GDD / architecture coverage:** GDD encounter handoff/surprise/reinforcement  
**Dependencies:** EP-006, EP-009

## Goal

Create tactical encounters from live world state, synchronize initial participants, determine Surprise, and admit later entities as zero-readiness reinforcements.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Initial participants are locked exactly once during handoff.
2. Uncommitted targeting is cancelled while committed actions finish.
3. Final world positions are snapshotted before combat cells are derived.
4. Each participant leaves Region Clock ownership and enters exactly one Encounter Clock atomically.
5. Unaware defender state grants the documented surprise readiness to the attacking side.
6. Later arrivals do not restart initial synchronization.
7. Reinforcement readiness begins at zero/fresh TurnInterval.
8. Failed handoff cannot strand an entity between world and encounter ownership.

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

### ST-010-01 — Encounter trigger and participant lock

Determine initial participants and create encounter identity.

**Acceptance cases**

- [ ] The encounter trigger and participant lock nominal fixture succeeds through the public/domain contract.
- [ ] The encounter trigger and participant lock boundary/error fixture follows documented validation without partial mutation.
- [ ] The encounter trigger and participant lock result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-010-02 — World-to-encounter handoff

Complete/cancel world actions and transfer time-domain ownership.

**Acceptance cases**

- [ ] The world-to-encounter handoff nominal fixture succeeds through the public/domain contract.
- [ ] The world-to-encounter handoff boundary/error fixture follows documented validation without partial mutation.
- [ ] The world-to-encounter handoff result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-010-03 — Surprise initialization

Initialize readiness based on awareness relationship.

**Acceptance cases**

- [ ] The surprise initialization nominal fixture succeeds through the public/domain contract.
- [ ] The surprise initialization boundary/error fixture follows documented validation without partial mutation.
- [ ] The surprise initialization result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-010-04 — Reinforcement admission

Admit later valid participants without restarting the encounter.

**Acceptance cases**

- [ ] The reinforcement admission nominal fixture succeeds through the public/domain contract.
- [ ] The reinforcement admission boundary/error fixture follows documented validation without partial mutation.
- [ ] The reinforcement admission result remains compatible with deterministic/headless execution where applicable.
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
