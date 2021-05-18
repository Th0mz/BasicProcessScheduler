#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define WHITE 0
#define GRAY 1
#define BLACK 2

#define INFINITY 2147483646

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

void print(LinkedList *list) {
    node *node = list->head;

    while(node != NULL) {
        printf("%d ", node->value);
        node = node->next;
    }

    printf("\n");
}

/* ------------ GRAPH ------------ */
    /* Structure */

typedef struct graph {
    /* Graph representation with adjency list */

    int vertices;
    int source, target;

    LinkedList *outGoingEdges;
    
    int **weights;
    int **graphF;

} Graph;

    /* Functions */

void initGraph(Graph *graph, int vertices) {
    /* Initialization of the graph properties */
    int i, j;

    vertices += 2;
    graph->vertices = vertices;

    graph->source = vertices - 2;
    graph->target = vertices - 1;


    graph->outGoingEdges = (LinkedList *) malloc(sizeof(LinkedList) * vertices);
    graph->weights = (int **) malloc(sizeof(int *) * vertices);
    graph->graphF = (int **) malloc(sizeof(int *) * vertices);

    for (i = 0; i < vertices; i++) {
        graph->weights[i] = (int *) malloc(sizeof(int) * vertices);
    }

    for (i = 0; i < vertices; i++) {
        graph->graphF[i] = (int *) malloc(sizeof(int) * vertices);
    }

    for (i = 0; i < vertices; i++) {
        for (j = 0; j < vertices; j++) {
            graph->weights[i][j] = 0;
        }
    }

    for (i = 0; i < vertices; i++) {
        initList(&(graph->outGoingEdges[i]));
    }
}

void addEdge(Graph *graph, int u, int v, int weight) {
    push(&(graph->outGoingEdges[u]), v);
    push(&(graph->outGoingEdges[v]), u);

    graph->weights[u][v] = weight;
    graph->weights[v][u] = weight;
}

LinkedList getAdjacencies(Graph *graph, int vertice) {
    return graph->outGoingEdges[vertice];
}

void destroyGraph(Graph *graph) {
    /* Frees the memory associated with the graph */
    int i;
    
    for (i = 0; i < graph->vertices; i++){
        destroyList(&(graph->outGoingEdges[i]));
    }

    for (i = 0; i < graph->vertices; i++) {
        int *toFree = graph->weights[i];
        free(toFree);
    } 

    for (i = 0; i < graph->vertices; i++) {
        int *toFree = graph->graphF[i];
        free(toFree);
    }

    free(graph->weights);
    free(graph->graphF);
    free(graph->outGoingEdges);
}

int getShortestPath(Graph *graph, int *path, int *pathLength, char *color, int *parent) {
    
    int i, head;
    *pathLength = 0;

    int *stack = (int *) malloc(sizeof(int) * graph->vertices);
    int pointer = 0;

    for (i = 0; i < graph->vertices; i++) {
        color[i] = WHITE;
        parent[i] = -1;
    }

    color[graph->source] = GRAY;
    stack[pointer] = graph->source;

    LinkedList adjacencies;
    node* adjacency;
    while (pointer >= 0) {
        head = stack[pointer];
        pointer--;

        adjacencies = getAdjacencies(graph, head);
        for(adjacency = adjacencies.head; adjacency != NULL; adjacency = adjacency->next) {
            i = adjacency->value;
            if((color[i] == WHITE) && (graph->graphF[head][i] > 0)){
                if (i == graph->target) {
                    parent[i] = head;

                    int minimumIncrease = INFINITY;
                    while (parent[i] != -1) {
                        path[*pathLength] = i;
                        (*pathLength)++;

                        if (graph->graphF[parent[i]][i] < minimumIncrease) {
                            minimumIncrease = graph->graphF[parent[i]][i]; 
                        }

                        i = parent[i];
                    }

                    path[*pathLength] = i;
                    (*pathLength)++;

                    free(stack);
                    return minimumIncrease;
                }

                pointer++;
                stack[pointer] = i;
                parent[i] = head;
                color[i] = GRAY;
            }
        }
    }
    return 0;   
}

int computeMinimumCost(Graph *graph) {

    int i, j;
    int minimumIncrease, minimumCost = 0;
    /* Init graphF */ 

    for (i = 0; i < graph->vertices; i++) {
        for (j = 0; j < graph->vertices; j++) {
            graph->graphF[i][j] = graph->weights[i][j];
        }
    }

    int *path = (int *) malloc(sizeof(int) * graph->vertices);
    int pathLength;

    char *color = (char*) malloc(sizeof(char) * graph->vertices);
    int *parent = (int*) malloc(sizeof(int) * graph->vertices);

    while (TRUE) {

        minimumIncrease = getShortestPath(graph, path, &pathLength, color, parent);

        if (pathLength == 0) {
            /* Free memory */
            free(color);
            free(parent);
            free(path);


            return minimumCost;
        }

        /*

        for (i = 0; i < pathLength; i++) {
            printf("%d ", path[i]);
        }
        printf(" : %d\n", minimumIncrease);


        printf("\nPesos : \n");
        for (i = 0; i < graph->vertices; i++) {
            for (j = 0; j < graph->vertices; j++) {
                printf("%d ", graph->graphF[i][j]);
            }
            printf("\n");
        }
        */

        for (i = 0; i < pathLength - 1; i++) {
            int u = path[i], v = path[i + 1];

            graph->graphF[v][u] -= minimumIncrease;
            graph->graphF[u][v] += minimumIncrease;
        }

        minimumCost += minimumIncrease;
    }
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

    Graph graph;
    processInput(&graph);

    /*
    printf("Arcos : \n");
    for( i = 0; i < graph.vertices; i++) {
        print(&graph.outGoingEdges[i]);
    }

    printf("\nPesos : \n");
    for (i = 0; i < graph.vertices; i++) {
        for (j = 0; j < graph.vertices; j++) {
            printf("%d ", graph.weights[i][j]);
        }
        printf("\n");
    }
    */

    int minimumCost = computeMinimumCost(&graph);
    printf("%d\n", minimumCost);
    
    destroyGraph(&graph);
    return 0;
}