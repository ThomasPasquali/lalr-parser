#ifndef LFC_GRAPH_H
#define LFC_GRAPH_H

typedef struct node { 
    int dest;
    char symbol;
    int isFinal;
    struct node* next; 
} Node; 

typedef struct { 
    Node *head;
    int x, y; //For graph drawing purposes
} LinkedList; 

/**
 * Graph rappresented with adjacency lists (LinkedLists)
 */
typedef struct { 
    int nodesCount;
    int* finals;
    LinkedList* nodes; 
} Graph; 


/**
 * Notice: the link is added at the beginning of the LinkedList
 */
Node* createNode(int n, char symbol, Node* next);
Graph* createGraph(int n, int* finals);
void addEdge(Graph* g, int src, int dest, char symbol);
void printGraph(Graph* g);
int linkedListSize(Node* head);

#endif //LFC_GRAPH_H