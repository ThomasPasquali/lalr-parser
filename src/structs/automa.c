#include <stdlib.h>
#include "automa.h"
#include "../lib/list.h"

void initAutoma(Automa* a,int initStatesLen) {
    a->nodes = malloc(sizeof *(a->nodes));
    initList(a->nodes, initStatesLen);
}