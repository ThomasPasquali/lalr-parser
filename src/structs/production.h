#ifndef LFC_PRODUCTION_H
#define LFC_PRODUCTION_H

#include "../lib/list.h"
#include "../lib/set.h"

typedef struct {
    char driver;
    char* body;
    /**
     * First(NON-TERMINAL(driver)) are computed once for efficency
     */
    SimpleSet* first;
} Production;

#endif //LFC_PRODUCTION_H
