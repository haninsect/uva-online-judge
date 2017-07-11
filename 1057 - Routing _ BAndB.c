/*1057 - Routing _ BAndB*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define MaxSize 100
#define errorT 0.00005



int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){

        int node, edge;
        scanf("%d%d", &node, &edge);
        if(node == 0)break;
        #ifdef DBUGM

        #endif
        int adjMatrix[node][node];
        for(i = 0; i < node; i++)
            for(j = 0; j < node; j++)
                adjMatrix[i][j] = 0;
        for(i = 0; i < edge; i++){
            int a, b;
            scanf("%d%d", &a, &b);
            a--;
            b--;
            adjMatrix[a][b] = 1;
        }
        /*Greedy find a not optimal solution to cut*/
        int used[node], greedyAns = 1;
        for(i = 0; i < node; i++) used[i] = 0;
        greedyAns += Dijkstra(0, 1, node, used, adjMatrix);
        greedyAns += Dijkstra(1, 0, node, used, adjMatrix);

        if(greedyAns >= LargeINT){
            printf("Network %d\n", caseNumber);
            printf("Impossible\n\n");
            caseNumber++;
            continue;
        }
        /*DFS find all path 1->2. Then Dijkstra calculate the best path 2->1 */
        printf("Network %d\n", caseNumber);
        for(i = 0; i < node; i++) used[i] = 0;
        used[0] = 1;
        printf("Minimum number of nodes = %d\n\n", DFS(0, 1, greedyAns, node, used, adjMatrix));
        caseNumber++;
    }
    return 0;
}
int DFS(int s, int visit, int min, int n, int used[n], int adjMatrix[n][n])
{
    if(visit >= min) return min;
    if(s == 1){ /*The target is found. Calculate back path by Dijkstra*/
        int back = Dijkstra(1, 0, n, used, adjMatrix);
        if(back + visit < min) return back + visit;
        else return min;
    }
    int i;
    for(i = 0; i < n; i++){ /*DFS*/
        if(adjMatrix[s][i] == 1 && used[i] == 0){
            used[i] = 1;
            int tmp = DFS(i, visit+1, min, n, used, adjMatrix);
            if(tmp < min) min = tmp;
            used[i] = 0;; /*The node can be visit by the other path*/
        }
    }
    return min;
}
int Dijkstra(int from, int to, int n, int used[n], int adjMatrix[n][n])
{
    int i, j;
    int valid[n], parent[n], key[n];
    for(i = 0; i < n; i++) valid[i] = 1, parent[i] = -1, key[i] = LargeINT;
    key[from] = 0;
    while(1){
        int deQ = -1, min = LargeINT;
        for(i = 0; i < n; i++){
            if(key[i] < min && valid[i] == 1){
                min = key[i];
                deQ = i;
            }
        }
        if(deQ == -1) break;
        valid[deQ] = 0;
        for(i = 0; i < n; i++){
            if(adjMatrix[deQ][i] != 0 && valid[i] == 1){
                int weight;
                if(used[i] == 1) weight = 0; /*If the node has been used, the edge length is 0*/
                else weight = adjMatrix[deQ][i];
                if(key[deQ] + weight < key[i]) {
                    key[i] = key[deQ] + weight;
                    parent[i] = deQ;
                }
            }
        }
    }
    /*Set the node on path been used*/
    int p = to;
    if(valid[to] == 0){
        while(p != -1){
            used[p] = 1;
            p = parent[p];
        }
    }
    return key[to];
}
