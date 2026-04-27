/*
 * renderer.h — wireframe vector renderer for piTrex DOOM
 */

#pragma once

/* Draw-line callback: platform provides this (SDL on Mac, piTrex on hardware).
 * Coordinates are in Vectrex space: X,Y ∈ [-127, 127], Y-up. */
typedef void (*renderer_draw_fn)(int x0, int y0, int x1, int y1);

/* Called once before doom_init().
 * draw_fn: platform-specific line drawing function. */
void renderer_init(renderer_draw_fn draw_fn);

/* Called after doom_update() each frame. */
void renderer_scene_begin(void);
void renderer_scene_end(void);
void renderer_frame_end(void);

/* Called after doom_update() to draw player weapon vectors on top of the 3D view. */
void renderer_draw_weapons(void);

/* Called after doom_update() to draw glow placeholders for world sprites (enemies, pickups). */
void renderer_draw_sprites(void);

/* Called after doom_update() to draw the automap when active (Tab). */
void renderer_draw_automap(void);
