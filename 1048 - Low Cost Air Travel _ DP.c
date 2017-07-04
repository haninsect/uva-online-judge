/*1048 - Low Cost Air Travel _ DP*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define MaxSize 200
#define LargeINT 1000000000
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k, l, m, n;
    int caseNumber = 1;
    while(1){

        int n;
        scanf("%d", &n);
        if(n == 0)break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int prize[n];
        int size[n];
        int airplane[n][10];
        int relabeled[MaxSize];
        for(i = 0; i < MaxSize; i++)relabeled[i] = -1;
        int cityNumber = 0;
        for(i = 0; i < n; i++){
            scanf("%d", &prize[i]);
            scanf("%d", &size[i]);
            for(j = 0; j < size[i]; j++){
                int input;
                scanf("%d", &input);
                int isFind = 0;
                for(k = 0; k < cityNumber; k++)
                    if(input == relabeled[k]) {
                        airplane[i][j] = k;
                        isFind = 1;
                        break;
                    }
                if(isFind == 0){
                    relabeled[cityNumber] = input;
                    airplane[i][j] = cityNumber;
                    cityNumber++;
                }

            }
        }
        #ifdef DBUGM
            printf("cityNumber: %d\n", cityNumber);
            for(i = 0 ; i <cityNumber; i++) printf("%d ", i);
            printf("\n");
            for(i = 0 ; i <cityNumber; i++) printf("%d ", relabeled[i]);
            printf("\n");
            for(i = 0; i < n; i++){
            for(j = 0; j < size[i]; j++) printf("%d ", airplane[i][j]);
            printf("\n");
            }
            printf("\n");
        #endif

        /*----------Should relabel here----------*/
        int query;
        scanf("%d", &query);
        for(i = 0; i < query; i++){
            int qSize;
            scanf("%d", &qSize);
            int qArray[qSize];
            for(j = 0; j < qSize; j++){
                int input;
                scanf("%d", &input);
                int isFind = 0;
                for(k = 0; k < cityNumber; k++)
                    if(input == relabeled[k]) {
                        qArray[j] = k;
                        isFind = 1;
                        break;
                    }
                if(isFind == 0)printf("Not Found\n");
            }
            #ifdef DBUGM
                 for(j = 0; j < qSize; j++) printf("%d ", qArray[j]);
                 printf("\n");

            #endif
            /*Initialize DP table*/
            int table[MaxSize][qSize+1];
            int ticketsUsed[MaxSize][qSize+1];
            int parent[MaxSize][qSize+1];
            for(j = 0; j < MaxSize; j++)
                for(k = 0; k < qSize+1; k++) table[j][k] = LargeINT, ticketsUsed[j][k] = -1, parent[j][k] = -1;
            table[qArray[0]][1] = 0;
            /*Start DP Q:1~qSize*/
            for(j = 1; j < qSize; j++){
                int valid[MaxSize];
                for(k = 0; k < MaxSize; k++) valid[k] = 1;
                while(1){
                    /*If there is no city available, do next q*/
                    int min = -1, minValue = LargeINT;
                    for(k = 0; k < MaxSize; k++){ /*Extract-Min*/
                        if(valid[k] == 0) continue;
                        if(table[k][j] < minValue) min = k, minValue = table[k][j];
                    }
                    if(min == -1) break;
                    valid[min] = 0;
                    /*Relax, for airplane start from min, relax all city going through*/
                    for(k = 0; k < n; k++){
                        if(airplane[k][0] != min) continue;
                        int tmpQ = j;
                        for(l = 1; l < size[k]; l++){
                            if(airplane[k][l] == qArray[tmpQ])tmpQ++;

                            if(table[airplane[k][l]][tmpQ] > minValue + prize[k]) {
                                table[airplane[k][l]][tmpQ] = minValue + prize[k];
                                ticketsUsed[airplane[k][l]][tmpQ] = k;
                                parent[airplane[k][l]][tmpQ] = min + j*MaxSize;
                            }

                            if(tmpQ == qSize) break;
                        }
                    }
                }
            }
            #ifdef DBUGM
                for(j = 0; j < 10; j++){
                    for(k = 1; k < qSize+1; k++ ){
                        printf("%10d ", table[j][k]);
                    }printf("\n");
                }printf("\n");
            #endif
            printf("Case %d, Trip %d: Cost = %d\n", caseNumber, i+1, table[qArray[qSize-1]][qSize]);
            /*BackTrace for tickets used*/
            printf("  Tickets used:");
            PrintParent(qArray[qSize-1], qSize, qSize, ticketsUsed, parent);
            printf("\n");

        }/*Done of query*/
        caseNumber++;
    }
    return 0;
}
void PrintParent(int r, int c, int qSize, int ticketsUsed[MaxSize][qSize+1], int parent[MaxSize][qSize+1])
{
    if(parent[r][c] == -1) return;
    PrintParent(parent[r][c]%MaxSize, parent[r][c]/MaxSize, qSize, ticketsUsed, parent);
    printf(" %d", ticketsUsed[r][c]+1);
}
/*
Case 1, Trip 1: Cost = 225
Tickets used: 1
Case 2, Trip 1: Cost = 100
Tickets used: 2
Case 2, Trip 2: Cost = 300
Tickets used: 3 1
*/
