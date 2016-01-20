#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <math.h>       /* pow */
#include <map>
#include <queue>
#include "graph.h"


//#define ADD_THEN_REMOVE


using namespace std;

struct Delay
{
    int nodeid;
    int delay;
    Delay(int n1, int n2) : nodeid(n1), delay(n2) { }
    bool operator<(const struct Delay& other) const
    {
        return delay < other.delay;
    }
};


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
    int mydegree;
    int i = k;
    int neighbor;
    int rn;

    //let every node determines itw own responds time
    //return top k nodes
    priority_queue<struct Delay> topK;
    int max = INT_MAX;
    for (int i = 0; i < graph->cap; ++i)
    {
        if (graph->degree[i] >= k)
        {
            int t0 = (1000 * m) / graph->degree[i];
            int treal = (rand() % t0);
            if (treal < max)
            {
                topK.push( Delay(i,treal) );
                struct Delay tmp50 = topK.top();
                max = tmp50.delay;
                if(topK.size() > k)
                    topK.pop();
            }
        }
    }
    //connects to these k nodes
    while( !topK.empty() )
    {        
        struct Delay tmp = topK.top();
        //cout << tmp.delay << ", degree=" << graph->degree[tmp.nodeid] << endl;
        addEdge(graph, id, tmp.nodeid);
        topK.pop();
    }
}

// Driver program to test above functions
int main()
{
    srand(time(NULL));

    int V = 150000;
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
    statics(graph, k, m);

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
    int init_start = 5000;
    for (int i = 2*k + 1; i < init_start; ++i)
    {
        joinNode(graph, k, m);
    }
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
    }    
    statics(graph, k, m);
#endif
    return 0;
}

