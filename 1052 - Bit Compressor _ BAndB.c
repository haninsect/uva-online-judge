/*1052 - Bit Compressor _ BAndB*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.005
#define ArraySize 50
int totalLen, numberOf1;
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){
        scanf("%d", &totalLen);
        if(totalLen == 0)break;
        totalLen++;
        scanf("%d", &numberOf1);
        #ifdef DBUGM
            printf("----------- Debug Message %d: %d %d-----------\n", caseNumber, totalLen, numberOf1);
        #endif
        int ans = 0;

        char input[ArraySize];
        for(i = 0; i < ArraySize; i++) input[i] = '\0';
        scanf("%s", &input);
        for(i = strlen(input); i > 0; i--) {
            input[i] = input[i-1];
        }
        input[0] = '0';
        #ifdef DBUGM
            for(i = 0; i < strlen(input); i++) printf("%3d", i);
            printf("\n");
            for(i = 0; i < strlen(input); i++) printf("%3c", input[i]);
            printf("\n");
        #endif

        ans = BranchAndBound(input, 0, 0, 0);
        printf("Case #%d: ", caseNumber);
        if(ans == 0) printf("NO\n");
        else if(ans == 1) printf("YES\n");
        else if(ans >= 2) printf("NOT UNIQUE\n");
        else exit(1);
        caseNumber++;
    }
    return 0;
}
int BranchAndBound(char input[ArraySize], int breakPoint, int tmpLen, int tmpOf1)
{
    if(tmpLen > totalLen || tmpOf1 > numberOf1) return 0;
    int len = strlen(input);
    #ifdef DBUGM
        printf("Now %d/%d: Len: %d, Number Of 1: %d\n", breakPoint, len, tmpLen, tmpOf1);
    #endif
    if(breakPoint == len) { /*End of recursively call*/
        if(tmpLen == totalLen && tmpOf1 == numberOf1) return 1;
        else return 0;
    }
    int i, j, k;
    /*Find a new start, must be ...01...)*/
    int nStart = -1;
    for(i = breakPoint+1; i < len; i++){
        /*Consume a bit*/
        tmpLen++;
        if(tmpLen > totalLen) return 0;
        if(input[i-1] == '0' && input[i] == '1') {
            nStart = i;
            break;
        }
    }
    if(nStart == -1) { /*All bits are consumed*/
        /*Handle the last bit*/
        tmpLen++;
        if(input[i-1] == '1')tmpOf1++;

        if(tmpLen == totalLen && tmpOf1 == numberOf1) return 1;
        else return 0;
    }

    /*New Branch*/
    char tmpInput[50] = {'\0'};
    int ans = 0;
    for(i = nStart; i-nStart <= 17 && i < len; i++){
        /*Consume a bit*/
        tmpInput[i-nStart] = input[i];
        int n1 =  strtol(tmpInput, NULL, 2);
        /*Cut*/
        if(n1 > 3 && (tmpLen + n1 > totalLen || tmpOf1 + n1 > numberOf1)) break;
        if(i < len-1)
            if(input[i+1] == '1') continue;
        #ifdef DBUGM
            printf("Find new branch: %d\n", i);
            printf("\t%d\n", n1);
        #endif
        /*1, 2, 3 has special case*/
        if(n1 == 1) {
            if(i < len-2){
                if(input[i+2] == '0') continue;
            }
            else if(i == len-2) continue;
            ans += BranchAndBound(input, i+1, tmpLen + n1, tmpOf1 + n1);
        }
        else if(n1 == 2) {
            ans += BranchAndBound(input, i+1, tmpLen + n1, tmpOf1 + n1-1);
        }
        else if(n1 == 3) {
            ans += BranchAndBound(input, i+1, tmpLen + n1-1, tmpOf1 + n1-1);
            ans += BranchAndBound(input, i+1, tmpLen + n1, tmpOf1 + n1);
        }
        /*General case*/
        else ans += BranchAndBound(input, i+1, tmpLen + n1, tmpOf1 + n1);
    }
    return ans;
}
