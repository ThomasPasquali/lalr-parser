#ifndef LFC_UTILS_H
#define LFC_UTILS_H

#include "../structs/grammar.h"
#include "set.h"

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define E       "\x1b[0m"

#define TRUE 1
#define FALSE 0

#define VALID_SYMBOLS "+*@=(){}[]" //TODO more?
#define EPSILON '#'

int isNonTerminal(char c);
int isTerminal(char c);
int isEpsilon(char c);
int length(char* s, char excludedChar);
char** str_split(char* str, const char delimiter, int* resSize);
/**
 * This function checks if all symbols are valid
 */
char* remove_spaces(char* s);
int validSymbol(char c);
/**
 * returns: a string 'c0'
 */
char* ctos(char c);
char itoc(int i);
int ctoi(char c);
SimpleSet* first(Grammar* g, char* s);
/**
 * This function looks for first(nt) in g, if not alreay initialized is computed and then returns
 */
SimpleSet* firstNT(Grammar* g, char nt);
char* mergeSetIntoString(SimpleSet*);

#endif //LFC_UTILS_H
