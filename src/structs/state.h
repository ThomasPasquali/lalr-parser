#ifndef LFC_STATE_H
#define LFC_STATE_H

#include "LR1item.h"

typedef struct {
    List* items;   //<LR1item>
    int kernelSize;
    uint64_t hash; //Optimization
} State;

void printState(State*, int);
State* createState(int initSize);
void destroyState(State*);
int sameKernel(State*, State*, int onlyLR0);
/**
 * Computes hash only if hash was 0
 */
int sameKernelHash(State*, State*);
/**
 * returns: TRUE if i is equals to one of the kernel expansion items
 */
int kernelExpansionContains(State* s, LR1item* i, int onlyLR0);
/**
 * returns: TRUE if exists an item has the marker at the end of the body of the production
 */
int isFinal(State*);
/**
 * The result hash is based on every kernel item hash
 */
uint64_t getStateKernelHash(State*);

#endif //LFC_STATE_H