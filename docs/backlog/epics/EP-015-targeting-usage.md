# EP-015 — Spell Targeting, LoS, AoE, and Usage Restrictions

**Meta-epic:** ME-003  
**Horizon:** H1  
**GDD / architecture coverage:** GDD targeting/restrictions  
**Dependencies:** EP-011, EP-012, EP-014

## Goal

Resolve valid tactical targets using 3D combat cells, range rules, line of sight, AoE patterns, AP cost, cooldown, and usage limits.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Min/max range boundaries are exact.
2. Fixed-range spells ignore only the modifiers they explicitly forbid.
3. LoS uses authoritative combat/world geometry contract, not rendered pixels.
4. Linear/diagonal restrictions accept/reject expected cell vectors.
5. AoE resolves deterministic target cell sets.
6. AP/cooldown/uses-per-turn/target/encounter are all enforced before mutation.
7. Invalid target command spends no AP and consumes no usage counter.
8. Target preview and authoritative resolution share rules but preview has no authority.

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

### ST-015-01 — Range and target categories

Validate selectable target cells/entities.

**Acceptance cases**

- [ ] The range and target categories nominal fixture succeeds through the public/domain contract.
- [ ] The range and target categories boundary/error fixture follows documented validation without partial mutation.
- [ ] The range and target categories result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-015-02 — Line of sight

Resolve obstruction in 3D cell space.

**Acceptance cases**

- [ ] The line of sight nominal fixture succeeds through the public/domain contract.
- [ ] The line of sight boundary/error fixture follows documented validation without partial mutation.
- [ ] The line of sight result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-015-03 — AoE patterns

Generate affected cells/targets deterministically.

**Acceptance cases**

- [ ] The aoe patterns nominal fixture succeeds through the public/domain contract.
- [ ] The aoe patterns boundary/error fixture follows documented validation without partial mutation.
- [ ] The aoe patterns result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-015-04 — Usage restrictions

Track AP, cooldown and use limits atomically.

**Acceptance cases**

- [ ] The usage restrictions nominal fixture succeeds through the public/domain contract.
- [ ] The usage restrictions boundary/error fixture follows documented validation without partial mutation.
- [ ] The usage restrictions result remains compatible with deterministic/headless execution where applicable.
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
