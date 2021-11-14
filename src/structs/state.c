#include <stdio.h>
#include <stdlib.h>
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

    return s;
}

int sameKernel(State* s1, State* s2) {
    int i, j;
    if(s1->kernelSize == s2->kernelSize) { //if s1 and s2 have same kernel size
        //Nested loops for order
        for(i = 0; i < s1->kernelSize; i++) { //foreach item in s1
            for(j = 0; j < s2->kernelSize; j++) //foreach item in s2
                if(itemsEqual(s1->items->data[i], s2->items->data[j]))
                    break;
            if(j >= s2->kernelSize)
                return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}