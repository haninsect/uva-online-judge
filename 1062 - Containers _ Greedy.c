/*1062 - Containers _ Greedy*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define MaxSize 350
#define errorT 0.00005
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){
        char input[2000] = {'\0'};
        scanf("%s", &input);
        if(strcmp(input, "end") == 0)break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int used[2000] = {0};
        int ans = 0;
        for(i = 0; i < strlen(input); i++){
            if(used[i] == 1) continue;
            char tmp = input[i];
            for(j = i+1; j < strlen(input); j++){
                if(used[j] == 1) continue;
                if(tmp >= input[j]){
                    used[j] = 1;
                    tmp = input[j];
                }
            }
            ans++;
        }
        printf("Case %d: %d\n", caseNumber, ans);
        caseNumber++;
    }
    return 0;
}
