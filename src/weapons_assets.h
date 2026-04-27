/* Auto-generated from pistol.vec, pistol_side.vec, pistol_flash.vec, shotgun.vec, shotgun_side.vec, shotgun_flash.vec
 * by tools/vec_to_c.py — 2026-04-18 — DO NOT EDIT */
#ifndef WEAPONS_ASSETS_H
#define WEAPONS_ASSETS_H

#include <stdint.h>
#ifndef VEC_SEG_T_DEFINED
#define VEC_SEG_T_DEFINED
typedef struct { int16_t x0, y0, x1, y1; } vec_seg_t;
#endif

#ifndef VEC3D_TYPES_DEFINED
#define VEC3D_TYPES_DEFINED
typedef struct { int8_t x, y, z; } vec3d_vert_t;
typedef struct { int pt_count; int closed; const uint8_t* idx; } vec3d_path_t;
#endif

/* --- pistol.vec --- */
static const vec_seg_t pistol_segs[] = {
    /* barrel_tube */
    {   -6,  -55,    6,  -55 },
    {    6,  -55,    6,  -95 },
    {    6,  -95,   -6,  -95 },
    {   -6,  -95,   -6,  -55 },
    /* front_sight */
    {    0,  -95,    0, -100 },
    /* slide_body */
    {  -13,  -55,   13,  -55 },
    {   13,  -55,   13,  -88 },
    {   13,  -88,  -13,  -88 },
    {  -13,  -88,  -13,  -55 },
    /* ejection_port */
    {    7,  -60,   12,  -60 },
    {   12,  -60,   12,  -82 },
    {   12,  -82,    7,  -82 },
    {    7,  -82,    7,  -60 },
    /* rear_sight */
    {   -4,  -55,    4,  -55 },
    /* frame_body */
    {  -15,  -38,   15,  -38 },
    {   15,  -38,   15,  -57 },
    {   15,  -57,  -15,  -57 },
    {  -15,  -57,  -15,  -38 },
    /* trigger_guard */
    {   -9,  -38,   -9,  -28 },
    {   -9,  -28,    9,  -28 },
    {    9,  -28,    9,  -38 },
    /* grip_outline */
    {  -12,  -10,   12,  -10 },
    {   12,  -10,   10,  -30 },
    {   10,  -30,  -10,  -30 },
    {  -10,  -30,  -12,  -10 },
    /* grip_line1 */
    {  -11,  -16,   11,  -16 },
    /* grip_line2 */
    {  -11,  -23,   11,  -23 },
};
static const int pistol_seg_count =
    (int)(sizeof(pistol_segs) / sizeof(pistol_segs[0]));

/* --- pistol.vec 3D table ---
 * 31 unique verts, 10 paths, 32 total point refs */
static const vec3d_vert_t pistol_verts[] = {
    {   -6,  -55,    0 },
    {    6,  -55,    0 },
    {    6,  -63,    0 },
    {   -6,  -63,    0 },
    {    0,  -63,    0 },
    {  -13,  -55,    0 },
    {   13,  -55,    0 },
    {   13,  -63,    0 },
    {  -13,  -63,    0 },
    {    7,  -60,    0 },
    {   12,  -60,    0 },
    {   12,  -63,    0 },
    {    7,  -63,    0 },
    {   -4,  -55,    0 },
    {    4,  -55,    0 },
    {  -15,  -38,    0 },
    {   15,  -38,    0 },
    {   15,  -57,    0 },
    {  -15,  -57,    0 },
    {   -9,  -38,    0 },
    {   -9,  -28,    0 },
    {    9,  -28,    0 },
    {    9,  -38,    0 },
    {  -12,  -10,    0 },
    {   12,  -10,    0 },
    {   10,  -30,    0 },
    {  -10,  -30,    0 },
    {  -11,  -16,    0 },
    {   11,  -16,    0 },
    {  -11,  -23,    0 },
    {   11,  -23,    0 },
};
static const int pistol_vert_count = 31;

static const uint8_t pistol_path0_idx[] = { 0, 1, 2, 3 };
static const uint8_t pistol_path1_idx[] = { 4, 4 };
static const uint8_t pistol_path2_idx[] = { 5, 6, 7, 8 };
static const uint8_t pistol_path3_idx[] = { 9, 10, 11, 12 };
static const uint8_t pistol_path4_idx[] = { 13, 14 };
static const uint8_t pistol_path5_idx[] = { 15, 16, 17, 18 };
static const uint8_t pistol_path6_idx[] = { 19, 20, 21, 22 };
static const uint8_t pistol_path7_idx[] = { 23, 24, 25, 26 };
static const uint8_t pistol_path8_idx[] = { 27, 28 };
static const uint8_t pistol_path9_idx[] = { 29, 30 };
static const vec3d_path_t pistol_paths[] = {
    { 4, 1, pistol_path0_idx },
    { 2, 0, pistol_path1_idx },
    { 4, 1, pistol_path2_idx },
    { 4, 1, pistol_path3_idx },
    { 2, 0, pistol_path4_idx },
    { 4, 1, pistol_path5_idx },
    { 4, 0, pistol_path6_idx },
    { 4, 1, pistol_path7_idx },
    { 2, 0, pistol_path8_idx },
    { 2, 0, pistol_path9_idx },
};
static const int pistol_path_count = 10;

/* --- pistol_side.vec --- */
static const vec_seg_t pistol_side_segs[] = {
    /* barrel */
    {  -60,  -70,   -5,  -70 },
    {   -5,  -70,   -5,  -77 },
    {   -5,  -77,  -60,  -77 },
    {  -60,  -77,  -60,  -70 },
    /* front_sight */
    {  -60,  -77,  -60,  -81 },
    /* slide_top */
    {  -10,  -65,   55,  -65 },
    /* slide_outline */
    {  -10,  -65,  -10,  -83 },
    {  -10,  -83,   55,  -83 },
    {   55,  -83,   55,  -65 },
    {   55,  -65,  -10,  -65 },
    /* ejection_port */
    {    5,  -66,    5,  -77 },
    {    5,  -77,   38,  -77 },
    {   38,  -77,   38,  -66 },
    {   38,  -66,    5,  -66 },
    /* rear_sight */
    {   50,  -65,   50,  -61 },
    {   50,  -61,   55,  -61 },
    {   55,  -61,   55,  -65 },
    /* frame_body */
    {    0,  -83,   62,  -83 },
    {   62,  -83,   62,  -93 },
    {   62,  -93,    0,  -93 },
    {    0,  -93,    0,  -83 },
    /* trigger_guard */
    {   10,  -93,   10, -103 },
    {   10, -103,   38, -103 },
    {   38, -103,   38,  -93 },
    /* grip_outline */
    {   38,  -93,   62,  -93 },
    {   62,  -93,   68, -115 },
    {   68, -115,   44, -115 },
    {   44, -115,   38,  -93 },
    /* grip_line1 */
    {   40,  -98,   63,  -98 },
    /* grip_line2 */
    {   42, -104,   65, -104 },
    /* grip_line3 */
    {   44, -110,   67, -110 },
};
static const int pistol_side_seg_count =
    (int)(sizeof(pistol_side_segs) / sizeof(pistol_side_segs[0]));

/* --- pistol_side.vec 3D table ---
 * 16 unique verts, 12 paths, 38 total point refs */
static const vec3d_vert_t pistol_side_verts[] = {
    {  -60,  -63,    0 },
    {   -5,  -63,    0 },
    {  -10,  -63,    0 },
    {   55,  -63,    0 },
    {    5,  -63,    0 },
    {   38,  -63,    0 },
    {   50,  -63,    0 },
    {   50,  -61,    0 },
    {   55,  -61,    0 },
    {    0,  -63,    0 },
    {   62,  -63,    0 },
    {   10,  -63,    0 },
    {   63,  -63,    0 },
    {   44,  -63,    0 },
    {   40,  -63,    0 },
    {   42,  -63,    0 },
};
static const int pistol_side_vert_count = 16;

static const uint8_t pistol_side_path0_idx[] = { 0, 1, 1, 0 };
static const uint8_t pistol_side_path1_idx[] = { 0, 0 };
static const uint8_t pistol_side_path2_idx[] = { 2, 3 };
static const uint8_t pistol_side_path3_idx[] = { 2, 2, 3, 3 };
static const uint8_t pistol_side_path4_idx[] = { 4, 4, 5, 5 };
static const uint8_t pistol_side_path5_idx[] = { 6, 7, 8, 3 };
static const uint8_t pistol_side_path6_idx[] = { 9, 10, 10, 9 };
static const uint8_t pistol_side_path7_idx[] = { 11, 11, 5, 5 };
static const uint8_t pistol_side_path8_idx[] = { 5, 10, 12, 13 };
static const uint8_t pistol_side_path9_idx[] = { 14, 12 };
static const uint8_t pistol_side_path10_idx[] = { 15, 12 };
static const uint8_t pistol_side_path11_idx[] = { 13, 12 };
static const vec3d_path_t pistol_side_paths[] = {
    { 4, 1, pistol_side_path0_idx },
    { 2, 0, pistol_side_path1_idx },
    { 2, 0, pistol_side_path2_idx },
    { 4, 1, pistol_side_path3_idx },
    { 4, 1, pistol_side_path4_idx },
    { 4, 0, pistol_side_path5_idx },
    { 4, 1, pistol_side_path6_idx },
    { 4, 0, pistol_side_path7_idx },
    { 4, 1, pistol_side_path8_idx },
    { 2, 0, pistol_side_path9_idx },
    { 2, 0, pistol_side_path10_idx },
    { 2, 0, pistol_side_path11_idx },
};
static const int pistol_side_path_count = 12;

/* --- pistol_flash.vec --- */
static const vec_seg_t pistol_flash_segs[] = {
    /* ring */
    {   25,  -32,   33,  -40 },
    {   33,  -40,   25,  -48 },
    {   25,  -48,   17,  -40 },
    {   17,  -40,   25,  -32 },
    /* ray_up */
    {   25,  -32,   25,  -22 },
    /* ray_upleft */
    {   17,  -32,   11,  -26 },
    /* ray_upright */
    {   33,  -32,   39,  -26 },
};
static const int pistol_flash_seg_count =
    (int)(sizeof(pistol_flash_segs) / sizeof(pistol_flash_segs[0]));

/* --- pistol_flash.vec 3D table ---
 * 9 unique verts, 4 paths, 10 total point refs */
static const vec3d_vert_t pistol_flash_verts[] = {
    {   25,  -32,    0 },
    {   33,  -40,    0 },
    {   25,  -48,    0 },
    {   17,  -40,    0 },
    {   25,  -22,    0 },
    {   17,  -32,    0 },
    {   11,  -26,    0 },
    {   33,  -32,    0 },
    {   39,  -26,    0 },
};
static const int pistol_flash_vert_count = 9;

static const uint8_t pistol_flash_path0_idx[] = { 0, 1, 2, 3 };
static const uint8_t pistol_flash_path1_idx[] = { 0, 4 };
static const uint8_t pistol_flash_path2_idx[] = { 5, 6 };
static const uint8_t pistol_flash_path3_idx[] = { 7, 8 };
static const vec3d_path_t pistol_flash_paths[] = {
    { 4, 1, pistol_flash_path0_idx },
    { 2, 0, pistol_flash_path1_idx },
    { 2, 0, pistol_flash_path2_idx },
    { 2, 0, pistol_flash_path3_idx },
};
static const int pistol_flash_path_count = 4;

/* --- shotgun.vec --- */
static const vec_seg_t shotgun_segs[] = {
    /* barrel_outer */
    {  -10,  -50,   10,  -50 },
    {   10,  -50,   10, -100 },
    {   10, -100,  -10, -100 },
    {  -10, -100,  -10,  -50 },
    /* barrel_inner */
    {   -6,  -52,    6,  -52 },
    {    6,  -52,    6,  -98 },
    {    6,  -98,   -6,  -98 },
    {   -6,  -98,   -6,  -52 },
    /* front_sight */
    {    0, -100,    0, -107 },
    /* pump_body */
    {  -14,  -74,   14,  -74 },
    {   14,  -74,   14,  -88 },
    {   14,  -88,  -14,  -88 },
    {  -14,  -88,  -14,  -74 },
    /* pump_rail_l */
    {  -10,  -74,  -10,  -88 },
    /* pump_rail_r */
    {   10,  -74,   10,  -88 },
    /* receiver_body */
    {  -18,  -88,   18,  -88 },
    {   18,  -88,   18, -104 },
    {   18, -104,  -18, -104 },
    {  -18, -104,  -18,  -88 },
    /* ejection_port */
    {    8,  -89,   16,  -89 },
    {   16,  -89,   16, -102 },
    {   16, -102,    8, -102 },
    {    8, -102,    8,  -89 },
    /* rear_sight */
    {   -4,  -88,    4,  -88 },
    /* trigger_guard */
    {   -8, -104,   -8, -118 },
    {   -8, -118,    8, -118 },
    {    8, -118,    8, -104 },
    /* grip_body */
    {  -14, -104,   14, -104 },
    {   14, -104,   12, -127 },
    {   12, -127,  -12, -127 },
    {  -12, -127,  -14, -104 },
    /* grip_line1 */
    {  -13, -112,   13, -112 },
    /* grip_line2 */
    {  -13, -120,   13, -120 },
};
static const int shotgun_seg_count =
    (int)(sizeof(shotgun_segs) / sizeof(shotgun_segs[0]));

/* --- shotgun.vec 3D table ---
 * 22 unique verts, 13 paths, 41 total point refs */
static const vec3d_vert_t shotgun_verts[] = {
    {  -10,  -50,    0 },
    {   10,  -50,    0 },
    {   10,  -63,    0 },
    {  -10,  -63,    0 },
    {   -6,  -52,    0 },
    {    6,  -52,    0 },
    {    6,  -63,    0 },
    {   -6,  -63,    0 },
    {    0,  -63,    0 },
    {  -14,  -63,    0 },
    {   14,  -63,    0 },
    {  -18,  -63,    0 },
    {   18,  -63,    0 },
    {    8,  -63,    0 },
    {   16,  -63,    0 },
    {   -4,  -63,    0 },
    {    4,  -63,    0 },
    {   -8,  -63,    0 },
    {   12,  -63,    0 },
    {  -12,  -63,    0 },
    {  -13,  -63,    0 },
    {   13,  -63,    0 },
};
static const int shotgun_vert_count = 22;

static const uint8_t shotgun_path0_idx[] = { 0, 1, 2, 3 };
static const uint8_t shotgun_path1_idx[] = { 4, 5, 6, 7, 4 };
static const uint8_t shotgun_path2_idx[] = { 8, 8 };
static const uint8_t shotgun_path3_idx[] = { 9, 10, 10, 9 };
static const uint8_t shotgun_path4_idx[] = { 3, 3 };
static const uint8_t shotgun_path5_idx[] = { 2, 2 };
static const uint8_t shotgun_path6_idx[] = { 11, 12, 12, 11 };
static const uint8_t shotgun_path7_idx[] = { 13, 14, 14, 13 };
static const uint8_t shotgun_path8_idx[] = { 15, 16 };
static const uint8_t shotgun_path9_idx[] = { 17, 17, 13, 13 };
static const uint8_t shotgun_path10_idx[] = { 9, 10, 18, 19 };
static const uint8_t shotgun_path11_idx[] = { 20, 21 };
static const uint8_t shotgun_path12_idx[] = { 20, 21 };
static const vec3d_path_t shotgun_paths[] = {
    { 4, 1, shotgun_path0_idx },
    { 5, 0, shotgun_path1_idx },
    { 2, 0, shotgun_path2_idx },
    { 4, 1, shotgun_path3_idx },
    { 2, 0, shotgun_path4_idx },
    { 2, 0, shotgun_path5_idx },
    { 4, 1, shotgun_path6_idx },
    { 4, 1, shotgun_path7_idx },
    { 2, 0, shotgun_path8_idx },
    { 4, 0, shotgun_path9_idx },
    { 4, 1, shotgun_path10_idx },
    { 2, 0, shotgun_path11_idx },
    { 2, 0, shotgun_path12_idx },
};
static const int shotgun_path_count = 13;

/* --- shotgun_side.vec --- */
static const vec_seg_t shotgun_side_segs[] = {
    /* barrel */
    {  -80,  -70,   10,  -70 },
    {   10,  -70,   10,  -78 },
    {   10,  -78,  -80,  -78 },
    {  -80,  -78,  -80,  -70 },
    /* front_sight */
    {  -80,  -78,  -80,  -83 },
    /* mag_tube */
    {  -78,  -78,    5,  -78 },
    {    5,  -78,    5,  -84 },
    {    5,  -84,  -78,  -84 },
    {  -78,  -84,  -78,  -78 },
    /* pump_handle */
    {  -30,  -84,   10,  -84 },
    {   10,  -84,   10,  -94 },
    {   10,  -94,  -30,  -94 },
    {  -30,  -94,  -30,  -84 },
    /* pump_line */
    {  -20,  -84,  -20,  -94 },
    /* receiver_body */
    {    5,  -65,   55,  -65 },
    {   55,  -65,   55,  -94 },
    {   55,  -94,    5,  -94 },
    {    5,  -94,    5,  -65 },
    /* ejection_port */
    {    8,  -66,   40,  -66 },
    {   40,  -66,   40,  -78 },
    {   40,  -78,    8,  -78 },
    {    8,  -78,    8,  -66 },
    /* stock_body */
    {   50,  -70,   75,  -70 },
    {   75,  -70,   80, -100 },
    {   80, -100,   55, -100 },
    {   55, -100,   50,  -70 },
    /* stock_line */
    {   55,  -75,   78,  -75 },
    /* trigger_guard */
    {   20,  -94,   20, -108 },
    {   20, -108,   48, -108 },
    {   48, -108,   48,  -94 },
    /* grip_outline */
    {   30,  -94,   55,  -94 },
    {   55,  -94,   58, -115 },
    {   58, -115,   35, -115 },
    {   35, -115,   30,  -94 },
    /* grip_line */
    {   33, -100,   57, -100 },
};
static const int shotgun_side_seg_count =
    (int)(sizeof(shotgun_side_segs) / sizeof(shotgun_side_segs[0]));

/* --- shotgun_side.vec 3D table ---
 * 17 unique verts, 12 paths, 40 total point refs */
static const vec3d_vert_t shotgun_side_verts[] = {
    {  -63,  -63,    0 },
    {   10,  -63,    0 },
    {    5,  -63,    0 },
    {  -30,  -63,    0 },
    {  -20,  -63,    0 },
    {   55,  -63,    0 },
    {    8,  -63,    0 },
    {   40,  -63,    0 },
    {   50,  -63,    0 },
    {   63,  -63,    0 },
    {   20,  -63,    0 },
    {   48,  -63,    0 },
    {   30,  -63,    0 },
    {   58,  -63,    0 },
    {   35,  -63,    0 },
    {   33,  -63,    0 },
    {   57,  -63,    0 },
};
static const int shotgun_side_vert_count = 17;

static const uint8_t shotgun_side_path0_idx[] = { 0, 1, 1, 0 };
static const uint8_t shotgun_side_path1_idx[] = { 0, 0 };
static const uint8_t shotgun_side_path2_idx[] = { 0, 2, 2, 0 };
static const uint8_t shotgun_side_path3_idx[] = { 3, 1, 1, 3 };
static const uint8_t shotgun_side_path4_idx[] = { 4, 4 };
static const uint8_t shotgun_side_path5_idx[] = { 2, 5, 5, 2 };
static const uint8_t shotgun_side_path6_idx[] = { 6, 7, 7, 6 };
static const uint8_t shotgun_side_path7_idx[] = { 8, 9, 9, 5 };
static const uint8_t shotgun_side_path8_idx[] = { 5, 9 };
static const uint8_t shotgun_side_path9_idx[] = { 10, 10, 11, 11 };
static const uint8_t shotgun_side_path10_idx[] = { 12, 5, 13, 14 };
static const uint8_t shotgun_side_path11_idx[] = { 15, 16 };
static const vec3d_path_t shotgun_side_paths[] = {
    { 4, 1, shotgun_side_path0_idx },
    { 2, 0, shotgun_side_path1_idx },
    { 4, 1, shotgun_side_path2_idx },
    { 4, 1, shotgun_side_path3_idx },
    { 2, 0, shotgun_side_path4_idx },
    { 4, 1, shotgun_side_path5_idx },
    { 4, 1, shotgun_side_path6_idx },
    { 4, 1, shotgun_side_path7_idx },
    { 2, 0, shotgun_side_path8_idx },
    { 4, 0, shotgun_side_path9_idx },
    { 4, 1, shotgun_side_path10_idx },
    { 2, 0, shotgun_side_path11_idx },
};
static const int shotgun_side_path_count = 12;

/* --- shotgun_flash.vec --- */
static const vec_seg_t shotgun_flash_segs[] = {
    /* ring */
    {    0,  -26,   12,  -38 },
    {   12,  -38,    0,  -50 },
    {    0,  -50,  -12,  -38 },
    {  -12,  -38,    0,  -26 },
    /* ray_up */
    {    0,  -26,    0,  -14 },
    /* ray_upleft */
    {  -12,  -26,  -20,  -18 },
    /* ray_upright */
    {   12,  -26,   20,  -18 },
    /* ray_left */
    {  -12,  -38,  -22,  -38 },
    /* ray_right */
    {   12,  -38,   22,  -38 },
};
static const int shotgun_flash_seg_count =
    (int)(sizeof(shotgun_flash_segs) / sizeof(shotgun_flash_segs[0]));

/* --- shotgun_flash.vec 3D table ---
 * 11 unique verts, 6 paths, 14 total point refs */
static const vec3d_vert_t shotgun_flash_verts[] = {
    {    0,  -26,    0 },
    {   12,  -38,    0 },
    {    0,  -50,    0 },
    {  -12,  -38,    0 },
    {    0,  -14,    0 },
    {  -12,  -26,    0 },
    {  -20,  -18,    0 },
    {   12,  -26,    0 },
    {   20,  -18,    0 },
    {  -22,  -38,    0 },
    {   22,  -38,    0 },
};
static const int shotgun_flash_vert_count = 11;

static const uint8_t shotgun_flash_path0_idx[] = { 0, 1, 2, 3 };
static const uint8_t shotgun_flash_path1_idx[] = { 0, 4 };
static const uint8_t shotgun_flash_path2_idx[] = { 5, 6 };
static const uint8_t shotgun_flash_path3_idx[] = { 7, 8 };
static const uint8_t shotgun_flash_path4_idx[] = { 3, 9 };
static const uint8_t shotgun_flash_path5_idx[] = { 1, 10 };
static const vec3d_path_t shotgun_flash_paths[] = {
    { 4, 1, shotgun_flash_path0_idx },
    { 2, 0, shotgun_flash_path1_idx },
    { 2, 0, shotgun_flash_path2_idx },
    { 2, 0, shotgun_flash_path3_idx },
    { 2, 0, shotgun_flash_path4_idx },
    { 2, 0, shotgun_flash_path5_idx },
};
static const int shotgun_flash_path_count = 6;

#endif /* WEAPONS_ASSETS_H */
