# EP-020 — Refining, Crafting, and Enchantment

**Meta-epic:** ME-004  
**Horizon:** H1  
**GDD / architecture coverage:** GDD refining/crafting/enchantment  
**Dependencies:** EP-018, EP-019, EP-021

## Goal

Convert resources into refined materials and player-made finished items, then apply deterministic enchantment transformations with authoritative RNG success and explicit costs.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Refining consumes recipe inputs atomically and creates configured outputs.
2. Higher tiers can require increasing raw material quantities without profession levels.
3. Crafting rolls configured stat ranges through authoritative deterministic RNG source.
4. Finished persistent equipment/spells originate from crafting/trading path, not ordinary monster generation.
5. Enchantment intended result is deterministic for selected operation.
6. Failure consumes configured cost and leaves target item unchanged in base rule.
7. Tag add/remove/replace enforces tradeoffs/type changes explicitly.
8. Crafted item visual/material variant can be selected without remeshing geometry when its voxel cells are unchanged.

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

### ST-020-01 — Refining recipes

Convert raw to refined resources.

**Acceptance cases**

- [ ] The refining recipes nominal fixture succeeds through the public/domain contract.
- [ ] The refining recipes boundary/error fixture follows documented validation without partial mutation.
- [ ] The refining recipes result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-020-02 — Crafting recipes and stat rolls

Create finished item instances from recipes and configured ranges.

**Acceptance cases**

- [ ] The crafting recipes and stat rolls nominal fixture succeeds through the public/domain contract.
- [ ] The crafting recipes and stat rolls boundary/error fixture follows documented validation without partial mutation.
- [ ] The crafting recipes and stat rolls result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-020-03 — Enchantment operations

Define deterministic tag/stat/type transformations.

**Acceptance cases**

- [ ] The enchantment operations nominal fixture succeeds through the public/domain contract.
- [ ] The enchantment operations boundary/error fixture follows documented validation without partial mutation.
- [ ] The enchantment operations result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-020-04 — Enchantment resolution

Consume costs and resolve success/failure atomically.

**Acceptance cases**

- [ ] The enchantment resolution nominal fixture succeeds through the public/domain contract.
- [ ] The enchantment resolution boundary/error fixture follows documented validation without partial mutation.
- [ ] The enchantment resolution result remains compatible with deterministic/headless execution where applicable.
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
