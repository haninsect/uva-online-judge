/*1078 - Steam Roller _NodeDuplication */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <map>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxSize 105
#define MaxNode 1005
using namespace std;
int row, col, sx, sy, ex, ey;
int gEdge[MaxSize*MaxSize][MaxSize*MaxSize];
struct Node{
    int key;
    int index;
    int dir, mul;
    explicit Node(int k,int i, int d, int m) : key(k), index(i), dir(d), mul(m) {}
};
struct my_sort {
    bool operator()(Node const& a, Node const& b) const {
        if(a.key == b.key){
            if(a.index == b.index){
                if(a.dir == b.dir) {
                    return a.mul < b.mul;
                }
                else return a.dir < b.dir;
            }
            else return a.index < b.index;
        }
        else return a.key < b.key;
    }
};
typedef map<Node, int, my_sort>::iterator ITER;
void Read()
{
    int i, j;
    for(i = 0; i < MaxSize*MaxSize; i++){
        for(j = 0; j < MaxSize*MaxSize; j++){
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
void ShowTree(map<Node, int, my_sort> data)
{
    printf("---Map:\n");
    if(data.empty()) {
        printf("Tree Empty\n");
        return;
    }
    for(ITER itn = data.begin(); itn != data.end(); ++itn )
    {
        printf("(%d, %d, %d), key: %d\n", itn->first.index, itn->first.dir, itn->first.mul, itn->first.key);
    }
}
int Dijkstra()
{
    map<Node, int, my_sort> data;
    int i, j, k, l;
    int key[MaxSize*MaxSize][4][2];
    int parent[MaxSize*MaxSize][4][2];
    int valid[MaxSize*MaxSize][4][2];
    for(i = 0; i < MaxSize*MaxSize; i++){
        for(j = 0; j < 4; j++){
            key[i][j][0] = LargeINT;
            key[i][j][1] = LargeINT;
            valid[i][j][0] = 1;
            valid[i][j][1] = 1;
            parent[i][j][0] = -1;
            parent[i][j][1] = -1;
        }
    }
    for(j = 0; j < 4; j++){
        key[(sx-1)*col+(sy-1)][j][1] = 0;
        data.insert(make_pair(Node(0 , (sx-1)*col+(sy-1), j, 1), 0));
    }
    int min = LargeINT;
    while(1){
        ITER itn = data.begin();
        if(itn == data.end()) break;
        int nb = FindNeighbor(itn->first.index, itn->first.dir);
        #ifdef DBUGM
            printf("DeQ: (%d, %d, %d), min: %d\n", itn->first.index, itn->first.dir, itn->first.mul, itn->first.key);
            printf("Neighbor: %d\n", nb);
        #endif        
        valid[itn->first.index][itn->first.dir][itn->first.mul] = 0;
        if(nb == -1) {
            data.erase(itn);
            continue;
        }
        if(gEdge[itn->first.index][nb] > 0){
            for(j = 0; j < 4; j++){
                if(j == itn->first.dir) k = 0;
                else k = 1;
                if(valid[nb][j][k] == 0) continue;
                int acc = 1;
                if(k == 1 || itn->first.mul == 1) acc = 2;
                if(itn->first.key + gEdge[itn->first.index][nb]*(acc) < key[nb][j][k]){
                    data.erase( Node(key[nb][j][k] , nb, j, k) );
                    key[nb][j][k] = itn->first.key + gEdge[itn->first.index][nb]*(acc);
                    if(nb == (ex-1)*col+(ey-1)) {
                        int tmp = key[nb][j][k];
                        if(acc == 1) tmp += gEdge[itn->first.index][nb];
                        if(min > tmp){
                            min = tmp;
                        }
                    };
                    parent[nb][j][k] = itn->first.index;
                    data.insert( make_pair(Node(key[nb][j][k] , nb, j, k), 0) );
                }
            }
        }
        data.erase(itn);
        #ifdef DBUGM
            ShowTree(data);
        #endif

    }
    /*Return length*/    
    if(min == LargeINT) return -1;
    else return min;

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
