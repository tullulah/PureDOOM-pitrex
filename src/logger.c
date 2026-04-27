/*
 * logger.c - file-based logging + UART output
 */

#include "logger.h"
#include "platform.h"
#include "../puredoom/PureDOOM.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef HOST_SDL
extern void RPI_AuxUartInit(int baud, int bits, int mhz);
#endif

struct logger_state {
    void* file_handle;
    char buffer[512];
};

logger_t* logger_init(void)
{
#ifndef HOST_SDL
    /* Init UART at 921600 baud before anything else */
    RPI_AuxUartInit(921600, 8, 250000000);
    printf("=== DOOM UART logger init ===\r\n");
#endif

    struct logger_state* log = (struct logger_state*)malloc(sizeof(struct logger_state));
    if (!log) return NULL;

    log->file_handle = platform_open("doom.log", "w");
    if (!log->file_handle) {
        printf("WARN: doom.log open failed, UART only\r\n");
    } else {
        const char* msg = "=== DOOM logger initialized ===\n";
        platform_write(log->file_handle, (void*)msg, strlen(msg));
    }

    return (logger_t*)log;
}

void logger_log(logger_t* log_ptr, const char* fmt, ...)
{
    if (!log_ptr) return;

    struct logger_state* log = (struct logger_state*)log_ptr;
    va_list args;
    va_start(args, fmt);

    int len = vsnprintf(log->buffer, sizeof(log->buffer), fmt, args);
    va_end(args);

    if (len > 0) {
#ifndef HOST_SDL
        printf("%s", log->buffer);
#endif
        if (log->file_handle)
            platform_write(log->file_handle, log->buffer, len);
    }
}

void logger_close(logger_t* log_ptr)
{
    if (!log_ptr) return;

    struct logger_state* log = (struct logger_state*)log_ptr;
    if (log->file_handle) {
        const char* msg = "=== DOOM logger closing ===\n";
        platform_write(log->file_handle, (void*)msg, strlen(msg));
        platform_close(log->file_handle);
    }
    free(log);
}
