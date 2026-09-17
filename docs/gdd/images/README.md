# Erelia GDD — Illustration Generation Prompts

This folder contains one standalone Markdown prompt for each planned Erelia GDD illustration.

Each file is intended to be given directly to an image-generation model. The files deliberately repeat the common style and mechanical constraints so they can be used independently.

## Shared art direction

All illustrations should look like hand-drawn game-design sketches:

- grayscale graphite / gray pencil / gray crayon;
- white or very light paper-like background;
- visible, controlled pencil strokes;
- voxel-fantasy visual language;
- clean silhouettes;
- restrained detail;
- minimal text;
- suitable for direct insertion between GDD paragraphs;
- not polished screenshots;
- not giant infographic posters.

## Output convention

The prompt files already specify stable output paths of the form:

```text
images/01_frontier_expedition.png
images/02_same_seed_different_state.png
...
images/25_rotating_enemy_camps.png
```

The GDD can then embed an illustration with Markdown such as:

```md
![A Hero party looking toward the frontier](images/01_frontier_expedition.png "The Frontier Expedition")
```

## Files

- `01_frontier_expedition.md` — The Frontier Expedition
- `02_same_seed_different_state.md` — Same Seed, Different Server State
- `03_sparse_world_civilization.md` — Sparse Civilization Across Worlds
- `04_world_portal_activation.md` — Grand Dungeon Portal Unlock
- `05_independent_town_buildings.md` — Independent Town Building Progression
- `06_outpost_lifecycle.md` — Temporary Outpost Lifecycle
- `07_roster_active_squad.md` — Roster and Active Squad
- `08_classless_loadout_logic.md` — Classless Loadout Logic
- `09_physical_followers_formation.md` — Physical Followers and Formation
- `10_detection_chase.md` — Detection and Real-Time Chase
- `11_exploration_action_targeting.md` — Exploration Action Targeting
- `12_voxel_traversal_surfaces.md` — Voxel Traversal Surfaces
- `13_world_encounter_simulation.md` — World and Encounter Simulation
- `14_encounter_handoff.md` — Encounter Handoff Synchronization
- `15_reinforcement_elastic_area.md` — Reinforcement and Elastic Combat Area
- `16_readiness_turn_interval.md` — Readiness, Turn Interval and Surprise
- `17_spell_targeting_geometry.md` — Tactical Targeting Geometry
- `18_flee_wait_outside_encounter.md` — Fleeing an Encounter
- `19_gambit_spatial_behavior.md` — Gambit AI in Space
- `20_shared_resource_node.md` — Shared Resource Node State
- `21_enchantment_result.md` — Enchantment Result and Failure
- `22_dungeon_multiple_entrances.md` — Multiple Dungeon Entrances
- `23_dungeon_rotation.md` — Deterministic Dungeon Rotation
- `24_marketplace_pve_reward.md` — Marketplace-Funded PvE Reward
- `25_rotating_enemy_camps.md` — Rotating Enemy Camp Sites

## Recommended first-pass set

If you do not want to generate all 25 immediately, the strongest first-pass set from the illustration review is:

`01, 03, 05, 06, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25`

The secondary/optional set is:

`02, 04, 07, 20`
