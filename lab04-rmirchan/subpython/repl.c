/*! \file
 * This file implements the Read-Eval-Print Loop (REPL) for the simple CS24
 * Python interpreter.
 */

#include <assert.h>
#include <getopt.h>
#include <stdio.h>

#ifndef NREADLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include <setjmp.h>
#include <unistd.h>

#include "alloc.h"
#include "eval.h"
#include "global.h"
#include "grammar.h"

#define DEFAULT_MEMORY_SIZE 1024

static int memory_size = DEFAULT_MEMORY_SIZE;
static int debug = 0;


/*!
 * This is the Read-Eval-Print-Loop (aka "REPL") function. We don't actually
 * use `readline` here because the parser is responsible for asking for more
 * input when needed.
 */
void read_eval_print_loop(FILE *input) {

#ifndef NREADLINE
    // Load command history, if any
    read_history(SUBPYTHON_HISTORY);

    // Disable filename auto-complete on TAB key
    rl_bind_key ('\t', rl_insert);
#endif

    // Flag to cause the main loop to Stop when asked to.
    int stop = 0;
    do {
        // Initialize the Flex / Bison scanner.
        yyscan_t scanner;
        yylex_init(&scanner);

        // Initialize Subpython's private data.
        subpy_udata_t udata;
        subpy_udata_init(&udata, input);
        yyset_extra(&udata, scanner);

        int result = yyparse(scanner);

        // If Bison returns an error code of 2 then there was a critical
        // failure. If it returns an error code of 3, then EOF was triggered
        // on an empty input, so we should exit.
        if (result >= 2) {
            if (!quiet) {
                printf("\nQuitting, goodbye.\n");
            }
            stop = 1;

        // If there was no parsing error, then the parse AST is located
        // in udata.tree.
        } else if (result == 0 && udata.tree) {
            if (setjmp(error_jmp) == 0) {
                eval_root(udata.tree);
            }

            clear_temporary_globals();

            if (debug) {
                printf("\n");

                print_globals();

                printf("\nMemory Contents:\n");
                memdump();

                printf("\n");
            }

        }

        // Cleanup this parser state.
        subpy_udata_destroy(&udata);
        yylex_destroy(scanner);
    } while (isatty(fileno(input)) && !stop);
}


/*! Prints the program's usage information. */
void usage(char *program) {
    printf("usage: %s [OPTION]...\n", program);
    printf("Runs the CS24 Sub-Python interpreter\n\n");
    printf(" -f file        file to run instead of standard input\n");
    printf(" -m memory_size amount of memory (in bytes) to use for the memory pool\n");
    printf(" -q             run in quite mode, supresses extra output\n");
    printf(" -d             run in debug mode:\n");
    printf("                  the REPL will printing out the current bindings and\n");
    printf("                  memory contents after every evaluation\n");
}


/*!
 * The entry point to the SubPython interpreter.  Several command-line
 * arguments can be specified, to control various aspects of the program
 * behavior.
 */
int main(int argc, char **argv) {
    int c;

    FILE *input = stdin;

    while ((c = getopt(argc, argv, "f:m:qd")) != -1) {
        switch (c) {
            case 'f':
                input = fopen(optarg, "r");
                if (input == NULL) {
                    fprintf(stderr, "%s: %s: %s", argv[0], optarg,
                                strerror(errno));
                    exit(1);
                }
                break;

            case 'm':
                memory_size = strtol(optarg, NULL, 10);
                if (memory_size <= 0) {
                    fprintf(stderr, "%s: invalid memory size\n", argv[0]);
                    usage(argv[0]);
                    exit(1);
                }
                break;

            case 'q':
                quiet = 1;
                break;

            case 'd':
                debug = 1;
                break;

            case '?':
                usage(argv[0]);
                exit(1);

            default:
                abort();
        }
    }

    if (optind < argc) {
        printf("%s: positional arguments are not accepted\n", argv[0]);
        exit(1);
    }

    if (!quiet) {
        printf("Subpython [CS24 SP19]\n");
        printf("Using a memory size of %d bytes.\n", memory_size);
    }

    mm_init(memory_size);
    eval_init();
    read_eval_print_loop(input);
    mm_cleanup();

    return 0;
}

