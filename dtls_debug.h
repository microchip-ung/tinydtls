/*******************************************************************************
 *
 * Copyright (c) 2011, 2012, 2013, 2014, 2015 Olaf Bergmann (TZI) and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v. 1.0 which accompanies this distribution.
 *
 * The Eclipse Public License is available at http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at 
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Olaf Bergmann  - initial API and implementation
 *    Hauke Mehrtens - memory optimization, ECC integration
 *
 *******************************************************************************/

#ifndef _DTLS_DEBUG_H_
#define _DTLS_DEBUG_H_

#include <stdlib.h>

#include "tinydtls.h"
#include "global.h"
#include "session.h"

#ifdef WITH_ZEPHYR
#include <logging/log.h>
#endif /* WITH_ZEPHYR */

#ifdef WITH_CONTIKI
# ifndef DEBUG
#  define DEBUG DEBUG_PRINT
# endif /* DEBUG */
#include "net/ip/uip-debug.h"

#ifdef CONTIKI_TARGET_MBXXX
extern char __Stack_Init, _estack;

static inline void check_stack(void) {
  const char *p = &__Stack_Init;
  while (p < &_estack && *p == 0x38) {
    p++;
  }

  PRINTF("Stack: %d bytes used (%d free)\n", &_estack - p, p - &__Stack_Init);
}
#else /* CONTIKI_TARGET_MBXXX */
static inline void check_stack(void) {
}
#endif /* CONTIKI_TARGET_MBXXX */
#elif defined(WITH_LMSTAX)

#define PRINTF(...) LMU_PRINTF(__VA_ARGS__)

static inline void check_stack(void) {
}
#else /* WITH_CONTKI */
#define PRINTF(...)

static inline void check_stack(void) {
}
#endif

/** Pre-defined log levels akin to what is used in \b syslog. */
typedef enum { DTLS_LOG_EMERG=0, DTLS_LOG_ALERT, DTLS_LOG_CRIT, DTLS_LOG_WARN, 
       DTLS_LOG_NOTICE, DTLS_LOG_INFO, DTLS_LOG_DEBUG
} log_t;

/** Returns a zero-terminated string with the name of this library. */
const char *dtls_package_name(void);

/** Returns a zero-terminated string with the library version. */
const char *dtls_package_version(void);

/** Returns the current log level. */
log_t dtls_get_log_level(void);

/** Sets the log level to the specified value. */
void dtls_set_log_level(log_t level);

/** 
 * Writes the given text to \c stdout. The text is output only when \p
 * level is below or equal to the log level that set by
 * set_log_level(). */
#ifdef HAVE_VPRINTF
#if (defined(__GNUC__))
void dsrv_log(log_t level, const char *format, ...) __attribute__ ((format(printf, 2, 3)));
#else /* !__GNUC__ */
void dsrv_log(log_t level, const char *format, ...);
#endif /* !__GNUC__ */
#else
#define dsrv_log(level, format, ...) PRINTF(format, ##__VA_ARGS__)
#endif

/** dumps packets in usual hexdump format */
void hexdump(const unsigned char *packet, int length);

/** dump as narrow string of hex digits */
void dump(unsigned char *buf, size_t len);

void dtls_dsrv_hexdump_log(log_t level, const char *name, const unsigned char *buf, size_t length, int extend);

void dtls_dsrv_log_addr(log_t level, const char *name, const session_t *addr);

/* A set of convenience macros for common log levels. */
#ifdef WITH_ZEPHYR
#define dtls_emerg(...) LOG_ERR(__VA_ARGS__)
#define dtls_alert(...) LOG_ERR(__VA_ARGS__)
#define dtls_crit(...) LOG_ERR(__VA_ARGS__)
#define dtls_warn(...) LOG_WRN(__VA_ARGS__)
#define dtls_notice(...) LOG_INF(__VA_ARGS__)
#define dtls_info(...) LOG_INF(__VA_ARGS__)
#define dtls_debug(...) LOG_DBG(__VA_ARGS__)
#define dtls_debug_hexdump(name, buf, length) { LOG_DBG("%s (%zu bytes):", name, length); LOG_HEXDUMP_DBG(buf, length, name); }
#define dtls_debug_dump(name, buf, length) { LOG_DBG("%s (%zu bytes):", name, length); LOG_HEXDUMP_DBG(buf, length, name); }

#elif defined(WITH_LMSTAX)

#define DTLS_LOG_LEVEL  DTLS_LOG_NOTICE
#define DTLS_PRINTF(level, prefix, ...) \
    do {if ((level)<=(DTLS_LOG_LEVEL)) LMU_PRINTF(prefix __VA_ARGS__);} while(0)

#define dtls_emerg(...) DTLS_PRINTF(DTLS_LOG_EMERG, "DTLS_EMERG: ", __VA_ARGS__)
#define dtls_alert(...) DTLS_PRINTF(DTLS_LOG_ALERT, "DTLS_ALERT: ", __VA_ARGS__)
#define dtls_crit(...) DTLS_PRINTF(DTLS_LOG_CRIT, "DTLS_CRIT: ", __VA_ARGS__)
#define dtls_warn(...) DTLS_PRINTF(DTLS_LOG_WARN, "DTLS_WARN: ", __VA_ARGS__)
#define dtls_notice(...) DTLS_PRINTF(DTLS_LOG_NOTICE, "DTLS_NOTICE: ", __VA_ARGS__)
#define dtls_info(...) DTLS_PRINTF(DTLS_LOG_INFO, "DTLS_INFO: ", __VA_ARGS__)
#define dtls_debug(...) DTLS_PRINTF(DTLS_LOG_DEBUG, "DTLS_DEBUG: ", __VA_ARGS__)
#define dtls_debug_hexdump(name, buf, length) DTLS_PRINTF(DTLS_LOG_DEBUG, "DTLS_DEBUG_HEXDUMP: not implemented\n")
#define dtls_debug_dump(name, buf, length) DTLS_PRINTF(DTLS_LOG_DEBUG, "DTLS_DEBUG_DUMP: not implemented\n")

#else /* WITH_ZEPHYR */
#define dtls_emerg(...) dsrv_log(DTLS_LOG_EMERG, __VA_ARGS__)
#define dtls_alert(...) dsrv_log(DTLS_LOG_ALERT, __VA_ARGS__)
#define dtls_crit(...) dsrv_log(DTLS_LOG_CRIT, __VA_ARGS__)
#define dtls_warn(...) dsrv_log(DTLS_LOG_WARN, __VA_ARGS__)
#define dtls_notice(...) dsrv_log(DTLS_LOG_NOTICE, __VA_ARGS__)
#define dtls_info(...) dsrv_log(DTLS_LOG_INFO, __VA_ARGS__)
#define dtls_debug(...) dsrv_log(DTLS_LOG_DEBUG, __VA_ARGS__)
#define dtls_debug_hexdump(name, buf, length) dtls_dsrv_hexdump_log(DTLS_LOG_DEBUG, name, buf, length, 1)
#define dtls_debug_dump(name, buf, length) dtls_dsrv_hexdump_log(DTLS_LOG_DEBUG, name, buf, length, 0)
#endif /* WITH_ZEPHYR */

#endif /* _DTLS_DEBUG_H_ */
