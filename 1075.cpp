/*1075 - Painter _ PlanSweep*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#define DBUGM
#define DBUGM_Inters
#define LargeINT 1000000000
#define errorT 0.0000001
#define M_PI 3.14159265358979323846
#define MaxTriangle 100005
using namespace std;

struct Index {
    double x, y;
    int index;
};
struct Point {
    double x, y;
    int triangle;
    int iEdges[2];
    int iN;
    int dEdges[2];
    int dN;
};
struct Edge {
    int triangle;
    int lPoint;
    int rPoint;
    double x1, y1;
    double x2, y2;
    /*a1*x + b1*y + c1 = 0, a1 >= 0*/
    double a1, b1, c1;
};

int  triangleN;
Point points[MaxTriangle*3];
Index sortedIndex[MaxTriangle*3];
int pNumber;
Edge edges[MaxTriangle*3];
int eNumber;
Edge verticalE[MaxTriangle*3];
int veNumber;
bool isIntersection;
int maxLevel;
void ShowEdge(Edge edgeA)
{
    printf("Triangle: %d, (%.2lf, %.2lf) -> (%.2lf, %.2lf)\n", edgeA.triangle, points[edgeA.lPoint].x, points[edgeA.lPoint].y, points[edgeA.rPoint].x, points[edgeA.rPoint].y);
}
int Upper(const Point& p, const Edge& e)
{
    /*ShowEdge(e);
    printf("P %lf %lf\n", p.x, p.y);*/
    if(fabs(e.a1*p.x + e.b1*p.y + e.c1 - 0) < errorT) {
        isIntersection = true;
    };
    if(fabs(e.b1 - 0) < errorT) {
        if(p.y > e.y2+errorT) return 1;
        else if(p.y < e.y1-errorT) return 0;
        else return -1;
    };
    if(fabs(e.a1 - 0) < errorT) {
        if (fabs(e.b1*p.y + e.c1 - 0) < errorT) return -1;
        else if(e.b1*p.y + e.c1 > 0) return 1;
        else return 0;
    }
    else {
        if(e.b1 > 0) {
            if (fabs(e.a1*p.x + e.b1*p.y + e.c1 - 0) < errorT) return -1;
            else if(e.a1*p.x + e.b1*p.y + e.c1 > 0) return 1;
            else return 0;
        }
        else {
            if (fabs(e.a1*p.x + e.b1*p.y + e.c1 - 0) < errorT) return -1;
            else if(e.a1*p.x + e.b1*p.y + e.c1 < 0) return 1;
            else return 0;
        }
    }
}
struct  mySort
{
   bool operator() (const Edge &edgeA, const Edge &edgeB) const
   {
       /*ShowEdge(edgeA);
       ShowEdge(edgeB);
       printf("\n");
        /*printf("t(%d, %d) ", edgeA.triangle, edgeB.triangle);*/
      if(edgeA.triangle == edgeB.triangle){
        double midA, midB;
        midA = (points[edgeA.lPoint].y + points[edgeA.rPoint].y)/2;
        midB = (points[edgeB.lPoint].y + points[edgeB.rPoint].y)/2;
        /*printf("(%lf, %lf) ", midA, midB);*/
        if(fabs(midA - midB) < errorT){
            return true;
            double midA2 = (points[edgeA.lPoint].x + points[edgeA.rPoint].x)/2;
            double midB2 = (points[edgeB.lPoint].x + points[edgeB.rPoint].x)/2;
            if(midA < midB-errorT) return true;
            else return false;
        }
        else {
            if(midA > midB+errorT) return true;
            else return false;
        }

      }
      else {
        if(points[edgeA.lPoint].x > points[edgeB.lPoint].x) {
            if(Upper(points[edgeA.lPoint], edgeB) == 1) {
                   /* printf("YEs - up\n");*/
                return true;
            }
            else return false;
        }
        else {
            if(Upper(points[edgeB.lPoint], edgeA) != 0 ) {
                    /*printf("YEs - lower\n");*/
                return false;
            }
            else return true;
        }
      }
   }
};
map<Edge, int, mySort> mapTree;
typedef map<Edge, int, mySort>::iterator ITER;
void AddEdge(int p1, int p2, int triangle)
{
    if(fabs(points[p1].x - points[p2].x) < errorT) {/*Vertical Line*/
        if(points[p1].y > points[p2].y + errorT) {
            AddEdge(p2, p1, triangle);
            return;
        }
        edges[eNumber].triangle = triangle;
        edges[eNumber].lPoint = p1;
        edges[eNumber].rPoint = p2;
        edges[eNumber].x1 = points[p1].x, edges[eNumber].y1 = points[p1].y;
        edges[eNumber].x2 = points[p2].x, edges[eNumber].y2 = points[p2].y;
        edges[eNumber].a1 = 1;
        edges[eNumber].b1 = 0;
        edges[eNumber].c1 = -points[p1].x;
        if(edges[eNumber].a1 < 0 || (fabs(edges[eNumber].a1 - 0) < errorT && edges[eNumber].b1 < 0) ) {
            edges[eNumber].a1 *= -1;
            edges[eNumber].b1 *= -1;
            edges[eNumber].c1 *= -1;
        }
        points[p1].iEdges[points[p1].iN++] = eNumber;
        points[p2].dEdges[points[p2].dN++] = eNumber;
        eNumber++;
        return;
    }
    if(points[p1].x > points[p2].x + errorT) {
        AddEdge(p2, p1, triangle);
        return;
    }
    edges[eNumber].triangle = triangle;
    edges[eNumber].lPoint = p1;
    edges[eNumber].rPoint = p2;
    edges[eNumber].x1 = points[p1].x, edges[eNumber].y1 = points[p1].y;
    edges[eNumber].x2 = points[p2].x, edges[eNumber].y2 = points[p2].y;
    edges[eNumber].a1 = points[p2].y - points[p1].y;
    edges[eNumber].b1 = points[p1].x - points[p2].x;
    edges[eNumber].c1 = points[p2].x*points[p1].y - points[p1].x*points[p2].y;
    if(edges[eNumber].a1 < 0 || (fabs(edges[eNumber].a1 - 0) < errorT && edges[eNumber].b1 < 0) ) {
        edges[eNumber].a1 *= -1;
        edges[eNumber].b1 *= -1;
        edges[eNumber].c1 *= -1;
    }
    points[p1].iEdges[points[p1].iN++] = eNumber;
    points[p2].dEdges[points[p2].dN++] = eNumber;
    /*printf("%d: %d %d %d\n", points[p1].triangle, edges[eNumber].triangle,  edges[points[p1].iEdges[points[p1].iN-1]].triangle);*/
    eNumber++;

}

void Read()
{
    mapTree.clear();
    isIntersection = false;
    maxLevel = 0, pNumber = 0, eNumber = 0, veNumber = 0;
    for(int i = 0; i < triangleN; i++){
        for(int j = 0; j < 3; j++) {
            scanf("%lf%lf", &points[pNumber].x, &points[pNumber].y);
            points[pNumber].triangle = i;
            points[pNumber].iN = 0, points[pNumber].dN = 0;
            sortedIndex[pNumber].x = points[pNumber].x;
            sortedIndex[pNumber].y = points[pNumber].y;
            sortedIndex[pNumber].index = pNumber;
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
void ShowTree()
{
    printf("---Map:\n");
    if(mapTree.empty()) {
        printf("Tree Empty\n");
        return;
    }
    for(ITER it = mapTree.begin(); it != mapTree.end(); ++it )
    {
        printf("Triangle: %d, (%.2lf, %.2lf) -> (%.2lf, %.2lf) level: %d\n", it->first.triangle, points[it->first.lPoint].x, points[it->first.lPoint].y, points[it->first.rPoint].x, points[it->first.rPoint].y, it->second);
    }
}
int CheckOnSide(Edge line, Point pt)
{
    double l1 = sqrt((line.x1-pt.x)*(line.x1-pt.x) + (line.y1-pt.y)*(line.y1-pt.y));
    double l2 = sqrt((line.x2-pt.x)*(line.x2-pt.x) + (line.y2-pt.y)*(line.y2-pt.y));
    double l3 = sqrt((line.x2-line.x1)*(line.x2-line.x1) + (line.y2-line.y1)*(line.y2-line.y1));
    #ifdef DBUGM_Inters
       printf("Check on Side: %05.2lf + %05.2lf - %05.2lf = %05.2lf: %d\n", l1, l2, l3, fabs(l1 + l2 - l3), fabs(l1 + l2 - l3) < errorT);
    #endif
    if(fabs(l1 + l2 - l3) < errorT) return 1;
    else return 0;
}
int FindIntersection(Edge edgeA, Edge edgeB)
{
    if(edgeA.triangle == edgeB.triangle) return 0;
    #ifdef DBUGM_Inters
        printf("\nFindIntersection %2.2lf\n", fabs(((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1))));
        ShowEdge(edgeA);
        ShowEdge(edgeB);
    #endif
    /*Same Slope*/
    if( fabs((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1)) < errorT ){
        if(CheckOnSide(edgeA, points[edgeB.lPoint])) return 1;
        if(CheckOnSide(edgeA, points[edgeB.rPoint])) return 1;
        if(CheckOnSide(edgeB, points[edgeA.lPoint])) return 1;
        if(CheckOnSide(edgeB, points[edgeA.rPoint])) return 1;
        return 0;
    }

    /*Compute the proportion from edgeA->P1 to the intersection*/
    /*ua = ((x4 - x3)(y1 - y3) - (y4-y3)(x1-x3)) / ((y4 - y3)(x2 - x1) - (x4 - x3)(y2 - y1))*/
    double ppB = ((edgeA.x2 - edgeA.x1)*(edgeB.y1 - edgeA.y1) - (edgeA.y2 - edgeA.y1)*(edgeB.x1 - edgeA.x1)) / ((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1));
    double ppA = ((edgeB.x2 - edgeB.x1)*(edgeA.y1 - edgeB.y1) - (edgeB.y2 - edgeB.y1)*(edgeA.x1 - edgeB.x1)) / ((edgeB.y2 - edgeB.y1)*(edgeA.x2 - edgeA.x1) - (edgeB.x2 - edgeB.x1)*(edgeA.y2 - edgeA.y1));
    #ifdef DBUGM_Inters
        printf("ppA, ppB: %2.2lf %2.2lf, %d\n", ppA, ppB, ppB > 1+errorT);
    #endif
    if(ppB > 1+errorT || ppB < 0-errorT || ppA > 1+errorT || ppA < 0-errorT) return 0;
    else {
        #ifdef DBUGM_Inters
            printf("---Ture! \n");
        #endif
        return 1;
    }
}
void EdgeCopy(Edge *A, Edge B)
{
    A->a1 = B.a1;
    A->b1 = B.b1;
    A->c1 = B.c1;
    A->lPoint = B.lPoint;
    A->rPoint = B.rPoint;
    A->triangle = B.triangle;
    A->x1 = B.x1;
    A->x2 = B.x2;
    A->y1 = B.y1;
    A->y2 = B.y2;
}
int cmpEdge2(const void* a, const void* b)
{
    Edge *ap = (Edge*)a;
    Edge *bp = (Edge*)b;
    if(fabs(ap->x1 - bp->x1) < errorT){
        if(fabs(ap->y1 - bp->y1) < errorT) return 0;
        else if (ap->y1 > bp->y1) return -1;
        else return 1;
    }
    else if (ap->x1 > bp->x1) return -1;
    else return 1;
}
int cmpEdge(const void* a, const void* b)
{
    Edge *ap = (Edge*)a;
    Edge *bp = (Edge*)b;
    if(fabs(ap->x1 - bp->x1) < errorT){
        if(fabs(ap->y1 - bp->y1) < errorT) return 0;
        else if (ap->y1 > bp->y1) return 1;
        else return -1;
    }
    else if (ap->x1 > bp->x1) return 1;
    else return -1;
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
        scanf("%d", &triangleN);
        if(triangleN == -1) break;
        Read();
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        /*Check Vertical Line Crossing*/
        /*Edge sortV[veNumber];*/
        /*printf("%d %d\n", veNumber, eNumber);*/
        /*for(i = 0; i < veNumber; i++) EdgeCopy(&sortV[i], verticalE[i]);
        qsort(sortV, veNumber, sizeof(Edge), cmpEdge2);

        Edge sortE[eNumber];
        for(i = 0; i < eNumber; i++) EdgeCopy(&sortE[i], edges[i]);
        qsort(sortE, eNumber, sizeof(Edge), cmpEdge);

        for(i = 0; i < veNumber && !isIntersection; i++){
            if(sortE[0].x1 > sortV[i].x1+errorT) break;
            for(j = 0; j < eNumber && !isIntersection; j++){
                if(sortE[j].x1 > sortV[i].x1+errorT) break;
                if(FindIntersection(sortV[i], sortE[j]) == 1) isIntersection = true;
            }
        }*/
        /*printf("isIntersection %d\n", isIntersection);*/
        qsort(sortedIndex, pNumber, sizeof(Index), cmp);
        #ifdef DBUGM
            printf("Sorted: %d\n", pNumber);
            for(i = 0; i < pNumber; i++){
                printf("(%lf, %lf) %d\n", points[sortedIndex[i].index].x, points[sortedIndex[i].index].y, points[sortedIndex[i].index].triangle);
            }
        #endif

        for(i = 0; i < pNumber && !isIntersection; i++){
            /*Delete*/
            #ifdef DBUGM
                 printf("\n----- Point: (%lf, %lf) %d %d\n", points[sortedIndex[i].index].x, points[sortedIndex[i].index].y, points[sortedIndex[i].index].triangle, edges[points[sortedIndex[i].index].iEdges[0]].triangle);
            #endif
            for(j = 0; j < points[sortedIndex[i].index].dN && !isIntersection; j++){
                #ifdef DBUGM
                    printf("Delete %d/%d: %d\n", j, points[sortedIndex[i].index].dN, points[sortedIndex[i].index].dEdges[j]);
                    printf("Triangle: %d, (%.2lf, %.2lf) -> (%.2lf, %.2lf)\n", edges[points[sortedIndex[i].index].dEdges[j]].triangle, points[edges[points[sortedIndex[i].index].dEdges[j]].lPoint].x, points[edges[points[sortedIndex[i].index].dEdges[j]].lPoint].y, points[edges[points[sortedIndex[i].index].dEdges[j]].rPoint].x, points[edges[points[sortedIndex[i].index].dEdges[j]].rPoint].y);
                #endif
                ITER itp = mapTree.find( edges[points[sortedIndex[i].index].dEdges[j]] );
                ITER itn = mapTree.find( edges[points[sortedIndex[i].index].dEdges[j]] );
                itn++;
                if(itp != mapTree.begin() && itn != mapTree.end()){
                    itp--;
                    if(FindIntersection(itp->first, itn->first) == 1) isIntersection = true;
                }
                ITER it = mapTree.find( edges[points[sortedIndex[i].index].dEdges[j]] );
                if(it != mapTree.end())  mapTree.erase(it);
                else printf("\t\t\t\t\t\t Not Found %d\n", mapTree.count(edges[points[sortedIndex[i].index].iEdges[j]]));
            }
            /*Insert*/
            for(j = 0; j < points[sortedIndex[i].index].iN && !isIntersection; j++){
                #ifdef DBUGM
                    printf("--Insert %d/%d: %d\n", j, points[sortedIndex[i].index].iN, points[sortedIndex[i].index].iEdges[j]);
                    printf("Triangle: %d, (%.2lf, %.2lf) -> (%.2lf, %.2lf)\n", edges[points[sortedIndex[i].index].iEdges[j]].triangle, points[edges[points[sortedIndex[i].index].iEdges[j]].lPoint].x, points[edges[points[sortedIndex[i].index].iEdges[j]].lPoint].y, points[edges[points[sortedIndex[i].index].iEdges[j]].rPoint].x, points[edges[points[sortedIndex[i].index].iEdges[j]].rPoint].y);
                #endif
                pair<ITER, bool> iter = mapTree.insert(make_pair( edges[points[sortedIndex[i].index].iEdges[j]] , 0));
                #ifdef DBUGM
                    printf("Insert Done\n");
                    ShowTree();
                    printf("ano\n");
                #endif
                /*Check intersection*/
                ITER itp = mapTree.find( edges[points[sortedIndex[i].index].iEdges[j]] );
                if(itp == mapTree.end()) printf("\t\t\t\t\t\t Insert Not Found %d\n", mapTree.count(edges[points[sortedIndex[i].index].iEdges[j]]));
                ITER itn = mapTree.find( edges[points[sortedIndex[i].index].iEdges[j]] );
                itn++;
                if(itp != mapTree.begin()){
                    itp--;
                    if(FindIntersection(itp->first, edges[points[sortedIndex[i].index].iEdges[j]]) == 1) isIntersection = true;
                }
                if(itn != mapTree.end()){
                    if(FindIntersection(itn->first, edges[points[sortedIndex[i].index].iEdges[j]]) == 1) isIntersection = true;
                }
                /*Update Level*/
                if(iter.first != mapTree.begin() && itn != mapTree.end()){
                    if( itp->first.triangle == itn->first.triangle) iter.first->second = itn->second+1;
                    else {
                        if(itp->second == itn->second) iter.first->second = itn->second;
                        else {
                            if(itp->second > itn->second) iter.first->second = itp->second;
                            else iter.first->second = itn->second;
                        }
                    }
                }
                else {
                    iter.first->second = 1;
                }
                if(iter.first->second > maxLevel) maxLevel = iter.first->second;

            }

            #ifdef DBUGM
                printf("Point %d Done\n", i);
                ShowTree();
            #endif
        }
        if(isIntersection) printf("Case %d: ERROR\n", caseNumber);
        else printf("Case %d: %d shades\n", caseNumber, maxLevel+1);
        caseNumber++;
    }
    return 0;
}
