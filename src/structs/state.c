#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state.h"
#include "LR1item.h"
#include "../lib/utils.h"

void printState(State* s, int n) {
    printf(BLUE"State %d:"E"\n", n);
    for(int i = 0; i < s->items->used; i++) {
        if(i == s->kernelSize) printf(BLUE"--------------"E"\n");
        printItem((LR1item*)s->items->data[i]);
    }
    if(s->kernelSize == s->items->used) printf(BLUE"--------------"E"\n");
    printf("\n");
}

State* createState(int initSize) {
    State* s = malloc(sizeof *s);

    List* l = malloc(sizeof *l);
    initList(l, initSize);
    s->items = l;
    s->kernelSize = 0;
    s->hash = 0;

    return s;
}

void destroyState(State* s) {
    freeList(s->items);
    free(s);
}

int sameKernel(State* s1, State* s2, int onlyLR0) {
    int i, j;
    if(s1->kernelSize == s2->kernelSize) { //if s1 and s2 have same kernel size
        //Nested loops for order
        for(i = 0; i < s1->kernelSize; i++) { //foreach item in s1
            for(j = 0; j < s2->kernelSize; j++) //foreach item in s2
                if(itemsEqual(s1->items->data[i], s2->items->data[j], onlyLR0))
                    break;
            if(j >= s2->kernelSize)
                return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

uint64_t getStateKernelHash(State* s) {
    if(s->kernelSize <= 0) return 0;
    uint64_t hash = getItemHash(s->items->data[0]);
    for (int i = 1; i < s->kernelSize; i++)
        hash ^= getItemHash(s->items->data[i]);
    return hash;
}

int sameKernelHash(State* s1, State* s2) {
    if(s1->kernelSize == s2->kernelSize) {
        if(s1->hash == 0) s1->hash = getStateKernelHash(s1);
        if(s2->hash == 0) s2->hash = getStateKernelHash(s2);
        return s1->hash == s2->hash;
    }
    return FALSE;
}

int kernelExpansionContains(State* s, LR1item* item, int onlyLR0) { //TODO OPTIMIZE WITH HASH
    for(int i = s->kernelSize; i < s->items->used; i++) 
        if(itemsEqual(s->items->data[i], item, onlyLR0))
            return TRUE;
    return FALSE;
}

int isFinal(State* s) {
    for(int i = 0; i < s->items->used; i++)
        if(markerAtTheEnd(s->items->data[i]))
            return TRUE;
    return FALSE;
}