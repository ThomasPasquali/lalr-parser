#include <stdio.h>
#include <string.h>
#include "LR1item.h"
#include "production.h"

void print(LR1item* item) {
    char s[strlen(item->p->body)+2];
    char c; int i = 0, j = 0, markerAdded = 0;
    while ((c = item->p->body[i]) != 0)
        if(!markerAdded && (markerAdded = i == item->marker))
            s[j++] = '.';
        else {
            s[j++] = c;
            i++;
        }
    if(markerAdded)
        s[j] = 0;
    else {
        s[j++] = '.';
        s[j] = 0;
    }
    printf("LR(1) item: %c -> %s\n", item->p->driver, s);
}