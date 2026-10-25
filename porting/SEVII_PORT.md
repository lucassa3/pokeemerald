# Sevii Islands port — status and next steps

Goal: bring FireRed's Sevii Islands into this Emerald hack, starting with One Island, reachable by ferry
from Slateport and Lilycove after the Elite Four. Maps are ported **1:1 from pokefirered using FireRed's own
tilesets** (the Sevii Islands look like FireRed; player, NPCs and UI stay Emerald).

Branch: `sevii-one-island` (from `master`, without the shiny-odds changes).
pokefirered checkout used as the source: `~/projects/pokefirered`.

## Approaches tried (and why this one)

1. Translating FRLG blockdata tile by tile to Emerald's tilesets: rough seams everywhere, because tilesets
   are families of edge/corner pieces that don't map 1:1 and building sizes differ.
2. An Emerald-native redesign (Navel Rock's cliff ring): polished but didn't look like One Island.
3. **Porting FRLG's tilesets (current):** exact, mechanical, scales to every Sevii map.

## How the tileset port works

Both engines have 1024 tiles, 1024 metatiles and 13 BG palettes per map, loaded into one contiguous space;
only the primary/secondary split differs:

|          | tiles     | metatiles | palettes |
|----------|-----------|-----------|----------|
| FRLG     | 640 + 384 | 640 + 384 | 7 + 6    |
| Emerald  | 512 + 512 | 512 + 512 | 6 + 7    |

`porting/frlg_tilesets.py` re-splits an FRLG primary+secondary pair at Emerald's boundaries. Every global
tile, metatile and palette number stays the same, so FRLG `map.bin` files are copied byte for byte. Each FRLG
secondary becomes its own Emerald secondary that starts with the last 128 tiles/metatiles and palette 6 of
FRLG's General tileset. Metatile attributes are converted from FRLG's 32-bit format to Emerald's 16-bit one
by behavior name; FRLG-only behaviors are listed in `APPROXIMATE_BEHAVIORS`.

## What's done

### Tilesets (`data/tilesets/`, registered in `src/data/tilesets/{graphics,metatiles,headers}.h`)
- `gTileset_FrlgGeneral` (primary) — FRLG General, with its animations ported as
  `InitTilesetAnim_FrlgGeneral` in `src/tileset_anims.c` (water/current, sandy shore, flowers).
- `gTileset_FrlgSeviiIslands123` (secondary) — FRLG Sevii Islands 1-2-3.
- `gTileset_FrlgIslandHarbor` (secondary) — FRLG Island Harbor (Seagallop terminal).

### Doors (`src/field_door.c`)
- FRLG door graphics in `graphics/door_anims/frlg_*.png` and `sFrlgDoorAnimGraphicsTable`, used only when the
  map's primary tileset is `gTileset_FrlgGeneral` (FRLG metatile IDs mean other doors in Emerald).
- New `DOOR_SIZE_FRLG`: FRLG doors animate a single metatile with their own frame offsets, as in pokefirered.

### Maps (new map group `gMapGroup_SeviiIslands`, appended so no existing map IDs change)
| Map | Layout | Notes |
|---|---|---|
| `OneIsland` | `LAYOUT_ONE_ISLAND` (FRLG, 24x20) | 1:1 with FRLG; FRLG sign texts; new light NPC dialogue |
| `OneIsland_Harbor` | `LAYOUT_FRLG_ISLAND_HARBOR` (FRLG, 17x13) | 1:1; sailor returns you to the port you came from; S.S. Tidal docked at (8, 8) |
| `OneIsland_PokemonCenter_1F/2F` | Emerald's `LAYOUT_POKEMON_CENTER_1F/2F` | Not FRLG yet (interiors pending) |
| `OneIsland_House1/2` | Emerald's `LAYOUT_HOUSE3` | Not FRLG yet |

- Heal location `HEAL_LOCATION_ONE_ISLAND` at (14, 6), in front of the Net Center.
- Music: `MUS_RG_SEVII_123` (town, houses, harbor); `MUS_POKE_CENTER` inside the Pokémon Center.
- Map name popup: `MAPSEC_ONE_ISLAND` (already in Emerald for FRLG link compatibility). No Fly, not on the Hoenn map.

### Ferry
- Slateport (`data/maps/SlateportCity_Harbor/scripts.inc`, lists in `src/data/script_menu.h`) and Lilycove
  (`src/script_menu.c`, `SSTIDAL_SELECTION_ONE_ISLAND`) offer ONE ISLAND. Both ferries already require
  `FLAG_SYS_GAME_CLEAR` (Elite Four beaten).
- Arrival: `MAP_ONE_ISLAND_HARBOR` (8, 5), FRLG's Seagallop arrival point.
- `VAR_ONE_ISLAND_FERRY_PORT` (was `VAR_UNUSED_0x404E`) remembers the departure port
  (`ONE_ISLAND_FERRY_FROM_SLATEPORT` / `_LILYCOVE`, in `include/constants/field_specials.h`).
- Lilycove's menu window is clamped to the top of the screen when it has 7 entries (possible with 3 of the 4
  event tickets). See "Ferry menu limits" below.

### Tools (`porting/`)
- `frlg_tilesets.py` — ports an FRLG tileset pair (see above).
- `frlg_maps.py` — renders maps/tilesets of either project to PNG (`render`, `sheet`), compares tiles
  (`compare`, `match`), and **`verify`**: checks a ported layout is identical to FRLG (blockdata and every
  pixel). `audit` (collision vs. vanilla Emerald usage) only applies to maps built from Emerald tilesets.

### Verification done
- `verify` reports 0 blockdata and 0 pixel differences for One Island and the harbor.
- Built tile graphics, all 925 metatiles, palette slots 0-12, animation frames and door graphics are
  byte-identical to FRLG; layer types all match.
- `make` succeeds. **Not yet tested in mGBA.**

## Known differences from FireRed
- Interiors (Pokémon Center, houses) are Emerald's.
- NPC sprites are Emerald's (a Fisherman stands in for FRLG's Balding Man).
- `MB_ROCK_STAIRS` → `MB_NORMAL`: FRLG slows the player on stairs; Emerald has no slow stairs (9 tiles on One Island).
- `MB_FAST_WATER` → `MB_OCEAN_WATER`: FRLG won't let you start surfing on it. Not used on One Island.
- Sign behaviors (`MB_SIGNPOST`, Pokémon Center/Mart signs, `MB_POWER_PLANT_MACHINE`) → `MB_NORMAL`; Emerald
  needs sign `bg_events` on the map for them to be readable (FRLG maps already have them for signposts).
- You travel on the S.S. Tidal instead of the Seagallop (by design).

## Next steps

1. **Test in mGBA**: Slateport → One Island → Net Center (heal) → both houses → harbor → back; same from
   Lilycove. Check the three door animations and the water/shore animations.
2. **Kindle Road (24x140) and Treasure Beach (24x40)** — same tilesets, so the maps are copies + `verify`.
   - Connections: One Island `right` → Kindle Road (offset -120), `down` → Treasure Beach (offset 0), and the
     reverse connections on those maps. This also fills the empty map edges around One Island.
   - Kindle Road: 13 trainers, 3 item balls, 13 Rock Smash rocks; Treasure Beach: 1 trainer, 1 NPC, 8 hidden items.
   - Wild encounters (land, water, fishing, rock smash) from pokefirered's `wild_encounters.json` (FireRed set).
   - **Trainer flag space**: only 9 free trainer slots (`TRAINERS_COUNT` 855 / `MAX_TRAINERS_COUNT` 864 in
     `include/constants/opponents.h`); these maps need 14. Raise the limit (costs save space) or move flags.
   - Trainer classes/sprites missing in Emerald (Crush Girl, Crush Kin double battle): use the closest Emerald
     ones (e.g. Battle Girl); keep FRLG parties, levels and text.
   - Close the exits to the Ember Spa and Mt. Ember until those are ported.
3. **Interiors**: port FRLG's Building primary and Pokémon Center / generic building secondaries the same way,
   then switch the One Island interiors to FRLG layouts.
4. **Mt. Ember / Ember Spa**: port FRLG's Mt. Ember secondary tileset; Mt. Ember Exterior is 57x54 plus cave floors.
5. Optional gameplay fidelity: slow rock stairs, fast water, FRLG NPC overworld sprites.
6. Before adding more destinations: make the Lilycove ferry menu always use the scrolling list (see below).

## Ferry menu limits

- Lilycove builds its destination list in `CreateLilycoveSSTidalMultichoice` (`src/script_menu.c`). With fewer
  than all `SSTIDAL_SELECTION_COUNT` (now 8) destinations it draws a plain window sized to the list, which only
  fits about 6-7 rows above the message box. Only when every destination is available does it switch to the
  scrolling list (`SCROLL_MULTI_SS_TIDAL_DESTINATION`), which shows 6 at a time
  (`MAX_SCROLL_MULTI_ON_SCREEN`) and holds at most 16 (`MAX_SCROLL_MULTI_LENGTH`), and is a fixed list.
- So adding more Sevii islands needs the scrolling list to be built from the destinations actually available
  (and used whenever there are more than 6), instead of the fixed full list.
- Slateport uses fixed `multichoice` lists (`MULTI_SSTIDAL_SLATEPORT_*`), which also stop fitting past about
  6-7 entries.
