#include <stdio.h>
#include <stdlib.h>

#include "output.h"

#include "../structs/graph.h"
#include "../structs/automa.h"
#include "../structs/state.h"
#include "../structs/LR1item.h"
#include "utils.h"

#define DOWN 0
#define RIGHT 1
#define LEFT 2
#define DOWN_RIGHT 3
#define DOWN_LEFT 4

const char* directionStrings[] = {
    "[below of =",
    "[right of =",
    "[left of =",
    "[below right of =",
    "[below left of ="
};

void drawNode(FILE* f, int n, int isFinal, int direction, int parent, char* furtherAttrs) {
    fprintf(f, "\\node[state%s] (%d) ", isFinal?", double":"", n);
    if(direction >= 0 && parent >= 0)
        fprintf(f, "%s %d%s]", directionStrings[direction], parent, furtherAttrs);
    fprintf(f, "{%d};\n", n);
}

void drawChilds(FILE* f, Graph* g, int* alreadyDrawn, Node* n, int parent, int direction, int prevDirection) {
    //if(!n) printf("Branch DONE!\n");

    if(n && !alreadyDrawn[n->dest]) {
        alreadyDrawn[n->dest] = TRUE;

        char* arg = "";
        if(direction == DOWN) {
            arg = malloc(25*sizeof(char));
            arg[sprintf(arg, ", node distance = %.1f cm", getRandom(10, 55)/(float)10)] = 0;
        }
        
        drawNode(f, n->dest, g->finals[n->dest], direction, parent<0?-parent:parent, arg); //Parent < 0 means new branch
        
        //Parent < 0 means start expanding to the right & left expansion are made twice
        int nextDirection = DOWN;
        switch (direction) {
            case RIGHT: nextDirection = prevDirection!=RIGHT ? DOWN : DOWN; break;
            case DOWN: nextDirection = LEFT; break;
            case LEFT: nextDirection = prevDirection!=LEFT ? DOWN_RIGHT : DOWN_RIGHT; break;
            case DOWN_RIGHT: nextDirection = DOWN_LEFT; break;
            case DOWN_LEFT: nextDirection = RIGHT; break;
            //TODO more
        }
        //printf("Drawing %d parent %d dir(%s)\n", n->dest, parent, directionStrings[direction]);
        drawChilds(f, g, alreadyDrawn, n->next, parent<0?n->dest:parent, nextDirection, direction); 
    }
}

void appendToFile(FILE* src, FILE* dst) {
    char byte;
    while(!feof(src)) {
        fread(&byte, sizeof(char), 1, src);
        fwrite(&byte, sizeof(char), 1, dst);
    }
}

void ouputLatexAutoma(Graph* g) {
    FILE *src = fopen(PRE_FILENAME, "rb");
    FILE *finalFile = fopen(OUT_TEX_FILENAME, "wb+");

    appendToFile(src, finalFile);
    fclose(src);

    int alreadyDrawn[g->nodesCount];
    for(int i = 0; i < g->nodesCount; i++) alreadyDrawn[i] = FALSE;

    //Positioning and drawing nodes SUPPOSING connected graph
    drawNode(finalFile, 0, g->finals[0], -1, -1, ", green"); alreadyDrawn[0] = TRUE;
    for (int i = 0; i < g->nodesCount; i++) {
        drawChilds(finalFile, g, alreadyDrawn, g->nodes[i].head, -i, i==0?RIGHT:DOWN, i==0?DOWN_LEFT:RIGHT);
    }

    //Drawing edges
    for(int i = 0; i < g->nodesCount; i++) { 
        Node* n = g->nodes[i].head;
        while(n) {
            fprintf(finalFile, 
                "\\draw%s (%d) to node {%s} (%d);\n", 
                i == n->dest?"[loop above]":"", i, n->symbol=='#'?"\\#":ctos(n->symbol), n->dest);
            n = n->next;
        }
    }

    src = fopen(POST_FILENAME, "rb");
    appendToFile(src, finalFile);
    fclose(src);
    
    fclose(finalFile);
}

void ouputRawAutoma(Graph* g, Automa* a) {
    FILE *f = fopen(OUT_RAW_FILENAME, "wb+");

    fprintf(f, "%d\n", g->nodesCount);

    for(int i = 0; i < g->nodesCount; i++) { 
        Node* n = g->nodes[i].head;
        fprintf(f, "%c", g->finals[i]?FINAL_MARKER:NON_FINAL_MARKER);
        while(n) {
            fprintf(f, " %c%d\n", n->symbol, n->dest);
            n = n->next;
        }
    }

    for (int i = 0; i < a->nodes->used; i++) {
        State* s = (State*)a->nodes->data[i];
        if(g->finals[i]) {
            fprintf(f, "%d ", i);
            int itemsCount = s->items->used;
            for (int j = 0; j < itemsCount; j++) {
                LR1item* item = s->items->data[j];
                printf("MAOOA %p\n", item);
                if(j == itemsCount-1)
                    fprintf(f, "%c%s%s %s", item->p->driver, R_ITEM_ARROW, item->p->body, mergeSetIntoString(item->ls));
                else
                    fprintf(f, "%c%s%s %s%c", item->p->driver, R_ITEM_ARROW, item->p->body, mergeSetIntoString(item->ls), R_ITEM_SEP);
            }     
        }
        fprintf(f, "\n");
    }

    //TODO accepting item

    fclose(f);
}
