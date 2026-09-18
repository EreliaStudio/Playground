# EP-013 — Combat Stats, Damage, Criticals, and Effects Vocabulary

**Meta-epic:** ME-003  
**Horizon:** H1  
**GDD / architecture coverage:** GDD stats/damage  
**Dependencies:** EP-000

## Goal

Provide bounded arithmetic and effect primitives shared by Heroes, enemies, spells, statuses, and summons.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Configured stat values serialize deterministically.
2. Damage pipeline applies formula -> raw -> crit -> defense -> final in documented order.
3. Defense uses selected diminishing-return formula once decision gate is resolved.
4. Crit behavior is deterministic under injected RNG.
5. No implicit Mana resource exists in baseline.
6. Effect vocabulary rejects unknown operations in content.
7. Boundary/overflow inputs fail or clamp only according to explicit contract.
8. Summons/enemies can use same statistic primitives as Heroes.

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

### ST-013-01 — Combat statistic set

Define configurable authoritative statistic container.

**Acceptance cases**

- [ ] The combat statistic set nominal fixture succeeds through the public/domain contract.
- [ ] The combat statistic set boundary/error fixture follows documented validation without partial mutation.
- [ ] The combat statistic set result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-013-02 — Damage pipeline

Implement ordered damage computation.

**Acceptance cases**

- [ ] The damage pipeline nominal fixture succeeds through the public/domain contract.
- [ ] The damage pipeline boundary/error fixture follows documented validation without partial mutation.
- [ ] The damage pipeline result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-013-03 — Critical resolution

Inject deterministic RNG and critical modifiers.

**Acceptance cases**

- [ ] The critical resolution nominal fixture succeeds through the public/domain contract.
- [ ] The critical resolution boundary/error fixture follows documented validation without partial mutation.
- [ ] The critical resolution result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-013-04 — Effect vocabulary

Define bounded reusable gameplay effect operations.

**Acceptance cases**

- [ ] The effect vocabulary nominal fixture succeeds through the public/domain contract.
- [ ] The effect vocabulary boundary/error fixture follows documented validation without partial mutation.
- [ ] The effect vocabulary result remains compatible with deterministic/headless execution where applicable.
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
