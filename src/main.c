#include <stdio.h>
#include <locale.h>

#include "lib/inputParser.h"
#include "lib/latexOutput.h"
#include "lib/utils.h"

#include "structs/LR1item.h"
#include "structs/automa.h"
#include "structs/state.h"
#include "structs/transition.h"

#include "alg/lalr.h"

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
        //printState((State*)automa->nodes->data[i], i);
    }

    for (int i = 0; i < automa->transitions->used; i++) {
        //printTransition((Transition*)automa->transitions->data[i]);
    }
    /*uint64_t len = set_length(automa->transitions);
    char** transitions = set_to_array(automa->transitions, &len);
    for (uint64_t i = 0; i < len; i++) {
        printTransition((Transition*)transitions[i]);
    }*/

    ouputLatexAutoma(automa);

    return 0;
}