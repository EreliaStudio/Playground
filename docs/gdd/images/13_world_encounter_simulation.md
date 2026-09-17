# Illustration 13 — World and Encounter Simulation

**Target output file:** `images/13_world_encounter_simulation.png`  
**GDD section:** 13. Encounter Time Model  
**Suggested placement:** After Section 13.3 Paused Tactical Decision-Making.  
**Intended GDD size:** Full-width  
**Suggested canvas:** 16:9 landscape

## Purpose

Show that several tactical encounters can exist inside one continuously running region and that pausing one player's tactical decision does not freeze the MMO world.

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

Draw one large voxel region from a high isometric viewpoint.

Within the same region:
- normal exploration entities continue moving outside combat, indicated by small movement arrows;
- one enemy patrol continues along a route;
- another player or party moves elsewhere.

Also show two distinct local encounter areas embedded in that same world:
- Encounter A contains a player-controlled unit waiting for a decision; use a small subtle `Paused` marker on the **diagram**, not as an in-game timer;
- Encounter B is still resolving, with movement/action arrows.

The encounters should feel like localized simulation domains inside the same physical region, not separate loaded arenas.

This is a technical GDD diagram. It may label simulation domains, but it must not depict an Encounter Clock timer as player-visible UI.

## Text inside the image

`World / Region`, `Encounter A`, `Encounter B`, and optionally `Paused`.

## Important exclusions

Absolutely no visible `Encounter Clock` countdown, stopwatch, clock icon, or HUD timer.

Also avoid:
- long captions inside the artwork;
- bullet lists inside the artwork;
- a “Key Takeaways” panel;
- presenting this as a finished commercial screenshot;
- repeating the surrounding GDD paragraph as text.

## Image-generation brief

Create **Illustration 13 — World and Encounter Simulation** for the Erelia Game Design Document.

It must be a **grayscale hand-drawn graphite / gray-pencil concept sketch on a very light paper background**, with clean readable silhouettes and restrained pencil shading. It should look like an experienced game designer or concept artist sketched it specifically to accompany a paragraph in a professional GDD.

Draw one large voxel region from a high isometric viewpoint.

Within the same region:
- normal exploration entities continue moving outside combat, indicated by small movement arrows;
- one enemy patrol continues along a route;
- another player or party moves elsewhere.

Also show two distinct local encounter areas embedded in that same world:
- Encounter A contains a player-controlled unit waiting for a decision; use a small subtle `Paused` marker on the **diagram**, not as an in-game timer;
- Encounter B is still resolving, with movement/action arrows.

The encounters should feel like localized simulation domains inside the same physical region, not separate loaded arenas.

This is a technical GDD diagram. It may label simulation domains, but it must not depict an Encounter Clock timer as player-visible UI.

Use the surrounding empty paper space intelligently so the image remains easy to place directly inside a Markdown GDD. Preserve the intended `16:9 landscape` composition. Keep explanatory text to the minimum specified above.

The result should communicate the mechanic through **space, state, composition, movement or object relationships**, rather than by turning the image into an infographic that repeats the written rules.
