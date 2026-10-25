#!/usr/bin/env python3
"""Helpers for porting FireRed/LeafGreen maps into pokeemerald using Emerald tilesets.

FRLG blockdata can't be copied as is: metatile IDs index each game's own tilesets,
which differ (and FRLG's primary tileset is larger). This script renders maps and
tilesets of either project to PNG so layouts can be compared, and converts FRLG
blockdata to Emerald metatiles through a hand-made translation table.

Usage:
  frlg_maps.py render  <project_dir> <LAYOUT_ID> <out.png>
  frlg_maps.py sheet   <project_dir> <primary_tileset> <secondary_tileset> <out.png>
  frlg_maps.py metatiles <project_dir> <LAYOUT_ID>      # list metatile IDs a layout uses
  frlg_maps.py render-bin <project_dir> <primary> <secondary> <width> <map.bin> <out.png>
  frlg_maps.py match   <pokefirered_dir> <LAYOUT_ID> <em_primary> <em_secondary>   # closest Emerald metatiles
  frlg_maps.py compare <pokefirered_dir> <frlg_secondary> <em_secondary> <out.png> <id|frlg:em>...
  frlg_maps.py audit   <LAYOUT_ID>                                  # collision vs vanilla Emerald usage
  frlg_maps.py verify  <pokefirered_dir> <FRLG_LAYOUT_ID> <EM_LAYOUT_ID>   # 1:1 check of a ported layout
"""

import json
import os
import re
import struct
import subprocess
import sys
import tempfile
import zlib

EMERALD_DIR = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..'))
GBAGFX = os.path.join(EMERALD_DIR, 'tools', 'gbagfx', 'gbagfx')
CACHE_DIR = os.path.join(tempfile.gettempdir(), 'frlg_maps_cache')


class Project:
    def __init__(self, root):
        self.root = os.path.abspath(root)
        fieldmap = open(os.path.join(self.root, 'include', 'fieldmap.h')).read()

        def define(name):
            return int(re.search(r'#define %s (\d+)' % name, fieldmap).group(1))

        self.num_tiles_primary = define('NUM_TILES_IN_PRIMARY')
        self.num_metatiles_primary = define('NUM_METATILES_IN_PRIMARY')
        self.num_pals_primary = define('NUM_PALS_IN_PRIMARY')
        self.is_frlg = self.num_metatiles_primary == 640
        metatiles_h = open(os.path.join(self.root, 'src', 'data', 'tilesets', 'metatiles.h')).read()
        self.tileset_dirs = {}
        for name, path in re.findall(r'gMetatiles_(\w+)\[\] = INCBIN_U16\("([^"]+)/metatiles.bin"\)', metatiles_h):
            self.tileset_dirs['gTileset_' + name] = os.path.join(self.root, path)
        layouts = json.load(open(os.path.join(self.root, 'data', 'layouts', 'layouts.json')))['layouts']
        self.layouts = {l['id']: l for l in layouts if 'id' in l}
        self._tilesets = {}

    def tileset(self, name):
        if name not in self._tilesets:
            self._tilesets[name] = Tileset(self, name)
        return self._tilesets[name]

    def read_blockdata(self, layout_id):
        layout = self.layouts[layout_id]
        data = open(os.path.join(self.root, layout['blockdata_filepath']), 'rb').read()
        return list(struct.unpack('<%dH' % (len(data) // 2), data)), layout['width'], layout['height']


class Tileset:
    def __init__(self, project, name):
        self.name = name
        d = project.tileset_dirs[name]
        self.dir = d
        self.tiles = self._load_tiles(os.path.join(d, 'tiles.png'))
        self.palettes = []
        for i in range(16):
            path = os.path.join(d, 'palettes', '%02d.pal' % i)
            self.palettes.append(self._load_pal(path) if os.path.exists(path) else [(0, 0, 0)] * 16)
        raw = open(os.path.join(d, 'metatiles.bin'), 'rb').read()
        self.metatiles = struct.unpack('<%dH' % (len(raw) // 2), raw)
        attr_size = 4 if project.is_frlg else 2
        raw = open(os.path.join(d, 'metatile_attributes.bin'), 'rb').read()
        self.attributes = struct.unpack('<%d%s' % (len(raw) // attr_size, 'I' if attr_size == 4 else 'H'), raw)

    @staticmethod
    def _load_tiles(png_path):
        os.makedirs(CACHE_DIR, exist_ok=True)
        key = re.sub(r'[^A-Za-z0-9]+', '_', os.path.abspath(png_path))
        out = os.path.join(CACHE_DIR, key + '.4bpp')
        if not os.path.exists(out) or os.path.getmtime(out) < os.path.getmtime(png_path):
            subprocess.run([GBAGFX, png_path, out], check=True)
        raw = open(out, 'rb').read()
        tiles = []
        for t in range(len(raw) // 32):
            px = []
            for b in raw[t * 32:(t + 1) * 32]:
                px.append(b & 0xF)
                px.append(b >> 4)
            tiles.append(px)
        return tiles

    @staticmethod
    def _load_pal(path):
        lines = open(path).read().split('\n')[3:19]
        cols = []
        for line in lines:
            parts = line.split()
            cols.append(tuple(int(p) for p in parts[:3]) if len(parts) >= 3 else (0, 0, 0))
        return cols


class TilesetPair:
    """A primary + secondary tileset, addressed with global metatile/tile/palette IDs like the game does."""

    def __init__(self, project, primary, secondary):
        self.p = project
        self.primary = project.tileset(primary)
        self.secondary = project.tileset(secondary)

    def metatile_entries(self, metatile_id):
        if metatile_id < self.p.num_metatiles_primary:
            ts, i = self.primary, metatile_id
        else:
            ts, i = self.secondary, metatile_id - self.p.num_metatiles_primary
        if (i + 1) * 8 > len(ts.metatiles):
            return None
        return ts.metatiles[i * 8:(i + 1) * 8]

    def num_metatiles(self):
        return self.p.num_metatiles_primary + len(self.secondary.metatiles) // 8

    def tile_pixels(self, tile_id):
        if tile_id < self.p.num_tiles_primary:
            tiles = self.primary.tiles
        else:
            tiles, tile_id = self.secondary.tiles, tile_id - self.p.num_tiles_primary
        return tiles[tile_id] if tile_id < len(tiles) else [0] * 64

    def palette(self, pal):
        # The game loads palettes 0..N-1 from the primary tileset and the rest from the secondary
        return (self.primary if pal < self.p.num_pals_primary else self.secondary).palettes[pal]

    def draw_metatile(self, img, img_w, ox, oy, metatile_id):
        entries = self.metatile_entries(metatile_id)
        backdrop = self.primary.palettes[0][0]
        for y in range(16):
            row = (oy + y) * img_w
            for x in range(16):
                img[row + ox + x] = backdrop
        if entries is None:
            return
        for layer in range(2):
            for q in range(4):
                e = entries[layer * 4 + q]
                pixels = self.tile_pixels(e & 0x3FF)
                hflip, vflip, pal = e & 0x400, e & 0x800, self.palette(e >> 12)
                qx, qy = (q & 1) * 8, (q >> 1) * 8
                for y in range(8):
                    sy = 7 - y if vflip else y
                    for x in range(8):
                        c = pixels[sy * 8 + (7 - x if hflip else x)]
                        if c:
                            img[(oy + qy + y) * img_w + ox + qx + x] = pal[c]


def write_png(path, width, height, pixels):
    raw = bytearray()
    for y in range(height):
        raw.append(0)
        for r, g, b in pixels[y * width:(y + 1) * width]:
            raw += bytes((r, g, b))

    def chunk(tag, data):
        c = tag + data
        return struct.pack('>I', len(data)) + c + struct.pack('>I', zlib.crc32(c) & 0xFFFFFFFF)

    with open(path, 'wb') as f:
        f.write(b'\x89PNG\r\n\x1a\n')
        f.write(chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 2, 0, 0, 0)))
        f.write(chunk(b'IDAT', zlib.compress(bytes(raw), 9)))
        f.write(chunk(b'IEND', b''))


def render_blocks(pair, blocks, width, height, out, grid=False, scale=1):
    img_w, img_h = width * 16, height * 16
    img = [(0, 0, 0)] * (img_w * img_h)
    for i, block in enumerate(blocks):
        pair.draw_metatile(img, img_w, (i % width) * 16, (i // width) * 16, block & 0x3FF)
    if grid:
        for y in range(img_h):
            for x in range(img_w):
                if x % 16 == 0 or y % 16 == 0:
                    r, g, b = img[y * img_w + x]
                    img[y * img_w + x] = ((r + 255) // 2, g // 2, b // 2) if (x % 128 == 0 or y % 128 == 0) else (r // 2 + 64, g // 2 + 64, b // 2 + 64)
    if scale > 1:
        big = []
        for y in range(img_h):
            row = []
            for px in img[y * img_w:(y + 1) * img_w]:
                row.extend([px] * scale)
            big.extend(row * scale)
        img, img_w, img_h = big, img_w * scale, img_h * scale
    write_png(out, img_w, img_h, img)


def cmd_render(root, layout_id, out, x0=None, y0=None, cw=None, ch=None):
    """Render a layout; optionally only the crop x0,y0,cw,ch, zoomed 3x."""
    p = Project(root)
    layout = p.layouts[layout_id]
    blocks, w, h = p.read_blockdata(layout_id)
    pair = TilesetPair(p, layout['primary_tileset'], layout['secondary_tileset'])
    if x0 is None:
        render_blocks(pair, blocks, w, h, out, grid=True)
        return
    x0, y0, cw, ch = int(x0), int(y0), int(cw), int(ch)
    crop = [blocks[(y0 + y) * w + x0 + x] for y in range(ch) for x in range(cw)]
    render_blocks(pair, crop, cw, ch, out, grid=True, scale=3)


def cmd_render_bin(root, primary, secondary, width, bin_path, out):
    p = Project(root)
    data = open(bin_path, 'rb').read()
    blocks = list(struct.unpack('<%dH' % (len(data) // 2), data))
    width = int(width)
    render_blocks(TilesetPair(p, primary, secondary), blocks, width, len(blocks) // width, out, grid=True)


def cmd_sheet(root, primary, secondary, out, start='0', count=None):
    """Metatiles of a tileset pair, 8 per row like porymap; ID = start + row * 8 + column.
    Red grid lines every 8 rows (64 IDs) make IDs easy to count."""
    p = Project(root)
    pair = TilesetPair(p, primary, secondary)
    start = int(start, 0)
    end = pair.num_metatiles() if count is None else min(pair.num_metatiles(), start + int(count, 0))
    blocks = list(range(start, end))
    render_blocks(pair, blocks, 8, (len(blocks) + 7) // 8, out, grid=True, scale=3)


def metatile_rgb(pair, metatile_id):
    img = [(0, 0, 0)] * 256
    pair.draw_metatile(img, 16, 0, 0, metatile_id)
    return img


def coarse(rgb):
    # 4x4 grid of averaged 4x4-pixel cells, for a fast first-pass comparison
    out = []
    for cy in range(4):
        for cx in range(4):
            r = g = b = 0
            for y in range(4):
                for x in range(4):
                    pr, pg, pb = rgb[(cy * 4 + y) * 16 + cx * 4 + x]
                    r += pr; g += pg; b += pb
            out += [r // 16, g // 16, b // 16]
    return out


def distance(a, b):
    return sum((x - y) * (x - y) for x, y in zip(a, b))


def full_distance(a, b):
    return sum((p[0] - q[0]) ** 2 + (p[1] - q[1]) ** 2 + (p[2] - q[2]) ** 2 for p, q in zip(a, b))


def best_matches(src_rgb, dst_cache, n=3):
    c = coarse(src_rgb)
    candidates = sorted(dst_cache, key=lambda e: distance(c, e[1]))[:12]
    scored = sorted(((full_distance(src_rgb, rgb), mid) for mid, _, rgb in candidates))
    return scored[:n]


def cmd_match(frlg_root, layout_id, primary, secondary):
    """For each metatile an FRLG layout uses, print the closest-looking Emerald metatiles.
    The output is a starting point for the hand-checked translation table."""
    fr = Project(frlg_root)
    layout = fr.layouts[layout_id]
    fr_pair = TilesetPair(fr, layout['primary_tileset'], layout['secondary_tileset'])
    em = Project(EMERALD_DIR)
    em_pair = TilesetPair(em, primary, secondary)
    dst = []
    for mid in range(em_pair.num_metatiles()):
        if em_pair.metatile_entries(mid) is None:
            continue
        rgb = metatile_rgb(em_pair, mid)
        dst.append((mid, coarse(rgb), rgb))
    blocks, w, h = fr.read_blockdata(layout_id)
    for mid in sorted(set(b & 0x3FF for b in blocks)):
        m = best_matches(metatile_rgb(fr_pair, mid), dst)
        # Per-pixel RMS error per channel, so the score reads the same for any metatile
        print('0x%03X -> %s' % (mid, '  '.join('0x%03X(%d)' % (e, int((d / 768) ** 0.5)) for d, e in m)))


def cmd_compare(frlg_root, frlg_secondary, em_secondary, out, *ids):
    """Render metatiles side by side, zoomed 4x: FRLG on the top row, Emerald on the bottom row.
    Each ID is either N (same ID in both) or FRLG:EMERALD (e.g. 0xB4:0x7C)."""
    fr = Project(frlg_root)
    em = Project(EMERALD_DIR)
    fp = TilesetPair(fr, 'gTileset_General', frlg_secondary)
    ep = TilesetPair(em, 'gTileset_General', em_secondary)
    pairs = []
    for i in ids:
        a, _, b = i.partition(':')
        pairs.append((int(a, 0), int(b or a, 0)))
    w = len(pairs) * 16 + (len(pairs) - 1) * 2
    img = [(255, 255, 255)] * (w * 32)
    for n, (a, b) in enumerate(pairs):
        fp.draw_metatile(img, w, n * 18, 0, a)
        ep.draw_metatile(img, w, n * 18, 16, b)
    scale = 4
    big = []
    for y in range(32):
        row = []
        for px in img[y * w:(y + 1) * w]:
            row.extend([px] * scale)
        big.extend(row * scale)
    write_png(out, w * scale, 32 * scale, big)


def layout_pixels(project, layout_id):
    layout = project.layouts[layout_id]
    blocks, w, h = project.read_blockdata(layout_id)
    pair = TilesetPair(project, layout['primary_tileset'], layout['secondary_tileset'])
    img = [(0, 0, 0)] * (w * 16 * h * 16)
    for i, b in enumerate(blocks):
        pair.draw_metatile(img, w * 16, (i % w) * 16, (i // w) * 16, b & 0x3FF)
    return img, blocks, w, h


def cmd_verify(frlg_root, frlg_layout, em_layout):
    """Check a layout ported with FRLG tilesets is 1:1 with FRLG: same blockdata (metatiles, collision,
    elevation) and the same picture, pixel for pixel, when each is drawn with its own game's tilesets."""
    fr = Project(frlg_root)
    em = Project(EMERALD_DIR)
    fr_img, fr_blocks, fw, fh = layout_pixels(fr, frlg_layout)
    em_img, em_blocks, ew, eh = layout_pixels(em, em_layout)
    ok = True
    if (fw, fh) != (ew, eh):
        print('size differs: FRLG %dx%d, Emerald %dx%d' % (fw, fh, ew, eh))
        return
    block_diffs = sum(1 for a, b in zip(fr_blocks, em_blocks) if a != b)
    pixel_diffs = sum(1 for a, b in zip(fr_img, em_img) if a != b)
    print('blockdata cells that differ: %d of %d' % (block_diffs, len(fr_blocks)))
    print('pixels that differ: %d of %d' % (pixel_diffs, len(fr_img)))
    print('1:1' if block_diffs == 0 and pixel_diffs == 0 else 'NOT 1:1')


def cmd_audit(layout_id):
    """Flag cells of an Emerald layout whose collision disagrees with how vanilla Emerald layouts
    usually use that metatile (e.g. invisible walls, or walkable cliffs)."""
    import collections
    em = Project(EMERALD_DIR)
    target = em.layouts[layout_id]
    votes = collections.defaultdict(collections.Counter)
    for lid, l in em.layouts.items():
        if lid == layout_id or l['primary_tileset'] != target['primary_tileset']:
            continue
        blocks, _, _ = em.read_blockdata(lid)
        for v in blocks:
            m = v & 0x3FF
            # Secondary metatile IDs only mean the same thing within the same secondary tileset
            key = m if m < em.num_metatiles_primary else (l['secondary_tileset'], m)
            votes[key][(v >> 10) & 3 != 0] += 1
    blocks, w, h = em.read_blockdata(layout_id)
    issues = 0
    for y in range(h):
        for x in range(w):
            v = blocks[y * w + x]
            m = v & 0x3FF
            key = m if m < em.num_metatiles_primary else (target['secondary_tileset'], m)
            blocked = (v >> 10) & 3 != 0
            c = votes.get(key)
            if not c:
                print('(%d,%d) 0x%03X: not used in any vanilla layout, check by hand' % (x, y, m))
                continue
            share = c[blocked] / sum(c.values())
            if share < 0.5:
                issues += 1
                print('(%d,%d) 0x%03X is %s, but %s in %d%% of vanilla uses' % (
                    x, y, m, 'blocked' if blocked else 'walkable', 'walkable' if blocked else 'blocked', round(100 * (1 - share))))
    print('%d cells disagree with vanilla' % issues)


def cmd_metatiles(root, layout_id):
    p = Project(root)
    blocks, w, h = p.read_blockdata(layout_id)
    layout = p.layouts[layout_id]
    pair = TilesetPair(p, layout['primary_tileset'], layout['secondary_tileset'])
    counts = {}
    for b in blocks:
        counts[b & 0x3FF] = counts.get(b & 0x3FF, 0) + 1
    for mid in sorted(counts):
        if mid < p.num_metatiles_primary:
            attr = pair.primary.attributes[mid]
        else:
            attr = pair.secondary.attributes[mid - p.num_metatiles_primary]
        behavior = attr & (0x1FF if p.is_frlg else 0xFF)
        print('0x%03X  x%-3d behavior=0x%02X' % (mid, counts[mid], behavior))
    print('grid (metatile ids, X = impassable):')
    for y in range(h):
        print(' '.join('%03x%s' % (blocks[y * w + x] & 0x3FF, 'X' if (blocks[y * w + x] >> 10) & 3 else '.') for x in range(w)))


if __name__ == '__main__':
    cmds = {'render': cmd_render, 'sheet': cmd_sheet, 'metatiles': cmd_metatiles, 'render-bin': cmd_render_bin,
            'match': cmd_match, 'compare': cmd_compare, 'audit': cmd_audit, 'verify': cmd_verify}
    if len(sys.argv) < 2 or sys.argv[1] not in cmds:
        print(__doc__)
        sys.exit(1)
    cmds[sys.argv[1]](*sys.argv[2:])
