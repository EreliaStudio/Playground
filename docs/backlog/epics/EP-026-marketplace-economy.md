# EP-026 — Marketplace, Gold Economy, Bank Transactions, and PvE Procurement

**Meta-epic:** ME-004  
**Horizon:** H1-thin→H3  
**GDD / architecture coverage:** GDD gold/marketplace/PvE procurement  
**Dependencies:** EP-018, EP-024

## Goal

Implement a server-wide marketplace and gold ledger, then allow PvE vaults/camps to buy eligible player-crafted listings within a reward budget without minting finished items.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Integration Test Catalogue

### Epic-specific cases

1. Gold changes only through authoritative ledger operations.
2. Listing transfers/reserves item without duplicating ownership.
3. Purchase transfers item and gold atomically.
4. Cancelled/expired listing returns ownership correctly.
5. PvE reward source cannot exceed configured budget.
6. Procured reward corresponds to a real eligible listing and seller receives gold.
7. Pricing-abuse policy remains a decision gate but interface supports eligibility/fair-price filter.
8. Concurrent purchase/procurement of same listing resolves at most once.

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

### ST-026-01 — Gold ledger

Track universal currency through auditable authoritative transactions.

**Acceptance cases**

- [ ] The gold ledger nominal fixture succeeds through the public/domain contract.
- [ ] The gold ledger boundary/error fixture follows documented validation without partial mutation.
- [ ] The gold ledger result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-026-02 — Marketplace listing lifecycle

Create/cancel/purchase player listings.

**Acceptance cases**

- [ ] The marketplace listing lifecycle nominal fixture succeeds through the public/domain contract.
- [ ] The marketplace listing lifecycle boundary/error fixture follows documented validation without partial mutation.
- [ ] The marketplace listing lifecycle result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-026-03 — Reward budget

Represent funded PvE reward-source budget.

**Acceptance cases**

- [ ] The reward budget nominal fixture succeeds through the public/domain contract.
- [ ] The reward budget boundary/error fixture follows documented validation without partial mutation.
- [ ] The reward budget result remains compatible with deterministic/headless execution where applicable.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-026-04 — PvE procurement

Select and buy eligible crafted listings for reward placement.

**Acceptance cases**

- [ ] The pve procurement nominal fixture succeeds through the public/domain contract.
- [ ] The pve procurement boundary/error fixture follows documented validation without partial mutation.
- [ ] The pve procurement result remains compatible with deterministic/headless execution where applicable.
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
