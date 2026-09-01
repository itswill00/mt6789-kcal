#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static void print_log(const char *level, const char *tag, const char *fmt, va_list args) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char time_buf[64];
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", t);

    printf("[%s] [%s] [%s] ", time_buf, level, tag);
    vprintf(fmt, args);
    printf("\n");
}

void log_info(const char *tag, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log("INFO", tag, fmt, args);
    va_end(args);
}

void log_warn(const char *tag, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log("WARN", tag, fmt, args);
    va_end(args);
}

void log_error(const char *tag, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_log("ERROR", tag, fmt, args);
    va_end(args);
}
