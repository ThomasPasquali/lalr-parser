#include <stdio.h>
#include <stdlib.h>

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
    printf("Usage: ./<executable> <flags> < <input>\nWhere:\n\tExecutable: executable filename (default main)\n\tFlags: if empty all flags are added otherwise a string composed by one or more of the following chars:\n\t\t'r' for generating raw output in output.txt file\n\t\t't' for generating graph.tex file\n\t\t'c' for console debug output\n\tInput: input stream containing the definition of the grammar (usually a filename)\ni.e. ./main tc < inputs/5 OR ./main r < inputs/1\n");
    exit(1);
}

int main(int argc, char *argv[]) {
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
    Automa* automa = generateLALRautoma(g);

    //TODO LRm

    int* finalStates = malloc(sizeof(int)*automa->nodes->used);

    if(flags & CONSOLE_OUTPUT) printf("AUTOMA:\n\n");

    for (int i = 0; i < automa->nodes->used; i++) {
        State* s = (State*)automa->nodes->data[i];
        finalStates[i] = isFinal(s);
        if(flags & CONSOLE_OUTPUT) printState(s, i);
    }

    Graph* graph = createGraph(automa->nodes->used, finalStates);

    for (int i = automa->transitions->used-1; i >= 0; i--) {
        Transition* t = automa->transitions->data[i];
        addEdge(graph, t->from, t->to, t->symbol);
        if(flags & CONSOLE_OUTPUT) printTransition(t);
    }

    if(flags & CONSOLE_OUTPUT) {
        printf("GRAPH:\n\n");
        printGraph(graph);
    }
    if(flags & TEX_OUTPUT) ouputLatexAutoma(graph);
    if(flags & RAW_OUTPUT) ouputRawAutoma(graph, automa);

    return 0;
}