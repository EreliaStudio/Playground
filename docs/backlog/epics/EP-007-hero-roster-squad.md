# EP-007 — Hero Roster, Active Squad, and Voxel Identity

**Meta-epic:** ME-004  
**Horizon:** H1  
**GDD / architecture coverage:** GDD Heroes/active squad  
**Dependencies:** EP-002, EP-024

## Goal

Represent unlimited blank Heroes per player, exactly three active expedition Heroes, and model-definition based Hero presentation independent from class/level systems.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. A player may own more than three Heroes while selecting exactly three active for an expedition when required.
2. No level/class/proficiency field is required to create a Hero.
3. Hero visual definition/palette is presentation content and does not imply combat stats.
4. Changing active squad does not duplicate/loss inventory ownership.
5. Multiplayer party composition preserves each player’s own active Heroes.
6. Stable Hero identity survives save/reload.
7. Invalid duplicate active selection is rejected.
8. Headless server can create/manage Heroes without loading GPU meshes.

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

### ST-007-01 — Hero identity and roster

Create persistent blank Hero records with stable ownership.

**Acceptance cases**

- [ ] The hero identity and roster nominal fixture succeeds through the public/domain contract.
- [ ] The hero identity and roster boundary/error fixture follows documented validation without partial mutation.
- [ ] The hero identity and roster result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-007-02 — Active squad selection

Select and validate expedition Heroes.

**Acceptance cases**

- [ ] The active squad selection nominal fixture succeeds through the public/domain contract.
- [ ] The active squad selection boundary/error fixture follows documented validation without partial mutation.
- [ ] The active squad selection result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-007-03 — Hero visual definition

Bind Hero identity to reusable articulated VoxelModel assembly and Material palette/reference set.

**Acceptance cases**

- [ ] The hero visual definition nominal fixture succeeds through the public/domain contract.
- [ ] The hero visual definition boundary/error fixture follows documented validation without partial mutation.
- [ ] The hero visual definition result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-007-04 — Party composition

Combine participating players’ active Heroes without transferring ownership.

**Acceptance cases**

- [ ] The party composition nominal fixture succeeds through the public/domain contract.
- [ ] The party composition boundary/error fixture follows documented validation without partial mutation.
- [ ] The party composition result remains compatible with deterministic/headless execution where applicable.
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
