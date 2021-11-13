#include <stdlib.h>
#include <stdio.h>
#include "../structs/automa.h"
#include "../structs/LR1item.h"
#include "../structs/grammar.h"
#include "../structs/production.h"
#include "../structs/state.h"
#include "lalr.h"
#include "../lib/utils.h"
#include "../lib/list.h"

/**
 * Usage of LR1items intead of LR0items is for simplicity
 * 
 * kernel: List*<LR1item>
 * 
 * returns: List*<LR1item> the rest of the items of the closure0
 */
List* closure0(Grammar* g, List* kernel) {
    List* items = malloc(sizeof *items);
    initList(items, kernel->used);
    
    for (int i = 0; i < kernel->used; i++) {
        LR1item* item = kernel->data[i];
        char c = item->p->body[item->marker];
        if(!isTerminal(c)) {
            for (int i = 0; i < g->used; i++) {
                Production* p = g->data[i];
                if(p->driver == c) {
                    LR1item* tmp = malloc(sizeof *tmp);
                    //TODO tmp->ls
                    tmp->marker = 0;
                    tmp->p = p;
                    insertList(items, tmp);
                }
            }
        }
    }
    return items;
}

/**
 * List*<LR1item> kernel
 * 
 * returns: List*<LR1item> the rest of the items of the closure1
 */
List* closure1(Grammar* g, List* kernel) {
    List* closure = closure0(g, kernel);
    
    //TODO compute lookahead set

    return closure;
}

/**
 * states: List*<State>
 */
void createNewStates(List* states, State* state) {
    for (int i = 0; i < state->kernel->used; i++) {
        LR1item* kItem = state->kernel->data[i];
        
        char symbol = kItem->p->body[kItem->marker];
        printf("Tr(%d, %c)\n", i, symbol); //TODO

        State* newState = createState(state->kernel->used, 1);
        
        LR1item* itm = createItem(kItem->p, kItem->marker+1);
        insertList(newState->kernel, itm);
        
        for (int i = 0; i < state->rest->used; i++) {
            LR1item* rItem = state->rest->data[i];
            if(getMarkedSymbol(rItem) == symbol) {
                itm = createItem(rItem->p, rItem->marker+1);
                insertList(newState->kernel, itm);
            }
            
        }
        
        insertList(states, newState);
    }
}

Automa* generateLALRautoma(Grammar* g) {
    Automa* a = malloc(sizeof *a);
    initAutoma(a, 10);//TODO change 10

    State* firstState = createState(1, 0);

    //S' -> S (~ -> .S)
    LR1item* firstItem = createItem((Production*)g->data[0], 0); //DEFAULT BEHAVIOUR
    insertList(firstState->kernel, firstItem);

    free(firstState->rest);
    firstState->rest = closure1(g, firstState->kernel);

    insertList(a->nodes, firstState);
    /*SimpleSet* ls = malloc(sizeof *ls);
        set_init(ls);
        lr1 = malloc(sizeof *lr1);
        lr1->marker = 0;
        lr1->p = p;
        lr1->ls = ls;*/

    createNewStates(a->nodes, firstState);
    

    return a;
}