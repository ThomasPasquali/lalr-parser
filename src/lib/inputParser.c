#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inputParser.h"
#include "../structs/production.h"
#include "../structs/grammar.h"
#include "utils.h"
#include "list.h"
#include "set.h"
#include "../structs/LR1item.h"

int count(char c, char* s) {
    int count = 0, i = 0;
    while(s[i] != 0)
        if(s[i++] == c)
            count++;
    return count;
}

void appendProductions(char* production, List* items) {
    char driver = production[0];
    
    int bodiesLen;
    char** bodies = str_split(production+3, '|', &bodiesLen);//TODO sintax check
    
    Production* p;
    for (int i = 0; i < bodiesLen; i++) {
        p = malloc(sizeof *p);
        p->driver = driver;

        for (size_t j = 0; j < strlen(bodies[i]); j++) //Removing ghost chars....idk
            if((int)bodies[i][j] > 0 && (int)bodies[i][j] < 32)
                bodies[i][j] = 0;
        
        p->body = bodies[i];
        p->first = 0;
        insertList(items, p);
    }
}

Grammar* parseInput(FILE* fp) {
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    if (fp == NULL) {
        fprintf(stderr, "Cannot read input file!\n");
        exit(EXIT_FAILURE);
    }

    Grammar* g = malloc(sizeof *g);
    initList(g, 5);

    while ((read = getline(&line, &len, fp)) != EOF) {
        if(line[read-1] == '\n') line[read-1] = 0; //Remove \n
        char* noSpacesProd = remove_spaces(line);

        if(g->used == 0) { //Aggiunta produzione canonica S' -> S (~ -> S)
            char* tmp = malloc(sizeof(char)*5);
            sprintf(tmp, "%c->%c", S1, noSpacesProd[0]);
            appendProductions(tmp, g);
            free(tmp);
        }

        appendProductions(noSpacesProd, g);
        free(noSpacesProd);
    }

    fclose(fp);
    if(line) free(line);

    return g;
}