# ST-032-04 — Deterministic compatible-vertex reuse

## Intent

Reuse exactly compatible vertices in the common `VoxelMesher` output path without changing emitted polygons, triangle semantics, hard normals, atlas UVs, or rendered output.

## Execution decision policy

- Do not invent, infer, or silently select behavior, data, assets, values, formulas, schemas, ownership, error policy, architecture, or technology that this ticket or an already-resolved linked decision does not define.
- Treat every unspecified choice that can affect public behavior, compatibility, persistence, rendering, tests, or later tickets as an Open Decision.
- Stop before implementing the affected portion, ask the user a focused clarification question, and record the approved answer in the applicable OD file—or create/link a new OD record when none exists.
- Continue only with work that is independent of the missing decision.
- Select an implementation detail without clarification only when this ticket explicitly delegates it and the choice cannot alter an observable contract.

## Starting state / prerequisites

- Merged PR #7 removed `Chunk::Baker` and moved all production consumers to the nested `Chunk::Mesher` specialization over the common `VoxelMesher` algorithm.
- The pre-change `VoxelMesher` at merged main commit `67d5471208d7da4188bb1034bc69d32a69eb1441`, the retained ST-032-01 semantic snapshots, and the 32 approved PNG references are the comparison baseline.
- ST-032-02 supplies generic `VoxelVolume`/`VoxelModel` iteration, scale-aware Shape transforms, cached occlusion/clipping, and the `Chunk::Mesher` external-neighbor override.
- [OD-020](../../../open-decisions/OD-020-mesher-merging-indexing-and-hard-normal-policy.md) resolves exact compatible-vertex reuse, deterministic first-seen indices, no polygon merging, and hard flat normals.
- [OD-024](../../../open-decisions/OD-024-current-chunk-golden-fixtures-and-runner.md) fixes graphical validation at `640 × 480` on the canonical Windows/OpenGL lane.

## Owned behavior

- Add one bake-local compatible-vertex lookup to the shared `VoxelMesher` mesh-emission path.
- Reuse a prior index only when position, stored polygon normal, and atlas UV are exactly equal.
- Assign indices on first encounter while preserving established cell, polygon, polygon-vertex, and triangle traversal order.
- Preserve every polygon currently emitted by authored and occlusion-clipped Shapes, including triangle topology/winding and hard flat normals.
- Apply the same implementation to generic `VoxelVolume`/`VoxelModel` and inherited `Chunk::Mesher` bakes.

## Explicitly not owned

- Greedy, face, or adjacent-polygon merging and any invented optimization threshold.
- Normal averaging, interpolation, recalculation, or smoothing.
- Palette resources, palette-element vertex data, Palette binding/range validation, voxel shader changes, or material migration; ST-003-02 owns those contracts.
- New fixture content or changed UV semantics merely to force a vertex reduction.
- Mesh caching, Chunk scheduling/generation/editing/streaming/gameplay changes, or a Chunk-specific indexing loop.
- Expected-PNG, comparison-tolerance, camera, atlas, or shader changes.

## Implementation inputs

### Already defined values/data

- Use the smallest existing production fixture that genuinely exposes exact cross-polygon compatible sharing.
- If atlas seams prevent every existing production fixture from sharing across polygons, record that evidence and ask the project owner before adding content or changing UV semantics.
- Deliberate failures originating in Playground continue to use `spk::Exception`.

## Behavioral contract

`VoxelMesher::bake` validates all referenced Definitions before creating observable output. Its vertex lookup is temporary per bake. A failed bake cannot retain partial index state or affect a later valid bake. Lookup may use a hash table, but table iteration never emits vertices or indices.

## Acceptance tests

### Nominal behavior and integration interactions

- [ ] Exact position, normal, and UV equality reuses the first-seen index; changing any participating attribute prevents sharing.
- [ ] Different normals retain distinct vertices at hard polygon boundaries.
- [ ] Atlas UV seams retain distinct vertices even when position and normal match.
- [ ] Repeated identical inputs produce byte-identical ordered vertex and index buffers.
- [ ] Existing orientation, vertical flip, uniform scaling, clipping, and cross-Chunk occlusion results remain valid.
- [ ] Generic volume/model and Chunk bakes exercise the same indexing implementation; `Chunk::Mesher` contains no second emission or lookup loop.
- [ ] Invalid Definition input throws `spk::Exception`, leaves no persistent indexing state, and cannot poison a later valid bake.
- [ ] ST-032-01 semantic snapshots remain unchanged.
- [ ] ST-032-02 fixtures remain semantically valid; only exact compatible-reuse vertex expectations may be updated after review, while index counts remain unchanged.
- [ ] Before/after vertex and index counts are recorded for deterministic representative fixtures.
- [ ] Representative timing is recorded only where the environment permits reliable measurement; no unstable timing assertion or unsupported performance claim is added.

### Boundaries and invalid/rejected operations

- [ ] Empty and minimum valid volumes continue to bake through the existing contract.
- [ ] Unknown runtime Definition IDs retain the established `spk::Exception` failure and recovery behavior.

### Determinism and lifecycle / retry / persistence

- [ ] Deterministic output never depends on pointer values, hash-table iteration, render frame rate, or wall-clock timing.
- [ ] The first deterministic encounter owns each emitted index, including after an earlier rejected bake.

### Rendering / golden images

- [ ] All 20 original Chunk and all 12 consolidated-mesher `640 × 480` references pass unchanged with existing tolerances.
- [ ] On failure, retain expected images and inspect actual/difference artifacts; do not approve a structural-change baseline.
- [ ] Semantic geometry/material/transform assertions pass independently of PNG comparison.

## Rendering impact

Yes. The controlled fixture uses a fixed camera, viewport, asset set, lighting, and supported GPU runner. Structural refactors must match the reviewed current baseline; intentional migrations require human review before a new versioned baseline is accepted.

## Open decisions

- [OD-020](../../../open-decisions/OD-020-mesher-merging-indexing-and-hard-normal-policy.md) — Resolved by the project owner: no polygon merging; exact position/normal/UV reuse; deterministic first-seen indices; hard flat normals.
- [OD-024](../../../open-decisions/OD-024-current-chunk-golden-fixtures-and-runner.md) — Resolved: preserve the approved `640 × 480` canonical visual baseline.

## Completion evidence

- Automated test names, exact before/after counts, implementation commit/PR, and both canonical CI lanes.
- Error-path assertion proving a rejected bake cannot poison later deterministic output.
- Confirmation that every approved expected PNG remained byte-for-byte unchanged.
- ST-003-02 becomes the next checkpoint when this ticket is complete.
