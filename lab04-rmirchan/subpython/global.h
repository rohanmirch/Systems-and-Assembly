/*! \file
 * This file contains the important global definitions for the CS24 Python
 * interpreter.
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <setjmp.h>

#include <stdnoreturn.h>

/*! History file for Sub-Python */
#define SUBPYTHON_HISTORY ".subpython"

/*! Maximum length of a single token. */
#define MAX_LENGTH 512

/*! Default initial size for realloc-growing arrays. */
#define INITIAL_SIZE 8

/* A handy macro to delineate an unreachable branch in switches. */
#define UNREACHABLE() \
    do { \
        fprintf(stderr, __FILE__ ":%d - unreachable!\n", __LINE__); \
        exit(-1); \
    } while (0)

extern bool quiet;

noreturn void error(const char *fmt, ...);
extern sigjmp_buf error_jmp;

#endif /* GLOBAL_H */
