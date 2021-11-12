#include <stdio.h>
#include "lib/inputParser.h"
#include "structs/LR1item.h"
/*
 * generazione dell'automa caratteristico per il parsing LALR
 * l'output deve comprendere le info deducibili dall'automa e necessarie alla compilazione della tabella di parsing, in particolare:
    - qual e' lo stato iniziale
    - quali reducing item appartengono a quale stato finale
 */

int main(int argc, char const *argv[]) {
    List* items = parseInput(stdin);
    for (int i = 0; i < items->used; i++) {
        print(items->data[i]);
    }
    return 0;
}