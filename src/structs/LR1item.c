#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LR1item.h"
#include "production.h"
#include "../lib/utils.h"

void printItem(LR1item* item) {
    char s[strlen(item->p->body)+2];
    char c; int i = 0, j = 0, markerAdded = 0;
    while ((c = item->p->body[i]) != 0)
        if(!markerAdded && (markerAdded = i == item->marker))
            s[j++] = '.';
        else {
            s[j++] = c;
            i++;
        }
    if(markerAdded)
        s[j] = 0;
    else {
        s[j++] = '.';
        s[j] = 0;
    }
    printf(GREEN"LR(1)item(%p): [%c -> %s, {%s}]"E"\n", item, item->p->driver, s, mergeSetIntoString(item->ls));
}

LR1item* createItem(Production* p,int marker) {
    LR1item* i = malloc(sizeof *i);
    i->marker = marker;
    SimpleSet* ls = malloc(sizeof *ls); set_init(ls);
    i->ls = ls;
    i->p = p;
    return i;
}

char getMarkedSymbol(LR1item* i) {
    return i->p->body[i->marker];
}

int itemsEqual(LR1item* i1, LR1item* i2, int onlyLR0) {
    int sameLS = TRUE;
    if(!onlyLR0) sameLS = set_cmp(i1->ls, i2->ls) == SET_EQUAL;
    return i1->marker == i2->marker && i1->p == i2->p && sameLS;
}

int markerAtTheEnd(LR1item* i) {
    return i->marker >= (int)strlen(i->p->body);
}