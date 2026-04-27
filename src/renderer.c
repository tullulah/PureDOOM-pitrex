/*
 * renderer.c — wireframe vector renderer
 *
 * This translation unit owns the DOOM implementation (DOOM_IMPLEMENTATION).
 * Defining DOOM_CUSTOM_WALL_RANGE renames PureDOOM's R_StoreWallRange to
 * R_StoreWallRange_doom, freeing the name for our wireframe override below.
 *
 * Strategy
 * --------
 * PureDOOM's BSP traversal calls R_StoreWallRange(start, stop) for every
 * visible wall segment.  At that point the following globals are live:
 *
 *   curline          seg_t*  — the wall segment
 *   frontsector      —  sector in front of the player
 *   backsector       —  sector behind the wall (NULL for solid walls)
 *   rw_scale         —  perspective scale at screen column `start`
 *   ds_p->scale2     —  perspective scale at screen column `stop`
 *   worldtop         —  ceiling height relative to viewz  (before >>4 shift)
 *   worldbottom      —  floor height relative to viewz    (before >>4 shift)
 *   centery          —  screen centre Y (pixels)
 *   centeryfrac      —  centery in fixed-point
 *
 * We compute the four corners of the wall quad in screen space and emit
 * two lines: top edge and bottom edge.  Vertical edges are skipped to
 * reduce vector count (they're implied).
 *
 * Coordinate mapping
 * ------------------
 * DOOM screen: X ∈ [0, 319], Y ∈ [0, 199]  (top-left origin)
 * Vectrex:     X ∈ [-127, 127], Y ∈ [-127, 127]  (centre origin, Y-up)
 *
 * doom_to_vec_x(x) = (x - 160) * 127 / 160
 * doom_to_vec_y(y) = (100 - y)  * 127 / 100
 *
 * Vector budget
 * -------------
 * The Vectrex beam can draw ~1000–1500 short vectors per frame at good
 * brightness.  We cap the frame at MAX_VECTORS_PER_FRAME and skip
 * additional walls once the budget is exhausted.
 */

#include "renderer.h"

/* NULL and size_t for baremetal (PureDOOM pulls in some headers on host only) */
#ifndef HOST_SDL
#  include <stddef.h>
#endif

/* This TU owns the full DOOM implementation. */
#define DOOM_CUSTOM_WALL_RANGE    /* renames built-in R_StoreWallRange → _doom */
#define DOOM_CUSTOM_PAGE_DRAWER   /* we supply D_PageDrawer() below             */
#define DOOM_CUSTOM_ST_DRAWER     /* we supply ST_Drawer() below                */
#define DOOM_CUSTOM_M_DRAWER      /* we supply M_Drawer() below                 */
#ifdef HOST_SDL
#  define DOOM_IMPLEMENT_FILE_IO  /* use real fopen/fread/fseek (host only)    */
#  define DOOM_IMPLEMENT_MALLOC   /* use real malloc/free (host only)           */
#endif
#define DOOM_IMPLEMENTATION
#include "../../puredoom/PureDOOM.h"

#include "doom_logo.h"           /* auto-generated vector segments for title screen */
#include "hud_assets.h"          /* auto-generated key icon segments               */
#include "weapons_assets.h"      /* auto-generated weapon vector segments           */

/* piTrex drawing API — uncomment when SDK is available */
#ifndef HOST_SDL
/* #include <pitrex/pitrex.h> */
#endif

/* Debug logging: only on host SDL build */
#ifdef HOST_SDL
#  include <stdio.h>
#  define RENDERER_LOG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#  define RENDERER_LOG(fmt, ...) ((void)0)
#endif

/* ================================================================
 * Config
 * ================================================================ */

#define DOOM_W              320
#define DOOM_H              200
/* Keep line count low enough for the Vectrex beam to complete in one cycle.
 * More than ~400 short lines causes visible flicker. */
#define MAX_VECTORS_PER_FRAME 350

/* ================================================================
 * State
 * ================================================================ */

static int s_vec_count = 0;        /* vectors emitted this frame */
static renderer_draw_fn s_draw_fn = 0; /* injected by renderer_init() */

/* Column occlusion buffer: tracks which screen X columns are covered
 * by a solid (one-sided) wall closer to the player.  BSP is front-to-back
 * so the first solid wall in any column is the nearest; everything behind
 * it is invisible and can be culled.
 * Bit-packed: s_col_solid[x/8] bit (x%8) = 1 → column x is occluded. */
#define COL_SOLID_BYTES ((DOOM_W + 7) / 8)
static uint8_t s_col_solid[COL_SOLID_BYTES];

static inline void col_solid_set(int x)
{
    if ((unsigned)x < (unsigned)DOOM_W)
        s_col_solid[x >> 3] |= (uint8_t)(1u << (x & 7));
}
static inline int col_solid_get(int x)
{
    if ((unsigned)x >= (unsigned)DOOM_W) return 0;
    return (s_col_solid[x >> 3] >> (x & 7)) & 1;
}
/* Returns 1 if every column in [x0..x1] is already solid-occluded. */
static int col_range_all_solid(int x0, int x1)
{
    for (int x = x0; x <= x1; x++)
        if (!col_solid_get(x)) return 0;
    return 1;
}

/*
 * draw_wall_line_clipped — draw a trapezoid edge (top or bottom of a wall quad)
 * clipped to the non-solid column ranges in [scr_x0 .. scr_x1].
 * vy_start / vy_stop are the Vectrex Y coords at the left/right ends.
 * Interpolates linearly between them, skipping any solid-column spans.
 */
/* Forward declarations — defined later in this file */
static inline int doom_to_vec_x(int x);
static void draw_vec(int x0, int y0, int x1, int y1);

static void draw_wall_line_clipped(int scr_x0, int scr_x1,
                                   int vy_start, int vy_stop)
{
    int range = scr_x1 - scr_x0;
    int xa = scr_x0;
    while (xa <= scr_x1) {
        /* skip solid columns */
        while (xa <= scr_x1 && col_solid_get(xa)) xa++;
        if (xa > scr_x1) break;
        /* find end of non-solid run */
        int xb = xa;
        while (xb + 1 <= scr_x1 && !col_solid_get(xb + 1)) xb++;
        /* interpolate vec Y at xa and xb */
        int vya = vy_start + (range > 0 ? (vy_stop - vy_start) * (xa - scr_x0) / range : 0);
        int vyb = vy_start + (range > 0 ? (vy_stop - vy_start) * (xb - scr_x0) / range : 0);
        draw_vec(doom_to_vec_x(xa), vya, doom_to_vec_x(xb), vyb);
        xa = xb + 1;
    }
}

/* Scene buffer: caches wall/BSP vectors so frames where DOOM doesn't
 * tick (60Hz SDL vs 35Hz DOOM) can replay the last rendered scene.
 * This eliminates the flicker caused by the rate mismatch. */
typedef struct { int16_t x0, y0, x1, y1; } scene_line_t;
#define SCENE_BUF_SIZE 700
static scene_line_t s_scene_buf[SCENE_BUF_SIZE];
static int s_scene_count = 0;   /* lines stored in buffer */
static int s_about_active = 0;  /* suppress walls during ABOUT screen */
static int s_scene_active = 0;  /* 1 while doom_update() is running */
static int s_scene_new = 0;     /* lines written this scene phase */
static int s_glow_tick = 0;     /* frame counter for glow pulse */

/* ================================================================
 * 3D rotation — 128-entry sin LUT (integer, no FPU)
 *
 * Angles: 0–127 = 0–360°  (matches VPY compiler convention)
 * Values: sin(angle * 2π/128) * 63, rounded, clamped to [-63,63]
 * For cos: use (angle+32)&0x7F  (quarter-period offset)
 *
 * Generated at compile time — safe for ARM baremetal (no math.h at runtime).
 * ================================================================ */
static const int8_t s_sin128[128] = {
      0,  3,  6,  9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 41, 44,
     46, 48, 51, 53, 55, 57, 58, 60, 61, 62, 63, 63, 63, 63, 63, 63,
     63, 63, 63, 63, 63, 62, 61, 60, 58, 57, 55, 53, 51, 48, 46, 44,
     41, 39, 36, 33, 30, 27, 24, 21, 18, 15, 12,  9,  6,  3,  0, -3,
     -6, -9,-12,-15,-18,-21,-24,-27,-30,-33,-36,-39,-41,-44,-46,-48,
    -51,-53,-55,-57,-58,-60,-61,-62,-63,-63,-63,-63,-63,-63,-63,-63,
    -63,-63,-63,-63,-63,-62,-61,-60,-58,-57,-55,-53,-51,-48,-46,-44,
    -41,-39,-36,-33,-30,-27,-24,-21,-18,-15,-12, -9, -6, -3
};

/* smul(val, angle) = val * sin(angle) / 63  — integer, no overflow for val ≤63 */
static inline int smul(int val, int angle)
{
    return (val * (int)s_sin128[angle & 0x7F]) / 63;
}
static inline int cmul(int val, int angle)
{
    return smul(val, (angle + 32) & 0x7F);
}

/* Rotate point (rx,ry,rz) by Euler XYZ angles (ax,ay,az) in 0-127 range.
 * Writes result into *out_x, *out_y (screen 2D projection = XY plane after rotation).
 * Matches VPY DV3D_ROTATE register-level algorithm exactly. */
static void rot3d(int rx, int ry, int rz,
                  int ax, int ay, int az,
                  int *out_x, int *out_y)
{
    /* X-axis rotation */
    int y1 = cmul(ry, ax) - smul(rz, ax);
    int z1 = smul(ry, ax) + cmul(rz, ax);
    /* Y-axis rotation */
    int x2 =  cmul(rx, ay) + smul(z1, ay);
    int z2 = -smul(rx, ay) + cmul(z1, ay);
    /* Z-axis rotation */
    int xs = cmul(x2, az) - smul(y1, az);
    int ys = smul(x2, az) + cmul(y1, az);
    (void)z2; /* depth — not used for 2D projection */
    *out_x = xs;
    *out_y = ys;
}

/* ================================================================
 * Coordinate conversion
 * ================================================================ */

static inline int doom_to_vec_x(int x)
{
    /* [0,319] → [-127,127] */
    return (x - 160) * 127 / 160;
}

static inline int doom_to_vec_y(int y)
{
    /* [0,199] → [127,-127]  (DOOM Y-down → Vectrex Y-up) */
    return (100 - y) * 127 / 100;
}

/* ================================================================
 * Low-level vector output
 * Stub: replace with real piTrex draw call.
 * ================================================================ */

static void draw_vec(int x0, int y0, int x1, int y1)
{
    /* During scene phase (doom_update): buffer only, never call draw_fn directly.
     * The actual v_directDraw32 calls happen in renderer_scene_end() and
     * renderer_draw_* so all drawing is within one v_WaitRecal cycle. */
    if (s_scene_active) {
        if (s_scene_new < SCENE_BUF_SIZE) {
            s_scene_buf[s_scene_new].x0 = (int16_t)x0;
            s_scene_buf[s_scene_new].y0 = (int16_t)y0;
            s_scene_buf[s_scene_new].x1 = (int16_t)x1;
            s_scene_buf[s_scene_new].y1 = (int16_t)y1;
            s_scene_new++;
        }
        return;
    }
    /* Outside scene phase (sprites, weapons, HUD): draw immediately */
    if (s_vec_count >= MAX_VECTORS_PER_FRAME || !s_draw_fn)
        return;
    s_draw_fn(x0, y0, x1, y1);
    s_vec_count++;
}

/* ================================================================
 * Wall segment → screen quad
 *
 * PureDOOM computes screen Y from fixed-point:
 *   topfrac    = (centeryfrac >> 4) - FixedMul(worldtop,    rw_scale)
 *   bottomfrac = (centeryfrac >> 4) - FixedMul(worldbottom, rw_scale)
 *   screen_y   = frac >> HEIGHTBITS   (HEIGHTBITS = 12)
 *
 * We reproduce that for the two endpoints (start, stop) of the seg.
 * ================================================================ */

/* HEIGHTBITS, HEIGHTUNIT, FRACBITS, FRACUNIT already defined by PureDOOM.h */

/* Compute projected screen Y for a world height h at a given scale.
 * h is already shifted right by 4 (as PureDOOM does before the loop). */
static inline int project_y(fixed_t h_shifted, fixed_t scale)
{
    /* Matches PureDOOM: frac = (centeryfrac>>4) - FixedMul(h, scale) */
    fixed_t frac = (centeryfrac >> 4) - FixedMul(h_shifted, scale);
    return frac >> HEIGHTBITS;
}

/*
 * wall_draw_segment — called from our patched R_StoreWallRange.
 *
 * By the time we arrive here (after the `worldtop >>= 4` lines in
 * R_StoreWallRange) the following are valid:
 *   worldtop / worldbottom  — already shifted
 *   rw_scale                — scale at x=start
 *   ds_p->scale2            — scale at x=stop
 */
static void wall_draw_segment(int start, int stop, int is_solid)
{
    /* Fast-path: if every column is already occluded, nothing to draw. */
    if (col_range_all_solid(start, stop)) return;

    fixed_t scale_start = rw_scale;
    fixed_t scale_stop  = ds_p->scale2;

    int top_start    = project_y(worldtop,    scale_start);
    int bot_start    = project_y(worldbottom, scale_start);
    int top_stop     = project_y(worldtop,    scale_stop);
    int bot_stop     = project_y(worldbottom, scale_stop);

    /* Clamp to screen */
#define CLAMP(v, lo, hi) ((v)<(lo)?(lo):((v)>(hi)?(hi):(v)))
    top_start = CLAMP(top_start, 0, DOOM_H - 1);
    bot_start = CLAMP(bot_start, 0, DOOM_H - 1);
    top_stop  = CLAMP(top_stop,  0, DOOM_H - 1);
    bot_stop  = CLAMP(bot_stop,  0, DOOM_H - 1);

    /* Convert to Vectrex coordinates */
    int vx0 = doom_to_vec_x(start);
    int vx1 = doom_to_vec_x(stop);
    int vyt0 = doom_to_vec_y(top_start);
    int vyb0 = doom_to_vec_y(bot_start);
    int vyt1 = doom_to_vec_y(top_stop);
    int vyb1 = doom_to_vec_y(bot_stop);

    if (is_solid) {
        /* Solid wall: draw fully (it's the nearest surface in these columns),
         * then mark those columns occluded so anything behind is clipped. */
        draw_vec(vx0, vyt0, vx1, vyt1);   /* top edge */
        draw_vec(vx0, vyb0, vx1, vyb1);   /* bottom edge */
        draw_vec(vx0, vyt0, vx0, vyb0);   /* left vertical */
        draw_vec(vx1, vyt1, vx1, vyb1);   /* right vertical */
        for (int x = start; x <= stop; x++) col_solid_set(x);
    } else {
        /* Transparent wall (portal/step): clip to non-solid column spans. */
        draw_wall_line_clipped(start, stop, vyt0, vyt1);   /* top edge */
        draw_wall_line_clipped(start, stop, vyb0, vyb1);   /* bottom edge */
        if (!col_solid_get(start)) draw_vec(vx0, vyt0, vx0, vyb0);
        if (!col_solid_get(stop))  draw_vec(vx1, vyt1, vx1, vyb1);
    }
}

/* ================================================================
 * Renderer init
 *
 * PureDOOM is a single header with no official hook point for
 * R_StoreWallRange.  We patch it by providing our own version.
 *
 * Because DOOM_IMPLEMENTATION is defined in main.c and PureDOOM.h
 * is included there, R_StoreWallRange is defined in that translation
 * unit.  To intercept it we use a linker-level weak symbol override:
 *
 *   - PureDOOM.h defines R_StoreWallRange (strong symbol in main.c TU)
 *   - We redefine it here with our version
 *
 * To make this work cleanly, patch PureDOOM.h once:
 *   Rename the original to R_StoreWallRange_original and mark it
 *   __attribute__((weak)), then call it from our override below.
 *   See README for the one-line patch required.
 * ================================================================ */

void renderer_init(renderer_draw_fn draw_fn)
{
    s_draw_fn  = draw_fn;
    s_vec_count = 0;
}

void renderer_scene_begin(void)
{
    s_scene_active = 1;
    s_scene_new = 0;
    s_glow_tick++;
    /* Reset column occlusion for this frame */
    for (int i = 0; i < COL_SOLID_BYTES; i++) s_col_solid[i] = 0;
}

/* ----------------------------------------------------------------
 * Line deduplication helpers
 * Adjacent wall segments share edges — normalise direction and sort
 * so exact duplicates can be removed with a single linear pass.
 * ---------------------------------------------------------------- */
static void scene_line_normalise(scene_line_t *l)
{
    if (l->x0 > l->x1 || (l->x0 == l->x1 && l->y0 > l->y1)) {
        int16_t t;
        t = l->x0; l->x0 = l->x1; l->x1 = t;
        t = l->y0; l->y0 = l->y1; l->y1 = t;
    }
}

static int scene_line_cmp(const void *a, const void *b)
{
    const scene_line_t *la = (const scene_line_t *)a;
    const scene_line_t *lb = (const scene_line_t *)b;
    if (la->x0 != lb->x0) return (int)la->x0 - (int)lb->x0;
    if (la->y0 != lb->y0) return (int)la->y0 - (int)lb->y0;
    if (la->x1 != lb->x1) return (int)la->x1 - (int)lb->x1;
    return (int)la->y1 - (int)lb->y1;
}

/* Sort + deduplicate s_scene_buf[0..count). Returns new count. */
static int scene_dedup(int count)
{
    if (count <= 1) return count;
    /* Normalise direction so (A→B) and (B→A) are identical */
    for (int i = 0; i < count; i++)
        scene_line_normalise(&s_scene_buf[i]);
    /* Sort */
    extern void qsort(void*, unsigned, unsigned, int(*)(const void*, const void*));
    qsort(s_scene_buf, (unsigned)count, sizeof(scene_line_t), scene_line_cmp);
    /* Remove exact duplicates */
    int unique = 1;
    for (int i = 1; i < count; i++) {
        if (scene_line_cmp(&s_scene_buf[i], &s_scene_buf[unique - 1]) != 0)
            s_scene_buf[unique++] = s_scene_buf[i];
    }
    return unique;
}

void renderer_scene_end(void)
{
    s_scene_active = 0;
    /* ABOUT screen: discard any walls collected this tic */
    if (s_about_active) { s_scene_new = 0; s_scene_count = 0; return; }
    /* If DOOM rendered something this tic, deduplicate and update cache */
    if (s_scene_new > 0)
        s_scene_count = scene_dedup(s_scene_new);
    /* Flush scene buffer to the Vectrex — the ONLY place v_directDraw32
     * is called for walls, within one v_WaitRecal cycle. */
    if (s_scene_count > 0 && s_draw_fn) {
        for (int i = 0; i < s_scene_count && s_vec_count < MAX_VECTORS_PER_FRAME; i++) {
            s_draw_fn(s_scene_buf[i].x0, s_scene_buf[i].y0,
                      s_scene_buf[i].x1, s_scene_buf[i].y1);
            s_vec_count++;
        }
    }
}

void renderer_frame_end(void)
{
    s_vec_count = 0;
    /* TODO: pitrex_end_frame() — wait for beam to finish */
}

/*
 * R_StoreWallRange — override
 *
 * This replaces PureDOOM's raster wall renderer with vector output.
 * The function runs all of PureDOOM's BSP/clipping setup so that
 * worldtop, worldbottom, rw_scale, ds_p->scale2 are correct, then
 * calls our vector drawing code instead of the column-by-column loop.
 *
 * IMPORTANT: this requires the one-line patch to PureDOOM.h described
 * in README.md.  Without it the linker will complain about duplicate
 * R_StoreWallRange symbols.
 */
void R_StoreWallRange(int start, int stop)
{
    /* ---- reproduce PureDOOM setup (everything before the column loop) ---- */

    fixed_t hyp, sineval;
    angle_t distangle, offsetangle;

    if (ds_p == &drawsegs[MAXDRAWSEGS]) return;

    sidedef = curline->sidedef;
    linedef = curline->linedef;
    linedef->flags |= ML_MAPPED;

    rw_normalangle = curline->angle + ANG90;
    offsetangle = (angle_t)doom_abs((int)rw_normalangle - (int)rw_angle1);
    if (offsetangle > ANG90) offsetangle = ANG90;
    distangle = ANG90 - offsetangle;
    hyp = R_PointToDist(curline->v1->x, curline->v1->y);
    sineval = finesine[distangle >> ANGLETOFINESHIFT];
    rw_distance = FixedMul(hyp, sineval);

    ds_p->x1 = rw_x = start;
    ds_p->x2 = stop;
    ds_p->curline = curline;
    rw_stopx = stop + 1;

    ds_p->scale1 = rw_scale =
        R_ScaleFromGlobalAngle(viewangle + xtoviewangle[start]);

    if (stop > start)
    {
        ds_p->scale2 = R_ScaleFromGlobalAngle(viewangle + xtoviewangle[stop]);
        ds_p->scalestep = rw_scalestep = (ds_p->scale2 - rw_scale) / (stop - start);
    }
    else
    {
        ds_p->scale2 = ds_p->scale1;
        rw_scalestep = 0;
    }

    worldtop    = frontsector->ceilingheight - viewz;
    worldbottom = frontsector->floorheight   - viewz;

    /* silhouette / drawseg bookkeeping (needed for sprite clipping) */
    midtexture = toptexture = bottomtexture = maskedtexture = 0;
    ds_p->maskedtexturecol = 0;

    if (!backsector)
    {
        markfloor = markceiling = 1;
        ds_p->silhouette = SIL_BOTH;
        ds_p->sprtopclip = screenheightarray;
        ds_p->sprbottomclip = negonearray;
        ds_p->bsilheight = DOOM_MAXINT;
        ds_p->tsilheight = DOOM_MININT;
    }
    else
    {
        /* Wireframe: always provide safe non-NULL clip arrays so R_DrawSprite
         * never dereferences a NULL pointer. Sprite clipping won't be precise
         * but it won't crash. */
        ds_p->sprtopclip    = screenheightarray;
        ds_p->sprbottomclip = negonearray;
        ds_p->silhouette    = 0;
        ds_p->bsilheight    = DOOM_MAXINT;
        ds_p->tsilheight    = DOOM_MININT;

        if (frontsector->floorheight > backsector->floorheight)
            ds_p->silhouette = SIL_BOTTOM;
        else if (backsector->floorheight > viewz)
            ds_p->silhouette = SIL_BOTTOM;

        if (frontsector->ceilingheight < backsector->ceilingheight)
            ds_p->silhouette |= SIL_TOP;
        else if (backsector->ceilingheight < viewz)
            ds_p->silhouette |= SIL_TOP;

        worldhigh = backsector->ceilingheight - viewz;
        worldlow  = backsector->floorheight   - viewz;

        /* skip segs with no visible change (open portals, same heights) */
        if (backsector->ceilingheight == frontsector->ceilingheight
            && backsector->floorheight == frontsector->floorheight)
        {
            ds_p++;
            return;
        }
    }

    /* ---- shift heights (PureDOOM does this before topfrac/bottomfrac) ---- */
    worldtop    >>= 4;
    worldbottom >>= 4;

    /* ---- draw the wall as vector lines ---- */
    /* Treat as solid (occluder) if:
     *   - one-sided wall (!backsector), OR
     *   - two-sided but back sector has no opening (closed sector = pillar) */
    int is_solid = !backsector ||
                   (backsector && (backsector->ceilingheight <= backsector->floorheight));
    wall_draw_segment(start, stop, is_solid);

    /* ---- door marker: key icon at center of doorway ---- */
    {
        int sp = linedef->special;
        int is_door = backsector && (
            sp==1||sp==2||sp==3||sp==4||sp==16||
            sp==26||sp==27||sp==28||sp==29||
            sp==31||sp==32||sp==33||sp==34||sp==46||
            sp==61||sp==63||sp==90||
            sp==105||sp==106||sp==107||sp==108||sp==109);
        if (is_door) {
            /* vertical center of the door opening */
            int mid_screen_y = (worldtop + worldbottom) / 2; /* already >>4 */
            if (mid_screen_y < 0)        mid_screen_y = 0;
            if (mid_screen_y > DOOM_H-1) mid_screen_y = DOOM_H-1;
            int cx = doom_to_vec_x((start + stop) / 2);
            int cy = doom_to_vec_y(mid_screen_y);

            /* key: diamond bow + shaft + two teeth */
            /* bow (small diamond) */
            draw_vec(cx-3, cy,   cx,   cy+3);
            draw_vec(cx,   cy+3, cx+3, cy  );
            draw_vec(cx+3, cy,   cx,   cy-3);
            draw_vec(cx,   cy-3, cx-3, cy  );
            /* shaft going right */
            draw_vec(cx+3, cy, cx+10, cy  );
            /* two teeth downward */
            draw_vec(cx+6, cy, cx+6,  cy-3);
            draw_vec(cx+9, cy, cx+9,  cy-3);
        }
    }

    /* advance drawseg pointer so sprite clipping still works */
    ds_p++;
}

/* ================================================================
 * Glow helpers — draw each shape 3x (outer halo + core) to
 * simulate phosphor glow on Vectrex.  Extra passes = brighter.
 * ================================================================ */

/* pulse_scale() — returns 100..130 using sin LUT and glow tick
 * Period ~2 seconds at 60Hz (tick mod 128). */
static int pulse_scale(void)
{
    /* s_sin128 range is [-63,63]; shift to [0,63] then map to [100,130] */
    int sv = s_sin128[s_glow_tick & 0x7F];  /* -63..63 */
    return 100 + (sv + 63) * 30 / 126;       /* 100..130 */
}

/* Diamond — hostile / enemy placeholder */
static void __attribute__((unused)) draw_glow_diamond(int cx, int cy, int r)
{
    int ps = pulse_scale();
    /* outer halo pulses, inner is fixed — creates breathing glow */
    int rh = r * ps / 100;
    draw_vec(cx,    cy+rh, cx+rh, cy   );
    draw_vec(cx+rh, cy,    cx,    cy-rh);
    draw_vec(cx,    cy-rh, cx-rh, cy   );
    draw_vec(cx-rh, cy,    cx,    cy+rh);
    /* mid ring */
    int rm = r * (ps + 100) / 200;
    draw_vec(cx,    cy+rm, cx+rm, cy   );
    draw_vec(cx+rm, cy,    cx,    cy-rm);
    draw_vec(cx,    cy-rm, cx-rm, cy   );
    draw_vec(cx-rm, cy,    cx,    cy+rm);
    /* solid core */
    draw_vec(cx,   cy+r, cx+r, cy  );
    draw_vec(cx+r, cy,   cx,   cy-r);
    draw_vec(cx,   cy-r, cx-r, cy  );
    draw_vec(cx-r, cy,   cx,   cy+r);
}

/* Cross/plus — pickup / item placeholder */
static void __attribute__((unused)) draw_glow_cross(int cx, int cy, int r)
{
    int rh = r * pulse_scale() / 100;
    draw_vec(cx-rh, cy,  cx+rh, cy   );
    draw_vec(cx,  cy-rh, cx,    cy+rh);
    /* solid core */
    draw_vec(cx-r, cy,  cx+r, cy   );
    draw_vec(cx,  cy-r, cx,   cy+r);
}

/* Octagon — small bonus / ammo placeholder */
static void __attribute__((unused)) draw_glow_circle(int cx, int cy, int r)
{
    static const int8_t OX[8] = {  0, 71, 100,  71,   0, -71, -100, -71 };
    static const int8_t OY[8] = {100, 71,   0, -71, -100, -71,    0,  71 };
    int ps = pulse_scale();
    /* outer pulsing ring */
    for (int i = 0; i < 8; i++) {
        int ni = (i+1)&7;
        draw_vec(cx + OX[i] *r*ps/10000, cy + OY[i] *r*ps/10000,
                 cx + OX[ni]*r*ps/10000, cy + OY[ni]*r*ps/10000);
    }
    /* solid core octagon */
    for (int i = 0; i < 8; i++) {
        int ni = (i+1)&7;
        draw_vec(cx + OX[i] *r/100, cy + OY[i] *r/100,
                 cx + OX[ni]*r/100, cy + OY[ni]*r/100);
    }
}

/* ================================================================
 * V_DrawPatch — override
 *
 * Converts a DOOM patch (UI sprite / font glyph) to vector lines.
 *
 * Strategy: rasterize the patch columns into a 1-bit local bitmap,
 * then scan each row for runs of set pixels and emit one vector
 * per run.  This works for text, numbers, and simple UI elements.
 *
 * Large patches (status bar background, full-screen graphics) are
 * skipped to stay within the vector budget.
 * ================================================================ */

#define PATCH_MAX_W  64   /* ignore patches wider than this  */
#define PATCH_MAX_H  64   /* ignore patches taller than this */

void V_DrawPatch(int x, int y, int scrn, patch_t* patch)
{
    (void)scrn;

    int pw = SHORT(patch->width);
    int ph = SHORT(patch->height);



    /* skip large patches (backgrounds, status bar, etc.) */
    if (pw > PATCH_MAX_W || ph > PATCH_MAX_H || pw <= 0 || ph <= 0)
        return;

    int px = x - SHORT(patch->leftoffset);
    int py = y - SHORT(patch->topoffset);

    /* rasterize into a small 1-bit bitmap */
    static unsigned char bmp[PATCH_MAX_H][PATCH_MAX_W];
    /* zero only the area we'll use */
    for (int r = 0; r < ph; r++)
        for (int c = 0; c < pw; c++)
            bmp[r][c] = 0;

    for (int col = 0; col < pw; col++)
    {
        column_t* column = (column_t*)((byte*)patch + LONG(patch->columnofs[col]));
        while (column->topdelta != 0xff)
        {
            byte* source = (byte*)column + 3;
            int   top    = column->topdelta;
            int   len    = column->length;
            for (int row = 0; row < len; row++)
            {
                int r = top + row;
                if (r >= 0 && r < ph && source[row])
                    bmp[r][col] = 1;
            }
            column = (column_t*)((byte*)column + column->length + 4);
        }
    }

    /* scan rows → horizontal vector runs (used for status bar icons/numbers) */
    for (int row = 0; row < ph; row++)
    {
        int col = 0;
        while (col < pw)
        {
            /* find start of a run */
            while (col < pw && !bmp[row][col]) col++;
            if (col >= pw) break;
            int run_start = col;
            /* find end of run */
            while (col < pw && bmp[row][col]) col++;
            int run_end = col - 1;

            /* skip single-pixel runs — too noisy */
            if (run_end == run_start) continue;

            /* map patch pixel coords → DOOM screen coords */
            int sx0 = px + run_start;
            int sy  = py + row;
            int sx1 = px + run_end;

            /* clip to screen */
            if (sy < 0 || sy >= DOOM_H) continue;
            sx0 = sx0 < 0 ? 0 : sx0;
            sx1 = sx1 >= DOOM_W ? DOOM_W - 1 : sx1;
            if (sx0 > sx1) continue;

            draw_vec(doom_to_vec_x(sx0), doom_to_vec_y(sy),
                     doom_to_vec_x(sx1), doom_to_vec_y(sy));
        }
    }
}

/* ================================================================
 * D_PageDrawer — override
 *
 * Called when gamestate == GS_DEMOSCREEN (title/attract cycle).
 * pagename is "TITLEPIC", "CREDIT", or "HELP2".
 * We draw the DOOM logo vector art for TITLEPIC; blank for the rest.
 * ================================================================ */
void D_PageDrawer(void)
{
    extern char* pagename;

    if (!pagename) return;

    /* Only draw the vector logo on the title screen */
    if (doom_strcmp(pagename, "TITLEPIC") == 0)
    {
        s_vec_count = 0;
        for (int i = 0; i < doom_logo_seg_count; i++)
        {
            draw_vec(doom_logo_segs[i].x0, doom_logo_segs[i].y0,
                     doom_logo_segs[i].x1, doom_logo_segs[i].y1);
        }
    }
    /* CREDIT / HELP2: leave screen blank — vectors are already cleared */
}

/* ================================================================
 * HUD — helper: draw a vec_seg_t array with an (ox,oy) offset
 * ================================================================ */
static void draw_vec_shape(const vec_seg_t* segs, int count, int ox, int oy)
{
    for (int i = 0; i < count; i++)
        draw_vec(segs[i].x0 + ox, segs[i].y0 + oy,
                 segs[i].x1 + ox, segs[i].y1 + oy);
}

/* scale_num/scale_den: e.g. 2,5 = 40%.  ox,oy applied after scaling. */
static void draw_vec_shape_scaled(const vec_seg_t* segs, int count,
                                  int ox, int oy,
                                  int scale_num, int scale_den)
{
    for (int i = 0; i < count; i++)
        draw_vec(ox + segs[i].x0 * scale_num / scale_den,
                 oy + segs[i].y0 * scale_num / scale_den,
                 ox + segs[i].x1 * scale_num / scale_den,
                 oy + segs[i].y1 * scale_num / scale_den);
}

/* ================================================================
 * draw_vec_shape_3d — 3D-rotated draw from vertex-indexed table
 *
 * Matches VPY DRAW_VECTOR_3D calling convention:
 *   verts      — unique vertex table (x,y,z clamped ±63)
 *   vert_count — number of unique vertices
 *   paths      — path table (pt_count, closed, idx[])
 *   path_count — number of paths
 *   ax,ay,az   — Euler XYZ rotation angles, 0–127 = 0–360°
 *   ox,oy      — screen offset (Vectrex coords ±127)
 *   scale_num/den — optional scale (use 1,1 for no scale)
 *
 * Phase 1: rotate all unique vertices → projected 2D cache
 * Phase 2: draw edges by index lookup (vertex-dedup saves CPU)
 * ================================================================ */
#define DV3D_MAX_VERTS 64
static void __attribute__((unused)) draw_vec_shape_3d(
    const vec3d_vert_t* verts, int vert_count,
    const vec3d_path_t* paths, int path_count,
    int ax, int ay, int az,
    int ox, int oy,
    int scale_num, int scale_den)
{
    /* Phase 1: project unique vertices */
    int16_t sx[DV3D_MAX_VERTS], sy[DV3D_MAX_VERTS];
    if (vert_count > DV3D_MAX_VERTS) vert_count = DV3D_MAX_VERTS;
    for (int i = 0; i < vert_count; i++) {
        int px, py;
        rot3d(verts[i].x, verts[i].y, verts[i].z, ax, ay, az, &px, &py);
        sx[i] = (int16_t)(ox + px * scale_num / scale_den);
        sy[i] = (int16_t)(oy + py * scale_num / scale_den);
    }
    /* Phase 2: draw paths by index */
    for (int p = 0; p < path_count; p++) {
        int n = paths[p].pt_count;
        const uint8_t* idx = paths[p].idx;
        for (int i = 0; i < n - 1; i++)
            draw_vec(sx[idx[i]], sy[idx[i]], sx[idx[i+1]], sy[idx[i+1]]);
        if (paths[p].closed && n >= 2)
            draw_vec(sx[idx[n-1]], sy[idx[n-1]], sx[idx[0]], sy[idx[0]]);
    }
}

/* ================================================================
 * HUD — drawn from .vec assets in assets/hud/ (regenerated by make).
 *
 * Digits: design in x∈[-4,4], y∈[-6,7] (1:1 scale, centre at digit box centre).
 * Face:   design in ±9 Vectrex units (1:1 scale, centre at (cx,cy)).
 * Percent: design in ±3 Vectrex units (1:1 scale, centre at (x+S,y+S)).
 * ================================================================ */

/* draw_number_hud — right-aligned integer 0-999 using .vec digit shapes.
 * rx = right edge X,  y = bottom edge Y,  DW/DH = digit size,  SP = gap */
static void draw_number_hud(int num, int rx, int y, int DW, int DH, int SP)
{
    if (num < 0)   num = 0;
    if (num > 999) num = 999;
    static const vec_seg_t* const segs[10] = {
        digit_0_segs, digit_1_segs, digit_2_segs, digit_3_segs, digit_4_segs,
        digit_5_segs, digit_6_segs, digit_7_segs, digit_8_segs, digit_9_segs };
    static const int counts[10] = {
        digit_0_seg_count, digit_1_seg_count, digit_2_seg_count, digit_3_seg_count,
        digit_4_seg_count, digit_5_seg_count, digit_6_seg_count, digit_7_seg_count,
        digit_8_seg_count, digit_9_seg_count };
    int step = DW + SP;
    int hw = DW/2, hh = DH/2;
    int cy = y + hh;
    draw_vec_shape(segs[num % 10],        counts[num % 10],        rx - hw,          cy);
    draw_vec_shape(segs[(num/10) % 10],   counts[(num/10) % 10],   rx - hw - step,   cy);
    if (num >= 100)
        draw_vec_shape(segs[num / 100],   counts[num / 100],       rx - hw - 2*step, cy);
}

/* Doomguy face states */
typedef enum {
    FACE_NORMAL = 0,
    FACE_PAIN,
    FACE_EVIL,
    FACE_GOD,
    FACE_DEAD
} face_state_t;

static void draw_face_hud(int cx, int cy, int R, face_state_t state)
{
    static const vec_seg_t* const segs[5] = {
        face_normal_segs, face_pain_segs, face_evil_segs,
        face_god_segs,    face_dead_segs };
    static const int counts[5] = {
        face_normal_seg_count, face_pain_seg_count, face_evil_seg_count,
        face_god_seg_count,    face_dead_seg_count };
    if ((unsigned)state < 5)
        draw_vec_shape_scaled(segs[state], counts[state], cx, cy, R, 9);
}

/* Forward declarations for stroke font helpers defined later in this file */
static int  draw_string_small(const char* str, int vx, int vy, int div);
static void draw_centered_small(const char* str, int vy, int div);
static void itoa3(int n, char* buf);

/* Forward declaration for menu helper (defined just before M_Drawer) */
static const char* m_lump_text(const char* name);

/* ================================================================
 * ST_Drawer — override
 *
 * HUD strip (Vectrex Y = -105 to -127, 22 units):
 *
 *   Y -106 to -119 : 7-seg numbers  (AMMO / HEALTH% / face / ARMOR%)
 *   Y -120         : 1-unit gap
 *   Y -121 to -126 : stroke-font labels  (AMMO / HEALTH / ARMOR)
 *                    + right ammo panel  (BULL/SHEL/RCKT/CELL + counts)
 *   Y -127         : bottom margin
 *
 * Label scale: div=24  → char ≈3 vx wide, 5 vx tall
 * Panel scale: div=30  → char ≈3 vx wide, 4 vx tall (4 rows × 5 = 20 vx)
 * ================================================================ */
void ST_Drawer(doom_boolean fullscreen, doom_boolean refresh)
{
    (void)fullscreen; (void)refresh;
    if (gamestate != GS_LEVEL || !gametic) return;

    player_t* plyr = &players[consoleplayer];

    /* ---- bar dividing line ---- */
    draw_vec(-127, -105, 127, -105);

    /* ---- layout constants (Vectrex coords, Y-up) ---- */
    /* Digit size: 8 wide × 13 tall, gap 2 */
    const int DW = 8, DH = 13, SP = 2;
    const int BAR_Y = -119; /* bottom of digits (top is BAR_Y+DH = -106) */

    /* ---- AMMO (left) ---- */
    {
        ammotype_t at = weaponinfo[plyr->readyweapon].ammo;
        int ammo = (at == am_noammo) ? 0 : plyr->ammo[at];
        /* right-edge of 3-digit block at X = -80 */
        draw_number_hud(ammo, -80, BAR_Y, DW, DH, SP);
    }

    /* ---- HEALTH (center-left) ---- */
    {
        int hp = plyr->health < 0 ? 0 : plyr->health;
        /* right-edge at X = -35 */
        draw_number_hud(hp, -35, BAR_Y, DW, DH, SP);
        /* '%' sign right after */
        draw_vec_shape(percent_segs, percent_seg_count, -30, BAR_Y + DH/4 + 3);
    }

    /* ---- FACE (center) ---- */
    {
        face_state_t fs;
        if (plyr->health <= 0)
            fs = FACE_DEAD;
        else if (plyr->powers[pw_invulnerability])
            fs = FACE_GOD;
        else if (plyr->damagecount > 20)
            fs = FACE_PAIN;
        else if (plyr->bonuscount > 0)
            fs = FACE_EVIL;
        else
            fs = FACE_NORMAL;
        draw_face_hud(0, -116, 9, fs);
    }

    /* ---- ARMOR (center-right) ---- */
    {
        int arm = plyr->armorpoints < 0 ? 0 : plyr->armorpoints;
        /* right-edge at X = 65 */
        draw_number_hud(arm, 65, BAR_Y, DW, DH, SP);
        draw_vec_shape(percent_segs, percent_seg_count, 70, BAR_Y + DH/4 + 3);
    }

    /* ---- HUD text labels (div=24: ~3vx wide, ~5vx tall) ---- */
    /* Positioned at LY=-126 so top (LY+5=-121) clears the digit bottom (-119). */
    {
        const int LY = -126;
        /* "AMMO" — center of ammo digit block (~x=-95), label 12vx → start -101 */
        draw_string_small("AMMO",   -101, LY, 24);
        /* "HEALTH" — center of health block (~x=-50), label 18vx → start -59 */
        draw_string_small("HEALTH",  -59, LY, 24);
        /* "ARMOR" — center of armor block (~x=+50), label 15vx → start +43 */
        draw_string_small("ARMOR",    43, LY, 24);
    }

    /* ---- Right ammo panel: BULL/SHEL/RCKT/CELL with current counts ----
     * div=30 → ~3vx wide, ~4vx tall; 4 rows × 5vx = 20vx (Y -112 to -127)
     * Positioned at X=76, well clear of armor digits (right-edge +65) + % sign. */
    {
        static const char* const s_labels[4] = {"BULL","SHEL","RCKT","CELL"};
        static const ammotype_t  s_ammot[4]  = {am_clip, am_shell, am_misl, am_cell};
        static const int         s_yrow[4]   = {-127, -122, -117, -112};
        char nbuf[5];
        for (int i = 0; i < 4; i++) {
            int vx = draw_string_small(s_labels[i], 76, s_yrow[i], 30);
            vx += 3; /* gap between label and count */
            itoa3(plyr->ammo[s_ammot[i]], nbuf);
            draw_string_small(nbuf, vx, s_yrow[i], 30);
        }
    }

    /* ---- Death overlay: flash "YOU DIED / PRESS SPACE" ---- */
    if (plyr->playerstate == PST_DEAD) {
        /* Only draw on even glow pulses to create a slow flash */
        if (pulse_scale() > 115) {
            /* "YOU DIED" centered — div=8 → ~7vx/char, 8 chars × 7 = 56vx wide → start -28 */
            draw_string_small("YOU DIED", -32, 20, 8);
            /* "PRESS SPACE" centered — div=11 → ~5vx/char, 11 chars × 5 = 55vx → start -28 */
            draw_string_small("PRESS A BUTTON", -38, 5, 11);
        }
    }
}

#include "sprites_assets.h"

/* ================================================================
 * Enemy / pickup drawing — now driven by .vec assets in
 * assets/sprites/ *.vec (edit in Vectrex Studio, regenerated by make).
 *
 * draw_sprite_vec(segs, count, cx, cy, r):
 *   Draws the shape twice: once at pulse scale (glow halo) and
 *   once at base r (solid core).  Scale maps .vec ±63 → r pixels.
 * ================================================================ */

static void draw_sprite_vec(const vec_seg_t* segs, int count,
                            int cx, int cy, int r)
{
    int rg = r * pulse_scale() / 100;
    /* glow halo */
    draw_vec_shape_scaled(segs, count, cx, cy, rg, 63);
    /* solid core */
    draw_vec_shape_scaled(segs, count, cx, cy, r,  63);
}

/* --- enemy type lookup ----------------------------------------- */
#define ET_HUMANOID  0
#define ET_IMP       1
#define ET_CACO      2
#define ET_SKULL     3
#define ET_DEMON     4
#define ET_BARON     5
#define ET_CYBER     6
#define ET_SPIDER    7
#define ET_OTHER     8

static int get_enemy_type(int patch_idx)
{
    const char* n = lumpinfo[firstspritelump + patch_idx].name;
    if (n[0]=='P'&&n[1]=='O'&&n[2]=='S'&&n[3]=='S') return ET_HUMANOID;
    if (n[0]=='S'&&n[1]=='P'&&n[2]=='O'&&n[3]=='S') return ET_HUMANOID;
    if (n[0]=='C'&&n[1]=='P'&&n[2]=='O'&&n[3]=='S') return ET_HUMANOID;
    if (n[0]=='V'&&n[1]=='I'&&n[2]=='L'&&n[3]=='E') return ET_HUMANOID;
    if (n[0]=='T'&&n[1]=='R'&&n[2]=='O'&&n[3]=='O') return ET_IMP;
    if (n[0]=='H'&&n[1]=='E'&&n[2]=='A'&&n[3]=='D') return ET_CACO;
    if (n[0]=='P'&&n[1]=='A'&&n[2]=='I'&&n[3]=='N') return ET_CACO;
    if (n[0]=='S'&&n[1]=='K'&&n[2]=='U'&&n[3]=='L') return ET_SKULL;
    if (n[0]=='S'&&n[1]=='A'&&n[2]=='R'&&n[3]=='G') return ET_DEMON;
    if (n[0]=='B'&&n[1]=='O'&&n[2]=='S'&&n[3]=='S') return ET_BARON;
    if (n[0]=='B'&&n[1]=='O'&&n[2]=='S'&&n[3]=='2') return ET_BARON;
    if (n[0]=='F'&&n[1]=='A'&&n[2]=='T'&&n[3]=='T') return ET_BARON;
    if (n[0]=='S'&&n[1]=='K'&&n[2]=='E'&&n[3]=='L') return ET_BARON;
    if (n[0]=='C'&&n[1]=='Y'&&n[2]=='B'&&n[3]=='R') return ET_CYBER;
    if (n[0]=='S'&&n[1]=='P'&&n[2]=='I'&&n[3]=='D') return ET_SPIDER;
    if (n[0]=='B'&&n[1]=='S'&&n[2]=='P'&&n[3]=='I') return ET_SPIDER;
    return ET_OTHER;
}

static void draw_enemy(int cx, int cy, int r, int et)
{
    typedef struct { const vec_seg_t* segs; int count; } entry_t;
    static const entry_t tbl[ET_OTHER] = {
        { enemy_humanoid_segs, enemy_humanoid_seg_count },
        { enemy_imp_segs,      enemy_imp_seg_count      },
        { enemy_caco_segs,     enemy_caco_seg_count     },
        { enemy_skull_segs,    enemy_skull_seg_count    },
        { enemy_demon_segs,    enemy_demon_seg_count    },
        { enemy_baron_segs,    enemy_baron_seg_count    },
        { enemy_cyber_segs,    enemy_cyber_seg_count    },
        { enemy_spider_segs,   enemy_spider_seg_count   },
    };
    if (et < ET_OTHER)
        draw_sprite_vec(tbl[et].segs, tbl[et].count, cx, cy, r);
    else
        draw_sprite_vec(unknown_enemy_segs, unknown_enemy_seg_count, cx, cy, r);
}

/* --- pickup type lookup ----------------------------------------- */
#define PT_HEALTH    0
#define PT_ARMOR     1
#define PT_AMMO_CLIP 2
#define PT_AMMO_RCKT 3
#define PT_AMMO_CELL 4
#define PT_WEAPON    5
#define PT_KEY       6
#define PT_POWERUP   7
#define PT_BARREL    8
#define PT_OTHER     9

static int get_pickup_type(int patch_idx)
{
    const char* n = lumpinfo[firstspritelump + patch_idx].name;
    if (n[0]=='S'&&n[1]=='T'&&n[2]=='I'&&n[3]=='M') return PT_HEALTH;
    if (n[0]=='M'&&n[1]=='E'&&n[2]=='D'&&n[3]=='I') return PT_HEALTH;
    if (n[0]=='B'&&n[1]=='O'&&n[2]=='N'&&n[3]=='1') return PT_HEALTH;
    if (n[0]=='A'&&n[1]=='R'&&n[2]=='M'&&n[3]=='1') return PT_ARMOR;
    if (n[0]=='A'&&n[1]=='R'&&n[2]=='M'&&n[3]=='2') return PT_ARMOR;
    if (n[0]=='B'&&n[1]=='O'&&n[2]=='N'&&n[3]=='2') return PT_ARMOR;
    if (n[0]=='C'&&n[1]=='L'&&n[2]=='I'&&n[3]=='P') return PT_AMMO_CLIP;
    if (n[0]=='A'&&n[1]=='M'&&n[2]=='M'&&n[3]=='O') return PT_AMMO_CLIP;
    if (n[0]=='S'&&n[1]=='H'&&n[2]=='E'&&n[3]=='L') return PT_AMMO_CLIP;
    if (n[0]=='S'&&n[1]=='B'&&n[2]=='O'&&n[3]=='X') return PT_AMMO_CLIP;
    if (n[0]=='R'&&n[1]=='C'&&n[2]=='K'&&n[3]=='T') return PT_AMMO_RCKT;
    if (n[0]=='M'&&n[1]=='I'&&n[2]=='S'&&n[3]=='L') return PT_AMMO_RCKT;
    if (n[0]=='B'&&n[1]=='R'&&n[2]=='O'&&n[3]=='K') return PT_AMMO_RCKT;
    if (n[0]=='C'&&n[1]=='E'&&n[2]=='L'&&n[3]=='L') return PT_AMMO_CELL;
    if (n[0]=='C'&&n[1]=='E'&&n[2]=='L'&&n[3]=='P') return PT_AMMO_CELL;
    if (n[0]=='S'&&n[1]=='H'&&n[2]=='O'&&n[3]=='T') return PT_WEAPON;
    if (n[0]=='S'&&n[1]=='G'&&n[2]=='N'&&n[3]=='2') return PT_WEAPON;
    if (n[0]=='M'&&n[1]=='G'&&n[2]=='U'&&n[3]=='N') return PT_WEAPON;
    if (n[0]=='L'&&n[1]=='A'&&n[2]=='U'&&n[3]=='N') return PT_WEAPON;
    if (n[0]=='P'&&n[1]=='L'&&n[2]=='A'&&n[3]=='S') return PT_WEAPON;
    if (n[0]=='B'&&n[1]=='F'&&n[2]=='U'&&n[3]=='G') return PT_WEAPON;
    if (n[0]=='C'&&n[1]=='S'&&n[2]=='A'&&n[3]=='W') return PT_WEAPON;
    if (n[0]=='B'&&n[1]=='K'&&n[2]=='E'&&n[3]=='Y') return PT_KEY;
    if (n[0]=='R'&&n[1]=='K'&&n[2]=='E'&&n[3]=='Y') return PT_KEY;
    if (n[0]=='Y'&&n[1]=='K'&&n[2]=='E'&&n[3]=='Y') return PT_KEY;
    if (n[0]=='B'&&n[1]=='S'&&n[2]=='K'&&n[3]=='U') return PT_KEY;
    if (n[0]=='R'&&n[1]=='S'&&n[2]=='K'&&n[3]=='U') return PT_KEY;
    if (n[0]=='Y'&&n[1]=='S'&&n[2]=='K'&&n[3]=='U') return PT_KEY;
    if (n[0]=='S'&&n[1]=='O'&&n[2]=='U'&&n[3]=='L') return PT_POWERUP;
    if (n[0]=='M'&&n[1]=='E'&&n[2]=='G'&&n[3]=='A') return PT_POWERUP;
    if (n[0]=='P'&&n[1]=='I'&&n[2]=='N'&&n[3]=='S') return PT_POWERUP;
    if (n[0]=='S'&&n[1]=='U'&&n[2]=='I'&&n[3]=='T') return PT_POWERUP;
    if (n[0]=='P'&&n[1]=='S'&&n[2]=='T'&&n[3]=='R') return PT_POWERUP;
    if (n[0]=='P'&&n[1]=='V'&&n[2]=='I'&&n[3]=='S') return PT_POWERUP;
    if (n[0]=='B'&&n[1]=='M'&&n[2]=='A'&&n[3]=='P') return PT_POWERUP;
    if (n[0]=='B'&&n[1]=='A'&&n[2]=='R'&&n[3]=='1') return PT_BARREL;
    if (n[0]=='B'&&n[1]=='E'&&n[2]=='X'&&n[3]=='P') return PT_BARREL;
    return PT_OTHER;
}

/* draw_pickup_spinning — like draw_sprite_vec but spins on Y axis each frame.
 * Uses gametic for the angle (0-127 = 0-360°).
 * Rotation speed: (gametic * 2) & 127 → one full spin every ~64 ticks (~1.8 s).
 * Also adds a gentle vertical bob: ±3 units at half the spin speed.
 * Each flat .vec endpoint (x,y) treated as (x,y,z=0); Y-rotation gives
 * the classic "spinning coin" effect without changing any asset files. */
static void draw_pickup_spinning(const vec_seg_t* segs, int count,
                                 int cx, int cy, int r)
{
    int ay  = (gametic * 2) & 127;                     /* Y-rotation angle   */
    int bob = smul(4, (gametic) & 127);                /* ±4 unit vertical bob */
    int rg  = r * pulse_scale() / 100;
    int oy  = cy + bob;

    for (int pass = 0; pass < 2; pass++) {
        int sc = (pass == 0) ? rg : r;  /* pass 0 = glow halo, 1 = solid core */
        for (int i = 0; i < count; i++) {
            /* scale .vec coords (±63) → r, then apply Y-rotation: x'=x*cos(ay), y'=y */
            int ax0 = segs[i].x0 * sc / 63;
            int ay0 = segs[i].y0 * sc / 63;
            int ax1 = segs[i].x1 * sc / 63;
            int ay1 = segs[i].y1 * sc / 63;

            int sx0 = cx + cmul(ax0, ay);
            int sy0 = oy + ay0;
            int sx1 = cx + cmul(ax1, ay);
            int sy1 = oy + ay1;
            draw_vec(sx0, sy0, sx1, sy1);
        }
    }
}

static void draw_pickup(int cx, int cy, int r, int pt)
{
    typedef struct { const vec_seg_t* segs; int count; } entry_t;
    static const entry_t tbl[PT_OTHER] = {
        { pickup_health_segs,     pickup_health_seg_count     },
        { pickup_armor_segs,      pickup_armor_seg_count      },
        { pickup_ammo_clip_segs,  pickup_ammo_clip_seg_count  },
        { pickup_rocket_segs,     pickup_rocket_seg_count     },
        { pickup_cell_segs,       pickup_cell_seg_count       },
        { pickup_weapon_segs,     pickup_weapon_seg_count     },
        { pickup_key_segs,        pickup_key_seg_count        },
        { pickup_powerup_segs,    pickup_powerup_seg_count    },
        { pickup_barrel_segs,     pickup_barrel_seg_count     },
    };
    const vec_seg_t* segs;
    int count;
    if (pt < PT_OTHER) {
        segs  = tbl[pt].segs;
        count = tbl[pt].count;
    } else {
        segs  = unknown_pickup_segs;
        count = unknown_pickup_seg_count;
    }
    draw_pickup_spinning(segs, count, cx, cy, r);
}

/* ================================================================
 * sprite_blocked_by_wall — returns 1 if a solid (one-sided) line
 * intersects the segment from player to sprite world position.
 *
 * Uses 2D cross-product sign test (integer, no FPU).
 * Coordinates are shifted right by 19 bits (FRACBITS+3) to keep
 * products within int32 range (max coord ~4096 map units).
 * ================================================================ */
static int sprite_blocked_by_wall(fixed_t spx, fixed_t spy)
{
    /* Player world position */
    int px = players[consoleplayer].mo->x >> 19;
    int py = players[consoleplayer].mo->y >> 19;
    int sx = spx >> 19;
    int sy = spy >> 19;

    /* Bounding box of the player->sprite segment (for quick rejection) */
    int bbx0 = px < sx ? px : sx;
    int bbx1 = px > sx ? px : sx;
    int bby0 = py < sy ? py : sy;
    int bby1 = py > sy ? py : sy;

    for (int i = 0; i < numlines; i++) {
        line_t* ld = &lines[i];
        /* Two-sided lines are passable openings — skip them */
        if (ld->backsector) continue;

        int lx0 = ld->v1->x >> 19;
        int ly0 = ld->v1->y >> 19;
        int lx1 = ld->v2->x >> 19;
        int ly1 = ld->v2->y >> 19;

        /* Bounding-box pre-rejection */
        int lbx0 = lx0 < lx1 ? lx0 : lx1;
        int lbx1 = lx0 > lx1 ? lx0 : lx1;
        int lby0 = ly0 < ly1 ? ly0 : ly1;
        int lby1 = ly0 > ly1 ? ly0 : ly1;
        if (lbx1 < bbx0 || lbx0 > bbx1 || lby1 < bby0 || lby0 > bby1)
            continue;

        /* Cross-product sign test for segment intersection */
        int d1 = (lx1-lx0)*(py-ly0) - (ly1-ly0)*(px-lx0);
        int d2 = (lx1-lx0)*(sy-ly0) - (ly1-ly0)*(sx-lx0);
        int d3 = (sx-px)*(ly0-py) - (sy-py)*(lx0-px);
        int d4 = (sx-px)*(ly1-py) - (sy-py)*(lx1-px);

        if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
            ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
            return 1;
    }
    return 0;
}

/* ================================================================
 * renderer_draw_automap — draw all map linedefs when automapactive
 *
 * Reads lines[]/numlines directly from PureDOOM internals.
 * Computes map bounds once per level, then maps to [-115,115] Vectrex coords.
 * ================================================================ */
void renderer_draw_automap(void)
{
    if (!automapactive || !s_draw_fn || numlines == 0) return;

    /* Compute map bounds (cached: reset when s_scene_count drops to 0 between levels) */
    static int s_map_min_x = 0, s_map_max_x = 0;
    static int s_map_min_y = 0, s_map_max_y = 0;
    static int s_map_scale = 0;
    static int s_map_lines_cached = -1;

    if (s_map_lines_cached != numlines) {
        s_map_min_x = lines[0].v1->x >> FRACBITS;
        s_map_max_x = s_map_min_x;
        s_map_min_y = lines[0].v1->y >> FRACBITS;
        s_map_max_y = s_map_min_y;
        for (int i = 0; i < numlines; i++) {
            int x1 = lines[i].v1->x >> FRACBITS, y1 = lines[i].v1->y >> FRACBITS;
            int x2 = lines[i].v2->x >> FRACBITS, y2 = lines[i].v2->y >> FRACBITS;
            if (x1 < s_map_min_x) s_map_min_x = x1;
            if (x1 > s_map_max_x) s_map_max_x = x1;
            if (y1 < s_map_min_y) s_map_min_y = y1;
            if (y1 > s_map_max_y) s_map_max_y = y1;
            if (x2 < s_map_min_x) s_map_min_x = x2;
            if (x2 > s_map_max_x) s_map_max_x = x2;
            if (y2 < s_map_min_y) s_map_min_y = y2;
            if (y2 > s_map_max_y) s_map_max_y = y2;
        }
        int w = s_map_max_x - s_map_min_x;
        int h = s_map_max_y - s_map_min_y;
        s_map_scale = (w > h) ? w : h;
        if (s_map_scale == 0) s_map_scale = 1;
        s_map_lines_cached = numlines;
    }

    for (int i = 0; i < numlines; i++) {
        int x1 = lines[i].v1->x >> FRACBITS;
        int y1 = lines[i].v1->y >> FRACBITS;
        int x2 = lines[i].v2->x >> FRACBITS;
        int y2 = lines[i].v2->y >> FRACBITS;
        /* normalize to [-115, 115] */
        int sx0 = (x1 - s_map_min_x) * 230 / s_map_scale - 115;
        int sy0 = (y1 - s_map_min_y) * 230 / s_map_scale - 115;
        int sx1 = (x2 - s_map_min_x) * 230 / s_map_scale - 115;
        int sy1 = (y2 - s_map_min_y) * 230 / s_map_scale - 115;
        s_draw_fn(sx0, sy0, sx1, sy1);
        if (++s_vec_count >= MAX_VECTORS_PER_FRAME) break;
    }
}

/*
 * renderer_draw_sprites — draw vector placeholders for world sprites
 *
 * Iterates the vissprite list built by DOOM's BSP renderer and draws
 * a glow shape at each sprite's screen position.
 * Sprites hidden behind solid (one-sided) lines are skipped.
 * ================================================================ */
/* classify_sprite: 0=enemy, 1=pickup, 2=decoration, -1=skip (blood/puff/projectile/player) */
static int classify_sprite(int patch_idx)
{
    const char* n = lumpinfo[firstspritelump + patch_idx].name;
    /* Skip: blood splats, bullet puffs, player corpse, projectiles, smoke */
    if (n[0]=='B'&&n[1]=='L'&&n[2]=='U'&&n[3]=='D') return -1;
    if (n[0]=='P'&&n[1]=='U'&&n[2]=='F'&&n[3]=='F') return -1;
    if (n[0]=='P'&&n[1]=='L'&&n[2]=='A'&&n[3]=='Y') return -1;
    if (n[0]=='B'&&n[1]=='A'&&n[2]=='L') return -1; /* BAL1/BAL2 fireballs */
    if (n[0]=='M'&&n[1]=='I'&&n[2]=='S'&&n[3]=='L') return -1; /* rocket in flight */
    if (n[0]=='M'&&n[1]=='A'&&n[2]=='N'&&n[3]=='F') return -1; /* plasma */
    if (n[0]=='A'&&n[1]=='P'&&n[2]=='L') return -1; /* APLS/APLX plasma ball */
    if (n[0]=='R'&&n[1]=='B'&&n[2]=='A'&&n[3]=='L') return -1; /* rev tracer */
    if (n[0]=='T'&&n[1]=='R'&&n[2]=='A'&&n[3]=='C') return -1; /* tracer */
    /* Skip: floor gore / blood pools / gibs */
    if (n[0]=='B'&&n[1]=='L'&&n[2]=='O'&&n[3]=='O') return -1; /* BLOO blood pool (flat) */
    if (n[0]=='D'&&n[1]=='E'&&n[2]=='A'&&n[3]=='D') return 2;  /* DEAD pre-placed corpses */
    if (n[0]=='G'&&n[1]=='I'&&n[2]=='B'&&n[3]=='S') return 2;  /* GIBS floor gore */
    /* Decorations: columns, lamps, torches, hanging things (draw as pillar) */
    if (n[0]=='C'&&n[1]=='O'&&n[2]=='L'&&n[3]=='U') return 2; /* COLU column */
    if (n[0]=='E'&&n[1]=='L'&&n[2]=='E'&&n[3]=='C') return 2; /* ELEC tech lamp */
    if (n[0]=='P'&&n[1]=='O'&&n[2]=='L') return 2;            /* POL1-POL6 */
    if (n[0]=='G'&&n[1]=='O'&&n[2]=='R') return 2;            /* GOR* hanging bodies */
    if (n[0]=='H'&&n[1]=='A'&&n[2]=='N') return 2;            /* HAN* hanging */
    if (n[0]=='T'&&n[1]=='R'&&n[2]=='E'&&n[3]=='E') return 2; /* TREE */
    if (n[0]=='C'&&n[1]=='A'&&n[2]=='N'&&n[3]=='D') return 2; /* CAND candelabra */
    if (n[0]=='C'&&n[1]=='B'&&n[2]=='R'&&n[3]=='A') return 2; /* CBRA candelabra */
    if (n[0]=='T'&&n[1]=='O'&&n[2]=='R'&&n[3]=='C') return 2; /* TORC torch */
    if (n[0]=='F'&&n[1]=='C'&&n[2]=='O'&&n[3]=='L') return 2; /* FCOL fire column */
    if (n[0]=='S'&&n[1]=='M'&&n[2]=='T'&&n[3]=='R') return 2; /* SMTR tech lamp */
    if (n[0]=='T'&&n[1]=='L'&&n[2]=='M'&&n[3]=='P') return 2; /* TLMP tech lamp */
    if (n[0]=='B'&&n[1]=='A'&&n[2]=='R'&&n[3]=='1') return 2; /* BAR1 barrel */
    if (n[0]=='B'&&n[1]=='E'&&n[2]=='X'&&n[3]=='P') return 2; /* BEXP barrel explosion */
    /* Enemies (by 4-char sprite prefix) */
    if (n[0]=='P'&&n[1]=='O'&&n[2]=='S'&&n[3]=='S') return 0;
    if (n[0]=='S'&&n[1]=='P'&&n[2]=='O'&&n[3]=='S') return 0;
    if (n[0]=='C'&&n[1]=='P'&&n[2]=='O'&&n[3]=='S') return 0;
    if (n[0]=='V'&&n[1]=='I'&&n[2]=='L'&&n[3]=='E') return 0;
    if (n[0]=='T'&&n[1]=='R'&&n[2]=='O'&&n[3]=='O') return 0;
    if (n[0]=='S'&&n[1]=='A'&&n[2]=='R'&&n[3]=='G') return 0;
    if (n[0]=='H'&&n[1]=='E'&&n[2]=='A'&&n[3]=='D') return 0;
    if (n[0]=='S'&&n[1]=='K'&&n[2]=='U'&&n[3]=='L') return 0;
    if (n[0]=='B'&&n[1]=='O'&&n[2]=='S'&&n[3]=='S') return 0;
    if (n[0]=='B'&&n[1]=='O'&&n[2]=='S'&&n[3]=='2') return 0;
    if (n[0]=='B'&&n[1]=='S'&&n[2]=='P'&&n[3]=='I') return 0;
    if (n[0]=='C'&&n[1]=='Y'&&n[2]=='B'&&n[3]=='R') return 0;
    if (n[0]=='S'&&n[1]=='P'&&n[2]=='I'&&n[3]=='D') return 0;
    if (n[0]=='P'&&n[1]=='A'&&n[2]=='I'&&n[3]=='N') return 0;
    if (n[0]=='S'&&n[1]=='K'&&n[2]=='E'&&n[3]=='L') return 0;
    if (n[0]=='F'&&n[1]=='A'&&n[2]=='T'&&n[3]=='T') return 0;
    /* Everything else: treat as pickup / decoration */
    return 1;
}

void renderer_draw_sprites(void)
{
    if (gamestate != GS_LEVEL || !gametic) return;

    vissprite_t* end = vissprite_p;
    for (vissprite_t* vis = vissprites; vis < end; vis++) {

        /* Occlusion: skip if a solid wall blocks line-of-sight */
        if (sprite_blocked_by_wall(vis->gx, vis->gy)) continue;

        /* screen X centre */
        int sx = (vis->x1 + vis->x2) / 2;

        /* screen Y centre */
        int sy = centery - (int)(((long long)vis->texturemid * vis->scale) >> 32);

        /* sprite height in screen pixels */
        int ph_raw = (int)(((long long)(vis->gzt - vis->gz) * vis->scale) >> 32);
        if (ph_raw < 1) ph_raw = 1;

        int cx = doom_to_vec_x(sx);
        int cy = doom_to_vec_y(sy);

        int r = ph_raw / 4;
        if (r < 3)  r = 3;
        if (r > 18) r = 18;

        int cls = classify_sprite(vis->patch);
        if (cls < 0) {
            /* Bullet puff → draw tracer line from gun muzzle to impact point */
            const char* pn = lumpinfo[firstspritelump + vis->patch].name;
            if (pn[0]=='P'&&pn[1]=='U'&&pn[2]=='F'&&pn[3]=='F') {
                /* Clamp impact to screen range */
                int ix = cx < -127 ? -127 : (cx > 127 ? 127 : cx);
                int iy = cy < -127 ? -127 : (cy > 127 ? 127 : cy);
                /* Muzzle tip: weapon is centered at x=0, barrel tip near bottom of 3D view */
                int mx = 0, my = -100;
                /* Tracer line from muzzle to impact */
                draw_vec(mx, my, ix, iy);
                /* Star burst at impact point */
                draw_vec(ix - 4, iy,     ix + 4, iy    );
                draw_vec(ix,     iy - 4, ix,     iy + 4);
                draw_vec(ix - 3, iy - 3, ix + 3, iy + 3);
                draw_vec(ix + 3, iy - 3, ix - 3, iy + 3);
            }
            continue;
        }
        if (cls == 2) {
            /* Decoration: use .vec assets for all types */
            const char* dn = lumpinfo[firstspritelump + vis->patch].name;
            int is_barrel_exp = (dn[0]=='B'&&dn[1]=='E'&&dn[2]=='X'&&dn[3]=='P');
            int is_barrel     = (dn[0]=='B'&&dn[1]=='A'&&dn[2]=='R');
            int is_corpse     = (dn[0]=='D'&&dn[1]=='E'&&dn[2]=='A'&&dn[3]=='D') ||
                                (dn[0]=='G'&&dn[1]=='I'&&dn[2]=='B'&&dn[3]=='S');
            if (is_corpse)
                draw_sprite_vec(corpse_segs, corpse_seg_count, cx, cy, r);
            else if (is_barrel_exp)
                draw_sprite_vec(barrel_explosion_segs, barrel_explosion_seg_count, cx, cy, r);
            else if (is_barrel)
                draw_sprite_vec(pickup_barrel_segs, pickup_barrel_seg_count, cx, cy, r);
            else
                draw_sprite_vec(decoration_pillar_segs, decoration_pillar_seg_count, cx, cy, r);
        } else if (cls == 0) {
            int et = get_enemy_type(vis->patch);
            if (et == ET_OTHER) {
                static char seen[32][9]; static int nseen = 0;
                const char* nm = lumpinfo[firstspritelump + vis->patch].name;
                int found = 0;
                for (int k = 0; k < nseen; k++) if (doom_strncmp(seen[k], nm, 8) == 0) { found = 1; break; }
                if (!found && nseen < 32) {
                    doom_strncpy(seen[nseen++], nm, 8);
                    RENDERER_LOG("[UNKNOWN ENEMY]  lump=%.8s  ph=%d\n", nm, ph_raw);
                }
            }
            draw_enemy(cx, cy, r, et);
        } else {
            int pt = get_pickup_type(vis->patch);
            if (pt == PT_OTHER) {
                static char seen[32][9]; static int nseen = 0;
                const char* nm = lumpinfo[firstspritelump + vis->patch].name;
                int found = 0;
                for (int k = 0; k < nseen; k++) if (doom_strncmp(seen[k], nm, 8) == 0) { found = 1; break; }
                if (!found && nseen < 32) {
                    doom_strncpy(seen[nseen++], nm, 8);
                    RENDERER_LOG("[UNKNOWN PICKUP] lump=%.8s  ph=%d\n", nm, ph_raw);
                }
            }
            draw_pickup(cx, cy, r, pt);
        }
    }
}

/* ================================================================
 * renderer_draw_weapons — draw player weapon vectors
 *
 * Called from main_sdl.c after doom_update() so the weapon art
 * is drawn on top of the 3D view in the same frame.
 *
 * Weapon geometry lives in weapons_assets.h (generated from .vec).
 * Currently supports: pistol (PISG) + muzzle flash (PISF).
 * Other weapons fall back to a simple generic gun silhouette.
 *
 * Flash detection: psprites[ps_flash].state != NULL.
 * ================================================================ */
void renderer_draw_weapons(void)
{
    if (gamestate != GS_LEVEL || !gametic) return;

    player_t* plyr = &players[consoleplayer];

    /* ---- Choose weapon art ---- */
    const vec_seg_t* w_segs       = NULL;
    int              w_count      = 0;
    const vec_seg_t* w_side_segs  = NULL;
    int              w_side_count = 0;
    const vec_seg_t* w_flash_segs = NULL;
    int              w_flash_count= 0;
    int ox = 0, oy = -45;  /* screen offset — adjust per weapon if needed */

    switch (plyr->readyweapon) {
        case wp_pistol:
            w_segs        = pistol_segs;        w_count       = pistol_seg_count;
            w_side_segs   = pistol_side_segs;   w_side_count  = pistol_side_seg_count;
            w_flash_segs  = pistol_flash_segs;  w_flash_count = pistol_flash_seg_count;
            ox = 0; oy = -45;
            break;
        case wp_shotgun:
        case wp_supershotgun:
            w_segs        = shotgun_segs;        w_count       = shotgun_seg_count;
            w_side_segs   = shotgun_side_segs;   w_side_count  = shotgun_side_seg_count;
            w_flash_segs  = shotgun_flash_segs;  w_flash_count = shotgun_flash_seg_count;
            ox = 0; oy = -25;
            break;
        default:
            break;
    }

    /* Draw weapon — front view during idle/fire, side view while raising/lowering. */
    if (w_segs && plyr->health > 0) {
        int lowering = (plyr->pendingweapon != wp_nochange);
        int recoil_y = plyr->psprites[ps_flash].state ? 8 : 0;
        if (lowering && w_side_segs) {
            draw_vec_shape_scaled(w_side_segs, w_side_count, ox, oy + recoil_y, 3, 5);
        } else {
            draw_vec_shape_scaled(w_segs, w_count, ox, oy + recoil_y, 3, 5);
        }
    }

    /* ---- Muzzle flash (ps_flash layer active) ---- */
    if (plyr->psprites[ps_flash].state && w_flash_segs) {
        draw_vec_shape_scaled(w_flash_segs, w_flash_count, -4, -20 + 8, 3, 5);
    }
}

/* ================================================================
 * M_Drawer — override
 *
 * Replaces DOOM's patch-based menu with stroke-font text.
 * Lump names (M_NGAME, M_OPTION…) are mapped to readable strings.
 * TXT_* items strip the prefix as fallback text.
 * Load/save slots use savegamestrings[] directly.
 * The skull cursor is replaced with a ">" marker.
 * ================================================================ */

/* Lump names that are not functional in vector mode — hidden from menu */
static int m_lump_hidden(const char* name)
{
    static const char* const skip[] = {
        "M_MSENS",   /* Mouse Sensitivity — not relevant on Vectrex               */
        "TXT_MMOV",  /* Mouse Move toggle  — not relevant on Vectrex              */
        "TXT_MOPT",  /* Mouse Options      — not relevant on Vectrex              */
        0
    };
    for (int i = 0; skip[i]; i++)
        if (doom_strcmp(skip[i], name) == 0) return 1;
    return 0;
}

static const char* m_lump_text(const char* name)
{
    static const struct { const char* lump; const char* text; } tbl[] = {
        {"M_NGAME",  "NEW GAME"},
        {"M_OPTION", "OPTIONS"},
        {"M_LOADG",  "LOAD GAME"},
        {"M_SAVEG",  "SAVE GAME"},
        {"M_RDTHIS", "ABOUT"},
        {"M_QUITG",  "QUIT GAME"},
        {"M_EPI1",   "EPISODE 1"},
        {"M_EPI2",   "EPISODE 2"},
        {"M_EPI3",   "EPISODE 3"},
        {"M_EPI4",   "EPISODE 4"},
        {"M_JKILL",  "TOO YOUNG TO DIE"},
        {"M_ROUGH",  "NOT TOO ROUGH"},
        {"M_HURT",   "HURT ME PLENTY"},
        {"M_ULTRA",  "ULTRA-VIOLENCE"},
        {"M_NMARE",  "NIGHTMARE!"},
        {"M_ENDGAM", "END GAME"},
        {"M_MESSG",  "MESSAGES"},
        {"M_SCRNSZ", "SCREEN SIZE"},
        {"M_SVOL",   "SOUND VOLUME"},
        {"M_SFXVOL", "SFX VOLUME"},
        {"M_MUSVOL", "MUSIC VOLUME"},
        {"M_MSENS",  "MOUSE SENS"},
        {"TXT_CROS", "CROSSHAIR"},
        {"TXT_ARUN", "ALWAYS RUN"},
        {"TXT_MOPT", "MOUSE OPTIONS"},
        {"TXT_MMOV", "MOUSE MOVE"},
        {0, 0}
    };
    for (int i = 0; tbl[i].lump; i++)
        if (doom_strcmp(tbl[i].lump, name) == 0)
            return tbl[i].text;
    /* TXT_ fallback: show text after prefix */
    if (doom_strncmp(name, "TXT_", 4) == 0) return name + 4;
    return name;
}

void M_Drawer(void)
{
    extern doom_boolean inhelpscreens;
    extern int          messageToPrint;
    extern char*        messageString;
    extern menu_t*      currentMenu;
    extern short        itemOn;
    extern char         savegamestrings[10][SAVESTRINGSIZE];

    /* Call the current menu's sub-drawer (e.g. M_DrawReadThis1) so that
     * DOOM's internal state (inhelpscreens, etc.) gets updated correctly.
     * V_DrawPatch will ignore the large HELP/CREDIT patches automatically. */
    if (menuactive && currentMenu && currentMenu->routine)
        currentMenu->routine();

    /* ---- ABOUT screen (triggered by M_RDTHIS / "ABOUT" menu item) ---- */
    if (inhelpscreens) {
        inhelpscreens = 0;
        s_about_active = 1; /* suppress walls + demo behind credits */
        /* Credits — all lines centered */
        draw_centered_small("DOOM",                90,  6);
        draw_vec(-70, 80, 70, 80);
        draw_centered_small("ORIGINAL GAME BY",    70, 10);
        draw_centered_small("ID SOFTWARE",         58, 10);
        draw_centered_small("JOHN CARMACK AND",    46, 10);
        draw_centered_small("JOHN ROMERO",         34, 10);
        draw_vec(-70, 25, 70, 25);
        draw_centered_small("BASED ON PUREDOOM",   15, 10);
        draw_centered_small("BY BRAINDIGITALIS",    3, 10);
        draw_vec(-70, -6, 70, -6);
        draw_centered_small("VECTREX PORT BY",    -16, 10);
        draw_centered_small("DANIEL FERRER",      -28, 10);
        draw_vec(-70,-37, 70,-37);
        draw_centered_small("DEDICATED TO",       -47, 10);
        draw_centered_small("VECTREX FANS UNITE!",-59, 10);
        draw_vec(-70,-68, 70,-68);
        draw_centered_small("PRESS ESC TO RETURN",-80, 11);
        return;
    }
    /* No longer in about screen */
    s_about_active = 0;
    inhelpscreens = 0;

    /* ---- Confirmation / quit messages (yes/no dialogs) ---- */
    if (messageToPrint) {
        const char* s = messageString;
        int nlines = 1;
        for (const char* p = s; *p; p++) if (*p == '\n') nlines++;
        int vy = (nlines - 1) * 7; /* top line, descending 14vx per line */
        while (*s) {
            const char* nl = s;
            while (*nl && *nl != '\n') nl++;
            int len = (int)(nl - s);
            if (len > 78) len = 78;
            char buf[80];
            doom_strncpy(buf, s, len);
            buf[len] = '\0';
            int vx = -(len * 8) / 2; /* center: ~8vx per char at div=12 */
            draw_string_small(buf, vx, vy, 12);
            vy -= 14;
            if (*nl) s = nl + 1; else break;
        }
        return;
    }

    if (!menuactive) return;

    /* ---- Menu items ---- */
    short mx     = currentMenu->x;
    short my     = currentMenu->y;
    short nitems = currentMenu->numitems;
    int   cur_y  = my;

    for (int i = 0; i < nitems; i++) {
        menuitem_t* item = currentMenu->menuitems + i;
        if (item->status < 0) { cur_y += LINEHEIGHT; continue; } /* separator */

        /* skip items that are not useful in vector mode */
        if (item->name[0] && m_lump_hidden(item->name)) { cur_y += LINEHEIGHT; continue; }

        const char* text = 0;
        if (item->name[0] == '\0') {
            if (i < 10) text = savegamestrings[i]; /* load/save slot */
        } else {
            text = m_lump_text(item->name);
        }

        if (text && text[0]) {
            int vx = doom_to_vec_x(mx);
            int vy = doom_to_vec_y(cur_y + 8); /* center within 16px LINEHEIGHT */
            draw_string_small(text, vx, vy, 12);
        }
        cur_y += LINEHEIGHT;
    }

    /* ---- Cursor ">" left of selected item — skip hidden items ---- */
    {
        /* If cursor is on a hidden item, don't draw it */
        menuitem_t* sel = currentMenu->menuitems + itemOn;
        if (!sel->name[0] || !m_lump_hidden(sel->name)) {
            int cy = my + itemOn * LINEHEIGHT + 8;
            int vx = doom_to_vec_x(mx) - 14;
            int vy = doom_to_vec_y(cy);
            draw_string_small(">", vx, vy, 12);
        }
    }
}

/* ================================================================
 * Stroke vector font
 *
 * Data adapted from piTrex SDK vectorFont.i.
 * Each glyph is an array of triplets {pattern, y_delta, x_delta}:
 *   (signed char)0xFF = pen-down draw to new position
 *   (signed char)0x00 = pen-up move to new position
 *              0x01   = end of character (terminates loop)
 *
 * Deltas are pre-multiplied by FONT_BU (= BLOW_UP = 15) in the
 * original source; the values are embedded here as literals.
 *
 * Rendering scale: divide stored deltas by FONT_DIV to get Vectrex
 * coordinate units.  FONT_DIV = 12 → character height ≈ 10 vx units.
 * ================================================================ */

#define FONT_BU  15   /* BLOW_UP factor baked into glyph deltas */
#define FONT_DIV 12   /* divisor: stored delta → Vectrex units  */

/* ---- glyph arrays (named to match ABC_N in vectorFont.i) ---- */

static const signed char s_g0[]= /* A */
{(signed char)0xFF,+4*FONT_BU,0,  (signed char)0xFF,+2*FONT_BU,0,
 (signed char)0xFF,+2*FONT_BU,+2*FONT_BU,
 (signed char)0xFF,-2*FONT_BU,+2*FONT_BU,  (signed char)0xFF,-2*FONT_BU,0,
 (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0x00,0,+4*FONT_BU,  (signed char)0xFF,-4*FONT_BU,0,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g1[]= /* B */
{(signed char)0xFF,+8*FONT_BU,0,  (signed char)0xFF,0,+2*FONT_BU,
 (signed char)0xFF,-1*FONT_BU,+2*FONT_BU,  (signed char)0xFF,-1*FONT_BU,0,
 (signed char)0xFF,-1*FONT_BU,-1*FONT_BU,
 (signed char)0x00,0,-3*FONT_BU,
 (signed char)0xFF,0,+3*FONT_BU,  (signed char)0xFF,-2*FONT_BU,+1*FONT_BU,
 (signed char)0xFF,-1*FONT_BU,0,  (signed char)0xFF,-2*FONT_BU,-2*FONT_BU,
 (signed char)0xFF,0,-2*FONT_BU,
 (signed char)0x00,0,+6*FONT_BU,  (signed char)0x01};

static const signed char s_g2[]= /* C */
{(signed char)0x00,+8*FONT_BU,+4*FONT_BU,
 (signed char)0xFF,0,-4*FONT_BU,  (signed char)0xFF,-8*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g3[]= /* D */
{(signed char)0xFF,+8*FONT_BU,0,  (signed char)0xFF,0,+2*FONT_BU,
 (signed char)0xFF,-3*FONT_BU,+2*FONT_BU,  (signed char)0xFF,-2*FONT_BU,0,
 (signed char)0xFF,-3*FONT_BU,-2*FONT_BU,  (signed char)0xFF,0,-2*FONT_BU,
 (signed char)0x00,0,+6*FONT_BU,  (signed char)0x01};

static const signed char s_g4[]= /* E */
{(signed char)0x00,+8*FONT_BU,+4*FONT_BU,
 (signed char)0xFF,0,-4*FONT_BU,  (signed char)0xFF,-3*FONT_BU,0,
 (signed char)0x00,0,+4*FONT_BU,
 (signed char)0xFF,0,-4*FONT_BU,  (signed char)0xFF,-5*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g5[]= /* F */
{(signed char)0xFF,+5*FONT_BU,0,  (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,0,-4*FONT_BU,
 (signed char)0xFF,+3*FONT_BU,0,  (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,-8*FONT_BU,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g6[]= /* G */
{(signed char)0x00,+7*FONT_BU,+4*FONT_BU,
 (signed char)0xFF,+1*FONT_BU,0,  (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0xFF,-8*FONT_BU,0,  (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0xFF,+5*FONT_BU,0,  (signed char)0xFF,0,-2*FONT_BU,
 (signed char)0x00,-5*FONT_BU,+4*FONT_BU,  (signed char)0x01};

static const signed char s_g7[]= /* H */
{(signed char)0xFF,+8*FONT_BU,0,
 (signed char)0x00,-3*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,+3*FONT_BU,0,
 (signed char)0xFF,-8*FONT_BU,0,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g8[]= /* I */
{(signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,0,-2*FONT_BU,
 (signed char)0xFF,+8*FONT_BU,0,
 (signed char)0x00,0,-2*FONT_BU,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,-8*FONT_BU,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g9[]= /* J */
{(signed char)0x00,+2*FONT_BU,0,
 (signed char)0xFF,-2*FONT_BU,+1*FONT_BU,  (signed char)0xFF,0,+3*FONT_BU,
 (signed char)0xFF,+8*FONT_BU,0,  (signed char)0xFF,0,-2*FONT_BU,
 (signed char)0x00,-8*FONT_BU,+4*FONT_BU,  (signed char)0x01};

static const signed char s_g10[]= /* K */
{(signed char)0xFF,+8*FONT_BU,0,
 (signed char)0x00,-3*FONT_BU,0,
 (signed char)0xFF,+3*FONT_BU,+4*FONT_BU,
 (signed char)0x00,-3*FONT_BU,-4*FONT_BU,
 (signed char)0xFF,-5*FONT_BU,+4*FONT_BU,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g11[]= /* L */
{(signed char)0x00,+8*FONT_BU,0,
 (signed char)0xFF,-8*FONT_BU,0,  (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g12[]= /* M */
{(signed char)0xFF,+8*FONT_BU,0,  (signed char)0xFF,-3*FONT_BU,+2*FONT_BU,
 (signed char)0xFF,+3*FONT_BU,+2*FONT_BU,  (signed char)0xFF,-8*FONT_BU,0,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g13[]= /* N */
{(signed char)0xFF,+8*FONT_BU,0,  (signed char)0xFF,-8*FONT_BU,+4*FONT_BU,
 (signed char)0xFF,+8*FONT_BU,0,
 (signed char)0x00,-8*FONT_BU,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g14[]= /* O */
{(signed char)0x00,0,0,
 (signed char)0xFF,+8*FONT_BU,0,  (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0xFF,-8*FONT_BU,0,  (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0x00,0,+6*FONT_BU,  (signed char)0x01};

static const signed char s_g15[]= /* P */
{(signed char)0xFF,+8*FONT_BU,0,  (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0xFF,-3*FONT_BU,0,  (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0x00,-5*FONT_BU,+6*FONT_BU,  (signed char)0x01};

static const signed char s_g16[]= /* Q */
{(signed char)0x00,0,+2*FONT_BU,
 (signed char)0xFF,0,-2*FONT_BU,
 (signed char)0xFF,+8*FONT_BU,0,  (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0xFF,-6*FONT_BU,0,  (signed char)0xFF,-2*FONT_BU,-2*FONT_BU,
 (signed char)0x00,+2*FONT_BU,0,
 (signed char)0xFF,-2*FONT_BU,+2*FONT_BU,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g17[]= /* R */
{(signed char)0xFF,+8*FONT_BU,0,  (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0xFF,-3*FONT_BU,0,  (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0xFF,-5*FONT_BU,+4*FONT_BU,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g18[]= /* S */
{(signed char)0x00,+8*FONT_BU,+4*FONT_BU,
 (signed char)0xFF,0,-4*FONT_BU,  (signed char)0xFF,-3*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,  (signed char)0xFF,-5*FONT_BU,0,
 (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0x00,0,+6*FONT_BU,  (signed char)0x01};

static const signed char s_g19[]= /* T */
{(signed char)0x00,0,+2*FONT_BU,
 (signed char)0xFF,+8*FONT_BU,0,
 (signed char)0x00,0,-2*FONT_BU,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,-8*FONT_BU,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g20[]= /* U */
{(signed char)0x00,+8*FONT_BU,0,
 (signed char)0xFF,-8*FONT_BU,0,  (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0xFF,+8*FONT_BU,0,
 (signed char)0x00,-8*FONT_BU,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g21[]= /* V */
{(signed char)0x00,+8*FONT_BU,0,
 (signed char)0xFF,-8*FONT_BU,+2*FONT_BU,  (signed char)0xFF,+8*FONT_BU,+2*FONT_BU,
 (signed char)0x00,-8*FONT_BU,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g22[]= /* W */
{(signed char)0x00,+8*FONT_BU,0,
 (signed char)0xFF,-8*FONT_BU,0,  (signed char)0xFF,+3*FONT_BU,+2*FONT_BU,
 (signed char)0xFF,-3*FONT_BU,+2*FONT_BU,  (signed char)0xFF,+8*FONT_BU,0,
 (signed char)0x00,-8*FONT_BU,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g23[]= /* X */
{(signed char)0xFF,+8*FONT_BU,+4*FONT_BU,
 (signed char)0x00,0,-4*FONT_BU,
 (signed char)0xFF,-8*FONT_BU,+4*FONT_BU,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g24[]= /* Y */
{(signed char)0x00,+8*FONT_BU,0,
 (signed char)0xFF,-3*FONT_BU,+2*FONT_BU,  (signed char)0xFF,+3*FONT_BU,+2*FONT_BU,
 (signed char)0x00,-3*FONT_BU,-2*FONT_BU,
 (signed char)0xFF,-5*FONT_BU,0,
 (signed char)0x00,0,+4*FONT_BU,  (signed char)0x01};

static const signed char s_g25[]= /* Z */
{(signed char)0x00,+8*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,  (signed char)0xFF,-8*FONT_BU,-4*FONT_BU,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g26[]= /* . (period) */
{(signed char)0xFF,0,+2*FONT_BU,  (signed char)0xFF,+2*FONT_BU,0,
 (signed char)0xFF,0,-2*FONT_BU,  (signed char)0xFF,-2*FONT_BU,0,
 (signed char)0x00,0,+6*FONT_BU,  (signed char)0x01};

static const signed char s_g27[]= /* ' ' (space / unsupported chars) */
{(signed char)0x00,0,+6*FONT_BU,  (signed char)0x01};

static const signed char s_g28[]= /* ! */
{(signed char)0xFF,0,+2*FONT_BU,  (signed char)0xFF,+2*FONT_BU,0,
 (signed char)0xFF,0,-2*FONT_BU,  (signed char)0xFF,-2*FONT_BU,0,
 (signed char)0x00,+3*FONT_BU,+1*FONT_BU,
 (signed char)0xFF,+5*FONT_BU,0,
 (signed char)0x00,-8*FONT_BU,+4*FONT_BU,  (signed char)0x01};

/* --- digits --- */

static const signed char s_g29[]= /* 1 */
{(signed char)0x00,+8*FONT_BU,+3*FONT_BU,
 (signed char)0xFF,-8*FONT_BU,0,
 (signed char)0x00,0,+3*FONT_BU,  (signed char)0x01};

static const signed char s_g30[]= /* 2 */
{(signed char)0x00,+8*FONT_BU,+4*FONT_BU,
 (signed char)0xFF,0,-4*FONT_BU,  (signed char)0xFF,-3*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,  (signed char)0xFF,-5*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g31[]= /* 3 */
{(signed char)0x00,+8*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,  (signed char)0xFF,-3*FONT_BU,0,
 (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0x00,0,+4*FONT_BU,
 (signed char)0xFF,-5*FONT_BU,0,  (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0x00,0,+6*FONT_BU,  (signed char)0x01};

static const signed char s_g32[]= /* 4 */
{(signed char)0x00,0,+4*FONT_BU,
 (signed char)0xFF,+8*FONT_BU,0,
 (signed char)0x00,0,-4*FONT_BU,
 (signed char)0xFF,-3*FONT_BU,0,  (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,-5*FONT_BU,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g33[]= /* 5 */
{(signed char)0xFF,0,+4*FONT_BU,  (signed char)0xFF,+5*FONT_BU,0,
 (signed char)0xFF,0,-4*FONT_BU,  (signed char)0xFF,+3*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,-8*FONT_BU,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g34[]= /* 6 */
{(signed char)0xFF,+8*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,-3*FONT_BU,-4*FONT_BU,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0xFF,-5*FONT_BU,0,  (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0x00,0,+6*FONT_BU,  (signed char)0x01};

static const signed char s_g35[]= /* 7 */
{(signed char)0x00,0,+4*FONT_BU,
 (signed char)0xFF,+8*FONT_BU,0,
 (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0x00,-8*FONT_BU,+6*FONT_BU,  (signed char)0x01};

static const signed char s_g36[]= /* 8 */
{(signed char)0xFF,+8*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0xFF,-8*FONT_BU,0,
 (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0x00,+5*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0x00,-5*FONT_BU,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g37[]= /* 9 */
{(signed char)0x00,+5*FONT_BU,+4*FONT_BU,
 (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0xFF,+3*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0xFF,-8*FONT_BU,0,
 (signed char)0x00,0,+2*FONT_BU,  (signed char)0x01};

static const signed char s_g38[]= /* 0 */
{(signed char)0xFF,+8*FONT_BU,0,  (signed char)0xFF,0,+4*FONT_BU,
 (signed char)0xFF,-8*FONT_BU,0,  (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0x00,0,+6*FONT_BU,  (signed char)0x01};

static const signed char s_g39[]= /* < */
{(signed char)0x00,+2*FONT_BU,+1*FONT_BU,
 (signed char)0xFF,+2*FONT_BU,-1*FONT_BU,  (signed char)0xFF,+2*FONT_BU,+1*FONT_BU,
 (signed char)0x00,-2*FONT_BU,+3*FONT_BU,
 (signed char)0xFF,0,-4*FONT_BU,
 (signed char)0x00,-4*FONT_BU,+6*FONT_BU,  (signed char)0x01};

static const signed char s_g40[]= /* > */
{(signed char)0x00,+4*FONT_BU,0,
 (signed char)0xFF,0,+4*FONT_BU,  (signed char)0xFF,+2*FONT_BU,-1*FONT_BU,
 (signed char)0x00,-4*FONT_BU,0,
 (signed char)0xFF,+2*FONT_BU,+1*FONT_BU,
 (signed char)0x00,-4*FONT_BU,+2*FONT_BU,  (signed char)0x01};

/* ---- lookup table: indexed by (ascii_code - 0x20) ------------ */
/* Covers 0x20 (' ') through 0x5A ('Z') = 59 entries             */

#define FONT_CHAR_COUNT 59

static const signed char* const s_font_abc[FONT_CHAR_COUNT] = {
    s_g27, /* 0x20   */  s_g28, /* !      */
    s_g27, /* "      */  s_g27, /* #      */
    s_g27, /* $      */  s_g27, /* %      */
    s_g27, /* &      */  s_g27, /* '      */
    s_g27, /* (      */  s_g27, /* )      */
    s_g27, /* *      */  s_g27, /* +      */
    s_g27, /* ,      */  s_g27, /* -      */
    s_g26, /* .      */  s_g27, /* /      */
    s_g38, /* 0      */  s_g29, /* 1      */
    s_g30, /* 2      */  s_g31, /* 3      */
    s_g32, /* 4      */  s_g33, /* 5      */
    s_g34, /* 6      */  s_g35, /* 7      */
    s_g36, /* 8      */  s_g37, /* 9      */
    s_g27, /* :      */  s_g27, /* ;      */
    s_g39, /* <      */  s_g27, /* =      */
    s_g40, /* >      */  s_g27, /* ?      */
    s_g27, /* @      */
    s_g0,  /* A      */  s_g1,  /* B      */
    s_g2,  /* C      */  s_g3,  /* D      */
    s_g4,  /* E      */  s_g5,  /* F      */
    s_g6,  /* G      */  s_g7,  /* H      */
    s_g8,  /* I      */  s_g9,  /* J      */
    s_g10, /* K      */  s_g11, /* L      */
    s_g12, /* M      */  s_g13, /* N      */
    s_g14, /* O      */  s_g15, /* P      */
    s_g16, /* Q      */  s_g17, /* R      */
    s_g18, /* S      */  s_g19, /* T      */
    s_g20, /* U      */  s_g21, /* V      */
    s_g22, /* W      */  s_g23, /* X      */
    s_g24, /* Y      */  s_g25, /* Z      */
};

/* ================================================================
 * draw_char_stroke
 *
 * Render one character at Vectrex position (vx, vy).
 * vy is the BOTTOM of the character; the font draws upward.
 * Returns the new vx after the character advance step.
 * ================================================================ */
static int draw_char_stroke(unsigned char c, int vx, int vy)
{
    const signed char* p;
    int cur_x = vx;
    int cur_y = vy;
    int idx;

    /* fold lowercase to uppercase */
    if (c >= 'a' && c <= 'z') c = (unsigned char)(c - 32);

    idx = (int)c - 0x20;
    if (idx < 0 || idx >= FONT_CHAR_COUNT)
    {
        /* unknown glyph: advance by one space */
        return vx + 6 * FONT_BU / FONT_DIV;
    }

    p = s_font_abc[idx];

    /* Loop while pattern byte ≤ 0  (0xFF=-1 draw, 0x00=0 move)
     * Stops when pattern byte > 0  (0x01 = end marker)          */
    while ((signed char)p[0] <= 0)
    {
        signed char pat = p[0];
        int dy  = (int)(signed char)p[1];
        int dx  = (int)(signed char)p[2];
        int nx  = cur_x + dx / FONT_DIV;
        int ny  = cur_y + dy / FONT_DIV;

        if (pat != 0) /* 0xFF = draw */
            draw_vec(cur_x, cur_y, nx, ny);

        cur_x = nx;
        cur_y = ny;
        p += 3;
    }

    return cur_x; /* x position for next character */
}

/* ================================================================
 * draw_string_vec
 *
 * Render a NUL-terminated string at Vectrex position (vx, vy).
 * vy is the BOTTOM of the characters; the font draws upward.
 * Returns the vx after the last character (useful for chaining).
 * ================================================================ */
static int draw_string_vec(const char* str, int vx, int vy)
{
    for (; *str; str++)
        vx = draw_char_stroke((unsigned char)*str, vx, vy);
    return vx;
}

/* ================================================================
 * draw_string_small — scaled-down stroke text
 *
 * Like draw_string_vec but with a custom divisor instead of FONT_DIV.
 * Larger div = smaller glyphs:
 *   div=24  → char ≈3 vx wide, 5 vx tall  (HUD bottom labels)
 *   div=30  → char ≈3 vx wide, 4 vx tall  (right ammo panel rows)
 * ================================================================ */
/* Return pixel width of str at given div, without drawing */
static int string_width_small(const char* str, int div)
{
    int vx = 0;
    for (; *str; str++) {
        unsigned char c = (unsigned char)*str;
        if (c >= 'a' && c <= 'z') c = (unsigned char)(c - 32);
        int idx = (int)c - 0x20;
        if (idx < 0 || idx >= FONT_CHAR_COUNT) { vx += 6 * FONT_BU / div; continue; }
        const signed char* p = s_font_abc[idx];
        while ((signed char)p[0] <= 0) {
            vx += (int)(signed char)p[2] / div;
            p += 3;
        }
    }
    return vx;
}

/* Draw str centered horizontally at vy */
static void draw_centered_small(const char* str, int vy, int div)
{
    int w = string_width_small(str, div);
    draw_string_small(str, -w / 2, vy, div);
}

static int draw_string_small(const char* str, int vx, int vy, int div)
{
    for (; *str; str++) {
        unsigned char c = (unsigned char)*str;
        if (c >= 'a' && c <= 'z') c = (unsigned char)(c - 32);
        int idx = (int)c - 0x20;
        if (idx < 0 || idx >= FONT_CHAR_COUNT) {
            vx += 6 * FONT_BU / div;
            continue;
        }
        const signed char* p = s_font_abc[idx];
        int cur_x = vx, cur_y = vy;
        while ((signed char)p[0] <= 0) {
            signed char pat = p[0];
            int dy = (int)(signed char)p[1];
            int dx = (int)(signed char)p[2];
            int nx = cur_x + dx / div;
            int ny = cur_y + dy / div;
            if (pat != 0)
                draw_vec(cur_x, cur_y, nx, ny);
            cur_x = nx;
            cur_y = ny;
            p += 3;
        }
        vx = cur_x;
    }
    return vx;
}

/* Convert integer 0..999 to null-terminated decimal string */
static void itoa3(int n, char* buf)
{
    if (n < 0)   n = 0;
    if (n > 999) n = 999;
    if (n == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    char tmp[4]; int i = 0, j = 0;
    while (n) { tmp[i++] = (char)('0' + n % 10); n /= 10; }
    while (i > 0) buf[j++] = tmp[--i];
    buf[j] = '\0';
}

/* ================================================================
 * HUlib_drawTextLine — override
 *
 * Intercepts all HU text-line rendering (messages, level title,
 * chat) and replaces patch-based bitmaps with the stroke font.
 *
 * l->y is the top of the text in DOOM screen coords (Y-down, 0-199).
 * We start the stroke font at vy = doom_to_vec_y(l->y + 14) so the
 * character (which draws upward ~10 vx units) sits within the area
 * DOOM allocated for it.
 * ================================================================ */
void HUlib_drawTextLine(hu_textline_t* l, doom_boolean drawcursor)
{
    char buf[HU_MAXLINELENGTH + 1];
    int i;

    (void)drawcursor; /* cursor not rendered in wireframe mode */

    for (i = 0; i < l->len && i < HU_MAXLINELENGTH; i++)
        buf[i] = l->l[i];
    buf[i] = '\0';

    draw_string_vec(buf,
                    doom_to_vec_x(l->x),
                    doom_to_vec_y(l->y + 14));
}

/* ================================================================
 * STlib_drawNum — override
 *
 * Replaces the patch-based status-bar number renderer with the
 * stroke font.  Called for health, ammo, armor, and frag counts.
 *
 * n->x        right edge of the number field (DOOM screen X)
 * n->y        top of the number field (DOOM screen Y)
 * *n->num     the value to display
 * n->width    number of digit slots (2 or 3)
 * n->p[0]     digit-0 patch — used only to read pixel width for layout
 * ================================================================ */
void STlib_drawNum(st_number_t* n, doom_boolean refresh)
{
    /* sentinel: DOOM uses 1994 to mean "don't draw" */
    char  buf[16];
    char* p;
    int   num       = *n->num;
    int   numdigits = n->width;
    int   digit_w   = SHORT(n->p[0]->width);   /* original pixel width / digit */
    int   neg;
    int   vx, vy;
    /* tiny manual int→string (no libc dependency for baremetal) */
    char  tmp[12];
    char* t;
    int   absnum;

    (void)refresh;

    if (num == 1994) return;

    neg    = (num < 0);
    absnum = neg ? -num : num;

    /* clamp like the original */
    if (neg)
    {
        if (numdigits == 2 && absnum > 9)  absnum = 9;
        if (numdigits == 3 && absnum > 99) absnum = 99;
    }

    /* build string right-to-left into tmp */
    t = tmp + sizeof(tmp) - 1;
    *t = '\0';
    if (absnum == 0)
    {
        *--t = '0';
    }
    else
    {
        int v = absnum;
        while (v)
        {
            *--t = (char)('0' + v % 10);
            v /= 10;
        }
    }
    if (neg) *--t = '-';

    p = buf;
    while (*t) *p++ = *t++;
    *p = '\0';

    /* Right-align: DOOM's right edge is n->x; each digit is digit_w px wide.
     * Start our string at the DOOM pixel position of the leftmost digit.  */
    vx = doom_to_vec_x(n->x - numdigits * digit_w);
    vy = doom_to_vec_y(n->y + digit_w);  /* digit_w ≈ patch height too */

    draw_string_vec(buf, vx, vy);
}
