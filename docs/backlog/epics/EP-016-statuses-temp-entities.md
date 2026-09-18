# EP-016 — Statuses, Combat Hooks, Temporary Entities, and Summons

**Meta-epic:** ME-003  
**Horizon:** H1  
**GDD / architecture coverage:** GDD statuses/hooks/temp entities/summons  
**Dependencies:** EP-013, EP-015

## Goal

Provide event-driven statuses and encounter-owned temporary entities/summons that reuse normal combat primitives without modifying immutable terrain.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Status hooks run only on subscribed authoritative events.
2. EverySecond uses Encounter Clock semantics.
3. Temporary walls/zones/traps do not mutate persistent terrain cells.
4. All encounter-owned temporary entities are cleaned up at encounter end.
5. Summons use normal CombatUnit readiness/stats/targeting pipeline.
6. Removing a status stops future hooks without leaving callbacks registered.
7. Hook ordering is deterministic/documented.
8. Presentation-only model/material effects can visualize statuses without becoming the status truth.

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

### ST-016-01 — Status lifecycle

Apply, refresh/remove and serialize statuses.

**Acceptance cases**

- [ ] The status lifecycle nominal fixture succeeds through the public/domain contract.
- [ ] The status lifecycle boundary/error fixture follows documented validation without partial mutation.
- [ ] The status lifecycle result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-016-02 — Combat event hooks

Dispatch bounded status triggers deterministically.

**Acceptance cases**

- [ ] The combat event hooks nominal fixture succeeds through the public/domain contract.
- [ ] The combat event hooks boundary/error fixture follows documented validation without partial mutation.
- [ ] The combat event hooks result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-016-03 — Temporary entities

Create encounter-owned walls/zones/traps/totems/portals/turrets.

**Acceptance cases**

- [ ] The temporary entities nominal fixture succeeds through the public/domain contract.
- [ ] The temporary entities boundary/error fixture follows documented validation without partial mutation.
- [ ] The temporary entities result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-016-04 — Summons

Create temporary CombatUnits through shared combat pipeline.

**Acceptance cases**

- [ ] The summons nominal fixture succeeds through the public/domain contract.
- [ ] The summons boundary/error fixture follows documented validation without partial mutation.
- [ ] The summons result remains compatible with deterministic/headless execution where applicable.
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
