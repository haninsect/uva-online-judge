/*1040 - The Traveling Judges Problem _ MST*/
#include <stdio.h>
#include <stdlib.h>
int cmp(const void *a, const void *b);

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k, l;
    int caseNumber = 1;
    while(1){
        int cityNumber, destination, roadNumber, judgeNumber;
        scanf("%d", &cityNumber);
        if(cityNumber == -1)break;
        scanf("%d", &destination);

        scanf("%d", &roadNumber);
        int adjmatrix[roadNumber][roadNumber];
        for(i = 0; i < roadNumber; i++)
            for(j = 0; j < roadNumber; j++) adjmatrix[i][j] = 0;
        for(i = 0; i < roadNumber; i++){
            int a, b, d;
            scanf("%d%d%d", &a, &b, &d);
            adjmatrix[a][b] = d;
            adjmatrix[b][a] = d;
        }

        scanf("%d", &judgeNumber);
        int judges[judgeNumber];
        for(i = 0; i < judgeNumber; i++){
            scanf("%d", &judges[i]);
        }
        /*For all possible choose of those unnecessary node*/
            /*Find MST*/


    }
    return 0;
}

