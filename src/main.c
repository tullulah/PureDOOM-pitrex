/*
 * main.c — piTrex DOOM entry point (baremetal)
 *
 * Entry point is main() — the piTrex libbaremetal boot code mounts the SD
 * card and calls main(). DOOM_IMPLEMENTATION lives in renderer.c.
 *
 * Coordinate mapping:
 *   Our renderer works in Vectrex space [-127,127]. The piTrex API uses
 *   v_directDraw32 with range ±18000 X / ±24000 Y, so we scale:
 *     pitrex_x = vec_x * 18000 / 127
 *     pitrex_y = vec_y * 24000 / 127
 */

/* PureDOOM declarations only — implementation lives in renderer.c */
#include "../../puredoom/PureDOOM.h"

#include "platform.h"   /* piTrex platform callbacks */
#include "renderer.h"   /* wireframe renderer */
#include "logger.h"     /* file-based logging */

#ifndef HOST_SDL
#include <vectrex/vectrexInterface.h>
#endif

/* ----------------------------------------------------------------
 * Static bump allocator  (DOOM needs ~4-6MB; Pi Zero has 512MB)
 * ---------------------------------------------------------------- */
#define HEAP_SIZE (32 * 1024 * 1024)
static unsigned char s_heap[HEAP_SIZE];
static unsigned int  s_heap_top = 0;

static void* heap_malloc(int size)
{
    size = (size + 3) & ~3;
    if (s_heap_top + (unsigned int)size > HEAP_SIZE)
        return 0;
    void* ptr = &s_heap[s_heap_top];
    s_heap_top += (unsigned int)size;
    return ptr;
}

static void heap_free(void* ptr) { (void)ptr; }

#ifndef HOST_SDL
/* ----------------------------------------------------------------
 * piTrex draw callback — scales from Vectrex [-127,127] to piTrex
 * ---------------------------------------------------------------- */
static void pitrex_draw_line(int x0, int y0, int x1, int y1)
{
    v_directDraw32(
        (int32_t)x0 * 18000 / 127,
        (int32_t)y0 * 24000 / 127,
        (int32_t)x1 * 18000 / 127,
        (int32_t)y1 * 24000 / 127,
        64  /* brightness 0-127 */
    );
}

/* ----------------------------------------------------------------
 * Baremetal entry point  (libbaremetal calls main after SD mount)
 * ---------------------------------------------------------------- */
/* ----------------------------------------------------------------
 * Baremetal entry point  (libbaremetal calls main after SD mount)
 * ---------------------------------------------------------------- */
/* ----------------------------------------------------------------
 * LED blink for diagnostics
 * ---------------------------------------------------------------- */
#define GPIO_BASE       0x3F200000UL
#define GPFSEL2         (*(volatile unsigned int*)(GPIO_BASE + 0x08))
#define GPSET0          (*(volatile unsigned int*)(GPIO_BASE + 0x1C))
#define GPCLR0          (*(volatile unsigned int*)(GPIO_BASE + 0x28))
#define ACT_PIN         29

static void blink_pattern(int times)
{
    GPFSEL2 &= ~(7u << 27);
    GPFSEL2 |=  (1u << 27);
    for (int i = 0; i < times; i++) {
        GPSET0 = (1u << ACT_PIN);
        for (volatile int d = 0; d < 5000000; d++) {}
        GPCLR0 = (1u << ACT_PIN);
        for (volatile int d = 0; d < 5000000; d++) {}
    }
    for (volatile int d = 0; d < 10000000; d++) {}
}

static void run_boot_vector_test(int frames, logger_t* log)
{
    if (log) logger_log(log, "boot vector test start (%d frames)\n", frames);

    for (int i = 0; i < frames; i++) {
        v_WaitRecal();
        v_readButtons();
        v_readJoystick1Analog();

        /* Minimal draw sanity check: one simple diagonal vector. */
        v_directDraw32(-10000, -10000, 10000, 10000, 96);
    }

    if (log) logger_log(log, "boot vector test end\n");
}

int main(int argc, char** argv)
{
    (void)argc; (void)argv;

    blink_pattern(3);  /* 3 blinks = in main() */

    /* Initialize logger first */
    logger_t* log = logger_init();
    if (!log) {
        blink_pattern(1); /* 1 blink = logger_init failed */
        return -1;
    }
    blink_pattern(4);  /* 4 blinks = logger OK */

    /* --- piTrex hardware init --- */
    if (log) logger_log(log, "calling vectrexinit(1)...\n");
    vectrexinit(1);
    if (log) logger_log(log, "vectrexinit(1) OK\n");

    if (log) logger_log(log, "calling v_init()...\n");
    v_init();
    if (log) logger_log(log, "v_init() OK\n");

    if (log) logger_log(log, "calling v_setRefresh(60)...\n");
    v_setRefresh(60);
    if (log) logger_log(log, "v_setRefresh(60) OK\n");

    /* Draw a simple vector for ~3 seconds before booting DOOM logic. */
    run_boot_vector_test(180, log);

    /* --- wire up PureDOOM callbacks --- */
    if (log) logger_log(log, "setting up PureDOOM callbacks...\n");
    doom_set_malloc(heap_malloc, heap_free);
    if (log) logger_log(log, "doom_set_malloc() OK\n");

    doom_set_file_io(platform_open,
                     platform_close,
                     platform_read,
                     platform_write,
                     platform_seek,
                     platform_tell,
                     platform_eof);
    if (log) logger_log(log, "doom_set_file_io() OK\n");

    doom_set_gettime(platform_gettime);
    if (log) logger_log(log, "doom_set_gettime() OK\n");

    doom_set_print(platform_print);
    if (log) logger_log(log, "doom_set_print() OK\n");

    /* Baremetal has no environment — return "/" for HOME so DOOM uses SD root */
    doom_set_getenv(platform_getenv);
    if (log) logger_log(log, "doom_set_getenv() OK\n");

    int flags = DOOM_FLAG_HIDE_MOUSE_OPTIONS |
                DOOM_FLAG_HIDE_SOUND_OPTIONS |
                DOOM_FLAG_HIDE_MUSIC_OPTIONS;

    if (log) logger_log(log, "calling doom_set_resolution(320, 200)...\n");
    doom_set_resolution(320, 200);
    if (log) logger_log(log, "doom_set_resolution() OK\n");

    if (log) logger_log(log, "calling renderer_init()...\n");
    renderer_init(pitrex_draw_line);
    if (log) logger_log(log, "renderer_init() OK\n");

    /* WAD is on the SD card root */
    if (log) logger_log(log, "calling doom_init()...\n");
    char* doom_argv[] = { "doom", "-iwad", "doom1.wad", "-nomusic", "-nosound" };
    doom_init(5, doom_argv, flags);
    if (log) logger_log(log, "doom_init() OK, entering main loop\n");

    /* --- main loop --- */
    for (;;)
    {
        v_WaitRecal();
        platform_poll_input();
        renderer_scene_begin();
        doom_update();
        renderer_scene_end();
        renderer_draw_sprites();
        renderer_draw_weapons();
        renderer_frame_end();
    }
    return 0;
}
#endif /* !HOST_SDL */

