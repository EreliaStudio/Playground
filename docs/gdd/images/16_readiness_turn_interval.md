# Illustration 16 — Readiness, Turn Interval and Surprise

**Target output file:** `images/16_readiness_turn_interval.png`  
**GDD section:** 15. Surprise / 17.1 Turn Interval / 17.2 Readiness State  
**Suggested placement:** After Section 17.2.  
**Intended GDD size:** Half-width  
**Suggested canvas:** 16:9 landscape

## Purpose

Clarify that lower Turn Interval means a shorter readiness cycle, and that Surprise changes initial readiness instead of creating a separate pre-combat attack phase.

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

Create a compact **design diagram**, not a full combat HUD.

Upper part:
- two horizontal readiness tracks start together;
- Unit A has a visibly shorter total track labelled `4 s`;
- Unit B has a longer track labelled `7 s`;
- both advance through comparable time increments;
- Unit A reaches `Ready` earlier because its cycle itself is shorter.

Lower inset:
- small encounter-start sketch where the surprising side's initial units begin with their readiness indicators already at `Ready`;
- opposing unaware-side units begin at a normal initial readiness state.

The illustration must communicate readiness, not an internal Encounter Clock.

## Text inside the image

Only `4 s`, `7 s`, and `Ready`.

## Important exclusions

No clock face, stopwatch, countdown timer, `Encounter Clock` HUD, or implication that both units have equal-length bars filling at different animation speeds.

Also avoid:
- long captions inside the artwork;
- bullet lists inside the artwork;
- a “Key Takeaways” panel;
- presenting this as a finished commercial screenshot;
- repeating the surrounding GDD paragraph as text.

## Image-generation brief

Create **Illustration 16 — Readiness, Turn Interval and Surprise** for the Erelia Game Design Document.

It must be a **grayscale hand-drawn graphite / gray-pencil concept sketch on a very light paper background**, with clean readable silhouettes and restrained pencil shading. It should look like an experienced game designer or concept artist sketched it specifically to accompany a paragraph in a professional GDD.

Create a compact **design diagram**, not a full combat HUD.

Upper part:
- two horizontal readiness tracks start together;
- Unit A has a visibly shorter total track labelled `4 s`;
- Unit B has a longer track labelled `7 s`;
- both advance through comparable time increments;
- Unit A reaches `Ready` earlier because its cycle itself is shorter.

Lower inset:
- small encounter-start sketch where the surprising side's initial units begin with their readiness indicators already at `Ready`;
- opposing unaware-side units begin at a normal initial readiness state.

The illustration must communicate readiness, not an internal Encounter Clock.

Use the surrounding empty paper space intelligently so the image remains easy to place directly inside a Markdown GDD. Preserve the intended `16:9 landscape` composition. Keep explanatory text to the minimum specified above.

The result should communicate the mechanic through **space, state, composition, movement or object relationships**, rather than by turning the image into an infographic that repeats the written rules.
