/*1040 - The Traveling Judges Problem _ MST*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int cmp(const void *a, const void *b);
int MST(int cityNumber, int adjMatrix[cityNumber][cityNumber], int validNode, int parent[cityNumber], int root);
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k, l;
    int caseNumber = 1;
    while(1){
        int cityNumber, destination, roadNumber, judgeNumber;
        scanf("%d", &cityNumber);
        if(cityNumber == -1)break;
        scanf("%d", &destination);
        destination--;
        scanf("%d", &roadNumber);
        int adjmatrix[cityNumber][cityNumber];

        for(i = 0; i < cityNumber; i++)
            for(j = 0; j < cityNumber; j++) adjmatrix[i][j] = 0;

        for(i = 0; i < roadNumber; i++){
            int a, b, d;
            scanf("%d%d%d", &a, &b, &d);
            a--;
            b--;
            adjmatrix[a][b] = d;
            adjmatrix[b][a] = d;

        }        
        scanf("%d", &judgeNumber);
        int judges[judgeNumber];
        for(i = 0; i < judgeNumber; i++){
            int ju;
            scanf("%d", &ju);
            judges[i] = ju-1;
        }
        /*For all possible choose(2^uNumber) of those unnecessary node, find MST*/
        int uNumber = cityNumber - judgeNumber - 1;
            /*Construct mask. ex: 4 uNumber -  0001 0010 0100 1000*/
        int mask[uNumber];
        for(i = 0; i < uNumber; i++){
            mask[i] = (int)pow(2, i);
        }
            /*Find all unnecessary node*/
        int unnecessaryNode[uNumber];
        int uNTable[cityNumber];
        for(i = 0; i < cityNumber; i++)uNTable[i] = 0;
        uNTable[destination] = 1;
        for(i = 0; i < judgeNumber; i++){
            uNTable[judges[i]] = 1;
        }
        for(i = 0, j = 0; i < cityNumber; i++){
            if(uNTable[i] == 1) continue;
            unnecessaryNode[j] = i;
            j++;
        }        
            /*MST*/
        int bestD = 1000000000;
        int bestParent[cityNumber];
        for(i = 0; i < (int)pow(2, uNumber); i++){
            /*Reconstruct new graph*/
            int validNode = 0;
            int tmpTable[cityNumber];
            for(j = 0; j < cityNumber; j++)tmpTable[j] = uNTable[j];
            for(j = 0; j < uNumber; j++){
                if(i&mask[j] > 0) tmpTable[unnecessaryNode[j]] = 1;
            }
            for(j = 0; j < cityNumber; j++)if(tmpTable[j] == 1) validNode++;

            int tmpAdjMatric[cityNumber][cityNumber];
            for(j = 0; j < cityNumber; j++){
                for(k = 0; k < cityNumber; k++){
                    if(adjmatrix[j][k] > 0 && tmpTable[j] == 1 && tmpTable[k] == 1){
                        tmpAdjMatric[j][k] = adjmatrix[j][k];
                    }
                    else tmpAdjMatric[j][k] = 0;

                }
            }
            /*Start MST with new graph. If find better ans, update weight and parent*/
            int tmpD;
            int tmpParent[cityNumber];
            tmpD = MST( cityNumber, tmpAdjMatric, validNode, tmpParent, destination);
            if(tmpD < bestD) {
                bestD = tmpD;
                for(j = 0; j < cityNumber; j++) bestParent[j] = tmpParent[j];                
            }
        }

        /*Output*/
        if(bestD == 1000000000) {
            printf("Error distance not find\n");
            exit(1);
        }
        printf("Case %d: distance = %d\n", caseNumber, bestD);
        for(i = 0; i < judgeNumber; i++){
            int p = judges[i];
            printf("   %d", p+1);
            while(p != destination){
                printf("-%d", bestParent[p]+1);
                p = bestParent[p];
            }
            printf("\n");
        }
        printf("\n");
        caseNumber++;
    }
    return 0;
}

int MST(int cityNumber, int adjMatrix[cityNumber][cityNumber], int validNode, int parent[cityNumber], int root)
{
    int i, j, k, l;
    int vn = 0, tWeight = 0;
    int visited[cityNumber];
    int key[cityNumber];
    for(i = 0; i < cityNumber; i++) key[i] = 1000000000, parent[i] = -1, visited[i] = 0;

    key[root] = 0;
    while(vn < validNode){
        int minN = -1, minW = 1000000000;
        for(i = 0; i < cityNumber; i++) {
            if( key[i] < minW && visited[i] == 0){
                minW = key[i];
                minN = i;
            }
        }
        if(minN == -1) {
            /*Graph is not connected.*/
            return 1000000000;
        }
        tWeight += minW;
        visited[minN] = 1;
        vn++;
        for(i = 0; i < cityNumber; i++){
            if(adjMatrix[minN][i] > 0 && visited[i] == 0 && adjMatrix[minN][i] < key[i]){
                key[i] = adjMatrix[minN][i];
                parent[i] = minN;
            }
        }
    }
    return tWeight;
}
