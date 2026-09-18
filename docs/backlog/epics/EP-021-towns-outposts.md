# EP-021 — Permanent Towns, Building Progression, Outposts, and Respawn Services

**Meta-epic:** ME-005  
**Horizon:** H1  
**GDD / architecture coverage:** GDD towns/outposts/respawn  
**Dependencies:** EP-004, EP-019, EP-020, EP-024

## Goal

Implement shared permanent infrastructure with independent building upgrades and predefined public outposts that consume upkeep and expose configured services.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Town buildings upgrade independently with no Town Level aggregate.
2. Service availability derives from building state.
3. Outposts activate only at predefined valid crystal sites.
4. Outpost upkeep consumes configured resources and deactivates at zero.
5. Public outpost service state is shared server-wide.
6. Respawn Point selection is persistent and valid.
7. Infrastructure visual models can change by state/upgrade without modifying rules unexpectedly.
8. Concurrent contributions/upgrades are transactionally safe.

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

### ST-021-01 — Town/building state

Represent permanent towns and independent building upgrades.

**Acceptance cases**

- [ ] The town/building state nominal fixture succeeds through the public/domain contract.
- [ ] The town/building state boundary/error fixture follows documented validation without partial mutation.
- [ ] The town/building state result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-021-02 — Building services

Expose crafting/refining/enchantment/bank/etc. from configuration.

**Acceptance cases**

- [ ] The building services nominal fixture succeeds through the public/domain contract.
- [ ] The building services boundary/error fixture follows documented validation without partial mutation.
- [ ] The building services result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-021-03 — Outpost lifecycle

Activate, supply and deactivate temporary public outposts.

**Acceptance cases**

- [ ] The outpost lifecycle nominal fixture succeeds through the public/domain contract.
- [ ] The outpost lifecycle boundary/error fixture follows documented validation without partial mutation.
- [ ] The outpost lifecycle result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-021-04 — Respawn services

Select/use valid persistent Respawn Points.

**Acceptance cases**

- [ ] The respawn services nominal fixture succeeds through the public/domain contract.
- [ ] The respawn services boundary/error fixture follows documented validation without partial mutation.
- [ ] The respawn services result remains compatible with deterministic/headless execution where applicable.
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
