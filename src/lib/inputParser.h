#ifndef LFC_INPUTPARSER_H
#define LFC_INPUTPARSER_H

#include <stdio.h>
#include "list.h"

List* parseInput(FILE*);
void appendProductions(char* production, List* productions);

#endif //LFC_INPUTPARSER_H
