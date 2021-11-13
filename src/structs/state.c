#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "LR1item.h"

void printState(State* s) {
    for(int i = 0; i < s->kernel->used; i++)
        printItem((LR1item*)s->kernel->data[i]);
    printf("--------------\n");
    for(int i = 0; i < s->rest->used; i++)
        printItem((LR1item*)s->rest->data[i]);
}

State* createState(int initKernelLen, int initRestLen) {
    State* s = malloc(sizeof *s);
    List* l = malloc(sizeof *l);
    initList(l, initKernelLen);
    s->kernel = l;
    l = malloc(sizeof *l);
    initList(l, initRestLen);
    s->rest = l;
    return s;
}