/* Auto-generated from doom_logo.vec
 * by tools/vec_to_c.py — 2026-04-24 — DO NOT EDIT */
#ifndef DOOM_LOGO_H
#define DOOM_LOGO_H

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

/* --- doom_logo.vec --- */
static const vec_seg_t doom_logo_segs[] = {
    /* dxf_0 */
    {   95,  -50,   95,   -5 },
    {   95,   -5,   88,  -30 },
    {   88,  -30,   81,   -5 },
    {   81,   -5,   81,  -39 },
    {   81,  -39,   58,  -22 },
    {   58,  -22,   60,  -20 },
    {   60,  -20,   60,   66 },
    {   60,   66,   81,   66 },
    {   81,   66,   88,   43 },
    {   88,   43,   95,   66 },
    {   95,   66,  127,   66 },
    {  127,   66,  116,   58 },
    {  116,   58,  116,  -66 },
    {  116,  -66,   95,  -50 },
    /* dxf_1 */
    { -116,   58, -127,   66 },
    { -127,   66,  -66,   66 },
    {  -66,   66,  -59,   60 },
    {  -59,   60,  -59,  -20 },
    {  -59,  -20,  -58,  -22 },
    {  -58,  -22, -116,  -66 },
    { -116,  -66, -116,   58 },
    /* dxf_2 */
    {  -81,   -5,  -81,   41 },
    {  -81,   41,  -82,   43 },
    {  -82,   43,  -95,   43 },
    {  -95,   43,  -95,  -16 },
    {  -95,  -16,  -81,   -5 },
    /* dxf_3 */
    {  -58,  -18,  -58,   60 },
    {  -58,   60,  -51,   66 },
    {  -51,   66,   -8,   66 },
    {   -8,   66,   -1,   60 },
    {   -1,   60,   -1,  -30 },
    {   -1,  -30,  -21,  -45 },
    {  -21,  -45,  -58,  -18 },
    /* dxf_4 */
    {  -24,   43,  -35,   43 },
    {  -35,   43,  -36,   41 },
    {  -36,   41,  -36,   -5 },
    {  -36,   -5,  -22,  -16 },
    {  -22,  -16,  -22,   41 },
    {  -22,   41,  -24,   43 },
    /* dxf_5 */
    {    1,  -30,    1,   60 },
    {    1,   60,    8,   66 },
    {    8,   66,   51,   66 },
    {   51,   66,   58,   60 },
    {   58,   60,   58,  -18 },
    {   58,  -18,   21,  -45 },
    {   21,  -45,    1,  -30 },
    /* dxf_6 */
    {   37,   -5,   37,   41 },
    {   37,   41,   35,   43 },
    {   35,   43,   24,   43 },
    {   24,   43,   22,   41 },
    {   22,   41,   22,  -16 },
    {   22,  -16,   37,   -5 },
};
static const int doom_logo_seg_count =
    (int)(sizeof(doom_logo_segs) / sizeof(doom_logo_segs[0]));

/* --- doom_logo.vec 3D table ---
 * 47 unique verts, 7 paths, 52 total point refs */
static const vec3d_vert_t doom_logo_verts[] = {
    {   63,  -50,    0 },
    {   63,   -5,    0 },
    {   63,  -30,    0 },
    {   63,  -39,    0 },
    {   58,  -22,    0 },
    {   60,  -20,    0 },
    {   60,   63,    0 },
    {   63,   63,    0 },
    {   63,   43,    0 },
    {   63,   58,    0 },
    {   63,  -63,    0 },
    {  -63,   58,    0 },
    {  -63,   63,    0 },
    {  -59,   60,    0 },
    {  -59,  -20,    0 },
    {  -58,  -22,    0 },
    {  -63,  -63,    0 },
    {  -63,   -5,    0 },
    {  -63,   41,    0 },
    {  -63,   43,    0 },
    {  -63,  -16,    0 },
    {  -58,  -18,    0 },
    {  -58,   60,    0 },
    {  -51,   63,    0 },
    {   -8,   63,    0 },
    {   -1,   60,    0 },
    {   -1,  -30,    0 },
    {  -21,  -45,    0 },
    {  -24,   43,    0 },
    {  -35,   43,    0 },
    {  -36,   41,    0 },
    {  -36,   -5,    0 },
    {  -22,  -16,    0 },
    {  -22,   41,    0 },
    {    1,  -30,    0 },
    {    1,   60,    0 },
    {    8,   63,    0 },
    {   51,   63,    0 },
    {   58,   60,    0 },
    {   58,  -18,    0 },
    {   21,  -45,    0 },
    {   37,   -5,    0 },
    {   37,   41,    0 },
    {   35,   43,    0 },
    {   24,   43,    0 },
    {   22,   41,    0 },
    {   22,  -16,    0 },
};
static const int doom_logo_vert_count = 47;

static const uint8_t doom_logo_path0_idx[] = { 0, 1, 2, 1, 3, 4, 5, 6, 7, 8, 7, 7, 9, 10 };
static const uint8_t doom_logo_path1_idx[] = { 11, 12, 12, 13, 14, 15, 16 };
static const uint8_t doom_logo_path2_idx[] = { 17, 18, 19, 19, 20 };
static const uint8_t doom_logo_path3_idx[] = { 21, 22, 23, 24, 25, 26, 27 };
static const uint8_t doom_logo_path4_idx[] = { 28, 29, 30, 31, 32, 33 };
static const uint8_t doom_logo_path5_idx[] = { 34, 35, 36, 37, 38, 39, 40 };
static const uint8_t doom_logo_path6_idx[] = { 41, 42, 43, 44, 45, 46 };
static const vec3d_path_t doom_logo_paths[] = {
    { 14, 1, doom_logo_path0_idx },
    { 7, 1, doom_logo_path1_idx },
    { 5, 1, doom_logo_path2_idx },
    { 7, 1, doom_logo_path3_idx },
    { 6, 1, doom_logo_path4_idx },
    { 7, 1, doom_logo_path5_idx },
    { 6, 1, doom_logo_path6_idx },
};
static const int doom_logo_path_count = 7;

#endif /* DOOM_LOGO_H */
