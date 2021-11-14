#ifndef LFC_LALR_H
#define LFC_LALR_H

#include "../structs/automa.h"
#include "../structs/state.h"
#include "../structs/grammar.h"

void closure0(Grammar*, State*);
void closure1(Grammar*, State*);
Automa* generateLALRautoma(Grammar*);

#endif //LFC_LALR_H
