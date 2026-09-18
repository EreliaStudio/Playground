# EP-024 — Authoritative Domain Server and Persistence

**Meta-epic:** ME-006  
**Horizon:** H1→H2  
**GDD / architecture coverage:** GDD authoritative server/persistence  
**Dependencies:** EP-000, EP-004, EP-006

## Goal

Make important shared state authoritative behind command validation, with durable snapshots/transactions and a headless execution path that can first run locally then in a dedicated server process.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Accounts/inventory/Heroes/progression/infrastructure/resources/camps/market/dungeons/encounters/rewards mutate only through authority boundary.
2. Rejected commands leave no partial persistent state.
3. Stable IDs and schema versions survive restart.
4. Snapshot/journal strategy can restore a consistent state after controlled failure fixture.
5. Server can run without rendering or external asset-authoring/import tooling.
6. Client-provided visual/material data cannot grant gameplay properties.
7. Idempotency/retry policy prevents duplicate transactional effects where applicable.
8. Authoritative state hash excludes presentation-only material animation.

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

### ST-024-01 — Authoritative command boundary

Validate actor ownership/preconditions before domain mutation.

**Acceptance cases**

- [ ] The authoritative command boundary nominal fixture succeeds through the public/domain contract.
- [ ] The authoritative command boundary boundary/error fixture follows documented validation without partial mutation.
- [ ] The authoritative command boundary result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-024-02 — Persistence snapshot schema

Version and persist shared domain state.

**Acceptance cases**

- [ ] The persistence snapshot schema nominal fixture succeeds through the public/domain contract.
- [ ] The persistence snapshot schema boundary/error fixture follows documented validation without partial mutation.
- [ ] The persistence snapshot schema result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-024-03 — Transactional mutations

Group multi-object authoritative changes atomically.

**Acceptance cases**

- [ ] The transactional mutations nominal fixture succeeds through the public/domain contract.
- [ ] The transactional mutations boundary/error fixture follows documented validation without partial mutation.
- [ ] The transactional mutations result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-024-04 — Local-to-dedicated host boundary

Keep authority services runnable in-process first and separately later.

**Acceptance cases**

- [ ] The local-to-dedicated host boundary nominal fixture succeeds through the public/domain contract.
- [ ] The local-to-dedicated host boundary boundary/error fixture follows documented validation without partial mutation.
- [ ] The local-to-dedicated host boundary result remains compatible with deterministic/headless execution where applicable.
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
