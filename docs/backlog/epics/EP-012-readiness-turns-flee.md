# EP-012 — Readiness, Turns, AP/MP, and Flee

**Meta-epic:** ME-003  
**Horizon:** H1  
**GDD / architecture coverage:** GDD readiness/AP/MP/flee/defeat  
**Dependencies:** EP-006, EP-010, EP-011

## Goal

Implement TurnInterval-based readiness, turn resources, decision pauses, turn completion, per-Hero fleeing, defeat recovery, and return to world control.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Lower TurnInterval reaches readiness sooner under same elapsed encounter time.
2. Only the deciding encounter pauses; Region Clock/other encounters continue.
3. AP and MP are refreshed/consumed according to configured rules.
4. A Hero can flee independently while owner remains in fight with other Heroes.
5. Owner fully detaches only when all owned participants have fled/been defeated.
6. Re-entry follows reinforcement rules.
7. Defeated Heroes recover at 1 HP after battle per baseline.
8. Full team defeat returns to last Respawn Point without default currency/durability/XP penalty.

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

### ST-012-01 — Readiness accumulation

Advance readiness from Encounter Clock and TurnInterval.

**Acceptance cases**

- [ ] The readiness accumulation nominal fixture succeeds through the public/domain contract.
- [ ] The readiness accumulation boundary/error fixture follows documented validation without partial mutation.
- [ ] The readiness accumulation result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-012-02 — Turn resources

Initialize/spend/end AP and MP.

**Acceptance cases**

- [ ] The turn resources nominal fixture succeeds through the public/domain contract.
- [ ] The turn resources boundary/error fixture follows documented validation without partial mutation.
- [ ] The turn resources result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-012-03 — Per-Hero flee

Resolve explicit flee attempts and outside-wait state.

**Acceptance cases**

- [ ] The per-hero flee nominal fixture succeeds through the public/domain contract.
- [ ] The per-hero flee boundary/error fixture follows documented validation without partial mutation.
- [ ] The per-hero flee result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-012-04 — Defeat and encounter exit

Resolve defeated participants and world return.

**Acceptance cases**

- [ ] The defeat and encounter exit nominal fixture succeeds through the public/domain contract.
- [ ] The defeat and encounter exit boundary/error fixture follows documented validation without partial mutation.
- [ ] The defeat and encounter exit result remains compatible with deterministic/headless execution where applicable.
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
