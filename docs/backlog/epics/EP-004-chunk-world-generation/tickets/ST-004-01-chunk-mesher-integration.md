# ST-004-01 — Transition Chunk consumers from Baker to ChunkMesher

## Intent

After consolidated ST-032-02 is complete, replace runtime use of `Chunk::Baker` with the validated `ChunkMesher` without changing Chunk output, scheduling, invalidation, or rendering behavior.

## Execution decision policy

- Do not invent unspecified behavior, data, assets, values, formulas, schemas, ownership, error policy, architecture, or technology.
- Treat every unspecified observable choice as an Open Decision and ask the project owner before implementing it.
- Deliberate failures originating in Playground throw `spk::Exception`.

## Starting state / prerequisites

- Consolidated ST-032-02 provides the reviewed `VoxelMesher` and `ChunkMesher` implementations plus semantic/numerical/golden evidence.
- `Chunk::Baker` remains the current runtime implementation and independent parity oracle.
- Existing `Chunk::BakeScheduler`, views, application setup, and current textured references are passing.

## Owned behavior

- Redirect `Chunk::BakeScheduler`, application setup, and other production Chunk-mesh consumers to one appropriately owned `ChunkMesher`.
- Preserve scheduler contracts, version/dirty behavior, neighbor rebuild scheduling, Chunk transforms, atlas use, and render-command behavior.
- Prove complete semantic and approved textured-image parity before removing obsolete `Chunk::Baker` implementation files.
- Remove `Chunk::Baker` only after no production or test consumer requires it and the parity evidence remains independently meaningful in retained fixtures.

## Explicitly not owned

- Changes to the generic meshing algorithm, outside-neighbor policy, cache, Shape transforms, or fixture baselines.
- OD-020 optimization/indexing choices or Palette/material migration.
- Chunk generation, streaming, editing, collision, or gameplay changes.

## Acceptance tests

- [ ] `Chunk::BakeScheduler` produces the same completion events and meshes through `ChunkMesher`.
- [ ] Editing a Chunk still schedules itself and every available face-neighbor exactly once under the existing contract.
- [ ] Application and view wiring use `ChunkMesher` without a second adapter or meshing loop.
- [ ] All ST-032-01 semantic snapshots and consolidated ST-032-02 numerical/semantic tests remain unchanged.
- [ ] All 20 current textured Chunk references and all approved consolidated-mesher references remain unchanged.
- [ ] Seeded generation output and deterministic replay remain unchanged.
- [ ] No production reference to `Chunk::Baker` remains before its implementation is removed.
- [ ] Invalid input uses `spk::Exception` and does not poison subsequent scheduling or baking.

## Rendering impact

Structural only. The approved `640 × 480` OD-024 baselines must remain unchanged; any visual difference blocks completion rather than creating a replacement reference.

## Open decisions

None identified. OD-020 remains outside this transition.

## Completion evidence

- Passing CPU/headless and Windows/OpenGL lanes after consumer migration.
- Search evidence showing no remaining production `Chunk::Baker` consumer.
- Implementation commit and final CI run recorded in `CURRENT-STATUS.md`.
