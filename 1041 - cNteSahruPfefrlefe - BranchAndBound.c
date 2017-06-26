/*1041 - cNteSahruPfefrlefe - BranchAndBound*/
#include <stdio.h>
#include <stdlib.h>
int cmp(const void* a, const void *b);
void BuildCorrect();
int BranchAndBound(int state[52], int times, int modification, int totalS);
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
		freopen("output2.out", "w", stdout);
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

        /*For ten possible shuffles*/
        for(i = 1; i <= 10; i++){
            /*Recursively find the answer*/
            c = 0;
            BranchAndBound(input, i, 0, i);
            //printf("Done\n");
        }
        if(errorNumber > 1)
            qsort(errorOrder, errorNumber, sizeof(int), cmp);
        if(caseNumber > 1)printf("\n");
        printf("Case %d\n", caseNumber);
        printf("Number of shuffles = %d\n", NumberOfShuffles);
        if(errorNumber == 0) printf("No error in any shuffle\n");
        else {
            for(i = 0; i < errorNumber; i++){
                printf("Error in shuffle %d at location %d\n", errorOccur[i][0], errorOccur[i][1]);
            }
        }
        caseNumber++;

    }
    return 0;
}


int BranchAndBound(int state[52], int times, int modification, int totalS)
{
    c++;
    //printf("%d\n",c);
    //printf("New Branch: %d %d %d\n", times, modification, totalS);
    /*Cut condition: Correct or Impossible*/
    int i, j, k, l;
    int diff = 0;
    for(i = 0; i < 52; i++){
        if(state[i] != Correct[times][i]) diff++;
    }
    //printf("%d\n", diff);
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
       // printf("Cut\n");
        return 0;
    }
    else {
        //system("pause");
        for(i = 0; i < 52; i++){
               // printf("\tNew Swap: %d\n", i);
            int tmp[52];
            if(i == 51) {
                ShuffleBack(state, tmp);
                if(BranchAndBound(tmp, times-1, modification, totalS) == 1) return 1;
            }
            else {
                for(j = 0; j < 52; j++) tmp[j] = state[j];
                Swap(state, i, i+1);
                ShuffleBack(state, tmp);
                tmpError[modification][0] = times;
                tmpError[modification][1] = i;
                if(BranchAndBound(tmp, times-1, modification+1, totalS) == 1)return 1;
                Swap(state, i, i+1);
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
        //ShuffleBack(Correct[i], Correct[i-1]);
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
    return  - errorOccur[*(int*)a][0] + errorOccur[*(int*)b][0];
}
