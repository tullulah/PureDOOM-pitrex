/*
 * logger.h - simple file-based logging for baremetal DOOM
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

/* Opaque logger handle */
typedef void logger_t;

/* Initialize logger - opens doom.log on SD */
logger_t* logger_init(void);

/* Log a formatted message */
void logger_log(logger_t* log, const char* fmt, ...);

/* Flush and close logger */
void logger_close(logger_t* log);

#endif
