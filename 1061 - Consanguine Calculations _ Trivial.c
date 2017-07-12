/*1061 - Consanguine Calculations _ Trivial*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define MaxSize 350
#define errorT 0.00005
const char blood[4][3] = {"A", "B", "AB", "O"};
const char Rh[4][2] = {"-", "+"};
int BloodTable[4][4][4]; /*[father][mother][child]*/
int RhTable[4][4][4]; /*[father][mother][child]*/
void ConstructBlood();
void FindBlood(char father[5], char mother[5], char child[5], int pBlood[5]);
void FindRh(char father[5], char mother[5], char child[5], int pRh[5]);
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    ConstructBlood();
    while(1){
        char father[5] = {'\0'}, mother[5] = {'\0'}, child[5] = {'\0'};
        scanf("%s", &father);
        if(strcmp(father, "E") == 0){
            scanf("%s", &father);
            scanf("%s", &father);
            break;
        }
        scanf("%s", &mother);
        scanf("%s", &child);
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int pBlood[5];
        for(i = 0; i < 5; i++) pBlood[i] = -1;
        FindBlood(father, mother, child, pBlood);
        int pRh[5];
        for(i = 0; i < 5; i++) pRh[i] = -1;
        FindRh(father, mother, child, pRh);
        #ifdef DBUGM
        for(i = 0; i < 5; i++) printf("%d ", pBlood[i]);
        printf("\n");
        for(i = 0; i < 5; i++) printf("%d ", pRh[i]);
        printf("\n");
        #endif
        int count = 0;
        char singleAns[5] = {'\0'};
        for(i = 0; pBlood[i] != -1; i++){
            for(j = 0; pRh[j] != -1; j++){
                   count++;
                   sprintf(singleAns, "%s%s", blood[pBlood[i]], Rh[pRh[j]]);
            }
        }
        /*Show result*/
        printf("Case %d:", caseNumber);
        if(father[0] == '?'){
            if(count == 0) printf(" IMPOSSIBLE");
            else if(count == 1) printf(" %s", singleAns);
            else {
                int time = 0;
                for(i = 3; i >= 0; i--){
                    if(pBlood[i] == -1) continue;
                    for(j = 0; pRh[j] != -1; j++){
                        if(time == 0) printf(" {%s%s",  blood[pBlood[i]], Rh[pRh[j]]);
                        else printf(", %s%s", blood[pBlood[i]], Rh[pRh[j]]);
                        time++;
                    }
                }
                printf("}");
            }
            printf(" %s", mother);
            printf(" %s", child);
        }
        else if(mother[0] == '?'){
            printf(" %s", father);
            if(count == 0) printf(" IMPOSSIBLE");
            else if(count == 1) printf(" %s", singleAns);
            else {
                int time = 0;
                for(i = 3; i >= 0; i--){
                    if(pBlood[i] == -1) continue;
                    for(j = 0; pRh[j] != -1; j++){
                        if(time == 0) printf(" {%s%s",  blood[pBlood[i]], Rh[pRh[j]]);
                        else printf(", %s%s", blood[pBlood[i]], Rh[pRh[j]]);
                        time++;
                    }
                }
                printf("}");
            }
            printf(" %s", child);
        }
        else if(child[0] == '?'){
            printf(" %s", father);
            printf(" %s", mother);
            if(count == 0) printf(" IMPOSSIBLE");
            else if(count == 1) printf(" %s", singleAns);
            else {
                int time = 0;
                for(i = 3; i >= 0; i--){
                    if(pBlood[i] == -1) continue;
                    for(j = 0; pRh[j] != -1; j++){
                        if(time == 0) printf(" {%s%s",  blood[pBlood[i]], Rh[pRh[j]]);
                        else printf(", %s%s", blood[pBlood[i]], Rh[pRh[j]]);
                        time++;
                    }
                }
                printf("}");
            }
        }
        printf("\n");
        caseNumber++;
    }
    return 0;
}

void FindRh(char father[5], char mother[5], char child[5], int pRh[5])
{
    int fb = -1, mb = -1, cb = -1;
    if(father[0] != '?'){
        if(father[strlen(father)-1] == '+') fb = 1;
        else fb = 0;
    }
    if(mother[0] != '?'){
        if(mother[strlen(mother)-1] == '+') mb = 1;
        else mb = 0;
    }
    if(child[0] != '?'){
        if(child[strlen(child)-1] == '+') cb = 1;
        else cb = 0;
    }
    if(fb == -1){
        int i;
        int pc = 0;
        for(i = 0; i < 2; i++) {
            if(RhTable[i][mb][cb] == 1) pRh[pc++] = i;
        }

    }
    else if(mb == -1){
        int i;
        int pc = 0;
        for(i = 0; i < 2; i++) {
            if(RhTable[fb][i][cb] == 1) pRh[pc++] = i;
        }
    }

    else if(cb == -1){
        int i;
        int pc = 0;
        for(i = 0; i < 2; i++) if(RhTable[fb][mb][i] == 1) pRh[pc++] = i;
    }

}
void FindBlood(char father[5], char mother[5], char child[5], int pBlood[5])
{
    int fb = -1, mb = -1, cb = -1;
    if(father[0] != '?'){
        if(strlen(father) == 2) {
            if(father[0] == 'O') fb = 3;
            else fb = father[0] - 'A';
        }
        else if(strlen(father) == 3) fb = 2;
    }
    if(mother[0] != '?'){
        if(strlen(mother) == 2) {
            if(mother[0] == 'O') mb = 3;
            else mb = mother[0] - 'A';
        }
        else if(strlen(mother) == 3) mb = 2;
    }
    if(child[0] != '?'){
        if(strlen(child) == 2) {
            if(child[0] == 'O') cb = 3;
            else cb = child[0] - 'A';
        }
        else if(strlen(child) == 3) cb = 2;
    }
    if(fb == -1){
        int i;
        int pc = 0;
        for(i = 0; i < 4; i++) {
            if(BloodTable[i][mb][cb] == 1) pBlood[pc++] = i;
        }

    }
    else if(mb == -1){
        int i;
        int pc = 0;
        for(i = 0; i < 4; i++) {
            if(BloodTable[fb][i][cb] == 1) pBlood[pc++] = i;
        }
    }

    else if(cb == -1){
        int i;
        int pc = 0;
        for(i = 0; i < 4; i++) if(BloodTable[fb][mb][i] == 1) pBlood[pc++] = i;
    }

}
void ConstructBlood()
{
    int i, j, k;
    for(i = 0; i < 2; i++){
        for(j = 0; j < 2; j++){
            RhTable[i][j][0] = 1;
            RhTable[i][j][1] = 1;
            if(i == 0 && j == 0 ) RhTable[i][j][1] = 0;
        }
    }
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            for(k = 0; k < 4; k++){
                BloodTable[i][j][k] = 0;
            }
        }
    }
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            if(i == 0 || j == 0 || i == 2 || j == 2) BloodTable[i][j][0] = 1;
            if(i == 1 || j == 1 || i == 2 || j == 2) BloodTable[i][j][1] = 1;
            if((i == 0 && j == 1) || (i == 1 && j == 0) || (i == 2 && j != 3) || (i != 3 && j == 2)) BloodTable[i][j][2] = 1;
            if(i != 2 && j != 2) BloodTable[i][j][3] = 1;
        }
    }
}
