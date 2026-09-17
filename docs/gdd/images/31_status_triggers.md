# Illustration 31 — Status Triggers

**Target output file:** `images/31_status_triggers.png`  
**GDD section:** 20.2 Examples  
**Suggested placement:** At the end of Section 20.2.  
**Intended GDD size:** Full-width  
**Suggested canvas:** 16:9 landscape

## Image-generation brief

Use case: infographic-diagram
Asset type: Illustration 31 — Status Triggers, for direct insertion in the Erelia Game Design Document.
Composition/framing: 16:9 landscape.
Style/medium: grayscale graphite and gray-pencil game-design sketch on very light paper, controlled visible strokes, restrained hatching, clear silhouettes and generous paper margins. Voxel-fantasy terrain and architecture; Heroes have the block-bodied, cube-headed proportions used in the later Erelia illustrations. No color, glossy rendering, photograph, watermark, decorative poster headings or dense explanatory paragraphs.
Global constraints: classless Heroes; no levels, experience, professions, Feats, class badges, Mana/Energy/Rage. Three active Heroes per player whenever a full squad is shown. Permanent voxel terrain is immutable. Equipment Types and Tags are semantic properties, with no automatic effects. AP pays for combat actions and spells; MP pays for tactical movement. Eight equipped spells per Hero. Readiness may be drawn, but never an Encounter Clock HUD timer. Do not imply new established mechanics or fixed numeric defaults.
Primary request: Make a compact precise pencil design diagram with THREE separate horizontal rows and tiny Hero silhouettes. Row Poison: three equally spaced discrete OnTurnStart events, each with a small damage mark; after the third mark the Poison icon fades out. Row Burning: six equal one-second ticks on an abstract technical simulation-time line, each with a small damage mark; at the sixth tick the Burning icon fades out. This row refers to advancing encounter simulation time, which stops during tactical decisions; it is NOT wall-clock time and never a visible player HUD countdown. Row Blood Pact: three variably spaced AP-spending events marked with AP pips, each leading downward to a tiny HP-loss mark; no stated expiration because that is not defined in the GDD. Precisely render only Poison, OnTurnStart, 3 triggers, Burning, EverySecond, 6 Encounter seconds, Blood Pact, OnAPSpent, HP. No clock faces or timer UI; no decorative title; restrained hatching and readable hand lettering.
## Final correction prompt

Use case: precise-object-edit. Input image 1 is the edit target: the Erelia grayscale pencil Status Triggers diagram. Change ONLY the third small Hero pose in the bottom Blood Pact / OnAPSpent row, at the bottom right above the third AP pips. It currently runs with trailing speed lines. Replace this running pose and its speed lines with a stationary, feet-planted cube-headed Hero casting a combat spell from an outstretched hand, with a few small magic spark squares. This is an AP-spending spell, not movement; movement spends MP. Preserve all other artwork, all three rows, every label, arrow, AP pip, HP/damage mark, expiration icon, paper background, grayscale graphite style, image dimensions and composition unchanged. No new text or symbols outside that one small pose.

