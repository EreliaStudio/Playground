# EP-000 — Test Harness and CI Quality Gates

**Meta-epic:** ME-007  
**Horizon:** H0  
**GDD / architecture coverage:** Cross-cutting implementation requirement  
**Dependencies:** None

## Goal

Create the executable quality infrastructure used by every later Erelia feature: unit/component tests, integration fixtures, deterministic replay, headless simulation, asset-validation tests, and performance regression fixtures.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.

## Unified voxel regression responsibility

Before EP-032 replaces the current Chunk::Baker path, EP-000 must provide deterministic semantic mesh-comparison fixtures for current cube/slab/slope/stair/cross and cross-Chunk behavior. These are regression gates, not screenshot-only tests.

## Integration Test Catalogue

### Epic-specific cases

1. A clean checkout can configure, build, and execute the complete automated test suite from one documented command path.
2. Headless tests execute without creating a Sparkle Window, Surface, or OpenGL context.
3. A deterministic fixture can record an accepted command stream and replay it to the same terminal state hash.
4. Voxel-volume/mesher/import fixtures can compare semantic outputs against stable expectations rather than raw byte layout where layout is allowed to evolve.
5. A deliberately malformed content fixture fails validation with source-identifying diagnostics and no crash.
6. Performance fixtures report distributions for current Chunk bake, generalized VoxelMesher, imported-model load/mesh-cache, encounter step, and representative render preparation workloads.
7. Test data has stable seeds and versions so failures are reproducible locally and in CI.
8. A failing integration test prevents the owning epic from being considered Done.

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

### ST-000-01 — Unit and component test baseline

Standardize GoogleTest fixtures, naming, deterministic seeds, temporary directories, and failure diagnostics.

**Acceptance cases**

- [ ] Representative Sparkle-independent domain tests execute in CI.
- [ ] Tests can create isolated temporary content/runtime stores.
- [ ] Failure output identifies seed, fixture and stable object IDs.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-000-02 — Headless integration harness

Provide a reusable application/runtime harness that instantiates domain services without presentation services.

**Acceptance cases**

- [ ] The harness can bootstrap a world/domain service graph without OpenGL.
- [ ] Tests can advance deterministic clocks explicitly.
- [ ] Presentation services can be substituted with no-op test adapters.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-000-03 — Replay and state hashing

Add accepted-command recording, deterministic replay, and semantic state hashing for regression tests.

**Acceptance cases**

- [ ] A recorded fixture replays to the same state hash.
- [ ] Changing an authoritative state field changes the semantic hash.
- [ ] Presentation-only state is excluded from the authoritative hash.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

### ST-000-04 — Performance and soak fixtures

Provide fixed workloads and reporting for regressions, leaks, lifecycle loops, and long-running simulations.

**Acceptance cases**

- [ ] A fixture can execute repeated load/unload cycles.
- [ ] Timing reports use repeated samples rather than one elapsed-time value.
- [ ] A threshold breach is distinguishable from normal test failure.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.

## Textured Chunk visual-test support

Provide the SparkleTestLibrary capture/compare workflow required by ST-001-01, including a supported Windows/OpenGL test job, explicit Playground reference/result paths, and failure image artifacts. Verify build/link availability of `Sparkle::TestLibrary`; its in-source CMake alias does not by itself establish installed-package availability. Keep CPU-only execution independent. A skipped/unavailable GPU job does not satisfy the image-regression gate. Detailed fixture, tolerance and baseline rules are in [ARCH-004](../architecture/ARCH-004-TEST-STRATEGY.md#textured-chunk-image-regression-gate).

## Epic Exit Criteria

- [ ] All activated stories meet their acceptance cases.
- [ ] Epic-specific and common integration cases are automated and passing.
- [ ] Invalid/corrupt content or commands cannot leave impossible partial state.
- [ ] Relevant deterministic/headless fixtures pass.
- [ ] Public/runtime schemas and decision gates are documented.
- [ ] Profiling exists for any path expected to be performance-sensitive.
