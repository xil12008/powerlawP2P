// graph.h
#ifndef _GRAPH_H // must be unique name in the project
#define _GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>       /* pow */
#include <map>
#include <queue>

using namespace std;

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
    int* marker;
    struct AdjList* array;
};
 
// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest);

// A utility function that creates a graph of V vertices (memory allocation)
struct Graph* createGraph(int V);
void deleteGraph(Graph* graph);

void addEdge(struct Graph* graph, int src, int dest);
void printGraph(struct Graph* graph);
void printNode(struct Graph* graph, int v);
int newNode(Graph* graph);
bool isConnected(Graph* graph, int a, int b);
int findNode(Graph* graph, int deg, int forbid);
void removeEdge(Graph* graph, int src, int dst);
void removeAllEdges(Graph* graph, int v);
int statics(Graph *graph, int k, int m, map<int, double> * summap);
int randomNeighbor(Graph* graph, int id);

void hitsFL(Graph* graph, int k, int ttl, map<int, double> *hitssummap, int size);

#endif
