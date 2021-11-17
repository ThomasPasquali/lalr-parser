#ifndef LFC_INPUTPARSER_H
#define LFC_INPUTPARSER_H

#include <stdio.h>
#include "list.h"
#include "../structs/grammar.h"

#define INPUT_DRIVER_BODY_SEP "->"

Grammar* parseInput(FILE*);
void appendProductions(char* production, List* productions);

#endif //LFC_INPUTPARSER_H
