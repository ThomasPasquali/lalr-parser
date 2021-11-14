#include <stdlib.h>
#include "automa.h"
#include "../lib/list.h"
#include "../lib/set.h"

void initAutoma(Automa* a,int initStatesLen) {
    a->nodes = malloc(sizeof *(a->nodes));
    initList(a->nodes, initStatesLen);

    a->transitions = malloc(sizeof *(a->transitions));
    initList(a->transitions, initStatesLen);

    a->transitions_keys = malloc(sizeof(SimpleSet));
    set_init(a->transitions_keys);
}