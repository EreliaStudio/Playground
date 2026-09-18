# EP-014 — Spell Items, Loadouts, Requirements, and Formulas

**Meta-epic:** ME-004  
**Horizon:** H1  
**GDD / architecture coverage:** GDD spell items/loadout/formulas  
**Dependencies:** EP-013, EP-018

## Goal

Make spells physical tradable items, equip exactly eight per Hero, validate equipment Type/Tag requirements, and execute constrained data-driven formulas.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Spell item has persistent ownership and can be traded/stored.
2. Hero exposes exactly eight equipped spell slots.
3. Equipment Type/Tag requirements are validated from current loadout.
4. Tags themselves have no intrinsic effect.
5. Formula evaluator exposes only allowed variables/functions.
6. Malformed/cyclic/unsupported formula content fails validation.
7. Formula result is deterministic for same authoritative inputs.
8. Equipping/unequipping a spell does not alter voxel meshes unless the presentation asset itself explicitly changes cell geometry.

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

### ST-014-01 — Spell item schema

Represent spells as physical item definitions/instances.

**Acceptance cases**

- [ ] The spell item schema nominal fixture succeeds through the public/domain contract.
- [ ] The spell item schema boundary/error fixture follows documented validation without partial mutation.
- [ ] The spell item schema result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-014-02 — Eight-slot loadout

Equip and validate exactly eight spell slots.

**Acceptance cases**

- [ ] The eight-slot loadout nominal fixture succeeds through the public/domain contract.
- [ ] The eight-slot loadout boundary/error fixture follows documented validation without partial mutation.
- [ ] The eight-slot loadout result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-014-03 — Equipment requirements

Evaluate Type/Tag requirements against equipped items.

**Acceptance cases**

- [ ] The equipment requirements nominal fixture succeeds through the public/domain contract.
- [ ] The equipment requirements boundary/error fixture follows documented validation without partial mutation.
- [ ] The equipment requirements result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-014-04 — Constrained formulas

Parse/validate/evaluate data-driven spell formulas.

**Acceptance cases**

- [ ] The constrained formulas nominal fixture succeeds through the public/domain contract.
- [ ] The constrained formulas boundary/error fixture follows documented validation without partial mutation.
- [ ] The constrained formulas result remains compatible with deterministic/headless execution where applicable.
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
