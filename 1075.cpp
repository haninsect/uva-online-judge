/*1075 - Painter _ PlanSweep*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#define DBUGM
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxTriangle 100005
using namespace std;

struct Point {
    double x, y;
    int tirangle;
    int iEdges[2];
    int iN;
    int dEdges[2];
    int dN;
};
struct Edge {
    int tirangle;
    int lPoint;
    int rPoint;
    struct equation {
        /*a1*x + b1*y + c1 = 0, a1 >= 0*/
        double a1, b1, c1;
        int Upper(Point& p){
            if(fabs(b1 - 0) < errorT) return -1;
            if(fabs(a1 - 0) < errorT) {
                if(b1*p.y + c1 > 0) return true;
                else return false;
            }
            else {
                if(b1 > 0) {
                    if(a1*p.x + b1*p.y + c1 > 0) return true;
                    else return false;
                }
                else {
                    if(a1*p.x + b1*p.y + c1 < 0) return true;
                    else return false;
                }
            }
        }
    };
    equation eqt;
};

int  triangleN;
Point points[MaxTriangle];
int pNumber;
Edge edges[MaxTriangle*3];
int eNumber;
Edge verticalE[MaxTriangle*3];
int veNumber;
bool isIntersection;
int maxLevel;

struct  mySort
{
   bool operator() (const Edge &edgeA, const Edge &edgeB) const
   {
      if(edgeA.tirangle == edgeB.tirangle){
        double midA, midB;
        midA = (points[edgeA.lPoint].y + points[edgeA.rPoint].y)/2;
        midB = (points[edgeB.lPoint].y + points[edgeB.rPoint].y)/2;
        if(midA < midB) return true;
        else return false;
      }
      else {
        if(points[edgeA.lPoint].x > points[edgeB.lPoint].x) {
            if(edgeB.eqt.Upper(points[edgeA.lPoint]) == 1) return true;
            else return false;
        }
        else {
            if(edgeA.eqt.Upper(points[edgeB.lPoint]) == 1) return false;
            else return true;
        }
      }
   }
};
map<Edge, int, mySort> mapTree;

void AddEdge(int p1, int p2, int tiangle)
{
    if(fabs(points[p1].x - points[p2].x) < errorT) {/*Vertical Line*/
        if(points[p1].y > points[p2].y + errorT) {
            AddEdge(p2, p1, triangleN);
            return;
        }
        verticalE[veNumber].lPoint = p1;
        verticalE[veNumber].rPoint = p2;
        verticalE[veNumber].eqt.a1 = 1;
        verticalE[veNumber].eqt.b1 = 0;
        verticalE[veNumber].eqt.c1 = -points[p1].x;
        veNumber++;
        return;
    }
    if(points[p1].x > points[p2].x + errorT) {
        AddEdge(p2, p1, triangleN);
        return;
    }
    edges[eNumber].tirangle = tiangle;
    edges[eNumber].lPoint = p1;
    edges[eNumber].rPoint = p2;
    edges[eNumber].eqt.a1 = points[p2].y - points[p1].y;
    edges[eNumber].eqt.b1 = points[p1].x - points[p2].x;
    edges[eNumber].eqt.c1 = points[p2].x*points[p1].y - points[p1].x*points[p2].y;
    if(edges[eNumber].eqt.a1 < 0) {
        edges[eNumber].eqt.a1 *= -1;
        edges[eNumber].eqt.b1 *= -1;
        edges[eNumber].eqt.c1 *= -1;
    }
    points[p1].iEdges[points[p1].iN++] = eNumber;
    points[p2].dEdges[points[p2].dN++] = eNumber;
    eNumber++;

}
void Read()
{
    isIntersection = false;
    maxLevel = 0, pNumber = 0, eNumber = 0, veNumber = 0;
    for(int i = 0; i < triangleN; i++){
        for(int j = 0; j < 3; j++) {
            scanf("%lf%lf", &points[pNumber].x, &points[pNumber].y);
            points[pNumber].tirangle = i;
            points[pNumber].iN = 0, points[pNumber].dN = 0;
            pNumber++;
        }
        AddEdge(pNumber-1, pNumber-2, i);
        AddEdge(pNumber-1, pNumber-3, i);
        AddEdge(pNumber-2, pNumber-3, i);
    }
}
int cmp(const void* a, const void* b)
{
    Point *ap = (Point*)a;
    Point *bp = (Point*)b;
    if(fabs(ap->x - bp->x) < errorT){
        if(fabs(ap->y - bp->y) < errorT) return 0;
        else if (ap->y > bp->y) return 1;
        else return -1;
    }
    else if (ap->x > bp->x) return 1;
    else return -1;
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
        scanf("%d", &triangleN);
        if(triangleN == -1) break;
        Read();
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        /*Check Vertical Line Crossing*/
        qsort(points, pNumber, sizeof(Point), cmp);
        for(i = 0; i < pNumber; i++){
            /*Insert*/
            for(j = 0; j < points[i].iN; i++){
                std::pair<map<Edge, int, mySort>::iterator, bool> res = mapTree.insert(std::make_pair(edges[points[i].iEdges[j]], 0));
            }
            /*Delete*/
            #ifdef DBUGM
                ShowTree();
            #endif
        }

        if(isIntersection) printf("Case 1: ERROR\n", caseNumber);
        else printf("Case 1: %d shades\n", caseNumber, maxLevel);

        caseNumber++;
    }
    return 0;
}
