#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

void closure0i(Grammar* g, State* s, LR1item* i) {
    char c = getMarkedSymbol(i);
    if(!isTerminal(c))
        for (int i = 0; i < g->used; i++) {
            Production* p = g->data[i];
            if(p->driver == c) {
                LR1item* newItem = createItem(p, 0);
                if(!kernelExpansionContains(s, newItem)) { //Avoid duplicates due to recursion
                    insertList(s->items, newItem);
                    closure0i(g, s, newItem);
                }else free(newItem);                
            }
        }
}

/**
 * Appends to s->items (not in kernel) closure0(s-kernel) items
 * Notice: Usage of LR1items intead of LR0items is for simplicity
 */
void closure0(Grammar* g, State* s) {
    for (int i = 0; i < s->kernelSize; i++) {
        LR1item* kItem = s->items->data[i];
        closure0i(g, s, kItem);
    }
}

/**
 * Computes closure0 and then adds the lookahead set
 */
void closure1(Grammar* g, State* s) {
    closure0(g, s);
    void** items = s->items->data;
    int i = 0;
    int n = s->items->used;
    int marked[n]; 
    for (; i < n; i++) marked[i] = FALSE;
    uint64_t j;
    i = 0;

    //printf("CLOSURE 0\n"); printState(s, -3);
    
    while(i < n) { //while exists a non-marked item
        if(!marked[i]) {
            marked[i] = TRUE;
            LR1item* item = items[i];
            char B = getMarkedSymbol(item);
            if(isNonTerminal(B)) {

                //Computing delta1
                SimpleSet* newLS = malloc(sizeof *newLS); set_init(newLS); //delta1
                uint64_t dsLen = set_length(item->ls);
                char** ds = set_to_array(item->ls, &dsLen); //Elements of items->ls

                //Extracting beta
                int betaLen = strlen(item->p->body)-item->marker;
                char* beta = malloc(sizeof(char)*betaLen);
                strncpy(beta, item->p->body+item->marker+1, betaLen);

                //printf("body:%s, beta:%s\n", item->p->body, beta);

                for (j = 0; j < dsLen; j++) {
                    //Creating beta d
                    char* bd = malloc(sizeof(char)*betaLen+1);
                    strncpy(bd, beta, betaLen);
                    bd[betaLen-1] = ds[j][0];
                    bd[betaLen] = 0;

                    //printf("bd:%s|\n",bd);

                    //Adding to delta1
                    SimpleSet* firstbd = first(g, bd);
                    //printf("first(bd)={%s}|\n", mergeSetIntoString(firstbd));
                    SimpleSet* tmp = malloc(sizeof *tmp); set_init(tmp);
                    set_union(tmp, firstbd, newLS);

                    free(bd); free(firstbd); free(newLS);
                    newLS = tmp;
                }

                //printf("delta1:%s|\n", mergeSetIntoString(newLS));
                free(beta);

                //Looking for items to update
                for (j = 0; j < (uint64_t)s->items->used; j++) {
                    LR1item* itm = items[j];
                    if(itm->p->driver == B && set_is_subset(newLS, itm->ls) == SET_FALSE) { //Found item to update
                        marked[j] = FALSE;

                        //Updating ls
                        SimpleSet* tmp = malloc(sizeof *tmp); set_init(tmp);
                        set_union(tmp, itm->ls, newLS);

                        free(itm->ls);
                        itm->ls = tmp;
                    }
                }

                free(newLS);

            }
            i = (i+1)%n;
        }else 
            i++;        
    }

    //printf("END CLOSURE 1\n"); printState(s, -3);
}

/**
 * fromState: state from which expand
 */
void expandAutoma(Grammar* g, Automa* a, int fromState) { 
    State* currentState = a->nodes->data[fromState];
    int newStatesCount = 0;
    //printf("Expanding state %d:\n", fromState);
    //printState(currentState, fromState);

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
                        SimpleSet* tmp = malloc(sizeof *tmp); set_init(tmp);
                        set_union(tmp, itm->ls, tmpItm->ls);
                        free(itm->ls);
                        itm->ls = tmp;
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
                    //printf("STATE EXISTS: %d\n", sameKernelI);
                    t->to = sameKernelI;
                }else {
                    //printf("ADDING ");
                    //printState(newState, -1);
                    insertList(a->nodes, newState);
                    t->to = a->nodes->used-1;
                    newStatesCount++;
                    closure1(g, newState);
                }
                
                insertList(a->transitions, t);
                //printTransition(t);
            }
        }
    }

    //if(fromState == 3) exit(0);
    /*printf("Added %d new states: ", newStatesCount);
    for (int i = a->nodes->used-newStatesCount; i < a->nodes->used; i++)
        printf("%d ",i);
    printf("\n\n");*/
    for (int i = a->nodes->used-newStatesCount; i < a->nodes->used; i++)
        expandAutoma(g, a, i);
    
}

Automa* generateLALRautoma(Grammar* g) {
    Automa* a = malloc(sizeof *a);
    initAutoma(a, 10);//TODO change 10

    State* firstState = createState(1);

    //[S' -> S, {$}] (~ -> .S)
    LR1item* firstItem = createItem((Production*)g->data[0], 0); //DEFAULT BEHAVIOUR
    set_add(firstItem->ls, ctos(EOL));
    insertList(firstState->items, firstItem); firstState->kernelSize++;
    closure1(g, firstState);
    insertList(a->nodes, firstState);

    expandAutoma(g, a, 0);

    return a;
}