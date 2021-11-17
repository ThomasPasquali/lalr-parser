#ifndef LFC_LALR_H
#define LFC_LALR_H

#include "../structs/automa.h"
#include "../structs/state.h"
#include "../structs/grammar.h"
#include "../structs/graph.h"

/**
 * Computes ONLY the closure0 for the state s
 */
void closure0(Grammar* g, State* s);
/**
 * Computes the closure0 for the state s and updates the lookahead sets
 */
void closure1(Grammar* g, State* s);
/**
 * returns: a LR(1) automa for the grammar g
 */
Automa* generateLR1automa(Grammar* g);
/**
 * returns: a NEW LRm(1) automa from the lr1A automa
 * Notice: items are referenced (NOT COPIED)
 */
Automa* generateLR1Mautoma(Automa* lr1A, Graph* lr1G);

#endif //LFC_LALR_H
