#ifndef LFC_GRAMMAR_H
#define LFC_GRAMMAR_H

#include "../lib/list.h"

/**
 * Notare che è una semplificazione di grammatica,
 * come è esplicitato nel README,
 * - terminali = {a,...,z}
 * - non-terminali = {A,...,Z}
 * - start-symbol: driver prima produzione in input
 */
typedef List Grammar; //List<Production>

#endif //LFC_GRAMMAR_H