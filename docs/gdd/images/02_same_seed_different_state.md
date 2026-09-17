# Illustration 02 — Same Seed, Different Server State

**Target output file:** `images/02_same_seed_different_state.png`  
**GDD section:** 3.2 Server Seed / 3.3 Immutable Voxel Environment  
**Suggested placement:** After the paragraph explaining that two servers can deliberately use the same ServerSeed.  
**Intended GDD size:** Half-width  
**Suggested canvas:** 3:2 landscape

## Purpose

Show that deterministic generation reproduces the same base World geometry while each server still owns separate dynamic progression and runtime state.

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

Create a clean **two-column comparison** of the same generated voxel landscape.

The left and right landscapes must be unmistakably identical in permanent geometry: same mountain silhouette, same river bend, same road, same bridge, same dungeon entrance, same cliff shapes, and same dormant-crystal location.

Change only dynamic server state.

For example:
- in Server A, the crystal site is an active outpost and the distant World portal is active;
- in Server B, the same crystal is dormant and the portal remains inactive;
- optionally show different active enemy-camp occupancy at predefined valid sites.

The point is that the physical World itself is the same while progression/runtime state differs.

## Text inside the image

Only `Server A` and `Server B`.

## Important exclusions

Do not change terrain shape between the two columns. Do not show player-built terrain or arbitrary voxel construction.

Also avoid:
- long captions inside the artwork;
- bullet lists inside the artwork;
- a “Key Takeaways” panel;
- presenting this as a finished commercial screenshot;
- repeating the surrounding GDD paragraph as text.

## Image-generation brief

Create **Illustration 02 — Same Seed, Different Server State** for the Erelia Game Design Document.

It must be a **grayscale hand-drawn graphite / gray-pencil concept sketch on a very light paper background**, with clean readable silhouettes and restrained pencil shading. It should look like an experienced game designer or concept artist sketched it specifically to accompany a paragraph in a professional GDD.

Create a clean **two-column comparison** of the same generated voxel landscape.

The left and right landscapes must be unmistakably identical in permanent geometry: same mountain silhouette, same river bend, same road, same bridge, same dungeon entrance, same cliff shapes, and same dormant-crystal location.

Change only dynamic server state.

For example:
- in Server A, the crystal site is an active outpost and the distant World portal is active;
- in Server B, the same crystal is dormant and the portal remains inactive;
- optionally show different active enemy-camp occupancy at predefined valid sites.

The point is that the physical World itself is the same while progression/runtime state differs.

Use the surrounding empty paper space intelligently so the image remains easy to place directly inside a Markdown GDD. Preserve the intended `3:2 landscape` composition. Keep explanatory text to the minimum specified above.

The result should communicate the mechanic through **space, state, composition, movement or object relationships**, rather than by turning the image into an infographic that repeats the written rules.
