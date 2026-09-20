# Erelia Implementation Status

**Last updated:** 20 September 2026

**Implementation base:** `main` at `7d7eb7b45e9274b88540682d864df60cdb05ce01`

**Delivery:** [PR #6](https://github.com/EreliaStudio/Playground/pull/6) from `feat/current-baker-semantic-fixtures`

**Current horizon:** H0 — Foundations and visual validation

**Current checkpoint:** ST-032-02 — Generic cell iteration and scale-aware Shape transform.

This is the living answer to:

> Where are we now, what has actually been completed, and what should be implemented next?

The word **complete** below means implementation and test evidence exist. A written ticket is only specified.

## Status legend

| Marker | Meaning |
|---|---|
| ✅ Complete | Finished with identified evidence. |
| 🟡 Current | The one active implementation checkpoint. |
| ⬜ Next | Ordered follow-up work whose prerequisites are known. |
| ⛔ Decision gate | Work must pause for user clarification or a recorded Open Decision. |
| 🔭 Later | Deliberately outside the current checkpoint/horizon. |

## Where we are now

```text
Working textured Chunk renderer                    ✅
Sparkle::TestLibrary export                        ✅
Playground TestLibrary consumption                 ✅
Prebuilt Sparkle package consumption               ✅
Current Chunk semantic + golden baseline           ✅
VoxelVolume contract + batched editor/versioning   ✅
Chunk inheritance / storage/editor migration       ✅
Runtime-sized VoxelModel                           ✅
Current Baker semantic fixtures                    ✅
Scale-aware VoxelMesher structural extraction      🟡 CURRENT
VoxelMesher boundary hook + ChunkMesher override   ⬜
Palette rendering migration                        ⬜
VS-000 unified voxel visual validation             ⬜
VS-001 first playable                              🔭
```

## Completed

### Current implementation baseline

- ✅ Data-driven voxel Shapes, Definitions, compact packed `Voxel::Cell`, fixed 16³ Chunk behavior, Collection/world lookup, and current `Chunk::Baker` exist.
- ✅ The existing textured Chunk renderer is the protected visual baseline for structural work.

### Installed test/package infrastructure

- ✅ Sparkle exports the optional installed `Sparkle::TestLibrary` component.
- ✅ Playground resolves `sparkle CONFIG REQUIRED COMPONENTS TestLibrary` and links `Sparkle::TestLibrary` through `PlaygroundTestLibrary`.
- ✅ CI downloads the checksum-pinned prebuilt Sparkle `0.1.2` package at `65c1091fcc3d3387f7d019486e3538c7df5572d6`; Playground does not rebuild Sparkle source.
- ✅ CPU/headless and Windows/OpenGL jobs are separate and retain diagnostics/artifacts.
- ✅ Architecture planning used the older Sparkle baseline `9784377d41509234d43e4adec16505382eef178f`; it is not presented as the currently consumed package.

### ST-001-01 — Current Chunk characterization

- ✅ Semantic tests cover storage/indexing, coordinates, Collection, Editor, scheduler, baker, transforms, catalog, Shape, and packed Cell behavior.
- ✅ The Windows/OpenGL harness covers five scene families from four fixed views each.
- ✅ All 20 approved current textured Chunk PNG references are checked in.
- ✅ The controlled UV-regression sensitivity path proves an altered UV fails comparison without changing tolerances or references.
- ✅ OD-024 records the approved fixtures, `640 × 480` capture, and canonical `windows-2025` software-OpenGL runner.
- ✅ Main-branch [CI run 35456326462](https://github.com/EreliaStudio/Playground/actions/runs/35456326462) passed both `CPU/headless tests` and `Windows/OpenGL golden candidates` for commit `0bd12e5`.

### Planning and decisions

- ✅ The backlog contains 34 epics, 137 tickets, and 25 persistent decision records.
- ✅ Four decisions are resolved: OD-011, OD-023, OD-024, and OD-025. Twenty-one remain open.
- ✅ OD-011 selects one concrete vector-owning `VoxelVolume` base and explicitly accepts replacing Chunk's fixed array during ST-001-03.
- ✅ OD-023 preserves the existing packed Cell orientation/vertical-flip representation until evidence justifies a separate expansion decision.

### ST-001-02 — VoxelVolume contract and generic editor

- ✅ `VoxelVolume` owns runtime-sized, zero-initialized `std::vector<Voxel::Cell>` storage.
- ✅ Runtime dimensions, finite positive uniform voxel size, checked coordinate access, read-only span access, and origin-based local bounds are implemented.
- ✅ Meshing-facing access is read-only; controlled mutation uses the generic nested `VoxelVolume::Editor`.
- ✅ `VoxelVolume` owns versioning, and a changed editor session publishes exactly one invalidation while a no-op session publishes none.
- ✅ VoxelVolume-owned failures use the selected `spk::Exception` policy with stable messages and source-location diagnostics.
- ✅ Twelve focused headless GoogleTests cover metadata/validation, access/topology, editor/version transactions, and Sparkle exception diagnostics.
- ✅ Branch [CI run 35460049379](https://github.com/EreliaStudio/Playground/actions/runs/35460049379) passed both complete lanes for remote commit `d00e467`.
- ✅ Editor/version refinement [CI run 35461166765](https://github.com/EreliaStudio/Playground/actions/runs/35461166765) passed both complete lanes for remote commit `9effce8`.
- ✅ Sparkle-exception refinement [CI run 35463284473](https://github.com/EreliaStudio/Playground/actions/runs/35463284473) passed both complete lanes for remote commit `7b04ab2`.
- ✅ No approved golden reference, tolerance, texture, UV, Chunk implementation, or rendering output changed.

### ST-001-03 — Chunk adapter/specialization

- ✅ `Chunk` now derives from `VoxelVolume` and constructs the shared storage as exactly `16×16×16` at voxel size `1.0`.
- ✅ Chunk's duplicate fixed array, direct `VersionedTrait` inheritance, and duplicate editor implementation were removed.
- ✅ `Chunk::edit()` resolves to the inherited `VoxelVolume::Editor`; changed and no-op sessions retain the established one-notification transaction behavior.
- ✅ Chunk keeps its coordinate, world conversion, Collection, generation, baking, scheduling, and rendering responsibilities.
- ✅ Focused Chunk tests cover the fixed constructor specialization and Chunk-owned coordinate/index behavior; inherited storage, access, editor, versioning, and diagnostics remain covered once by the VoxelVolume suites.
- ✅ BakeScheduler coverage proves an edited Chunk queues itself and every available face-neighbor for rebuild.
- ✅ Implementation commit: [`27c095f`](https://github.com/EreliaStudio/Playground/commit/27c095f467cdb4bd1525aa5be81bccfab889b5fb).
- ✅ Test-ownership refinement commit: [`f3f855b`](https://github.com/EreliaStudio/Playground/commit/f3f855b71c67bfb96100bb09a3e52fbedd14683b).
- ✅ PR #4 [CI run 35467461734](https://github.com/EreliaStudio/Playground/actions/runs/35467461734) passed both `CPU/headless tests` and `Windows/OpenGL golden candidates` for commit `f3f855b`.
- ✅ No approved reference, comparison tolerance, texture, UV, or intended rendering output changed.

### ST-001-04 — Runtime-sized VoxelModel storage

- ✅ `VoxelModel` is one final semantic specialization over `VoxelVolume`; runtime dimensions and uniform voxel scale remain constructor data rather than template parameters.
- ✅ Representative `10×10×14`, `12×7×14`, and `32×32×22` models use the same C++ type.
- ✅ An `8×8×16` model at scale `0.1` reports local bounds `0.8×0.8×1.6`.
- ✅ `VoxelModel` inherits the common packed-cell read contract, batched editor, version notification, and `spk::Exception` validation policy without adding a model-specific mutation API.
- ✅ A provisional `spk::JSON::Reader` constructor accepts runtime dimensions, voxel size, and sparse `{position, value, orientation, flip}` entries without changing packed Cell storage.
- ✅ Sparse-loader duplicate detection uses `spk::Vector3Int` directly as an ordered key through Sparkle `0.1.2`; the temporary tuple adapter is removed.
- ✅ Orientation and flip use reviewed named strings; `orientation` is required and omitted `flip` defaults to `positive-y`.
- ✅ Duplicate or out-of-range positions, unknown fields/enum names, and values beyond packed Cell capacity fail with JSON file/path diagnostics.
- ✅ Nine focused headless GoogleTests cover runtime storage plus the provisional sparse JSON loader and its rejection behavior.
- ✅ No stable asset format, file-loading API, catalog mapping, rendering, Palette, GPU, transform, anchor, animation, or Chunk-world behavior was added.
- ✅ Implementation head: [`affc850`](https://github.com/EreliaStudio/Playground/commit/affc850945e233928acfa813e7add7d7a8a6cf69).
- ✅ PR #5 [CI run 35498639959](https://github.com/EreliaStudio/Playground/actions/runs/35498639959) passed both `CPU/headless tests` and `Windows/OpenGL golden candidates`.

### ST-032-01 — Current Baker semantic golden fixtures

- ✅ Seven minimal fixtures lock the current cube, slab, slope, stair, cross, unloaded-neighbor, and cross-Chunk occlusion semantics.
- ✅ Canonical snapshots compare positions, normals, atlas UVs, winding, and visible topology while ignoring triangle emission order, vertex-buffer indices, and cyclic starting vertices.
- ✅ Exact Chunk/cell coordinates, stable Definition names/runtime IDs, orientations, flips, topology, and atlas expectations are recorded with the fixtures.
- ✅ Boundary coverage proves an absent adjacent Chunk is empty and an available solid neighbor removes only the shared face across local X coordinates `15 → 0`.
- ✅ Repeated inputs produce byte-identical semantic output; an unknown Definition ID throws without poisoning subsequent valid bakes.
- ✅ The current `Chunk::Baker` remains intact as the later `VoxelMesher` parity oracle.
- ✅ No approved textured reference, comparison tolerance, camera, atlas, shader, or canonical runner changed.
- ✅ OD-025 records the approved one-base-mesher design: shared cached occlusion in VoxelMesher and only external-neighbor lookup overridden by ChunkMesher.
- ✅ Implementation head: [`aa1b9c2`](https://github.com/EreliaStudio/Playground/commit/aa1b9c216a86476ebef0bd46000bf38b9c82e8a8).
- ✅ PR #6 [CI run 35501490841](https://github.com/EreliaStudio/Playground/actions/runs/35501490841) passed both `CPU/headless tests` and `Windows/OpenGL golden candidates`.

## Current implementation checkpoint

### 🟡 ST-032-02 — Generic cell iteration and scale-aware Shape transform

The next implementation must:

- move Shape expansion and deterministic cell iteration from Chunk-specific code toward the common volume-based meshing path;
- map each vertex through the integer voxel coordinate and the volume's uniform `voxelSize` while keeping occupancy/neighbor decisions on the discrete grid;
- preserve unit/correct normals, current orientation and vertical-flip semantics, material/UV behavior, and the ST-032-01 canonical snapshots;
- keep the current Baker available as the semantic and textured-output oracle until the later parity gate passes;
- follow resolved OD-025: the generic VoxelMesher owns the common algorithm and occlusion cache, while only outside-volume lookup is virtual and defaults to `Voxel::Cell{}`;
- keep final merging, indexing, and hard-normal behavior blocked on OD-020 rather than selecting it during structural extraction.

ST-032-03 owns the outside-volume hook and default empty-boundary behavior. EP-004/ST-004-02 then supplies the `ChunkMesher` override backed by `Chunk::Collection`; neither ticket may duplicate the generic meshing/cache algorithm. Any newly discovered observable ambiguity becomes an Open Decision rather than an improvised implementation.

## Next implementation sequence

| Order | Status | Work | Why it comes here |
|---:|---|---|---|
| 1 | ✅ | [ST-001-02](epics/EP-001-unified-voxel-volume/tickets/ST-001-02-read-only-voxelvolume-contract.md) — owning VoxelVolume/read/edit contract | Generic storage, reads, editing/versioning, Sparkle diagnostics, and both complete CI lanes are passing. |
| 2 | ✅ | [ST-001-03](epics/EP-001-unified-voxel-volume/tickets/ST-001-03-chunk-adapter-specialization.md) — Chunk inheritance/storage/editor migration | Chunk now uses inherited 16³/1.0 storage, reads, editing, and versioning while retaining Chunk-only behavior and unchanged rendering. |
| 3 | ✅ | [ST-001-04](epics/EP-001-unified-voxel-volume/tickets/ST-001-04-runtime-sized-voxelmodel-storage.md) — runtime-sized VoxelModel semantic type | One semantic model type now forwards runtime dimensions and scale to shared storage and inherits editing/versioning unchanged. |
| 4 | ✅ | [ST-032-01](epics/EP-032-unified-voxel-mesher/tickets/ST-032-01-current-baker-semantic-golden-fixtures.md) — current Baker semantic fixtures | Seven canonical fixtures now protect positions, normals, atlas UVs, winding, visible topology, and both Chunk-boundary outcomes. |
| 5 | 🟡 | [ST-032-02](epics/EP-032-unified-voxel-mesher/tickets/ST-032-02-generic-cell-iteration-and-scale-aware-shape-transform.md) — generic iteration and scale-aware Shape transform | Extracts the common volume/scale transform without selecting later optimization or occlusion policy. |
| 6 | ⬜ | [ST-032-03](epics/EP-032-unified-voxel-mesher/tickets/ST-032-03-occlusionresolver-meshing-context.md) — base boundary hook and cached occlusion | Adds the default outside-is-empty hook while keeping occlusion evaluation/cache in VoxelMesher. |
| 7 | ⬜ | [ST-004-02](epics/EP-004-chunk-world-generation/tickets/ST-004-02-chunk-occlusion-resolver.md) — ChunkMesher override | Restores cross-Chunk lookup by overriding only external-neighbor resolution. |
| 8 | ⛔ | Resolve [OD-020](open-decisions/OD-020-mesher-merging-indexing-and-hard-normal-policy.md) when profiling/parity evidence exists | Final optimization policy remains evidence-driven. |
| 9 | ⬜ | Prove unchanged semantic mesh and textured golden-image parity | Structural migration is not complete without both forms of evidence. |
| 10 | ⬜ | [ST-003-02](epics/EP-003-runtime-rendering-mesh-cache/tickets/ST-003-02-palette-resource-binding-and-voxel-shader-contract.md) | Introduces the accepted per-Palette SSBO/common-shader contract after structural parity. |
| 11 | ⬜ | EP-033 palette/multi-scale visual fixtures | Intentionally changes the visual pipeline and creates reviewed versioned baselines. |
| 12 | ⬜ | EP-030 → EP-002 → EP-031 asset import, runtime assembly, and authored character workflow | Builds model/assembly content on validated runtime contracts. |
| 13 | ⬜ | [VS-000](milestones/VS-000-UNIFIED-VOXEL-VISUAL-VALIDATION.md) | Validates terrain, model, assembly, equipment, Palette, and performance together. |
| 14 | 🔭 | [VS-001](milestones/VS-001-FIRST-PLAYABLE.md) | Starts the gameplay vertical slice after the visual/runtime foundation. |

## Known user gates

- OD-020 when profiling/parity evidence is available for mesh merging/indexing/hard normals.
- OD-025 is resolved: VoxelMesher owns the common cached algorithm; ChunkMesher overrides only outside-volume neighbor lookup.
- Approval of any model, assembly, animation, equipment, import, or material-mapping schema before it is frozen.
- Review of expected, produced, and difference images before accepting the later Palette migration baseline.

Additional ambiguity discovered during implementation becomes an Open Decision. The executing agent must not improvise it.

## How to update this file

1. Move a ticket to **Complete** only when implementation, automated tests, applicable CI, decision provenance, and visual evidence exist.
2. Keep exactly one immediate checkpoint **Current** unless independent active work truly exists.
3. Record the next smallest executable ticket, not an entire epic.
4. Update this file in the same branch as the code that changes project state.
5. Preserve completed history and distinguish architecture baselines from currently consumed dependency revisions.

## Quick links

- [Backlog overview](README.md)
- [Epic index](EPIC-INDEX.md)
- [Open Decisions](open-decisions/README.md)
- [Planning horizons](milestones/HORIZONS.md)
- [VS-000](milestones/VS-000-UNIFIED-VOXEL-VISUAL-VALIDATION.md)
- [VS-001](milestones/VS-001-FIRST-PLAYABLE.md)
- [Ticket execution prompt](templates/TICKET-EXECUTION-PROMPT.md)
