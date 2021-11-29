#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../structs/grammar.h"
#include "../structs/production.h"
#include "../lib/set.h"

char itoc(int i) {
    return '0'+i;
}

int ctoi(char c) {
    return c-'0';
}

char* ctos(char c) {
    char* tmp = malloc(sizeof(char)*2);
    tmp[0] = c; tmp[1] = 0;
    return tmp;
}

int inValidSymbols(char c) {
    int res = 0, i = 0;
    while(VALID_SYMBOLS[i] != 0 && (res = VALID_SYMBOLS[i] != c))i++;
    return !res;
}

int validSymbol(char c) {
    return isTerminal(c) || isNonTerminal(c);
}

/**
 * SEMPLIFICATO
 */
int isTerminal(char c) {
    return (c >= 'a' && c <= 'z') || isEpsilon(c) || inValidSymbols(c);
}

int isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

int isEpsilon(char c) {
    return c == EPSILON;
}

int length(char* s, char excludedChar) {
    char c; int i = 0, len = 0;
    while((c = s[i++]) != 0)
        if(c != excludedChar)
            len++;
    return len;    
}

char* remove_spaces(char* s) {
    char* newS = malloc(sizeof(char)*length(s, ' '));
    char c; int i = 0, j = 0;
    while((c = s[i++]) != 0) 
        if(c != ' ') {
            if(c != '>' && c != '-' && c != '|' && !validSymbol(c)) {
                fprintf(stderr, "Non valid symbol %d, exiting...\n", c);
                exit(1);
            }
            newS[j++] = c;
        }
    return newS;
}

char** str_split(char* str, const char delimiter, int* resSize) {
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = delimiter;
    delim[1] = 0;

    while(*tmp) {
        if(delimiter == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count += last_comma < (str + strlen(str) - 1);
    count++;

    result = malloc(sizeof(char*) * count);
    *resSize = count-1;

    if(result) {
        size_t idx  = 0;
        char* token = strtok(str, delim);

        while(token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

SimpleSet* first(Grammar* g, char* s) {
    SimpleSet* set = malloc(sizeof *set);
    SimpleSet* newSet;
    set_init(set);
    char* e = ctos(EPSILON);

    //printf("Computing first(%s)\n", s);
    char c; int i = 0, n = strlen(s);
    for (; i < n; i++) {
        c = s[i];
        if(isEpsilon(c))
            continue;
        else if(isTerminal(c)) {
            set_add(set, ctos(c));
            break;
        }else {
            SimpleSet* tmp = firstNT(g, c);
            
            int containedEpsilon;
            if((containedEpsilon = set_contains(tmp, e) == SET_TRUE)) set_remove(tmp, e);

            newSet = malloc(sizeof *newSet); set_init(newSet);
            set_union(newSet, set, tmp); free(set); //NOT freed tmp because is stored in a production
            set = newSet;
            
            if(!containedEpsilon) break;
            else set_add(tmp, e); //tmp is stored in a production
        }
    }
    if(i == n) set_add(set, e);
    
    return set;
}

SimpleSet* firstNT(Grammar* g, char nt) {
    Production* p = 0;
    for (int i = 0; i < g->used; i++) //looking for production s.t. nt == driver
        if(((Production*)g->data[i])->driver == nt) {
            p = g->data[i];
            break;
        }
        
    if(p == 0) {
        fprintf(stderr, "A non terminal (%c) has no production, exiting...\n", nt);
        exit(1);
    }

    if(p->first != 0) //if already computed return
        return p->first;  //TODO check if need to copy

    //compute and store value then return
    SimpleSet* set = malloc(sizeof *set);
    SimpleSet* newSet;
    set_init(set);

    for (int i = 0; i < g->used; i++) { //first(A) = U A->alpha first(alpha)
        Production* p1 = g->data[i]; //A->alpha
        if(p1->body[0] == p->driver) continue; //Avoid loop computing first(A)=first(A)...
        if(nt == p1->driver) {
            SimpleSet* tmp = first(g, p1->body); //first(alpha)

            newSet = malloc(sizeof *newSet); set_init(newSet);
            set_union(newSet, set, tmp);
            free(tmp); free(set);

            set = newSet;
        }
    }
    
    //printf("DEBUG: first(%c)={%s}\n", nt, mergeSetIntoString(set));
    for (int i = 0; i < g->used; i++)
        if(((Production*)g->data[i])->driver == nt)
            ((Production*)g->data[i])->first = set; //Store first in productions
    return set;
}

char* mergeSetIntoString(SimpleSet* set) {
    uint64_t setLen = set_length(set), sLen = 0, sI = 0;
    char** a = set_to_array(set, &setLen);

    for (uint64_t i = 0; i < setLen; i++) sLen += strlen(a[i]);
    
    char* s = malloc(sizeof(char)*sLen+1);
    for (uint64_t i = 0; i < setLen; i++)
        for (size_t j = 0; j < strlen(a[i]); j++)
            s[sI++] = a[i][j];
    s[sI] = 0;
    
    for (uint64_t i = 0; i < setLen; i++) free(a[i]);
    free(a);

    return s;
}

int getRandom(int lower, int upper) {
    return (rand() % (upper - lower + 1)) + lower;
}
  