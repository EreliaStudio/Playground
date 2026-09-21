# OD-028 — WorldPalette terrain mapping and precedence

## Status

Open

## Problem

ST-033-01 needs a deterministic rule that maps each emitted Chunk polygon to one element of the shared `WorldPalette`.

The meshing contract already exposes two independent inputs:

- `Shape::Polygon::materialSlot`, which expresses authored semantic intent such as `top`, `side`, or another Shape-defined slot;
- `Shape::Polygon::outerSide`, which expresses the polygon's spatial boundary side when applicable.

The architecture also anticipates:

- a per-Definition default mapping;
- slot-specific mappings;
- side-specific mappings;
- exact slot+side mappings.

What remains undefined is how those mappings are represented semantically and which one wins when several match the same polygon.

Without an explicit rule, ST-033-01 would have to invent content behavior and later terrain assets could depend accidentally on an implementation detail.

## Affected epics/tickets

- EP-033 / ST-033-01 directly depends on this decision.
- Later Chunk material/content-schema work in EP-033 must preserve the selected semantics.
- EP-030/EP-031 authoring/import work may need to represent the selected mapping without redefining it.

## Already-fixed constraints

- Chunks normally render with one shared `WorldPalette`.
- `MaterialResolver` receives immutable volume, Definition, transformed polygon, coordinate, and Cell context and returns one `Palette::ElementIndex`.
- `materialSlot` and `outerSide` are distinct concepts.
- The mesh/shader contract must not gain a GPU palette ID.
- Palette/material changes do not alter geometry or require a remesh unless the resolved per-polygon palette element itself changes as part of a rebuild.
- The broader `Material` / `MaterialCollection` abstraction is not introduced merely to resolve this decision.
- Procedural palette variation is governed separately by OD-019.
- Effects beyond base Palette color are governed separately by OD-018.

## Questions to resolve

1. What is the precedence among exact slot+side, slot-only, side-only, and default mappings?
2. Is the mapping semantically per Voxel Definition, global, or layered in another way?
3. Is a default mapping mandatory?
4. How does `Side::None` participate in lookup?
5. What happens when a requested mapping is absent: fallback or rejection?
6. Should the selected rule be independent of the eventual serialized schema/API representation?

## Options considered

Pending owner discussion.

## Decision

Pending.

## Rationale

Pending.

## Consequences

Until resolved, ST-033-01 must not freeze terrain material-mapping behavior or its content schema.

## Validation / evidence

The selected rule should be validated with the smallest deterministic fixture that proves:

- one-material fallback;
- distinct top/side/bottom behavior;
- one exact per-side override;
- absence of Chunk-boundary seams caused by lookup semantics;
- exact resolved Palette indices before golden-image comparison.

## Resolution provenance

Pending project-owner approval on the H0 open-decision review branch.
