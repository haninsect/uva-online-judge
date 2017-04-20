/*1028 - Carl the Ant*/
#include <stdio.h>
#include <limits.h>
#define DEBUG_MESSAGE
typedef enum {N, U, D, L, R}Direction;
const int dx[5] = {0, 0, 0, -1, 1};
const int dy[5] = {0, 1, -1, 0, 0};
typedef struct {
    Direction direction[2];
    int CarlPassTimes;
    int d;
    int antHere;
    int isIntersection;
} road;
typedef struct {
    int hx, hy;
    int tx, ty;
    int isMoved;
    int spawnTime;
    int waitTime; /*in intersection*/
    int isArrived;
} ant;
#define mapSize 202
void ConstructMap(road map[mapSize][mapSize], int edgesNumber);
void spawnAnt(road map[mapSize][mapSize], int antsNumber, ant ants[antsNumber], int* antsSpawn, int clock);
void UpdataAnts(int antUpdating, int antsNumber, ant ants[antsNumber], road map[mapSize][mapSize], int clock, int* antsArrived, int arriveOrder[antsNumber] );
int endX, endY;
int CarlTime = 0;
int main ()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
	int i,j,k;
	int caseNumber = 1;
	int caseTotal;
	scanf("%d", &caseTotal);
    while(caseNumber <= caseTotal){
        /*Get input and construct the map with antNumber, direction, CarlPassTimes, isIntersection*/
        int edgesNumber, antsNumber, respawnTime;
        scanf("%d %d %d", &edgesNumber, &antsNumber, &respawnTime);
        road map[mapSize][mapSize];
        ConstructMap(map, edgesNumber);
        #ifndef DEBUG_MESSAGE
        int x = 100, y = 100;
        printf("Map:\n%d %d\n", x, y);
        for(; map[x][y].direction[0]!=N; i++){
            int nx = dx[map[x][y].direction[map[x][y].CarlPassTimes]] + x, ny = dy[map[x][y].direction[map[x][y].CarlPassTimes]] + y;
            if(map[x][y].isIntersection){
                if(map[x][y].CarlPassTimes == 0) map[x][y].CarlPassTimes = 1;
                else map[x][y].CarlPassTimes = 0;
            }
            x = nx;
            y = ny;
            printf("%d %d: %d\n", x, y, map[x][y].direction[map[x][y].CarlPassTimes]);
        }
        printf("Start simulate\n", x, y);
        #endif

        /*Start simulate*/
        int clock = 0;
        int antsArrived = 0;
        int antsSpawn = 0;
        int shouldSpawn = 0;
        ant ants[antsNumber];
        int arriveOrder[antsNumber];
        while(antsArrived < antsNumber){
            #ifndef DEBUG_MESSAGE
            printf("clock: %d\n", clock);
            #endif
            /*Spawn a new ant when interval time out.*/
            /*Update old ants' location*/
            for(i = 0; i < antsSpawn; i++){
                if(ants[i].isMoved == 0 && ants[i].isArrived == 0){
                    UpdataAnts(i, antsNumber, ants, map, clock, &antsArrived, arriveOrder);
                }
            }
            if(clock % respawnTime == 0 && shouldSpawn < antsNumber){
                shouldSpawn++;
            }
            if(map[100][100].antHere == -1 && antsSpawn < shouldSpawn){
                spawnAnt(map, antsNumber, ants, &antsSpawn, clock);
            }

            for(i = 0; i < antsSpawn; i++){
                ants[i].isMoved = 0;
            }
            clock++;
        }


        /*Show result*/
        printf("Case %d:\n", caseNumber);
        printf("Carl finished the path at time %d\n", CarlTime);
        printf("The ants finished in the following order:\n");

        printf("%d", arriveOrder[0]);
        for(i = 1; i < antsNumber; i++){
            printf(" %d", arriveOrder[i]);
        }printf("\n");

        printf("The last ant finished the path at time %d\n", clock-1);
        caseNumber++;
    }
}

void UpdataAnts(int antUpdating, int antsNumber, ant ants[antsNumber], road map[mapSize][mapSize], int clock, int* antsArrived, int arriveOrder[antsNumber])
{
    #ifndef DEBUG_MESSAGE
    printf("\nAnt:%d\n", antUpdating);
    #endif

    /*Check goal*/
    /*Check the next road have ant or not*/
        /*Yes -> update the ant*/
        /*NO  -> Check the road is intersection or not*/
            /*Yes -> Compare to another direction's ant*/
            /*No -> move*/
    if(ants[antUpdating].hx == endX && ants[antUpdating].hy == endY){

        map[endX][endY].antHere = -1;
        map[ants[antUpdating].tx][ants[antUpdating].ty].antHere = -1;
        ants[antUpdating].isArrived = 1;
        ants[antUpdating].isMoved = 1;
        arriveOrder[*antsArrived] = antUpdating;
        (*antsArrived)++;
        if(*antsArrived == 1) CarlTime = clock;
        #ifndef DEBUG_MESSAGE
        printf("Arrive %d:\n", *antsArrived);
        printf("Ant:%d  time: %d isArrived: %d\n", antUpdating, clock, ants[antUpdating].isArrived);
        #endif

        return;
    }
    else {
        Direction nextD;
        if(antUpdating == 0) nextD = map[ants[antUpdating].hx][ants[antUpdating].hy].direction[map[ants[antUpdating].hx][ants[antUpdating].hy].CarlPassTimes];
        else nextD = map[ants[antUpdating].hx][ants[antUpdating].hy].direction[map[ants[antUpdating].hx][ants[antUpdating].hy].d];
        int nextX = ants[antUpdating].hx + dx[nextD], nextY = ants[antUpdating].hy + dy[nextD];
        #ifndef DEBUG_MESSAGE
        printf("Update:\n");
        printf("Ant %d:  head: (%d: %d) tail: (%d %d)\n", antUpdating, ants[antUpdating].hx, ants[antUpdating].hy, ants[antUpdating].tx, ants[antUpdating].ty);
        printf("         next: (%d: %d) nextD: %d\n", nextX, nextY, nextD);
        //system("PAUSE");
        #endif


        if (map[nextX][nextY].antHere != -1){
            //printf("u: %d %d\n", map[ants[antUpdating].hx][ants[antUpdating].hy].direction[0], map[ants[antUpdating].hx][ants[antUpdating].hy].direction[1]);
            if(ants[map[nextX][nextY].antHere].isMoved == 0) UpdataAnts(map[nextX][nextY].antHere, antsNumber, ants, map, clock, antsArrived, arriveOrder);
        }
        if (map[nextX][nextY].antHere != -1){ /*means block*/
            if (map[nextX][nextY].isIntersection == 1 && ants[antUpdating].waitTime == INT_MAX){
                ants[antUpdating].waitTime = clock;
            }
            ants[antUpdating].isMoved = 1;
            return;
        }
        else {
            if (map[nextX][nextY].isIntersection == 1){
                Direction anotherD;
                if(nextD == map[nextX][nextY].direction[0])
                    anotherD = map[nextX][nextY].direction[1];
                else
                    anotherD = map[nextX][nextY].direction[0];
                int anoX = nextX - dx[anotherD], anoY = nextY - dy[anotherD];

                if(map[anoX][anoY].antHere != -1 && ants[map[anoX][anoY].antHere].isMoved == 0){
                    if(ants[map[anoX][anoY].antHere].waitTime != ants[antUpdating].waitTime){
                        if(ants[map[anoX][anoY].antHere].waitTime < ants[antUpdating].waitTime){
                            if (ants[antUpdating].waitTime == INT_MAX){
                                ants[antUpdating].waitTime = clock;
                            }
                            ants[antUpdating].isMoved = 1;

                        }
                        else {
                            map[nextX][nextY].antHere = antUpdating;
                            if (antUpdating == 0) {
                                if(map[ants[antUpdating].hx][ants[antUpdating].hy].isIntersection && map[ants[antUpdating].hx][ants[antUpdating].hy].CarlPassTimes == 1){
                                    map[ants[antUpdating].hx][ants[antUpdating].hy].d = 1;
                                }
                                else map[ants[antUpdating].hx][ants[antUpdating].hy].CarlPassTimes = 1;

                            }
                            if(!(ants[antUpdating].hx == 100 && ants[antUpdating].hy ==100))map[ants[antUpdating].tx][ants[antUpdating].ty].antHere = -1;
                            ants[antUpdating].tx = ants[antUpdating].hx;
                            ants[antUpdating].ty = ants[antUpdating].hy;
                            ants[antUpdating].hx = nextX;
                            ants[antUpdating].hy = nextY;
                            ants[antUpdating].isMoved = 1;
                            ants[antUpdating].waitTime = INT_MAX;
                            return;
                        }
                    }
                    else { /*The same waitTime between two ants.*/
                        if(ants[map[anoX][anoY].antHere].spawnTime < ants[antUpdating].spawnTime){
                            if (ants[antUpdating].waitTime == INT_MAX){
                                ants[antUpdating].waitTime = clock;
                            }
                            ants[antUpdating].isMoved = 1;

                        }
                        else {
                            map[nextX][nextY].antHere = antUpdating;
                            if (antUpdating == 0) {
                                if(map[ants[antUpdating].hx][ants[antUpdating].hy].isIntersection && map[ants[antUpdating].hx][ants[antUpdating].hy].CarlPassTimes == 1){
                                    map[ants[antUpdating].hx][ants[antUpdating].hy].d = 1;
                                }
                                else map[ants[antUpdating].hx][ants[antUpdating].hy].CarlPassTimes = 1;

                            }
                            if(!(ants[antUpdating].hx == 100 && ants[antUpdating].hy ==100))map[ants[antUpdating].tx][ants[antUpdating].ty].antHere = -1;
                            ants[antUpdating].tx = ants[antUpdating].hx;
                            ants[antUpdating].ty = ants[antUpdating].hy;
                            ants[antUpdating].hx = nextX;
                            ants[antUpdating].hy = nextY;
                            ants[antUpdating].isMoved = 1;
                            ants[antUpdating].waitTime = INT_MAX;
                            return;
                        }

                    }
                }
                else {
                    map[nextX][nextY].antHere = antUpdating;
                    if (antUpdating == 0) {
                        if(map[ants[antUpdating].hx][ants[antUpdating].hy].isIntersection && map[ants[antUpdating].hx][ants[antUpdating].hy].CarlPassTimes == 1){
                            map[ants[antUpdating].hx][ants[antUpdating].hy].d = 1;
                        }
                        else map[ants[antUpdating].hx][ants[antUpdating].hy].CarlPassTimes = 1;

                    }
                    if(!(ants[antUpdating].hx == 100 && ants[antUpdating].hy ==100))map[ants[antUpdating].tx][ants[antUpdating].ty].antHere = -1;
                    ants[antUpdating].tx = ants[antUpdating].hx;
                    ants[antUpdating].ty = ants[antUpdating].hy;
                    ants[antUpdating].hx = nextX;
                    ants[antUpdating].hy = nextY;
                    ants[antUpdating].isMoved = 1;
                    ants[antUpdating].waitTime = INT_MAX;
                    return;
                }
            }
            else {
                map[nextX][nextY].antHere = antUpdating;
                if (antUpdating == 0) {
                    if(map[ants[antUpdating].hx][ants[antUpdating].hy].isIntersection && map[ants[antUpdating].hx][ants[antUpdating].hy].CarlPassTimes == 1){
                        map[ants[antUpdating].hx][ants[antUpdating].hy].d = 1;
                    }
                    else map[ants[antUpdating].hx][ants[antUpdating].hy].CarlPassTimes = 1;

                }
                if(!(ants[antUpdating].hx == 100 && ants[antUpdating].hy ==100))map[ants[antUpdating].tx][ants[antUpdating].ty].antHere = -1;
                ants[antUpdating].tx = ants[antUpdating].hx;
                ants[antUpdating].ty = ants[antUpdating].hy;
                ants[antUpdating].hx = nextX;
                ants[antUpdating].hy = nextY;
                ants[antUpdating].isMoved = 1;
                ants[antUpdating].waitTime = INT_MAX;
                return;
            }
        }
    }
}
void spawnAnt(road map[mapSize][mapSize], int antsNumber, ant ants[antsNumber], int* antsSpawn, int clock)
{
    #ifndef DEBUG_MESSAGE
    printf("---Spawn: %d\n", *antsSpawn);
    #endif
    map[100][100].antHere = *antsSpawn;
    ants[*antsSpawn].hx = 100;
    ants[*antsSpawn].hy = 100;
    ants[*antsSpawn].isMoved = 1;
    ants[*antsSpawn].spawnTime = clock;
    ants[*antsSpawn].waitTime = INT_MAX;
    ants[*antsSpawn].isArrived = 0;
    (*antsSpawn)++;
}
void ConstructMap(road map[mapSize][mapSize], int edgesNumber)
{
    int i,j,k;
    Direction pd;
    for(i = 0; i < mapSize; i++){
        for(j = 0; j < mapSize; j++){
            map[i][j].antHere = -1;
            map[i][j].CarlPassTimes = 0;
            map[i][j].direction[0] = N;
            map[i][j].direction[1] = N;
            map[i][j].isIntersection = 0;
            map[i][j].d = 0;
        }
    }
    int px = 100, py = 100;
    for(i = 0; i < edgesNumber; i++){
        int x, y;
        scanf("%d %d", &x, &y);
        x+=100;
        y+=100;
        if(x == px){
            if(y > py){
                for(j = py; j < y; j++){
                    if(map[x][j].direction[0] != N){
                        map[x][j].direction[1] = U;
                        map[x][j].isIntersection = 1;
                    }
                    else
                        map[x][j].direction[0] = U;
                }
                pd = U;
            }
            else {
                for(j = py; j > y; j--){
                    if(map[x][j].direction[0] != N){
                        map[x][j].direction[1] = D;
                        map[x][j].isIntersection = 1;
                    }
                    else
                        map[x][j].direction[0] = D;
                }
                pd = D;
            }
        }
        else {
            if(x > px){
                for(j = px; j < x; j++){
                    if(map[j][y].direction[0] != N){
                        map[j][y].direction[1] = R;
                        map[j][y].isIntersection = 1;
                    }
                    else
                        map[j][y].direction[0] = R;
                }
                pd = R;
            }
            else {
                for(j = px; j > x; j--){
                    if(map[j][y].direction[0] != N){
                        map[j][y].direction[1] = L;
                        map[j][y].isIntersection = 1;
                    }
                    else
                        map[j][y].direction[0] = L;
                }
                pd = L;
            }
        }
        px = x;
        py = y;
    }
    endX = px;
    endY = py;
    map[endX][endY].direction[0] = pd;
}
