/*1045 - The Great Wall Game_MatchAndFlow*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DBUGMs
#define DBUGM2s
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
		//freopen("output.out", "w", stdout);
	#endif
	#ifdef DBUGM
       printf("X is y-axis\n", caseNumber);
    #endif
    int i, j, k, l;
    int caseNumber = 1;
    while(1){
        int n;
        scanf("%d", &n);
        if(n == 0)break;

        #ifdef DBUGM
           printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        point source[n];
        int sortedX[n], sortedY[n];

        for(i = 0; i < n; i++){
            scanf("%d %d", &source[i].y, &source[i].x);
            sortedX[i] = --source[i].x;
            sortedY[i] = --source[i].y;
        }

        #ifdef DBUGM
            ShowBoard(n, source);
        #endif
        /*Find four kind of match( vertical Median and horizontal Median and two slash), chose the optimal solution by minimum cost maximum flow*/
        qsort(sortedX, n, sizeof(int), cmp);
        qsort(sortedY, n, sizeof(int), cmp);
        int yM = FindMedian(n, sortedY);
        int xM = FindMedian(n, sortedX);
            /*vertical Median*/
        int ans[4] = {0};
        int size = 2*n+2;
        edge graph[size][size];
        point target[n];
        for(i = 0; i < n; i++){
            target[i].x = xM ;
            target[i].y =  i;
        }
        ConstructGraph(size, graph, n, source, target);
        ans[0] = MinCostMaxFlow(size, graph);
            /*horizontal Median*/
        for(i = 0; i < n; i++){
            target[i].x = i ;
            target[i].y = yM ;
        }
        ConstructGraph(size, graph, n, source, target);
        ans[1] = MinCostMaxFlow(size, graph);
            /*Left slash*/
        for(i = 0; i < n; i++){
            target[i].x = i ;
            target[i].y = i ;
        }
        ConstructGraph(size, graph, n, source, target);
        ans[2] = MinCostMaxFlow(size, graph);
            /*Right slash*/
        for(i = 0; i < n; i++){
            target[i].x = n-i-1 ;
            target[i].y = i ;
        }
        ConstructGraph(size, graph, n, source, target);
        ans[3] = MinCostMaxFlow(size, graph);
        #ifdef DBUGM2
        printf("Medians: (%d, %d)\n", xM, yM);
        printf("Ans:\n");
        for(i = 0; i < 4; i++){
            printf("%d ", ans[i]);
        }
        printf("\n");
        #endif
        /*Output the best one*/
        int bestAns = LargeINT;
        for(i = 0; i < 4; i++){
            if(bestAns > ans[i]) bestAns = ans[i];
        }
        printf("Board %d: %d moves required.\n\n", caseNumber, bestAns);
        caseNumber++;
    }
    return 0;
}
int cmp(const void* a, const void *b)
{
    return  *(int*)a - *(int*)b;
}

int ConstructGraph(int size, edge graph[size][size], int n, point source[n], point target[n])
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
    for(i = 0; i < n; i++){
        graph[0][i+1].edgeLen = 0;
        graph[0][i+1].capacity = 1;
        /*graph[i+1][0].edgeLen = 0;
        graph[i+1][0].capacity = 1;
        graph[i+1][0].flow = 1;*/
        /*graph[size-1][i+n+1].edgeLen = 0;
        graph[size-1][i+n+1].capacity = 1;
        graph[size-1][i+n+1].flow = 1;*/
        graph[i+n+1][size-1].edgeLen = 0;
        graph[i+n+1][size-1].capacity = 1;
    }

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            graph[i+1][j+n+1].edgeLen = abs(source[i].x - target[j].x) + abs(source[i].y - target[j].y);
            graph[i+1][j+n+1].capacity = 1;
            graph[j+n+1][i+1].edgeLen = -(abs(source[i].x - target[j].x) + abs(source[i].y - target[j].y));
            graph[j+n+1][i+1].capacity = 1;
            graph[j+n+1][i+1].flow = 1;
        }
    }
    #ifdef DBUGM
        ShowGraph(size, graph);
    #endif
    return 1;
}
int MinCostMaxFlow(int size, edge graph[size][size])
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
    int cost = 0;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++) {
            if(graph[i][j].flow > 0 && graph[i][j].edgeLen >= 0) {
                cost += graph[i][j].edgeLen*graph[i][j].flow;
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

int FindMedian(int n, int array[n])
{
    return array[n/2];
}

void ShowBoard(int n, point source[n])
{
    int i, j, k;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
                int flag = 0;
            for(k = 0; k < n; k++){
                if(source[k].x == i && source[k].y == j){
                    printf("|*");
                    flag =1;
                    break;
                }
            }
            if(flag == 0)printf("| ");
            if(j == n-1)printf("|\n");
        }

    }
    printf("\n");
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
