/*1028 - Carl the Ant*/
#include <stdio.h>
#include <limits.h>
#define DEBUG_MESSAGE
#define LOG
typedef enum {N, U, D, L, R}Direction;
const int dx[5] = {0, 0, 0, -1, 1};
const int dy[5] = {0, 1, -1, 0, 0};
typedef struct {
    Direction direction[2];
    int CarlPassTimes;
    int d;
    int antHere;
    int antHere2;
    int antArrived;
    int antArrived2;
    int isIntersection;
} road;
typedef struct {
    int hx, hy;
    int ux, uy;
    int tx, ty;
    int isMoved;
    int isStayed;
    int spawnTime;
    int waitTime; /*in intersection*/
    int isArrived;
} ant;
#define mapSize 210
int ConstructMap(road map[mapSize][mapSize], int edgesNumber);
void spawnAnt(road map[mapSize][mapSize], int antsNumber, ant ants[antsNumber], int* antsSpawn, int clock);
void UpdataAnts(int antUpdating, int antsNumber, ant ants[antsNumber], road map[mapSize][mapSize], int clock, int* antsArrived, int arriveOrder[antsNumber] );
void StayAnt(int antUpdating, int antsNumber, ant ants[antsNumber], road map[mapSize][mapSize], int clock);
void MoveAnt(int antUpdating, int antsNumber, ant ants[antsNumber], road map[mapSize][mapSize], int nextX, int nextY );
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
            printf("-------Clock: %d-------\n", clock);
            #endif
            /*Spawn a new ant when interval time out.*/
            /*Update old ants' location*/
            if(clock % respawnTime == 0 && shouldSpawn < antsNumber){
                shouldSpawn++;
            }
            if(map[100][100].antHere == -1 && antsSpawn < shouldSpawn){
                spawnAnt(map, antsNumber, ants, &antsSpawn, clock);
            }
            for(i = 0; i < antsSpawn; i++){
                #ifndef DEBUG_MESSAGE
                printf("  Start: %d\n", i);
                #endif
                if(ants[i].isMoved == 0 && ants[i].isArrived == 0){
                    UpdataAnts(i, antsNumber, ants, map, clock, &antsArrived, arriveOrder);
                    #ifndef DEBUG_MESSAGE
                    printf("  Finish: %d\n\n", i);
                    #endif
                }
            }

            for(i = 0; i < antsSpawn; i++){
                if(ants[i].isArrived) continue;
                if(ants[i].ux == -1 && ants[i].uy == -1){
                    ants[i].isMoved = 0;
                    continue;
                }
                if(map[ants[i].hx][ants[i].hy].antHere == i) map[ants[i].hx][ants[i].hy].antHere = -1;
                else if(map[ants[i].hx][ants[i].hy].antHere2 == i) map[ants[i].hx][ants[i].hy].antHere2 = -1;
                if(i == 0 && (ants[i].hx != ants[i].ux || ants[i].hy != ants[i].uy)) {
                    if(map[ants[i].hx][ants[i].hy].isIntersection && map[ants[i].hx][ants[i].hy].CarlPassTimes == 1){
                        map[ants[i].hx][ants[i].hy].d = 1;
                    }
                    else map[ants[i].hx][ants[i].hy].CarlPassTimes = 1;
                }
                if((ants[i].hx != ants[i].ux || ants[i].hy != ants[i].uy))  ants[i].waitTime = INT_MAX;
                if((ants[i].hx != ants[i].ux || ants[i].hy != ants[i].uy)) {
                    ants[i].tx = ants[i].hx;
                    ants[i].ty = ants[i].hy;
                }
                ants[i].hx = ants[i].ux;
                ants[i].hy = ants[i].uy;
                if(map[ants[i].ux][ants[i].uy].antArrived == i){
                    map[ants[i].ux][ants[i].uy].antArrived = -1;
                    map[ants[i].ux][ants[i].uy].antHere = i;
                }
                else {
                    map[ants[i].ux][ants[i].uy].antArrived2 = -1;
                    map[ants[i].ux][ants[i].uy].antHere2 = i;
                }
                ants[i].isMoved = 0;
                ants[i].isStayed = 0;
            }
            #ifndef LOG
            printf("Clock %d over\n", clock);
            for(i = 0; i < antsSpawn; i++){
                printf("Ant %d: h(%d, %d) t(%d, %d)\n", i, ants[i].hx-100, ants[i].hy-100, ants[i].tx-100, ants[i].ty-100 );
            }
            #endif
            clock++;
        }


        /*Show result*/
        if(caseNumber > 1)printf("\n");
        printf("Case %d:\n", caseNumber);
        printf("Carl finished the path at time %d\n", CarlTime);
        printf("The ants finished in the following order:\n");

        printf("%d", arriveOrder[0]);
        for(i = 1; i < antsNumber; i++){
            printf(" %d", arriveOrder[i]);
        }printf("\n");

        printf("The last ant finished the path at time %d\n", clock);
        caseNumber++;
    }
}

void StayAnt(int antUpdating, int antsNumber, ant ants[antsNumber], road map[mapSize][mapSize], int clock)
{
    #ifndef DEBUG_MESSAGE
    printf("Stay:%d\n", antUpdating);
    system("PAUSE");
    #endif
    if (ants[antUpdating].waitTime == INT_MAX && map[ants[antUpdating].ux][ants[antUpdating].uy].isIntersection){
        ants[antUpdating].waitTime = clock;
    }
    ants[antUpdating].ux = ants[antUpdating].hx;
    ants[antUpdating].uy = ants[antUpdating].hy;
    ants[antUpdating].isMoved = 1;
    ants[antUpdating].isStayed = 1;
    if(map[ants[antUpdating].hx][ants[antUpdating].hy].isIntersection){
       if(map[ants[antUpdating].hx][ants[antUpdating].hy].antArrived == -1) map[ants[antUpdating].hx][ants[antUpdating].hy].antArrived = antUpdating;
        else map[ants[antUpdating].hx][ants[antUpdating].hy].antArrived2 = antUpdating;
    }
    else {
        if(map[ants[antUpdating].hx][ants[antUpdating].hy].antArrived != -1 && ants[map[ants[antUpdating].hx][ants[antUpdating].hy].antArrived].isStayed==0) StayAnt(map[ants[antUpdating].hx][ants[antUpdating].hy].antArrived, antsNumber, ants, map, clock);
        map[ants[antUpdating].hx][ants[antUpdating].hy].antArrived = antUpdating;
    }

    return;
}

void MoveAnt(int antUpdating, int antsNumber, ant ants[antsNumber], road map[mapSize][mapSize], int nextX, int nextY)
{
    #ifndef DEBUG_MESSAGE
    printf("Move:%d\n", antUpdating);
    printf("         next: (%d: %d)\n", nextX, nextY);
    #endif

    if(map[nextX][nextY].antArrived == -1) map[nextX][nextY].antArrived = antUpdating;
    else map[nextX][nextY].antArrived2 = antUpdating;
    ants[antUpdating].ux = nextX;
    ants[antUpdating].uy = nextY;
    ants[antUpdating].isMoved = 1;
    return;
}
void UpdataAnts(int antUpdating, int antsNumber, ant ants[antsNumber], road map[mapSize][mapSize], int clock, int* antsArrived, int arriveOrder[antsNumber])
{
    #ifndef DEBUG_MESSAGE
    printf("\nAnt:%d\n", antUpdating);
    #endif
    /*If no ant in the next location, move ant*/
    /*Else if is not in a intersection, stay ant */
    /*Else compare two ant, winner move and loser stay*/
    ants[antUpdating].isMoved = 1;
    if(ants[antUpdating].hx == endX && ants[antUpdating].hy == endY){
        map[endX][endY].antHere = -1;
        ants[antUpdating].isArrived = 1;
        ants[antUpdating].isMoved = 1;
        arriveOrder[*antsArrived] = antUpdating;
        (*antsArrived)++;
        if(*antsArrived == 1) CarlTime = clock+1;
        #ifndef DEBUG_MESSAGE
        printf("Arrive %d:\n", *antsArrived);
        printf("Ant:%d  time: %d isArrived: %d\n", antUpdating, clock+1, ants[antUpdating].isArrived);
        #endif

        return;
    }
    else {
        Direction nextD;
        if(antUpdating == 0) nextD = map[ants[antUpdating].hx][ants[antUpdating].hy].direction[map[ants[antUpdating].hx][ants[antUpdating].hy].CarlPassTimes];
        else nextD = map[ants[antUpdating].hx][ants[antUpdating].hy].direction[map[ants[antUpdating].hx][ants[antUpdating].hy].d];
        int nextX = ants[antUpdating].hx + dx[nextD], nextY = ants[antUpdating].hy + dy[nextD];
        ants[antUpdating].ux = nextX;
        ants[antUpdating].uy = nextY;
        int anoAnt = -1;
        if(map[ants[antUpdating].hx][ants[antUpdating].hy].antHere != antUpdating) anoAnt = map[ants[antUpdating].hx][ants[antUpdating].hy].antHere;
        else anoAnt = map[ants[antUpdating].hx][ants[antUpdating].hy].antHere2;
        /*In intersection*/
        if(map[ants[antUpdating].hx][ants[antUpdating].hy].isIntersection && anoAnt != -1){

            #ifndef DEBUG_MESSAGE
            printf("Intersection ants: %d v.s %d:\n", antUpdating, anoAnt);
            printf("             wait: %d v.s %d\n", ants[antUpdating].waitTime, ants[anoAnt].waitTime);
            #endif
            if(ants[anoAnt].waitTime != ants[antUpdating].waitTime){
                if(ants[anoAnt].waitTime < ants[antUpdating].waitTime){
                    StayAnt(antUpdating, antsNumber, ants, map, clock);
                }
                else {
                    StayAnt(anoAnt, antsNumber, ants, map, clock);
                   return;
                }
            }
            else { /*The same waitTime between two ants.*/
                if(ants[anoAnt].spawnTime < ants[antUpdating].spawnTime){
                    StayAnt(antUpdating, antsNumber, ants, map, clock);
                }
                else {
                    StayAnt(anoAnt, antsNumber, ants, map, clock);
                    return;
                }

            }
        }
        else{

            /*Not in intersection*/
            #ifndef DEBUG_MESSAGE
            printf("Update:\n");
            printf("Ant %d:  head: (%d: %d) tail: (%d %d)\n", antUpdating, ants[antUpdating].hx, ants[antUpdating].hy, ants[antUpdating].tx, ants[antUpdating].ty);
            printf("         next: (%d: %d) nextD: %d\n", nextX, nextY, nextD);
            printf("         waitTime: %d\n", ants[antUpdating].waitTime);
            printf("         nextArrived: %d isIntersection: %d\n", map[nextX][nextY].antArrived, map[nextX][nextY].isIntersection);
            //system("PAUSE");
            #endif
            if (map[nextX][nextY].antArrived == -1){
                /*Move*/
                MoveAnt(antUpdating, antsNumber, ants, map, nextX, nextY);
                return;
            }
            else {
                if(map[nextX][nextY].antArrived2 == -1){
                    if(map[nextX][nextY].isIntersection == 1 && (ants[antUpdating].hx != ants[map[nextX][nextY].antArrived].tx ||ants[antUpdating].hy != ants[map[nextX][nextY].antArrived].ty ))
                        MoveAnt(antUpdating, antsNumber, ants, map, nextX, nextY);
                    else
                        StayAnt(antUpdating, antsNumber, ants, map, clock);
                }
                else
                    StayAnt(antUpdating, antsNumber, ants, map, clock);

                return;
            }
        }/*Not in intersection*/
    }
}
void spawnAnt(road map[mapSize][mapSize], int antsNumber, ant ants[antsNumber], int* antsSpawn, int clock)
{
    #ifndef DEBUG_MESSAGE
    printf("       Spawn: %d\n", *antsSpawn);
    #endif
    map[100][100].antHere = *antsSpawn;
    ants[*antsSpawn].hx = 100;
    ants[*antsSpawn].hy = 100;
    ants[*antsSpawn].ux = -1;
    ants[*antsSpawn].uy = -1;
    ants[*antsSpawn].tx = -1;
    ants[*antsSpawn].ty = -1;
    ants[*antsSpawn].isMoved = 0;
    ants[*antsSpawn].isStayed = 0;
    ants[*antsSpawn].spawnTime = clock;
    ants[*antsSpawn].waitTime = INT_MAX;
    ants[*antsSpawn].isArrived = 0;
    (*antsSpawn)++;
}
int ConstructMap(road map[mapSize][mapSize], int edgesNumber)
{
    int i,j,k;
    Direction pd;
    for(i = 0; i < mapSize; i++){
        for(j = 0; j < mapSize; j++){
            map[i][j].antHere = -1;
            map[i][j].antHere2 = -1;
            map[i][j].CarlPassTimes = 0;
            map[i][j].direction[0] = N;
            map[i][j].direction[1] = N;
            map[i][j].isIntersection = 0;
            map[i][j].d = 0;
            map[i][j].antArrived = -1;
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
    return 0;
}
