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

void topK(priority_queue<struct Delay> *topK)
{

}

// add k neighbors when a new node join the network
void joinNode(Graph* graph, int k, int m)
{

    int id = newNode(graph); // node to be added
    int mydegree;
    int i;
    int neighbor;
    int rn;

    //let every node determines itw own responds time
    //return top k nodes
    priority_queue<struct Delay> topK;
    int max = 9999999;
    for (int trial = 0; trial < 500; ++trial)
    {
        while (1)
        {
            i = rand() % graph->cap;
            if (graph->degree[i] >= k && graph->degree[i] < m && ! isConnected(graph, id, i)) break;
        }
        int t0 = (1000 * m) / graph->degree[i];
        int treal = (rand() % t0);
        if (treal < max)
        {
            topK.push( Delay(i,treal) );
            struct Delay tmp50 = topK.top();
            max = tmp50.delay;
            while (topK.size() > k)
                topK.pop();
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

void gaian_remove(struct Graph *graph, int k, int m, int rid)
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
int main(int argc, char *argv[])
{
    srand(time(NULL));

    int V = 150000;
    int k = atoi(argv[1]);
    int m = atoi(argv[2]);

    int init_start = 5000;
    int repeats = 10;
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
            if ( rand() % 3)   
                joinNode(graph, k, m);
            else 
            {
                int rid;
                while (1)
                {
                    rid = rand() % graph->cap;
                    if (graph->degree[rid] < 2*k) break;
                }
                gaian_remove(graph, k, m, rid);
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

