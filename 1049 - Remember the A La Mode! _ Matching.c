/*1049 - Remember the A La Mode! _ Matching*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define FlowFactor 10000
#define errorT 0.005
typedef struct {
    int flow;
    int capacity;
    double edgeLen;
} edge;
typedef struct {
    int x, y;
} point;

double MinCostMaxFlow(int size, edge graph[size][size]);
double MaxCostMaxFlow(int size, edge graph[size][size]);
void ShowGraph(int size, edge graph[size][size]);

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k, l, m, n;
    int caseNumber = 1;
    while(1){

        int pie, ice;
        scanf("%d%d", &pie, &ice);
        if(pie == 0 && ice == 0)break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        double minCost, maxCost= 100;

        int size = pie +1 + ice +1;
        edge graph[size][size];

        ConstructGraph(size, graph, pie, ice);
        minCost =  MinCostMaxFlow(size, graph);

        ReConstructGraph(size, graph, pie, ice);
        maxCost =  MaxCostMaxFlow(size, graph);




        printf("Problem %d: %.2lf to %.2lf\n", caseNumber, minCost, maxCost);
        caseNumber++;
    }
    return 0;
}
int ConstructGraph(int size, edge graph[size][size], int pie, int ice)
{
    /*0 is super source, size-1 is super target, 1~n is source n+1~2n is target*/
    int i, j, k;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++) {
            graph[i][j].edgeLen = 0;
            graph[i][j].flow = 0;
            graph[i][j].capacity = -LargeINT;
        }
    }
    for(i = 0; i < pie; i++){
        double pCapacity;
        scanf("%lf", &pCapacity);
        graph[0][i+1].edgeLen = 0;
        graph[0][i+1].capacity = pCapacity;
        graph[i+1][0].edgeLen = 0;
        graph[i+1][0].capacity = pCapacity;
        /*graph[i+1][0].flow = pCapacity;*/
    }
    for(i = 0; i < ice; i++){
        double iCapacity;
        scanf("%lf", &iCapacity);
        graph[size-1][i+pie+1].edgeLen = 0;
        /*graph[size-1][i+pie+1].capacity = iCapacity;*/
        graph[size-1][i+pie+1].flow = iCapacity;
        graph[i+pie+1][size-1].edgeLen = 0;
        graph[i+pie+1][size-1].capacity = iCapacity;
    }

    for(i = 0; i < pie; i++){
        for(j = 0; j < ice; j++){
            double len;
            scanf("%lf", &len);
            if(len < 0 )continue;
            graph[i+1][j+pie+1].edgeLen = len;
            graph[i+1][j+pie+1].capacity = LargeINT;
            graph[j+pie+1][i+1].edgeLen = -len;
            graph[j+pie+1][i+1].capacity = LargeINT;
            graph[j+pie+1][i+1].flow = LargeINT;
        }
    }
    #ifdef DBUGM
        ShowGraph(size, graph);
    #endif
    return 1;
}
double MinCostMaxFlow(int size, edge graph[size][size])
{
    int i, j, k;
    int p;
    while( 1 ){
        int parent[size];
        if(!FindPath(size, graph, parent, 0, size-1)) break;
        int minCut = LargeINT;
        int p = size-1;
        while(1){
            if(parent[p] == -1)break;
            if(minCut > graph[parent[p]][p].capacity - graph[parent[p]][p].flow) minCut = graph[parent[p]][p].capacity - graph[parent[p]][p].flow;
            p = parent[p];
        }
        p = size-1;
        while(1){
            if(parent[p] == -1)break;
            graph[parent[p]][p].flow += minCut;
            graph[p][parent[p]].flow -= minCut;
            p = parent[p];
        }
        #ifdef DBUGM
            ShowGraph(size, graph);
        #endif
    }
    double cost = 0;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++) {
            if(graph[i][j].flow > 0 && graph[i][j].edgeLen > 0) {
                cost += (graph[i][j].edgeLen)*graph[i][j].flow;
            }
        }
    }
    return cost;
}
int FindPath(int size, edge graph[size][size], int parent[size], int root, int target)
{
    /*initial parent as -1*/
    int i, j, k;
    for(i = 0; i < size; i++) parent[i] = -1;

    /*Modify Dijkstra's  Shortest path for negative edge*/
    int visited[size], vn = 0;
    double key[size];
    for(i = 0; i < size; i++) key[i] = LargeINT, visited[i] = 0;

    key[root] = 0;
    #ifdef DBUGM
        for(i = 0; i < size; i++){
            printf("%10d ", i);
        }printf("\n\n");
    #endif
    while(1){
        int deQ = -1;
        for(i = 0; i < size; i++) if(visited[i] == 0){
            deQ = i;
            break;
        }
        if(deQ == -1) break;
        visited[i] = 1;
        for(i = 0; i < size; i++){
            if(graph[deQ][i].capacity > graph[deQ][i].flow && graph[deQ][i].edgeLen + key[deQ] < key[i] - errorT){
                key[i] = graph[deQ][i].edgeLen + key[deQ];
                parent[i] =deQ;
                visited[i] = 0;
            }
        }
        #ifdef DBUGM
        printf("Relax key and parent%d\n", deQ);
        for(i = 0; i < size; i++){
            printf("%10lf ", key[i]);
        }printf("\n");
        for(i = 0; i < size; i++){
            printf("%10d ", parent[i]);
        }
        printf("\n");
        #endif
        visited[deQ] = 1;
        vn++;
    }
    return parent[target] != -1;
}
int ReConstructGraph(int size, edge graph[size][size], int pie, int ice)
{
    /*0 is super source, size-1 is super target, 1~n is source n+1~2n is target*/

    int i, j, k;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++) {
            graph[i][j].flow = 0;
        }
    }
    for(i = 0; i < pie; i++){
        graph[i+1][0].flow = graph[i+1][0].capacity;
    }
    for(i = 0; i < ice; i++){
        graph[size-1][i+pie+1].flow = graph[size-1][i+pie+1].capacity;
    }

    for(i = 0; i < pie; i++){
        for(j = 0; j < ice; j++){
            graph[i+1][j+pie+1].edgeLen = FlowFactor - graph[i+1][j+pie+1].edgeLen ;
            graph[j+pie+1][i+1].edgeLen = -(FlowFactor + graph[j+pie+1][i+1].edgeLen);
            graph[j+pie+1][i+1].flow = LargeINT;
        }
    }
    #ifdef DBUGM
        ShowGraph(size, graph);
    #endif
    return 1;
}
double MaxCostMaxFlow(int size, edge graph[size][size])
{
    int i, j, k;
    int p;
    while( 1 ){
        int parent[size];
        if(!FindPath(size, graph, parent, 0, size-1)) break;
        int minCut = LargeINT;
        int p = size-1;
        while(1){
            if(parent[p] == -1)break;
            if(minCut > graph[parent[p]][p].capacity - graph[parent[p]][p].flow) minCut = graph[parent[p]][p].capacity - graph[parent[p]][p].flow;
            p = parent[p];
        }
        p = size-1;
        while(1){
            if(parent[p] == -1)break;
            graph[parent[p]][p].flow += minCut;
            graph[p][parent[p]].flow -= minCut;
            p = parent[p];
        }
        #ifdef DBUGM
            ShowGraph(size, graph);
        #endif
    }
    double cost = 0;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++) {
            if(graph[i][j].flow > 0 && graph[i][j].edgeLen > 0) {
                cost += (-(graph[i][j].edgeLen - FlowFactor))*graph[i][j].flow;
                /*cost += (graph[i][j].edgeLen)*graph[i][j].flow;*/
            }
        }
    }
    return cost;
}
void ShowGraph(int size, edge graph[size][size])
{
    int i, j, k;
    printf("Capacity:\n");
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            printf("%2d ",graph[i][j].capacity);
        }printf("\n");
    }printf("\n");

    printf("flow:\n");
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            printf("%2d ",graph[i][j].flow);
        }printf("\n");
    }printf("\n");

    printf("edgeLen:\n");
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            printf("%2lf ",graph[i][j].edgeLen);
        }printf("\n");
    }printf("\n");
}

