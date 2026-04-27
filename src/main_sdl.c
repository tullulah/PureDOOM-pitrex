/*
 * main_sdl.c — host SDL build for testing the wireframe renderer on Mac/Linux
 *
 * Renders DOOM's wireframe output as green lines on a black window.
 * The coordinate space matches the Vectrex: X,Y ∈ [-127,127], Y-up,
 * so what you see here is exactly what the Vectrex beam will draw.
 *
 * Build & run:
 *   make run_sdl
 *
 * Controls: WASD / arrow keys / Ctrl=fire / Space=use
 */

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* PureDOOM declarations only — implementation lives in renderer.c */
#include "../../puredoom/PureDOOM.h"

#include "renderer.h"

/* ================================================================
 * Window config
 * SDL window is 512x512. The Vectrex square [-127,127] is centered.
 * ================================================================ */
#define WIN_W  512
#define WIN_H  512
#define VEC_SCALE 2   /* pixels per Vectrex unit */

static SDL_Window*      s_window     = 0;
static SDL_Renderer*    s_renderer   = 0;
static SDL_GameController* s_pad      = NULL;
static SDL_Joystick*    s_joy        = NULL;

/* ================================================================
 * Joystick / gamepad input
 *
 * Analog axes → virtual key state (fire key_down/key_up on edge).
 * Works with both SDL_GameController (Xbox/PS) and raw SDL_Joystick.
 * ================================================================ */
#define JOY_DEADZONE 10000   /* out of ±32767 */

enum { JK_FWD=0, JK_BACK, JK_TLEFT, JK_TRIGHT, JK_SLEFT, JK_SRIGHT, JK__COUNT };
static int s_jk[JK__COUNT];
static const doom_key_t JK_MAP[JK__COUNT] = {
    DOOM_KEY_UP_ARROW, DOOM_KEY_DOWN_ARROW,
    DOOM_KEY_LEFT_ARROW, DOOM_KEY_RIGHT_ARROW,
    DOOM_KEY_COMMA, DOOM_KEY_PERIOD
};

static void jk_set(int idx, int on)
{
    if (s_jk[idx] == on) return;
    s_jk[idx] = on;
    if (on) doom_key_down(JK_MAP[idx]);
    else    doom_key_up(JK_MAP[idx]);
}

/* Poll analog axes each frame (threshold → discrete key) */
static void joy_poll_axes(void)
{
    Sint16 lx, ly, rx;
    if (s_pad) {
        lx = SDL_GameControllerGetAxis(s_pad, SDL_CONTROLLER_AXIS_LEFTX);
        ly = SDL_GameControllerGetAxis(s_pad, SDL_CONTROLLER_AXIS_LEFTY);
        rx = SDL_GameControllerGetAxis(s_pad, SDL_CONTROLLER_AXIS_RIGHTX);
    } else if (s_joy) {
        lx = SDL_JoystickGetAxis(s_joy, 0);
        ly = SDL_JoystickGetAxis(s_joy, 1);
        rx = SDL_JoystickNumAxes(s_joy) > 2 ? SDL_JoystickGetAxis(s_joy, 2) : 0;
    } else return;

    jk_set(JK_FWD,    ly < -JOY_DEADZONE);
    jk_set(JK_BACK,   ly >  JOY_DEADZONE);
    jk_set(JK_TLEFT,  lx < -JOY_DEADZONE);
    jk_set(JK_TRIGHT, lx >  JOY_DEADZONE);
    jk_set(JK_SLEFT,  rx < -JOY_DEADZONE);
    jk_set(JK_SRIGHT, rx >  JOY_DEADZONE);
}

/* Map gamepad button index → DOOM key */
static doom_key_t pad_button_key(Uint8 btn)
{
    switch (btn) {
        case SDL_CONTROLLER_BUTTON_A:           return DOOM_KEY_CTRL;   /* fire  */
        case SDL_CONTROLLER_BUTTON_B:           return DOOM_KEY_SPACE;  /* use   */
        case SDL_CONTROLLER_BUTTON_X:           return DOOM_KEY_SHIFT;  /* run   */
        case SDL_CONTROLLER_BUTTON_Y:           return DOOM_KEY_TAB;    /* map   */
        case SDL_CONTROLLER_BUTTON_START:       return DOOM_KEY_ENTER;
        case SDL_CONTROLLER_BUTTON_BACK:        return DOOM_KEY_ESCAPE;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:  return DOOM_KEY_COMMA;   /* strafe L */
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return DOOM_KEY_PERIOD;  /* strafe R */
        case SDL_CONTROLLER_BUTTON_DPAD_UP:     return DOOM_KEY_UP_ARROW;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:   return DOOM_KEY_DOWN_ARROW;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:   return DOOM_KEY_LEFT_ARROW;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:  return DOOM_KEY_RIGHT_ARROW;
        default:                                return DOOM_KEY_UNKNOWN;
    }
}

/* Map raw joystick button → DOOM key (generic fallback) */
static doom_key_t joy_button_key(Uint8 btn)
{
    switch (btn) {
        case 0:  return DOOM_KEY_CTRL;    /* fire  */
        case 1:  return DOOM_KEY_SPACE;   /* use   */
        case 2:  return DOOM_KEY_SHIFT;   /* run   */
        case 3:  return DOOM_KEY_TAB;     /* map   */
        case 7:  return DOOM_KEY_ENTER;   /* start */
        case 6:  return DOOM_KEY_ESCAPE;  /* select*/
        default: return DOOM_KEY_UNKNOWN;
    }
}

/* D-pad hat → discrete key events */
static Uint8 s_hat_prev = SDL_HAT_CENTERED;
static void hat_update(Uint8 hat)
{
    Uint8 changed = hat ^ s_hat_prev;
    if (!changed) return;
    s_hat_prev = hat;
    struct { Uint8 mask; doom_key_t k; } dirs[] = {
        { SDL_HAT_UP,    DOOM_KEY_UP_ARROW    },
        { SDL_HAT_DOWN,  DOOM_KEY_DOWN_ARROW  },
        { SDL_HAT_LEFT,  DOOM_KEY_LEFT_ARROW  },
        { SDL_HAT_RIGHT, DOOM_KEY_RIGHT_ARROW },
    };
    for (int i = 0; i < 4; i++) {
        if (!(changed & dirs[i].mask)) continue;
        if (hat & dirs[i].mask) doom_key_down(dirs[i].k);
        else                    doom_key_up(dirs[i].k);
    }
}

/* ================================================================
 * draw_fn — SDL implementation of the renderer callback
 * Converts Vectrex coords [-127,127] to SDL window pixels.
 * ================================================================ */
static void sdl_draw_line(int x0, int y0, int x1, int y1)
{
    /* Vectrex → window:
     *   win_x = (vec_x + 127) * WIN_W / 254
     *   win_y = (127 - vec_y) * WIN_H / 254   (Y-flip: Vectrex Y-up, SDL Y-down)
     */
    int wx0 = (x0 + 127) * WIN_W / 254;
    int wy0 = (127 - y0) * WIN_H / 254;
    int wx1 = (x1 + 127) * WIN_W / 254;
    int wy1 = (127 - y1) * WIN_H / 254;

    SDL_RenderDrawLine(s_renderer, wx0, wy0, wx1, wy1);
}

/* ================================================================
 * PureDOOM platform callbacks (host: use real libc + SDL)
 * ================================================================ */

/* doom_gettime_fn passes sec + usec */
static void  host_gettime(int* sec, int* usec)              { uint32_t ms = SDL_GetTicks(); *sec = (int)(ms/1000); *usec = (int)((ms%1000)*1000); }
static void  host_print(const char* msg)                    { fputs(msg, stdout); }

/* DOOMWADDIR is intercepted to point at the directory containing our WAD.
 * Set by main() before doom_init(). */
static char s_wad_dir[1024];
static char* host_getenv(const char* var)
{
    if (s_wad_dir[0] && strcmp(var, "DOOMWADDIR") == 0)
        return s_wad_dir;
    return getenv(var);
}

/* ================================================================
 * SDL key → DOOM key
 * ================================================================ */
static doom_key_t sdl_key(SDL_Scancode sc)
{
    switch (sc) {
        case SDL_SCANCODE_LEFT:      return DOOM_KEY_LEFT_ARROW;
        case SDL_SCANCODE_RIGHT:     return DOOM_KEY_RIGHT_ARROW;
        case SDL_SCANCODE_UP:        return DOOM_KEY_UP_ARROW;
        case SDL_SCANCODE_DOWN:      return DOOM_KEY_DOWN_ARROW;
        case SDL_SCANCODE_W:         return DOOM_KEY_UP_ARROW;
        case SDL_SCANCODE_S:         return DOOM_KEY_DOWN_ARROW;
        case SDL_SCANCODE_A:         return DOOM_KEY_COMMA;       /* strafe left  */
        case SDL_SCANCODE_D:         return DOOM_KEY_PERIOD;      /* strafe right */
        case SDL_SCANCODE_Q:         return DOOM_KEY_LEFT_ARROW;  /* turn left    */
        case SDL_SCANCODE_E:         return DOOM_KEY_RIGHT_ARROW; /* turn right   */
        case SDL_SCANCODE_LCTRL:
        case SDL_SCANCODE_RCTRL:     return DOOM_KEY_CTRL;
        case SDL_SCANCODE_SPACE:     return DOOM_KEY_SPACE;
        case SDL_SCANCODE_LSHIFT:
        case SDL_SCANCODE_RSHIFT:    return DOOM_KEY_SHIFT;
        case SDL_SCANCODE_LALT:
        case SDL_SCANCODE_RALT:      return DOOM_KEY_ALT;
        case SDL_SCANCODE_RETURN:    return DOOM_KEY_ENTER;
        case SDL_SCANCODE_ESCAPE:    return DOOM_KEY_ESCAPE;
        case SDL_SCANCODE_TAB:       return DOOM_KEY_TAB;
        case SDL_SCANCODE_F1:        return DOOM_KEY_F1;
        case SDL_SCANCODE_F2:        return DOOM_KEY_F2;
        case SDL_SCANCODE_F3:        return DOOM_KEY_F3;
        case SDL_SCANCODE_F5:        return DOOM_KEY_F5;
        case SDL_SCANCODE_F6:        return DOOM_KEY_F6;
        case SDL_SCANCODE_F7:        return DOOM_KEY_F7;
        case SDL_SCANCODE_F9:        return DOOM_KEY_F9;
        case SDL_SCANCODE_F10:       return DOOM_KEY_F10;
        case SDL_SCANCODE_1:         return DOOM_KEY_1;
        case SDL_SCANCODE_2:         return DOOM_KEY_2;
        case SDL_SCANCODE_3:         return DOOM_KEY_3;
        case SDL_SCANCODE_4:         return DOOM_KEY_4;
        case SDL_SCANCODE_5:         return DOOM_KEY_5;
        case SDL_SCANCODE_6:         return DOOM_KEY_6;
        case SDL_SCANCODE_7:         return DOOM_KEY_7;
        default:                     return DOOM_KEY_UNKNOWN;
    }
}

/* ================================================================
 * main
 * ================================================================ */
int main(int argc, char** argv)
{
    /* --- SDL init --- */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    s_window = SDL_CreateWindow(
        "piTrex DOOM — wireframe preview",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIN_W, WIN_H, 0);

    s_renderer = SDL_CreateRenderer(
        s_window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    /* --- Joystick / gamepad init --- */
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            s_pad = SDL_GameControllerOpen(i);
            if (s_pad) { printf("Gamepad: %s\n", SDL_GameControllerName(s_pad)); break; }
        }
    }
    if (!s_pad && SDL_NumJoysticks() > 0) {
        s_joy = SDL_JoystickOpen(0);
        if (s_joy) printf("Joystick: %s\n", SDL_JoystickName(s_joy));
    }
    if (!s_pad && !s_joy) printf("No joystick detected — keyboard only\n");

    /* --- PureDOOM callbacks --- */
    /* malloc and file I/O: use PureDOOM's built-in defaults
     * (requires DOOM_IMPLEMENT_FILE_IO + DOOM_IMPLEMENT_MALLOC in renderer.c) */
    doom_set_gettime(host_gettime);
    doom_set_print(host_print);
    doom_set_getenv(host_getenv);

    doom_set_resolution(320, 200);

    int flags = DOOM_FLAG_HIDE_MOUSE_OPTIONS |
                DOOM_FLAG_HIDE_SOUND_OPTIONS |
                DOOM_FLAG_HIDE_MUSIC_OPTIONS;

    /* --- wireframe renderer --- */
    renderer_init(sdl_draw_line);

    /* --- find WAD --- */

    /* Build argc/argv for doom_init — no -warp so the title screen shows */
    char* doom_argv[16];
    int   doom_argc = 1;
    doom_argv[0] = argv[0];
    /* Disable music/sound: prevents W_CacheLumpNum crash on level load
     * (S_ChangeMusic with lump=-1 when no midi player callback is set) */
    doom_argv[doom_argc++] = "-nomusic";
    doom_argv[doom_argc++] = "-nosound";

    /* 1. User-provided -iwad takes priority */
    const char* wad_path = NULL;
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "-iwad") == 0) {
            wad_path = argv[i + 1];
            break;
        }
    }

    /* 2. Auto-detect: try common relative paths and resolve to absolute */
    static char wad_buf[1024];
    static char wad_real[1024];
    if (!wad_path) {
        const char* candidates[] = {
            "../puredoom/doom1.wad",
            "doom1.wad",
            NULL
        };
        for (int i = 0; candidates[i]; i++) {
            FILE* f = fopen(candidates[i], "rb");
            if (f) {
                fclose(f);
                if (realpath(candidates[i], wad_real))
                    wad_path = wad_real;
                else
                    wad_path = candidates[i];
                break;
            }
        }
    }
    (void)wad_buf; /* suppress unused warning */

    /* 3. Fallback: same directory */
    if (!wad_path) {
        FILE* f = fopen("doom1.wad", "rb");
        if (f) { fclose(f); wad_path = "doom1.wad"; }
    }

    if (!wad_path) {
        fprintf(stderr, "ERROR: doom1.wad not found. Set DOOMWADDIR or copy doom1.wad here.\n");
        return 1;
    }

    /* Extract directory from wad_path → s_wad_dir (used by host_getenv) */
    const char* last_slash = strrchr(wad_path, '/');
    if (last_slash) {
        size_t dir_len = (size_t)(last_slash - wad_path);
        strncpy(s_wad_dir, wad_path, dir_len);
        s_wad_dir[dir_len] = '\0';
    } else {
        strcpy(s_wad_dir, ".");
    }
    printf("Using WAD dir: %s\n", s_wad_dir);

    doom_init(doom_argc, doom_argv, flags);

    /* --- main loop --- */
    int running = 1;
    while (running) {
        /* events */
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    if (!e.key.repeat)
                        doom_key_down(sdl_key(e.key.keysym.scancode));
                    break;
                case SDL_KEYUP:
                    doom_key_up(sdl_key(e.key.keysym.scancode));
                    break;
                /* --- Gamepad (SDL_GameController) --- */
                case SDL_CONTROLLERBUTTONDOWN: {
                    doom_key_t k = pad_button_key(e.cbutton.button);
                    if (k != DOOM_KEY_UNKNOWN) doom_key_down(k);
                    break;
                }
                case SDL_CONTROLLERBUTTONUP: {
                    doom_key_t k = pad_button_key(e.cbutton.button);
                    if (k != DOOM_KEY_UNKNOWN) doom_key_up(k);
                    break;
                }
                /* --- Raw joystick buttons --- */
                case SDL_JOYBUTTONDOWN:
                    if (!s_pad) {
                        doom_key_t k = joy_button_key(e.jbutton.button);
                        if (k != DOOM_KEY_UNKNOWN) doom_key_down(k);
                    }
                    break;
                case SDL_JOYBUTTONUP:
                    if (!s_pad) {
                        doom_key_t k = joy_button_key(e.jbutton.button);
                        if (k != DOOM_KEY_UNKNOWN) doom_key_up(k);
                    }
                    break;
                /* --- D-pad hat --- */
                case SDL_JOYHATMOTION:
                    if (!s_pad) hat_update(e.jhat.value);
                    break;
            }
        }
        /* analog axes → keys each frame */
        joy_poll_axes();

        /* clear — black background like a vector screen */
        SDL_SetRenderDrawColor(s_renderer, 0, 0, 0, 255);
        SDL_RenderClear(s_renderer);

        /* green vectors, like a phosphor screen */
        SDL_SetRenderDrawColor(s_renderer, 0, 255, 80, 255);

        /* tick DOOM — this triggers BSP traversal → R_StoreWallRange → sdl_draw_line */
        renderer_scene_begin();
        doom_update();
        renderer_scene_end();

        /* draw automap when active (Tab) */
        renderer_draw_automap();

        /* draw world sprite placeholders (enemies, pickups) */
        renderer_draw_sprites();

        /* draw player weapon vectors on top of 3D view */
        renderer_draw_weapons();

        renderer_frame_end();

        SDL_RenderPresent(s_renderer);
    }

    if (s_pad) SDL_GameControllerClose(s_pad);
    if (s_joy) SDL_JoystickClose(s_joy);
    SDL_DestroyRenderer(s_renderer);
    SDL_DestroyWindow(s_window);
    SDL_Quit();
    return 0;
}
