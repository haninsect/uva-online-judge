/* 1068 - Air Conditioning Machinery _ Simulate */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxSize 25
const int delta[6][3] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
const int iTod[7] = {5, 3, 1, -1, 0, 2, 4};
const int dIndex[6] = {-3, -2, -1, 1, 2, 3};
int size[3];
int enterP[3], enterD;
int exitP[3], exitD;
int min;
int table[MaxSize][MaxSize][MaxSize];
void Read()
{
    int i, j, k;
    for(i = 0; i < MaxSize; i++){
        for(j = 0; j < MaxSize; j++){
            for(k = 0; k < MaxSize; k++){
                table[i][j][k] = 0;
            }
        }
    }
    min = LargeINT;
    scanf("%d%d", &size[1], &size[2]);
    for(i = 0; i < 3; i++) scanf("%d", &enterP[i]);
    char dir[4];
    scanf("%s", &dir);
    if(dir[1] == 'x') enterD = 1;
    else if(dir[1] == 'y') enterD = 2;
    else if(dir[1] == 'z') enterD = 3;
    if(dir[0] == '-') enterD *= -1;

    for(i = 0; i < 3; i++) scanf("%d", &exitP[i]);
    scanf("%s", &dir);
    if(dir[1] == 'x') exitD = 1;
    else if(dir[1] == 'y') exitD = 2;
    else if(dir[1] == 'z') exitD = 3;
    if(dir[0] == '-') exitD *= -1;
    for(i = 0; i < 3; i++){
        enterP[i] += delta[iTod[-enterD+3]][i];
    }
    return;
}
int CheckOutside(int nowP[3])
{
    int i;
    for(i = 0; i < 3; i++){
        if(nowP[i] > size[i] || nowP[i] <= 0) return 1;
    }
    return 0;
}
int Move(int nowP[3], int nowD, int step)
{
    int i, j;
    for(j = 0; j < step; j++){
        for(i = 0; i < 3; i++){
            nowP[i] += delta[iTod[nowD+3]][i];
        }
        #ifdef DBUGM
            printf("Next Point (%d %d %d): %d\n", nowP[0], nowP[1], nowP[2], nowD);
        #endif
        if(CheckOutside(nowP) == 1) {
            #ifdef DBUGM
                printf("Outside\n");
            #endif
            return j;
        }
        if(table[nowP[0]][nowP[1]][nowP[2]] == 1) {
            #ifdef DBUGM
                printf("Overlap\n");
            #endif
            return j;
        }
        table[nowP[0]][nowP[1]][nowP[2]] = 1;
    }
    return j;
}
void MoveBack(int nowP[3], int nowD, int step)
{
    int i, j;
    for(j = 0; j < step; j++){
        table[nowP[0]][nowP[1]][nowP[2]] = 0;
        for(i = 0; i < 3; i++){
            nowP[i] += delta[iTod[nowD+3]][i];
        }
    }
    return;
}
void DFS(int used, int nowP[3], int nowD)
{
    #ifdef DBUGM
        printf("----------- DFS %d/%d-----------\n", used, min);
        printf("(%d %d %d): %d  -> (%d %d %d): %d \n", nowP[0], nowP[1], nowP[2], nowD, exitP[0], exitP[1], exitP[2], exitD);
    #endif
    int i, j, k, c;
    /*Goal !*/
    int flag = (nowD == exitD);
    for(i = 0; i < 3 && flag == 1; i++){
        if(nowP[i] != exitP[i]) flag = 0;
    }
    if(flag == 1){
        if(used < min) min = used;
        return;
    }
    if(used > min || used >= 6) return;

    /*Open 1*/
        /*4 direction*/
    for(c = 0; c < 6; c++){
        if(dIndex[c] == nowD || dIndex[c] == -nowD) continue;
        int tmpP[3], tmpD = nowD;
        for(j = 0; j < 3; j++) tmpP[j] = nowP[j];
        int moves;
        if(moves = Move(tmpP, tmpD, 2) != 2) {
            #ifdef DBUGM
                printf("InValid move 1-1\n");
            #endif
            MoveBack(tmpP, -nowD, moves);
            continue;
        }
        tmpD = dIndex[c];
        if(moves = Move(tmpP, tmpD, 2) != 2) {
            #ifdef DBUGM
                printf("InValid move 1-2\n");
            #endif
            MoveBack(tmpP, -tmpD, 2);
            MoveBack(tmpP, -nowD, moves);
            continue;
        }
        DFS(used+1, tmpP, tmpD);
        MoveBack(tmpP, -tmpD, 2);
        MoveBack(tmpP, -nowD, 2);
    }
    /*Open 2*/
        /*4 direction*/
    for(c = 0; c < 6; c++){
        if(dIndex[c] == nowD || dIndex[c] == -nowD) continue;

        int tmpP[3], tmpD = nowD;
        for(j = 0; j < 3; j++) tmpP[j] = nowP[j];
        int moves;
        if(moves = Move(tmpP, tmpD, 3) != 3) {
            #ifdef DBUGM
                 printf("InValid move 2-1\n");
            #endif
            MoveBack(tmpP, -nowD, moves);
            continue;
        }
        tmpD = dIndex[c];
        if(moves = Move(tmpP, tmpD, 1) != 1) {
            #ifdef DBUGM
                printf("InValid move 2-2\n");
            #endif
            MoveBack(tmpP, -tmpD, moves);
            MoveBack(tmpP, -nowD, 3);
            continue;
        }
        DFS(used+1, tmpP, tmpD);
        MoveBack(tmpP, -tmpD, 1);
        MoveBack(tmpP, -nowD, 3);
    }
    #ifdef DBUGM
        printf("back\n");
    #endif
    return;
}
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;

    while(1){
        scanf("%d", &size[0]);
        if(size[0] == 0) break;
        Read();
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        DFS(0, enterP, enterD);
        if(min == LargeINT) printf("Case %d: Impossible\n", caseNumber++);
        else printf("Case %d: %d\n", caseNumber++, min);

    }
    return 0;
}
