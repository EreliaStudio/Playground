# EP-004 — Deterministic Chunk World Runtime

**Meta-epic:** ME-002  
**Horizon:** H0→H1  
**GDD / architecture coverage:** GDD deterministic immutable Worlds; current Chunk/Collection/Generator/streaming behavior  
**Dependencies:** EP-001, EP-032

## Goal

Preserve the current headless 16³ Chunk world runtime while adapting Chunk to the unified VoxelVolume contract and delegating geometry generation to the common VoxelMesher through a Chunk-aware neighbor resolver.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.
- Chunk remains a semantic world specialization with 16³ cells and voxelSize 1.0 for the baseline world.
- Generation, streaming, collection membership and world/local coordinate conversion remain outside generic volume code.
- Same ServerSeed + WorldID + generation/content version yields the same immutable base cells.
- `Chunk::Collection` remains the source of cross-Chunk world cell availability; VoxelMesher does not depend on it directly.
- World runtime remains headless and does not own GPU meshes.
- Current Chunk editing/version/bake scheduling behavior is preserved or migrated under explicit tests.

## Integration Test Catalogue

### Epic-specific cases

1. Existing stable Chunk generation fixture is unchanged by adapting Chunk to the volume contract.
2. Chunk dimensions remain exactly 16³ and baseline voxelSize is exactly 1 world unit.
3. Chunk resolver maps an out-of-bounds local neighbor to the correct adjacent Chunk cell when available.
4. Missing/unavailable neighboring Chunk follows the documented current/fallback visibility policy deterministically.
5. Replacing `Chunk::Baker` use with VoxelMesher + Chunk resolver preserves representative current mesh semantics.
6. Same seed/content/generation version regenerates the same base Chunk cells after unload/reload.
7. World generation and Chunk collection run without Material/GPU/model-import dependencies.
8. Editing one Chunk increments/version-dirties it and required neighbor bake scheduling without mutating immutable generated source rules.

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

### ST-004-01 — Chunk-to-volume adaptation

Expose current Chunk through EP-001 without changing 16³ storage or world helpers.

**Acceptance cases**

- [ ] Current `at`, `cells`, `index`, `contains` behavior remains covered.
- [ ] Chunk reports voxelSize 1.0 through meshing-facing access.
- [ ] Generic volume code never requires Chunk coordinate.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-004-02 — Chunk neighbor resolver

Implement meshing context that resolves local out-of-bounds neighbors through Chunk::Collection.

**Acceptance cases**

- [ ] All six boundary directions resolve correct world coordinates.
- [ ] Adjacent available Chunk cell is returned exactly.
- [ ] Standalone model resolver behavior is not hard-coded into Chunk resolver.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-004-03 — Deterministic World generation/streaming

Keep seeded generation, request/publish/remove and regeneration behavior independent of rendering.

**Acceptance cases**

- [ ] Same seed/version regenerates same cells.
- [ ] Pending/available/absent transitions reject invalid lifecycle operations.
- [ ] Removing a Chunk releases world ownership without invalidating unrelated model assets.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-004-04 — Bake scheduling after unified mesher

Route Chunk mesh requests through EP-032 while preserving dirty/version and neighbor invalidation semantics.

**Acceptance cases**

- [ ] A changed boundary cell causes every required affected Chunk mesh to be refreshed.
- [ ] Unchanged version can reuse mesh/cache according to scheduler policy.
- [ ] Meshing failure leaves last valid render state or explicit missing state per policy.
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
