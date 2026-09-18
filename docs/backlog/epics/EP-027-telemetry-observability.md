# EP-027 — Telemetry, Profiling, and Operational Observability

**Meta-epic:** ME-007  
**Horizon:** H0→H3  
**GDD / architecture coverage:** GDD telemetry/operations  
**Dependencies:** EP-000

## Goal

Emit authoritative gameplay telemetry and technical performance measurements without coupling simulation correctness to analytics availability.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Gameplay events originate from authoritative accepted outcomes.
2. Telemetry failure never rolls back otherwise valid gameplay unless explicitly configured.
3. Events use stable versioned schemas/IDs.
4. PII/secrets are not placed in generic event payloads.
5. Profiler can measure model compile/load/chunk bake/render preparation separately.
6. Dropped/backpressured analytics follows documented policy.
7. Offline/local runs can disable telemetry.
8. Event ordering/time domain is explicit enough for later analysis.

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

### ST-027-01 — Gameplay event envelope

Define versioned authoritative event metadata.

**Acceptance cases**

- [ ] The gameplay event envelope nominal fixture succeeds through the public/domain contract.
- [ ] The gameplay event envelope boundary/error fixture follows documented validation without partial mutation.
- [ ] The gameplay event envelope result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-027-02 — Domain event families

Emit combat/economy/dungeon/world/etc. telemetry.

**Acceptance cases**

- [ ] The domain event families nominal fixture succeeds through the public/domain contract.
- [ ] The domain event families boundary/error fixture follows documented validation without partial mutation.
- [ ] The domain event families result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-027-03 — Technical profiling

Instrument CPU/GPU/content pipeline/runtime hotspots.

**Acceptance cases**

- [ ] The technical profiling nominal fixture succeeds through the public/domain contract.
- [ ] The technical profiling boundary/error fixture follows documented validation without partial mutation.
- [ ] The technical profiling result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-027-04 — Telemetry sink boundary

Buffer/export events without coupling to gameplay success.

**Acceptance cases**

- [ ] The telemetry sink boundary nominal fixture succeeds through the public/domain contract.
- [ ] The telemetry sink boundary boundary/error fixture follows documented validation without partial mutation.
- [ ] The telemetry sink boundary result remains compatible with deterministic/headless execution where applicable.
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
