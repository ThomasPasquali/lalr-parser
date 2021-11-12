#ifndef LFC_UTILS_H
#define LFC_UTILS_H

int length(char* s, char excludedChar);
char** str_split(char* str, const char delimiter, int* resSize);
char* remove_spaces(char* s);

#endif //LFC_UTILS_H
