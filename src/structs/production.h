#ifndef LFC_PRODUCTION_H
#define LFC_PRODUCTION_H

#include "../lib/list.h"

typedef struct {
    char driver;
    char* body;
} Production;

void appendProductions(char* production, List* productions);

#endif //LFC_PRODUCTION_H
