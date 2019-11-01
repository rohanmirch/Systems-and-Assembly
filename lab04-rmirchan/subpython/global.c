#include <stdarg.h>
#include <string.h>

#include "global.h"

bool quiet;

sigjmp_buf error_jmp;
void error(const char *fmt, ...)  {
    fprintf(stderr, "Error: ");

    va_list argptr;
    va_start(argptr, fmt);
    vfprintf(stderr, fmt, argptr);
    va_end(argptr);

    fprintf(stderr, "\n");

    longjmp(error_jmp, 1);
}
