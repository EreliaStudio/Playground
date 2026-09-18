# EP-025 — Dungeon Completion, Grand Dungeon, and Reward Vaults

**Meta-epic:** ME-005  
**Horizon:** H1  
**GDD / architecture coverage:** GDD dungeon completion/Grand Dungeon/vault  
**Dependencies:** EP-022, EP-023, EP-026

## Goal

Complete dungeon runs through explicit objectives/vaults, keep Grand Dungeon structurally consistent with normal dungeons, and trigger the server-wide portal reward once.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Completion only occurs after configured objective criteria.
2. Vault becomes available only after completion.
3. Reward claim is idempotent per run/claim policy.
4. Grand Dungeon reuses ordinary dungeon instance/generation contracts.
5. First successful Grand Dungeon completion emits exactly one progression unlock mutation.
6. Later completions still produce normal repeat rewards.
7. Leaving/resetting dungeon clears runtime state but not server progression.
8. Reward presentation asset cannot change reward contents.

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

### ST-025-01 — Dungeon completion state

Track objectives and completed run state.

**Acceptance cases**

- [ ] The dungeon completion state nominal fixture succeeds through the public/domain contract.
- [ ] The dungeon completion state boundary/error fixture follows documented validation without partial mutation.
- [ ] The dungeon completion state result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-025-02 — Vault lifecycle

Unlock and claim generated rewards.

**Acceptance cases**

- [ ] The vault lifecycle nominal fixture succeeds through the public/domain contract.
- [ ] The vault lifecycle boundary/error fixture follows documented validation without partial mutation.
- [ ] The vault lifecycle result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-025-03 — Grand Dungeon specialization

Configure Grand Dungeon through normal dungeon framework.

**Acceptance cases**

- [ ] The grand dungeon specialization nominal fixture succeeds through the public/domain contract.
- [ ] The grand dungeon specialization boundary/error fixture follows documented validation without partial mutation.
- [ ] The grand dungeon specialization result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-025-04 — First-clear progression trigger

Atomically connect first success to portal unlock.

**Acceptance cases**

- [ ] The first-clear progression trigger nominal fixture succeeds through the public/domain contract.
- [ ] The first-clear progression trigger boundary/error fixture follows documented validation without partial mutation.
- [ ] The first-clear progression trigger result remains compatible with deterministic/headless execution where applicable.
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
