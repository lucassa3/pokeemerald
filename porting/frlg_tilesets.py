#!/usr/bin/env python3
"""Ports a FireRed/LeafGreen tileset pair (primary + secondary) into pokeemerald, unchanged.

Both engines have 1024 tiles, 1024 metatiles and 13 BG palettes per map, loaded into one contiguous
space; they only split it differently between the primary and secondary tileset:

                 tiles       metatiles    palettes
    FRLG       640 + 384    640 + 384     7 + 6
    Emerald    512 + 512    512 + 512     6 + 7

Re-splitting FRLG's data at Emerald's boundaries keeps every global tile, metatile and palette number
the same, so FRLG metatiles and map blockdata work in Emerald byte for byte. The new Emerald primary is
the first 512 tiles/metatiles and 6 palettes of the FRLG primary; the new Emerald secondary is the rest
of the FRLG primary followed by the FRLG secondary. Each FRLG secondary therefore gets its own Emerald
secondary that starts with the tail of FRLG's primary.

Metatile attributes are converted from FRLG's 32-bit format to Emerald's 16-bit one, matching
behaviors by name. Behaviors without an exact Emerald counterpart are listed in APPROXIMATE_BEHAVIORS.

Usage:
  frlg_tilesets.py <pokefirered_dir> <frlg_primary_dir> <frlg_secondary_dir> <em_primary_dir> <em_secondary_dir>
e.g.
  frlg_tilesets.py ~/projects/pokefirered general sevii_islands_123 frlg_general frlg_sevii_islands_123
"""

import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile

EMERALD_DIR = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
GBAGFX = os.path.join(EMERALD_DIR, 'tools', 'gbagfx', 'gbagfx')

FRLG_TILES_IN_PRIMARY, EM_TILES_IN_PRIMARY = 640, 512
FRLG_METATILES_IN_PRIMARY, EM_METATILES_IN_PRIMARY = 640, 512
FRLG_PALS_IN_PRIMARY, EM_PALS_IN_PRIMARY = 7, 6
TILE_BYTES = 32
METATILE_BYTES = 16

# FRLG behaviors whose names don't exist in Emerald -> the Emerald behavior that does the same job
APPROXIMATE_BEHAVIORS = {
    'MB_WARP_DOOR': 'MB_ANIMATED_DOOR',
    'MB_CAVE_DOOR': 'MB_NON_ANIMATED_DOOR',
    'MB_TRASH_BIN': 'MB_TRASH_CAN',
    # FRLG's diagonal stairs warp as you step off them sideways; Emerald's arrow warps trigger the same way
    'MB_UP_RIGHT_STAIR_WARP': 'MB_EAST_ARROW_WARP',
    'MB_DOWN_RIGHT_STAIR_WARP': 'MB_EAST_ARROW_WARP',
    'MB_UP_LEFT_STAIR_WARP': 'MB_WEST_ARROW_WARP',
    'MB_DOWN_LEFT_STAIR_WARP': 'MB_WEST_ARROW_WARP',
    # FRLG reads these signs from the metatile; Emerald uses sign bg_events on the map instead
    'MB_SIGNPOST': 'MB_NORMAL',
    'MB_POKEMON_CENTER_SIGN': 'MB_NORMAL',
    'MB_POKEMART_SIGN': 'MB_NORMAL',
    'MB_INDIGO_PLATEAU_SIGN_1': 'MB_NORMAL',
    'MB_INDIGO_PLATEAU_SIGN_2': 'MB_NORMAL',
    'MB_POWER_PLANT_MACHINE': 'MB_NORMAL',
    # FRLG flavor text when facing furniture ("It's a nicely made dresser."); Emerald has no such behaviors
    'MB_WINDOW': 'MB_NORMAL',
    'MB_DRESSER': 'MB_NORMAL',
    'MB_KITCHEN': 'MB_NORMAL',
    'MB_CABINET': 'MB_NORMAL',
    'MB_COMPUTER': 'MB_NORMAL',
    'MB_CUP': 'MB_NORMAL',
    'MB_PORTHOLE': 'MB_NORMAL',
    'MB_FOOD': 'MB_NORMAL',
    'MB_PAINTING': 'MB_NORMAL',
    'MB_CABLE_CLUB_WIRELESS_MONITOR': 'MB_NORMAL',
    'MB_BATTLE_RECORDS': 'MB_CABLE_BOX_RESULTS_1',  # link battle records screen, as in Emerald's Pokémon Centers  # "What could this machine be?" flavor text; needs a sign event if used
    # Gameplay differences, not visual ones:
    'MB_ROCK_STAIRS': 'MB_NORMAL',        # FRLG slows the player on stairs; Emerald has no slow stairs
    'MB_FAST_WATER': 'MB_OCEAN_WATER',    # FRLG won't let you start surfing on it ("current is too fast")
}


def emerald_behaviors():
    src = open(os.path.join(EMERALD_DIR, 'include', 'constants', 'metatile_behaviors.h')).read()
    body = src[src.index('enum {') + len('enum {'): src.index('};')]
    values, n = {}, 0
    for line in body.split('\n'):
        line = re.sub(r'//.*', '', line).strip().rstrip(',')
        if not line:
            continue
        if '=' in line:
            name, value = [s.strip() for s in line.split('=')]
            n = int(value, 0)
        else:
            name = line
        values[name] = n
        n += 1
    return values


def frlg_behaviors(frlg_dir):
    src = open(os.path.join(frlg_dir, 'include', 'constants', 'metatile_behaviors.h')).read()
    return {int(v, 0): k for k, v in re.findall(r'#define (MB_\w+) (0x[0-9A-Fa-f]+)', src)}


def convert_attribute(attr, frlg_names, em_values, report):
    behavior = attr & 0x1FF
    layer_type = (attr >> 29) & 0x3
    name = frlg_names.get(behavior)
    if name is None:
        raise SystemExit('Unknown FRLG behavior 0x%X' % behavior)
    em_name = name if name in em_values else APPROXIMATE_BEHAVIORS.get(name)
    if em_name is None:
        raise SystemExit('No Emerald behavior for %s; add it to APPROXIMATE_BEHAVIORS' % name)
    if em_name != name:
        report[name] = em_name
    return em_values[em_name] | (layer_type << 12)


def read_4bpp(png):
    with tempfile.TemporaryDirectory() as tmp:
        out = os.path.join(tmp, 'tiles.4bpp')
        subprocess.run([GBAGFX, png, out], check=True)
        return open(out, 'rb').read()


def write_png(tiles, png, palette):
    with tempfile.TemporaryDirectory() as tmp:
        raw = os.path.join(tmp, 'tiles.4bpp')
        open(raw, 'wb').write(tiles)
        subprocess.run([GBAGFX, raw, png, '-width', '16', '-palette', palette], check=True)


def main(frlg_dir, frlg_primary, frlg_secondary, em_primary, em_secondary):
    fr_p = os.path.join(frlg_dir, 'data', 'tilesets', 'primary', frlg_primary)
    fr_s = os.path.join(frlg_dir, 'data', 'tilesets', 'secondary', frlg_secondary)
    em_p = os.path.join(EMERALD_DIR, 'data', 'tilesets', 'primary', em_primary)
    em_s = os.path.join(EMERALD_DIR, 'data', 'tilesets', 'secondary', em_secondary)
    for d in (em_p, em_s):
        os.makedirs(os.path.join(d, 'palettes'), exist_ok=True)

    # Tiles
    p_tiles = read_4bpp(os.path.join(fr_p, 'tiles.png'))
    s_tiles = read_4bpp(os.path.join(fr_s, 'tiles.png'))
    assert len(p_tiles) == FRLG_TILES_IN_PRIMARY * TILE_BYTES, 'FRLG primary should have exactly 640 tiles'
    new_p_tiles = p_tiles[:EM_TILES_IN_PRIMARY * TILE_BYTES]
    new_s_tiles = p_tiles[EM_TILES_IN_PRIMARY * TILE_BYTES:] + s_tiles
    assert len(new_s_tiles) <= (1024 - EM_TILES_IN_PRIMARY) * TILE_BYTES, 'Too many tiles for the Emerald secondary'
    write_png(new_p_tiles, os.path.join(em_p, 'tiles.png'), os.path.join(fr_p, 'palettes', '00.pal'))
    write_png(new_s_tiles, os.path.join(em_s, 'tiles.png'), os.path.join(fr_s, 'palettes', '07.pal'))

    # Palettes: global slots 0-5 come from the primary, 6-12 from the secondary in both engines'
    # Emerald layout, so FRLG's primary palette 6 moves into the secondary's slot 6
    for i in range(16):
        shutil.copy(os.path.join(fr_p, 'palettes', '%02d.pal' % i), os.path.join(em_p, 'palettes', '%02d.pal' % i))
        src = fr_p if EM_PALS_IN_PRIMARY <= i < FRLG_PALS_IN_PRIMARY else fr_s
        shutil.copy(os.path.join(src, 'palettes', '%02d.pal' % i), os.path.join(em_s, 'palettes', '%02d.pal' % i))

    # Metatiles
    p_meta = open(os.path.join(fr_p, 'metatiles.bin'), 'rb').read()
    s_meta = open(os.path.join(fr_s, 'metatiles.bin'), 'rb').read()
    assert len(p_meta) == FRLG_METATILES_IN_PRIMARY * METATILE_BYTES
    split = EM_METATILES_IN_PRIMARY * METATILE_BYTES
    open(os.path.join(em_p, 'metatiles.bin'), 'wb').write(p_meta[:split])
    new_s_meta = p_meta[split:] + s_meta
    assert len(new_s_meta) <= (1024 - EM_METATILES_IN_PRIMARY) * METATILE_BYTES
    open(os.path.join(em_s, 'metatiles.bin'), 'wb').write(new_s_meta)

    # Metatile attributes
    frlg_names, em_values = frlg_behaviors(frlg_dir), emerald_behaviors()
    report = {}

    def attrs(path):
        raw = open(path, 'rb').read()
        return [convert_attribute(a, frlg_names, em_values, report) for a in struct.unpack('<%dI' % (len(raw) // 4), raw)]

    p_attrs = attrs(os.path.join(fr_p, 'metatile_attributes.bin'))
    s_attrs = attrs(os.path.join(fr_s, 'metatile_attributes.bin'))
    new_p_attrs = p_attrs[:EM_METATILES_IN_PRIMARY]
    new_s_attrs = p_attrs[EM_METATILES_IN_PRIMARY:] + s_attrs
    open(os.path.join(em_p, 'metatile_attributes.bin'), 'wb').write(struct.pack('<%dH' % len(new_p_attrs), *new_p_attrs))
    open(os.path.join(em_s, 'metatile_attributes.bin'), 'wb').write(struct.pack('<%dH' % len(new_s_attrs), *new_s_attrs))

    # Tile animation frames (the animation code lives in src/tileset_anims.c)
    if os.path.isdir(os.path.join(fr_p, 'anim')):
        shutil.copytree(os.path.join(fr_p, 'anim'), os.path.join(em_p, 'anim'), dirs_exist_ok=True)

    print('%s: %d tiles, %d metatiles' % (em_primary, len(new_p_tiles) // TILE_BYTES, len(new_p_attrs)))
    print('%s: %d tiles, %d metatiles' % (em_secondary, len(new_s_tiles) // TILE_BYTES, len(new_s_attrs)))
    for fr_name, em_name in sorted(report.items()):
        print('  approximated %s -> %s' % (fr_name, em_name))


if __name__ == '__main__':
    if len(sys.argv) != 6:
        print(__doc__)
        sys.exit(1)
    main(*sys.argv[1:])
