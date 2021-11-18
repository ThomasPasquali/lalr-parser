#ifndef LFC_LR1ITEM_H
#define LFC_LR1ITEM_H

#include "production.h"
#include "../lib/set.h"
#include "../lib/list.h"

typedef struct {
    Production* p;
    int marker;
    SimpleSet* ls; //Lookahead Set
    uint32_t hash; //Optimization
} LR1item;

void printItem(LR1item*);
LR1item* createItem(Production*,int);
char getMarkedSymbol(LR1item*);
int itemsEqual(LR1item*, LR1item*, int onlyLR0);
int markerAtTheEnd(LR1item*);
void destroyItem(LR1item*);
/**
 * This hash is based on ls elemets (no order), marker and production
 */
uint64_t getItemHash(LR1item* i);

#endif //LFC_LR1ITEM_H