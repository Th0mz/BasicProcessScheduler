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

    int source, target;

    int vertices;
    LinkedList *outGoingEdges;

    int **weights;
} Graph;

    /* Functions */

void initGraph(Graph *graph, int vertices) {
    /* Initialization of the graph properties */
    int i, j;

    graph->vertices = vertices;

    graph->source = vertices;
    graph->target = vertices + 1;

    graph->outGoingEdges = (LinkedList *) malloc(sizeof(LinkedList) * vertices);
    graph->weights = (int **) malloc(sizeof(int *) * (vertices + 2));

    for (i = 0; i < vertices + 2; i++) {
        graph->weights[i] = (int *) malloc(sizeof(int) * (vertices + 2));
    }

    for (i = 0; i < vertices + 2; i++) {
        for (j = 0; j < vertices + 2; j++) {
            graph->weights[i][j] = 0;
        }
    }

    for (i = 0; i < vertices + 2; i++) {
        initList(&(graph->outGoingEdges[i]));
    }
}

void destroyGraph(Graph *graph) {
    /* Frees the memory associated with the graph */
    int i;
    
    for (i = 0; i < graph->vertices + 2; i++){
        destroyList(&(graph->outGoingEdges[i]));
    }

    for (i = 0; i < graph->vertices + 2; i++) {
        free(graph->weights[i]);
    } 

    free(graph->weights);
    free(graph->outGoingEdges);
    free(graph);
}

void addEdge(Graph *graph, int u, int v, int weight) {
    push(&(graph->outGoingEdges[u]), v);
    push(&(graph->outGoingEdges[v]), u);

    graph->weights[u][v] = weight;
    graph->weights[v][u] = weight;

}

void processInput(Graph *graph) {

    int vertices, edges, i;
    scanf("%d %d", &vertices, &edges);

    initGraph(graph, vertices);
    for (i = 0; i < vertices; i++) {
        int x, y;
        scanf("%d %d", &x, &y);

        addEdge(graph, graph->source, i, x);
        addEdge(graph, graph->target, i, y);
    }

    for (i = 0; i < edges; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);

        addEdge(graph, u - 1, v - 1, w);
    }

}

int main() {

    int i, j;
    Graph graph;

    processInput(&graph);

    printf("%d\n", graph.outGoingEdges[0].head);

    printf("Pesos : \n");
    for (i = 0; i < graph.vertices + 2; i++) {
        for (j = 0; j < graph.vertices + 2; j++) {
            printf("%d ", graph.weights[i][j]);
        }
        printf("\n");
    }

    destroyGraph(&graph);
    return 0;
}