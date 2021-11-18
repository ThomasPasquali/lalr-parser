#ifndef LFC_TRANSITION_H
#define LFC_TRANSITION_H

#include "../lib/list.h"

typedef struct {
    int from;
    int to;
    char symbol;
} Transition;

Transition* createTransition(int from, int to, char symbol);
/**
 * returns: if(onlyKey) "<from> <symbol> <to>"
 *          else        "<from> <symbol>"
 */
char* serializeTransition(Transition*, int onlyKey);
/**
 * Parses a string like "<from> <symbol> <to>"
 */
Transition* parseTransition(char*);
void printTransition(Transition*);

#endif //LFC_TRANSITION_H
