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

void destroyItem(LR1item* i) {
    set_destroy(i->ls);
    free(i);
}

uint64_t getItemHash(LR1item* i) {
    if(i->hash != 0) return i->hash;

    uint64_t lsLen = set_length(i->ls);
    char** ls = set_to_array(i->ls, &lsLen);

    uint64_t hash = 5381;

    /*Jenkins one at a time 
    for(uint64_t i = 0; i < lsLen; ++i) {
        hash += ls[i][0]; hash += (hash << 10); hash ^= (hash >> 6);
    }
    hash += (hash << 3); hash ^= (hash >> 11); hash += (hash << 15);*/

    /*for(uint64_t i = 0; i < lsLen; ++i)
        hash = ((hash << 5) + hash) + ls[i][0];

    hash += ((hash << 5) + hash) + i->marker;
    hash += ((hash << 5) + hash) + i->p->driver;

    char c; char* str = i->p->body;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;*/

    for(uint64_t i = 0; i < lsLen; ++i) {
        hash ^= ls[i][0];
        hash *= 0x5bd1e9955bd1e995;
        hash ^= hash >> 47;
    }

    hash ^= i->marker; hash *= 0x5bd1e9955bd1e995; hash ^= hash >> 47;
    hash ^= i->p->driver; hash *= 0x5bd1e9955bd1e995; hash ^= hash >> 47;

    char c; char* str = i->p->body;
    while ((c = *str++)) {
        hash ^= c;
        hash *= 0x5bd1e9955bd1e995;
        hash ^= hash >> 47;
    }

    return (i->hash = hash);
}