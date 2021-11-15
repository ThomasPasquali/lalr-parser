#include <stdio.h>

#include "latexOutput.h"

#include "../structs/automa.h"
#include "../structs/state.h"
#include "../structs/transition.h"

#include "../lib/set.h"
#include "../lib/utils.h"

void appendToFile(FILE* src, FILE* dst) {
    char byte;
    while(!feof(src)) {
        fread(&byte, sizeof(char), 1, src);
        fwrite(&byte, sizeof(char), 1, dst);
    }
}

void ouputLatexAutoma(Automa* a) {
    FILE *src = fopen(PRE_FILENAME, "rb");
    FILE *finalFile = fopen(OUT_FILENAME, "wb+");

    appendToFile(src, finalFile);
    fclose(src);

    for (int i = 0; i < a->nodes->used; i++) {
        State* s = a->nodes->data[i];
        fprintf(finalFile, "\\node[state%s] (%d) ", (isFinal(s)?", double":""), i);
        if(i != 0) fprintf(finalFile, (i%2)==0?"[below of = %d]":"[right of = %d]", i-1);
        fprintf(finalFile, "{%d};\n", i);
    }

    for (int i = 0; i < a->transitions->used; i++) {
        Transition* t = a->transitions->data[i];
        fprintf(finalFile, 
            "\\draw (%d) to node {%c} (%d);\n", 
            t->from, t->symbol, t->to);
    }

    src = fopen(POST_FILENAME, "rb");
    appendToFile(src, finalFile);
    fclose(src);
    
    fclose(finalFile);
}