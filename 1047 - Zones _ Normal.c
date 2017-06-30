/*1047 - Zones _ Normal*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DBUGM5
#define LargeINT 1000000000
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, l;
    int caseNumber = 1;
    while(1){
        int n, k;
        scanf("%d %d", &n, &k);
        if(n == 0 && k == 0)break;
        if(n*k == 0) {
            printf("Case Number %d\n", caseNumber);
            printf("Number of Customers: %d\n", 0);
            printf("Locations recommended:\n");
            caseNumber++;
            continue;
        }
        #ifdef DBUGM
           printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int size[n];
        for(i = 0; i < n; i++) scanf("%d", &size[i]);
        int m;
        scanf("%d", &m);
        /*Construct table*/
        int table[m][n];
        int overlap[m];

        for(i = 0; i < m; i++){
            for(j = 0; j < n; j++){
                table[i][j] = 0;
            }
        }
        for(i = 0; i < m; i++){
            int a;
            scanf("%d", &a);
            for(j = 0; j < a; j++) {
                int o;
                scanf("%d", &o);
                table[i][--o] = 1;
            }
            scanf("%d", &overlap[i]);
        }
        /*For C(n, k) possible, calculate size*/
        int max = 0;
        int bestQ[k];
        int mask[n];
        for(i = 0; i < n; i++) mask[i] = pow(2, i);

        for(i = 0; i < (int)pow(2, n); i++){
            int queue[n];
            int qp = 0;
            for(j = 0; j < n; j++) {
                if((i&mask[j]) > 0) {
                    queue[qp++] = j;
                }
            }

            if(qp != k) continue;
            int tmp = 0;
            for(j = 0; j < qp; j++){
                tmp += size[queue[j]];
            }
            int counter[m];
            for(j = 0; j < m; j++)counter[j] = 0;
            for(j = 0; j < qp; j++){
                for(l = 0; l < m; l++){
                    if(table[l][queue[j]] == 1) counter[l]++;
                }
            }
            for(j = 0; j < m; j++) {
                if(counter[j] <= 1) continue;
                tmp -= overlap[j]*(counter[j]-1);
            }
            if(tmp > max){
                max = tmp;
                for(j = 0; j < k; j++) {
                    bestQ[j] = queue[j];
                }
            }
            if(tmp == max){
                int update = 0;
                for(j = 0; j < k; j++){
                    if(bestQ[j] == queue[j]) break;
                    if(bestQ[j] > queue[j]) update = 1;
                    break;
                }
                if(update){
                    for(j = 0; j < k; j++) {
                        bestQ[j] = queue[j];
                    }
                }
            }
        }

        printf("Case Number  %d\n", caseNumber);
        printf("Number of Customers: %d\n", max);
        printf("Locations recommended:");
        for(i = 0; i < k; i++){
            printf(" %d", bestQ[i]+1);
        }
        printf("\n\n");
        caseNumber++;
    }
    return 0;
}
