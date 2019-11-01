#include "lists.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smallobj.h"
/*! A single node of the linked-list data structure. */
typedef struct ll_node_t {
    /*! The value stored in this linked-list node. */
    int value;

    /*! The next linked-list node in the sequence, or NULL if end of list. */
    struct ll_node_t *next;
} ll_node_t;


typedef struct linkedlist_t linkedlist_t;


/*! A singly linked list data structure. */
struct linkedlist_t {
    // Standard list operations

    int  (*size)     (linkedlist_t *list);
    int  (*get)      (linkedlist_t *list, int index);
    void (*clear)    (linkedlist_t *list);
    void (*insert)   (linkedlist_t *list, int index, int value);
    void (*append)   (linkedlist_t *list, int value);
    bool (*contains) (linkedlist_t *list, int value);

    void (*sort)     (linkedlist_t *list);

    // List-iteration operations

    ll_node_t * (*iter)      (linkedlist_t *list);
    int         (*iter_get)  (linkedlist_t *list, ll_node_t *iter);
    ll_node_t * (*iter_next) (linkedlist_t *list, ll_node_t *iter);

    // Internal linked-list state

    /*! The head of the linked list, or NULL if the list is empty. */    
    ll_node_t *head;

    /*! The tail of the linked list, or NULL if the list is empty. */
    ll_node_t *tail;

#ifdef LL_NODEPOOL
    /* Make a small-obj pool to take nodes from */
    smallobj_pool_t *pool;

#endif
};


ll_node_t * alloc_node(linkedlist_t *list) {
    assert(list != NULL);
#ifdef LL_NODEPOOL
    return so_alloc(list->pool);

#else
    return malloc(sizeof(ll_node_t));

#endif
}


int linkedlist_size(linkedlist_t *list) {
    int count = 0;
    ll_node_t *curr = list->head;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}


int linkedlist_get(linkedlist_t *list, int index) {
    assert(list != NULL);
    assert(index >= 0);

    ll_node_t *curr = list->head;
    while (curr != NULL && index != 0) {
        curr = curr->next;
        index--;
    }

    // If this fails, the index is past the end of the list
    assert(curr != NULL);
    return curr->value;
}


void linkedlist_clear(linkedlist_t *list) {
    assert(list != NULL);

    // Walk through the linked list and free each node.
    ll_node_t *node = list->head;
    while (node != NULL) {
        ll_node_t *next = node->next;
#ifdef LL_NODEPOOL
        so_free(list->pool, node);
#else
        free(node);
#endif
        node = next;
    }

    list->head = list->tail = NULL;
}


void linkedlist_insert(linkedlist_t *list, int index, int value) {
    ll_node_t *prev = NULL;
    ll_node_t *curr = list->head;

    for (int i = 0; i < index; i++) {
        if (curr == NULL) {
            // Index was invalid.
            assert(false);
        }

        prev = curr;
        curr = curr->next;
    }

    ll_node_t *node = alloc_node(list);
    if (!node) {
        fprintf(stderr, "ERROR:  Couldn't allocate linked-list node\n");
        abort();
    }

    node->value = value;

    if (prev == NULL)
        list->head = node;
    else
        prev->next = node;

    node->next = curr;
    if (curr == NULL)
        list->tail = NULL;
}


void linkedlist_append(linkedlist_t *list, int value) {
    ll_node_t *node = alloc_node(list);
    if (!node) {
        fprintf(stderr, "ERROR:  Couldn't allocate linked-list node\n");
        abort();
    }

    node->value = value;
    node->next = NULL;

    if (list->tail != NULL) {
        list->tail->next = node;
        list->tail = node;
    }
    else {
        list->head = node;
        list->tail = node;
    }
}


bool linkedlist_contains(linkedlist_t *list, int value) {
    ll_node_t *node = list->head;
    while (node != NULL) {
        if (node->value == value)
            return true;

        node = node->next;
    }

    return false;
}


ll_node_t * ll_quicksort(ll_node_t *nodes) {
    if (nodes->next == NULL)
        return nodes;

    // Peel off the first node from the rest of the list.  We will use it as
    // the pivot.

    ll_node_t *pivot = nodes;
    ll_node_t *rest = pivot->next;

    // Partition the list based on the value of the first.

    int pivot_value = pivot->value;
    ll_node_t *higher = NULL;
    ll_node_t *lower = NULL;

    while (rest != NULL) {
        ll_node_t *node = rest;
        rest = node->next;

        if (node->value >= pivot_value) {
            node->next = higher;
            higher = node;
        }
        else {
            node->next = lower;
            lower = node;
        }
    }

    // Sort each sublist.

    if (lower != NULL)
        lower = ll_quicksort(lower);

    if (higher != NULL)
        higher = ll_quicksort(higher);

    // Stitch all the parts together into a new sorted list.

    ll_node_t *result = NULL;

    if (lower != NULL) {
        // We have a list of "lower values."  Find the end and stitch the pivot
        // onto the end.
        ll_node_t *end_lower = lower;
        while (end_lower->next != NULL)
            end_lower = end_lower->next;

        end_lower->next = pivot;

        result = lower;
    }
    else {
        // No "lower values" list.
        result = pivot;
    }

    pivot->next = higher;

    return result;
}


void linkedlist_sort(linkedlist_t *list) {
    if (list->head == NULL)
        return;

    // In-place sort the list.
    list->head = ll_quicksort(list->head);

    // Find the new tail.
    ll_node_t * new_end = list->head;
    while (new_end->next != NULL)
        new_end = new_end->next;

    list->tail = list->head;
}


ll_node_t * linkedlist_iter(linkedlist_t *list) {
    assert(list != NULL);
    return list->head;
}


int linkedlist_iter_get(linkedlist_t *list, ll_node_t *iter) {
    assert(list != NULL);
    assert(iter != NULL);
    return iter->value;
}


ll_node_t * linkedlist_iter_next(linkedlist_t *list, ll_node_t *iter) {
    assert(list != NULL);
    assert(iter != NULL);
    return iter->next;
}


list_t * alloc_linkedlist(void) {
    linkedlist_t *list = malloc(sizeof(linkedlist_t));
    if (!list) {
        fprintf(stderr, "ERROR:  Couldn't allocate linked-list\n");
        abort();
    }

    bzero(list, sizeof(linkedlist_t));

    list->size      = linkedlist_size;
    list->get       = linkedlist_get;
    list->clear     = linkedlist_clear;
    list->insert    = linkedlist_insert;
    list->append    = linkedlist_append;
    list->contains  = linkedlist_contains;

    list->sort      = linkedlist_sort;

    list->iter      = linkedlist_iter;
    list->iter_get  = linkedlist_iter_get;
    list->iter_next = linkedlist_iter_next;

#ifdef LL_NODEPOOL
    
#ifdef LL_NODEPOOL_BADCACHE
    // The bad cache only fits one object per 64 bit cache line.
    list->pool = make_so_pool(4 * sizeof(ll_node_t), 10000);
#else
    // Make our pool.
    list->pool = make_so_pool(sizeof(ll_node_t), 10000);
#endif

#endif

    return (list_t *) list;
}


void free_linkedlist(list_t *list) {
    if (list == NULL)
        return;

    linkedlist_t *linkedlist = (linkedlist_t *) list;


    // Free the nodes of the linked list.
    linkedlist_clear(linkedlist);

#ifdef LL_NODEPOOL
    // Release the pool if necessary.
    release_so_pool(linkedlist->pool);
#endif

    // Free the linked list.
    free(list);
}

