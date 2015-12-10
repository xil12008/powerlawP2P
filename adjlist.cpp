// A C Program to demonstrate adjacency list representation of graphs
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// A structure to represent an adjacency list node
struct AdjListNode
{
    int dest;
    struct AdjListNode* next;
};
 
// A structure to represent an adjacency list
struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph
{
    int V;
    int cap;
    int* degree;
    struct AdjList* array;
};
 
// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest)
{
    struct AdjListNode* newNode =
            (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}
// A utility function that creates a graph of V vertices

struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    graph->cap = 0;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
    graph->degree = (int*) malloc(V * sizeof(int));
 
     // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;
        graph->degree[i] = 0;
 
    return graph;
}
 
// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest)
{
    if (src >= graph->cap || dest >= graph->cap) {
        fprintf(stderr, "Sorry, wrong node id src=%d dest=%d", src, dest);
        return;
    }

    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    ++graph->degree[src];
 
    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
    ++graph->degree[dest];
}
 
// A utility function to print the adjacenncy list representation of graph
void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d (degree %d)\n head ", v, graph->degree[v]);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->dest);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

// return the id of newly joining node
int newNode(Graph* graph)
{
    if ( graph->cap >= graph->V) {
        fprintf(stderr, "Sorry, insufficient memory space when adding node.\n");
        return -1;
    }
    return graph->cap++;
}

// find a random node ID with desired degree 
// @TODO complete randomness
int findNode(Graph* graph, int deg) 
{
    int rnum = rand() % graph->cap;
    for (int i = rnum; i < graph->cap; ++i)
    {
        if (graph->degree[i] == deg)
        {
            return i;
        } 
    }
    for (int i = 0; i < rnum; ++i)
    {
        if (graph->degree[i] == deg)
        {
            return i;
        }
    }
    fprintf(stderr, "No node has degree %d\n", deg);
    return -1;
}

// add k neighbors when a new node join the network
int joinNode(Graph* graph, int k, int m)
{
    int id = newNode(graph);
    int degree;
    int k
    while(k>0)
    {
        degree = rand() % (m-k) + k;  
        neighbor = findNode(graph, degree);
        if (neighbor != -1)
            addEdge(graph, id, neighbor); 
        --k;
    }
    return 1;
}

// Driver program to test above functions
int main()
{
    srand(time(NULL));

    int V = 1000;
    int k = 1;
    int m = 10;

    // create the initial graph 
    struct Graph* graph = createGraph(V);
    for (int i; ;)
    {

    }  
    int a = newNode(graph);
    int b = newNode(graph);
    int c = newNode(graph);
    int d = newNode(graph);
    int e = newNode(graph);
    addEdge(graph, a, b);
    addEdge(graph, c, d);
    addEdge(graph, e, a);
    printf("=%d", findNode(graph, 1) );
 
    // print the adjacency list representation of the above graph
    printGraph(graph);
 
    return 0;
}
