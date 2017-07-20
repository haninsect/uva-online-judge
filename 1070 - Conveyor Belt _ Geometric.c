/*1070 - Conveyor Belt _ Geometric */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxcircleNumber 25
typedef struct {
    double x, y;
} Vec;
typedef struct Vertex {
    double x, y;
} Point;
typedef struct {
    double x1, x2;
    double y1, y2;
    double len;
} Segment;

typedef struct {
    Point center;
    double radius;
    int direction; /*1 clockwise, -1 counterclockwise*/
} Circle;

typedef struct {
    int isValid;
    Segment cutLine;
} Edge;

int circleNumber;
Circle circles[MaxcircleNumber];
int start, end, limit;
Edge edgeTable[MaxcircleNumber][MaxcircleNumber];
int shortestPath[MaxcircleNumber][MaxcircleNumber];
int Max(int c1, int c2)
{
    if(circles[c1].radius > circles[c2].radius) return c1;
    else return c2;
}
int Min(int c1, int c2)
{
    if(circles[c1].radius < circles[c2].radius) return c1;
    else return c2;
}
void Read()
{
    int i, j;
    for(i = 0; i < circleNumber; i++){
        char c[5];
        scanf("%lf%lf%lf%s", &circles[i].center.x, &circles[i].center.y, circles[i].radius, &c);
        if(strcmp(c, "CC") == 0) circles[i].direction = 1;
        else if(strcmp(c, "C") == 0) circles[i].direction = -1;
        else {
            printf("Input Error\n");
            exit(1);
        }
    }
    scanf("%lf%lf%lf", &start, &end, &limit);

}
void FindLine(Point a, Point b, Segment* line)
{
    line->x1 = a.x;
    line->y1 = a.y;
    line->x2 = b.x;
    line->y2 = b.y;
    line->len = sqrt((b.x-a.x)*(b.x-a.x) + (b.y-a.y)*(b.y-a.y));
}
Vec InverseVec(Vec v)
{
    Vec iV;
    iV.x = v.x;
    iV.y = v.y;
    return iV;
}
void RotaeVec(Vec v, double CosTheta, int direction, Vec *rv, double len)
{
    rv->x = v.x*CosTheta - v.y*(1 - CosTheta*CosTheta);
    rv->y = v.x*(1 - CosTheta*CosTheta) + v.y*CosTheta;
    double vlen = sqrt(rv->x*rv->x + rv->y*rv->y);
    rv->x = len*rv->x/vlen;
    rv->y = len*rv->y/vlen;
}
void FindPoint(Point p, Vec v, Point* np)
{
    np->x = p.x + v.x;
    np->y = p.y + v.y;
}
int CheckCross(Circle c1, Segment s1)
{
    int cross = 0;
    /*ToDo*/
    return cross;
}
double FindCutLine(int r1, int r2)
{
    int i, j;
    /*Radius1 < Radius2*/
    double c1c2 = sqrt((circles[r2].center.y - circles[r1].center.y)*(circles[r2].center.y - circles[r1].center.y) + (circles[r2].center.x - circles[r1].center.x)*(circles[r2].center.x - circles[r1].center.x));
    double CosTheta;
    if(circles[r1].direction * circles[r2].direction > 0) {
        /*Outer Common Tangent */
        double CosTheta = (fabs(circles[r2].radius - circles[r1].radius))/c1c2;
    }
    else {
        /*Inner Common Tangent */
        double CosTheta = (circles[r2].radius + circles[r1].radius)/c1c2;
    }
    Vec r2r1;
    r2r1.x =  circles[r2].center.x - circles[r1].center.x;
    r2r1.y =  circles[r2].center.y - circles[r1].center.y;
    Vec oV1, iV2;
    RotaeVec(r2r1, CosTheta, circles[r2].direction, &iV2, circles[r2].radius);
    RotaeVec(InverseVec(r2r1), CosTheta, -circles[r1].direction, &oV1, circles[r1].radius);
    Point iP2, oP1;
    FindPoint(circles[r2].center, iV2, &iP2);
    FindPoint(circles[r1].center, oV1, &oP1);
    FindLine(oP1, iP2, &edgeTable[r1][r2].cutLine);
    /*Check Valid*/
    int valid = 1;
    if(edgeTable[r1][r2].cutLine.len >= limit-errorT) valid = 0;
    for(i = 0; i < circleNumber && valid == 1; i++){
        if(CheckCross(circles[i], edgeTable[r1][r2].cutLine) == 1) valid = 0;
    }
    if(valid == 1) {
        edgeTable[r1][r2].isValid = 1;
        return edgeTable[r1][r2].cutLine.len;
    }
    else {
        edgeTable[r1][r2].isValid = 0;
        return 0;
    }
}
void FindEdges()
{
    int i, j;
    for(i = 0; i < circleNumber; i++){
        for(j = 0; j < circleNumber; j++){
            if(i == j) continue;
            shortestPath[i][j] = FindCutLine(i, j);
        }
    }
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
        scanf("%d", &circleNumber);
        if(circleNumber == 0) break;
        Read();
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        /*Preprocessing*/
        /*Find all edges for circles*/
        FindEdges();
        /*Calculate APSP*/
        for(k = 0; k < circleNumber; k++){
            for(i = 0; i < circleNumber; i++){
                for(j = 0; j < circleNumber; j++){
                    if(shortestPath[i][k] + shortestPath[k][j] < shortestPath[i][j])
                        shortestPath[i][j] = shortestPath[i][k] + shortestPath[k][j];
                }
            }
        }
        /*Start Branch and Bound, use APSP to estimate possible cut*/

        caseNumber++;
    }
    return 0;
}
