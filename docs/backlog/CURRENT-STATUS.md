# Erelia Implementation Status

**Last updated:** 19 September 2026  
**Tracking branch:** `docs/integrate-unified-voxel-backlog`  
**Current horizon:** H0 — Foundations and visual validation  
**Current checkpoint:** Backlog/architecture ready; Playground visual-regression integration is the next implementation work.

This is the living answer to:

> Where are we now, what has actually been completed, and what should be implemented next?

The word **complete** below means there is implementation or documentation evidence. A written ticket is **specified**, not implemented.

## Status legend

| Marker | Meaning |
|---|---|
| ✅ Complete | Finished with identified evidence. |
| 🟡 Current | The next active implementation ticket or checkpoint. |
| ⬜ Next | Ordered follow-up work whose prerequisites are known. |
| ⛔ Decision gate | Work must pause for user clarification or a recorded Open Decision. |
| 🔭 Later | Deliberately outside the current checkpoint/horizon. |

## Where we are now

```text
Working textured Chunk renderer
        ↓
Sparkle::TestLibrary export                    ✅
        ↓
Backlog restructuring and architecture         ✅
        ↓
Playground consumes TestLibrary                 🟡 CURRENT
        ↓
Current Chunk semantic + golden baselines       ⬜ NEXT
        ↓
VoxelVolume → VoxelMesher structural refactor   ⬜
        ↓
Palette rendering migration                     ⬜
        ↓
VS-000 unified voxel visual validation          ⬜
        ↓
VS-001 first playable                           🔭
```

## Completed

### Existing implementation baseline

- ✅ Data-driven voxel `Shape` polygons, normals, UVs, material slots, `outerSide`, and full-side coverage exist.
- ✅ `Voxel::Definition` and compact `Voxel::Cell` with orientation/vertical flip exist.
- ✅ Headless fixed 16×16×16 Chunk storage, Chunk collection/world-cell access, and the current `Chunk::Baker` exist.
- ✅ The existing textured Chunk renderer is the behavior to preserve during the structural refactor.

These are existing capabilities, not evidence that the new unified volume/mesher/Palette architecture is implemented.

### Sparkle-side testing support

- ✅ Sparkle PR #1 is merged into `Version0.1.1`.
- ✅ Optional installed `Sparkle::TestLibrary` export exists.
- ✅ Installed-package consumer support and reusable image comparison exist.
- ✅ Sparkle-side CI for the final PR head succeeded.

### Planning and architecture

- ✅ The GDD and implementation backlog are integrated.
- ✅ All 34 epics have their own folder, epic contract, coverage matrix, integration acceptance suite, and ticket index.
- ✅ All 137 tickets are standalone and contain inputs, behavioral acceptance, rendering classification, decision links, and the no-improvisation rule.
- ✅ All 23 Open Decisions have persistent records.
- ✅ `VoxelVolume`, `OcclusionResolver`, `ChunkOcclusionResolver`, `MaterialResolver`, and `VoxelMesher` ownership is separated.
- ✅ Pure VoxelModel data, assembly-only anchors, transform animation, and reusable equipment attachments are specified.
- ✅ The per-renderable Palette SSBO architecture and common Chunk/model shader contract are specified.
- ✅ The EP-020/EP-021 crafting/building dependency cycle is removed.
- ✅ Relative Markdown links and backlog structure have been validated.

## Current implementation checkpoint

### 🟡 Playground TestLibrary consumption and current-render characterization

The immediate goal is to make the current textured Chunk output reproducibly testable **before** changing volume access, meshing, or Palette rendering.

Relevant tickets:

1. [ST-000-05 — Sparkle test utilities](epics/EP-000-test-harness-ci/tickets/ST-000-05-export-sparkle-test-utilities-for-downstream-projects.md)
   - Sparkle-side export is complete.
   - Playground-side remaining work: consume the installed component, configure Playground-owned reference/result roots, verify the supported Windows/OpenGL runner, and retain actual/difference artifacts on failure.
2. [ST-001-01 — Current Chunk regression characterization](epics/EP-001-unified-voxel-volume/tickets/ST-001-01-current-chunk-regression-characterization.md)
   - Capture semantic fixtures for current Chunk storage/cell behavior.
   - Capture current textured Chunk images.
   - Review those images manually before accepting them as golden references.
   - Prove that a controlled texture/UV change makes the image comparison fail.

### Exit condition for the current checkpoint

Do not begin the structural VoxelVolume/VoxelMesher migration until all of the following are true:

- [ ] Playground resolves and links the installed `Sparkle::TestLibrary` component.
- [ ] CPU-only tests remain executable without a Window/OpenGL context.
- [ ] The supported GPU test job renders the controlled current-Chunk fixtures.
- [ ] Reference and result directories are owned/configured by Playground.
- [ ] Actual and difference images are retained on failure.
- [ ] Current textured references have been visually reviewed and approved by the user.
- [ ] Semantic Chunk fixtures and the golden-image tests pass together.
- [ ] A deliberate texture/UV alteration fails the comparison.

## Next implementation sequence

| Order | Status | Work | Why it comes here |
|---:|---|---|---|
| 1 | 🟡 | Playground TestLibrary consumption and `ST-001-01` baseline capture | Protects the working renderer before structural changes. |
| 2 | ⛔ | Resolve [OD-011](open-decisions/OD-011-c-ownership-and-view-design-for-voxelvolume.md) when the exact public C++ ownership/view contract becomes necessary | The semantic volume contract is fixed; its concrete ownership API is intentionally not invented. |
| 3 | ⬜ | [ST-001-02](epics/EP-001-unified-voxel-volume/tickets/ST-001-02-read-only-voxelvolume-contract.md) — minimal read-only VoxelVolume contract | Gives the mesher one source-independent volume interface. |
| 4 | ⬜ | [ST-001-03](epics/EP-001-unified-voxel-volume/tickets/ST-001-03-chunk-adapter-specialization.md) — adapt existing Chunk | Preserves fixed 16³ storage while exposing the common contract. |
| 5 | ⬜ | [ST-001-04](epics/EP-001-unified-voxel-volume/tickets/ST-001-04-runtime-sized-voxelmodel-storage.md) — runtime-sized VoxelModel storage | Adds the model-side volume without meshing/rendering changes. |
| 6 | ⬜ | [ST-032-01](epics/EP-032-unified-voxel-mesher/tickets/ST-032-01-current-baker-semantic-golden-fixtures.md) through ST-032-03 | Extract semantic parity, Shape expansion, and generic OcclusionResolver. |
| 7 | ⬜ | [ST-004-02](epics/EP-004-chunk-world-generation/tickets/ST-004-02-chunk-occlusion-resolver.md) and Chunk mesher integration | Restores cross-Chunk occlusion outside the generic mesher. |
| 8 | ⛔ | Resolve [OD-020](open-decisions/OD-020-mesher-merging-indexing-and-hard-normal-policy.md) only when final optimization policy is required | Correctness-first output can proceed; merging/indexing policy must be evidence-driven. |
| 9 | ⬜ | Prove unchanged semantic mesh and textured golden-image parity | Structural migration is not complete without both forms of evidence. |
| 10 | ⬜ | [ST-003-02 — Palette Resource Binding](epics/EP-003-runtime-rendering-mesh-cache/tickets/ST-003-02-palette-resource-binding-and-voxel-shader-contract.md) | Introduces the accepted per-Palette SSBO/common-shader contract after structural parity. |
| 11 | ⬜ | EP-033 palette/multi-scale visual fixtures | Intentionally changes the visual pipeline and creates reviewed versioned baselines. |
| 12 | ⬜ | EP-030 → EP-002 → EP-031 asset import, runtime assembly, and authored character workflow | Builds model/assembly content on validated runtime contracts. |
| 13 | ⬜ | [VS-000 — Unified Voxel Visual Validation](milestones/VS-000-UNIFIED-VOXEL-VISUAL-VALIDATION.md) | Validates terrain, model, assembly, equipment, Palette, and performance together. |
| 14 | 🔭 | [VS-001 — First Playable](milestones/VS-001-FIRST-PLAYABLE.md) | Starts the narrow gameplay vertical slice only after VS-000 validates the visual/runtime foundation. |

Dependencies in this table are contract dependencies, not permission to turn the backlog into a global waterfall. Independent work may proceed when its precise prerequisites are satisfied.

## Known gates requiring user input

The currently foreseeable user decisions are:

- approval of the initial textured Chunk golden images;
- OD-011 when selecting the exact C++ ownership/view form of `VoxelVolume`;
- approval of any model, assembly, animation, equipment, or material-mapping schema before it is frozen;
- OD-020 when profiling/parity evidence is available for mesh merging/indexing/hard normals;
- review of old, produced, and difference images before accepting the Palette migration baseline.

Additional ambiguity discovered during implementation is automatically an Open Decision. The executing agent must not improvise it.

## How to update this file

Update this status in the same commit that changes project state:

1. Move a ticket to **Complete** only when its completion evidence exists: implementation commit/PR, automated tests, CI result, decision provenance, and visual artifacts where required.
2. Set exactly one immediate checkpoint to **Current** unless two pieces of work are genuinely independent and active.
3. Record the next smallest executable ticket, not an entire epic, whenever possible.
4. Add newly discovered user decisions to both this file and the appropriate OD/ticket.
5. Never infer implementation completion merely because a ticket, architecture document, or test plan was written.
6. Preserve completed history; summarize it rather than deleting it when the file grows.

## Quick links

- [Backlog overview](README.md)
- [Epic index](EPIC-INDEX.md)
- [Open Decisions](open-decisions/README.md)
- [Planning horizons](milestones/HORIZONS.md)
- [VS-000](milestones/VS-000-UNIFIED-VOXEL-VISUAL-VALIDATION.md)
- [VS-001](milestones/VS-001-FIRST-PLAYABLE.md)
- [Ticket execution prompt](templates/TICKET-EXECUTION-PROMPT.md)
