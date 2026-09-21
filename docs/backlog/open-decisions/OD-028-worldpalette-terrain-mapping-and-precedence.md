# OD-028 — WorldPalette semantic slot binding and visual fallback

## Status

Resolved

## Problem

ST-033-01 needs a deterministic and simple rule that maps each emitted Chunk polygon to one element of the shared `WorldPalette`.

The Shape already gives each polygon one human-authored semantic `materialSlot`, such as `top`, `bottom`, or `side`. A Voxel Definition must be able to reuse the same Shape while choosing its own human-readable material identifiers, such as `dirt-top`, `dirt-side`, or `stone-top`.

The runtime renderer ultimately needs a `Palette::ElementIndex`, not a string. Missing visual bindings should remain visible to developers without preventing the game from continuing.

## Decision

### One semantic slot per polygon

Each `Shape::Polygon` continues to expose exactly one semantic `materialSlot`.

A Shape decides how much distinction it needs. A simple cube may give all four lateral faces the same `side` slot. Another Shape may instead expose distinct slots for individual faces.

No automatic material precedence based on `outerSide` is introduced.

A possible future extension may allow an ordered list of alternative slots, with the current string form acting as a one-element list. Multi-slot lookup semantics are deliberately not defined until a concrete content use case requires them.

### Definition maps Shape slot to a human-readable material identifier

A Voxel Definition provides the semantic mapping:

```text
Shape slot
    ↓
Voxel Definition material identifier
    ↓
MaterialResolver binding
    ↓
Palette::ElementIndex
```

Example:

```text
Shape polygon slot: "top"

Grass Definition:
    "top" -> "dirt-top"

MaterialResolver:
    "dirt-top" -> 15

WorldPalette:
    element 15
```

The authored material identifier remains a string. The runtime mesh/shader receives only `Palette::ElementIndex`.

### MaterialResolver binds authored IDs to Palette indices

The terrain resolver provides explicit bindings equivalent to:

```cpp
resolver.bind("dirt-top", 15);
resolver.bind("dirt-side", 16);
resolver.bind("stone-top", 17);
```

The concrete resolver may hold or otherwise reference the Palette it resolves against. The existing abstract `MaterialResolver` contract may remain focused on returning one `Palette::ElementIndex`.

### Palette owns the fallback index

`Palette` gains an explicit default/fallback element index selected with a method named:

```cpp
palette.setDefault(index);
```

The selected index must address a valid element of that Palette according to the Palette's normal validation contract.

For the H0 terrain prototype, the default element is an opaque plain magenta color so unresolved visual content is immediately visible.

No palette index is universally reserved as a magic fallback value.

### Missing visual bindings are recoverable

Both of these conditions resolve to the Palette default instead of aborting meshing:

1. the Shape polygon references a slot that the Voxel Definition does not map;
2. the Voxel Definition maps a slot to a material identifier that the MaterialResolver does not bind.

These are visual-content errors, not structural/runtime-fatal errors.

They must not throw merely to stop the game. The unresolved polygon is emitted using `palette.default` / the index configured through `setDefault()`.

Malformed structural data and genuinely invalid runtime state may still use the project's normal `spk::Exception` policy.

### Warning diagnostics are intentionally deferred

Sparkle `0.1.2` has no reusable logger/warning facility.

Once Sparkle provides logging, each recoverable missing visual binding should emit a warning identifying enough context to locate the bad content, while continuing with the Palette default.

Until that logger is available in the Playground-consumed Sparkle package, the fallback behavior is implemented without a console-warning requirement. Missing logging must not block ST-033-01.

A separate Sparkle issue will request the logger for the `0.1.3` line.

## Rationale

This keeps material lookup data-driven and intentionally small:

- Shapes own semantic polygon slots;
- Definitions assign human-readable material meaning to those slots;
- the resolver translates stable authored identifiers to GPU-facing Palette element indices;
- the Palette owns the visual fallback element.

Directional specialization does not require special resolver rules. Content that needs per-face distinctions can use a Shape exposing distinct per-face slot names.

The magenta fallback keeps missing visual content conspicuous without turning a render-content mistake into a game-stopping exception.

## Consequences

- The previously proposed default/slot/side/exact precedence system is removed.
- `outerSide` remains useful geometry/occlusion metadata but is not part of this material lookup rule.
- No broader `Material` / `MaterialCollection` abstraction is required by this decision.
- No procedural Palette variation is defined here; OD-019 remains separate.
- No Material effect beyond current Palette data is defined here; OD-018 remains separate.
- A future multi-slot Shape syntax remains possible without changing the single-slot contract already authored today.
- ST-033-01 may implement direct semantic slot bindings and magenta fallback before Sparkle logging support exists.

## Validation / evidence

ST-033-01 should prove at minimum:

- one Shape reused by multiple Definitions with different human-readable bindings;
- a simple cube with shared lateral `side` semantics;
- a Shape may expose more specific face slots without resolver-side directional precedence;
- exact resolved Palette indices for valid bindings;
- missing Definition slot mapping resolves to the configured Palette default;
- missing resolver binding resolves to the configured Palette default;
- the fallback element renders as plain opaque magenta;
- no recoverable missing visual binding aborts meshing;
- once logger support is consumed, the recoverable cases emit warnings.

## Resolution provenance

Resolved by the project owner during the H0 open-decision review on 21 September 2026:

- one semantic Shape slot per polygon for the current contract;
- Definition slot -> human-readable material identifier;
- MaterialResolver identifier -> `Palette::ElementIndex`;
- `Palette::setDefault(index)` owns the fallback element index;
- opaque magenta is the H0 fallback appearance;
- missing visual slot/material bindings use fallback rather than exceptions;
- warning output is deferred until a Sparkle logger is available.
