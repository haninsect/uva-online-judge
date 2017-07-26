/*1075 - Painter _ PlanSweep*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
int main()
{
     #ifndef ONLINE_JUDGE
		//freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){
        scanf("%d", &triangleN);
        if(triangleN == 0) break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        if(isIntersection) printf("Case 1: ERROR\n", caseNumber);
        else printf("Case 1: 5 shades\n", caseNumber, maxLevel);

        caseNumber++;
    }
    return 0;
}
