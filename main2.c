#include <stdio.h>
#include <stdlib.h>

/* ------------ LINKEDLIST ------------ */
    /* Structures */

typedef struct node
{
    struct node *next;
    int value;
} node;


typedef struct linkedList {

    node *head;
} LinkedList;

    /* Functions */

void initList(LinkedList *list)
{
    /* Initialization of the list as a empty list */
    list->head = NULL;
}

int pop(LinkedList *list) {
    /* Removes the first element in the list */
    node *removedElement;
    int value;

    if (list->head == NULL)
        return -1;

    /* Store locally the fist element so it can be freed */
    removedElement = list->head;
    value = removedElement->value;

    /* Update list head */
    list->head = list->head->next;

    free(removedElement);
    return value;
}

void push(LinkedList *list, int value) {
    /* Adds a new element to the begining of the list */
    node *newNode;

    /* Allocate memory */
    newNode = (node*) malloc(sizeof(node));

    /* Define newNode */
    newNode->value = value;
    newNode->next = list->head;
    
    list->head = newNode;

}

void destroyList(LinkedList *list) {
    /* Frees all the memory associated with the list */
    node *node, *nextNode;

    node = list->head;

    while(node != NULL) {
        nextNode = node->next;
        free(node);
        node = nextNode;
    }
}

int isEmpty(LinkedList list) {
    if (list.head == NULL)
        return 1;
    else
        return 0;
}

/* ------------ GRAPH ------------ */
    /* Structure */

typedef struct graph {
    /* Graph representation with adjency list */

    int vertices;
    LinkedList *outGoingEdges;
} Graph;

    /* Functions */

void initGraph(Graph *graph, int vertices) {
    /* Initialization of the graph properties */
    int i;

    graph->vertices = vertices;

    graph->outGoingEdges = (LinkedList *) malloc(sizeof(LinkedList) * vertices);

    for (i = 0; i < vertices; i++) {
        initList(&(graph->outGoingEdges[i]));
    }
}

void destroyGraph(Graph *graph) {
    /* Frees the memory associated with the graph */
    int i;
    
    for (i = 0; i < graph->vertices; i++){
        destroyList(&(graph->outGoingEdges[i]));
    }    

    free(graph->outGoingEdges);
}

void processInput(Graph *graph) {

    int vertices, edges, i;
    scanf("%d %d", &vertices, &edges);

    initGraph(graph, vertices);

    for (i = 0; i < vertices; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
    }

    for (i = 0; i < edges; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
    }

}

int main() {

    int i, j;
    Graph graph;

    processInput(&graph);

    destroyGraph(&graph);
    return 0;
}