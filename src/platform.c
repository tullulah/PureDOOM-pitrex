/*
 * platform.c — piTrex baremetal platform implementation
 *
 * Implements the PureDOOM platform callbacks against the piTrex SDK:
 *   - File I/O via FatFs (ff.h) — SD card is already mounted by v_init()
 *   - Time via v_millis()
 *   - Print via printf() — redefined in vectrexInterface.h to go to UART
 *   - Input via v_readButtons() / v_readJoystick1Analog()
 */

#include "platform.h"
#include "../../puredoom/PureDOOM.h"  /* doom_key_down/up, doom_seek_t */

#ifndef HOST_SDL
#include <vectrex/vectrexInterface.h>
#include <baremetal/ff.h>
#endif

/* ================================================================
 * File I/O  (FatFs backend — SD already mounted by v_init())
 * We allocate FIL objects from a small static pool.
 * ================================================================ */
#ifndef HOST_SDL

#define MAX_OPEN_FILES 4
static FIL s_fil_pool[MAX_OPEN_FILES];
static int  s_fil_used[MAX_OPEN_FILES];

static FIL* alloc_fil(void)
{
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (!s_fil_used[i]) { s_fil_used[i] = 1; return &s_fil_pool[i]; }
    }
    return 0;
}

static void free_fil(FIL* f)
{
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (&s_fil_pool[i] == f) { s_fil_used[i] = 0; return; }
    }
}

void* platform_open(const char* path, const char* mode)
{
    FIL* f = alloc_fil();
    if (!f) return 0;
    BYTE fa = (mode[0] == 'r') ? FA_READ : (FA_WRITE | FA_CREATE_ALWAYS);
    if (f_open(f, path, fa) != FR_OK) { free_fil(f); return 0; }
    return f;
}

void platform_close(void* handle)
{
    if (!handle) return;
    f_close((FIL*)handle);
    free_fil((FIL*)handle);
}

int platform_read(void* handle, void* buf, int count)
{
    UINT br = 0;
    f_read((FIL*)handle, buf, (UINT)count, &br);
    return (int)br;
}

int platform_write(void* handle, const void* buf, int count)
{
    UINT bw = 0;
    f_write((FIL*)handle, buf, (UINT)count, &bw);
    return (int)bw;
}

int platform_seek(void* handle, int offset, doom_seek_t origin)
{
    FIL* f = (FIL*)handle;
    FSIZE_t pos;
    switch (origin) {
        case DOOM_SEEK_SET: pos = (FSIZE_t)offset; break;
        case DOOM_SEEK_CUR: pos = f_tell(f) + (FSIZE_t)offset; break;
        case DOOM_SEEK_END: pos = f_size(f) + (FSIZE_t)offset; break;
        default:            pos = (FSIZE_t)offset; break;
    }
    return (f_lseek(f, pos) == FR_OK) ? 0 : -1;
}

int platform_tell(void* handle)
{
    return (int)f_tell((FIL*)handle);
}

int platform_eof(void* handle)
{
    return f_eof((FIL*)handle);
}

/* ================================================================
 * Time  (v_millis returns ms since boot)
 * ================================================================ */
void platform_gettime(int* sec, int* usec)
{
    uint32_t ms = v_millis();
    *sec  = (int)(ms / 1000);
    *usec = (int)((ms % 1000) * 1000);
}

/* ================================================================
 * Debug print  (printf is remapped to UART by vectrexInterface.h)
 * ================================================================ */
void platform_print(const char* msg)
{
    printf("%s", msg);
}

/* ================================================================
 * Environment (baremetal: no env vars — return SD root for HOME)
 * ================================================================ */
char* platform_getenv(const char* var)
{
    if (var && var[0] == 'H' && var[1] == 'O' && var[2] == 'M' && var[3] == 'E' && var[4] == '\0')
        return "/";
    return (char*)0;
}

/* ================================================================
 * Input  (Vectrex controller → DOOM keys)
 *
 * Vectrex controller layout:
 *   Stick up/down    → forward/back  (DOOM_KEY_UP/DOWN_ARROW)
 *   Stick left/right → turn          (DOOM_KEY_LEFT/RIGHT_ARROW)
 *   BTN1 (bit 0)     → Fire   (CTRL)
 *   BTN2 (bit 1)     → Use    (Space)
 *   BTN3 (bit 2)     → Run    (Shift)
 *   BTN4 (bit 3)     → Strafe toggle (Alt)
 * ================================================================ */
#define STICK_THRESH 40

void platform_poll_input(void)
{
    static uint8_t prev_btns = 0;
    static int8_t  prev_x = 0, prev_y = 0;

    v_readButtons();
    v_readJoystick1Analog();

    uint8_t btns = currentButtonState;
    int8_t  jx   = currentJoy1X;
    int8_t  jy   = currentJoy1Y;

#define BTN_EVENT(mask, key) \
    if ( (btns & (mask)) && !(prev_btns & (mask))) doom_key_down(key); \
    if (!(btns & (mask)) &&  (prev_btns & (mask))) doom_key_up(key);

    BTN_EVENT(0x01, DOOM_KEY_CTRL)
    BTN_EVENT(0x02, DOOM_KEY_SPACE)
    BTN_EVENT(0x04, DOOM_KEY_SHIFT)
    BTN_EVENT(0x08, DOOM_KEY_ALT)

    /* forward / back */
    if ( jy >  STICK_THRESH && prev_y <= STICK_THRESH)  doom_key_down(DOOM_KEY_UP_ARROW);
    if ( jy <= STICK_THRESH && prev_y >  STICK_THRESH)  doom_key_up(DOOM_KEY_UP_ARROW);
    if ( jy < -STICK_THRESH && prev_y >= -STICK_THRESH) doom_key_down(DOOM_KEY_DOWN_ARROW);
    if ( jy >=-STICK_THRESH && prev_y <  -STICK_THRESH) doom_key_up(DOOM_KEY_DOWN_ARROW);

    /* turn left / right */
    if ( jx < -STICK_THRESH && prev_x >= -STICK_THRESH) doom_key_down(DOOM_KEY_LEFT_ARROW);
    if ( jx >=-STICK_THRESH && prev_x <  -STICK_THRESH) doom_key_up(DOOM_KEY_LEFT_ARROW);
    if ( jx >  STICK_THRESH && prev_x <= STICK_THRESH)  doom_key_down(DOOM_KEY_RIGHT_ARROW);
    if ( jx <= STICK_THRESH && prev_x >  STICK_THRESH)  doom_key_up(DOOM_KEY_RIGHT_ARROW);

    prev_btns = btns;
    prev_x = jx;
    prev_y = jy;
}

/* platform_init: hardware already initialized in main() via vectrexinit/v_init */
void platform_init(void) {}

#endif /* !HOST_SDL */

