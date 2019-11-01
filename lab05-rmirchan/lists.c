#include "lists.h"

void list_clear(list_t *list) {
    list->clear(list);
}

int list_size(list_t *list) {
    return list->size(list);
}

int list_get(list_t *list, int index) {
    return list->get(list, index);
}

void list_insert(list_t *list, int index, int value) {
    list->insert(list, index, value);
}

void list_append(list_t *list, int value) {
    list->append(list, value);
}

bool list_contains(list_t *list, int value) {
    return list->contains(list, value);
}


void list_sort(list_t *list) {
    return list->sort(list);
}


list_iter_t * list_iter(list_t *list) {
    return list->iter(list);
}

int list_iter_get(list_t *list, list_iter_t *iter) {
    return list->iter_get(list, iter);
}

list_iter_t * list_iter_next(list_t *list, list_iter_t *iter) {
    return list->iter_next(list, iter);
}
