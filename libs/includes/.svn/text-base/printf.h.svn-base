#ifndef _PRINTF_H_
#define _PRINTF_H_

#include <stdarg.h>

#define NOFLOAT

int printf_without_mutex(const char *format, ...);
int printf_without_lock(const char *format, ...);
int printf(const char *format, ...);
int sprintf(char *out, const char *format, ...);
inline int vprintf(char *fmt, va_list va);

#endif
