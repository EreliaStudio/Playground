# Illustration 23 — Deterministic Dungeon Rotation

**Target output file:** `images/23_dungeon_rotation.png`  
**GDD section:** 32.2 Handcrafted Modular Rooms / 32.3 Deterministic Rotation  
**Suggested placement:** After Section 32.3.  
**Intended GDD size:** Full-width  
**Suggested canvas:** 2.3:1 landscape

## Purpose

Show the relationship between handcrafted room modules, stable layout within one rotation, reset runtime state, and a different deterministic layout after the rotation changes.

## Shared visual direction

Create this as a **hand-drawn game-design illustration**, not a polished game screenshot.

The visual language must be consistent with the rest of the Erelia GDD:

- grayscale graphite / gray pencil / gray crayon;
- visible but controlled sketch strokes;
- clean concept-art line work;
- white or very light warm paper-like background;
- restrained shading made with pencil hatching or soft graphite;
- voxel-fantasy forms, terrain and architecture;
- readable silhouettes and spatial relationships;
- professional GDD concept-sketch quality;
- no glossy 3D render;
- no photorealism;
- no painterly full-color concept art;
- no giant infographic layout;
- no decorative poster typography;
- no long explanatory text;
- no paragraph-shaped labels;
- very limited UI unless the prompt explicitly asks for UI;
- no fake screenshot chrome.

Subtle gray-value differences may be used to distinguish states, reachable zones, borders, active/inactive elements, or foreground/background. A **single restrained accent tone** may be used only if it materially improves readability, but grayscale should remain dominant.

Keep the drawing relatively clean and intentionally unfinished, as though an experienced game designer or concept artist sketched it specifically to sit between paragraphs of a professional design document.

## Global Erelia fidelity rules

Do not introduce generic MMORPG conventions that are absent from the GDD.

In particular:

- no character levels;
- no class labels such as Tank, Mage, Warrior, Healer, Rogue, etc.;
- no experience bars;
- no profession levels;
- no Mana/Energy/Rage bar unless explicitly requested;
- do not depict editable/destructible Minecraft-like terrain;
- permanent voxel terrain is immutable during ordinary gameplay;
- Heroes are classless and are defined through equipment, item Types, item Tags, stats, eight equipped spells, and tactical choices;
- an adventuring player controls an active squad of three Heroes;
- AP is for spells/combat actions and MP is for tactical movement;
- the Encounter Clock is an internal simulation concept and must **never** appear as a visible HUD timer;
- when combat time must be visible to the player, use readiness/turn-state imagery instead;
- combat terrain comes from the same voxel environment used during exploration.

## Required image content

Create three simplified dungeon-map panels built from visibly reusable handcrafted room shapes.

Panel 1 — `Rotation N`:
- a particular arrangement of room modules and connectors;
- small enemy/trap/door symbols occupy the rooms.

Panel 2 — `Re-entry` during the same rotation:
- **exactly the same structural room arrangement**;
- runtime symbols such as enemies, traps or doors are reset/refreshed.

Panel 3 — `Rotation N+1`:
- use recognizably the same room-module vocabulary;
- assemble the modules into a different valid layout.

The structural distinction must be obvious without explaining the seed formula.

## Text inside the image

Only `Rotation N`, `Re-entry`, `Rotation N+1`.

## Important exclusions

Do not make the second panel structurally different from the first. Do not depict every room as purely random geometry.

Also avoid:
- long captions inside the artwork;
- bullet lists inside the artwork;
- a “Key Takeaways” panel;
- presenting this as a finished commercial screenshot;
- repeating the surrounding GDD paragraph as text.

## Image-generation brief

Create **Illustration 23 — Deterministic Dungeon Rotation** for the Erelia Game Design Document.

It must be a **grayscale hand-drawn graphite / gray-pencil concept sketch on a very light paper background**, with clean readable silhouettes and restrained pencil shading. It should look like an experienced game designer or concept artist sketched it specifically to accompany a paragraph in a professional GDD.

Create three simplified dungeon-map panels built from visibly reusable handcrafted room shapes.

Panel 1 — `Rotation N`:
- a particular arrangement of room modules and connectors;
- small enemy/trap/door symbols occupy the rooms.

Panel 2 — `Re-entry` during the same rotation:
- **exactly the same structural room arrangement**;
- runtime symbols such as enemies, traps or doors are reset/refreshed.

Panel 3 — `Rotation N+1`:
- use recognizably the same room-module vocabulary;
- assemble the modules into a different valid layout.

The structural distinction must be obvious without explaining the seed formula.

Use the surrounding empty paper space intelligently so the image remains easy to place directly inside a Markdown GDD. Preserve the intended `2.3:1 landscape` composition. Keep explanatory text to the minimum specified above.

The result should communicate the mechanic through **space, state, composition, movement or object relationships**, rather than by turning the image into an infographic that repeats the written rules.
