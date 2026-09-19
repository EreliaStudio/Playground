# ST-003-02 — Palette Resource Binding and Unified Voxel Shader Contract

## Intent

Introduce the accepted per-Palette GPU binding architecture for both Chunk and VoxelModel rendering while retaining the current atlas path until structural mesh parity is proven.

## Execution decision policy

- Do not invent, infer, or silently select behavior, data, assets, values, formulas, schemas, ownership, error policy, architecture, or technology that this ticket or an already-resolved linked decision does not define.
- Treat every unspecified choice that can affect public behavior, compatibility, persistence, rendering, tests, or later tickets as an Open Decision.
- Stop before implementing the affected portion, ask the user a focused clarification question, and record the approved answer in the applicable OD file—or create/link a new OD record when none exists.
- Continue only with work that is independent of the missing decision.
- Select an implementation detail without clarification only when this ticket explicitly delegates it and the choice cannot alter an observable contract.

## Starting state / prerequisites

- The dependency contracts `EP-001, EP-032` used by this story are available; an epic dependency denotes a required contract, not global waterfall completion.
- The test begins from an explicitly constructed, valid state with no pending operation unless the case says otherwise.
- Rendering, authoring data, and display names are not authoritative gameplay state.

## Owned behavior

- Represent each `Palette` as an independent CPU-side resource associated with its own GPU SSBO.
- Make `VoxelRenderCommand` reference and bind the Palette required by the current renderable.
- Feed a flat integer `paletteElementIndex` from `VoxelMesh` to the shared voxel shader.
- Validate Palette compatibility and element bounds before drawing.
- Keep Palette selection independent from pure VoxelModel volume data and cached mesh geometry.
- Preserve the current textured/atlas rendering path until the deliberate palette visual migration.

## Explicitly not owned

- Behavior assigned to sibling tickets or later epics.
- Resolution of linked Open Decisions.
- New balance values, content, schemas, or platform choices not approved by the user.
- CPU ownership syntax such as `shared_ptr` versus another safe resource handle; this remains an implementation choice only if lifetime and observable behavior stay identical.
- Exact SSBO wrapper type, binding number, packed vertex layout, Palette sorting, and redundant-bind optimization.
- Palette variation/sampling policy, advanced Material effects, and serialized derivative mesh-cache policy.
- Shape-to-WorldPalette content-schema precedence, which requires its own approved example and ticket contract.

## Implementation inputs

### Already defined values/data

- Story intent: bind the Palette required by each voxel render command and use one shared Chunk/VoxelModel shader contract.
- Required dependency contracts: EP-001, EP-032.
- Test fixture rule: Use the smallest deterministic fixture that exposes the owned behavior. Record exact dimensions, cell coordinates, palette indices, transforms, expected vertices/state, and stable asset IDs in the test source; do not substitute an unspecified representative asset.

### Confirmed architecture decisions

These choices are closed for this ticket and must not be redesigned during implementation:

1. `PaletteCollection`, if present, is CPU-side resource management only. The GPU does **not** receive one global PaletteCollection SSBO.
2. Every `Palette` is independently bindable and owns or is associated with its own SSBO.
3. A `VoxelRenderCommand` references the Palette required by the current renderable. Execution is conceptually: bind voxel program → bind mesh vertex/index buffers → bind required Palette SSBO → bind transform/render data → draw.
4. `VoxelMesh` semantically carries at least `position`, `normal`, and integer `paletteElementIndex`. Exact packing is deliberately not fixed here.
5. `paletteElementIndex` addresses the currently bound Palette and is transported as a flat/non-interpolated integer. It is not a Voxel ID.
6. The initial shader contract has no GPU `paletteId` and performs no PaletteCollection lookup.
7. Chunk and VoxelModel rendering use the same VoxelMesh vertex semantics and the same voxel shader.
8. Palette selection belongs to the renderable/part instance/render command, never to the pure `.vxm` volume asset.
9. The same cached mesh may be rendered with multiple compatible Palettes. Changing the Palette does not change geometry, invalidate the mesh, or invoke `VoxelMesher`.
10. Chunks normally share one relatively large `WorldPalette`; models/equipment may use smaller Palettes. This does not create a different shader path.
11. Every Palette used with a mesh must contain all referenced element indices. Missing Palette data and out-of-range indices are detected rather than sampled from GPU memory.
12. `MaterialResolver` selects an element per emitted polygon. For Chunks it may use both semantic `materialSlot` and spatial `outerSide`; these concepts remain distinct.
13. All vertices of one emitted polygon normally carry the same resolved element index.
14. Palette grouping or redundant-bind avoidance may be added later as a measured optimization without changing this resource model.

### User validation required before implementation

No user clarification is required for the 14 confirmed choices above. Ask the user before implementing any unspecified behavior, especially Palette serialization, ownership that changes lifetime semantics, missing-Palette fallback behavior beyond fail-closed validation, or Shape/material mapping precedence. Before accepting a changed image baseline, present old, produced, and difference images for human approval.

## Behavioral contract

For every draw, the command validates that a Palette is present and that every mesh element index is valid for it, binds that Palette's SSBO, and draws through the common voxel shader. No previously bound Palette may silently satisfy a command with a missing Palette. Palette changes affect material output only; mesh bytes, mesh-cache identity, source volume, and mesher invocation count remain unchanged.

## Acceptance tests

### Nominal behavior and integration interactions

- [ ] Given the declared fixture and valid dependency state, when the public operation is performed, `One generalized mesh representation can be rendered for both a Chunk-derived mesh and a VoxelModel-derived mesh.` is observed exactly; no private-state shortcut is used.
- [ ] Given the declared fixture and valid dependency state, when the public operation is performed, `Object-local material variation moves with a barrel/sword and does not swim as the object moves through world space.` is observed exactly; no private-state shortcut is used.
- [ ] Existing atlas-backed Definition fixture continues to render during migration.
- [ ] A base-color/palette Material can render a new definition without changing mesher logic.
- [ ] Missing required material binding fails visibly/predictably rather than sampling garbage.
- [ ] Invalid input is rejected atomically and leaves the previously valid state unchanged.
- [ ] Identical deterministic inputs produce identical observable results.
- [ ] The exact lower and upper supported boundaries succeed; one-step-outside values are rejected before mutation.
- [ ] A rejected command leaves state, ownership, resources, version counters, scheduled work, and emitted authoritative events byte-for-byte or semantically unchanged.
- [ ] Repeating the same seed, configuration, starting snapshot, and ordered commands produces the same result and event order.
- [ ] Create → use → serialize where applicable → unload → restore/recreate → retry preserves stable IDs and does not duplicate the operation.
- [ ] The nearest upstream and downstream contracts named in prerequisites are exercised together; dependency failure follows the documented fail-closed or rollback behavior.

- [ ] `VoxelRenderCommand` binds one independently owned Palette SSBO for the current renderable; the shader uses flat integer `paletteElementIndex` and has no global PaletteCollection buffer or GPU `paletteId` lookup.
- [ ] The same cached mesh rendered with `HumanPalette` and `OrcPalette` produces two reviewed images while mesh bytes and mesh-cache identity remain identical.
- [ ] Missing Palette, an empty Palette, and an out-of-range element index fail before draw with no stale prior-Palette binding accepted as success.
- [ ] Chunk and VoxelModel commands use the same vertex semantics and shader; only their bound Palette resources and transforms differ.
- [ ] A command trace proves the binding order: voxel program → vertex/index buffers → selected Palette SSBO → transform/render data → draw.
- [ ] Two consecutive commands using different Palettes observe their own Palette values; command B cannot sample command A's buffer.
- [ ] Two consecutive commands sharing `WorldPalette` may avoid a redundant bind only if the observable command result is unchanged.
- [ ] A polygon with element `2` carries integer `2` on all of its vertices and reaches fragment processing without interpolation.
- [ ] Model and Chunk meshes using element `2` both read entry `2` from their currently bound, potentially different, Palettes.
- [ ] Changing a renderable from `HumanPalette` to compatible `OrcPalette` leaves positions, normals, indices, cache key, source model, and mesher-call counter unchanged.
- [ ] A mesh referencing index `3` is rejected against a three-entry Palette whose valid indices are `0..2`; no draw is submitted.
- [ ] A Palette resource destroyed or unavailable before command execution follows the validated resource-lifetime failure contract; the implementation must ask the user if that contract is not already defined by Sparkle.

### Boundaries and invalid/rejected operations

- [ ] Missing IDs, foreign ownership, malformed content, stale versions, and unsupported enum/tag values are rejected with the documented error category.
- [ ] Empty/minimum/maximum fixtures are exercised where the public contract permits them; unsupported empty state is rejected atomically.

### Determinism and lifecycle / retry / persistence

- [ ] Deterministic iteration never depends on pointer values, hash-table accident, render frame rate, or wall-clock timing.
- [ ] A duplicate/retried operation is either idempotent or rejected as already applied, according to the story contract, without duplicating state or events.

### Rendering / golden images

- [ ] Render the deterministic fixture at `512 × 512`; compare against its reviewed PNG with the tolerance recorded by the Sparkle TestLibrary fixture. On failure, retain the old expected image and publish actual/difference images for review.
- [ ] Assert semantic geometry/material/transform values independently of the PNG comparison.

## Rendering impact

Yes. The controlled fixture uses a fixed camera, viewport, asset set, lighting, and supported GPU runner. Structural refactors must match the reviewed current baseline; intentional migrations require human review before a new versioned baseline is accepted.

## Open decisions

- [OD-018](../../../open-decisions/OD-018-first-material-effects-beyond-palette-color.md) — Does not block base Palette binding; blocks only effects beyond the agreed Palette material lookup.
- [OD-019](../../../open-decisions/OD-019-palette-variation-algorithm-and-sampling-declaration.md) — Does not block direct element lookup; blocks procedural/variation sampling behavior.
- [OD-021](../../../open-decisions/OD-021-serialized-derivative-mesh-cache-versus-rebuild-on-load.md) — Does not block runtime Palette binding; blocks only serialized derivative mesh-cache policy.

## Completion evidence

- Automated test names and passing CI run.
- Exact fixtures and expected values checked into the test resources.
- Error-path assertion proving no partial mutation.
- Decision-file update and user approval reference when a gate was resolved.
- For graphical work: expected, actual, and difference-image artifacts plus approval of any changed baseline.
