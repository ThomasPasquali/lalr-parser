#include "production.h"
#include "../lib/set.h"
#include "LR1item.h"
#include "../lib/utils.h"
#include "../lib/list.h"
#include <stdio.h>
#include <stdlib.h>

int count(char c, char* s) {
    int count = 0, i = 0;
    while(s[i] != 0)
        if(s[i++] == c)
            count++;
    return count;
}

void appendProductions(char* production, List* items) {
    char driver = production[0];
    
    int bodiesLen;
    char** bodies = str_split(production+3, '|', &bodiesLen);//TODO sintax check
    
    LR1item* lr1;
    Production* p;
    for (int i = 0; i < bodiesLen; i++) {
        p = malloc(sizeof *p);
        p->driver = driver;
        p->body = bodies[i];

        SimpleSet* ls = malloc(sizeof *ls);
        set_init(ls);

        lr1 = malloc(sizeof *lr1);
        lr1->marker = 3;
        lr1->p = p;
        lr1->ls = ls;

        insertList(items, lr1);
    }
}