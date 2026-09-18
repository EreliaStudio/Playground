# EP-001 — Unified Voxel Volume and Cell Contract

**Meta-epic:** ME-001  
**Horizon:** H0  
**GDD / architecture coverage:** Unified multi-scale voxel architecture supporting existing world and future model content  
**Dependencies:** EP-000

## Goal

Extract one meshing-facing volume/grid contract from the current Chunk data model while preserving the existing compact Voxel::Cell, Definition/Shape chain and optimized fixed-size Chunk storage.

## Architectural constraints

- Simulation/domain code remains executable headlessly without a Window, Surface, OpenGL context, or GPU.
- Presentation is a view of authoritative state and must never create gameplay outcomes.
- Stable content IDs are distinct from display names and file-system paths.
- Invalid commands, content, or assets fail before partial authoritative mutation.
- Open GDD values remain configuration or explicit decision gates rather than silently invented constants.
- The exact public class/template shape is not predetermined; prefer a minimal read contract the mesher can consume.
- Runtime dimensions are data for imported models; do not encode arbitrary model dimensions as C++ template types.
- `Chunk` may keep `std::array<Voxel::Cell, 16³>` internally while adapting to the common contract.
- Voxel scale is a uniform scalar initially and belongs to the volume contract.
- Do not widen/change packed `Voxel::Cell` orientation/flip during this refactor unless a separate requirement proves necessary.
- Current texture/UV fields remain valid until the later Material migration.
- Generic volume code owns no streaming, generation, world coordinate or entity gameplay semantics.

## Integration Test Catalogue

### Epic-specific cases

1. A 16×16×16 adapter over the current Chunk exposes exactly the same cells and indexing as direct Chunk access.
2. A runtime-sized 8×8×16 volume stores/retrieves cells correctly with no template type dedicated to that dimension.
3. A 16³ volume at scale 1.0 reports 16×16×16 local extent.
4. An 8×8×16 volume at scale 0.1 reports 0.8×0.8×1.6 local extent within defined floating tolerance.
5. Scale changes local geometry/bounds but never changes discrete neighbor relationships or cell IDs.
6. Empty/minimum dimensions and invalid coordinates follow the chosen explicit validation policy without aliasing valid storage.
7. Existing packed Cell ID/orientation/flip round-trips byte-for-byte.
8. Volume access and scale/bounds calculations run headlessly and without Definition catalog/GPU ownership.

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

### ST-001-01 — Current Chunk regression characterization

Capture stable fixtures around the existing 16³ storage/indexing and Cell semantics before extraction.

**Acceptance cases**

- [ ] Fixtures cover first/last valid coordinate and representative negative world-to-local conversion through Chunk helpers.
- [ ] Packed Cell fixtures cover empty, oriented and vertically flipped cells.
- [ ] Regression fixture records current semantic output rather than relying on implementation-private addresses.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-001-02 — Read-only VoxelVolume contract

Introduce the smallest common dimensions/cell/scale/bounds access contract required by meshing.

**Acceptance cases**

- [ ] Runtime dimensions are queryable as data.
- [ ] `at()` or equivalent cannot expose an invalid coordinate as a valid cell.
- [ ] Uniform `voxelSize` is positive and validated.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-001-03 — Chunk adapter/specialization

Make current Chunk satisfy/adapt to the common contract without moving Chunk-specific responsibilities into it.

**Acceptance cases**

- [ ] Chunk remains fixed 16³ and keeps current optimized storage.
- [ ] Chunk coordinate/world conversion APIs remain Chunk concerns.
- [ ] Existing generator/collection/editor call sites need no model-specific concepts.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The behavior is testable without rendering unless the story is explicitly presentation-only.
### ST-001-04 — Runtime-sized VoxelModel storage

Add a simple owning volume suitable for imported props/parts with arbitrary dimensions and smaller scale.

**Acceptance cases**

- [ ] A model can be 10×10×14, 12×7×14 and 32×32×22 without new C++ types.
- [ ] Model-local bounds derive from dimensions × voxelSize.
- [ ] Model storage uses the same Voxel::Cell representation as Chunk.
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
