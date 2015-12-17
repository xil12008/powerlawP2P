// A C Program to demonstrate adjacency list representation of graphs
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>       /* pow */
#include <map>

using namespace std;

#define TRIAL_FIND_DEGREE 20

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
        fprintf(stderr, "Sorry, wrong node id src=%d dest=%d.\n", src, dest);
        return;
    }
    //if (graph->degree[src] == 0 || graph->degree[dest] == 0)
    //{
    //    fprintf(stderr, "Sorry, node does NOT exist. node id src=%d dest=%d.\n", src, dest);
    //    return;
    //}

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
    printf("  cap=%d", graph->cap);
    for (v = 0; v < graph->cap; ++v)
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
    return graph->cap++; //the assigned node id increases by 1
}

// One edge from a --> b  ??
bool isConnected(Graph* graph, int a, int b)
{
    if (a == -1 || b == -1) return false;
    struct AdjListNode* pCrawl = graph->array[a].head;
    while (pCrawl)
    {
        if (pCrawl->dest == b)
        {
            //printf("conflict:%d->%d\n", a, b);
            return true;  
        } 
        pCrawl = pCrawl->next;
    }
    return false;
}

// find a random node ID with desired degree 
// @TODO complete randomness
int findNode(Graph* graph, int deg, int forbid = -1) 
{
    int rnum;
    for (int i = 0; i < TRIAL_FIND_DEGREE; ++i)
    {
        rnum = rand() % graph->cap;
        // not connected with forbid, in case return the same node twice.
        if (graph->degree[rnum] == deg && ! isConnected(graph, rnum, forbid) )
        {
            return rnum;
        } 
    }
    //if it did not find the node with desired deg, then
    vector<int> vec; 
    for (int i = 0; i < graph->cap; ++i)
    {
        if (graph->degree[i] == deg)
        {
            vec.push_back(i);
        }
    }
    if (vec.size() > 0)
    {
        int tmp_count = 30 * vec.size();
        while (--tmp_count > 0) 
        {
            int ret = rand() % vec.size();
            if( ! isConnected(graph, vec[ret], forbid) && ! graph->degree[vec[ret]] == 0)
                return vec[ret];
        }
    }
    fprintf(stderr, "WARNING: No node has degree %d (current graph size = %d)\n", deg, graph->cap);
    return -1;
}

//randomly generate degree by distribution ai
int joinDegree(int k, int m, vector<double>* ai)
{
    double rsum = (double) rand() / (RAND_MAX);
    //printf("rsum=%f\t", rsum);
    for (int i = 0; i < m-k; ++i)
    {
        rsum -= (double) (*ai)[i] / k;
        //printf("rsum - %f =%f\t", (double) (*ai)[i] / k, rsum);
        if (rsum < 0)
        {
            //printf("return %d\n", i + k);
            return i + k;
        }
    } 
    fprintf(stderr, "ERROR generate degree!\n");
    return m - 1;
}

// add k neighbors when a new node join the network
void joinNode(Graph* graph, int k, int m, vector<double>* ai)
{
    int id = newNode(graph); // node to be added
    int mydegree;
    int i = k;
    int neighbor;
    int rn;
    while (i > 0)
    {   
        mydegree = joinDegree(k, m, ai);
        // passing parameter id to findNode() because
        // findNode() should not return a neighbor of node id.
        neighbor = findNode(graph, mydegree, id); 
        if (neighbor != -1)
        {
            addEdge(graph, id, neighbor); 
        }
        else
        {
            //@TODO in this case, some improvements could be made by
            //choosing a node closer to the desired degree "mydegree".
            while (1) // prevent connecting to a non-exist node
            {
                rn = rand() % graph->cap;
                if (graph->degree[rn] >= k && graph->degree[rn] < m)
                {
                    break;
                }
            }
            addEdge(graph, id, rn);
        }
        --i;
    }
}

vector<double>* generate_ai(int k, int m, double gamma)
{
    vector<double>* v = new vector<double>();
    //calculate f[i], a[i]
    double sum = 0;
    for(int i = k; i < m; ++i)
    {
        sum += 1.0 * (m - i) / pow(i, gamma);
    }
    double ai = 1;
    double hold = 0;
    for(int i =k; i < m; ++i)
    {
        double fi = 1.0 * (m - 2 * k) / pow( i, gamma) / sum;
        hold += fi;
        ai = ai - fi;
        v->push_back(ai);
        printf("f%d=%f a%d=%f\n", i , fi, i, ai);
    }
    printf("f%d=%f\n", m, 1 - hold);

    printf("[");
    for( int i = k; i < m; ++i)
    {
        printf(" %f, ", 1.0 * (m - 2 * k) / pow( i, gamma) / sum );
    }
    printf("%f\n", 1 - hold);
    printf("]\n");

    //check if correct
    printf("%f == %f\n", 1.0 - hold, ai);
    return v;
}

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

void statics(Graph *graph, int k, int m)
{
    map<int, int> v;
    for( int i = k; i <= m; ++i)
    {
        v[i] = 0;
    }
    int count = 0;
    double checksum = 0;
    for ( int i = 0; i < graph->cap; ++i)
    {
        int d = graph->degree[i];
        if (d >= k && d <= m )
        {
            v[ graph->degree[i] ] += 1;
            count++;            
        }
        else if ( d != 0) 
        {
            printf("Statics exception: node %d has degree %d.\n", i, d);
        }
    }
    printf("total %d nodes\n", count);
    for( int i = k; i <= m; ++i)
    {
        //printf("d%i fraction=%f\n", i, (double) 1.0 * v[i] / count );
        checksum += (double) 1.0 * v[i] / count;
    }
    printf("[");
    for( int i = k; i < m; ++i)
    {
        printf(" %f, ", (double) 1.0 * v[i] / count );
    }
    printf(" %f ", (double) 1.0 * v[m] / count );
    printf("]\n");
    printf("checksum = %f\n", checksum);
}

int findK(Graph* graph, int k, int id)
{
    int tmp = id;
    int a;
    int count = 10;
    while (graph->degree[tmp] > k && --count > -1)
    {
        tmp = (graph->array[tmp].head)->dest;
        if (tmp == id)
        {
            return -1;
        }
    }
    if (count > -1) return tmp;
    else return -2;
}

void removeEdge(Graph* graph, int src, int dst)
{
    struct AdjListNode* pCrawl = graph->array[src].head;
    struct AdjListNode* prev = graph->array[src].head;
    while (pCrawl)
    {
        //printf("%d dst%d\n", pCrawl->dest, dst);
        if (pCrawl->dest == dst)
        {
            if (pCrawl == graph->array[src].head)
            {
                graph->array[src].head = pCrawl->next;
            }
            else
                prev->next = pCrawl->next;
            graph->degree[src]--;
            return;
        }
        prev = pCrawl;
        pCrawl = pCrawl->next;
    }
    fprintf(stderr, "ERROR: remove edge failed src=%d dst=%d\n", src, dst);
}

void removeAllEdges(Graph* graph, int v)
{
    struct AdjListNode* pCrawl = graph->array[v].head;
    while (pCrawl)
    {
        removeEdge(graph, pCrawl->dest, v);
        pCrawl = pCrawl->next;
    }
    graph->array[v].head = NULL;
    graph->degree[v] = 0;
}

//randomly generate degree by distribution ai
int deleteDegree(int k, int m, int b)
{
    double rsum = (double) rand() / (RAND_MAX);
    for (int i = 0; i < b-k; ++i)
    {
        rsum -= (double) 1.0 / (b - k);
        if (rsum < 0)
        {
            return i + k;
        }
    } 
    fprintf(stderr, "ERROR generate degree while removing node!\n");
    return - 1;
}

void deleteBroadcast(Graph* graph, int id, int k, int m, int b)
{
    if (b <= k)
    {
        fprintf(stderr, "ERROR: deleteBroadcast\n");
        return;
    }
    int degree = deleteDegree(k, m, b);
    //printf("degree=%d b=%d\n", degree, b);
    int node = findNode(graph, degree, id); 
    if (node != -1)
    {
        addEdge(graph, node, id);
        //printf("addEdge (%d, %d)\n", node, id);
    }
    else
    {
        int rn = -1;
        while (1) // prevent connecting to a non-exist node
        {
            rn = rand() % graph->cap;
            if (graph->degree[rn] >= k && graph->degree[rn] < b)
            {
                break;
            }
        }
        addEdge(graph, rn, id);
        //printf("addEdge (random=%d, %d)\n", rn, id);
    }
}

void deleteNode(int id, Graph* graph, int k, int m, int ithnode)
{
    int K = findK(graph, k, id);
    if (K == -1)
    {
        fprintf(stderr, "delete %d-th node\n", ithnode);
        //fprintf(stderr, "Error: delete node failed. Dead loop.\n");
        fprintf(stderr, "ERROR: delete node failed. finding node with degree k goes into a loop!\n");
        return;
    }
    if (K == -2)
    {
        fprintf(stderr, "delete %d-th node\n", ithnode);
        fprintf(stderr, "ERROR: delete node failed. No node with degree k in 10 last-connect hops.\n");
        return;
    }
    int b  = graph->degree[id];
    //printf("R=%d b=%d K=%d\n", id, b, K);

    //@TODO delete then remove, or remove then delete???
    if (b == k) K = id;
    removeAllEdges(graph, K); // if K and R are neighbors, then degree(R) = b-1 .
    struct AdjListNode* pCrawl = graph->array[id].head;
    /*for(int i = 0; i < graph->degree[id] - k; ++i)
    {
        //b-k or b-k-1 broadcasts...
        deleteBroadcast(graph, pCrawl->dest, k, m, graph->degree[id]);
        pCrawl = pCrawl->next;
    }*/
    while (pCrawl)
    {
        if (pCrawl->dest != K)
            addEdge(graph, K, pCrawl->dest);
        pCrawl = pCrawl->next;
    }
    removeAllEdges(graph, id);
}

void testJoinDegree(int k, int m, vector<double>* ai){
    int tmp = 0;
    int hold = 0;
    while(tmp++<100)
    {
        int degree = joinDegree(k, m, ai);
        if (degree == k) hold++;
        printf("%d\n", degree);
    }
    printf("hold==%d", hold);
}

// Driver program to test above functions
int main()
{
    srand(time(NULL));

    int V = 40000;
    int k = 2;
    int m = 10;
    double gamma = 2.5;
    
    struct Graph* graph = createGraph(V);
    initGraph(graph, k);
    printGraph(graph);
    printf("============start adding=============\n");

    vector<double> *ai = generate_ai(k, m, gamma);
    
    // nodes join
    for (int i = 2*k + 1; i< V; ++i)
    {
        joinNode(graph, k, m, ai);
    }  
    //printGraph(graph);
    statics(graph, k, m);

    printf("=========now, remove==============\n");
    //node quiting
    for (int i = 0; i< 10000; ++i)
    {
        int rid;
        while (1){
            rid = rand() % graph->cap;
            if (graph->degree[rid] >= k) break;
        }
        deleteNode(rid, graph, k, m, i);
        //removeAllEdges(graph, rid);
    } 
    //printGraph(graph);
    statics(graph, k, m);

    return 0;
}