/*1054 - Pilgrimage _ Simulate*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.005
#define QSize 2000*50
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){

        int n;
        scanf("%d", &n);
        if(n == 0)break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int isInfinite = 1;
        int isValid = 0;
        int lowerB = 1;
        int pCount = 0;
        int pay = 0;
        int queue[QSize] = {0};
        for(i = 0; i < n; i++){
            char op[10];
            int m;
            scanf("%s %d", &op, &m);
            /*If not encounter first in or out*/
            if(isValid == 0) {
                if(strcmp(op, "IN") == 0 || strcmp(op, "OUT") == 0) {
                    isValid = 1;
                    if(strcmp(op, "OUT") == 0){
                        pCount = -m;
                        lowerB = -pCount + 1;
                    }
                    else {
                        pCount = m;
                    }
                    pay = 0;
                }
            }
            else {
                /*In, out : if pay is not 0, if is first time, make the factor 1, else  check the 1 is the factor of pay*/
                if(strcmp(op, "IN") == 0){
                    if(pay == 0){
                        pCount += m;
                        continue;
                    }
                    else {
                        if(isInfinite == 1){
                            for(j = 1; j <= pay; j++){
                                if(pay%(j) == 0){
                                    if(j - pCount >= 1 && j - pCount >= lowerB){
                                        queue[j - pCount] = 1;
                                    }
                                }
                            }
                            isInfinite = 0;
                        }
                        else {
                            for(j = 1; j < QSize; j++){
                                if(j + pCount >= 1 && j >= lowerB){
                                    if( queue[j] == 1 && pay%(j + pCount) == 0) {
                                      continue;
                                    }
                                }
                                queue[j] = 0;
                            }
                        }
                    }
                    pay = 0;
                    pCount += m;
                }
                else if(strcmp(op, "OUT") == 0){
                    if(pay == 0){
                        pCount -= m;
                        lowerB = -pCount + 1;
                        continue;
                    }
                    else {
                        if(isInfinite == 1){
                            for(j = 1; j <= pay; j++){
                                if(pay%(j) == 0){
                                    if(j - pCount >= 1 && j - pCount >= lowerB){
                                        queue[j - pCount] = 1;
                                    }
                                }
                            }
                            isInfinite = 0;
                        }
                        else {
                            for(j = 1; j < QSize; j++){
                                if(j + pCount >= 1 && j >= lowerB){
                                    if( queue[j] == 1 && pay%(j + pCount) == 0) {
                                      continue;
                                    }
                                }
                                queue[j] = 0;
                            }
                        }
                    }
                    pay = 0;
                    pCount -= m;
                    lowerB = -pCount + 1;
                }
                else if(strcmp(op, "PAY") == 0){
                    pay += m;
                }
            }
        }
        if(lowerB < 1) lowerB = 1;
        if(isInfinite) printf("SIZE >= %d\n", lowerB);
        else {
            int flag = 0;
            for(i = lowerB; i < QSize; i++){
                if(queue[i] == 0) continue;
                if(flag == 0) printf("%d", i), flag = 1;
                else  printf(" %d", i);
            }
            if(flag == 1)printf("\n");
            else printf("IMPOSSIBLE\n");
        }


        caseNumber++;
    }
    return 0;
}
