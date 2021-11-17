#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "../lib/utils.h"

Node* createNode(int dest, char symbol, Node* next) { 
    Node* node = malloc(sizeof *node); 
    node->dest = dest; 
    node->next = next; 
    node->symbol = symbol;
    return node; 
} 

Graph* createGraph(int n, int* finals) { 
    Graph* g = malloc(sizeof *g);
    g->nodes = malloc(n * sizeof(LinkedList)); 
    g->nodesCount = n;
    g->finals = finals;

    for (int i = 0; i < n; i++) g->nodes[i].head = 0; 

    return g; 
} 

void addEdge(Graph* g, int src, int dest, char symbol) {
    Node* newNode = createNode(dest, symbol, g->nodes[src].head);
    g->nodes[src].head = newNode;
} 

void printGraph(Graph* g) {
    for(int i = 0; i < g->nodesCount; i++) { 
        Node* n = g->nodes[i].head; 
        printf(CYAN"Node %-3d%-6s: head "E, i, g->finals[i]?"(F)":"");
        while(n) { 
            printf(CYAN"-%c> %d "E, n->symbol, n->dest); 
            n = n->next; 
        } 
        printf("\n"); 
    } 
}

int linkedListSize(Node* head) {
    int count = 0;
    while(head && (head = head->next)) count++;
    return count;
}