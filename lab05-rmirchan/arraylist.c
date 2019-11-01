#include "lists.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct arraylist_t arraylist_t;


/*! An array-backed list data structure. */
struct arraylist_t {
    int  (*size)     (arraylist_t *list);
    int  (*get)      (arraylist_t *list, int index);
    void (*clear)    (arraylist_t *list);
    void (*insert)   (arraylist_t *list, int index, int value);
    void (*append)   (arraylist_t *list, int value);
    bool (*contains) (arraylist_t *list, int value);

    void (*sort)     (arraylist_t *list);

    // List-iteration operations

    int * (*iter)      (arraylist_t *list);
    int   (*iter_get)  (arraylist_t *list, int *iter);
    int * (*iter_next) (arraylist_t *list, int *iter);

    /*! Number of actual elements in the list.  Invariant:  size <= capacity */
    int elems_size;

    /*! Number of elements we have room to store. */
    int elems_capacity;

    /*! Pointer to the array of elements that we store. */
    int *elems;
};


int arraylist_size(arraylist_t *list) {
    assert(list != NULL);
    return list->elems_size;
}


int arraylist_get(arraylist_t *list, int index) {
    assert(list != NULL);
    assert(index >= 0);
    assert(index < list->elems_size);

    return list->elems[index];
}


void arraylist_clear(arraylist_t *list) {
    assert(list != NULL);

    list->elems_size = 0;

    free(list->elems);
    list->elems = NULL;
    list->elems_capacity = 0;
}


void ensure_space_available(arraylist_t *list) {
    assert(list != NULL);

    
    
    if (list->elems_size >= list->elems_capacity) {
        int new_capacity = list->elems_capacity * 2;
        if (new_capacity == 0)
            new_capacity = 16;

#ifdef AL_RESIZE_OPT

        int *new_elems = realloc(list->elems, new_capacity * sizeof(int));        
        if (new_elems == NULL) {
            fprintf(stderr, "ERROR:  Failed to allocate in arraylist\n");
            abort();
        }

#else
        
        // Allocate a new region of memory, and copy the data to the new region.
        // Abort the program if allocation fails.
        int *new_elems = malloc(new_capacity * sizeof(int));        
        if (new_elems == NULL) {
            fprintf(stderr, "ERROR:  Failed to allocate in arraylist\n");
            abort();
        }
        memcpy(new_elems, list->elems, list->elems_capacity * sizeof(int));
        free(list->elems);

#endif

        list->elems_capacity = new_capacity;
        list->elems = new_elems;
    }

    
   
}


void arraylist_insert(arraylist_t *list, int index, int value) {
    assert(list != NULL);
    assert(index >= 0);
    assert(index <= list->elems_size);

    ensure_space_available(list);

#ifdef AL_INSERT_OPT
    // move everything after the index right by 1
    memmove(list->elems + index + 1, list->elems + index, 
             sizeof(int) * (list->elems_size - index));

#else

    // Make space for the new value being stored into the arraylist.
    for (int i = list->elems_size; i > index; i--) {
        list->elems[i] = list->elems[i - 1];
    }

#endif

    list->elems[index] = value;
    list->elems_size++;
}


void arraylist_append(arraylist_t *list, int value) {
    assert(list != NULL);

    ensure_space_available(list);

    list->elems[list->elems_size] = value;
    list->elems_size++;
}


bool arraylist_contains(arraylist_t *list, int value) {
    assert(list != NULL);

    for (int i = 0; i < list->elems_size; i++) {
        if (list->elems[i] == value)
            return true;
    }

    return false;
}


int al_compare_elems(const void *a, const void *b) {
    return *((int *) a) - *((int *) b);
}


void arraylist_sort(arraylist_t *list) {
    assert(list != NULL);

    // Use the C standard library sort implementation to do this.
    qsort(list->elems, list->elems_size, sizeof(int), al_compare_elems);
}


int * arraylist_iter(arraylist_t *list) {
    assert(list != NULL);
    return list->elems;
}


int arraylist_iter_get(arraylist_t *list, int *iter) {
    assert(list != NULL);
    assert(iter != NULL);
    assert(iter - list->elems >= 0);
    assert(iter - list->elems < list->elems_size);

    return *iter;
}


int * arraylist_iter_next(arraylist_t *list, int *iter) {
    assert(list != NULL);
    assert(iter != NULL);
    assert(iter - list->elems >= 0);
    assert(iter - list->elems < list->elems_size);

    int *next = iter + 1;
    if (next - list->elems >= list->elems_size)
        next = NULL;

    return next;
}


list_t * alloc_arraylist(void) {
    arraylist_t *list = malloc(sizeof(arraylist_t));
    if (!list)
        return NULL;

    bzero(list, sizeof(arraylist_t));

    list->size      = arraylist_size;
    list->get       = arraylist_get;
    list->clear     = arraylist_clear;
    list->insert    = arraylist_insert;
    list->append    = arraylist_append;
    list->contains  = arraylist_contains;

    list->sort      = arraylist_sort;

    list->iter      = arraylist_iter;
    list->iter_get  = arraylist_iter_get;
    list->iter_next = arraylist_iter_next;

    return (list_t *) list;
}


void free_arraylist(list_t *list) {
    if (list == NULL)
        return;

    arraylist_t *arraylist = (arraylist_t *) list;

    if (arraylist->elems != NULL)
        free(arraylist->elems);

    free(arraylist);
}
