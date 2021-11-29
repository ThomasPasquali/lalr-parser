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
#define UP 5

const char* directionStrings[] = {
    "[below of =",
    "[right of =",
    "[left of =",
    "[below right of =",
    "[below left of =",
    "[above of ="
};

#define GRID_SIZE 7
int occupied[GRID_SIZE][GRID_SIZE];

void drawNode(FILE* f, int n, int isFinal, int direction, int parent, char* furtherAttrs, char* nodeAttrs) {
    fprintf(f, "\\node[state%s%s] (%d) ", isFinal?", accepting":"", nodeAttrs, n);
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
        
        drawNode(f, n->dest, g->finals[n->dest], direction, parent<0?-parent:parent, arg, ""); //Parent < 0 means new branch
        
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

void ouputLatexAutoma(Graph* g, Automa* a) {
    FILE *src = fopen(PRE_FILENAME, "rb");
    FILE *finalFile = fopen(OUT_TEX_FILENAME, "wb+");

    appendToFile(src, finalFile);
    fclose(src);

    int alreadyDrawn[g->nodesCount];
    for(int i = 0; i < g->nodesCount; i++) alreadyDrawn[i] = FALSE;

    for (int i = 0; i < GRID_SIZE; i++)
        for (int j = 0; j < GRID_SIZE; j++)
            occupied[i][j] = FALSE;

    /*drawNode(finalFile, 0, g->finals[0], -1, -1, "", ", red"); alreadyDrawn[0] = TRUE;
    drawNode(finalFile, 1, g->finals[1], UP, 0, "", ", green"); alreadyDrawn[1] = TRUE;
    occupied[GRID_SIZE/2][0] = TRUE; g->nodes[0].x = GRID_SIZE/2; g->nodes[0].y = 0;
    for (int i = 0; i < g->nodesCount; i++) {
        LinkedList l = g->nodes[i];
        drawChilds(finalFile, g, alreadyDrawn, l.head, -i, DOWN, RIGHT, l.x, l.y);
    }*/

    //Positioning and drawing nodes SUPPOSING connected graph
    drawNode(finalFile, 0, g->finals[0], -1, -1, "", ", red"); alreadyDrawn[0] = TRUE;
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

    //Reducing items
    fprintf(finalFile, "\\end{tikzpicture}\n\n\\textbf{Reducing items:}\n\\begin{itemize}\n");
    for (int i = 0; i < a->nodes->used; i++) {
        State* s = (State*)a->nodes->data[i];
        if(g->finals[i]) {
            fprintf(finalFile, "\\item[%d:] ", i);
            int itemsCount = s->items->used;
            for (int j = 0; j < itemsCount; j++) {
                LR1item* item = s->items->data[j];
                if(markerAtTheEnd(item)) {
                    char c; int k = 0;
                    fprintf(finalFile, "%c %s ", item->p->driver, T_ITEM_ARROW);
                    while((c = item->p->body[k++])) if(c == '#') fprintf(finalFile, "\\#"); else fprintf(finalFile, "%c ", c);

                    //Lookahead set
                    k = 0;
                    fprintf(finalFile, "\\{ ");
                    char* ls = mergeSetIntoString(item->ls); 
                    while((c = ls[k++])) {
                        if(c == '$') 
                            fprintf(finalFile, "\\$ ");
                        else
                            fprintf(finalFile, "%c ", c);
                        if(ls[k] != 0) fprintf(finalFile, ",");
                    }
                    free(ls);
                    fprintf(finalFile, "\\} ");
                }
            }
            fprintf(finalFile, "\n");
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
        fprintf(f, "%d %c", i, g->finals[i]?FINAL_MARKER:NON_FINAL_MARKER);
        while(n) {
            fprintf(f, " %c%c%d", n->symbol, EDGE_SYMBOL_STATE_SEP, n->dest);
            n = n->next;
        }
        fprintf(f, "\n");
    }

    for (int i = 0; i < a->nodes->used; i++) {
        State* s = (State*)a->nodes->data[i];
        if(g->finals[i]) {
            fprintf(f, "%d ", i);
            int itemsCount = s->items->used;
            for (int j = 0; j < itemsCount; j++) {
                LR1item* item = s->items->data[j];
                if(markerAtTheEnd(item)) {
                    if(j == itemsCount-1)
                        fprintf(f, "%c%s%s %s", item->p->driver, R_ITEM_ARROW, item->p->body, mergeSetIntoString(item->ls));
                    else
                        fprintf(f, "%c%s%s %s%c", item->p->driver, R_ITEM_ARROW, item->p->body, mergeSetIntoString(item->ls), R_ITEM_SEP);
                }
            }
            fprintf(f, "\n");
        }        
    }

    fprintf(f, "\n");

    int firstStateI = 0;
    State* firstState = a->nodes->data[firstStateI];
    LR1item* firstItem = firstState->items->data[firstStateI];
    char startSymbol = firstItem->p->body[firstItem->marker];
    fprintf(f, "%c %d", startSymbol, firstStateI);

    fclose(f);
}



/*
TENTATIVO MISERAMENTE FALLITO
void drawChilds(FILE* f, Graph* g, int* alreadyDrawn, Node* n, int parent, int direction, int prevDirection, int x, int y) {
    //if(!n) printf("Branch DONE!\n");

    if(n) {
        if(!alreadyDrawn[n->dest]) {
            alreadyDrawn[n->dest] = TRUE;
            occupied[x][y] = TRUE;

            char* arg = "";
            if(direction == DOWN) {
                arg = malloc(25*sizeof(char));
                arg[sprintf(arg, ", node distance = %.1f cm", getRandom(20, 55)/(float)10)] = 0;
            }
            
            drawNode(f, n->dest, g->finals[n->dest], direction, parent<0?-parent:parent, arg, ""); //Parent < 0 means new branch           
        }
        
        //Parent < 0 means start expanding to the right
        int nextDirection = DOWN;
        switch (direction) {
            case RIGHT:
                nextDirection = 
                    prevDirection == LEFT ?
                        occupied[x][y+1] ? DOWN : RIGHT :
                        occupied[x+1][y] ? LEFT : DOWN;
                break;
            case DOWN: 
                nextDirection = 
                    prevDirection == RIGHT ?
                        occupied[x-1][y] ? DOWN : LEFT :
                        occupied[x][y+1] ? RIGHT : DOWN;
            case LEFT: 
                nextDirection = 
                    prevDirection == DOWN ?
                        occupied[x+1][y] ? LEFT : RIGHT :
                        occupied[x-1][y] ? DOWN : LEFT;
                break;
            //TODO more
        }

        //printf("Drawing %d parent %d dir(%s)\n", n->dest, parent, directionStrings[direction]);
        drawChilds(f, g, alreadyDrawn, n->next, parent<0?n->dest:parent, nextDirection, direction, nextDirection==RIGHT?x+1:x, nextDirection==DOWN?y+1:y); 
    }
}*/
