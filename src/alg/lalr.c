#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../structs/automa.h"
#include "../structs/LR1item.h"
#include "../structs/grammar.h"
#include "../structs/production.h"
#include "../structs/state.h"
#include "../structs/transition.h"
#include "../structs/graph.h"
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
                while(isEpsilon(getMarkedSymbol(newItem))) newItem->marker++;
                if(!kernelExpansionContains(s, newItem, FALSE)) { //Avoid duplicates due to recursion
                    insertList(s->items, newItem);
                    closure0i(g, s, newItem);
                }else destroyItem(newItem);                
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
    int n = s->items->used;
    uint64_t j;

    //printf("CLOSURE 0\n"); printState(s, -3);
    
    for(int i = 0; i < n; i++) { //while exists a non-marked item

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

                free(bd); set_destroy(firstbd); set_destroy(newLS);
                newLS = tmp;
            }
            //printf("delta1:%s|\n", mergeSetIntoString(newLS));
            free(beta);

            //Looking for items to update
            for (j = s->kernelSize; j < (uint64_t)s->items->used; j++) {
                LR1item* itm = items[j];
                if(itm->p->driver == B && set_is_subset(newLS, itm->ls) == SET_FALSE) { //Found item to update
                    //Updating ls
                    SimpleSet* tmp = malloc(sizeof *tmp); set_init(tmp);
                    set_union(tmp, itm->ls, newLS);

                    set_destroy(itm->ls);
                    itm->ls = tmp;
                }
            }
            free(newLS);
        }
        
    }

    //printf("END CLOSURE 1\n"); printState(s, -3);
}

/**
 * fromState: state from which expand
 */
void expandLR1Automa(Grammar* g, Automa* a, int fromState, int useHashKernelComp) { 
    State* currentState = a->nodes->data[fromState];
    int newStatesCount = 0;
    //printf("Expanding state %d:\n", fromState);
    //printState(currentState, fromState);

    for (int i = 0; i < currentState->items->used; i++) {
        LR1item* currItem = currentState->items->data[i];
        
        char symbol = getMarkedSymbol(currItem);

        if(validSymbol(symbol)) {
            Transition* t = createTransition(fromState, -1, symbol);
            
            if(set_add(a->transitions_keys, serializeTransition(t, TRUE)) == SET_TRUE) { //t(from, symbol) not present
                State* newState = createState(currentState->items->used);
                
                for (int j = i; j < currentState->items->used; j++) {
                    LR1item* tmpItm = currentState->items->data[j];
                    if(getMarkedSymbol(tmpItm) == symbol) {
                        LR1item* itm = createItem(tmpItm->p, tmpItm->marker+1);
                        SimpleSet* tmp = malloc(sizeof *tmp); set_init(tmp);
                        set_union(tmp, itm->ls, tmpItm->ls);
                        set_destroy(itm->ls);
                        itm->ls = tmp;
                        insertList(newState->items, itm); newState->kernelSize++;
                    }
                }

                int alreadyExists = FALSE, sameKernelI;
                //Find if exists a state with the same kernel
                for (sameKernelI = 0; sameKernelI < a->nodes->used; sameKernelI++) { //foreach state (aState) in the automa
                    //if(useHashKernelComp && sameKernelHash(a->nodes->data[sameKernelI], newState)) { printState(a->nodes->data[sameKernelI], sameKernelI); printState(newState, -1); printf("%s\n%ld\n%ld\n------------\n", sameKernelHash(a->nodes->data[sameKernelI], newState)?"SI":"NO", ((State*)a->nodes->data[sameKernelI])->hash, newState->hash); }
                    if(
                        (useHashKernelComp && sameKernelHash(a->nodes->data[sameKernelI], newState))       //if program launched with optimization flag
                        || 
                        (!useHashKernelComp && sameKernel(a->nodes->data[sameKernelI], newState, FALSE))   //otherwise
                    ) {
                        alreadyExists = TRUE;
                        break;
                    }
                }

                if(alreadyExists) { //printf("STATE EXISTS: %d\n", sameKernelI);
                    t->to = sameKernelI;
                    destroyState(newState);
                }else {
                    //printf("ADDING "); printState(newState, -1);
                    insertList(a->nodes, newState);
                    t->to = a->nodes->used-1;
                    newStatesCount++;
                    closure1(g, newState);
                    newState->hash = 0;
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
        expandLR1Automa(g, a, i, useHashKernelComp);
}

Automa* generateLR1automa(Grammar* g, int useHashKernelComp) {
    Automa* a = malloc(sizeof *a);
    initAutoma(a, g->used*2);

    State* firstState = createState(1);

    //[S' -> S, {$}] (~ -> .S)
    LR1item* firstItem = createItem((Production*)g->data[0], 0); //DEFAULT BEHAVIOUR
    set_add(firstItem->ls, ctos(EOW));
    insertList(firstState->items, firstItem); firstState->kernelSize++;
    closure1(g, firstState);
    insertList(a->nodes, firstState);

    expandLR1Automa(g, a, 0, useHashKernelComp);

    return a;
}

Automa* generateLR1Mautoma(Automa* lr1A, Graph* lr1G) {
    Automa* mergedA = malloc(sizeof *mergedA);
    initAutoma(mergedA, lr1A->nodes->used);
    int mI;

    //mergesToDo[lr1A state index] = mergedA state terget index
    int mergesToDo[lr1A->nodes->used]; //no need to init

    for (int i = 0; i < lr1A->nodes->used; i++) {
        State* lr1S = lr1A->nodes->data[i];
        
        /*printf("---------------\n");
        printState(lr1S, i);
        printf("++++++++++++++\n");*/

        for(mI = 0; mI < mergedA->nodes->used; mI++) { //Finding if exists a mergedA node with same LR(0) kernel
            //if(sameKernel(mergedA->nodes->data[mI], lr1S, TRUE)) printState(mergedA->nodes->data[mI], mI);
            if(sameKernel(mergedA->nodes->data[mI], lr1S, TRUE)) //Comparing LR(0) kernel
                break;
        }

        if(mI >= mergedA->nodes->used) { //Add new state to mergedA (only kernel is merged for now)
            //printf("State %-3d goes to NEW state %d\n", i, mI);
            State* newS = createState(lr1S->kernelSize);
            for (int j = 0; j < lr1S->kernelSize; j++)
                insertList(newS->items, lr1S->items->data[j]);
            newS->kernelSize += lr1S->kernelSize;
            insertList(mergedA->nodes, newS);
        }else{ //Need to merge
            //printf("State %-3d goes to EXISTING state %d\n", i, mI);
            State* mS = mergedA->nodes->data[mI];
            for (int j = 0; j < lr1S->kernelSize; j++)
                insertList(mS->items, lr1S->items->data[j]);
            mS->kernelSize += lr1S->kernelSize;
        } 
        
        mergesToDo[i] = mI;
    }
    
    for (int i = 0; i < lr1A->nodes->used; i++) { //Do kernel expansion merges
        State* fromS = lr1A->nodes->data[i];
        State* toS = mergedA->nodes->data[mergesToDo[i]];
        for (int j = fromS->kernelSize; j < fromS->items->used; j++)
            insertList(toS->items, fromS->items->data[j]);

        Node* lr1Node = lr1G->nodes[i].head;
        //Update transitions
        while(lr1Node) {
            Transition* t = createTransition(mergesToDo[i], mergesToDo[lr1Node->dest], lr1Node->symbol);
            if(set_add(mergedA->transitions_keys, serializeTransition(t, TRUE)) == SET_TRUE)
                insertList(mergedA->transitions, t);
            else free(t);
            lr1Node = lr1Node->next;
        }        
    }

    return mergedA;
}