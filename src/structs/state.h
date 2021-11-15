#ifndef LFC_STATE_H
#define LFC_STATE_H

#include "LR1item.h"

typedef struct {
    List* items;   //<LR1item>
    int kernelSize;
} State;

void printState(State*, int);
State* createState(int initSize);
int sameKernel(State*, State*);
/**
 * returns: TRUE if i is equals to one of the kernel expansion items
 */
int kernelExpansionContains(State* s, LR1item* i);
/**
 * returns: TRUE if exists an item has the marker at the end of the body of the production
 */
int isFinal(State*);

#endif //LFC_STATE_H