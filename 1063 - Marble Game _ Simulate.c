/*1063 - Marble Game _ Simulate */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM
#define LargeINT 1000000000
#define MaxSize 4
#define errorT 0.00005
#define MaxGame 500000

const int sr[4] = {1, 0, 0, 0}, sc[4] = {0, 0, 1, 0};
const int fr[4] = {0, 0, 1, 1}, fc[4] = {1, 1, 0, 0};
const int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
const int idr[4] = {1, -1, 0, 0}, idc[4] = {0, 0, 1, -1};
const int dTod[4] = {1, 0, 3, 2};
long long int pow8[17];
int size, ball, wall;
int board[MaxSize][MaxSize];
long long int game[MaxGame];
int gameNumber = 0;
int holes[MaxSize][MaxSize];
int walls[MaxSize][MaxSize][4];
int minStep;
void Read()
{
    #ifdef DBUGM
        printf("----------- Read-----------\n");
    #endif
    int i, j, k;
    /*Initial*/
    minStep = LargeINT;
    gameNumber = 0;
    for(i = 0; i < MaxGame; i++) game[i] = 0;
    for(i = 0; i < MaxSize; i++)
        for(j = 0; j < MaxSize; j++)
            board[i][j] = 0, holes[i][j] = 0;
    for(i = 0; i < MaxSize; i++)
        for(j = 0; j < MaxSize; j++)
            for(k = 0; k < 4; k++)
                walls[i][j][k];
    for(i = 0; i < size; i++) walls[0][i][0] = 1, walls[size-1][i][1] = 1, walls[i][0][2] = 1, walls[i][size-1][3] = 1;
    pow8[0] = 1;
    for(i = 1; i < 17; i++){
        pow8[i] = pow8[i-1]*8;
    }
    /*Read*/
    for(i = 1; i <= ball; i++){
        int a, b;
        scanf("%d%d", &a, &b);
        board[a][b] = i;
        game[gameNumber] += pow8[a*size + b]*i;
    }
    gameNumber++;
    for(i = 1; i <= ball; i++){
        int a, b;
        scanf("%d%d", &a, &b);
        holes[a][b] = i;
    }
     for(i = 0; i < wall; i++){
        int a, b, ta, tb;
        scanf("%d%d%d%d", &a, &b, &ta, &tb);
        int d = -1;
        for(j = 0; j < 4; j++){
            if(dr[j] == ta-a && dc[j] == tb-b) d = j;
        }
        if(d == -1) printf("Direction not found\n");
        walls[a][b][d] = 1;
        walls[ta][tb][dTod[d]] = 1;
    }
    #ifdef DBUGM
        printf("----------- Done-----------\n");
    #endif
}
void showBoard(int size, int board[size][size])
{
    int i, j;
    int validBall[ball];
    for(i = 0; i < ball; i++) validBall[i] = 0;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            if(board[i][j] != 0) validBall[board[i][j]] = 1;
        }
    }

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            if(holes[i][j] != 0 && validBall[holes[i][j]]== 1) printf("%2d ", -holes[i][j]);
            else printf("%2d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void getGame(int size, int board[size][size], long long int* game)
{
    int i, j;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            (*game) += pow8[i*size + j]*board[i][j];
        }
    }
}

void DFSgame(int now, int steps, int prevD)
{
    #ifdef DBUGM
        printf("----------- DFS: game: %d   steps: %d-----------\n", now, steps);
    #endif
    if(steps > minStep) return;
    int i, j, k;
    int validBall[ball];
    for(i = 0; i < ball; i++) validBall[i] = 0;
    /*Find new game*/
        /*Check duplicate*/
    int nowBoard[size][size];
    long long int tmpGame = game[now];
    for(i = 0; i < size*size; i++){
        int a = i/size, b = i%size;
        nowBoard[a][b] = (int)((tmpGame - tmpGame/pow8[i+1]*pow8[i+1])/pow8[i]);
        if(nowBoard[a][b] != 0) validBall[nowBoard[a][b]] = 1;
    }
    #ifdef DBUGM
        printf("Game: %llu\n", tmpGame);
        showBoard(size, nowBoard);
    #endif
    for(i = 0; i < 4; i++){
        if(prevD == i) continue;
        int tmpBoard[size][size];
        for(j = 0; j < size; j++){
            for(k = 0; k < size; k++) tmpBoard[j][k] = nowBoard[j][k];
        }
        /*fill holes*/
        #ifdef DBUGM
            printf("Direction: %d\n", i);
        #endif
        int inversed = dTod[i];
        int re = 0;
        for(j = 0; j < size && re == 0; j++){
            for(k = 0; k < size && re == 0; k++){
                int nr = sr[i]*size + dr[i]*k + fr[i]*j, nc = sc[i]*size + dc[i]*k + fc[i]*j;
                if(holes[nr][nc] != 0 && validBall[holes[nr][nc]] == 1 ){
                    int tmpr = nr, tmpc = nc;
                    #ifdef DBUGM
                        printf("D: %d, Id: %d, x, y: (%d, %d) hole: %d\n", i, inversed, nr, nc, -holes[nr][nc]);
                    #endif
                    while(1){
                        if(walls[tmpr][tmpc][inversed] == 1) break;
                        tmpr += dr[inversed], tmpc += dc[inversed];
                        if(tmpBoard[tmpr][tmpc] != 0){
                            if(tmpBoard[tmpr][tmpc] == holes[nr][nc]) {
                                tmpBoard[tmpr][tmpc] = 0;
                                validBall[holes[nr][nc]] = 0;
                            }
                            else {
                                re = 1;
                                break;
                            }
                        }
                    }
                }
            }
        }
        #ifdef DBUGM
            printf("Fill hole\n");
            showBoard(size, nowBoard);
        #endif
        if(re == 1){
            #ifdef DBUGM
                printf("---Wrong hole\n");
            #endif
            continue;
        }
        /*move balls*/
        for(j = 0; j < size; j++){
            for(k = 0; k < size; k++){
                int nr = sr[inversed]*size + dr[inversed]*k + fr[inversed]*j, nc = sc[inversed]*size + dc[inversed]*k + fc[inversed]*j;
                if(tmpBoard[nr][nc] != 0 || holes[nr][nc] != 0) continue;
                int tmpr = nr, tmpc = nc;
                while(1){
                    if(walls[tmpr][tmpc][inversed] == 1) break;
                    tmpr += dr[inversed], tmpc += dc[inversed];
                    if(tmpBoard[tmpr][tmpc] != 0){
                        tmpBoard[nr][nc] = tmpBoard[tmpr][tmpc];
                        tmpBoard[tmpr][tmpc] = 0;
                        break;
                    }
                }
            }
        }

        /*Check duplicate*/
        long long int newGame = 0;
        getGame(size, tmpBoard, &newGame);
        #ifdef DBUGM
            printf("Move Ball\n");
            //printf("New game: %lld\n", newGame);
            showBoard(size, tmpBoard);
        #endif
        if(newGame == 0) {
            if(minStep > steps) minStep = steps;
            #ifdef DBUGM
                printf("New answer: %d/%d\n", steps, minStep);
            #endif
            return;
        }
        int dup = 0;
        for(j = 0; j < gameNumber; j++){
            if(game[j] == newGame){
                dup = 1;
                break;
            }
        }
        if(dup == 1) continue;
        game[gameNumber++] = newGame;
        if(gameNumber > MaxGame) exit(1);
        DFSgame(gameNumber-1, steps+1, i);
    }
    #ifdef DBUGM
        printf("---Back\n");
    #endif
}


int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;

    while(1){
        scanf("%d%d%d", &size, &ball, &wall);
        if(size == 0) break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        Read();
        DFSgame(0, 0, -1);
        printf("Case %d: ", caseNumber);
        if(minStep == LargeINT) printf("impossible\n");
        else printf("%d moves\n\n", minStep);
        caseNumber++;
    }
    return 0;
}
