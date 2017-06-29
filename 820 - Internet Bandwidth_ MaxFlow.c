/*820 - Internet Bandwidth_ MaxFlow*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DBUGMs
#define LargeINT 1000000000
int cmp(const void* a, const void *b);
typedef struct {
    int flow;
    int capacity;
    int edgeLen;
} edge;
typedef struct {
    int x, y;
} point;

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k, l;
    int caseNumber = 1;
    while(1){
        #ifdef DBUGM
           printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int n;
        scanf("%d", &n);
        if(n == 0)break;

        int root, target, eNumber;
        scanf("%d %d %d", &root, &target, &eNumber);
        root--;
        target--;
        edge graph[n][n];
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                graph[i][j].edgeLen = 0;
                graph[i][j].flow = 0;
                graph[i][j].capacity = 0;
            }
        }
        for(i = 0; i < eNumber; i++){
            int a, b, f;
            scanf("%d %d %d", &a, &b, &f);
            a--;
            b--;
            graph[a][b].capacity += f;
            graph[b][a].capacity += f;
        }
        int maxFlow = MinCostMaxFlow(n, graph, root, target);
        printf("Network %d\n", caseNumber);
        printf("The bandwidth is %d.\n\n", maxFlow);
        caseNumber++;
    }
    return 0;
}
int MinCostMaxFlow(int size, edge graph[size][size], int root, int target)
{
    int i, j, k;
    int p;
    while( 1 ){
        int parent[size];
        if(!FindPath(size, graph, parent, root, target)) break;
        int minCut = LargeINT;
        int p = target;
        while(1){
            if(parent[p] == -1)break;
            if(minCut > graph[parent[p]][p].capacity - graph[parent[p]][p].flow) minCut = graph[parent[p]][p].capacity - graph[parent[p]][p].flow;
            p = parent[p];
        }
        p = target;
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
    int flow = 0;
    for(i = 0; i < size; i++){
        if(graph[i][target].flow > 0) {
            flow += graph[i][target].flow;
        }
    }
    return flow;
}
int FindPath(int size, edge graph[size][size], int parent[size], int root, int target)
{
    /*initial parent as -1*/
    int i, j, k;
    for(i = 0; i < size; i++) parent[i] = -1;

    /*Modify Dijkstra's  Shortest path for negative edge*/
    int key[size], visited[size], vn = 0;
    for(i = 0; i < size; i++) key[i] = LargeINT, visited[i] = 0;

    key[root] = 0;
    #ifdef DBUGM
        for(i = 0; i < size; i++){
            printf("%10d ", i);
        }printf("\n\n");
    #endif
    while(1){
        int deQ = -1;
        for(i = 0; i < size; i++){
            if(visited[i] == 0) {
                deQ = i;
                break;
            }
        }
        if(deQ == -1) break;
        for(i = 0; i < size; i++){
            if(graph[deQ][i].capacity > graph[deQ][i].flow && graph[deQ][i].edgeLen + key[deQ] < key[i]){
                key[i] = graph[deQ][i].edgeLen + key[deQ];
                parent[i] =deQ;
                visited[i] = 0;
            }
        }
        #ifdef DBUGM
        printf("Relax key and parent%d\n", deQ);
        for(i = 0; i < size; i++){
            printf("%10d ", key[i]);
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
            printf("%2d ",graph[i][j].edgeLen);
        }printf("\n");
    }printf("\n");
}
