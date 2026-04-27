/* Auto-generated from digit_0.vec, digit_1.vec, digit_2.vec, digit_3.vec, digit_4.vec, digit_5.vec, digit_6.vec, digit_7.vec, digit_8.vec, digit_9.vec, percent.vec, face_normal.vec, face_pain.vec, face_evil.vec, face_god.vec, face_dead.vec, key_card.vec, key_skull.vec
 * by tools/vec_to_c.py — 2026-04-18 — DO NOT EDIT */
#ifndef HUD_ASSETS_H
#define HUD_ASSETS_H

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

/* --- digit_0.vec --- */
static const vec_seg_t digit_0_segs[] = {
    /* s0 */
    {   -4,    7,    4,    7 },
    /* s1 */
    {    4,    0,    4,    7 },
    /* s2 */
    {    4,   -6,    4,    0 },
    /* s3 */
    {   -4,   -6,    4,   -6 },
    /* s4 */
    {   -4,   -6,   -4,    0 },
    /* s5 */
    {   -4,    0,   -4,    7 },
};
static const int digit_0_seg_count =
    (int)(sizeof(digit_0_segs) / sizeof(digit_0_segs[0]));

/* --- digit_0.vec 3D table ---
 * 6 unique verts, 6 paths, 12 total point refs */
static const vec3d_vert_t digit_0_verts[] = {
    {   -4,    7,    0 },
    {    4,    7,    0 },
    {    4,    0,    0 },
    {    4,   -6,    0 },
    {   -4,   -6,    0 },
    {   -4,    0,    0 },
};
static const int digit_0_vert_count = 6;

static const uint8_t digit_0_path0_idx[] = { 0, 1 };
static const uint8_t digit_0_path1_idx[] = { 2, 1 };
static const uint8_t digit_0_path2_idx[] = { 3, 2 };
static const uint8_t digit_0_path3_idx[] = { 4, 3 };
static const uint8_t digit_0_path4_idx[] = { 4, 5 };
static const uint8_t digit_0_path5_idx[] = { 5, 0 };
static const vec3d_path_t digit_0_paths[] = {
    { 2, 0, digit_0_path0_idx },
    { 2, 0, digit_0_path1_idx },
    { 2, 0, digit_0_path2_idx },
    { 2, 0, digit_0_path3_idx },
    { 2, 0, digit_0_path4_idx },
    { 2, 0, digit_0_path5_idx },
};
static const int digit_0_path_count = 6;

/* --- digit_1.vec --- */
static const vec_seg_t digit_1_segs[] = {
    /* s0 */
    {    4,    0,    4,    7 },
    /* s1 */
    {    4,   -6,    4,    0 },
};
static const int digit_1_seg_count =
    (int)(sizeof(digit_1_segs) / sizeof(digit_1_segs[0]));

/* --- digit_1.vec 3D table ---
 * 3 unique verts, 2 paths, 4 total point refs */
static const vec3d_vert_t digit_1_verts[] = {
    {    4,    0,    0 },
    {    4,    7,    0 },
    {    4,   -6,    0 },
};
static const int digit_1_vert_count = 3;

static const uint8_t digit_1_path0_idx[] = { 0, 1 };
static const uint8_t digit_1_path1_idx[] = { 2, 0 };
static const vec3d_path_t digit_1_paths[] = {
    { 2, 0, digit_1_path0_idx },
    { 2, 0, digit_1_path1_idx },
};
static const int digit_1_path_count = 2;

/* --- digit_2.vec --- */
static const vec_seg_t digit_2_segs[] = {
    /* s0 */
    {   -4,    7,    4,    7 },
    /* s1 */
    {    4,    0,    4,    7 },
    /* s2 */
    {   -4,   -6,    4,   -6 },
    /* s3 */
    {   -4,   -6,   -4,    0 },
    /* s4 */
    {   -4,    0,    4,    0 },
};
static const int digit_2_seg_count =
    (int)(sizeof(digit_2_segs) / sizeof(digit_2_segs[0]));

/* --- digit_2.vec 3D table ---
 * 6 unique verts, 5 paths, 10 total point refs */
static const vec3d_vert_t digit_2_verts[] = {
    {   -4,    7,    0 },
    {    4,    7,    0 },
    {    4,    0,    0 },
    {   -4,   -6,    0 },
    {    4,   -6,    0 },
    {   -4,    0,    0 },
};
static const int digit_2_vert_count = 6;

static const uint8_t digit_2_path0_idx[] = { 0, 1 };
static const uint8_t digit_2_path1_idx[] = { 2, 1 };
static const uint8_t digit_2_path2_idx[] = { 3, 4 };
static const uint8_t digit_2_path3_idx[] = { 3, 5 };
static const uint8_t digit_2_path4_idx[] = { 5, 2 };
static const vec3d_path_t digit_2_paths[] = {
    { 2, 0, digit_2_path0_idx },
    { 2, 0, digit_2_path1_idx },
    { 2, 0, digit_2_path2_idx },
    { 2, 0, digit_2_path3_idx },
    { 2, 0, digit_2_path4_idx },
};
static const int digit_2_path_count = 5;

/* --- digit_3.vec --- */
static const vec_seg_t digit_3_segs[] = {
    /* s0 */
    {   -4,    7,    4,    7 },
    /* s1 */
    {    4,    0,    4,    7 },
    /* s2 */
    {    4,   -6,    4,    0 },
    /* s3 */
    {   -4,   -6,    4,   -6 },
    /* s4 */
    {   -4,    0,    4,    0 },
};
static const int digit_3_seg_count =
    (int)(sizeof(digit_3_segs) / sizeof(digit_3_segs[0]));

/* --- digit_3.vec 3D table ---
 * 6 unique verts, 5 paths, 10 total point refs */
static const vec3d_vert_t digit_3_verts[] = {
    {   -4,    7,    0 },
    {    4,    7,    0 },
    {    4,    0,    0 },
    {    4,   -6,    0 },
    {   -4,   -6,    0 },
    {   -4,    0,    0 },
};
static const int digit_3_vert_count = 6;

static const uint8_t digit_3_path0_idx[] = { 0, 1 };
static const uint8_t digit_3_path1_idx[] = { 2, 1 };
static const uint8_t digit_3_path2_idx[] = { 3, 2 };
static const uint8_t digit_3_path3_idx[] = { 4, 3 };
static const uint8_t digit_3_path4_idx[] = { 5, 2 };
static const vec3d_path_t digit_3_paths[] = {
    { 2, 0, digit_3_path0_idx },
    { 2, 0, digit_3_path1_idx },
    { 2, 0, digit_3_path2_idx },
    { 2, 0, digit_3_path3_idx },
    { 2, 0, digit_3_path4_idx },
};
static const int digit_3_path_count = 5;

/* --- digit_4.vec --- */
static const vec_seg_t digit_4_segs[] = {
    /* s0 */
    {    4,    0,    4,    7 },
    /* s1 */
    {    4,   -6,    4,    0 },
    /* s2 */
    {   -4,    0,   -4,    7 },
    /* s3 */
    {   -4,    0,    4,    0 },
};
static const int digit_4_seg_count =
    (int)(sizeof(digit_4_segs) / sizeof(digit_4_segs[0]));

/* --- digit_4.vec 3D table ---
 * 5 unique verts, 4 paths, 8 total point refs */
static const vec3d_vert_t digit_4_verts[] = {
    {    4,    0,    0 },
    {    4,    7,    0 },
    {    4,   -6,    0 },
    {   -4,    0,    0 },
    {   -4,    7,    0 },
};
static const int digit_4_vert_count = 5;

static const uint8_t digit_4_path0_idx[] = { 0, 1 };
static const uint8_t digit_4_path1_idx[] = { 2, 0 };
static const uint8_t digit_4_path2_idx[] = { 3, 4 };
static const uint8_t digit_4_path3_idx[] = { 3, 0 };
static const vec3d_path_t digit_4_paths[] = {
    { 2, 0, digit_4_path0_idx },
    { 2, 0, digit_4_path1_idx },
    { 2, 0, digit_4_path2_idx },
    { 2, 0, digit_4_path3_idx },
};
static const int digit_4_path_count = 4;

/* --- digit_5.vec --- */
static const vec_seg_t digit_5_segs[] = {
    /* s0 */
    {   -4,    7,    4,    7 },
    /* s1 */
    {    4,   -6,    4,    0 },
    /* s2 */
    {   -4,   -6,    4,   -6 },
    /* s3 */
    {   -4,    0,   -4,    7 },
    /* s4 */
    {   -4,    0,    4,    0 },
};
static const int digit_5_seg_count =
    (int)(sizeof(digit_5_segs) / sizeof(digit_5_segs[0]));

/* --- digit_5.vec 3D table ---
 * 6 unique verts, 5 paths, 10 total point refs */
static const vec3d_vert_t digit_5_verts[] = {
    {   -4,    7,    0 },
    {    4,    7,    0 },
    {    4,   -6,    0 },
    {    4,    0,    0 },
    {   -4,   -6,    0 },
    {   -4,    0,    0 },
};
static const int digit_5_vert_count = 6;

static const uint8_t digit_5_path0_idx[] = { 0, 1 };
static const uint8_t digit_5_path1_idx[] = { 2, 3 };
static const uint8_t digit_5_path2_idx[] = { 4, 2 };
static const uint8_t digit_5_path3_idx[] = { 5, 0 };
static const uint8_t digit_5_path4_idx[] = { 5, 3 };
static const vec3d_path_t digit_5_paths[] = {
    { 2, 0, digit_5_path0_idx },
    { 2, 0, digit_5_path1_idx },
    { 2, 0, digit_5_path2_idx },
    { 2, 0, digit_5_path3_idx },
    { 2, 0, digit_5_path4_idx },
};
static const int digit_5_path_count = 5;

/* --- digit_6.vec --- */
static const vec_seg_t digit_6_segs[] = {
    /* s0 */
    {   -4,    7,    4,    7 },
    /* s1 */
    {    4,   -6,    4,    0 },
    /* s2 */
    {   -4,   -6,    4,   -6 },
    /* s3 */
    {   -4,   -6,   -4,    0 },
    /* s4 */
    {   -4,    0,   -4,    7 },
    /* s5 */
    {   -4,    0,    4,    0 },
};
static const int digit_6_seg_count =
    (int)(sizeof(digit_6_segs) / sizeof(digit_6_segs[0]));

/* --- digit_6.vec 3D table ---
 * 6 unique verts, 6 paths, 12 total point refs */
static const vec3d_vert_t digit_6_verts[] = {
    {   -4,    7,    0 },
    {    4,    7,    0 },
    {    4,   -6,    0 },
    {    4,    0,    0 },
    {   -4,   -6,    0 },
    {   -4,    0,    0 },
};
static const int digit_6_vert_count = 6;

static const uint8_t digit_6_path0_idx[] = { 0, 1 };
static const uint8_t digit_6_path1_idx[] = { 2, 3 };
static const uint8_t digit_6_path2_idx[] = { 4, 2 };
static const uint8_t digit_6_path3_idx[] = { 4, 5 };
static const uint8_t digit_6_path4_idx[] = { 5, 0 };
static const uint8_t digit_6_path5_idx[] = { 5, 3 };
static const vec3d_path_t digit_6_paths[] = {
    { 2, 0, digit_6_path0_idx },
    { 2, 0, digit_6_path1_idx },
    { 2, 0, digit_6_path2_idx },
    { 2, 0, digit_6_path3_idx },
    { 2, 0, digit_6_path4_idx },
    { 2, 0, digit_6_path5_idx },
};
static const int digit_6_path_count = 6;

/* --- digit_7.vec --- */
static const vec_seg_t digit_7_segs[] = {
    /* s0 */
    {   -4,    7,    4,    7 },
    /* s1 */
    {    4,    0,    4,    7 },
    /* s2 */
    {    4,   -6,    4,    0 },
};
static const int digit_7_seg_count =
    (int)(sizeof(digit_7_segs) / sizeof(digit_7_segs[0]));

/* --- digit_7.vec 3D table ---
 * 4 unique verts, 3 paths, 6 total point refs */
static const vec3d_vert_t digit_7_verts[] = {
    {   -4,    7,    0 },
    {    4,    7,    0 },
    {    4,    0,    0 },
    {    4,   -6,    0 },
};
static const int digit_7_vert_count = 4;

static const uint8_t digit_7_path0_idx[] = { 0, 1 };
static const uint8_t digit_7_path1_idx[] = { 2, 1 };
static const uint8_t digit_7_path2_idx[] = { 3, 2 };
static const vec3d_path_t digit_7_paths[] = {
    { 2, 0, digit_7_path0_idx },
    { 2, 0, digit_7_path1_idx },
    { 2, 0, digit_7_path2_idx },
};
static const int digit_7_path_count = 3;

/* --- digit_8.vec --- */
static const vec_seg_t digit_8_segs[] = {
    /* s0 */
    {   -4,    7,    4,    7 },
    /* s1 */
    {    4,    0,    4,    7 },
    /* s2 */
    {    4,   -6,    4,    0 },
    /* s3 */
    {   -4,   -6,    4,   -6 },
    /* s4 */
    {   -4,   -6,   -4,    0 },
    /* s5 */
    {   -4,    0,   -4,    7 },
    /* s6 */
    {   -4,    0,    4,    0 },
};
static const int digit_8_seg_count =
    (int)(sizeof(digit_8_segs) / sizeof(digit_8_segs[0]));

/* --- digit_8.vec 3D table ---
 * 6 unique verts, 7 paths, 14 total point refs */
static const vec3d_vert_t digit_8_verts[] = {
    {   -4,    7,    0 },
    {    4,    7,    0 },
    {    4,    0,    0 },
    {    4,   -6,    0 },
    {   -4,   -6,    0 },
    {   -4,    0,    0 },
};
static const int digit_8_vert_count = 6;

static const uint8_t digit_8_path0_idx[] = { 0, 1 };
static const uint8_t digit_8_path1_idx[] = { 2, 1 };
static const uint8_t digit_8_path2_idx[] = { 3, 2 };
static const uint8_t digit_8_path3_idx[] = { 4, 3 };
static const uint8_t digit_8_path4_idx[] = { 4, 5 };
static const uint8_t digit_8_path5_idx[] = { 5, 0 };
static const uint8_t digit_8_path6_idx[] = { 5, 2 };
static const vec3d_path_t digit_8_paths[] = {
    { 2, 0, digit_8_path0_idx },
    { 2, 0, digit_8_path1_idx },
    { 2, 0, digit_8_path2_idx },
    { 2, 0, digit_8_path3_idx },
    { 2, 0, digit_8_path4_idx },
    { 2, 0, digit_8_path5_idx },
    { 2, 0, digit_8_path6_idx },
};
static const int digit_8_path_count = 7;

/* --- digit_9.vec --- */
static const vec_seg_t digit_9_segs[] = {
    /* s0 */
    {   -4,    7,    4,    7 },
    /* s1 */
    {    4,    0,    4,    7 },
    /* s2 */
    {    4,   -6,    4,    0 },
    /* s3 */
    {   -4,   -6,    4,   -6 },
    /* s4 */
    {   -4,    0,   -4,    7 },
    /* s5 */
    {   -4,    0,    4,    0 },
};
static const int digit_9_seg_count =
    (int)(sizeof(digit_9_segs) / sizeof(digit_9_segs[0]));

/* --- digit_9.vec 3D table ---
 * 6 unique verts, 6 paths, 12 total point refs */
static const vec3d_vert_t digit_9_verts[] = {
    {   -4,    7,    0 },
    {    4,    7,    0 },
    {    4,    0,    0 },
    {    4,   -6,    0 },
    {   -4,   -6,    0 },
    {   -4,    0,    0 },
};
static const int digit_9_vert_count = 6;

static const uint8_t digit_9_path0_idx[] = { 0, 1 };
static const uint8_t digit_9_path1_idx[] = { 2, 1 };
static const uint8_t digit_9_path2_idx[] = { 3, 2 };
static const uint8_t digit_9_path3_idx[] = { 4, 3 };
static const uint8_t digit_9_path4_idx[] = { 5, 0 };
static const uint8_t digit_9_path5_idx[] = { 5, 2 };
static const vec3d_path_t digit_9_paths[] = {
    { 2, 0, digit_9_path0_idx },
    { 2, 0, digit_9_path1_idx },
    { 2, 0, digit_9_path2_idx },
    { 2, 0, digit_9_path3_idx },
    { 2, 0, digit_9_path4_idx },
    { 2, 0, digit_9_path5_idx },
};
static const int digit_9_path_count = 6;

/* --- percent.vec --- */
static const vec_seg_t percent_segs[] = {
    /* s0 */
    {   -3,    0,   -3,    3 },
    /* s1 */
    {   -3,    3,    0,    3 },
    /* s2 */
    {    0,    3,    0,    0 },
    /* s3 */
    {    0,    0,   -3,    0 },
    /* s4 */
    {    0,    0,    3,    0 },
    /* s5 */
    {    3,    0,    3,   -3 },
    /* s6 */
    {    3,   -3,    0,   -3 },
    /* s7 */
    {    0,   -3,    0,    0 },
    /* s8 */
    {    3,    3,   -3,   -3 },
};
static const int percent_seg_count =
    (int)(sizeof(percent_segs) / sizeof(percent_segs[0]));

/* --- percent.vec 3D table ---
 * 9 unique verts, 9 paths, 18 total point refs */
static const vec3d_vert_t percent_verts[] = {
    {   -3,    0,    0 },
    {   -3,    3,    0 },
    {    0,    3,    0 },
    {    0,    0,    0 },
    {    3,    0,    0 },
    {    3,   -3,    0 },
    {    0,   -3,    0 },
    {    3,    3,    0 },
    {   -3,   -3,    0 },
};
static const int percent_vert_count = 9;

static const uint8_t percent_path0_idx[] = { 0, 1 };
static const uint8_t percent_path1_idx[] = { 1, 2 };
static const uint8_t percent_path2_idx[] = { 2, 3 };
static const uint8_t percent_path3_idx[] = { 3, 0 };
static const uint8_t percent_path4_idx[] = { 3, 4 };
static const uint8_t percent_path5_idx[] = { 4, 5 };
static const uint8_t percent_path6_idx[] = { 5, 6 };
static const uint8_t percent_path7_idx[] = { 6, 3 };
static const uint8_t percent_path8_idx[] = { 7, 8 };
static const vec3d_path_t percent_paths[] = {
    { 2, 0, percent_path0_idx },
    { 2, 0, percent_path1_idx },
    { 2, 0, percent_path2_idx },
    { 2, 0, percent_path3_idx },
    { 2, 0, percent_path4_idx },
    { 2, 0, percent_path5_idx },
    { 2, 0, percent_path6_idx },
    { 2, 0, percent_path7_idx },
    { 2, 0, percent_path8_idx },
};
static const int percent_path_count = 9;

/* --- face_normal.vec --- */
static const vec_seg_t face_normal_segs[] = {
    /* s0 */
    {    0,    9,    7,    6 },
    /* s1 */
    {    7,    6,    9,    0 },
    /* s2 */
    {    9,    0,    7,   -6 },
    /* s3 */
    {    7,   -6,    0,   -9 },
    /* s4 */
    {    0,   -9,   -7,   -6 },
    /* s5 */
    {   -7,   -6,   -9,    0 },
    /* s6 */
    {   -9,    0,   -7,    6 },
    /* s7 */
    {   -7,    6,    0,    9 },
    /* s8 */
    {   -7,    6,    7,    6 },
    /* s9 */
    {   -8,    5,   -2,    5 },
    /* s10 */
    {   -2,    5,   -2,   -1 },
    /* s11 */
    {   -2,   -1,   -8,   -1 },
    /* s12 */
    {   -8,   -1,   -8,    5 },
    /* s13 */
    {    2,    5,    8,    5 },
    /* s14 */
    {    8,    5,    8,   -1 },
    /* s15 */
    {    8,   -1,    2,   -1 },
    /* s16 */
    {    2,   -1,    2,    5 },
    /* s17 */
    {   -5,   -5,    5,   -5 },
};
static const int face_normal_seg_count =
    (int)(sizeof(face_normal_segs) / sizeof(face_normal_segs[0]));

/* --- face_normal.vec 3D table ---
 * 18 unique verts, 18 paths, 36 total point refs */
static const vec3d_vert_t face_normal_verts[] = {
    {    0,    9,    0 },
    {    7,    6,    0 },
    {    9,    0,    0 },
    {    7,   -6,    0 },
    {    0,   -9,    0 },
    {   -7,   -6,    0 },
    {   -9,    0,    0 },
    {   -7,    6,    0 },
    {   -8,    5,    0 },
    {   -2,    5,    0 },
    {   -2,   -1,    0 },
    {   -8,   -1,    0 },
    {    2,    5,    0 },
    {    8,    5,    0 },
    {    8,   -1,    0 },
    {    2,   -1,    0 },
    {   -5,   -5,    0 },
    {    5,   -5,    0 },
};
static const int face_normal_vert_count = 18;

static const uint8_t face_normal_path0_idx[] = { 0, 1 };
static const uint8_t face_normal_path1_idx[] = { 1, 2 };
static const uint8_t face_normal_path2_idx[] = { 2, 3 };
static const uint8_t face_normal_path3_idx[] = { 3, 4 };
static const uint8_t face_normal_path4_idx[] = { 4, 5 };
static const uint8_t face_normal_path5_idx[] = { 5, 6 };
static const uint8_t face_normal_path6_idx[] = { 6, 7 };
static const uint8_t face_normal_path7_idx[] = { 7, 0 };
static const uint8_t face_normal_path8_idx[] = { 7, 1 };
static const uint8_t face_normal_path9_idx[] = { 8, 9 };
static const uint8_t face_normal_path10_idx[] = { 9, 10 };
static const uint8_t face_normal_path11_idx[] = { 10, 11 };
static const uint8_t face_normal_path12_idx[] = { 11, 8 };
static const uint8_t face_normal_path13_idx[] = { 12, 13 };
static const uint8_t face_normal_path14_idx[] = { 13, 14 };
static const uint8_t face_normal_path15_idx[] = { 14, 15 };
static const uint8_t face_normal_path16_idx[] = { 15, 12 };
static const uint8_t face_normal_path17_idx[] = { 16, 17 };
static const vec3d_path_t face_normal_paths[] = {
    { 2, 0, face_normal_path0_idx },
    { 2, 0, face_normal_path1_idx },
    { 2, 0, face_normal_path2_idx },
    { 2, 0, face_normal_path3_idx },
    { 2, 0, face_normal_path4_idx },
    { 2, 0, face_normal_path5_idx },
    { 2, 0, face_normal_path6_idx },
    { 2, 0, face_normal_path7_idx },
    { 2, 0, face_normal_path8_idx },
    { 2, 0, face_normal_path9_idx },
    { 2, 0, face_normal_path10_idx },
    { 2, 0, face_normal_path11_idx },
    { 2, 0, face_normal_path12_idx },
    { 2, 0, face_normal_path13_idx },
    { 2, 0, face_normal_path14_idx },
    { 2, 0, face_normal_path15_idx },
    { 2, 0, face_normal_path16_idx },
    { 2, 0, face_normal_path17_idx },
};
static const int face_normal_path_count = 18;

/* --- face_pain.vec --- */
static const vec_seg_t face_pain_segs[] = {
    /* s0 */
    {    0,    9,    7,    6 },
    /* s1 */
    {    7,    6,    9,    0 },
    /* s2 */
    {    9,    0,    7,   -6 },
    /* s3 */
    {    7,   -6,    0,   -9 },
    /* s4 */
    {    0,   -9,   -7,   -6 },
    /* s5 */
    {   -7,   -6,   -9,    0 },
    /* s6 */
    {   -9,    0,   -7,    6 },
    /* s7 */
    {   -7,    6,    0,    9 },
    /* s8 */
    {   -7,    6,    7,    6 },
    /* s9 */
    {   -9,    7,   -1,    7 },
    /* s10 */
    {   -1,    7,   -1,   -3 },
    /* s11 */
    {   -1,   -3,   -9,   -3 },
    /* s12 */
    {   -9,   -3,   -9,    7 },
    /* s13 */
    {    1,    7,    9,    7 },
    /* s14 */
    {    9,    7,    9,   -3 },
    /* s15 */
    {    9,   -3,    1,   -3 },
    /* s16 */
    {    1,   -3,    1,    7 },
    /* s17 */
    {   -4,   -5,    4,   -5 },
    /* s18 */
    {    4,   -5,    4,   -8 },
    /* s19 */
    {    4,   -8,   -4,   -8 },
    /* s20 */
    {   -4,   -8,   -4,   -5 },
};
static const int face_pain_seg_count =
    (int)(sizeof(face_pain_segs) / sizeof(face_pain_segs[0]));

/* --- face_pain.vec 3D table ---
 * 20 unique verts, 21 paths, 42 total point refs */
static const vec3d_vert_t face_pain_verts[] = {
    {    0,    9,    0 },
    {    7,    6,    0 },
    {    9,    0,    0 },
    {    7,   -6,    0 },
    {    0,   -9,    0 },
    {   -7,   -6,    0 },
    {   -9,    0,    0 },
    {   -7,    6,    0 },
    {   -9,    7,    0 },
    {   -1,    7,    0 },
    {   -1,   -3,    0 },
    {   -9,   -3,    0 },
    {    1,    7,    0 },
    {    9,    7,    0 },
    {    9,   -3,    0 },
    {    1,   -3,    0 },
    {   -4,   -5,    0 },
    {    4,   -5,    0 },
    {    4,   -8,    0 },
    {   -4,   -8,    0 },
};
static const int face_pain_vert_count = 20;

static const uint8_t face_pain_path0_idx[] = { 0, 1 };
static const uint8_t face_pain_path1_idx[] = { 1, 2 };
static const uint8_t face_pain_path2_idx[] = { 2, 3 };
static const uint8_t face_pain_path3_idx[] = { 3, 4 };
static const uint8_t face_pain_path4_idx[] = { 4, 5 };
static const uint8_t face_pain_path5_idx[] = { 5, 6 };
static const uint8_t face_pain_path6_idx[] = { 6, 7 };
static const uint8_t face_pain_path7_idx[] = { 7, 0 };
static const uint8_t face_pain_path8_idx[] = { 7, 1 };
static const uint8_t face_pain_path9_idx[] = { 8, 9 };
static const uint8_t face_pain_path10_idx[] = { 9, 10 };
static const uint8_t face_pain_path11_idx[] = { 10, 11 };
static const uint8_t face_pain_path12_idx[] = { 11, 8 };
static const uint8_t face_pain_path13_idx[] = { 12, 13 };
static const uint8_t face_pain_path14_idx[] = { 13, 14 };
static const uint8_t face_pain_path15_idx[] = { 14, 15 };
static const uint8_t face_pain_path16_idx[] = { 15, 12 };
static const uint8_t face_pain_path17_idx[] = { 16, 17 };
static const uint8_t face_pain_path18_idx[] = { 17, 18 };
static const uint8_t face_pain_path19_idx[] = { 18, 19 };
static const uint8_t face_pain_path20_idx[] = { 19, 16 };
static const vec3d_path_t face_pain_paths[] = {
    { 2, 0, face_pain_path0_idx },
    { 2, 0, face_pain_path1_idx },
    { 2, 0, face_pain_path2_idx },
    { 2, 0, face_pain_path3_idx },
    { 2, 0, face_pain_path4_idx },
    { 2, 0, face_pain_path5_idx },
    { 2, 0, face_pain_path6_idx },
    { 2, 0, face_pain_path7_idx },
    { 2, 0, face_pain_path8_idx },
    { 2, 0, face_pain_path9_idx },
    { 2, 0, face_pain_path10_idx },
    { 2, 0, face_pain_path11_idx },
    { 2, 0, face_pain_path12_idx },
    { 2, 0, face_pain_path13_idx },
    { 2, 0, face_pain_path14_idx },
    { 2, 0, face_pain_path15_idx },
    { 2, 0, face_pain_path16_idx },
    { 2, 0, face_pain_path17_idx },
    { 2, 0, face_pain_path18_idx },
    { 2, 0, face_pain_path19_idx },
    { 2, 0, face_pain_path20_idx },
};
static const int face_pain_path_count = 21;

/* --- face_evil.vec --- */
static const vec_seg_t face_evil_segs[] = {
    /* s0 */
    {    0,    9,    7,    6 },
    /* s1 */
    {    7,    6,    9,    0 },
    /* s2 */
    {    9,    0,    7,   -6 },
    /* s3 */
    {    7,   -6,    0,   -9 },
    /* s4 */
    {    0,   -9,   -7,   -6 },
    /* s5 */
    {   -7,   -6,   -9,    0 },
    /* s6 */
    {   -9,    0,   -7,    6 },
    /* s7 */
    {   -7,    6,    0,    9 },
    /* s8 */
    {   -7,    6,    7,    6 },
    /* s9 */
    {   -8,    2,   -2,    2 },
    /* s10 */
    {    2,    2,    8,    2 },
    /* s11 */
    {   -8,    6,   -2,    3 },
    /* s12 */
    {    2,    3,    8,    6 },
    /* s13 */
    {   -6,   -4,   -3,   -6 },
    /* s14 */
    {   -3,   -6,    3,   -6 },
    /* s15 */
    {    3,   -6,    6,   -4 },
};
static const int face_evil_seg_count =
    (int)(sizeof(face_evil_segs) / sizeof(face_evil_segs[0]));

/* --- face_evil.vec 3D table ---
 * 20 unique verts, 16 paths, 32 total point refs */
static const vec3d_vert_t face_evil_verts[] = {
    {    0,    9,    0 },
    {    7,    6,    0 },
    {    9,    0,    0 },
    {    7,   -6,    0 },
    {    0,   -9,    0 },
    {   -7,   -6,    0 },
    {   -9,    0,    0 },
    {   -7,    6,    0 },
    {   -8,    2,    0 },
    {   -2,    2,    0 },
    {    2,    2,    0 },
    {    8,    2,    0 },
    {   -8,    6,    0 },
    {   -2,    3,    0 },
    {    2,    3,    0 },
    {    8,    6,    0 },
    {   -6,   -4,    0 },
    {   -3,   -6,    0 },
    {    3,   -6,    0 },
    {    6,   -4,    0 },
};
static const int face_evil_vert_count = 20;

static const uint8_t face_evil_path0_idx[] = { 0, 1 };
static const uint8_t face_evil_path1_idx[] = { 1, 2 };
static const uint8_t face_evil_path2_idx[] = { 2, 3 };
static const uint8_t face_evil_path3_idx[] = { 3, 4 };
static const uint8_t face_evil_path4_idx[] = { 4, 5 };
static const uint8_t face_evil_path5_idx[] = { 5, 6 };
static const uint8_t face_evil_path6_idx[] = { 6, 7 };
static const uint8_t face_evil_path7_idx[] = { 7, 0 };
static const uint8_t face_evil_path8_idx[] = { 7, 1 };
static const uint8_t face_evil_path9_idx[] = { 8, 9 };
static const uint8_t face_evil_path10_idx[] = { 10, 11 };
static const uint8_t face_evil_path11_idx[] = { 12, 13 };
static const uint8_t face_evil_path12_idx[] = { 14, 15 };
static const uint8_t face_evil_path13_idx[] = { 16, 17 };
static const uint8_t face_evil_path14_idx[] = { 17, 18 };
static const uint8_t face_evil_path15_idx[] = { 18, 19 };
static const vec3d_path_t face_evil_paths[] = {
    { 2, 0, face_evil_path0_idx },
    { 2, 0, face_evil_path1_idx },
    { 2, 0, face_evil_path2_idx },
    { 2, 0, face_evil_path3_idx },
    { 2, 0, face_evil_path4_idx },
    { 2, 0, face_evil_path5_idx },
    { 2, 0, face_evil_path6_idx },
    { 2, 0, face_evil_path7_idx },
    { 2, 0, face_evil_path8_idx },
    { 2, 0, face_evil_path9_idx },
    { 2, 0, face_evil_path10_idx },
    { 2, 0, face_evil_path11_idx },
    { 2, 0, face_evil_path12_idx },
    { 2, 0, face_evil_path13_idx },
    { 2, 0, face_evil_path14_idx },
    { 2, 0, face_evil_path15_idx },
};
static const int face_evil_path_count = 16;

/* --- face_god.vec --- */
static const vec_seg_t face_god_segs[] = {
    /* s0 */
    {    0,    9,    7,    6 },
    /* s1 */
    {    7,    6,    9,    0 },
    /* s2 */
    {    9,    0,    7,   -6 },
    /* s3 */
    {    7,   -6,    0,   -9 },
    /* s4 */
    {    0,   -9,   -7,   -6 },
    /* s5 */
    {   -7,   -6,   -9,    0 },
    /* s6 */
    {   -9,    0,   -7,    6 },
    /* s7 */
    {   -7,    6,    0,    9 },
    /* s8 */
    {   -7,    6,    7,    6 },
    /* s9 */
    {   -8,    5,   -2,    5 },
    /* s10 */
    {   -2,    5,   -2,   -1 },
    /* s11 */
    {   -2,   -1,   -8,   -1 },
    /* s12 */
    {   -8,   -1,   -8,    5 },
    /* s13 */
    {    2,    5,    8,    5 },
    /* s14 */
    {    8,    5,    8,   -1 },
    /* s15 */
    {    8,   -1,    2,   -1 },
    /* s16 */
    {    2,   -1,    2,    5 },
    /* s17 */
    {   -5,   -5,    5,   -5 },
    /* s18 */
    {   -3,    9,   -5,   12 },
    /* s19 */
    {    0,    9,    0,   13 },
    /* s20 */
    {    3,    9,    5,   12 },
};
static const int face_god_seg_count =
    (int)(sizeof(face_god_segs) / sizeof(face_god_segs[0]));

/* --- face_god.vec 3D table ---
 * 23 unique verts, 21 paths, 42 total point refs */
static const vec3d_vert_t face_god_verts[] = {
    {    0,    9,    0 },
    {    7,    6,    0 },
    {    9,    0,    0 },
    {    7,   -6,    0 },
    {    0,   -9,    0 },
    {   -7,   -6,    0 },
    {   -9,    0,    0 },
    {   -7,    6,    0 },
    {   -8,    5,    0 },
    {   -2,    5,    0 },
    {   -2,   -1,    0 },
    {   -8,   -1,    0 },
    {    2,    5,    0 },
    {    8,    5,    0 },
    {    8,   -1,    0 },
    {    2,   -1,    0 },
    {   -5,   -5,    0 },
    {    5,   -5,    0 },
    {   -3,    9,    0 },
    {   -5,   12,    0 },
    {    0,   13,    0 },
    {    3,    9,    0 },
    {    5,   12,    0 },
};
static const int face_god_vert_count = 23;

static const uint8_t face_god_path0_idx[] = { 0, 1 };
static const uint8_t face_god_path1_idx[] = { 1, 2 };
static const uint8_t face_god_path2_idx[] = { 2, 3 };
static const uint8_t face_god_path3_idx[] = { 3, 4 };
static const uint8_t face_god_path4_idx[] = { 4, 5 };
static const uint8_t face_god_path5_idx[] = { 5, 6 };
static const uint8_t face_god_path6_idx[] = { 6, 7 };
static const uint8_t face_god_path7_idx[] = { 7, 0 };
static const uint8_t face_god_path8_idx[] = { 7, 1 };
static const uint8_t face_god_path9_idx[] = { 8, 9 };
static const uint8_t face_god_path10_idx[] = { 9, 10 };
static const uint8_t face_god_path11_idx[] = { 10, 11 };
static const uint8_t face_god_path12_idx[] = { 11, 8 };
static const uint8_t face_god_path13_idx[] = { 12, 13 };
static const uint8_t face_god_path14_idx[] = { 13, 14 };
static const uint8_t face_god_path15_idx[] = { 14, 15 };
static const uint8_t face_god_path16_idx[] = { 15, 12 };
static const uint8_t face_god_path17_idx[] = { 16, 17 };
static const uint8_t face_god_path18_idx[] = { 18, 19 };
static const uint8_t face_god_path19_idx[] = { 0, 20 };
static const uint8_t face_god_path20_idx[] = { 21, 22 };
static const vec3d_path_t face_god_paths[] = {
    { 2, 0, face_god_path0_idx },
    { 2, 0, face_god_path1_idx },
    { 2, 0, face_god_path2_idx },
    { 2, 0, face_god_path3_idx },
    { 2, 0, face_god_path4_idx },
    { 2, 0, face_god_path5_idx },
    { 2, 0, face_god_path6_idx },
    { 2, 0, face_god_path7_idx },
    { 2, 0, face_god_path8_idx },
    { 2, 0, face_god_path9_idx },
    { 2, 0, face_god_path10_idx },
    { 2, 0, face_god_path11_idx },
    { 2, 0, face_god_path12_idx },
    { 2, 0, face_god_path13_idx },
    { 2, 0, face_god_path14_idx },
    { 2, 0, face_god_path15_idx },
    { 2, 0, face_god_path16_idx },
    { 2, 0, face_god_path17_idx },
    { 2, 0, face_god_path18_idx },
    { 2, 0, face_god_path19_idx },
    { 2, 0, face_god_path20_idx },
};
static const int face_god_path_count = 21;

/* --- face_dead.vec --- */
static const vec_seg_t face_dead_segs[] = {
    /* s0 */
    {    0,    9,    7,    6 },
    /* s1 */
    {    7,    6,    9,    0 },
    /* s2 */
    {    9,    0,    7,   -6 },
    /* s3 */
    {    7,   -6,    0,   -9 },
    /* s4 */
    {    0,   -9,   -7,   -6 },
    /* s5 */
    {   -7,   -6,   -9,    0 },
    /* s6 */
    {   -9,    0,   -7,    6 },
    /* s7 */
    {   -7,    6,    0,    9 },
    /* s8 */
    {   -7,    6,    7,    6 },
    /* s9 */
    {   -8,    5,   -2,   -1 },
    /* s10 */
    {   -8,   -1,   -2,    5 },
    /* s11 */
    {    2,    5,    8,   -1 },
    /* s12 */
    {    2,   -1,    8,    5 },
    /* s13 */
    {   -5,   -7,   -2,   -5 },
    /* s14 */
    {   -2,   -5,    2,   -5 },
    /* s15 */
    {    2,   -5,    5,   -7 },
};
static const int face_dead_seg_count =
    (int)(sizeof(face_dead_segs) / sizeof(face_dead_segs[0]));

/* --- face_dead.vec 3D table ---
 * 20 unique verts, 16 paths, 32 total point refs */
static const vec3d_vert_t face_dead_verts[] = {
    {    0,    9,    0 },
    {    7,    6,    0 },
    {    9,    0,    0 },
    {    7,   -6,    0 },
    {    0,   -9,    0 },
    {   -7,   -6,    0 },
    {   -9,    0,    0 },
    {   -7,    6,    0 },
    {   -8,    5,    0 },
    {   -2,   -1,    0 },
    {   -8,   -1,    0 },
    {   -2,    5,    0 },
    {    2,    5,    0 },
    {    8,   -1,    0 },
    {    2,   -1,    0 },
    {    8,    5,    0 },
    {   -5,   -7,    0 },
    {   -2,   -5,    0 },
    {    2,   -5,    0 },
    {    5,   -7,    0 },
};
static const int face_dead_vert_count = 20;

static const uint8_t face_dead_path0_idx[] = { 0, 1 };
static const uint8_t face_dead_path1_idx[] = { 1, 2 };
static const uint8_t face_dead_path2_idx[] = { 2, 3 };
static const uint8_t face_dead_path3_idx[] = { 3, 4 };
static const uint8_t face_dead_path4_idx[] = { 4, 5 };
static const uint8_t face_dead_path5_idx[] = { 5, 6 };
static const uint8_t face_dead_path6_idx[] = { 6, 7 };
static const uint8_t face_dead_path7_idx[] = { 7, 0 };
static const uint8_t face_dead_path8_idx[] = { 7, 1 };
static const uint8_t face_dead_path9_idx[] = { 8, 9 };
static const uint8_t face_dead_path10_idx[] = { 10, 11 };
static const uint8_t face_dead_path11_idx[] = { 12, 13 };
static const uint8_t face_dead_path12_idx[] = { 14, 15 };
static const uint8_t face_dead_path13_idx[] = { 16, 17 };
static const uint8_t face_dead_path14_idx[] = { 17, 18 };
static const uint8_t face_dead_path15_idx[] = { 18, 19 };
static const vec3d_path_t face_dead_paths[] = {
    { 2, 0, face_dead_path0_idx },
    { 2, 0, face_dead_path1_idx },
    { 2, 0, face_dead_path2_idx },
    { 2, 0, face_dead_path3_idx },
    { 2, 0, face_dead_path4_idx },
    { 2, 0, face_dead_path5_idx },
    { 2, 0, face_dead_path6_idx },
    { 2, 0, face_dead_path7_idx },
    { 2, 0, face_dead_path8_idx },
    { 2, 0, face_dead_path9_idx },
    { 2, 0, face_dead_path10_idx },
    { 2, 0, face_dead_path11_idx },
    { 2, 0, face_dead_path12_idx },
    { 2, 0, face_dead_path13_idx },
    { 2, 0, face_dead_path14_idx },
    { 2, 0, face_dead_path15_idx },
};
static const int face_dead_path_count = 16;

/* --- key_card.vec --- */
static const vec_seg_t key_card_segs[] = {
    /* body */
    {   -9,   -4,    9,   -4 },
    {    9,   -4,    9,    4 },
    {    9,    4,   -9,    4 },
    {   -9,    4,   -9,   -4 },
    /* grip_indent */
    {   -9,   -2,   -6,   -2 },
    {   -6,   -2,   -6,    2 },
    {   -6,    2,   -9,    2 },
    {   -9,    2,   -9,   -2 },
    /* notch */
    {    4,   -4,    9,   -4 },
    {    9,   -4,    9,   -1 },
    {    9,   -1,    4,   -1 },
    {    4,   -1,    4,   -4 },
    /* detail */
    {   -6,   -4,   -6,    4 },
};
static const int key_card_seg_count =
    (int)(sizeof(key_card_segs) / sizeof(key_card_segs[0]));

/* --- key_card.vec 3D table ---
 * 13 unique verts, 4 paths, 14 total point refs */
static const vec3d_vert_t key_card_verts[] = {
    {   -9,   -4,    0 },
    {    9,   -4,    0 },
    {    9,    4,    0 },
    {   -9,    4,    0 },
    {   -9,   -2,    0 },
    {   -6,   -2,    0 },
    {   -6,    2,    0 },
    {   -9,    2,    0 },
    {    4,   -4,    0 },
    {    9,   -1,    0 },
    {    4,   -1,    0 },
    {   -6,   -4,    0 },
    {   -6,    4,    0 },
};
static const int key_card_vert_count = 13;

static const uint8_t key_card_path0_idx[] = { 0, 1, 2, 3 };
static const uint8_t key_card_path1_idx[] = { 4, 5, 6, 7 };
static const uint8_t key_card_path2_idx[] = { 8, 1, 9, 10 };
static const uint8_t key_card_path3_idx[] = { 11, 12 };
static const vec3d_path_t key_card_paths[] = {
    { 4, 1, key_card_path0_idx },
    { 4, 1, key_card_path1_idx },
    { 4, 1, key_card_path2_idx },
    { 2, 0, key_card_path3_idx },
};
static const int key_card_path_count = 4;

/* --- key_skull.vec --- */
static const vec_seg_t key_skull_segs[] = {
    /* skull */
    {   -6,    9,    0,   11 },
    {    0,   11,    6,    9 },
    {    6,    9,    7,    4 },
    {    7,    4,    6,   -1 },
    {    6,   -1,    5,   -3 },
    {    5,   -3,   -5,   -3 },
    {   -5,   -3,   -6,   -1 },
    {   -6,   -1,   -7,    4 },
    {   -7,    4,   -6,    9 },
    /* eye_left */
    {   -5,    6,   -2,    6 },
    {   -2,    6,   -2,    3 },
    {   -2,    3,   -5,    3 },
    {   -5,    3,   -5,    6 },
    /* eye_right */
    {    2,    6,    5,    6 },
    {    5,    6,    5,    3 },
    {    5,    3,    2,    3 },
    {    2,    3,    2,    6 },
    /* jaw */
    {   -5,   -3,    5,   -3 },
    {    5,   -3,    5,   -8 },
    {    5,   -8,   -5,   -8 },
    {   -5,   -8,   -5,   -3 },
    /* teeth_div1 */
    {   -2,   -3,   -2,   -8 },
    /* teeth_div2 */
    {    2,   -3,    2,   -8 },
    /* grip */
    {   -2,   -8,    2,   -8 },
    {    2,   -8,    2,  -13 },
    {    2,  -13,   -2,  -13 },
    {   -2,  -13,   -2,   -8 },
};
static const int key_skull_seg_count =
    (int)(sizeof(key_skull_segs) / sizeof(key_skull_segs[0]));

/* --- key_skull.vec 3D table ---
 * 25 unique verts, 7 paths, 29 total point refs */
static const vec3d_vert_t key_skull_verts[] = {
    {   -6,    9,    0 },
    {    0,   11,    0 },
    {    6,    9,    0 },
    {    7,    4,    0 },
    {    6,   -1,    0 },
    {    5,   -3,    0 },
    {   -5,   -3,    0 },
    {   -6,   -1,    0 },
    {   -7,    4,    0 },
    {   -5,    6,    0 },
    {   -2,    6,    0 },
    {   -2,    3,    0 },
    {   -5,    3,    0 },
    {    2,    6,    0 },
    {    5,    6,    0 },
    {    5,    3,    0 },
    {    2,    3,    0 },
    {    5,   -8,    0 },
    {   -5,   -8,    0 },
    {   -2,   -3,    0 },
    {   -2,   -8,    0 },
    {    2,   -3,    0 },
    {    2,   -8,    0 },
    {    2,  -13,    0 },
    {   -2,  -13,    0 },
};
static const int key_skull_vert_count = 25;

static const uint8_t key_skull_path0_idx[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
static const uint8_t key_skull_path1_idx[] = { 9, 10, 11, 12 };
static const uint8_t key_skull_path2_idx[] = { 13, 14, 15, 16 };
static const uint8_t key_skull_path3_idx[] = { 6, 5, 17, 18 };
static const uint8_t key_skull_path4_idx[] = { 19, 20 };
static const uint8_t key_skull_path5_idx[] = { 21, 22 };
static const uint8_t key_skull_path6_idx[] = { 20, 22, 23, 24 };
static const vec3d_path_t key_skull_paths[] = {
    { 9, 1, key_skull_path0_idx },
    { 4, 1, key_skull_path1_idx },
    { 4, 1, key_skull_path2_idx },
    { 4, 1, key_skull_path3_idx },
    { 2, 0, key_skull_path4_idx },
    { 2, 0, key_skull_path5_idx },
    { 4, 1, key_skull_path6_idx },
};
static const int key_skull_path_count = 7;

#endif /* HUD_ASSETS_H */
