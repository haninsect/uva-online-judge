/*1051 - Bipartite Numbers _ Math*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.005
#define MaxSize 99999
int table[MaxSize][10];
int CompareAnswer(int l1, int d1, int l2, int d2, int *m, int *s, int *n, int *t, int number);
int powInt(int a, int p);
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){

        int number;
        scanf("%d", &number);
        if(number == 0)break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int m = LargeINT, s = LargeINT, n = LargeINT, t = LargeINT;
        /*  aaabbbb = aaaaaaa +/- cccc   */
        for(i = 0; i < MaxSize; i++)
            for(j = 0; j < 10; j++) table[i][j] = -1;

        int remainder[10] = {0};
        int Found = 0;
        for(i = 1; i <= 1000000 && Found == 0; i++){
            for(j = 1; j <= 9; j++){
                /* Calculate remainder */
                int rm = (remainder[j] * 10 + j)%number;
                remainder[j] = rm;
                /* Check Bipartite Number*/
                /*Minus*/
                for(k = 1; k < 10; k++){
                    if(table[rm][k] == -1) continue;
                    if(j - k >= 0){
                        Found |= CompareAnswer(i, j, table[rm][k], j-k, &m, &s, &n, &t, number);
                    }
                }
                /*Add*/
                for(k = 1; k < 10; k++){
                    if(table[(number - rm)%number][k] == -1) continue;
                    if(j + k <= 9 ){
                         Found |= CompareAnswer(i, j, table[(number - rm)%number][k], j+k, &m, &s, &n, &t, number);
                    }
                }
                if(Found == 0){
                    table[rm][j] = i;
                }

            }
        }
        if(Found == 0) printf("Not Found\n");
        else printf("%d: %d %d %d %d\n", number, m, s, n, t);
    }
    return 0;
}
int powInt(int a, int p)
{
    int i;
    int r = 1;
    for(i = 0; i < p; i++){
        r *= a;
    }
    return r;
}
int CompareAnswer(int l1, int d1, int l2, int d2, int *m, int *s, int *n, int *t, int number)
{
    int ten = 1;
    int tmpN = 0;
    int i;
    if(l1 <= l2) return 0;
    if(l1 < 6){
        if( powInt(10, l1-1) <= number) {
            for(i = 0; i < l1; i++){
                if(i < l2) tmpN += ten*d2;
                else tmpN += ten*d1;
                ten *= 10;
            }
            #ifdef DBUGM
                printf("%d %d %d %d\n", l1, d1, l2, d2);
                printf("!%d\n", tmpN);
            #endif
            if(tmpN <= number || tmpN < 10) return 0;
        }

    }


    if(l1 > *m + *n) return 0;
    int replace = 0;
    if(l1 < *m + *n) replace = 1;
    else {
        if(d1 < *s) replace = 1;
        else if(d1 > *s) return 0;
        else {
            if(l2 == *n){
                if(d2 < *t) replace = 1;
                else return 0;
            }
            else if(l2 > *n){
                if(d2 < d1) replace = 1;
                else return 0;
            }
            else {
                if(d1 < *t) replace = 1;
                else return 0;
            }
        }
    }
    if(replace){
        *m = l1 - l2;
        *s = d1;
        *n = l2;
        *t = d2;
        #ifdef DBUGM
            printf("Now Ans : %d %d %d %d\n", *m, *s, *n, *t);
        #endif
        return 1;
    }
    return 0;
}

void ShowTable(int number, int table[number][10])
{
    int i, j;
    printf("%6d:", -1);
    for(j = 1; j < 10; j++){
        printf(" %2d", j);
    }printf("\n");
    for(i = 0; i < number; i++){
        printf("%6d:", i);
        for(j = 1; j < 10; j++){
            printf(" %2d", table[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
