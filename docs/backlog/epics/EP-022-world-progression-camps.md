# EP-022 — Worlds, Portals, Frontier Progression, and Enemy Camps

**Meta-epic:** ME-005  
**Horizon:** H1  
**GDD / architecture coverage:** GDD Worlds/portal/camps  
**Dependencies:** EP-004, EP-021, EP-024

## Goal

Represent multiple deterministic Worlds per server, permanent server-wide next-World portal unlocks, sparse civilization, tiered content, and rotating camps at predefined valid sites.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. WorldSeed derives from ServerSeed + WorldID.
2. First Grand Dungeon progression event permanently activates next-World portal exactly once.
3. Later completions remain repeatable without duplicating progression mutation.
4. Camp sites are predefined/generated valid locations separate from active selection.
5. Clearing a camp rewards once then deactivates it.
6. Replacement camp activation follows configured deterministic/server policy.
7. World progression survives restart.
8. Portal/camp visual models do not define authoritative activation state.

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

### ST-022-01 — World catalog and seeds

Represent multiple World identities and generation parameters.

**Acceptance cases**

- [ ] The world catalog and seeds nominal fixture succeeds through the public/domain contract.
- [ ] The world catalog and seeds boundary/error fixture follows documented validation without partial mutation.
- [ ] The world catalog and seeds result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-022-02 — Portal progression state

Persist server-wide unlocked transitions.

**Acceptance cases**

- [ ] The portal progression state nominal fixture succeeds through the public/domain contract.
- [ ] The portal progression state boundary/error fixture follows documented validation without partial mutation.
- [ ] The portal progression state result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-022-03 — Camp site catalog

Generate/store valid potential camp locations.

**Acceptance cases**

- [ ] The camp site catalog nominal fixture succeeds through the public/domain contract.
- [ ] The camp site catalog boundary/error fixture follows documented validation without partial mutation.
- [ ] The camp site catalog result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-022-04 — Camp rotation lifecycle

Activate, clear, reward and replace overworld camps.

**Acceptance cases**

- [ ] The camp rotation lifecycle nominal fixture succeeds through the public/domain contract.
- [ ] The camp rotation lifecycle boundary/error fixture follows documented validation without partial mutation.
- [ ] The camp rotation lifecycle result remains compatible with deterministic/headless execution where applicable.
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
