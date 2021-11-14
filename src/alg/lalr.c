#include <stdlib.h>
#include <stdio.h>
#include "../structs/automa.h"
#include "../structs/LR1item.h"
#include "../structs/grammar.h"
#include "../structs/production.h"
#include "../structs/state.h"
#include "../structs/transition.h"
#include "lalr.h"
#include "../lib/utils.h"
#include "../lib/list.h"
#include "../lib/set.h"

/**
 * Appends to s->items (not in kernel) closure0(s-kernel) items
 * Notice: Usage of LR1items intead of LR0items is for simplicity
 */
void closure0(Grammar* g, State* s) { //TODO FIX
    for (int i = 0; i < s->kernelSize; i++) {
        LR1item* kItem = s->items->data[i];
        char c = getMarkedSymbol(kItem);
        if(!isTerminal(c))
            for (int i = 0; i < g->used; i++) {
                Production* p = g->data[i];
                if(p->driver == c)
                    insertList(s->items, createItem(p, 0));
            }
    }
}

/**
 * Computes closure0 and then adds the lookahead set
 */
void closure1(Grammar* g, State* s) {
    closure0(g, s);
    
    //TODO compute lookahead set
}

/**
 * fromState: state from which expand
 */
void expandAutoma(Grammar* g, Automa* a, int fromState) {
    State* currentState = a->nodes->data[fromState];
    int newStatesCount = 0;
    printf("Expanding state %d:\n", fromState);
    printState(currentState, fromState);

    for (int i = 0; i < currentState->items->used; i++) {
        LR1item* currItem = currentState->items->data[i];
        
        char symbol = getMarkedSymbol(currItem);

        if(validSymbol(symbol)) {
            Transition* t = createTransition(fromState, -1, symbol);
            //TODO is it right?
            char* t_key = serializeTransition(t, TRUE);
            
            if(set_add(a->transitions_keys, t_key) == SET_TRUE) { //t(from, symbol) not present
                State* newState = createState(currentState->items->used);
                
                for (int j = i; j < currentState->items->used; j++) {
                    LR1item* tmpItm = currentState->items->data[j];
                    if(getMarkedSymbol(tmpItm) == symbol) {
                        LR1item* itm = createItem(tmpItm->p, tmpItm->marker+1);
                        insertList(newState->items, itm); newState->kernelSize++;
                    }
                }

                int alreadyExists = FALSE, sameKernelI;
                //Find if exists a state with the same kernel TODO optimize with set
                for (sameKernelI = 0; sameKernelI < a->nodes->used; sameKernelI++) { //foreach state (aState) in the automa
                    if(sameKernel(a->nodes->data[sameKernelI], newState)) {
                        alreadyExists = TRUE;
                        break;
                    }
                }

                if(alreadyExists) {
                    printf("STATE EXISTS: %d\n", sameKernelI);
                    t->to = sameKernelI;
                }else {
                    printf("ADDING ");
                    printState(newState, -1);
                    insertList(a->nodes, newState);
                    t->to = a->nodes->used-1;
                    newStatesCount++;
                    closure1(g, newState);
                }
                
                insertList(a->transitions, t);
                printTransition(t);
            }
        }
    }//TODO CAPIRE PERCHE SU INPUT 3 SE NE SBATTE DELLO STATO 6 

    printf("Added %d new states\n\n", newStatesCount);
    for (int i = 1; i <= newStatesCount; i++) {
        
        expandAutoma(g, a, fromState+i);
    }
}

Automa* generateLALRautoma(Grammar* g) {
    Automa* a = malloc(sizeof *a);
    initAutoma(a, 10);//TODO change 10

    State* firstState = createState(1);

    //S' -> S (~ -> .S)
    LR1item* firstItem = createItem((Production*)g->data[0], 0); //DEFAULT BEHAVIOUR
    insertList(firstState->items, firstItem); firstState->kernelSize++;
    closure1(g, firstState);

    insertList(a->nodes, firstState);
    /*SimpleSet* ls = malloc(sizeof *ls);
        set_init(ls);
        lr1 = malloc(sizeof *lr1);
        lr1->marker = 0;
        lr1->p = p;
        lr1->ls = ls;*/

    expandAutoma(g, a, 0);
    

    return a;
}