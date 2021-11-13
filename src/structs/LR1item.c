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
    printf("LR(1)item(%p): %c -> %s\n", item, item->p->driver, s);
}

LR1item* createItem(Production* p,int marker) {
    LR1item* i = malloc(sizeof *i);
    i->marker = marker;
    //TODO i->ls
    i->p = p;
    return i;
}

char getMarkedSymbol(LR1item* i) {
    return i->p->body[i->marker];
}