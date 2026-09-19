# Erelia Implementation Status

**Last updated:** 19 September 2026

**Baseline branch/state:** `main` at `0bd12e5053e482fd6a4f1a4f2a843279627f2306`

**Active implementation branch:** `feat/voxel-volume-contract`

**Current horizon:** H0 — Foundations and visual validation

**Current checkpoint:** ST-001-02 — verify generic VoxelVolume editor/version ownership.

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
VoxelVolume contract + batched editor/versioning   🟡 CURRENT
Chunk inheritance / storage/editor migration       ⬜ NEXT
Runtime-sized VoxelModel                           ⬜
VoxelMesher structural extraction                  ⬜
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
- ✅ CI downloads a checksum-pinned prebuilt Sparkle `0.1.1` package at `68c260aa2108cd22c26ba09a2c8ca6f62913ad39`; Playground does not rebuild Sparkle source.
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

- ✅ The backlog contains 34 epics, 137 tickets, and 24 persistent decision records.
- ✅ Three decisions are resolved: OD-011, OD-023, and OD-024. Twenty-one remain open.
- ✅ OD-011 selects one concrete vector-owning `VoxelVolume` base and explicitly accepts replacing Chunk's fixed array during ST-001-03.
- ✅ OD-023 preserves the existing packed Cell orientation/vertical-flip representation until evidence justifies a separate expansion decision.

### ST-001-02 — VoxelVolume contract and generic editor

- ✅ `VoxelVolume` owns runtime-sized, zero-initialized `std::vector<Voxel::Cell>` storage.
- ✅ Runtime dimensions, finite positive uniform voxel size, checked coordinate access, read-only span access, and origin-based local bounds are implemented.
- ✅ Meshing-facing access is read-only; controlled mutation uses the generic nested `VoxelVolume::Editor`.
- ✅ `VoxelVolume` owns versioning, and a changed editor session publishes exactly one invalidation while a no-op session publishes none.
- ✅ Ten focused headless GoogleTests cover metadata/validation, access/topology, and editor/version transaction behavior.
- ✅ Branch [CI run 35460049379](https://github.com/EreliaStudio/Playground/actions/runs/35460049379) passed both complete lanes for remote commit `d00e467`.
- ✅ No approved golden reference, tolerance, texture, UV, Chunk implementation, or rendering output changed.

## Current implementation checkpoint

### ⬜ ST-001-03 — Chunk adapter/specialization

The next implementation must:

- derive/adapt `Chunk` to `VoxelVolume` and construct the base as `16×16×16` at scale `1.0`;
- remove Chunk's duplicate fixed array and use the base-owned vector approved in OD-011;
- remove Chunk's duplicate direct `VersionedTrait` and `Chunk::Editor` implementation in favor of inherited VoxelVolume behavior;
- preserve current coordinate/index order, version invalidation, Collection, generator, baker, scheduler, and renderer behavior;
- pass the complete CPU/headless suite and all 20 approved golden comparisons without changing references or tolerances.

ST-001-03 becomes Current after the refined ST-001-02 contract passes both CI lanes. No unresolved decision currently blocks it. Any new observable ambiguity becomes an Open Decision rather than an improvised implementation.

## Next implementation sequence

| Order | Status | Work | Why it comes here |
|---:|---|---|---|
| 1 | 🟡 | [ST-001-02](epics/EP-001-unified-voxel-volume/tickets/ST-001-02-read-only-voxelvolume-contract.md) — owning VoxelVolume/read/edit contract | Generic editor/version behavior is implemented locally and awaiting complete CI verification. |
| 2 | ⬜ | [ST-001-03](epics/EP-001-unified-voxel-volume/tickets/ST-001-03-chunk-adapter-specialization.md) — Chunk inheritance/storage/editor migration | Makes Chunk construct VoxelVolume as 16³ at scale 1.0, removes duplicate storage/editor/version behavior, and proves unchanged output. |
| 3 | ⬜ | [ST-001-04](epics/EP-001-unified-voxel-volume/tickets/ST-001-04-runtime-sized-voxelmodel-storage.md) — runtime-sized VoxelModel semantic type | Adds the model-specific construction/editing boundary over common storage. |
| 4 | ⬜ | [ST-032-01](epics/EP-032-unified-voxel-mesher/tickets/ST-032-01-current-baker-semantic-golden-fixtures.md) through ST-032-03 | Extracts semantic parity, Shape expansion, and generic OcclusionResolver. |
| 5 | ⬜ | [ST-004-02](epics/EP-004-chunk-world-generation/tickets/ST-004-02-chunk-occlusion-resolver.md) and Chunk mesher integration | Restores cross-Chunk occlusion outside the generic mesher. |
| 6 | ⛔ | Resolve [OD-020](open-decisions/OD-020-mesher-merging-indexing-and-hard-normal-policy.md) when profiling/parity evidence exists | Final optimization policy remains evidence-driven. |
| 7 | ⬜ | Prove unchanged semantic mesh and textured golden-image parity | Structural migration is not complete without both forms of evidence. |
| 8 | ⬜ | [ST-003-02](epics/EP-003-runtime-rendering-mesh-cache/tickets/ST-003-02-palette-resource-binding-and-voxel-shader-contract.md) | Introduces the accepted per-Palette SSBO/common-shader contract after structural parity. |
| 9 | ⬜ | EP-033 palette/multi-scale visual fixtures | Intentionally changes the visual pipeline and creates reviewed versioned baselines. |
| 10 | ⬜ | EP-030 → EP-002 → EP-031 asset import, runtime assembly, and authored character workflow | Builds model/assembly content on validated runtime contracts. |
| 11 | ⬜ | [VS-000](milestones/VS-000-UNIFIED-VOXEL-VISUAL-VALIDATION.md) | Validates terrain, model, assembly, equipment, Palette, and performance together. |
| 12 | 🔭 | [VS-001](milestones/VS-001-FIRST-PLAYABLE.md) | Starts the gameplay vertical slice after the visual/runtime foundation. |

## Known user gates

- OD-020 when profiling/parity evidence is available for mesh merging/indexing/hard normals.
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
