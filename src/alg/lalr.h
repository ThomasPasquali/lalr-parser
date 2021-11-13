#ifndef LFC_LALR_H
#define LFC_LALR_H

#include "../structs/automa.h"
#include "../structs/LR1item.h"
#include "../lib/list.h"
#include "../structs/grammar.h"

List* closure0(Grammar*, List*);
List* closure1(Grammar*, List*);
Automa* generateLALRautoma(Grammar*);

#endif //LFC_LALR_H
