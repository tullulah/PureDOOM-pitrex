/*
 * platform.h — piTrex baremetal platform interface
 *
 * Thin wrappers around the piTrex framework that satisfy PureDOOM's
 * callback signatures.  Implement these in platform.c against the
 * actual piTrex API headers.
 */

#pragma once

#include <stdint.h>
/* doom_seek_t is defined in PureDOOM.h — include it for the seek callback type */
#include "../../puredoom/PureDOOM.h"

/* --- lifecycle --- */
void platform_init(void);

/* --- file I/O (backed by FatFs) --- */
void* platform_open (const char* path, const char* mode);
void  platform_close(void* handle);
int   platform_read (void* handle, void* buf, int count);
int   platform_write(void* handle, const void* buf, int count);
int   platform_seek (void* handle, int offset, doom_seek_t origin);
/* Note: PureDOOM doom_seek_fn returns int; platform_seek matches that */
int   platform_tell (void* handle);
int   platform_eof  (void* handle);

/* --- time: matches PureDOOM's doom_gettime_fn signature --- */
void platform_gettime(int* sec, int* usec);

/* --- environment: returns "/" for HOME (SD card root), NULL for others --- */
char* platform_getenv(const char* var);

/* --- debug output (UART or bit-bang) --- */
void platform_print(const char* msg);

/* --- input: call once per frame, pumps doom_key_down/up internally --- */
void platform_poll_input(void);
