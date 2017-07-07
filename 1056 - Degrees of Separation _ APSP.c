/*1056 - Degrees of Separation _ Diameter*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.005
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){

        int n, e;
        scanf("%d%d", &n, &e);
        if(n == 0)break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int adjMatrix[n][n];
        for(i = 0; i < n; i++)
            for(j = 0; j < n; j++){
                adjMatrix[i][j] = LargeINT;
                adjMatrix[i][i] = 0;
            }

        /*Relabel input name*/
        char name[50][10000] = {'\0'};
        int np = 0;
        for(i = 0; i < e; i++){
            char a[10000], b[10000];
            scanf("%s%s", &a, &b);
            int ra = -1, rb = -1;
            for(j = 0; j < np; j++){
                if(strcmp(a, name[j]) == 0) ra = j;
                if(strcmp(b, name[j]) == 0) rb = j;
            }
            if(ra == -1){
                ra = np;
                strcpy(name[np], a);
                np++;
            }
            if(rb == -1){
                rb = np;
                strcpy(name[np], b);
                np++;
            }
            adjMatrix[ra][rb] = 1;
            adjMatrix[rb][ra] = 1;
        }
        /*All pair shortest path*/
        for(k = 0; k < n; k++){
            for(i = 0; i < n; i++){
                for(j = 0; j < n; j++){
                    if(adjMatrix[i][k] + adjMatrix[k][j] < adjMatrix[i][j])
                        adjMatrix[i][j] = adjMatrix[i][k] + adjMatrix[k][j];
                }
            }
        }
        /*Find max*/
        int radius = 0;
        for(i = 0; i < n; i++){
            for(j = 0; j < n; j++){
                /*printf("%10d ", adjMatrix[i][j]);*/
                if(radius < adjMatrix[i][j])
                    radius = adjMatrix[i][j];
            }
        }

        printf("Network %d: ", caseNumber);
        if(radius == LargeINT) printf("DISCONNECTED\n");
        else printf("%d\n", radius);
        printf("\n");
        caseNumber++;
    }
    return 0;
}
