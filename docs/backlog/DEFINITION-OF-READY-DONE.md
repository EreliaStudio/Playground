# Definition of Ready / Done

## Story Ready

A story is Ready when:

- its intended user/domain behavior is clear enough to test;
- dependencies needed to start are available or stubbed behind a stable contract;
- unresolved exact values are identified as configuration/decision gates;
- acceptance cases include failure/boundary behavior;
- persistence/network/content compatibility impact is identified when relevant.

## Story Done

- implementation is reviewed/builds in supported configuration;
- acceptance cases are automated and passing;
- invalid input leaves no partial invalid state;
- headless test exists unless presentation-only;
- deterministic behavior has a reproducible fixture where expected;
- public schema/content changes are versioned/documented;
- performance-sensitive paths have at least a representative measurement.

## Unified voxel / importer / mesher-specific Done

A voxel architecture or asset-pipeline story additionally requires, where applicable:

- current Chunk behavior is covered by regression fixtures before structural replacement;
- Chunk and VoxelModel paths use the same volume/mesher contract rather than hidden duplicate algorithms;
- importer output is deterministic at the semantic level for fixed source/mapping/converter version;
- diagnostics identify useful source/cell/definition context where possible;
- invalid import/mesh output never replaces the last valid published asset/cache entry;
- headless volume/import/meshing tests do not depend on editor, Window, Surface or GPU;
- transform/material-only changes are demonstrated not to trigger remeshing.
