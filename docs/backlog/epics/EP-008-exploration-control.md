# EP-008 — Third-Person Movement and Camera

**Meta-epic:** ME-002  
**Horizon:** H1  
**GDD / architecture coverage:** GDD exploration control  
**Dependencies:** EP-004, EP-005, EP-007

## Goal

Deliver continuous third-person control for the selected Hero using character-controller collision against voxel terrain, with camera transitions to tactical targeting views.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Movement consumes continuous input rather than node-to-node commands.
2. Character controller collides with authoritative world collision, not articulated visual VoxelModel meshes by default.
3. Camera state does not affect authoritative movement validity.
4. Changing visual animation pose cannot move the authoritative collision shape unless explicit gameplay logic updates it.
5. Movement is deterministic enough under the chosen authoritative/prediction contract.
6. Blocked movement cannot tunnel through fixture geometry.
7. Top-down targeting view can be entered/exited without pausing Region Clock.
8. Headless movement validation works without camera.

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

### ST-008-01 — Character controller

Implement authoritative-friendly continuous movement/collision.

**Acceptance cases**

- [ ] The character controller nominal fixture succeeds through the public/domain contract.
- [ ] The character controller boundary/error fixture follows documented validation without partial mutation.
- [ ] The character controller result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-008-02 — Third-person camera

Implement exploration follow/orbit camera.

**Acceptance cases**

- [ ] The third-person camera nominal fixture succeeds through the public/domain contract.
- [ ] The third-person camera boundary/error fixture follows documented validation without partial mutation.
- [ ] The third-person camera result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-008-03 — Movement command contract

Separate sampled client intent from authoritative movement result.

**Acceptance cases**

- [ ] The movement command contract nominal fixture succeeds through the public/domain contract.
- [ ] The movement command contract boundary/error fixture follows documented validation without partial mutation.
- [ ] The movement command contract result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-008-04 — Top-down targeting transition

Switch presentation mode for explicit exploration actions.

**Acceptance cases**

- [ ] The top-down targeting transition nominal fixture succeeds through the public/domain contract.
- [ ] The top-down targeting transition boundary/error fixture follows documented validation without partial mutation.
- [ ] The top-down targeting transition result remains compatible with deterministic/headless execution where applicable.
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
