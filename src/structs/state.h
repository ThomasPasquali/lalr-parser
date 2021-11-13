#ifndef LFC_STATE_H
#define LFC_STATE_H

#include "LR1item.h"

typedef struct {
    List* kernel;   //<LR1item>
    List* rest;     //<LR1item>
} State;

void printState(State*);
State* createState(int initKernelLen, int initRestLen);

#endif //LFC_STATE_H