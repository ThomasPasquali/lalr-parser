#ifndef LFC_AUTOMA_H
#define LFC_AUTOMA_H

#include "../lib/list.h"

typedef struct {
    List* nodes; //<State>
} Automa;

void initAutoma(Automa*,int);

#endif //LFC_AUTOMA_H
