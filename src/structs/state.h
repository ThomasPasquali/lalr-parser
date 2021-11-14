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

#endif //LFC_STATE_H