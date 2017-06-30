/*10080 - Gopher II_ Matching */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DBUGM5
#define LargeINT 1000000000
int cmp(const void* a, const void *b);
typedef struct {
    int flow;
    int capacity;
    int edgeLen;
} edge;
typedef struct {
    double x, y;
} point;
double limit;
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k, l;
    int caseNumber = 1;
    while(1){
        int n, m;
        double s, t;
        if(scanf("%d %d %lf %lf", &n, &m, &s, &t) == EOF)break;
        if(n == 0 || m == 0 || s == 0 || t == 0){
            printf("%d\n", n);
        }
        #ifdef DBUGM
           printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        limit = s*t;
        point source[n];
        for(i = 0; i < n; i++){
            scanf("%lf %lf", &source[i].x, &source[i].y);
        }
        point target[m];
        for(i = 0; i < m; i++){
            scanf("%lf %lf", &target[i].x, &target[i].y);
        }
        int size = n+1 + m+1;
        edge graph[size][size];

        ConstructGraph(size, graph, n, source, m, target);
        /*Actually calculate max Flow without cose*/
        int match = MinCostMaxFlow(size, graph, 0, size-1);
        printf("%d\n", n - match);

    }
    return 0;
}

int ConstructGraph(int size, edge graph[size][size], int n, point source[n], int m, point target[m])
{
    /*0 is super source, size-1 is super target, 1~n is source n+1~2n is target*/
    int i, j, k;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++) {
            graph[i][j].edgeLen = 0;
            graph[i][j].flow = 0;
            graph[i][j].capacity = 0;
        }
    }
    for(i = 0; i < n; i++){
        graph[0][i+1].edgeLen = 0;
        graph[0][i+1].capacity = 1;
        graph[i+1][0].edgeLen = 0;
        graph[i+1][0].capacity = 1;
        graph[i+1][0].flow = 1;
    }
    for(i = 0; i < m; i++){
        graph[size-1][i+n+1].edgeLen = 0;
        graph[size-1][i+n+1].capacity = 1;
        graph[size-1][i+n+1].flow = 1;
        graph[i+n+1][size-1].edgeLen = 0;
        graph[i+n+1][size-1].capacity = 1;
    }
    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            double d = sqrt((source[i].x - target[j].x)*(source[i].x - target[j].x) + (source[i].y - target[j].y)*(source[i].y - target[j].y));
            if( d > limit ) continue;
            graph[i+1][j+n+1].capacity = 1;
            graph[j+n+1][i+1].capacity = 1;
            graph[j+n+1][i+1].flow = 1;
        }
    }
    #ifdef DBUGM
        ShowGraph(size, graph);
    #endif
    return 1;
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
        printf("Relax key and parent %d\n", deQ);
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

