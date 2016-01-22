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

// add k neighbors when a new node join the network
void joinNode(Graph* graph, int k, int m)
{
    int id = newNode(graph); // node to be added
    int rid;

    //find a random node in network
    while (1)
    {
        rid = rand() % graph->cap;
        if (graph->degree[rid] >= k && graph->degree[rid] < m && rid != id) break;
    }
    addEdge(graph, id, rid);
    
    int tmp59;
    set<int> route;
    route.insert(id);
    int count = 0;
    while (graph->degree[id] < k && count++ < 1000)
    {
        int count2 = 0;
        while (count2 ++ < 1000)
        {
            tmp59 = randomNeighbor(graph, rid);   
            //connect to this node and random neighbor of it, making sure no duplicated node
            if (tmp59 != -1 && route.find(tmp59) == route.end()) break;
        }
        route.insert(tmp59);
        //printf("%d\n", tmp59);     
        if (graph->degree[tmp59] < m)
            addEdge(graph, id, tmp59);
        rid = tmp59;
    }
    //printf("join, degree = %d\n", graph->degree[id]);
}

void hapa_remove(struct Graph *graph, int k, int m, int rid)
{
    vector<int> v;
    struct AdjListNode* pCrawl = graph->array[rid].head;
    while (pCrawl)
    {
        v.push_back(pCrawl->dest);
        pCrawl = pCrawl->next;
    }
    removeAllEdges(graph, rid);

    for (vector<int>::iterator it = v.begin() ; it != v.end(); ++it)
    {
        int id = *it;
        while (graph->degree[id] < k) // to keep the min degree k
        {
            int rid2;
            while (1)
            {
                rid2 = rand() % graph->cap;
                if (graph->degree[rid2] >= k && graph->degree[rid2] < m && ! isConnected(graph, id, rid2)) break;
            }
            addEdge(graph, id, rid2);
            //printf(" + 1 ->%d\n", graph->degree[id]);
        }
    }
}

// Driver program to test above functions
int main()
{
    srand(time(NULL));

    int V = 150000;
    int k = 3;
    int m = 50;
    double gamma = 2.5;
    int init_start = 5000;
    int repeats = 20;
    map<int, double> summap;
    for (int i = 0;i < 100; ++i)
    {
        summap[i] = 0;
    }
    for (int i = 0;i < repeats; ++i)
    {
        printf("Repeated %d time\n", i);
        struct Graph* graph = createGraph(V);
        initGraph(graph, k);
        for (int i = 2*k + 1; i < init_start; ++i)
            joinNode(graph, k, m);
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
                hapa_remove(graph, k, m, rid);
            }   
        }    
        statics( graph, k, m, &summap); 
        deleteGraph(graph);
    }
    printf("====================\n[");
    for( int i = 0; i < 100; ++i)
    {
        printf(" %f, ", (double) 1.0 * summap[i] / repeats );
        summap[i] += (double) 1.0 * summap[i] / repeats;
    }
    printf(" %f ", (double) 1.0 * summap[100] / repeats );    
    printf("]\n");
    return 0;
}

