/* 1071 - The Hare and the Hounds _ Simulate*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxRoad 200
#define MaxIntersection 105
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
int mode, restMove, backPoint, validRoad[MaxRoad], roadChosen;
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
    for(i = 0; i < nroad; i++){
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
void ActiveMode(int now, int target, int tR)
{
    backPoint = now;
    mode = 1;
    restMove = limit;
    int i;
    for(i = 0; i < roadNumber[target]; i++) validRoad[i] = 1;
    for(; i < MaxRoad; i++) validRoad[i] = 0;
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
        if(now != roadIndex[r.index].a) tr = roadIndex[r.index].bn;
        ActiveMode(now, r.target, tr);
    }
    printf("%d\n", r.dir);
    Simulate(r.target, r.dir);
}
void Back(int backLen)
{
    int i;
    for(i = ansNumber-1; ansRoad[i] != backPoint; i--){
        ansNumber--;
        wrgLen -= roadIndex[ansRoad[i]].len;
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
    if(mode == 1 && now == backPoint) {

        roadChosen = returRoad;
        validRoad[returRoad] = 0;
    }
    else {
        int minTheta = LargeINT;
        for(i = 0; i < roadNumber[now]; i++){
            if(abs(dir - roads[now][i].theta) < minTheta){
                minTheta = abs(dir - roads[now][i].theta);
                returRoad = i;
            }
            else if(abs(dir - roads[now][i].theta) == minTheta){
                if(dir - roads[now][i].theta < 0){ /*rightmost*/
                    minTheta = abs(dir - roads[now][i].theta);
                    returRoad = i;
                }
            }
        }
    }
    /*Return -1 as no road valid*/
    return returRoad;
}
void Simulate(int now, int dir)
{
    #ifdef DBUGM
        printf("----------- Simulate %d %d-----------\n", now, dir);
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
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        Simulate(start, sdir);

        caseNumber++;
    }
    return 0;
}
