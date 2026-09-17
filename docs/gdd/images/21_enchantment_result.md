# Illustration 21 — Enchantment Result and Failure

**Target output file:** `images/21_enchantment_result.png`  
**GDD section:** 27. Enchantment  
**Suggested placement:** Between Sections 27.1 Deterministic Result and 27.2 Success Chance.  
**Intended GDD size:** Half-width  
**Suggested canvas:** 16:9 landscape

## Purpose

Separate deterministic transformation from probabilistic operation success: the chosen result is known, but the attempt may fail.

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

Use a simple branching **before / outcome** illustration.

Center-left:
- one base sword or piece of equipment;
- beside it, a small pile of magical resources and gold being consumed for an enchantment operation.

Success branch:
- the same item now carries a small `Flame` Tag emblem;
- optionally show one short reduced-stat indicator, such as a tiny shortened Physical Power bar, to imply the known tradeoff.

Failure branch:
- the item remains visually identical to the original;
- the consumed resource/gold pile is gone.

The Flame Tag must appear as semantic metadata, not as automatic literal fire damage or a permanently burning weapon.

## Text inside the image

`Flame`, and optionally `Success` / `Failure`.

## Important exclusions

No random roulette of different possible Tags. No automatic flaming sword effect implying the Tag itself deals fire damage.

Also avoid:
- long captions inside the artwork;
- bullet lists inside the artwork;
- a “Key Takeaways” panel;
- presenting this as a finished commercial screenshot;
- repeating the surrounding GDD paragraph as text.

## Image-generation brief

Create **Illustration 21 — Enchantment Result and Failure** for the Erelia Game Design Document.

It must be a **grayscale hand-drawn graphite / gray-pencil concept sketch on a very light paper background**, with clean readable silhouettes and restrained pencil shading. It should look like an experienced game designer or concept artist sketched it specifically to accompany a paragraph in a professional GDD.

Use a simple branching **before / outcome** illustration.

Center-left:
- one base sword or piece of equipment;
- beside it, a small pile of magical resources and gold being consumed for an enchantment operation.

Success branch:
- the same item now carries a small `Flame` Tag emblem;
- optionally show one short reduced-stat indicator, such as a tiny shortened Physical Power bar, to imply the known tradeoff.

Failure branch:
- the item remains visually identical to the original;
- the consumed resource/gold pile is gone.

The Flame Tag must appear as semantic metadata, not as automatic literal fire damage or a permanently burning weapon.

Use the surrounding empty paper space intelligently so the image remains easy to place directly inside a Markdown GDD. Preserve the intended `16:9 landscape` composition. Keep explanatory text to the minimum specified above.

The result should communicate the mechanic through **space, state, composition, movement or object relationships**, rather than by turning the image into an infographic that repeats the written rules.
