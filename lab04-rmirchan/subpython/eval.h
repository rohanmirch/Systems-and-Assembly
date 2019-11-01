#ifndef EVAL_H
#define EVAL_H

#include <stdbool.h>

#include "grammar.h"
#include "types.h"

void ref_print(FILE *os, Reference ref);
void ref_println(FILE *os, Reference ref);

void eval_init();
Reference eval_root(struct Node *root);

bool ref_is_none(Reference r);
bool ref_is_true(Reference r);
bool ref_is_false(Reference r);

int foreach_global(void (*f)(const char *name, Reference ref));
void print_globals(void);

void clear_temporary_globals(void);

#endif /* EVAL_H */
