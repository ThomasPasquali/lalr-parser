#ifndef LFC_LR1ITEM_H
#define LFC_LR1ITEM_H

#include "production.h"
#include "../lib/set.h"

typedef struct {
    Production* p;
    int marker;
    SimpleSet* ls; //Lookahead Set
} LR1item;

void print(LR1item*);


#endif //LFC_LR1ITEM_H