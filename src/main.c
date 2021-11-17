#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib/inputParser.h"
#include "lib/output.h"
#include "lib/utils.h"

#include "structs/LR1item.h"
#include "structs/automa.h"
#include "structs/state.h"
#include "structs/transition.h"
#include "structs/grammar.h"
#include "structs/graph.h"

#include "alg/lalr.h"

#define ARGS_LEN 4
enum {
    RAW_OUTPUT = 1 << 0, // 1_2
    TEX_OUTPUT = 1 << 1, // 10_2
    CONSOLE_OUTPUT = 1 << 2, // 100_2
};

void printUsageAndExit() {
    printf("Usage: ./<executable> <flags> < <input> OR ./<executable> <flags>\nWhere:\n\tExecutable: executable filename (default main);\n\tFlags: if empty all flags are added otherwise a string composed by one or more of the following chars:\n\t\t'r' for generating raw output in output.txt file;\n\t\t't' for generating graph.tex file;\n\t\t'c' for console debug output.\n\tInput: input stream containing the definition of the grammar (usually a filename).\ni.e. ./main tc < inputs/5 OR ./main r < inputs/1\n");
    exit(1);
}

Graph* createGraphFromAutoma(Automa* a, int print, char* header) {
    int* finalStates = malloc(sizeof(int)*a->nodes->used);

    if(print) printf(header);

    for (int i = 0; i < a->nodes->used; i++) {
        State* s = (State*)a->nodes->data[i];
        finalStates[i] = isFinal(s);
        if(print) printState(s, i);
    }

    Graph* g = createGraph(a->nodes->used, finalStates);

    for (int i = a->transitions->used-1; i >= 0; i--) {
        Transition* t = a->transitions->data[i];
        addEdge(g, t->from, t->to, t->symbol);
        if(print) printTransition(t);
    }

    return g;
}

int main(int argc, char *argv[]) {
    srand(time(0));
    char* args = "rtc";
    if(argc > 2) // too many args
        printUsageAndExit();
    else if(argc == 2) //no args
        args = argv[1];

    int flags = 0;
    for (int i = 0; i < ARGS_LEN; i++) {
        if(args[i] == 0) break;
        if(args[i] == 'r') flags = flags | RAW_OUTPUT;
        if(args[i] == 't') flags = flags | TEX_OUTPUT;
        if(args[i] == 'c') flags = flags | CONSOLE_OUTPUT;
    }

    if(flags == 0) printUsageAndExit(); //wrong flags
    
    Grammar* g = parseInput(stdin);

    Automa* lr1A = generateLR1automa(g);
    Graph* lr1G = createGraphFromAutoma(lr1A, flags & CONSOLE_OUTPUT, "\nLR(1) automa:\n\n");

    Automa* lr1mA = generateLR1Mautoma(lr1A, lr1G);
    Graph* lr1mG = createGraphFromAutoma(lr1mA, flags & CONSOLE_OUTPUT, "\nLR(1)m automa:\n\n");


    if(flags & CONSOLE_OUTPUT) {
        printf("\nLR(1) graph:\n\n");
        printGraph(lr1G);
        printf("\nLR(1)m graph:\n\n");
        printGraph(lr1mG);
    }

    if(flags & TEX_OUTPUT) ouputLatexAutoma(lr1mG);
    if(flags & RAW_OUTPUT) ouputRawAutoma(lr1mG, lr1mA);

    return 0;
}