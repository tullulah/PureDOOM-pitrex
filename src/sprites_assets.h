/* Auto-generated from enemy_humanoid.vec, enemy_imp.vec, enemy_caco.vec, enemy_skull.vec, enemy_demon.vec, enemy_baron.vec, enemy_cyber.vec, enemy_spider.vec, pickup_health.vec, pickup_armor.vec, pickup_ammo_clip.vec, pickup_rocket.vec, pickup_cell.vec, pickup_weapon.vec, pickup_key.vec, pickup_powerup.vec, pickup_barrel.vec, unknown_enemy.vec, unknown_pickup.vec, corpse.vec, decoration_pillar.vec, barrel_explosion.vec
 * by tools/vec_to_c.py — 2026-04-19 — DO NOT EDIT */
#ifndef SPRITES_ASSETS_H
#define SPRITES_ASSETS_H

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

/* --- enemy_humanoid.vec --- */
static const vec_seg_t enemy_humanoid_segs[] = {
    /* s0 */
    {    0,   55,   10,   45 },
    /* s1 */
    {   10,   45,    0,   35 },
    /* s2 */
    {    0,   35,  -10,   45 },
    /* s3 */
    {  -10,   45,    0,   55 },
    /* s4 */
    {    0,   35,    0,   10 },
    /* s5 */
    {  -18,   22,   18,   22 },
    /* s6 */
    {  -18,   22,  -20,    5 },
    /* s7 */
    {   18,   22,   20,    5 },
    /* s8 */
    {    0,   10,  -12,  -25 },
    /* s9 */
    {    0,   10,   12,  -25 },
    /* s10 */
    {  -12,  -25,  -18,  -25 },
    /* s11 */
    {   12,  -25,   18,  -25 },
};
static const int enemy_humanoid_seg_count =
    (int)(sizeof(enemy_humanoid_segs) / sizeof(enemy_humanoid_segs[0]));

/* --- enemy_humanoid.vec 3D table ---
 * 13 unique verts, 12 paths, 24 total point refs */
static const vec3d_vert_t enemy_humanoid_verts[] = {
    {    0,   55,    0 },
    {   10,   45,    0 },
    {    0,   35,    0 },
    {  -10,   45,    0 },
    {    0,   10,    0 },
    {  -18,   22,    0 },
    {   18,   22,    0 },
    {  -20,    5,    0 },
    {   20,    5,    0 },
    {  -12,  -25,    0 },
    {   12,  -25,    0 },
    {  -18,  -25,    0 },
    {   18,  -25,    0 },
};
static const int enemy_humanoid_vert_count = 13;

static const uint8_t enemy_humanoid_path0_idx[] = { 0, 1 };
static const uint8_t enemy_humanoid_path1_idx[] = { 1, 2 };
static const uint8_t enemy_humanoid_path2_idx[] = { 2, 3 };
static const uint8_t enemy_humanoid_path3_idx[] = { 3, 0 };
static const uint8_t enemy_humanoid_path4_idx[] = { 2, 4 };
static const uint8_t enemy_humanoid_path5_idx[] = { 5, 6 };
static const uint8_t enemy_humanoid_path6_idx[] = { 5, 7 };
static const uint8_t enemy_humanoid_path7_idx[] = { 6, 8 };
static const uint8_t enemy_humanoid_path8_idx[] = { 4, 9 };
static const uint8_t enemy_humanoid_path9_idx[] = { 4, 10 };
static const uint8_t enemy_humanoid_path10_idx[] = { 9, 11 };
static const uint8_t enemy_humanoid_path11_idx[] = { 10, 12 };
static const vec3d_path_t enemy_humanoid_paths[] = {
    { 2, 0, enemy_humanoid_path0_idx },
    { 2, 0, enemy_humanoid_path1_idx },
    { 2, 0, enemy_humanoid_path2_idx },
    { 2, 0, enemy_humanoid_path3_idx },
    { 2, 0, enemy_humanoid_path4_idx },
    { 2, 0, enemy_humanoid_path5_idx },
    { 2, 0, enemy_humanoid_path6_idx },
    { 2, 0, enemy_humanoid_path7_idx },
    { 2, 0, enemy_humanoid_path8_idx },
    { 2, 0, enemy_humanoid_path9_idx },
    { 2, 0, enemy_humanoid_path10_idx },
    { 2, 0, enemy_humanoid_path11_idx },
};
static const int enemy_humanoid_path_count = 12;

/* --- enemy_imp.vec --- */
static const vec_seg_t enemy_imp_segs[] = {
    /* s0 */
    {    0,   32,   20,   12 },
    /* s1 */
    {   20,   12,    0,   -8 },
    /* s2 */
    {    0,   -8,  -20,   12 },
    /* s3 */
    {  -20,   12,    0,   32 },
    /* s4 */
    {    0,   32,   -8,   55 },
    /* s5 */
    {    0,   32,    8,   55 },
    /* s6 */
    {  -20,   12,  -45,    2 },
    /* s7 */
    {  -45,    2,  -35,  -15 },
    /* s8 */
    {   20,   12,   45,    2 },
    /* s9 */
    {   45,    2,   35,  -15 },
    /* s10 */
    {   -8,   -8,  -14,  -35 },
    /* s11 */
    {    8,   -8,   14,  -35 },
};
static const int enemy_imp_seg_count =
    (int)(sizeof(enemy_imp_segs) / sizeof(enemy_imp_segs[0]));

/* --- enemy_imp.vec 3D table ---
 * 14 unique verts, 12 paths, 24 total point refs */
static const vec3d_vert_t enemy_imp_verts[] = {
    {    0,   32,    0 },
    {   20,   12,    0 },
    {    0,   -8,    0 },
    {  -20,   12,    0 },
    {   -8,   55,    0 },
    {    8,   55,    0 },
    {  -45,    2,    0 },
    {  -35,  -15,    0 },
    {   45,    2,    0 },
    {   35,  -15,    0 },
    {   -8,   -8,    0 },
    {  -14,  -35,    0 },
    {    8,   -8,    0 },
    {   14,  -35,    0 },
};
static const int enemy_imp_vert_count = 14;

static const uint8_t enemy_imp_path0_idx[] = { 0, 1 };
static const uint8_t enemy_imp_path1_idx[] = { 1, 2 };
static const uint8_t enemy_imp_path2_idx[] = { 2, 3 };
static const uint8_t enemy_imp_path3_idx[] = { 3, 0 };
static const uint8_t enemy_imp_path4_idx[] = { 0, 4 };
static const uint8_t enemy_imp_path5_idx[] = { 0, 5 };
static const uint8_t enemy_imp_path6_idx[] = { 3, 6 };
static const uint8_t enemy_imp_path7_idx[] = { 6, 7 };
static const uint8_t enemy_imp_path8_idx[] = { 1, 8 };
static const uint8_t enemy_imp_path9_idx[] = { 8, 9 };
static const uint8_t enemy_imp_path10_idx[] = { 10, 11 };
static const uint8_t enemy_imp_path11_idx[] = { 12, 13 };
static const vec3d_path_t enemy_imp_paths[] = {
    { 2, 0, enemy_imp_path0_idx },
    { 2, 0, enemy_imp_path1_idx },
    { 2, 0, enemy_imp_path2_idx },
    { 2, 0, enemy_imp_path3_idx },
    { 2, 0, enemy_imp_path4_idx },
    { 2, 0, enemy_imp_path5_idx },
    { 2, 0, enemy_imp_path6_idx },
    { 2, 0, enemy_imp_path7_idx },
    { 2, 0, enemy_imp_path8_idx },
    { 2, 0, enemy_imp_path9_idx },
    { 2, 0, enemy_imp_path10_idx },
    { 2, 0, enemy_imp_path11_idx },
};
static const int enemy_imp_path_count = 12;

/* --- enemy_caco.vec --- */
static const vec_seg_t enemy_caco_segs[] = {
    /* s0 */
    {    0,   50,   35,   35 },
    /* s1 */
    {   35,   35,   50,    0 },
    /* s2 */
    {   50,    0,   35,  -35 },
    /* s3 */
    {   35,  -35,    0,  -50 },
    /* s4 */
    {    0,  -50,  -35,  -35 },
    /* s5 */
    {  -35,  -35,  -50,    0 },
    /* s6 */
    {  -50,    0,  -35,   35 },
    /* s7 */
    {  -35,   35,    0,   50 },
    /* s8 */
    {   -8,   18,    8,   18 },
    /* s9 */
    {    0,   10,    0,   26 },
    /* s10 */
    {  -35,  -35,  -20,  -50 },
    /* s11 */
    {  -20,  -50,    0,  -40 },
    /* s12 */
    {    0,  -40,   20,  -50 },
    /* s13 */
    {   20,  -50,   35,  -35 },
};
static const int enemy_caco_seg_count =
    (int)(sizeof(enemy_caco_segs) / sizeof(enemy_caco_segs[0]));

/* --- enemy_caco.vec 3D table ---
 * 15 unique verts, 14 paths, 28 total point refs */
static const vec3d_vert_t enemy_caco_verts[] = {
    {    0,   50,    0 },
    {   35,   35,    0 },
    {   50,    0,    0 },
    {   35,  -35,    0 },
    {    0,  -50,    0 },
    {  -35,  -35,    0 },
    {  -50,    0,    0 },
    {  -35,   35,    0 },
    {   -8,   18,    0 },
    {    8,   18,    0 },
    {    0,   10,    0 },
    {    0,   26,    0 },
    {  -20,  -50,    0 },
    {    0,  -40,    0 },
    {   20,  -50,    0 },
};
static const int enemy_caco_vert_count = 15;

static const uint8_t enemy_caco_path0_idx[] = { 0, 1 };
static const uint8_t enemy_caco_path1_idx[] = { 1, 2 };
static const uint8_t enemy_caco_path2_idx[] = { 2, 3 };
static const uint8_t enemy_caco_path3_idx[] = { 3, 4 };
static const uint8_t enemy_caco_path4_idx[] = { 4, 5 };
static const uint8_t enemy_caco_path5_idx[] = { 5, 6 };
static const uint8_t enemy_caco_path6_idx[] = { 6, 7 };
static const uint8_t enemy_caco_path7_idx[] = { 7, 0 };
static const uint8_t enemy_caco_path8_idx[] = { 8, 9 };
static const uint8_t enemy_caco_path9_idx[] = { 10, 11 };
static const uint8_t enemy_caco_path10_idx[] = { 5, 12 };
static const uint8_t enemy_caco_path11_idx[] = { 12, 13 };
static const uint8_t enemy_caco_path12_idx[] = { 13, 14 };
static const uint8_t enemy_caco_path13_idx[] = { 14, 3 };
static const vec3d_path_t enemy_caco_paths[] = {
    { 2, 0, enemy_caco_path0_idx },
    { 2, 0, enemy_caco_path1_idx },
    { 2, 0, enemy_caco_path2_idx },
    { 2, 0, enemy_caco_path3_idx },
    { 2, 0, enemy_caco_path4_idx },
    { 2, 0, enemy_caco_path5_idx },
    { 2, 0, enemy_caco_path6_idx },
    { 2, 0, enemy_caco_path7_idx },
    { 2, 0, enemy_caco_path8_idx },
    { 2, 0, enemy_caco_path9_idx },
    { 2, 0, enemy_caco_path10_idx },
    { 2, 0, enemy_caco_path11_idx },
    { 2, 0, enemy_caco_path12_idx },
    { 2, 0, enemy_caco_path13_idx },
};
static const int enemy_caco_path_count = 14;

/* --- enemy_skull.vec --- */
static const vec_seg_t enemy_skull_segs[] = {
    /* s0 */
    {  -20,   55,   20,   55 },
    /* s1 */
    {   20,   55,   35,   30 },
    /* s2 */
    {   35,   30,   28,   -5 },
    /* s3 */
    {   28,   -5,   18,  -18 },
    /* s4 */
    {   18,  -18,  -18,  -18 },
    /* s5 */
    {  -18,  -18,  -28,   -5 },
    /* s6 */
    {  -28,   -5,  -35,   30 },
    /* s7 */
    {  -35,   30,  -20,   55 },
    /* s8 */
    {  -22,   35,  -12,   20 },
    /* s9 */
    {  -12,   35,  -22,   20 },
    /* s10 */
    {   12,   35,   22,   20 },
    /* s11 */
    {   22,   35,   12,   20 },
    /* s12 */
    {  -15,   55,  -20,   63 },
    /* s13 */
    {    0,   55,    0,   65 },
    /* s14 */
    {   15,   55,   20,   63 },
};
static const int enemy_skull_seg_count =
    (int)(sizeof(enemy_skull_segs) / sizeof(enemy_skull_segs[0]));

/* --- enemy_skull.vec 3D table ---
 * 22 unique verts, 15 paths, 30 total point refs */
static const vec3d_vert_t enemy_skull_verts[] = {
    {  -20,   55,    0 },
    {   20,   55,    0 },
    {   35,   30,    0 },
    {   28,   -5,    0 },
    {   18,  -18,    0 },
    {  -18,  -18,    0 },
    {  -28,   -5,    0 },
    {  -35,   30,    0 },
    {  -22,   35,    0 },
    {  -12,   20,    0 },
    {  -12,   35,    0 },
    {  -22,   20,    0 },
    {   12,   35,    0 },
    {   22,   20,    0 },
    {   22,   35,    0 },
    {   12,   20,    0 },
    {  -15,   55,    0 },
    {  -20,   63,    0 },
    {    0,   55,    0 },
    {    0,   63,    0 },
    {   15,   55,    0 },
    {   20,   63,    0 },
};
static const int enemy_skull_vert_count = 22;

static const uint8_t enemy_skull_path0_idx[] = { 0, 1 };
static const uint8_t enemy_skull_path1_idx[] = { 1, 2 };
static const uint8_t enemy_skull_path2_idx[] = { 2, 3 };
static const uint8_t enemy_skull_path3_idx[] = { 3, 4 };
static const uint8_t enemy_skull_path4_idx[] = { 4, 5 };
static const uint8_t enemy_skull_path5_idx[] = { 5, 6 };
static const uint8_t enemy_skull_path6_idx[] = { 6, 7 };
static const uint8_t enemy_skull_path7_idx[] = { 7, 0 };
static const uint8_t enemy_skull_path8_idx[] = { 8, 9 };
static const uint8_t enemy_skull_path9_idx[] = { 10, 11 };
static const uint8_t enemy_skull_path10_idx[] = { 12, 13 };
static const uint8_t enemy_skull_path11_idx[] = { 14, 15 };
static const uint8_t enemy_skull_path12_idx[] = { 16, 17 };
static const uint8_t enemy_skull_path13_idx[] = { 18, 19 };
static const uint8_t enemy_skull_path14_idx[] = { 20, 21 };
static const vec3d_path_t enemy_skull_paths[] = {
    { 2, 0, enemy_skull_path0_idx },
    { 2, 0, enemy_skull_path1_idx },
    { 2, 0, enemy_skull_path2_idx },
    { 2, 0, enemy_skull_path3_idx },
    { 2, 0, enemy_skull_path4_idx },
    { 2, 0, enemy_skull_path5_idx },
    { 2, 0, enemy_skull_path6_idx },
    { 2, 0, enemy_skull_path7_idx },
    { 2, 0, enemy_skull_path8_idx },
    { 2, 0, enemy_skull_path9_idx },
    { 2, 0, enemy_skull_path10_idx },
    { 2, 0, enemy_skull_path11_idx },
    { 2, 0, enemy_skull_path12_idx },
    { 2, 0, enemy_skull_path13_idx },
    { 2, 0, enemy_skull_path14_idx },
};
static const int enemy_skull_path_count = 15;

/* --- enemy_demon.vec --- */
static const vec_seg_t enemy_demon_segs[] = {
    /* s0 */
    {    0,   18,   45,    5 },
    /* s1 */
    {   45,    5,    0,  -12 },
    /* s2 */
    {    0,  -12,  -45,    5 },
    /* s3 */
    {  -45,    5,    0,   18 },
    /* s4 */
    {   45,    5,   60,   18 },
    /* s5 */
    {   45,  -10,   60,  -18 },
    /* s6 */
    {  -28,  -12,  -32,  -38 },
    /* s7 */
    {   -8,  -12,   -8,  -38 },
    /* s8 */
    {    8,  -12,    8,  -38 },
    /* s9 */
    {   28,  -12,   32,  -38 },
};
static const int enemy_demon_seg_count =
    (int)(sizeof(enemy_demon_segs) / sizeof(enemy_demon_segs[0]));

/* --- enemy_demon.vec 3D table ---
 * 15 unique verts, 10 paths, 20 total point refs */
static const vec3d_vert_t enemy_demon_verts[] = {
    {    0,   18,    0 },
    {   45,    5,    0 },
    {    0,  -12,    0 },
    {  -45,    5,    0 },
    {   60,   18,    0 },
    {   45,  -10,    0 },
    {   60,  -18,    0 },
    {  -28,  -12,    0 },
    {  -32,  -38,    0 },
    {   -8,  -12,    0 },
    {   -8,  -38,    0 },
    {    8,  -12,    0 },
    {    8,  -38,    0 },
    {   28,  -12,    0 },
    {   32,  -38,    0 },
};
static const int enemy_demon_vert_count = 15;

static const uint8_t enemy_demon_path0_idx[] = { 0, 1 };
static const uint8_t enemy_demon_path1_idx[] = { 1, 2 };
static const uint8_t enemy_demon_path2_idx[] = { 2, 3 };
static const uint8_t enemy_demon_path3_idx[] = { 3, 0 };
static const uint8_t enemy_demon_path4_idx[] = { 1, 4 };
static const uint8_t enemy_demon_path5_idx[] = { 5, 6 };
static const uint8_t enemy_demon_path6_idx[] = { 7, 8 };
static const uint8_t enemy_demon_path7_idx[] = { 9, 10 };
static const uint8_t enemy_demon_path8_idx[] = { 11, 12 };
static const uint8_t enemy_demon_path9_idx[] = { 13, 14 };
static const vec3d_path_t enemy_demon_paths[] = {
    { 2, 0, enemy_demon_path0_idx },
    { 2, 0, enemy_demon_path1_idx },
    { 2, 0, enemy_demon_path2_idx },
    { 2, 0, enemy_demon_path3_idx },
    { 2, 0, enemy_demon_path4_idx },
    { 2, 0, enemy_demon_path5_idx },
    { 2, 0, enemy_demon_path6_idx },
    { 2, 0, enemy_demon_path7_idx },
    { 2, 0, enemy_demon_path8_idx },
    { 2, 0, enemy_demon_path9_idx },
};
static const int enemy_demon_path_count = 10;

/* --- enemy_baron.vec --- */
static const vec_seg_t enemy_baron_segs[] = {
    /* s0 */
    {  -15,   62,   15,   62 },
    /* s1 */
    {   15,   62,   15,   38 },
    /* s2 */
    {   15,   38,  -15,   38 },
    /* s3 */
    {  -15,   38,  -15,   62 },
    /* s4 */
    {  -15,   62,  -22,   76 },
    /* s5 */
    {   15,   62,   22,   76 },
    /* s6 */
    {    0,   38,    0,   15 },
    /* s7 */
    {  -35,   24,   35,   24 },
    /* s8 */
    {  -35,   24,  -28,    0 },
    /* s9 */
    {   35,   24,   28,    0 },
    /* s10 */
    {    0,   12,  -18,  -32 },
    /* s11 */
    {    0,   12,   18,  -32 },
};
static const int enemy_baron_seg_count =
    (int)(sizeof(enemy_baron_segs) / sizeof(enemy_baron_segs[0]));

/* --- enemy_baron.vec 3D table ---
 * 15 unique verts, 12 paths, 24 total point refs */
static const vec3d_vert_t enemy_baron_verts[] = {
    {  -15,   62,    0 },
    {   15,   62,    0 },
    {   15,   38,    0 },
    {  -15,   38,    0 },
    {  -22,   63,    0 },
    {   22,   63,    0 },
    {    0,   38,    0 },
    {    0,   15,    0 },
    {  -35,   24,    0 },
    {   35,   24,    0 },
    {  -28,    0,    0 },
    {   28,    0,    0 },
    {    0,   12,    0 },
    {  -18,  -32,    0 },
    {   18,  -32,    0 },
};
static const int enemy_baron_vert_count = 15;

static const uint8_t enemy_baron_path0_idx[] = { 0, 1 };
static const uint8_t enemy_baron_path1_idx[] = { 1, 2 };
static const uint8_t enemy_baron_path2_idx[] = { 2, 3 };
static const uint8_t enemy_baron_path3_idx[] = { 3, 0 };
static const uint8_t enemy_baron_path4_idx[] = { 0, 4 };
static const uint8_t enemy_baron_path5_idx[] = { 1, 5 };
static const uint8_t enemy_baron_path6_idx[] = { 6, 7 };
static const uint8_t enemy_baron_path7_idx[] = { 8, 9 };
static const uint8_t enemy_baron_path8_idx[] = { 8, 10 };
static const uint8_t enemy_baron_path9_idx[] = { 9, 11 };
static const uint8_t enemy_baron_path10_idx[] = { 12, 13 };
static const uint8_t enemy_baron_path11_idx[] = { 12, 14 };
static const vec3d_path_t enemy_baron_paths[] = {
    { 2, 0, enemy_baron_path0_idx },
    { 2, 0, enemy_baron_path1_idx },
    { 2, 0, enemy_baron_path2_idx },
    { 2, 0, enemy_baron_path3_idx },
    { 2, 0, enemy_baron_path4_idx },
    { 2, 0, enemy_baron_path5_idx },
    { 2, 0, enemy_baron_path6_idx },
    { 2, 0, enemy_baron_path7_idx },
    { 2, 0, enemy_baron_path8_idx },
    { 2, 0, enemy_baron_path9_idx },
    { 2, 0, enemy_baron_path10_idx },
    { 2, 0, enemy_baron_path11_idx },
};
static const int enemy_baron_path_count = 12;

/* --- enemy_cyber.vec --- */
static const vec_seg_t enemy_cyber_segs[] = {
    /* s0 */
    {  -12,   62,   12,   62 },
    /* s1 */
    {   12,   62,   12,   40 },
    /* s2 */
    {   12,   40,  -12,   40 },
    /* s3 */
    {  -12,   40,  -12,   62 },
    /* s4 */
    {  -12,   62,  -18,   76 },
    /* s5 */
    {   12,   62,   18,   76 },
    /* s6 */
    {  -25,   35,   25,   35 },
    /* s7 */
    {   25,   35,   25,    5 },
    /* s8 */
    {   25,    5,  -25,    5 },
    /* s9 */
    {  -25,    5,  -25,   35 },
    /* s10 */
    {   25,   28,   42,   28 },
    /* s11 */
    {   42,   36,   58,   36 },
    /* s12 */
    {   42,   20,   58,   20 },
    /* s13 */
    {   58,   36,   58,   20 },
    /* s14 */
    {  -18,    5,  -22,  -32 },
    /* s15 */
    {   18,    5,   22,  -32 },
    /* s16 */
    {  -22,  -32,  -30,  -32 },
    /* s17 */
    {   22,  -32,   30,  -32 },
};
static const int enemy_cyber_seg_count =
    (int)(sizeof(enemy_cyber_segs) / sizeof(enemy_cyber_segs[0]));

/* --- enemy_cyber.vec 3D table ---
 * 22 unique verts, 18 paths, 36 total point refs */
static const vec3d_vert_t enemy_cyber_verts[] = {
    {  -12,   62,    0 },
    {   12,   62,    0 },
    {   12,   40,    0 },
    {  -12,   40,    0 },
    {  -18,   63,    0 },
    {   18,   63,    0 },
    {  -25,   35,    0 },
    {   25,   35,    0 },
    {   25,    5,    0 },
    {  -25,    5,    0 },
    {   25,   28,    0 },
    {   42,   28,    0 },
    {   42,   36,    0 },
    {   58,   36,    0 },
    {   42,   20,    0 },
    {   58,   20,    0 },
    {  -18,    5,    0 },
    {  -22,  -32,    0 },
    {   18,    5,    0 },
    {   22,  -32,    0 },
    {  -30,  -32,    0 },
    {   30,  -32,    0 },
};
static const int enemy_cyber_vert_count = 22;

static const uint8_t enemy_cyber_path0_idx[] = { 0, 1 };
static const uint8_t enemy_cyber_path1_idx[] = { 1, 2 };
static const uint8_t enemy_cyber_path2_idx[] = { 2, 3 };
static const uint8_t enemy_cyber_path3_idx[] = { 3, 0 };
static const uint8_t enemy_cyber_path4_idx[] = { 0, 4 };
static const uint8_t enemy_cyber_path5_idx[] = { 1, 5 };
static const uint8_t enemy_cyber_path6_idx[] = { 6, 7 };
static const uint8_t enemy_cyber_path7_idx[] = { 7, 8 };
static const uint8_t enemy_cyber_path8_idx[] = { 8, 9 };
static const uint8_t enemy_cyber_path9_idx[] = { 9, 6 };
static const uint8_t enemy_cyber_path10_idx[] = { 10, 11 };
static const uint8_t enemy_cyber_path11_idx[] = { 12, 13 };
static const uint8_t enemy_cyber_path12_idx[] = { 14, 15 };
static const uint8_t enemy_cyber_path13_idx[] = { 13, 15 };
static const uint8_t enemy_cyber_path14_idx[] = { 16, 17 };
static const uint8_t enemy_cyber_path15_idx[] = { 18, 19 };
static const uint8_t enemy_cyber_path16_idx[] = { 17, 20 };
static const uint8_t enemy_cyber_path17_idx[] = { 19, 21 };
static const vec3d_path_t enemy_cyber_paths[] = {
    { 2, 0, enemy_cyber_path0_idx },
    { 2, 0, enemy_cyber_path1_idx },
    { 2, 0, enemy_cyber_path2_idx },
    { 2, 0, enemy_cyber_path3_idx },
    { 2, 0, enemy_cyber_path4_idx },
    { 2, 0, enemy_cyber_path5_idx },
    { 2, 0, enemy_cyber_path6_idx },
    { 2, 0, enemy_cyber_path7_idx },
    { 2, 0, enemy_cyber_path8_idx },
    { 2, 0, enemy_cyber_path9_idx },
    { 2, 0, enemy_cyber_path10_idx },
    { 2, 0, enemy_cyber_path11_idx },
    { 2, 0, enemy_cyber_path12_idx },
    { 2, 0, enemy_cyber_path13_idx },
    { 2, 0, enemy_cyber_path14_idx },
    { 2, 0, enemy_cyber_path15_idx },
    { 2, 0, enemy_cyber_path16_idx },
    { 2, 0, enemy_cyber_path17_idx },
};
static const int enemy_cyber_path_count = 18;

/* --- enemy_spider.vec --- */
static const vec_seg_t enemy_spider_segs[] = {
    /* s0 */
    {    0,   35,   24,   24 },
    /* s1 */
    {   24,   24,   35,    0 },
    /* s2 */
    {   35,    0,   24,  -24 },
    /* s3 */
    {   24,  -24,    0,  -35 },
    /* s4 */
    {    0,  -35,  -24,  -24 },
    /* s5 */
    {  -24,  -24,  -35,    0 },
    /* s6 */
    {  -35,    0,  -24,   24 },
    /* s7 */
    {  -24,   24,    0,   35 },
    /* s8 */
    {  -25,   25,  -50,   12 },
    /* s9 */
    {   25,   25,   50,   12 },
    /* s10 */
    {  -25,  -25,  -50,  -12 },
    /* s11 */
    {   25,  -25,   50,  -12 },
    /* s12 */
    {  -50,   12,  -62,    0 },
    /* s13 */
    {   50,   12,   62,    0 },
    /* s14 */
    {  -50,  -12,  -62,    0 },
    /* s15 */
    {   50,  -12,   62,    0 },
};
static const int enemy_spider_seg_count =
    (int)(sizeof(enemy_spider_segs) / sizeof(enemy_spider_segs[0]));

/* --- enemy_spider.vec 3D table ---
 * 18 unique verts, 16 paths, 32 total point refs */
static const vec3d_vert_t enemy_spider_verts[] = {
    {    0,   35,    0 },
    {   24,   24,    0 },
    {   35,    0,    0 },
    {   24,  -24,    0 },
    {    0,  -35,    0 },
    {  -24,  -24,    0 },
    {  -35,    0,    0 },
    {  -24,   24,    0 },
    {  -25,   25,    0 },
    {  -50,   12,    0 },
    {   25,   25,    0 },
    {   50,   12,    0 },
    {  -25,  -25,    0 },
    {  -50,  -12,    0 },
    {   25,  -25,    0 },
    {   50,  -12,    0 },
    {  -62,    0,    0 },
    {   62,    0,    0 },
};
static const int enemy_spider_vert_count = 18;

static const uint8_t enemy_spider_path0_idx[] = { 0, 1 };
static const uint8_t enemy_spider_path1_idx[] = { 1, 2 };
static const uint8_t enemy_spider_path2_idx[] = { 2, 3 };
static const uint8_t enemy_spider_path3_idx[] = { 3, 4 };
static const uint8_t enemy_spider_path4_idx[] = { 4, 5 };
static const uint8_t enemy_spider_path5_idx[] = { 5, 6 };
static const uint8_t enemy_spider_path6_idx[] = { 6, 7 };
static const uint8_t enemy_spider_path7_idx[] = { 7, 0 };
static const uint8_t enemy_spider_path8_idx[] = { 8, 9 };
static const uint8_t enemy_spider_path9_idx[] = { 10, 11 };
static const uint8_t enemy_spider_path10_idx[] = { 12, 13 };
static const uint8_t enemy_spider_path11_idx[] = { 14, 15 };
static const uint8_t enemy_spider_path12_idx[] = { 9, 16 };
static const uint8_t enemy_spider_path13_idx[] = { 11, 17 };
static const uint8_t enemy_spider_path14_idx[] = { 13, 16 };
static const uint8_t enemy_spider_path15_idx[] = { 15, 17 };
static const vec3d_path_t enemy_spider_paths[] = {
    { 2, 0, enemy_spider_path0_idx },
    { 2, 0, enemy_spider_path1_idx },
    { 2, 0, enemy_spider_path2_idx },
    { 2, 0, enemy_spider_path3_idx },
    { 2, 0, enemy_spider_path4_idx },
    { 2, 0, enemy_spider_path5_idx },
    { 2, 0, enemy_spider_path6_idx },
    { 2, 0, enemy_spider_path7_idx },
    { 2, 0, enemy_spider_path8_idx },
    { 2, 0, enemy_spider_path9_idx },
    { 2, 0, enemy_spider_path10_idx },
    { 2, 0, enemy_spider_path11_idx },
    { 2, 0, enemy_spider_path12_idx },
    { 2, 0, enemy_spider_path13_idx },
    { 2, 0, enemy_spider_path14_idx },
    { 2, 0, enemy_spider_path15_idx },
};
static const int enemy_spider_path_count = 16;

/* --- pickup_health.vec --- */
static const vec_seg_t pickup_health_segs[] = {
    /* s0 */
    {  -20,   52,   20,   52 },
    /* s1 */
    {   20,   52,   20,   20 },
    /* s2 */
    {   20,   20,   52,   20 },
    /* s3 */
    {   52,   20,   52,  -20 },
    /* s4 */
    {   52,  -20,   20,  -20 },
    /* s5 */
    {   20,  -20,   20,  -52 },
    /* s6 */
    {   20,  -52,  -20,  -52 },
    /* s7 */
    {  -20,  -52,  -20,  -20 },
    /* s8 */
    {  -20,  -20,  -52,  -20 },
    /* s9 */
    {  -52,  -20,  -52,   20 },
    /* s10 */
    {  -52,   20,  -20,   20 },
    /* s11 */
    {  -20,   20,  -20,   52 },
};
static const int pickup_health_seg_count =
    (int)(sizeof(pickup_health_segs) / sizeof(pickup_health_segs[0]));

/* --- pickup_health.vec 3D table ---
 * 12 unique verts, 12 paths, 24 total point refs */
static const vec3d_vert_t pickup_health_verts[] = {
    {  -20,   52,    0 },
    {   20,   52,    0 },
    {   20,   20,    0 },
    {   52,   20,    0 },
    {   52,  -20,    0 },
    {   20,  -20,    0 },
    {   20,  -52,    0 },
    {  -20,  -52,    0 },
    {  -20,  -20,    0 },
    {  -52,  -20,    0 },
    {  -52,   20,    0 },
    {  -20,   20,    0 },
};
static const int pickup_health_vert_count = 12;

static const uint8_t pickup_health_path0_idx[] = { 0, 1 };
static const uint8_t pickup_health_path1_idx[] = { 1, 2 };
static const uint8_t pickup_health_path2_idx[] = { 2, 3 };
static const uint8_t pickup_health_path3_idx[] = { 3, 4 };
static const uint8_t pickup_health_path4_idx[] = { 4, 5 };
static const uint8_t pickup_health_path5_idx[] = { 5, 6 };
static const uint8_t pickup_health_path6_idx[] = { 6, 7 };
static const uint8_t pickup_health_path7_idx[] = { 7, 8 };
static const uint8_t pickup_health_path8_idx[] = { 8, 9 };
static const uint8_t pickup_health_path9_idx[] = { 9, 10 };
static const uint8_t pickup_health_path10_idx[] = { 10, 11 };
static const uint8_t pickup_health_path11_idx[] = { 11, 0 };
static const vec3d_path_t pickup_health_paths[] = {
    { 2, 0, pickup_health_path0_idx },
    { 2, 0, pickup_health_path1_idx },
    { 2, 0, pickup_health_path2_idx },
    { 2, 0, pickup_health_path3_idx },
    { 2, 0, pickup_health_path4_idx },
    { 2, 0, pickup_health_path5_idx },
    { 2, 0, pickup_health_path6_idx },
    { 2, 0, pickup_health_path7_idx },
    { 2, 0, pickup_health_path8_idx },
    { 2, 0, pickup_health_path9_idx },
    { 2, 0, pickup_health_path10_idx },
    { 2, 0, pickup_health_path11_idx },
};
static const int pickup_health_path_count = 12;

/* --- pickup_armor.vec --- */
static const vec_seg_t pickup_armor_segs[] = {
    /* s0 */
    {  -50,   30,   50,   30 },
    /* s1 */
    {   50,   30,   35,  -22 },
    /* s2 */
    {   35,  -22,    0,  -52 },
    /* s3 */
    {    0,  -52,  -35,  -22 },
    /* s4 */
    {  -35,  -22,  -50,   30 },
    /* s5 */
    {  -25,    5,   25,    5 },
};
static const int pickup_armor_seg_count =
    (int)(sizeof(pickup_armor_segs) / sizeof(pickup_armor_segs[0]));

/* --- pickup_armor.vec 3D table ---
 * 7 unique verts, 6 paths, 12 total point refs */
static const vec3d_vert_t pickup_armor_verts[] = {
    {  -50,   30,    0 },
    {   50,   30,    0 },
    {   35,  -22,    0 },
    {    0,  -52,    0 },
    {  -35,  -22,    0 },
    {  -25,    5,    0 },
    {   25,    5,    0 },
};
static const int pickup_armor_vert_count = 7;

static const uint8_t pickup_armor_path0_idx[] = { 0, 1 };
static const uint8_t pickup_armor_path1_idx[] = { 1, 2 };
static const uint8_t pickup_armor_path2_idx[] = { 2, 3 };
static const uint8_t pickup_armor_path3_idx[] = { 3, 4 };
static const uint8_t pickup_armor_path4_idx[] = { 4, 0 };
static const uint8_t pickup_armor_path5_idx[] = { 5, 6 };
static const vec3d_path_t pickup_armor_paths[] = {
    { 2, 0, pickup_armor_path0_idx },
    { 2, 0, pickup_armor_path1_idx },
    { 2, 0, pickup_armor_path2_idx },
    { 2, 0, pickup_armor_path3_idx },
    { 2, 0, pickup_armor_path4_idx },
    { 2, 0, pickup_armor_path5_idx },
};
static const int pickup_armor_path_count = 6;

/* --- pickup_ammo_clip.vec --- */
static const vec_seg_t pickup_ammo_clip_segs[] = {
    /* s0 */
    {  -42,   26,   42,   26 },
    /* s1 */
    {   42,   26,   42,  -26 },
    /* s2 */
    {   42,  -26,  -42,  -26 },
    /* s3 */
    {  -42,  -26,  -42,   26 },
    /* s4 */
    {   14,   26,   14,  -26 },
};
static const int pickup_ammo_clip_seg_count =
    (int)(sizeof(pickup_ammo_clip_segs) / sizeof(pickup_ammo_clip_segs[0]));

/* --- pickup_ammo_clip.vec 3D table ---
 * 6 unique verts, 5 paths, 10 total point refs */
static const vec3d_vert_t pickup_ammo_clip_verts[] = {
    {  -42,   26,    0 },
    {   42,   26,    0 },
    {   42,  -26,    0 },
    {  -42,  -26,    0 },
    {   14,   26,    0 },
    {   14,  -26,    0 },
};
static const int pickup_ammo_clip_vert_count = 6;

static const uint8_t pickup_ammo_clip_path0_idx[] = { 0, 1 };
static const uint8_t pickup_ammo_clip_path1_idx[] = { 1, 2 };
static const uint8_t pickup_ammo_clip_path2_idx[] = { 2, 3 };
static const uint8_t pickup_ammo_clip_path3_idx[] = { 3, 0 };
static const uint8_t pickup_ammo_clip_path4_idx[] = { 4, 5 };
static const vec3d_path_t pickup_ammo_clip_paths[] = {
    { 2, 0, pickup_ammo_clip_path0_idx },
    { 2, 0, pickup_ammo_clip_path1_idx },
    { 2, 0, pickup_ammo_clip_path2_idx },
    { 2, 0, pickup_ammo_clip_path3_idx },
    { 2, 0, pickup_ammo_clip_path4_idx },
};
static const int pickup_ammo_clip_path_count = 5;

/* --- pickup_rocket.vec --- */
static const vec_seg_t pickup_rocket_segs[] = {
    /* s0 */
    {  -12,   46,   12,   46 },
    /* s1 */
    {  -12,   46,  -12,  -32 },
    /* s2 */
    {   12,   46,   12,  -32 },
    /* s3 */
    {  -12,  -32,   12,  -32 },
    /* s4 */
    {  -12,   46,    0,   63 },
    /* s5 */
    {   12,   46,    0,   63 },
    /* s6 */
    {  -12,  -32,  -26,  -52 },
    /* s7 */
    {   12,  -32,   26,  -52 },
};
static const int pickup_rocket_seg_count =
    (int)(sizeof(pickup_rocket_segs) / sizeof(pickup_rocket_segs[0]));

/* --- pickup_rocket.vec 3D table ---
 * 7 unique verts, 8 paths, 16 total point refs */
static const vec3d_vert_t pickup_rocket_verts[] = {
    {  -12,   46,    0 },
    {   12,   46,    0 },
    {  -12,  -32,    0 },
    {   12,  -32,    0 },
    {    0,   63,    0 },
    {  -26,  -52,    0 },
    {   26,  -52,    0 },
};
static const int pickup_rocket_vert_count = 7;

static const uint8_t pickup_rocket_path0_idx[] = { 0, 1 };
static const uint8_t pickup_rocket_path1_idx[] = { 0, 2 };
static const uint8_t pickup_rocket_path2_idx[] = { 1, 3 };
static const uint8_t pickup_rocket_path3_idx[] = { 2, 3 };
static const uint8_t pickup_rocket_path4_idx[] = { 0, 4 };
static const uint8_t pickup_rocket_path5_idx[] = { 1, 4 };
static const uint8_t pickup_rocket_path6_idx[] = { 2, 5 };
static const uint8_t pickup_rocket_path7_idx[] = { 3, 6 };
static const vec3d_path_t pickup_rocket_paths[] = {
    { 2, 0, pickup_rocket_path0_idx },
    { 2, 0, pickup_rocket_path1_idx },
    { 2, 0, pickup_rocket_path2_idx },
    { 2, 0, pickup_rocket_path3_idx },
    { 2, 0, pickup_rocket_path4_idx },
    { 2, 0, pickup_rocket_path5_idx },
    { 2, 0, pickup_rocket_path6_idx },
    { 2, 0, pickup_rocket_path7_idx },
};
static const int pickup_rocket_path_count = 8;

/* --- pickup_cell.vec --- */
static const vec_seg_t pickup_cell_segs[] = {
    /* s0 */
    {  -30,   46,   30,   46 },
    /* s1 */
    {   30,   46,   30,  -46 },
    /* s2 */
    {   30,  -46,  -30,  -46 },
    /* s3 */
    {  -30,  -46,  -30,   46 },
    /* s4 */
    {   15,   28,  -10,    4 },
    /* s5 */
    {  -10,    4,   15,  -24 },
    /* s6 */
    {  -12,   46,  -12,   60 },
    /* s7 */
    {   12,   46,   12,   60 },
    /* s8 */
    {  -12,   60,   12,   60 },
};
static const int pickup_cell_seg_count =
    (int)(sizeof(pickup_cell_segs) / sizeof(pickup_cell_segs[0]));

/* --- pickup_cell.vec 3D table ---
 * 11 unique verts, 9 paths, 18 total point refs */
static const vec3d_vert_t pickup_cell_verts[] = {
    {  -30,   46,    0 },
    {   30,   46,    0 },
    {   30,  -46,    0 },
    {  -30,  -46,    0 },
    {   15,   28,    0 },
    {  -10,    4,    0 },
    {   15,  -24,    0 },
    {  -12,   46,    0 },
    {  -12,   60,    0 },
    {   12,   46,    0 },
    {   12,   60,    0 },
};
static const int pickup_cell_vert_count = 11;

static const uint8_t pickup_cell_path0_idx[] = { 0, 1 };
static const uint8_t pickup_cell_path1_idx[] = { 1, 2 };
static const uint8_t pickup_cell_path2_idx[] = { 2, 3 };
static const uint8_t pickup_cell_path3_idx[] = { 3, 0 };
static const uint8_t pickup_cell_path4_idx[] = { 4, 5 };
static const uint8_t pickup_cell_path5_idx[] = { 5, 6 };
static const uint8_t pickup_cell_path6_idx[] = { 7, 8 };
static const uint8_t pickup_cell_path7_idx[] = { 9, 10 };
static const uint8_t pickup_cell_path8_idx[] = { 8, 10 };
static const vec3d_path_t pickup_cell_paths[] = {
    { 2, 0, pickup_cell_path0_idx },
    { 2, 0, pickup_cell_path1_idx },
    { 2, 0, pickup_cell_path2_idx },
    { 2, 0, pickup_cell_path3_idx },
    { 2, 0, pickup_cell_path4_idx },
    { 2, 0, pickup_cell_path5_idx },
    { 2, 0, pickup_cell_path6_idx },
    { 2, 0, pickup_cell_path7_idx },
    { 2, 0, pickup_cell_path8_idx },
};
static const int pickup_cell_path_count = 9;

/* --- pickup_weapon.vec --- */
static const vec_seg_t pickup_weapon_segs[] = {
    /* s0 */
    {  -55,   10,   30,   10 },
    /* s1 */
    {   30,   10,   30,  -10 },
    /* s2 */
    {  -10,   10,  -10,  -42 },
    /* s3 */
    {  -55,   10,  -55,  -22 },
    /* s4 */
    {  -55,  -22,  -10,  -42 },
    /* s5 */
    {  -10,  -12,    6,  -36 },
    /* s6 */
    {    6,  -36,   -6,  -42 },
};
static const int pickup_weapon_seg_count =
    (int)(sizeof(pickup_weapon_segs) / sizeof(pickup_weapon_segs[0]));

/* --- pickup_weapon.vec 3D table ---
 * 9 unique verts, 7 paths, 14 total point refs */
static const vec3d_vert_t pickup_weapon_verts[] = {
    {  -55,   10,    0 },
    {   30,   10,    0 },
    {   30,  -10,    0 },
    {  -10,   10,    0 },
    {  -10,  -42,    0 },
    {  -55,  -22,    0 },
    {  -10,  -12,    0 },
    {    6,  -36,    0 },
    {   -6,  -42,    0 },
};
static const int pickup_weapon_vert_count = 9;

static const uint8_t pickup_weapon_path0_idx[] = { 0, 1 };
static const uint8_t pickup_weapon_path1_idx[] = { 1, 2 };
static const uint8_t pickup_weapon_path2_idx[] = { 3, 4 };
static const uint8_t pickup_weapon_path3_idx[] = { 0, 5 };
static const uint8_t pickup_weapon_path4_idx[] = { 5, 4 };
static const uint8_t pickup_weapon_path5_idx[] = { 6, 7 };
static const uint8_t pickup_weapon_path6_idx[] = { 7, 8 };
static const vec3d_path_t pickup_weapon_paths[] = {
    { 2, 0, pickup_weapon_path0_idx },
    { 2, 0, pickup_weapon_path1_idx },
    { 2, 0, pickup_weapon_path2_idx },
    { 2, 0, pickup_weapon_path3_idx },
    { 2, 0, pickup_weapon_path4_idx },
    { 2, 0, pickup_weapon_path5_idx },
    { 2, 0, pickup_weapon_path6_idx },
};
static const int pickup_weapon_path_count = 7;

/* --- pickup_key.vec --- */
static const vec_seg_t pickup_key_segs[] = {
    /* s0 */
    {    0,   52,   18,   44 },
    /* s1 */
    {   18,   44,   26,   26 },
    /* s2 */
    {   26,   26,   18,    7 },
    /* s3 */
    {   18,    7,    0,    0 },
    /* s4 */
    {    0,    0,  -18,    7 },
    /* s5 */
    {  -18,    7,  -26,   25 },
    /* s6 */
    {  -26,   25,  -18,   44 },
    /* s7 */
    {  -18,   44,    0,   52 },
    /* s8 */
    {    0,    0,    0,  -52 },
    /* s9 */
    {    0,  -18,   16,  -18 },
    /* s10 */
    {    0,  -36,   16,  -36 },
};
static const int pickup_key_seg_count =
    (int)(sizeof(pickup_key_segs) / sizeof(pickup_key_segs[0]));

/* --- pickup_key.vec 3D table ---
 * 13 unique verts, 11 paths, 22 total point refs */
static const vec3d_vert_t pickup_key_verts[] = {
    {    0,   52,    0 },
    {   18,   44,    0 },
    {   26,   26,    0 },
    {   18,    7,    0 },
    {    0,    0,    0 },
    {  -18,    7,    0 },
    {  -26,   25,    0 },
    {  -18,   44,    0 },
    {    0,  -52,    0 },
    {    0,  -18,    0 },
    {   16,  -18,    0 },
    {    0,  -36,    0 },
    {   16,  -36,    0 },
};
static const int pickup_key_vert_count = 13;

static const uint8_t pickup_key_path0_idx[] = { 0, 1 };
static const uint8_t pickup_key_path1_idx[] = { 1, 2 };
static const uint8_t pickup_key_path2_idx[] = { 2, 3 };
static const uint8_t pickup_key_path3_idx[] = { 3, 4 };
static const uint8_t pickup_key_path4_idx[] = { 4, 5 };
static const uint8_t pickup_key_path5_idx[] = { 5, 6 };
static const uint8_t pickup_key_path6_idx[] = { 6, 7 };
static const uint8_t pickup_key_path7_idx[] = { 7, 0 };
static const uint8_t pickup_key_path8_idx[] = { 4, 8 };
static const uint8_t pickup_key_path9_idx[] = { 9, 10 };
static const uint8_t pickup_key_path10_idx[] = { 11, 12 };
static const vec3d_path_t pickup_key_paths[] = {
    { 2, 0, pickup_key_path0_idx },
    { 2, 0, pickup_key_path1_idx },
    { 2, 0, pickup_key_path2_idx },
    { 2, 0, pickup_key_path3_idx },
    { 2, 0, pickup_key_path4_idx },
    { 2, 0, pickup_key_path5_idx },
    { 2, 0, pickup_key_path6_idx },
    { 2, 0, pickup_key_path7_idx },
    { 2, 0, pickup_key_path8_idx },
    { 2, 0, pickup_key_path9_idx },
    { 2, 0, pickup_key_path10_idx },
};
static const int pickup_key_path_count = 11;

/* --- pickup_powerup.vec --- */
static const vec_seg_t pickup_powerup_segs[] = {
    /* s0 */
    {    0,   52,    0,   28 },
    /* s1 */
    {    0,   28,   24,   14 },
    /* s2 */
    {   45,   26,   24,   14 },
    /* s3 */
    {   24,   14,   24,  -13 },
    /* s4 */
    {   45,  -25,   24,  -13 },
    /* s5 */
    {   24,  -13,    0,  -28 },
    /* s6 */
    {    0,  -52,    0,  -28 },
    /* s7 */
    {    0,  -28,  -24,  -14 },
    /* s8 */
    {  -45,  -26,  -24,  -14 },
    /* s9 */
    {  -24,  -14,  -24,   14 },
    /* s10 */
    {  -45,   26,  -24,   14 },
    /* s11 */
    {  -24,   14,    0,   28 },
};
static const int pickup_powerup_seg_count =
    (int)(sizeof(pickup_powerup_segs) / sizeof(pickup_powerup_segs[0]));

/* --- pickup_powerup.vec 3D table ---
 * 12 unique verts, 12 paths, 24 total point refs */
static const vec3d_vert_t pickup_powerup_verts[] = {
    {    0,   52,    0 },
    {    0,   28,    0 },
    {   24,   14,    0 },
    {   45,   26,    0 },
    {   24,  -13,    0 },
    {   45,  -25,    0 },
    {    0,  -28,    0 },
    {    0,  -52,    0 },
    {  -24,  -14,    0 },
    {  -45,  -26,    0 },
    {  -24,   14,    0 },
    {  -45,   26,    0 },
};
static const int pickup_powerup_vert_count = 12;

static const uint8_t pickup_powerup_path0_idx[] = { 0, 1 };
static const uint8_t pickup_powerup_path1_idx[] = { 1, 2 };
static const uint8_t pickup_powerup_path2_idx[] = { 3, 2 };
static const uint8_t pickup_powerup_path3_idx[] = { 2, 4 };
static const uint8_t pickup_powerup_path4_idx[] = { 5, 4 };
static const uint8_t pickup_powerup_path5_idx[] = { 4, 6 };
static const uint8_t pickup_powerup_path6_idx[] = { 7, 6 };
static const uint8_t pickup_powerup_path7_idx[] = { 6, 8 };
static const uint8_t pickup_powerup_path8_idx[] = { 9, 8 };
static const uint8_t pickup_powerup_path9_idx[] = { 8, 10 };
static const uint8_t pickup_powerup_path10_idx[] = { 11, 10 };
static const uint8_t pickup_powerup_path11_idx[] = { 10, 1 };
static const vec3d_path_t pickup_powerup_paths[] = {
    { 2, 0, pickup_powerup_path0_idx },
    { 2, 0, pickup_powerup_path1_idx },
    { 2, 0, pickup_powerup_path2_idx },
    { 2, 0, pickup_powerup_path3_idx },
    { 2, 0, pickup_powerup_path4_idx },
    { 2, 0, pickup_powerup_path5_idx },
    { 2, 0, pickup_powerup_path6_idx },
    { 2, 0, pickup_powerup_path7_idx },
    { 2, 0, pickup_powerup_path8_idx },
    { 2, 0, pickup_powerup_path9_idx },
    { 2, 0, pickup_powerup_path10_idx },
    { 2, 0, pickup_powerup_path11_idx },
};
static const int pickup_powerup_path_count = 12;

/* --- pickup_barrel.vec --- */
static const vec_seg_t pickup_barrel_segs[] = {
    /* s0 */
    {  -28,   30,  -28,  -42 },
    /* s1 */
    {   28,   30,   28,  -42 },
    /* s2 */
    {  -28,  -42,   28,  -42 },
    /* s3 */
    {  -28,   30,    0,   46 },
    /* s4 */
    {    0,   46,   28,   30 },
    /* s5 */
    {  -28,   16,   28,   16 },
    /* s6 */
    {  -28,  -10,   28,  -10 },
};
static const int pickup_barrel_seg_count =
    (int)(sizeof(pickup_barrel_segs) / sizeof(pickup_barrel_segs[0]));

/* --- pickup_barrel.vec 3D table ---
 * 9 unique verts, 7 paths, 14 total point refs */
static const vec3d_vert_t pickup_barrel_verts[] = {
    {  -28,   30,    0 },
    {  -28,  -42,    0 },
    {   28,   30,    0 },
    {   28,  -42,    0 },
    {    0,   46,    0 },
    {  -28,   16,    0 },
    {   28,   16,    0 },
    {  -28,  -10,    0 },
    {   28,  -10,    0 },
};
static const int pickup_barrel_vert_count = 9;

static const uint8_t pickup_barrel_path0_idx[] = { 0, 1 };
static const uint8_t pickup_barrel_path1_idx[] = { 2, 3 };
static const uint8_t pickup_barrel_path2_idx[] = { 1, 3 };
static const uint8_t pickup_barrel_path3_idx[] = { 0, 4 };
static const uint8_t pickup_barrel_path4_idx[] = { 4, 2 };
static const uint8_t pickup_barrel_path5_idx[] = { 5, 6 };
static const uint8_t pickup_barrel_path6_idx[] = { 7, 8 };
static const vec3d_path_t pickup_barrel_paths[] = {
    { 2, 0, pickup_barrel_path0_idx },
    { 2, 0, pickup_barrel_path1_idx },
    { 2, 0, pickup_barrel_path2_idx },
    { 2, 0, pickup_barrel_path3_idx },
    { 2, 0, pickup_barrel_path4_idx },
    { 2, 0, pickup_barrel_path5_idx },
    { 2, 0, pickup_barrel_path6_idx },
};
static const int pickup_barrel_path_count = 7;

/* --- unknown_enemy.vec --- */
static const vec_seg_t unknown_enemy_segs[] = {
    /* s0 */
    {    0,   63,   63,    0 },
    /* s1 */
    {   63,    0,    0,  -63 },
    /* s2 */
    {    0,  -63,  -63,    0 },
    /* s3 */
    {  -63,    0,    0,   63 },
};
static const int unknown_enemy_seg_count =
    (int)(sizeof(unknown_enemy_segs) / sizeof(unknown_enemy_segs[0]));

/* --- unknown_enemy.vec 3D table ---
 * 4 unique verts, 4 paths, 8 total point refs */
static const vec3d_vert_t unknown_enemy_verts[] = {
    {    0,   63,    0 },
    {   63,    0,    0 },
    {    0,  -63,    0 },
    {  -63,    0,    0 },
};
static const int unknown_enemy_vert_count = 4;

static const uint8_t unknown_enemy_path0_idx[] = { 0, 1 };
static const uint8_t unknown_enemy_path1_idx[] = { 1, 2 };
static const uint8_t unknown_enemy_path2_idx[] = { 2, 3 };
static const uint8_t unknown_enemy_path3_idx[] = { 3, 0 };
static const vec3d_path_t unknown_enemy_paths[] = {
    { 2, 0, unknown_enemy_path0_idx },
    { 2, 0, unknown_enemy_path1_idx },
    { 2, 0, unknown_enemy_path2_idx },
    { 2, 0, unknown_enemy_path3_idx },
};
static const int unknown_enemy_path_count = 4;

/* --- unknown_pickup.vec --- */
static const vec_seg_t unknown_pickup_segs[] = {
    /* s0 */
    {  -63,    0,   63,    0 },
    /* s1 */
    {    0,  -63,    0,   63 },
};
static const int unknown_pickup_seg_count =
    (int)(sizeof(unknown_pickup_segs) / sizeof(unknown_pickup_segs[0]));

/* --- unknown_pickup.vec 3D table ---
 * 4 unique verts, 2 paths, 4 total point refs */
static const vec3d_vert_t unknown_pickup_verts[] = {
    {  -63,    0,    0 },
    {   63,    0,    0 },
    {    0,  -63,    0 },
    {    0,   63,    0 },
};
static const int unknown_pickup_vert_count = 4;

static const uint8_t unknown_pickup_path0_idx[] = { 0, 1 };
static const uint8_t unknown_pickup_path1_idx[] = { 2, 3 };
static const vec3d_path_t unknown_pickup_paths[] = {
    { 2, 0, unknown_pickup_path0_idx },
    { 2, 0, unknown_pickup_path1_idx },
};
static const int unknown_pickup_path_count = 2;

/* --- corpse.vec --- */
static const vec_seg_t corpse_segs[] = {
    /* head0 */
    {  -50,   20,  -38,   10 },
    /* head1 */
    {  -38,   10,  -50,    0 },
    /* head2 */
    {  -50,    0,  -62,   10 },
    /* head3 */
    {  -62,   10,  -50,   20 },
    /* neck */
    {  -38,   10,  -28,   12 },
    /* torso_top */
    {  -28,   18,   20,   16 },
    /* torso_bottom */
    {  -28,    4,   20,    6 },
    /* torso_left */
    {  -28,    4,  -28,   18 },
    /* torso_right */
    {   20,    6,   20,   16 },
    /* arm_up */
    {  -15,   18,  -20,   36 },
    /* arm_down */
    {  -15,    4,  -20,  -14 },
    /* leg_up */
    {   20,   16,   42,   26 },
    /* leg_down */
    {   20,    6,   42,   -4 },
};
static const int corpse_seg_count =
    (int)(sizeof(corpse_segs) / sizeof(corpse_segs[0]));

/* --- corpse.vec 3D table ---
 * 15 unique verts, 13 paths, 26 total point refs */
static const vec3d_vert_t corpse_verts[] = {
    {  -50,   20,    0 },
    {  -38,   10,    0 },
    {  -50,    0,    0 },
    {  -62,   10,    0 },
    {  -28,   12,    0 },
    {  -28,   18,    0 },
    {   20,   16,    0 },
    {  -28,    4,    0 },
    {   20,    6,    0 },
    {  -15,   18,    0 },
    {  -20,   36,    0 },
    {  -15,    4,    0 },
    {  -20,  -14,    0 },
    {   42,   26,    0 },
    {   42,   -4,    0 },
};
static const int corpse_vert_count = 15;

static const uint8_t corpse_path0_idx[] = { 0, 1 };
static const uint8_t corpse_path1_idx[] = { 1, 2 };
static const uint8_t corpse_path2_idx[] = { 2, 3 };
static const uint8_t corpse_path3_idx[] = { 3, 0 };
static const uint8_t corpse_path4_idx[] = { 1, 4 };
static const uint8_t corpse_path5_idx[] = { 5, 6 };
static const uint8_t corpse_path6_idx[] = { 7, 8 };
static const uint8_t corpse_path7_idx[] = { 7, 5 };
static const uint8_t corpse_path8_idx[] = { 8, 6 };
static const uint8_t corpse_path9_idx[] = { 9, 10 };
static const uint8_t corpse_path10_idx[] = { 11, 12 };
static const uint8_t corpse_path11_idx[] = { 6, 13 };
static const uint8_t corpse_path12_idx[] = { 8, 14 };
static const vec3d_path_t corpse_paths[] = {
    { 2, 0, corpse_path0_idx },
    { 2, 0, corpse_path1_idx },
    { 2, 0, corpse_path2_idx },
    { 2, 0, corpse_path3_idx },
    { 2, 0, corpse_path4_idx },
    { 2, 0, corpse_path5_idx },
    { 2, 0, corpse_path6_idx },
    { 2, 0, corpse_path7_idx },
    { 2, 0, corpse_path8_idx },
    { 2, 0, corpse_path9_idx },
    { 2, 0, corpse_path10_idx },
    { 2, 0, corpse_path11_idx },
    { 2, 0, corpse_path12_idx },
};
static const int corpse_path_count = 13;

/* --- decoration_pillar.vec --- */
static const vec_seg_t decoration_pillar_segs[] = {
    /* left */
    {  -15,  -58,  -15,   58 },
    /* right */
    {   15,  -58,   15,   58 },
    /* top_cap */
    {  -15,   58,   15,   58 },
    /* base */
    {  -15,  -58,   15,  -58 },
    /* band_upper */
    {  -15,   20,   15,   20 },
    /* band_lower */
    {  -15,  -20,   15,  -20 },
};
static const int decoration_pillar_seg_count =
    (int)(sizeof(decoration_pillar_segs) / sizeof(decoration_pillar_segs[0]));

/* --- decoration_pillar.vec 3D table ---
 * 8 unique verts, 6 paths, 12 total point refs */
static const vec3d_vert_t decoration_pillar_verts[] = {
    {  -15,  -58,    0 },
    {  -15,   58,    0 },
    {   15,  -58,    0 },
    {   15,   58,    0 },
    {  -15,   20,    0 },
    {   15,   20,    0 },
    {  -15,  -20,    0 },
    {   15,  -20,    0 },
};
static const int decoration_pillar_vert_count = 8;

static const uint8_t decoration_pillar_path0_idx[] = { 0, 1 };
static const uint8_t decoration_pillar_path1_idx[] = { 2, 3 };
static const uint8_t decoration_pillar_path2_idx[] = { 1, 3 };
static const uint8_t decoration_pillar_path3_idx[] = { 0, 2 };
static const uint8_t decoration_pillar_path4_idx[] = { 4, 5 };
static const uint8_t decoration_pillar_path5_idx[] = { 6, 7 };
static const vec3d_path_t decoration_pillar_paths[] = {
    { 2, 0, decoration_pillar_path0_idx },
    { 2, 0, decoration_pillar_path1_idx },
    { 2, 0, decoration_pillar_path2_idx },
    { 2, 0, decoration_pillar_path3_idx },
    { 2, 0, decoration_pillar_path4_idx },
    { 2, 0, decoration_pillar_path5_idx },
};
static const int decoration_pillar_path_count = 6;

/* --- barrel_explosion.vec --- */
static const vec_seg_t barrel_explosion_segs[] = {
    /* frag_up */
    {    0,   10,    0,   55 },
    /* frag_ur */
    {    8,    8,   42,   42 },
    /* frag_right */
    {   10,    0,   55,    0 },
    /* frag_dr */
    {    8,   -8,   38,  -38 },
    /* frag_down */
    {    0,  -10,    0,  -50 },
    /* frag_dl */
    {   -8,   -8,  -38,  -38 },
    /* frag_left */
    {  -10,    0,  -55,    0 },
    /* frag_ul */
    {   -8,    8,  -42,   42 },
    /* core_h */
    {   -8,    0,    8,    0 },
    /* core_v */
    {    0,   -8,    0,    8 },
    /* ring1 */
    {  -20,   20,   20,   20 },
    /* ring2 */
    {  -20,  -20,   20,  -20 },
};
static const int barrel_explosion_seg_count =
    (int)(sizeof(barrel_explosion_segs) / sizeof(barrel_explosion_segs[0]));

/* --- barrel_explosion.vec 3D table ---
 * 24 unique verts, 12 paths, 24 total point refs */
static const vec3d_vert_t barrel_explosion_verts[] = {
    {    0,   10,    0 },
    {    0,   55,    0 },
    {    8,    8,    0 },
    {   42,   42,    0 },
    {   10,    0,    0 },
    {   55,    0,    0 },
    {    8,   -8,    0 },
    {   38,  -38,    0 },
    {    0,  -10,    0 },
    {    0,  -50,    0 },
    {   -8,   -8,    0 },
    {  -38,  -38,    0 },
    {  -10,    0,    0 },
    {  -55,    0,    0 },
    {   -8,    8,    0 },
    {  -42,   42,    0 },
    {   -8,    0,    0 },
    {    8,    0,    0 },
    {    0,   -8,    0 },
    {    0,    8,    0 },
    {  -20,   20,    0 },
    {   20,   20,    0 },
    {  -20,  -20,    0 },
    {   20,  -20,    0 },
};
static const int barrel_explosion_vert_count = 24;

static const uint8_t barrel_explosion_path0_idx[] = { 0, 1 };
static const uint8_t barrel_explosion_path1_idx[] = { 2, 3 };
static const uint8_t barrel_explosion_path2_idx[] = { 4, 5 };
static const uint8_t barrel_explosion_path3_idx[] = { 6, 7 };
static const uint8_t barrel_explosion_path4_idx[] = { 8, 9 };
static const uint8_t barrel_explosion_path5_idx[] = { 10, 11 };
static const uint8_t barrel_explosion_path6_idx[] = { 12, 13 };
static const uint8_t barrel_explosion_path7_idx[] = { 14, 15 };
static const uint8_t barrel_explosion_path8_idx[] = { 16, 17 };
static const uint8_t barrel_explosion_path9_idx[] = { 18, 19 };
static const uint8_t barrel_explosion_path10_idx[] = { 20, 21 };
static const uint8_t barrel_explosion_path11_idx[] = { 22, 23 };
static const vec3d_path_t barrel_explosion_paths[] = {
    { 2, 0, barrel_explosion_path0_idx },
    { 2, 0, barrel_explosion_path1_idx },
    { 2, 0, barrel_explosion_path2_idx },
    { 2, 0, barrel_explosion_path3_idx },
    { 2, 0, barrel_explosion_path4_idx },
    { 2, 0, barrel_explosion_path5_idx },
    { 2, 0, barrel_explosion_path6_idx },
    { 2, 0, barrel_explosion_path7_idx },
    { 2, 0, barrel_explosion_path8_idx },
    { 2, 0, barrel_explosion_path9_idx },
    { 2, 0, barrel_explosion_path10_idx },
    { 2, 0, barrel_explosion_path11_idx },
};
static const int barrel_explosion_path_count = 12;

#endif /* SPRITES_ASSETS_H */
