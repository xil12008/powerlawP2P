#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <math.h>       /* pow */
#include <map>
#include <set>
#include "graph.h"


//#define ADD_THEN_REMOVE
#define TRIALS

using namespace std;

//randomly generate degree by distribution ai
void initGraph(Graph* graph, int k)
{
    // create the initial graph 
    for (int i = 0; i < 2*k + 1; ++i) 
        newNode(graph);
    for (int i = 0; i < 2*k + 1; ++i)
    {
        for(int j = i; j < 2*k + 1; ++j)
        {
            if (i != j)
            {
                addEdge(graph, i, j);
            }
        }
    }
}

// return the number of hits in the graph 
int hitsFL(Graph* graph, int ttl)
{
    int hits = 0;
    vector<int> front;
    for (int trials = 0; trials < TRIALS; ++trials)
    {
        for ( int i = 0; i < graph->cap; ++i)
        {
            graph->marker[i] = UNVISITTED;
        }
        //find a random node in network
        while (1)
        {
            rid = rand() % graph->cap;
            if (graph->degree[rid] >= 1) break; //ignore the range of degree
        }
        graph->marker[rid] == VISITTED;
        front.insert(rid);
        ++hits;
        while ( !front.empty() ) 
        {
            struct AdjlistNode* pCrawl = graph->array[front.pop_back()].head;
            while (pCrawl)
            {
                if (graph->marker[pCrawl->dest] == UNVISITTED)
                {
                    graph->marker[pCrawl->dest] = VISITTED;
                    front.insert(pCrawl->dest);
                    ++hits;
                }
                pCrawl = pCrawl -> next;
            }   
        } 
    }
}

// Driver program to test above functions
int main()
{
    srand(time(NULL));

    int V = 10000;
    int k = 2;
    int m = 10;
    double gamma = 2.5;
    
    struct Graph* graph = createGraph(V);
    initGraph(graph, k);

#ifdef ADD_THEN_REMOVE
    printf("============start adding=============\n");
    for (int i = 2*k + 1; i < V; ++i)
    {
        joinNode(graph, k, m);
    }  
    //printGraph(graph);
    statics(graph, k, m);

    //return 0;

    printf("=========now, remove==============\n");
    for (int i = 0; i < V/3; ++i)
    {
        int rid;
        while (1)
        {
            rid = rand() % graph->cap;
            if (graph->degree[rid] >= k) break;
        }
        removeAllEdges(graph, rid);
    } 
    //printGraph(graph);
    statics(graph, k, m);
#else
    int init_start = 1000;
    printf("Pattern 1\n");
    // nodes join
    for (int i = 2*k + 1; i < init_start; ++i)
    {
        joinNode(graph, k, m);
    }
    //printf("a\n");
    statics(graph, k, m);
    for (int i = init_start; i < V; ++i)
    {
        if ( rand()%3 )   
            joinNode(graph, k, m);
        else 
        {
            int rid;
            while (1)
            {
                rid = rand() % graph->cap;
                if (graph->degree[rid] >= k) break;
            }
            removeAllEdges(graph, rid);
        }   
        if ( i % V/10 == 0)
        {
            printf("------------grow to %d------------\n", i);
            statics(graph, k, m);
            printf("---------------------------------\n");
        }
    }    
    //printf("Pattern2\n");
    statics(graph, k, m);
#endif
    return 0;
}

