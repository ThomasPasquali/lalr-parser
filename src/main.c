#include <stdio.h>
#include "lib/inputParser.h"
#include "structs/LR1item.h"
#include "structs/automa.h"
#include "structs/state.h"
#include "alg/lalr.h"
#include <locale.h>
/*
 * generazione dell'automa caratteristico per il parsing LALR
 * l'output deve comprendere le info deducibili dall'automa e necessarie alla compilazione della tabella di parsing, in particolare:
    - qual e' lo stato iniziale
    - quali reducing item appartengono a quale stato finale
 */

int main(/*int argc, char const *argv[]*/) {
    setlocale(LC_ALL, "");
    List* items = parseInput(stdin);
    Automa* automa = generateLALRautoma(items);

    printf("AUTOMA:\n\n");
    for (int i = 0; i < automa->nodes->used; i++) {
        printf("State %d:\n", i);
        printState((State*)automa->nodes->data[i]);
        printf("\n");
    }
        

    return 0;
}