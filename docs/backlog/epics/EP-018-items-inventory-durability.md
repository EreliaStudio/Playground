# EP-018 — Items, Equipment, Expedition Inventory, Bank, and Durability

**Meta-epic:** ME-004  
**Horizon:** H1  
**GDD / architecture coverage:** GDD items/equipment/inventory/bank/durability  
**Dependencies:** EP-007, EP-024

## Goal

Represent persistent physical items with ownership/location, equipment slot rules, stats/Type/Tags, fixed-size expedition inventory, global bank, and configurable durability lifecycle.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. One item has exactly one authoritative owner/location.
2. Equipment validates Main/Off Hand, armor/accessory slots and two-handed occupancy.
3. Item has exactly one Type and zero or more Tags.
4. Fixed-size expedition inventory rejects overflow atomically.
5. Bank moves preserve item identity and ownership.
6. Durability reaching zero permanently destroys equipment per configured rule.
7. Repair changes durability according to decision-gated rules without duplicating item.
8. Visual item VoxelModel/material presentation can vary independently of the authoritative item stat instance.

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

### ST-018-01 — Item identity and ownership

Create persistent item instances and one-location invariant.

**Acceptance cases**

- [ ] The item identity and ownership nominal fixture succeeds through the public/domain contract.
- [ ] The item identity and ownership boundary/error fixture follows documented validation without partial mutation.
- [ ] The item identity and ownership result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-018-02 — Equipment slots

Equip/unequip with hand occupancy validation.

**Acceptance cases**

- [ ] The equipment slots nominal fixture succeeds through the public/domain contract.
- [ ] The equipment slots boundary/error fixture follows documented validation without partial mutation.
- [ ] The equipment slots result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-018-03 — Expedition inventory and bank

Move items atomically between player storage locations.

**Acceptance cases**

- [ ] The expedition inventory and bank nominal fixture succeeds through the public/domain contract.
- [ ] The expedition inventory and bank boundary/error fixture follows documented validation without partial mutation.
- [ ] The expedition inventory and bank result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-018-04 — Durability lifecycle

Damage, repair and destroy equipment using configurable rules.

**Acceptance cases**

- [ ] The durability lifecycle nominal fixture succeeds through the public/domain contract.
- [ ] The durability lifecycle boundary/error fixture follows documented validation without partial mutation.
- [ ] The durability lifecycle result remains compatible with deterministic/headless execution where applicable.
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
