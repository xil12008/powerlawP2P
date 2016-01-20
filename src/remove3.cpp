#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>       /* pow */
#include <map>
#include "graph.h"

using namespace std;

//#define TRIAL_FIND_DEGREE 20
//#define ADD_THEN_REMOVE

//randomly generate degree by distribution ai
int joinDegree(int k, int m, vector<double>* ai)
{
    double rsum = (double) (1.0 * rand() / RAND_MAX);
    //printf("rsum=%f\t", rsum);
    for (int i = 0; i < m-k; ++i)
    {
        rsum -= (double) (*ai)[i] / k;
        //printf("rsum - %f =%f\t", (double) (*ai)[i] / k, rsum);
        if (rsum < 0.0001)
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
            while (mydegree >= k)
            {
                rn = findNode(graph, mydegree, id);
                if (rn != -1)
                {
                    //printf("Connect neighbor with degree %d\n", graph->degree[rn]);   
                    addEdge(graph, id, rn); 
                    break;
                }
                --mydegree;
            }

            //@TODO in this case, some improvements could be made by
            //choosing a node closer to the desired degree "mydegree".
            while (mydegree < k) // prevent connecting to a non-existing node
            {
                rn = rand() % graph->cap;
                if (graph->degree[rn] >= k && graph->degree[rn] < m && ! isConnected(graph, rn, id) )
                {
                    break;
                }
            }
            if (mydegree < k)
            {
                //printf("Connect random neighbor with degree %d\n", graph->degree[rn]);   
                addEdge(graph, id, rn);    
            }
        }
        --i;
    }
}

vector<double>* generate_ai(int k, int m, double gamma)
{
    vector<double>* v = new vector<double>();
    //calculate f[i], a[i]
    double sum = 0;
    for (int i = k; i < m; ++i)
    {
        sum += 1.0 * (m - i) / pow(i, gamma);
    }
    double ai = 1;
    double hold = 0;
    for (int i = k; i < m; ++i)
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
    printf("%f]\n", 1 - hold);

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

void statics_neighbor_degree(Graph* graph, int k, int m, int b)
{
    map<int, int> nd; //neighbor's degree
    for( int i = k; i <= m; ++i)
    {
        nd[i] = 0;
    }
    int count = 0;
    for ( int i = 0; i < graph->cap; ++i)
    {
        int d = graph->degree[i];
        if ( d == b )
        {
            struct AdjListNode* pCrawl = graph->array[i].head;
            while( pCrawl )
            {
                //printf("i=%d(%d) neighbor=%d nd=%d\n", i, b, pCrawl->dest, graph->degree[pCrawl->dest]);
                ++nd[graph->degree[pCrawl->dest]];
                pCrawl = pCrawl->next;
            }
            count++;            
        }
    }
    printf("nd[%d] = [", b);
    double checksum = 0;
    for (int i = k; i < m ; ++i)
    {
        printf("%f,\t", (double)(1.0 * nd[i] / count));
        checksum += (double)(1.0 * nd[i] / count);
    }
    printf("%f]\n", (double)(1.0 * nd[m] / count));
    //printf("checksum=%f\n", checksum);
}

//====================================//
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

void delete_case1(Graph* graph, int helper, struct AdjListNode* pCrawl)
{
    int neighbor = randomNeighbor(graph, helper);
    while (isConnected(graph, neighbor, pCrawl->dest) || neighbor == pCrawl->dest)
    {
        /*
        printf("Stuck finding neighbor here\n");
        if (isConnected(graph, neighbor, pCrawl->dest))
            printf("%d connected with %d", neighbor, pCrawl->dest);
        else if (neighbor == pCrawl->dest)
            printf("neighbor %d is pCrawl->dest %d", neighbor, pCrawl->dest);
        printNode(graph, helper); 
        */
        neighbor = randomNeighbor(graph, helper);
    }
    //printf("%d--X--%d--+--%d\n", helper, neighbor, pCrawl->dest);
    removeEdge(graph, helper, neighbor);
    removeEdge(graph, neighbor, helper);
    addEdge(graph, neighbor, pCrawl->dest);
}

void deleteNode(int id, Graph* graph, int k, int m, int ithnode, vector<double>* ai)
{
    int b = graph->degree[id];
    //printf("R= %d b=%d\n", id, b);
    struct AdjListNode* pCrawl = graph->array[id].head;
    int mydegree, helper;
    for (int i = 0; i < k; ++i)
    {
        mydegree = joinDegree(k, m, ai) + 1; // +1 because q(i+1) = a(i)
        //printf("delete degree=%d\n", mydegree);
        helper = findNode(graph, mydegree, pCrawl->dest); 
        if (helper == -1)
        {
            //@TODO in this case, some improvements could be made by
            //choosing a node closer to the desired degree "mydegree".
            // @TODO and should not let pCrawl->dest be removed if its' degree = k
            if (mydegree == m)
            {               
                printf("No node has degree %d. So skip one shuffle. %dth node\n" ,
                                     mydegree, ithnode);
            }
            else
            {
                int helper_2 = findNode(graph, mydegree + 1, pCrawl->dest); 
                if (helper_2 != -1)
                {
                    delete_case1(graph, helper_2, pCrawl);
                }
                else
                {
                    printf("No node has degree %d or %d. So skip one shuffle. %dth node\n" ,
                                         mydegree, mydegree + 1, ithnode);
                }
            }
        }
        else{
            delete_case1(graph, helper, pCrawl);
        }
        pCrawl = pCrawl->next;
    }
    int degree_accum = k;
    while (pCrawl)
    {
        //printf("push degree=%d\t", degree_accum);
        helper = findNode(graph, degree_accum, id); 
        if (helper == -1)
        {
            // @TODO should not let pCrawl->dest be removed if its' degree = k
            printf("No node has degree %d. So skip one push.%dth node\n" , mydegree, ithnode);
        }
        else{
            addEdge(graph, helper, pCrawl->dest);
            //printf("%d push %d( => degree%d)\n", pCrawl->dest, helper, graph->degree[helper]);
        }
        pCrawl = pCrawl->next;
        ++degree_accum;
    }
    removeAllEdges(graph, id);
}

void deleteNodeSHUFFLE(struct Graph* graph, int k, int m, int ithnode, struct AdjListNode* pCrawl, vector<double>* ai)
{
    int mydegree = joinDegree(k, m, ai) + 1; // +1 because q(i+1) = a(i)
    //printf("delete degree=%d\n", mydegree);
    int helper = findNode(graph, mydegree, pCrawl->dest); 
    if (helper == -1)
    {
        //@TODO in this case, some improvements could be made by
        //choosing a node closer to the desired degree "mydegree".
        // @TODO and should not let pCrawl->dest be removed if its' degree = k
        /*
        if (mydegree == m)
        {               
            printf("No node has degree %d. So skip one shuffle. %dth node\n" ,
                                 mydegree, ithnode);
        }
        else
        {
            int helper_2 = findNode(graph, mydegree + 1, pCrawl->dest); 
            if (helper_2 != -1)
            {
                delete_case1(graph, helper_2, pCrawl);
            }
            else
            {
                printf("No node has degree %d or %d. So skip one shuffle. %dth node\n" ,
                                     mydegree, mydegree + 1, ithnode);
            }
        }
        statics(graph, k, m);*/
        printf("No node has degree %d. So skip one shuffle. %dth node\n" ,
                                 mydegree, ithnode);
    }
    else{
        delete_case1(graph, helper, pCrawl);
    }
}

void deleteNodePUSH(struct Graph* graph, int k, int m, int id, int b, int ithnode, struct AdjListNode* pCrawl, int degree_accum)
{
    //printf("push degree=%d\t", degree_accum);
    int randdegree = (int) rand() % (b-k) + k;
    int helper = findNode(graph, randdegree, id); 
    if (helper == -1)
    {
        // @TODO should not let pCrawl->dest be removed if its' degree = k
        printf("No node has degree %d. So skip one push.%dth node\n" , b, ithnode);
    }
    else
    {
        addEdge(graph, helper, pCrawl->dest);
        //printf("%d push %d( => degree%d)\n", pCrawl->dest, helper, graph->degree[helper]);
    }
}

void deleteNode_Distributed(int id, Graph* graph, int k, int m, int ithnode, vector<double>* ai)
{
    int b = graph->degree[id];
    int degree_accum = k;
    //printf("R= %d b=%d\n", id, b);
    struct AdjListNode* pCrawl = graph->array[id].head;
    for (int i = 0; i < b; ++i)
    {
        int randint = (int) rand() % b;
        if (randint < k) // randint = 0 ~ k-1
        {
            deleteNodeSHUFFLE(graph, k, m, ithnode, pCrawl, ai);
        }
        else     // randint = k ~ b-1
        {
            deleteNodePUSH(graph, k, m, id, b, ithnode, pCrawl, degree_accum);
        }
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

    int V = 150000;
    int k = 2;
    int m = 10;
    double gamma = 2.5;
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
        vector<double> *ai = generate_ai(k, m, gamma);
        for (int i = 2*k + 1; i < init_start; ++i)
            joinNode(graph, k, m, ai);
        for (int i = init_start; i < V; ++i)
        {
            if ( rand()%3 )   
                joinNode(graph, k, m, ai);
            else 
            {
                int rid;
                while (1)
                {
                    rid = rand() % graph->cap;
                    if (graph->degree[rid] >= k) break;
                }
                deleteNode_Distributed(rid, graph, k, m, i, ai);
                //removeAllEdges(graph, rid);
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
    //for printing purpose
    generate_ai(k, m, gamma);
    return 0;
}

