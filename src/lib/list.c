#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "../structs/production.h"

void initList(List* a, int initialSize) {
    a->data = malloc(initialSize * sizeof(void*));
    a->used = 0;
    a->size = initialSize;
}

void insertList(List* a, void* element) {
    if (a->used == a->size) {
        a->size *= 2;
        a->data = realloc(a->data, a->size * sizeof(void*));
    }
    //printf("ADDING: %p\n", element);
    a->data[a->used++]= element;
}

/**
 * Notice that the second list is freed
 */
void mergeLists(List* l1, List* l2) {
    for (int i = 0; i < l2->size; i++)
        insertList(l1, l2->data[i]);
    freeList(l2);
}

void freeList(List* a) {
    free(a->data);
    a->data = NULL;
    a->used = a->size = 0;
}
