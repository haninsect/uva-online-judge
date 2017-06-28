/*1041 - cNteSahruPfefrlefe - BranchAndBound*/
#include <stdio.h>
#include <stdlib.h>
#define Show_Debug
int cmp(const void* a, const void *b);
void BuildCorrect();
int BranchAndBound(int state[52], int times, int modification, int totalS, int diff);
void ShuffleBack(int state[52],int tmp[52]);
void Swap(int state[52], int s, int p);

int Correct[11][52] = {0};
int NumberOfShuffles;
int errorNumber = 100000;
int errorOccur[10][2] = {0};
int tmpError[10][2] = {0};
int errorOrder[10] = {0};

int c = 0;
int main()
{
     #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k, l;
    int caseNumber = 1;
    int totalCase;
    scanf("%d", &totalCase);
    BuildCorrect();
    while(caseNumber <= totalCase){
        int input[52];
        NumberOfShuffles = 0;
        errorNumber = 100000;
        for(i = 0; i < 10; i++) errorOrder[i] = i;
        for(i = 0; i < 10; i++)
            for(j = 0; j < 2; j++) {
                tmpError[i][j] = 0;
                errorOccur[i][j] = 0;
            }

        for(i = 0; i < 52; i++){
            scanf("%d", &input[i]);
        }

        /*Greedy to find the possible shuffle number*/
        int minDiff = 0;
        int greedyNumber = FindMinDiff(input, &minDiff);
        BranchAndBound(input, greedyNumber, 0, greedyNumber, minDiff);

        if(errorNumber > 1)
            qsort(errorOrder, errorNumber, sizeof(int), cmp);
        if(caseNumber > 1)printf("\n");
        printf("Case %d\n", caseNumber);
        printf("Number of shuffles = %d\n", NumberOfShuffles);
        if(errorNumber == 0) printf("No error in any shuffle\n");
        else {
            for(i = 0; i < errorNumber; i++){
                printf("Error in shuffle %d at location %d\n", errorOccur[errorOrder[i]][0], errorOccur[errorOrder[i]][1]);
            }
        }
        caseNumber++;

    }
        printf("\n");
    return 0;
}

int FindMinDiff(int input[52], int* minDiff)
{
    int i, j, k;
    int minD = 100;
    int min = 0;
    for(i = 0; i < 11; i++){
        int diff = 0;
        for(j = 0; j < 52; j++){
            if(input[j] != Correct[i][j])diff++;
        }
        if(diff < minD){
            minD = diff;
            min = i;
        }
    }
    *minDiff = minD;
    return min;
}
int BranchAndBound(int state[52], int times, int modification, int totalS, int diff)
{

    #ifndef Show_Debug
    printf("New Branch: %d %d %d\n", times, modification, totalS);
    printf("%d\n", diff);
    #endif
    int i, j, k, l;
    /*Cut condition: Correct or Impossible or can't be better*/
    if(modification > errorNumber) return 0;
    if(diff == 0){
        if (errorNumber > modification) {
            errorNumber = modification;
            NumberOfShuffles = totalS;
            for(i = 0; i < modification; i++){
                errorOccur[i][0] = tmpError[i][0];
                errorOccur[i][1] = tmpError[i][1];
            }

        }
        return 1;
    }
    if(diff > times*2) {
        #ifndef Show_Debug
        printf("Cut\n");
        #endif
        return 0;
    }
    else {
        int tmp[52];
        ShuffleBack(state, tmp);
        for(i = 0; i < 52; i++){
            #ifndef Show_Debug
            printf("\tNew Swap: %d\n", i);
            #endif
            if(i == 51) {
                BranchAndBound(tmp, times-1, modification, totalS, diff);
            }
            else {
                /*Calculate the two index to swap, and the difference*/
                int s1, s2, c = 0;
                if(i%2 == 0) s1 = 26+i/2, s2 = (i+1)/2;
                else s1 = i/2, s2 = 26+(i+1)/2;
                Swap(tmp, s1, s2);
                tmpError[modification][0] = times;
                tmpError[modification][1] = i;
                if(state[i] == Correct[times][i]) c++;
                else if(state[i+1] == Correct[times][i]) c--;
                if(state[i+1] == Correct[times][i+1]) c++;
                else if(state[i] == Correct[times][i+1]) c--;
                
                BranchAndBound(tmp, times-1, modification+1, totalS, diff+c);
                Swap(tmp, s1, s2);
            }
        }

    }
}
void ShuffleBack(int state[52], int tmp[52])
{
    int i;
    for(i = 0; i < 52; i++){
        if(i%2 == 0) tmp[26+i/2] = state[i];
        else tmp[i/2] = state[i];
    }
}
void Swap(int state[52], int s, int p)
{
    int tmp = state[s];
    state[s] = state[p];
    state[p] = tmp;
}
void BuildCorrect()
{
    int i, j, k, l;
    for(i = 0; i < 52; i++){
        Correct[0][i] = i;
    }
    /*Shuffle ten times*/
    for(i = 1; i <= 10; i++){
        for(j = 0; j < 52; j++){
            if(j%2 == 1) Correct[i][j] = Correct[i-1][j/2];
            else Correct[i][j] = Correct[i-1][j/2+26];
        }

    }
    /*
    for(i = 1; i <= 10; i++){
        for(j = 0; j < 52; j++){
            printf("%d ", Correct[i][j]);
        }
        printf("\n");
    }
    */
    return ;
}

int cmp(const void* a, const void *b)
{
    return  errorOccur[*(int*)a][0] - errorOccur[*(int*)b][0];
}
