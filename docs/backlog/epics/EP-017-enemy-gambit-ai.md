# EP-017 — Enemy Combat Model and Gambit AI

**Meta-epic:** ME-003  
**Horizon:** H1  
**GDD / architecture coverage:** GDD enemy/Gambit AI  
**Dependencies:** EP-012, EP-015, EP-016

## Goal

Run enemies through the same combat model as Heroes while choosing tactical actions through serialized ordered Condition → Target → Reaction Gambit rules.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Enemy stats/spells do not require equipment ownership.
2. Gambit rules are evaluated in deterministic priority order.
3. Condition/target/reaction content is validated before runtime.
4. First valid rule chooses action according to documented semantics.
5. No valid rule resolves safe fallback/end-turn policy.
6. AI cannot bypass AP/targeting/cooldown rules.
7. Same state and deterministic RNG produce same chosen command.
8. Enemy visual model/palette has no influence on decision logic.

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

### ST-017-01 — Enemy combat definition

Define authored enemy stats/spells/visual reference.

**Acceptance cases**

- [ ] The enemy combat definition nominal fixture succeeds through the public/domain contract.
- [ ] The enemy combat definition boundary/error fixture follows documented validation without partial mutation.
- [ ] The enemy combat definition result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-017-02 — Gambit rule schema

Serialize and validate Condition → Target → Reaction entries.

**Acceptance cases**

- [ ] The gambit rule schema nominal fixture succeeds through the public/domain contract.
- [ ] The gambit rule schema boundary/error fixture follows documented validation without partial mutation.
- [ ] The gambit rule schema result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-017-03 — Gambit evaluation

Choose an action from ordered rules.

**Acceptance cases**

- [ ] The gambit evaluation nominal fixture succeeds through the public/domain contract.
- [ ] The gambit evaluation boundary/error fixture follows documented validation without partial mutation.
- [ ] The gambit evaluation result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-017-04 — AI command execution

Submit selected actions through normal combat command validation.

**Acceptance cases**

- [ ] The ai command execution nominal fixture succeeds through the public/domain contract.
- [ ] The ai command execution boundary/error fixture follows documented validation without partial mutation.
- [ ] The ai command execution result remains compatible with deterministic/headless execution where applicable.
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
