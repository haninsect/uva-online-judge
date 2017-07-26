/*1073 - Glenbow Museum _ DP */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxSize 510
int size;
long long int dp[MaxSize][MaxSize][2][2];

int main()
{
    #ifndef ONLINE_JUDGE
		//freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    for(i = 0; i < MaxSize; i++){
        for(j = 0; j < MaxSize; j++){
            if(i == 0 && j == 0){
                dp[i][j][0][1] = 0;
                dp[i][j][1][0] = 0;
                dp[i][j][1][1] = 0;
            }
            else if(i == 0 && j != 0) {
                dp[i][j][0][1] = 0;
                dp[i][j][1][0] = 0;
                dp[i][j][1][1] = 1;
            }
            else if(i != 0 && j == 0) {
                dp[i][j][0][1] = 0;
                dp[i][j][1][0] = 0;
                dp[i][j][1][1] = 0;
            }
            else {
                dp[i][j][0][1] = dp[i-1][j][1][1];
                dp[i][j][1][0] = dp[i-1][j][1][1];
                dp[i][j][1][1] = dp[i][j-1][1][1] + dp[i][j-1][0][1];
            }
        }
    }
    while(1){
        scanf("%d", &size);
        if(size == 0) break;
        if(size < 4 || size%2 == 1) {
            printf("Case %d: 0\n", caseNumber++);
            continue;
        }
        else if( size == 4) {
            printf("Case %d: 1\n", caseNumber++);
            continue;
        }
        else if( size == 6) {
            printf("Case %d: 6\n", caseNumber++);
            continue;
        }
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        printf("Case %d: %lld\n", caseNumber, dp[(size-4)/2][(size-4)/2+4][0][1] + dp[(size-4)/2][(size-4)/2+4][1][0] + dp[(size-4)/2][(size-4)/2+4][1][1]);
        caseNumber++;
    }
    return 0;
}
