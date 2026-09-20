# EP-032 — Unified Voxel Mesher and Occlusion Context

**Dependencies:** EP-000, EP-001  
**Dependency semantics:** only the named contracts are required; this is not a mandatory waterfall requiring every earlier epic to be globally complete.

## Purpose

Implement one generalized, headless, scale-aware VoxelMesher and its narrow ChunkMesher outside-neighbor specialization while retaining the current Baker for a later consumer transition.

## Starting state

- Contracts supplied by `EP-000, EP-001` exist at the narrowest story/API level needed by this epic.
- Authoritative state is valid, deterministic test fixtures are available, and unresolved choices remain explicit decision gates.
- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.
- The base VoxelMesher must not depend on `Chunk::Collection`, entity types, streaming or authoring formats; only the EP-004 ChunkMesher subclass may hold Chunk lookup context.
- Start by preserving current textured output/UV/material-slot behavior; Material migration is EP-003/033.
- Voxel scale multiplies positions only; visibility remains discrete-grid logic.
- Shape normalization, current orientation and vertical flip semantics remain unchanged.
- Current partial-neighbor/occlusion behavior is regression-locked before any improvement.
- EP-001 current-texture PNG references must pass unchanged after mesher extraction, alongside semantic mesh parity; see [ARCH-004](../../architecture/ARCH-004-TEST-STRATEGY.md#textured-chunk-image-regression-gate).
- A fixed-size Chunk optimization may exist behind the common read contract but must not create a second mesher.
- Meshing is CPU/headless data transformation; GPU upload is EP-003.

## Target state

- Every subject in the coverage matrix has an implementation owner and executable acceptance evidence.
- Invalid operations fail before partial mutation; deterministic inputs reproduce state, events, and visible output where applicable.
- Ticket-specific user inputs and decisions are recorded rather than invented by implementers.

## Scope

- Generic cached occlusion inside VoxelMesher
- Protected outside-volume neighbor hook with standalone outside-is-empty behavior
- ChunkMesher specialization point without a second meshing algorithm
- MaterialResolver per emitted polygon
- Shape expansion, visibility, deterministic vertex/index and palette-element output

## Out of scope

- Responsibilities explicitly assigned to neighboring epics.
- Final choices tracked by unresolved Open Decisions.
- Unapproved balance values, content, serialization syntax, or technology selections.

## Architecture / behavioral overview

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.
- The base VoxelMesher must not depend on `Chunk::Collection`, entity types, streaming or authoring formats; only the EP-004 ChunkMesher subclass may hold Chunk lookup context.
- Start by preserving current textured output/UV/material-slot behavior; Material migration is EP-003/033.
- Voxel scale multiplies positions only; visibility remains discrete-grid logic.
- Shape normalization, current orientation and vertical flip semantics remain unchanged.
- Current partial-neighbor/occlusion behavior is regression-locked before any improvement.
- EP-001 current-texture PNG references must pass unchanged after mesher extraction, alongside semantic mesh parity; see [ARCH-004](../../architecture/ARCH-004-TEST-STRATEGY.md#textured-chunk-image-regression-gate).
- A fixed-size Chunk optimization may exist behind the common read contract but must not create a second mesher.
- Meshing is CPU/headless data transformation; GPU upload is EP-003.

## Subjects / capabilities covered

| Subject / capability | Implemented by tickets | Tested by tickets |
|---|---|---|
| Current Baker semantic and textured-image characterization | [ST-032-01](tickets/ST-032-01-current-baker-semantic-golden-fixtures.md) | [ST-032-01](tickets/ST-032-01-current-baker-semantic-golden-fixtures.md) |
| Shared iteration, Shape/scale transforms, cached occlusion, base boundary behavior, and Chunk specialization | [ST-032-02](tickets/ST-032-02-generic-cell-iteration-and-scale-aware-shape-transform.md) | [ST-032-02](tickets/ST-032-02-generic-cell-iteration-and-scale-aware-shape-transform.md) |
| MaterialResolver and deterministic vertex/index/palette-element output | [ST-032-04](tickets/ST-032-04-optimization-and-indexed-output.md) | [ST-032-04](tickets/ST-032-04-optimization-and-indexed-output.md) |

## Ticket index

- [ST-032-01 — Current Baker semantic golden fixtures](tickets/ST-032-01-current-baker-semantic-golden-fixtures.md) — Capture current mesh semantics before code extraction and build comparison helpers tolerant of harmless vertex ordering differences.
- [ST-032-02 — Unified VoxelMesher and ChunkMesher implementation](tickets/ST-032-02-generic-cell-iteration-and-scale-aware-shape-transform.md) — Consolidates generic iteration/scale, cached occlusion, default boundary behavior, Chunk specialization, numerical evidence, and golden fixtures.
- [ST-032-03 — Superseded by consolidated ST-032-02](tickets/ST-032-03-occlusionresolver-meshing-context.md) — Stable historical link; owns no remaining implementation.
- [ST-032-04 — Optimization and indexed output](tickets/ST-032-04-optimization-and-indexed-output.md) — Preserve/remove hidden surfaces and emit efficient indexed mesh sections while profiling against the current Baker.

## Epic integration acceptance tests

1. Golden current Chunk fixture produces semantically equivalent geometry/material data through old Baker and new VoxelMesher.
2. Solid adjacent cube cells emit no internal shared face.
3. Slab/slope/stair/cross fixtures preserve current transformed geometry, normals and occlusion behavior.
4. Orientation and vertical flip yield equivalent results to current Baker behavior.
5. Standalone volume treats outside-grid neighbors as empty.
6. Chunk context resolves outside-grid neighbor from adjacent Chunk and preserves cross-Chunk occlusion.
7. Identical cell arrangements at scale 1.0 and 0.1 have equivalent topology/material assignments while vertex positions/bounds scale by 10×.
8. The checked-in JSON cross statue and equivalent procedural raw volume use exactly the same mesher implementation and Definition/Shape catalog resolution as a Chunk.

Additional integration invariants:

- Invalid input is rejected without partial state, event, resource, or scheduling mutation.
- Exact supported boundaries and one-step-outside values are exercised using explicit fixture configuration.
- Same seed/configuration/snapshot/commands produce the same result and ordering.
- Create → use → unload → restore/recreate is free of stale registrations and duplicate effects.
- Persistent state, where owned, preserves semantics and stable IDs across save/load and retry.
- Rendering comparisons supplement and never replace semantic/value assertions.

## Decision gates

- [OD-020](../../open-decisions/OD-020-mesher-merging-indexing-and-hard-normal-policy.md) — Status at ticket authoring: Open. The fixed contracts in this ticket may proceed; behavior requiring the final choice remains blocked.
- [OD-025](../../open-decisions/OD-025-voxelmesher-chunk-specialization-and-occlusion-cache.md) — Resolved: one generic cached VoxelMesher algorithm with an outside-volume hook; ChunkMesher overrides only that hook.

## Rendering validation

Graphical tickets use OD-024's deterministic `640 × 480` Sparkle TestLibrary comparisons. Structural refactors preserve reviewed baselines. New references remain candidates until actual/difference images receive human approval.

## Required user-provided inputs

- None beyond linked decisions and ticket-specific gates.

## Epic exit criteria

- [ ] Every ticket's behavioral, boundary, rejection, determinism, lifecycle, integration, rendering, and evidence sections pass.
- [ ] Every coverage-matrix row has implementation and test evidence.
- [ ] Applicable decisions are resolved in their persistent OD files, with provenance.
- [ ] No graphical baseline was replaced solely because a comparison failed.
