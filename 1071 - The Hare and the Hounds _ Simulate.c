/* 1071 - The Hare and the Hounds _ Simulate*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxRoad 400
#define MaxIntersection 250
typedef struct {
    int index;
    int target;
    int theta;
    int len;
    int hasMarker;
    int markerLocation;
    int dir;
} Road;

typedef struct {
    int a, b;
    int an, bn;
    int len;
} Index;

int ncp, nroad, ncm, limit, start, end, sdir;
int mode, restMove, backPoint, validRoad[MaxRoad], roadChosen, roadBack;
int isChoicePoint[MaxIntersection];
Road roads[MaxIntersection][MaxRoad];
int roadNumber[MaxIntersection];
Index roadIndex[MaxRoad];
int ansRoad[MaxRoad*MaxRoad];
int ansNumber;
int crtLen;
int wrgLen;

void AddRoad(int a, int b, int adir, int bdir, int len, int index)
{
    roads[a][roadNumber[a]].theta = adir;
    roads[a][roadNumber[a]].len = len;
    roads[a][roadNumber[a]].hasMarker = 0;
    roads[a][roadNumber[a]].markerLocation = 0;
    roads[a][roadNumber[a]].index = index;
    roads[a][roadNumber[a]].target = b;
    roads[a][roadNumber[a]].dir = (bdir+360-180)%360;
    roadNumber[a]++;
}
void Read()
{
    int i, j;
    for(i = 0; i < MaxIntersection; i++){
        isChoicePoint[i] = 0;
        roadNumber[i] = 0;
    }
    ansNumber = 0;
    crtLen = 0;
    wrgLen = 0;
    mode = 0;
    for(i = 0; i < ncp; i++){
        int cp;
        scanf("%d", &cp);
        isChoicePoint[cp] = 1;
    }
    for(i = 1; i <= nroad; i++){
        int a, b, adir, bdir, len;
        scanf("%d%d%d%d%d", &a, &b, &adir, &bdir, &len);
        AddRoad(a, b, adir, bdir, len, i);
        AddRoad(b, a, bdir, adir, len, i);
        roadIndex[i].a = a;
        roadIndex[i].b = b;
        roadIndex[i].an = roadNumber[a]-1;
        roadIndex[i].bn = roadNumber[b]-1;
        roadIndex[i].len = len;
    }
    for(i = 0; i < ncm; i++){
        int a, r, len;
        scanf("%d%d%d", &a, &r, &len);
        if(roadIndex[r].a != a) {
            len = roads[roadIndex[r].a][roadIndex[r].an].len - len;
        }
        roads[roadIndex[r].a][roadIndex[r].an].hasMarker = 1;
        roads[roadIndex[r].a][roadIndex[r].an].markerLocation = len;
        roads[roadIndex[r].b][roadIndex[r].bn].hasMarker = 1;
        roads[roadIndex[r].b][roadIndex[r].bn].markerLocation = roads[roadIndex[r].a][roadIndex[r].an].len - len;
    }
}
void ActiveMode(int now, int target, int tR, int rBack)
{
    mode = 1;
    roadBack = rBack;
    backPoint = target;
    restMove = limit;
    int i;
    for(i = 0; i < MaxRoad; i++) validRoad[i] = 0;
    for(i = 0; i < roadNumber[target]; i++) validRoad[i] = 1;
    validRoad[tR] = 0;
}
void UpdataAns(Road r)
{
    ansRoad[ansNumber] = r.index;
    ansNumber++;
    crtLen += r.len;
    wrgLen += r.len;
}
void Move(int now, Road r)
{
    UpdataAns(r);
    if(isChoicePoint[r.target] == 1){
        int tr = roadIndex[r.index].an;
        if(now == roadIndex[r.index].a) tr = roadIndex[r.index].bn;
        ActiveMode(now, r.target, tr, r.index);
    }
    Simulate(r.target, r.dir);
}
void Back(int backLen)
{
    int i;
    for(i = ansNumber-1; ansRoad[i] != roadBack; i--){
        ansNumber--;
        crtLen -= roadIndex[ansRoad[i]].len;
    }
    restMove = limit;
    wrgLen += backLen;
    Simulate(backPoint, (roads[backPoint][roadChosen].theta+360-180)%360);
}
int FindRoad(int now, int dir)
{
    /*mode == 1, set the road chose inValid*/
    int i;
    int returRoad = -1;
    #ifdef DBUGM
        printf("Find Mode: %d backPoint: %d\n", mode, backPoint);
    #endif

    if(mode == 1 && now == backPoint) {
        int minTheta = LargeINT;
        int c = 0;
        for(i = 0; i < roadNumber[now]; i++){
            if(validRoad[i] == 0) {
                c++;
                continue;
            }
            int theta = abs(dir - roads[now][i].theta);
            if(abs(dir - roads[now][i].theta) >= 180) theta = 360 - abs(dir - roads[now][i].theta);
            if(theta < minTheta){
                minTheta = theta;
                returRoad = i;
            }
            else if(theta == minTheta){
                if(roads[now][i].theta + 360 - dir == theta || roads[now][i].theta - dir == theta){ /*rightmost*/
                    minTheta = abs(dir - roads[now][i].theta);
                    returRoad = i;
                }
            }
            #ifdef DBUGM
                printf("%d: Tmp road: %d, theta: %d\n", i, returRoad, theta);
            #endif
        }
        if(returRoad == -1 && c == roadNumber[now]) exit(1);
        roadChosen = returRoad;
        validRoad[returRoad] = 0;
    }
    else {
        int minTheta = LargeINT;
        for(i = 0; i < roadNumber[now]; i++){
            int theta = abs(dir - roads[now][i].theta);
            if(abs(dir - roads[now][i].theta) >= 180) theta = 360 - abs(dir - roads[now][i].theta);
            if(theta < minTheta){
                minTheta = theta;
                returRoad = i;
            }
            else if(theta == minTheta){
                if(roads[now][i].theta + 360 - dir == theta || roads[now][i].theta - dir == theta){ /*rightmost*/
                    minTheta = abs(dir - roads[now][i].theta);
                    returRoad = i;
                }
            }
            #ifdef DBUGM
                printf("Tmp road: %d, theta: %d\n", returRoad, theta);
            #endif
        }
        /*if(returRoad == -1 &&mode != 1) exit(1);*/
    }
    /*Return -1 as no road valid*/
    #ifdef DBUGM
        printf("   Find Road %d, target: %d, dir: %d\n", returRoad, roads[now][returRoad].target, roads[now][returRoad].theta);
    #endif
    return returRoad;
}
void Simulate(int now, int dir)
{
    #ifdef DBUGM
        printf("\n----------- Simulate %d %d %d-----------\n", now, dir, mode);
    #endif
    if(now == end && mode == 0) return;

    int nextRoad = FindRoad(now, dir);
    if(mode == 0){
        Move(now, roads[now][nextRoad]);
        return;
    }
    else { /*Search marker*/
        /*Is there a marker in the road?*/
        if(nextRoad == -1) { /*Dead end*/
            Back(limit - restMove);
            return;
        }
        if(roads[now][nextRoad].hasMarker){
            if(roads[now][nextRoad].markerLocation <= restMove){ /*Target found */
                mode = 0;
                Move(now, roads[now][nextRoad]);
                return;
            }
            else { /*exceed limit*/
                wrgLen += restMove;
                Back(limit);
                return;
            }
        }
        else {
            if(roads[now][nextRoad].len >= restMove){ /*exceed limit*/
                wrgLen += restMove;
                Back(limit);
                return;
            }
            else {
                if(isChoicePoint[roads[now][nextRoad].target] == 1){ /*Another choice point*/
                    wrgLen += roads[now][nextRoad].len*2;
                    Back(limit - restMove);
                    return;
                }
                else {
                    restMove -= roads[now][nextRoad].len;
                    Move(now, roads[now][nextRoad]);
                    return;
                }
            }
        }
    }
}
void ShowRoad()
{
    int i, j;
    for(i = 1; i < 5; i++){
        printf("--- Intersection: %d: ---\n", i);
        for(j = 0; j < roadNumber[i]; j++){
            printf("target: %d, theta: %d, direction: %d\n", roads[i][j].target, roads[i][j].theta, roads[i][j].dir);
            if(roads[i][j].hasMarker) printf("Has Marker in : %d\n", roads[i][j].markerLocation);
        }
    }
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
        scanf("%d%d%d%d%d%d%d", &ncp, &nroad, &ncm, &limit, &start, &end, &sdir);
        if(ncp == 0 && nroad == 0) break;
        Read();
        #ifdef DBUGM
            ShowRoad();
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        Simulate(start, sdir);
        printf("Case %d:\n", caseNumber);
        printf("   Length of hare's route is %d\n", crtLen);
        printf("   Length of hound's search is %d\n", wrgLen);
        printf("   Route:");
        for(i = 0; i < ansNumber; i++) printf(" %d", ansRoad[i]);
        printf("\n\n");
        caseNumber++;
    }
    return 0;
}
