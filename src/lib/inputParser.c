#include <stdio.h>
#include <stdlib.h>
#include "inputParser.h"
#include "../structs/production.h"
#include "utils.h"
#include "list.h"
#include "set.h"
#include "../structs/LR1item.h"
#include <wchar.h>

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
        p->body = bodies[i];
        insertList(items, p);
    }
}

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

        if(productions->used == 0) { //Aggiunta produzione canonica S' -> S (~ -> S)
            char* tmp = malloc(sizeof(char)*5);
            sprintf(tmp, "~->%c", noSpacesProd[0]);
            appendProductions(tmp, productions);
            free(tmp);
        }

        appendProductions(noSpacesProd, productions);
        free(noSpacesProd);
    }

    fclose(fp);
    if(line) free(line);

    return productions;
}