# Kanto Pokémon League port — status and next steps

Goal: after Master Rank, Scott takes the player from Lilycove to FireRed's Route 23 and Indigo Plateau, ported
pixel-perfect with their interiors, to challenge Kanto's Elite Four (`TRAINER_*_EXHIBITION`) and Blue.
Source: `~/projects/pokefirered`. Tools: `porting/frlg_tilesets.py`, `porting/frlg_maps.py` (from the
`sevii-one-island` branch, which has the method's full write-up in `porting/SEVII_PORT.md`).

## Done (phases 1-3)

### Tilesets (re-split with `frlg_tilesets.py`, registered in `src/data/tilesets/{graphics,metatiles,headers}.h`)
| Emerald tileset | From FireRed | Used by |
|---|---|---|
| `gTileset_FrlgGeneral` (primary) | General, with its animations (`InitTilesetAnim_FrlgGeneral`) | Route 23, Indigo Plateau |
| `gTileset_FrlgIndigoPlateau` | General tail + Indigo Plateau | Route 23, Indigo Plateau |
| `gTileset_FrlgBuilding` (primary) | Building | Indigo interiors (phase 4) |
| `gTileset_FrlgPokemonCenter` | Building tail + Pokémon Center | Indigo Pokémon Center (phase 4) |
| `gTileset_FrlgPokemonLeague` | Building tail + Pokémon League | Elite Four and Champion rooms (phase 4) |

New behavior approximations in `APPROXIMATE_BEHAVIORS`: Indigo Plateau signs and furniture flavor behaviors
(window, painting, kitchen...) → `MB_NORMAL`; FireRed's diagonal stair warps → Emerald arrow warps; battle records
→ `MB_CABLE_BOX_RESULTS_1`.

### Maps (new group `gMapGroup_Kanto`, appended so no existing map IDs change)
All FireRed blockdata; `frlg_maps.py verify` reports 0 differing blocks and pixels for every one.

| Map | Notes |
|---|---|
| `Route23` (24x160) | No connection to Route 22. Scott stands in front of Victory Road's exit at (18, 29), blocking the cave; he drops the player off at (18, 30) and takes them back to Lilycove. The rest of the route (badge guards, Route 22 gate, Victory Road's entrance) is out of reach, as in FireRed; the gate turns the player back if reached. FireRed's 8 hidden items (flags `0x264-0x26B`) and wild encounters. |
| `IndigoPlateau_Exterior` (24x20) | Pokémon Center door with FireRed's sliding door animation. Heal location `HEAL_LOCATION_INDIGO_PLATEAU` (11, 7). |
| `IndigoPlateau_PokemonCenter_1F` | Nurse, mart (FireRed's stock), Gym Guy, the Elite Four door guard and FireRed's post-game gossipers. Escalator to 2F. |
| `IndigoPlateau_PokemonCenter_2F` (layout `FrlgPokemonCenter_2F`) | Link room: its doors and attendants are where Emerald's are, so Emerald's Cable Club scripts run unchanged. |
| `PokemonLeague_{Loreleis,Brunos,Agathas,Lances}Room` | FireRed's scenes: walking in closes the entry, beating the trainer opens the next door; Lance's corridor walk. Progress shares Ever Grande's `VAR_ELITE_4_STATE` and `FLAG_DEFEATED_ELITE_4_*`, which reset on whiting out and in the Hall of Fame. |
| `PokemonLeague_ChampionsRoom` | Blue (team by the Hoenn starter), his starter gift and Red hint, then Prof. Oak walks in and takes the player to the Hall of Fame. |

### Engine support
- **Doors** (`src/field_door.c`): maps on `gTileset_FrlgGeneral`/`gTileset_FrlgBuilding` use their own door table
  (`sFrlgDoorAnimGraphicsTable`), since FireRed's metatile IDs mean other doors in Emerald. FireRed's one-metatile
  doors get `DOOR_SIZE_FRLG_1x1` and FireRed's frames; two-metatile doors (link room) work like Emerald's.
- **Escalators** (`src/fldeff_escalator.c`): same algorithm as FireRed; FireRed's metatile IDs on `gTileset_FrlgBuilding`.
- **PC and TV** (`src/field_specials.c`, `src/tv.c`): on `gTileset_FrlgBuilding` the PC turns on with FireRed's
  metatiles (0x062/0x063); Emerald's TV-on metatiles are skipped (no Indigo map has a TV).
- **Healing machine** (`src/field_effect.c`): in FireRed's Pokémon Centers the monitor is FireRed's sprite (four
  frames, palette tag `FLDEFF_PAL_TAG_FRLG_POKECENTER_MONITOR`) at FireRed's position (x 128 instead of 124).
- **Lighting** (`DoPokemonLeagueLightingEffect`, `src/field_specials.c`): FireRed's flickering room lights in the
  Elite Four and Champion rooms.
- **Prof. Oak** (`OBJ_EVENT_GFX_PROF_OAK`, was the unused Porygon2 doll): uses `PALSLOT_NPC_SPECIAL_2`, a second special
  NPC palette in the special NPC's reflection slot, so he and Blue keep their FireRed colors together. Only for maps
  without reflections.

### Story (`data/scripts/kanto_league.inc`)
- Scott's call after Master Rank (`ShouldDoScottKantoLeagueCall`, `FLAG_KANTO_LEAGUE_INVITED`).
- Scott by Lilycove's harbor door (13, 33) (`FLAG_HIDE_LILYCOVE_CITY_SCOTT`) → Route 23 (18, 29); Scott in Victory
  Road's exit → Lilycove (12, 33).
- The run ends in Ever Grande's Hall of Fame (`FLAG_KANTO_HALL_OF_FAME`), with Oak in the champion's place.
- Red waits on Mt. Pyre once Blue has been beaten.

## Known differences from FireRed
- Hall of Fame: Ever Grande's (decided), with Oak.
- NPC sprites are Emerald's (Gym Guy, Cooltrainers, receptionists); Kanto's Elite Four, Blue and Oak are FireRed's.
- No Elite Four mugshot transitions for the Kanto battles.
- `MB_ROCK_STAIRS` (slow stairs) and furniture flavor behaviors don't exist in Emerald (see `APPROXIMATE_BEHAVIORS`).
- Battle backgrounds use Emerald's Elite Four arenas (Lorelei: Glacia's, Bruno: Sidney's, Agatha: Phoebe's,
  Lance: Drake's).
