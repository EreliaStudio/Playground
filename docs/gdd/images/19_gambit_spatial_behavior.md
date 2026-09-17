# Illustration 19 — Gambit AI in Space

**Target output file:** `images/19_gambit_spatial_behavior.png`  
**GDD section:** 24. Enemy AI — Gambit System  
**Suggested placement:** Beside or immediately after the Goblin Archer example.  
**Intended GDD size:** Half-width  
**Suggested canvas:** 2.2:1 landscape

## Purpose

Translate an ordered Condition → Target → Reaction rule list into visible tactical behavior instead of repeating the rules as text.

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

Draw three small top-down tactical situations featuring the **same enemy archer**.

Situation 1:
- a Hero is too close;
- the archer has a movement arrow leading away from the Hero.

Situation 2:
- a Hero is within the archer's preferred band;
- the archer has a clean firing line toward the valid target.

Situation 3:
- the Hero is too far away;
- the archer has a movement arrow leading toward a better range band.

Use tiny `1`, `2`, `3` markers to imply ordered rule priority.

Do not reproduce the Gambit rule text in the image.

## Text inside the image

Only `1`, `2`, `3` if useful.

## Important exclusions

No long IF/THEN labels, no behavior-tree infographic, no class labels.

Also avoid:
- long captions inside the artwork;
- bullet lists inside the artwork;
- a “Key Takeaways” panel;
- presenting this as a finished commercial screenshot;
- repeating the surrounding GDD paragraph as text.

## Image-generation brief

Create **Illustration 19 — Gambit AI in Space** for the Erelia Game Design Document.

It must be a **grayscale hand-drawn graphite / gray-pencil concept sketch on a very light paper background**, with clean readable silhouettes and restrained pencil shading. It should look like an experienced game designer or concept artist sketched it specifically to accompany a paragraph in a professional GDD.

Draw three small top-down tactical situations featuring the **same enemy archer**.

Situation 1:
- a Hero is too close;
- the archer has a movement arrow leading away from the Hero.

Situation 2:
- a Hero is within the archer's preferred band;
- the archer has a clean firing line toward the valid target.

Situation 3:
- the Hero is too far away;
- the archer has a movement arrow leading toward a better range band.

Use tiny `1`, `2`, `3` markers to imply ordered rule priority.

Do not reproduce the Gambit rule text in the image.

Use the surrounding empty paper space intelligently so the image remains easy to place directly inside a Markdown GDD. Preserve the intended `2.2:1 landscape` composition. Keep explanatory text to the minimum specified above.

The result should communicate the mechanic through **space, state, composition, movement or object relationships**, rather than by turning the image into an infographic that repeats the written rules.
