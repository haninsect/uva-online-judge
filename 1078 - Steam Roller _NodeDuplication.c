/*1078 - Steam Roller _NodeDuplication */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxSize 105
#define MaxNode 1005
int row, col, sx, sy, ex, ey;
int gEdge[MaxSize*MaxSize][MaxSize*MaxSize];
typedef struct {
    int index;
    int dir, mul;
} Node;

void Read()
{
    int i, j;
    for(i = 0; i < MaxSize; i++){
        for(j = 0; j < MaxSize; j++){
            gEdge[i][j] = 0;
        }
    }
    for(i = 0; i < row*2-1; i++){
        int tmpc = col;
        if(i%2 == 0) tmpc = col-1;
        for(j = 0; j < tmpc; j++){
            if(i%2 == 0) {
                scanf("%d", &gEdge[i/2*col + j][i/2*col + j+1]);
                gEdge[i/2*col + j+1][i/2*col + j] = gEdge[i/2*col + j][i/2*col + j+1];
            }
            else {
                scanf("%d", &gEdge[i/2*col + j][i/2*col + col+j]);
                gEdge[i/2*col + col+j][i/2*col + j] = gEdge[i/2*col + j][i/2*col + col+j];
            }
        }
    }
}
int FindNeighbor(int index, int dir)
{
    int dx[4] = {-col, col, 0, 0};
    int dy[4] = {0, 0, -1, 1};
    int r = index + dx[dir] + dy[dir];
    if(r < 0 || r >= row*col) return -1;
    else return r;
}
int Dijkstra()
{

    int i, j, k, l;
    int valid[MaxSize*MaxSize][4][2];
    int inQueue[MaxSize*MaxSize][4][2] = {0};
    int key[MaxSize*MaxSize][4][2];
    for(i = 0; i < MaxSize*MaxSize; i++){
        for(j = 0; j < 4; j++){
            key[i][j][0] = LargeINT;
            key[i][j][1] = LargeINT;
            valid[i][j][0] = 1;
            valid[i][j][1] = 1;
        }
    }
    Node queue[MaxNode];
    int qp = 0, qs = 0;
    for(j = 0; j < 4; j++){
        key[(sx-1)*col+(sy-1)][j][1] = 0;
        inQueue[(sx-1)*col+(sy-1)][j][1] = 1;
        queue[qs].index = (sx-1)*col+(sy-1);
        queue[qs].dir = j;
        queue[qs].mul = 1;
        qs++;
    }
    int min = LargeINT;
    while(1){
        Node deQ;
        int qn;
        min = LargeINT;
        for(i = qp; i != qs; i = (i+1)%MaxNode){
            if(key[queue[i].index][queue[i].dir][queue[i].mul] < min && valid[queue[i].index][queue[i].dir][queue[i].mul] == 1){
                min = key[queue[i].index][queue[i].dir][queue[i].mul];
                deQ.index = queue[i].index;
                deQ.dir = queue[i].dir;
                deQ.mul = queue[i].mul;
                qn = i;
            }
        }
        if(min == LargeINT) break;
        valid[deQ.index][deQ.dir][deQ.mul] = 0;
        for(i = qn; i != qs; i = (i+1)%MaxNode){
            queue[i].index = queue[(i+1)%MaxNode].index;
            queue[i].dir = queue[(i+1)%MaxNode].dir;
            queue[i].mul = queue[(i+1)%MaxNode].mul;
        }
        qs = (qs - 1 + MaxNode)%MaxNode;
        int nb = FindNeighbor(deQ.index, deQ.dir);
        #ifdef DBUGM
            printf("DeQ: (%d, %d, %d), min: %d\n", deQ.index, deQ.dir, deQ.mul, min);
            printf("Neighbor: %d\n", nb);
        #endif
        if(deQ.index == (ex-1)*col+(ey-1)) break;
        if(nb == -1) continue;
        if(gEdge[deQ.index][nb] > 0){
            for(j = 0; j < 4; j++){
                if(j == deQ.dir) k = 0;
                else k = 1;
                if(valid[nb][j][k] == 0) continue;
                int acc = 1;
                if(k == 1 || deQ.mul == 1 || nb == (ex-1)*col+(ey-1)) acc = 2;
                if(key[deQ.index][deQ.dir][deQ.mul] + gEdge[deQ.index][nb]*(acc) < key[nb][j][k]){
                    key[nb][j][k] = key[deQ.index][deQ.dir][deQ.mul] + gEdge[deQ.index][nb]*(acc);
                    if(inQueue[nb][j][k] == 0){
                        inQueue[nb][j][k] = 1;
                        queue[qs].index = nb;
                        queue[qs].dir = j;
                        queue[qs].mul = k;
                        qs++;
                    }
                }
            }
        }
    }
    /*Return length*/

    if(min == LargeINT) return -1;
    else return min;
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
        scanf("%d%d%d%d%d%d", &row, &col, &sx, &sy, &ex, &ey);
        if(row == 0) break;
        Read();
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int ans = Dijkstra();
        if(ans == -1) printf("Case %d: Impossible\n", caseNumber);
        else printf("Case %d: %d\n", caseNumber, ans);
        caseNumber++;
    }
    return 0;
}
