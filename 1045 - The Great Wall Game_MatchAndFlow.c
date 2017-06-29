/*1045 - The Great Wall Game_MatchAndFlow*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DBUGM
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
        #ifndef DBUGM
           printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int n;
        scanf("%d", &n);
        if(n == 0)break;

        point source[n];
        int sortedX[n], sortedY[n];

        for(i = 0; i < n; i++){
            scanf("%d %d", &source[i].x, &source[i].y);
            sortedX[i] = --source[i].x;
            sortedY[i] = --source[i].y;
        }
        ShowBoard(n, source);
        int ans[4] = {0};
        /*Find vertical and horizontal Median , greedy chose the optimal solution*/
        qsort(sortedX, n, sizeof(int), cmp);
        qsort(sortedY, n, sizeof(int), cmp);
        int xM = FindMedian(n, sortedY);
        int yM = FindMedian(n, sortedX);
        ans[0] = FindVerticalAns(n, source, xM);
        ans[1] = FindHorizontalAns(n, source, yM);

        /*For tow Slash, chose the optimal solution by minimum cost maximum flow*/
        /*Left slash*/
            /*Construct graph*/
        int size = 2*n+2;
        edge graph[size][size];
        point target[n];
        for(i = 0; i < n; i++){
            target[i].x = i ;
            target[i].y = i ;
        }
        ConstructGraph(size, graph, n, source, target);
            /*Compute max flow*/
        ans[2] = MinCostMaxFlow(size, graph);
        /*Right slash*/
            /*Construct graph*/
        for(i = 0; i < n; i++){
            target[i].x = n-i-1 ;
            target[i].y = i ;
        }
        ConstructGraph(size, graph, n, source, target);
            /*Compute max flow*/
        ans[3] = MinCostMaxFlow(size, graph);
        /*Compare four solution, chose the best one*/
        #ifdef DBUGM
        printf("X median: %d, \nY median: %d\n", xM, yM);
        printf("Ans:\n");
        for(i = 0; i < 4; i++){
            printf("%d ", ans[i]);
        }
        printf("\n");

        #endif
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
            graph[i][j].edgeLen = -1;
            graph[i][j].flow = 0;
            graph[i][j].capacity = 0;
        }
    }
    for(i = 0; i < n; i++){
        graph[0][i+1].edgeLen = 0;
        graph[0][i+1].capacity = 1;
        graph[i+1][0].edgeLen = 0;
        graph[i+1][0].capacity = 1;
        graph[size-1][i+n+1].edgeLen = 0;
        graph[size-1][i+n+1].capacity = 1;
        graph[i+n+1][size-1].edgeLen = 0;
        graph[i+n+1][size-1].capacity = 1;
    }

    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            graph[i+1][j+n+1].edgeLen = abs(source[i].x - target[j].x) + abs(source[i].y - target[j].y);
            graph[i+1][j+n+1].capacity = 1;
            graph[j+n+1][i+1].edgeLen = abs(source[i].x - target[j].x) + abs(source[i].y - target[j].y);
            graph[j+n+1][i+1].capacity = 1;
        }
    }
    #ifdef DBUGM
    printf("Capacity:\n");
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            printf("%2d ",graph[i][j].capacity);
        }printf("\n");
    }printf("\n");
    printf("edgeLen:\n");
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            printf("%2d ",graph[i][j].edgeLen);
        }printf("\n");
    }printf("\n");
    #endif
    return 1;
}
int MinCostMaxFlow(int size, edge graph[size][size])
{
    int i, j, k;
    int parent[size];
    int p;
    while( FindPath(size, graph, parent, 0, size-1) ){
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
    }
    int cost = 0;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++) {
            if(graph[i][j].flow > 0) {
                if(graph[i][j].edgeLen !=0 )printf("Chose (%d, %d) w: %d\n", i, j, graph[i][j].edgeLen);
                cost += graph[i][j].edgeLen*graph[i][j].flow;
            }
        }
    }
    printf("\tCost %d\n", cost);
    #ifdef DBUGM
    printf("flow:\n");
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            printf("%2d ",graph[i][j].flow);
        }printf("\n");
    }printf("\n");

    #endif
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
    while(vn < size){
        int deQ = -1;
        for(i = 0; i < size; i++){
            if(visited[i] == 0) {
                deQ = i;
                break;
            }
        }
        visited[deQ] = 1;
        vn++;
        for(i = 0; i < size; i++){
            if(graph[deQ][i].edgeLen != -1 && graph[deQ][i].capacity > graph[deQ][i].flow && graph[deQ][i].edgeLen + key[deQ] < key[i]){
                key[i] = graph[deQ][i].edgeLen + key[deQ];
                parent[i] =deQ;
                visited[i] = 0;
            }
        }
    }
    #ifdef DBUGM
    printf("path: %d -> %d\n", root, target);
    for(i = 0; i < size; i++){
        printf("%2d ", parent[i]);
    }
    printf("\n\n");
    #endif
    return parent[target] != -1;

}

int FindMedian(int n, int array[n])
{
    return array[n/2];
}
int FindVerticalAns(int n, point source[n], int xM)
{
    int i, j, k;
    int ans = 0;
    int visit[n], valid[n];
    int slot[n];
    for(i = 0; i < n; i++) visit[i] = 0, valid[i] = 1, slot[i] = 0;
    for(i = 0; i < n; i++) slot[source[i].x]++;
    for(i = 0; i < n; i++) {
        if(slot[i]== 0) continue;
        for(j = 0; j < n; j++){
            if(source[j].x == i){
                visit[j] = 1;
                valid[i] = 0;
                ans += abs(source[j].y - xM);
                break;
            }
        }
    }
    for(i = 0; i < n; i++){
        if(valid[i] == 0)continue;
        int minL = LargeINT, minS = LargeINT;
        for(k = 0; k < n; k++){
            if(visit[k] == 1) continue;
            if(abs(source[k].x - i) + abs(source[k].y - xM) < minL){
                minL = abs(source[k].x - i) + abs(source[k].y - xM);
                minS = k;
            }
        }
        ans += minL;
        visit[minS] = 1;
        valid[i] = 0;
    }
    return ans;
}
int FindHorizontalAns(int n, point source[n], int yM)
{
    int i, j, k;
    int ans = 0;
    int visit[n], valid[n];
    int slot[n];
    for(i = 0; i < n; i++) visit[i] = 0, valid[i] = 1, slot[i] = 0;
    for(i = 0; i < n; i++) slot[source[i].y]++;
    for(i = 0; i < n; i++) {
        if(slot[i]== 0) continue;
        for(j = 0; j < n; j++){
            if(source[j].y == i){
                visit[j] = 1;
                valid[i] = 0;
                ans += abs(source[j].x - yM);
                break;
            }
        }
    }
    for(i = 0; i < n; i++){
        if(valid[i] == 0)continue;
        int minL = LargeINT, minS = LargeINT;
        for(k = 0; k < n; k++){
            if(visit[k] == 1) continue;
            if(abs(source[k].x - yM) + abs(source[k].y - i) < minL){
                minL = abs(source[k].x - yM) + abs(source[k].y - i);
                minS = k;
            }
        }
        ans += minL;
        visit[minS] = 1;
        valid[i] = 0;
    }
    return ans;
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
