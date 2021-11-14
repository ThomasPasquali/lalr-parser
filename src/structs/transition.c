#include "transition.h"
#include "../lib/utils.h"
#include <stdlib.h>
#include <stdio.h>

//Supposing not having more than 999 states
#define MAX_SERIAL_STR_LEN 10 //3*2(from, to) + 1(symbol) + 2(spaces) + 1(\0)

Transition* createTransition(int from, int to, char symbol) {
    Transition* t = malloc(sizeof *t); 
    t->from = from;
    t->to = to;
    t->symbol = symbol;
    return t;
}

char* serializeTransition(Transition* t, int onlyKey) {
    char* s = malloc(sizeof *s * MAX_SERIAL_STR_LEN);
    int i;
    if(onlyKey) i=sprintf(s, "%d %c %d", t->from, t->symbol, t->to);
    else        i=sprintf(s, "%d %c", t->from, t->symbol);
    s[i] = 0;
    return s;
}
Transition* parseTransition(char* s) {
    int from, to;
    char symbol;
    sscanf(s, "%d %c %d", &from, &symbol, &to);
    return createTransition(from, to, symbol);
}

void printTransition(Transition* t) {
    printf(MAGENTA"Transition: (%d, %c) = %d"E"\n", t->from, t->symbol, t->to);
}