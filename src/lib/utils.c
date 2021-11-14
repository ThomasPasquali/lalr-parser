#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define VALID_SYMBOLS "+*@#" //TODO more?

char itoc(int i) {
    return '0'+i;
}

int ctoi(char c) {
    return c-'0';
}

int inValidSymbols(char c) {
    int res = 0, i = 0;
    while(VALID_SYMBOLS[i] != 0 && (res = VALID_SYMBOLS[i] != c))i++;
    return !res;
}

int validSymbol(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || inValidSymbols(c);
}

/**
 * SEMPLIFICATO
 */
int isTerminal(char c) {
    return (c >= 'a' && c <= 'z') || c < 0; //TODO check epsilon c < 0 for epsilon
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
        if(c != ' ')
            newS[j++] = c;
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
