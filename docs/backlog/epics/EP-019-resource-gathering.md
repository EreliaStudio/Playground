# EP-019 — Shared Resource Nodes and Gathering

**Meta-epic:** ME-004  
**Horizon:** H1  
**GDD / architecture coverage:** GDD gathering/resource nodes  
**Dependencies:** EP-004, EP-018, EP-024

## Goal

Generate shared resource spawn points whose availability is authoritative server state, gate gathering by tool family/tier, and respawn nodes without modifying terrain.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. One node depletion is visible to all players sharing server state.
2. Gather validation checks required tool/tier.
3. Successful gather transfers resources atomically then marks node depleted.
4. Terrain VoxelDefinitions are unchanged by depletion.
5. Respawn timer uses authoritative world time.
6. Concurrent gather attempts cannot duplicate yield.
7. Node visual material/model may change with state without changing terrain.
8. Save/reload preserves depletion/respawn schedule.

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

### ST-019-01 — Resource-site generation

Place deterministic valid spawn sites independent of availability state.

**Acceptance cases**

- [ ] The resource-site generation nominal fixture succeeds through the public/domain contract.
- [ ] The resource-site generation boundary/error fixture follows documented validation without partial mutation.
- [ ] The resource-site generation result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-019-02 — Gather command

Validate tool/tier and grant yield atomically.

**Acceptance cases**

- [ ] The gather command nominal fixture succeeds through the public/domain contract.
- [ ] The gather command boundary/error fixture follows documented validation without partial mutation.
- [ ] The gather command result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-019-03 — Shared depletion

Represent server-wide depleted/available state.

**Acceptance cases**

- [ ] The shared depletion nominal fixture succeeds through the public/domain contract.
- [ ] The shared depletion boundary/error fixture follows documented validation without partial mutation.
- [ ] The shared depletion result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-019-04 — Respawn scheduling

Restore availability after configured world-clock duration.

**Acceptance cases**

- [ ] The respawn scheduling nominal fixture succeeds through the public/domain contract.
- [ ] The respawn scheduling boundary/error fixture follows documented validation without partial mutation.
- [ ] The respawn scheduling result remains compatible with deterministic/headless execution where applicable.
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
