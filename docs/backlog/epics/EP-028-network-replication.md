# EP-028 — Networking, Replication, Interest Management, and Client Reconciliation

**Meta-epic:** ME-006  
**Horizon:** H2  
**GDD / architecture coverage:** GDD multiplayer/server authority  
**Dependencies:** EP-024

## Goal

Connect remote clients to the authoritative host, replicate only relevant dynamic state, preserve deterministic chunk generation where possible, and reconcile predicted exploration without giving clients authority.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Client cannot authoritatively grant items/gold/progression/combat outcomes.
2. Interest management limits replicated dynamic entities to relevant regions/encounters.
3. Immutable deterministic chunk data may be generated locally only when content/generation versions match.
4. Mismatch falls back to authoritative compatible source policy.
5. Movement prediction/reconciliation does not alter server truth.
6. Duplicate/reordered/lost message fixtures preserve transactional invariants.
7. Reconnect restores current state without duplicating entities/items.
8. VoxelModel/assembly/material asset IDs replicate as presentation references, not trusted gameplay payloads.

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

### ST-028-01 — Connection/session identity

Authenticate/associate client session with player authority.

**Acceptance cases**

- [ ] The connection/session identity nominal fixture succeeds through the public/domain contract.
- [ ] The connection/session identity boundary/error fixture follows documented validation without partial mutation.
- [ ] The connection/session identity result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-028-02 — Replication snapshots/events

Serialize relevant dynamic authoritative state.

**Acceptance cases**

- [ ] The replication snapshots/events nominal fixture succeeds through the public/domain contract.
- [ ] The replication snapshots/events boundary/error fixture follows documented validation without partial mutation.
- [ ] The replication snapshots/events result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-028-03 — Interest management

Track region/encounter relevance.

**Acceptance cases**

- [ ] The interest management nominal fixture succeeds through the public/domain contract.
- [ ] The interest management boundary/error fixture follows documented validation without partial mutation.
- [ ] The interest management result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-028-04 — Prediction and reconciliation

Reconcile continuous exploration movement under latency.

**Acceptance cases**

- [ ] The prediction and reconciliation nominal fixture succeeds through the public/domain contract.
- [ ] The prediction and reconciliation boundary/error fixture follows documented validation without partial mutation.
- [ ] The prediction and reconciliation result remains compatible with deterministic/headless execution where applicable.
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
