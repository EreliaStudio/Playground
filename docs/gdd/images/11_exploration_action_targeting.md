# Illustration 11 — Exploration Action Targeting

**Target output file:** `images/11_exploration_action_targeting.png`  
**GDD section:** 11. Exploration Traversal Actions  
**Suggested placement:** After Section 11.2 World Clock Continues.  
**Intended GDD size:** Full-width  
**Suggested canvas:** 2.4:1 landscape

## Purpose

Show how continuous third-person exploration temporarily reuses tactical cell targeting for actions such as Jump or Climb without pausing the World.

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

Create a **four-frame sequence**.

Frame 1:
- third-person exploration;
- controlled Hero stands at the edge of a gap or ledge;
- followers are behind.

Frame 2:
- camera has transitioned to a simplified top-down or high-angle targeting view;
- only valid destination cells for Jump are lightly outlined or shaded.

Frame 3:
- one destination cell is selected;
- show the Hero executing the jump.

Frame 4:
- normal third-person exploration resumes on the far side.

Across Frames 2 and 3, include a small patrolling or chasing enemy whose position changes slightly, clearly implying the surrounding World continued to run while the player targeted the action.

Do not show an Encounter Clock or pause timer.

## Text inside the image

None.

## Important exclusions

No global pause symbol, no separate traversal resource unless already visually neutral, no platformer-style free jumping.

Also avoid:
- long captions inside the artwork;
- bullet lists inside the artwork;
- a “Key Takeaways” panel;
- presenting this as a finished commercial screenshot;
- repeating the surrounding GDD paragraph as text.

## Image-generation brief

Create **Illustration 11 — Exploration Action Targeting** for the Erelia Game Design Document.

It must be a **grayscale hand-drawn graphite / gray-pencil concept sketch on a very light paper background**, with clean readable silhouettes and restrained pencil shading. It should look like an experienced game designer or concept artist sketched it specifically to accompany a paragraph in a professional GDD.

Create a **four-frame sequence**.

Frame 1:
- third-person exploration;
- controlled Hero stands at the edge of a gap or ledge;
- followers are behind.

Frame 2:
- camera has transitioned to a simplified top-down or high-angle targeting view;
- only valid destination cells for Jump are lightly outlined or shaded.

Frame 3:
- one destination cell is selected;
- show the Hero executing the jump.

Frame 4:
- normal third-person exploration resumes on the far side.

Across Frames 2 and 3, include a small patrolling or chasing enemy whose position changes slightly, clearly implying the surrounding World continued to run while the player targeted the action.

Do not show an Encounter Clock or pause timer.

Use the surrounding empty paper space intelligently so the image remains easy to place directly inside a Markdown GDD. Preserve the intended `2.4:1 landscape` composition. Keep explanatory text to the minimum specified above.

The result should communicate the mechanic through **space, state, composition, movement or object relationships**, rather than by turning the image into an infographic that repeats the written rules.
