#ifndef LFC_AUTOMA_H
#define LFC_AUTOMA_H

#include "../lib/list.h"
#include "../lib/set.h"
#include "transition.h"

typedef struct {
    List* nodes;                    //<State>
    List* transitions;              //<Transition*>
    SimpleSet* transitions_keys;    //<Transition*>
                                    /**
                                     * transitions_keys contain only τ(from,symbol)
                                     */
} Automa;

void initAutoma(Automa*,int);

#endif //LFC_AUTOMA_H
