#ifndef LFC_UTILS_H
#define LFC_UTILS_H

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define E       "\x1b[0m"

#define TRUE 1
#define FALSE 0

int isTerminal(char c);
int length(char* s, char excludedChar);
char** str_split(char* str, const char delimiter, int* resSize);
char* remove_spaces(char* s);
int validSymbol(char c);
char itoc(int i);
int ctoi(char c);

#endif //LFC_UTILS_H
