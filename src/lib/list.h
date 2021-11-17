#ifndef LFC_LIST_H
#define LFC_LIST_H

typedef struct {
    void** data;
    int used;
    int size;
} List;

void initList(List*, int);
void insertList(List*, void*);
/**
 * Notice that the second list is freed
 */
void mergeLists(List*, List*);
void freeList(List*);

#endif //LFC_LIST_H
