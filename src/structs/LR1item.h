#ifndef LFC_LR1ITEM_H
#define LFC_LR1ITEM_H

#include "production.h"
#include "../lib/set.h"
#include "../lib/list.h"

typedef struct {
    Production* p;
    int marker;
    SimpleSet* ls; //Lookahead Set
} LR1item;

void printItem(LR1item*);
LR1item* createItem(Production*,int);
char getMarkedSymbol(LR1item*);
int itemsEqual(LR1item*, LR1item*);

#endif //LFC_LR1ITEM_H