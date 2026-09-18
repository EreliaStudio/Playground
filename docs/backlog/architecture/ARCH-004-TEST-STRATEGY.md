# ARCH-004 — Test Strategy

## Levels

| Level | Purpose |
|---|---|
| Story acceptance | unit/component behavior of one deliverable |
| Epic integration | boundaries between components/services |
| Meta-epic/system | capability and cross-feature invariants |
| Vertical slice | end-to-end player/server workflow |

## Current-renderer regression gate

Before replacing `Chunk::Baker`, capture deterministic fixtures for representative current content:

- solid cube neighborhood;
- adjacent Chunk boundary;
- slab against cube/empty neighbor;
- slope orientations and vertical flip;
- stair geometry;
- crossed vegetation plane;
- material/texture-slot assignment;
- generated debug chunk or stable World fixture.

For structural refactor stages, compare semantic mesh output rather than byte identity when vertex ordering is not part of the contract: exposed polygon topology, transformed positions, normals, material slots and bounds must remain equivalent.

## Cross-cutting resilience catalogue

Every relevant subsystem should cover:

1. nominal behavior;
2. exact boundaries;
3. invalid/corrupt input;
4. create/use/unload/recreate lifecycle;
5. determinism where expected;
6. persistence/version migration;
7. retry/idempotency where relevant;
8. concurrency and cancellation where relevant;
9. authority/trust-boundary validation;
10. dependency failure and rollback/fail-closed behavior;
11. headless execution for simulation/data paths;
12. regression and performance fixtures.

## Unified voxel property candidates

- every valid coordinate maps to exactly one valid cell index;
- out-of-bounds coordinates never alias valid cells;
- volume local extent equals dimensions × uniform voxel size;
- changing voxel scale changes positions/bounds but not grid neighbor relationships;
- same cell arrangement at two scales yields equivalent visibility/topology;
- current `Voxel::Cell` pack/unpack round-trips exactly;
- orientation/flip preserve current Shape transform semantics;
- standalone outside-volume neighbor resolves empty;
- Chunk neighbor resolution matches `Chunk::Collection::worldCell` semantics;
- meshing never requires Window/Surface/OpenGL;
- transform animation never dirties voxel mesh;
- cell edit dirties only the owning volume/affected chunk neighbors required by the chosen scheduler.

## Material visual-regression fixtures

Render fixed camera/light scenes for:

- palette-only terrain across multiple Chunk boundaries;
- same Material on moving object using local sampling;
- world-space terrain variation with no chunk seam;
- camera movement with no pattern swimming;
- emissive prototype when that property is introduced.

Image comparison should allow explicit tolerances for GPU/driver differences while preserving obvious seam/swimming regressions.

## Fuzz/property targets

- volume coordinate/index conversion;
- cell orientation/flip transforms;
- Shape parser/content validation;
- imported voxel volume parser/converter;
- VoxelMesher neighbor visibility decisions;
- formula/content parsers;
- network protocol decoding.

## Performance methodology

Use fixed fixtures and distributions, not one stopwatch sample. Measure separately:

- current Chunk::Baker baseline;
- generalized VoxelMesher on equivalent 16³ fixtures;
- small/medium/large VoxelModel meshing;
- mesh upload/cache hit/miss;
- material rendering;
- world generation and simulation.

The generic abstraction is acceptable only if it does not introduce an unexplained terrain regression. Optimize storage/specialization behind the shared contract before splitting the meshing architecture.
