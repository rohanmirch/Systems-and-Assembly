#ifndef LISTS_H
#define LISTS_H

#include <stdbool.h>


typedef struct list_t list_t;
typedef void list_iter_t;


/*! Generic operations provided by all kinds of lists. */
struct list_t {
    // Standard list operations

    int  (*size)     (list_t *list);
    int  (*get)      (list_t *list, int index);
    void (*clear)    (list_t *list);
    void (*insert)   (list_t *list, int index, int value);
    void (*append)   (list_t *list, int value);
    bool (*contains) (list_t *list, int value);

    void (*sort)     (list_t *list);

    // List-iteration operations
    // (And, proof why object-oriented languages are superior to doing this
    // stuff in C.)

    list_iter_t * (*iter)          (list_t *list);
    int           (*iter_get)      (list_t *list, list_iter_t *iter);
    list_iter_t * (*iter_next)     (list_t *list, list_iter_t *iter);
};

list_t * alloc_arraylist(void);
void free_arraylist(list_t *list);

list_t * alloc_linkedlist(void);
void free_linkedlist(list_t *list);

void list_clear(list_t *list);
int list_size(list_t *list);
int list_get(list_t *list, int index);
void list_insert(list_t *list, int index, int value);
void list_append(list_t *list, int value);
bool list_contains(list_t *list, int value);

void list_sort(list_t *list);

list_iter_t * list_iter(list_t *list);
int list_iter_get(list_t *list, list_iter_t *iter);
list_iter_t * list_iter_next(list_t *list, list_iter_t *iter);

#endif // LISTS_H
