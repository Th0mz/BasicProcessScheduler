#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define WHITE 0
#define GRAY 1
#define BLACK 2

#define INFINITY 2147483646

/* ------------ GRAPH ------------ */
    /* Structure */

typedef struct graph {
    /* Graph representation with adjency list */

    int vertices;
    int source, target;

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

    /* Allocate memory for residual network */
    graph->graphF = (int **) malloc(sizeof(int *) * vertices);

    for (i = 0; i < vertices; i++) {
        graph->graphF[i] = (int *) malloc(sizeof(int) * vertices);
    }

    /* Initialize the residual cost to 0 */
    for (i = 0; i < vertices; i++) {
        for (j = 0; j < vertices; j++) {
            graph->graphF[i][j] = 0;
        }
    }
}

void addEdge(Graph *graph, int u, int v, int weight) {
    /* Adds a non-directed edge (u, v) with the given weight */
    graph->graphF[u][v] = weight;
    graph->graphF[v][u] = weight;
}

void destroyGraph(Graph *graph) {
    /* Frees the memory associated with the graph */
    int i;

    for (i = 0; i < graph->vertices; i++) {
        int *toFree = graph->graphF[i];
        free(toFree);
    }

    free(graph->graphF);
}

int getShortestPath(Graph *graph, int *path, int *pathLength, char *color, int *parent) {
    /* Calculate the shortest path in number of edges between
       the source and the target in the residual network, 
       using BFS */

    int i, head;
    *pathLength = 0;

    /* Initialize stack */
    int *stack = (int *) malloc(sizeof(int) * graph->vertices);
    int stackHead = 0;

    /* Initialize each vertice proprities (color and parent) */
    for (i = 0; i < graph->vertices; i++) {
        color[i] = WHITE;
        parent[i] = -1;
    }

    color[graph->source] = GRAY;
    stack[stackHead] = graph->source;

    while (stackHead >= 0) {
        /* Remove the first element from the stack */
        head = stack[stackHead]; stackHead--;

        /* Add all of its adjencies to the stack and update 
           their proprieties if they werent visited yet*/
        for(i = 0; i < graph->vertices; i++) {
            if((color[i] == WHITE) && (graph->graphF[head][i] > 0)){
                if (i == graph->target) {
                    /* The target was reached, return the shortestPath
                       calculated between the source and target */
                    
                    int minimumIncrease = INFINITY;
                    parent[i] = head;

                    /* Copy the calculated path to an array */
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

                /* Add the adjency to the stack */
                stackHead++; stack[stackHead] = i;
                parent[i] = head;
                color[i] = GRAY;
            }
        }
    }

    free(stack);
    return 0;   
}

int computeMinimumCost(Graph *graph) {
    /* Calculates the minimum cost of the graph aka minimum cut */
    int i;
    int minimumIncrease, minimumCost = 0;

    /* Allocate memory */
    int *path = (int *) malloc(sizeof(int) * graph->vertices);
    int pathLength;

    char *color = (char*) malloc(sizeof(char) * graph->vertices);
    int *parent = (int*) malloc(sizeof(int) * graph->vertices);

    while (TRUE) {
        /* Calculate shotest path and the minimum 
        capacity of that path = minimumIncrease */
        minimumIncrease = getShortestPath(graph, path, &pathLength, color, parent);

        if (pathLength == 0) {
            /* If there is no path between the source and target
            => minimumCost is the minimum cut */
            
            /* Free memory */
            free(color);
            free(parent);
            free(path);

            return minimumCost;
        }

        /* Update the edges capacities after increasing the flow */
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

    /* Sets the source and target edges */
    for (i = 0; i < vertices; i++) {
        int x, y;
        scanf("%d %d", &x, &y);

        addEdge(graph, graph->source, i, x);
        addEdge(graph, graph->target, i, y);
    }

    /* Sets the edges between processes */
    for (i = 0; i < edges; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);

        addEdge(graph, u - 1, v - 1, w);
    }

}

int main() {

    Graph graph;
    processInput(&graph);

    int minimumCost = computeMinimumCost(&graph);
    printf("%d\n", minimumCost);
    
    destroyGraph(&graph);
    return 0;
}