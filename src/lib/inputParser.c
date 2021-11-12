#include <stdio.h>
#include <stdlib.h>
#include "inputParser.h"
#include "../structs/production.h"
#include "utils.h"
#include "list.h"

const int MAX_INPUT_ROW_LENGTH = 100;

List* parseInput(FILE* fp) {
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    if (fp == NULL) {
        fprintf(stderr, "Cannot read input file!\n");
        exit(EXIT_FAILURE);
    }

    List* productions = malloc(sizeof *productions);
    initList(productions, 5);

    while ((read = getline(&line, &len, fp)) != EOF) {
        if(line[read-1] == '\n') line[read-1] = 0; //Remove \n
        char* noSpacesProd = remove_spaces(line);
        appendProductions(noSpacesProd, productions);
        free(noSpacesProd);
    }

    fclose(fp);
    if(line) free(line);

    return productions;
}