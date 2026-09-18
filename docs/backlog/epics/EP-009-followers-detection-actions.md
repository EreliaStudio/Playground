# EP-009 — Followers, Formation, Detection, and Exploration Actions

**Meta-epic:** ME-002  
**Horizon:** H1  
**GDD / architecture coverage:** GDD followers/detection/jump-climb  
**Dependencies:** EP-005, EP-008

## Goal

Make non-controlled squad Heroes physically follow/stay, reproduce special traversal where possible, and participate in vision-driven enemy detection/chase.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Followers occupy real world positions and can trigger detection/traps.
2. Follow/hold changes behavior without teleporting followers.
3. Follower traversal attempts valid actions and holds if impossible.
4. Vision detection uses authoritative positions/occlusion inputs.
5. Detection transitions to chase before encounter handoff.
6. Explicit jump/climb targeting does not pause Region Clock.
7. Encounter start cancels uncommitted targeting according to handoff rules.
8. Follower visuals are independent of navigation state.

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

### ST-009-01 — Follow/hold behavior

Implement physical follower locomotion modes.

**Acceptance cases**

- [ ] The follow/hold behavior nominal fixture succeeds through the public/domain contract.
- [ ] The follow/hold behavior boundary/error fixture follows documented validation without partial mutation.
- [ ] The follow/hold behavior result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-009-02 — Vision detection

Implement initial enemy awareness by vision.

**Acceptance cases**

- [ ] The vision detection nominal fixture succeeds through the public/domain contract.
- [ ] The vision detection boundary/error fixture follows documented validation without partial mutation.
- [ ] The vision detection result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-009-03 — Real-time chase

Transition detected enemies into pursuit before engagement.

**Acceptance cases**

- [ ] The real-time chase nominal fixture succeeds through the public/domain contract.
- [ ] The real-time chase boundary/error fixture follows documented validation without partial mutation.
- [ ] The real-time chase result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-009-04 — Explicit traversal actions

Implement jump/climb target selection and follower replication rules.

**Acceptance cases**

- [ ] The explicit traversal actions nominal fixture succeeds through the public/domain contract.
- [ ] The explicit traversal actions boundary/error fixture follows documented validation without partial mutation.
- [ ] The explicit traversal actions result remains compatible with deterministic/headless execution where applicable.
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
