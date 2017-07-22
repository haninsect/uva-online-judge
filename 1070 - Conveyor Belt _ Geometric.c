/*1070 - Conveyor Belt _ Geometric */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM_Vec1
#define DBUGM_Line1
#define DBUGM_BAB1
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.00000000001
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
    int direction; /*-1 clockwise, 1 counterclockwise*/
} Circle;

typedef struct {
    int isValid;
    Segment cutLine;
} Edge;

int circleNumber;
Circle circles[MaxcircleNumber];
int start, end;
double limit;
Edge edgeTable[MaxcircleNumber][MaxcircleNumber];
int shortestPath[MaxcircleNumber][MaxcircleNumber];
double minLen;
void ShowVec(Vec v)
{
    printf("   Vec: (%2.2lf %2.2lf )\n", v.x, v.y);
}
void ShowCircle(Circle c1)
{
    printf("Circle direction: %d\n", c1.direction);
    ShowPoint(c1.center);
    printf("   Radius: %2.2lf\n", c1.radius);
}
void ShowLine(Segment edgeA)
{
    printf("Line: (%2.2lf, %2.2lf) -> (%2.2lf, %2.2lf): %lf\n", edgeA.x1, edgeA.y1, edgeA.x2, edgeA.y2, edgeA.len);
}
void ShowPoint(Point v)
{
    printf("   Point: (%2.2lf %2.2lf )\n", v.x, v.y);
}
int Max(int c1, int c2)
{
    if(circles[c1].radius > circles[c2].radius-errorT) return c1;
    else return c2;
}
int Min(int c1, int c2)
{
    if(circles[c1].radius < circles[c2].radius-errorT) return c1;
    else return c2;
}
void Read()
{
    int i, j;
    minLen = LargeINT;
    for(i = 0; i < circleNumber; i++){
        char c[5];
        scanf("%lf%lf%lf%s", &circles[i].center.x, &circles[i].center.y, &circles[i].radius, &c);
        if(strcmp(c, "CC") == 0) circles[i].direction = 1;
        else if(strcmp(c, "C") == 0) circles[i].direction = -1;
        else {
            printf("Input Error\n");
            exit(1);
        }
    }
    scanf("%d%d%lf", &start, &end, &limit);
    /*printf("%d %d %lf\n", start, end, limit);*/

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
    iV.x = -v.x;
    iV.y = -v.y;
    return iV;
}
void RotaeVec(Vec v, double CosTheta, int direction, Vec *rv, double len)
{

    double SinTheta = sqrt(1 - CosTheta*CosTheta) * direction;
    rv->x = v.x*CosTheta - v.y*SinTheta;
    rv->y = v.x*SinTheta + v.y*CosTheta;
    #ifdef DBUGM_Vec
        printf("\n---- Rotate Vector ----\n");
        printf("cos: %2.2lf, sin: %lf, radius: %2.2lf\n", CosTheta, sin(acos(CosTheta)), len);
        ShowVec(v);
        ShowVec(*rv);
    #endif
    double vlen = sqrt(rv->x*rv->x + rv->y*rv->y);
    rv->x = len*(rv->x/vlen);
    rv->y = len*(rv->y/vlen);
    #ifdef DBUGM_Vec
        ShowVec(*rv);
    #endif
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
void CheckValid(int r1, int r2)
{
    /*Check Valid*/
    int i, j;
    int valid = 1;
    if(edgeTable[r1][r2].cutLine.len > limit+errorT) valid = 0;
    for(i = 0; i < circleNumber && valid == 1; i++){
        if(CheckCross(circles[i], edgeTable[r1][r2].cutLine) == 1) valid = 0;
    }
    if(valid == 1) {
        edgeTable[r1][r2].isValid = 1;
        shortestPath[r1][r2] = edgeTable[r1][r2].cutLine.len;
    }
    else {
        edgeTable[r1][r2].isValid = 0;
        shortestPath[r1][r2] = 0;
    }
    #ifdef DBUGM_Line
        printf("Find Common Tangent %d -> %d, valid: %d\n", r1, r2, edgeTable[r1][r2].isValid);
        ShowCircle(circles[r1]);
        ShowCircle(circles[r2]);
        ShowLine(edgeTable[r1][r2].cutLine);
    #endif

}
double FindCommonTangent(int r1, int r2)
{
    int i, j;
    /*Radius1 < Radius2*/
    double c1c2 = sqrt((circles[r2].center.y - circles[r1].center.y)*(circles[r2].center.y - circles[r1].center.y) + (circles[r2].center.x - circles[r1].center.x)*(circles[r2].center.x - circles[r1].center.x));
    Vec oV1, iV2, oV2, iV1;
    if(circles[r1].direction * circles[r2].direction > 0) {
        /*Outer Common Tangent */
        double CosTheta = (fabs(circles[r2].radius - circles[r1].radius))/c1c2;
        Vec r2r1;
        r2r1.x =  circles[r1].center.x - circles[r2].center.x;
        r2r1.y =  circles[r1].center.y - circles[r2].center.y;
        RotaeVec(r2r1, CosTheta, circles[r2].direction, &iV2, circles[r2].radius);
        RotaeVec(r2r1, CosTheta, circles[r2].direction, &oV1, circles[r1].radius);
        RotaeVec(r2r1, CosTheta, -circles[r2].direction, &oV2, circles[r2].radius);
        RotaeVec(r2r1, CosTheta, -circles[r2].direction, &iV1, circles[r1].radius);
    }
    else {
        /*Inner Common Tangent */
        double CosTheta = (circles[r2].radius + circles[r1].radius)/c1c2;
        Vec r2r1;
        r2r1.x =  circles[r1].center.x - circles[r2].center.x;
        r2r1.y =  circles[r1].center.y - circles[r2].center.y;
        RotaeVec(r2r1, CosTheta, circles[r2].direction, &iV2, circles[r2].radius);
        RotaeVec(InverseVec(r2r1), CosTheta, -circles[r1].direction, &oV1, circles[r1].radius);
        RotaeVec(r2r1, CosTheta, -circles[r2].direction, &oV2, circles[r2].radius);
        RotaeVec(InverseVec(r2r1), CosTheta, circles[r1].direction, &iV1, circles[r1].radius);
    }
    Point iP2, oP1, iP1, oP2;
    FindPoint(circles[r2].center, iV2, &iP2);
    FindPoint(circles[r1].center, oV1, &oP1);
    FindLine(oP1, iP2, &edgeTable[r1][r2].cutLine);
    CheckValid(r1, r2);
    FindPoint(circles[r2].center, oV2, &oP2);
    FindPoint(circles[r1].center, iV1, &iP1);
    FindLine(oP2, iP1, &edgeTable[r2][r1].cutLine);
    CheckValid(r2, r1);
}
void FindEdges()
{
    int i, j;
    for(i = 0; i < circleNumber-1; i++){
        for(j = i+1; j < circleNumber; j++){
            if(fabs(circles[i].radius - circles[j].radius) < 0) FindCommonTangent(i, j);
            else FindCommonTangent(Min(i, j), Max(i, j));
        }
    }
}
int FindIntersection(Segment edgeA, Segment edgeB)
{
    #ifdef DBUGM
        printf("\n---FindIntersection %2.2lf\n", fabs(((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1))));
        ShowLine(edgeA);
        ShowLine(edgeB);
    #endif

     if( fabs((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1)) < errorT ){
        return 0;
    }

    /*Compute the proportion from edgeA->P1 to the intersection*/
    /*ua = ((x4 - x3)(y1 - y3) - (y4-y3)(x1-x3)) / ((y4 - y3)(x2 - x1) - (x4 - x3)(y2 - y1))*/
    double ppB = ((edgeA.x2 - edgeA.x1)*(edgeB.y1 - edgeA.y1) - (edgeA.y2 - edgeA.y1)*(edgeB.x1 - edgeA.x1)) / ((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1));
    double ppA = ((edgeB.x2 - edgeB.x1)*(edgeA.y1 - edgeB.y1) - (edgeB.y2 - edgeB.y1)*(edgeA.x1 - edgeB.x1)) / ((edgeB.y2 - edgeB.y1)*(edgeA.x2 - edgeA.x1) - (edgeB.x2 - edgeB.x1)*(edgeA.y2 - edgeA.y1));
    #ifdef DBUGM
        printf("ppA, ppB: %2.2lf %2.2lf\n", ppA, ppB);
    #endif
    if(ppB > 1+errorT || ppB < 0-errorT || ppA > 1+errorT || ppA < 0-errorT) return 0;
    else return 1;
}
double CalculateArc(int c1, Segment s1, Segment s2)
{
    double theta1 = atan2(s1.y2 - circles[c1].center.y, s1.x2 - circles[c1].center.x)*180/M_PI;
    if(theta1 < 0-errorT) theta1 = theta1+360;
    double theta2 = atan2(s2.y1 - circles[c1].center.y, s2.x1 - circles[c1].center.x)*180/M_PI;
    if(theta2 < 0-errorT) theta2 = theta2+360;
    double delta;
    if(circles[c1].direction == 1){
        delta = theta2 - theta1;
        if(delta < 0) delta += 360;
    }
    else {
        delta = theta1 - theta2;
        if(delta < 0) delta += 360;
    }
    #ifdef DBUGM
        printf("%lf %lf\n", s1.x2 - circles[c1].center.x, s1.y2 - circles[c1].center.y);
        printf("%lf %lf\n", s2.x1 - circles[c1].center.x, s2.y1 - circles[c1].center.y);
        printf("%lf %lf\n", theta1, theta2);
    #endif
    return circles[c1].radius*2*M_PI*((delta)/360);
}
void BranchAndBound(int nowC, int valid[circleNumber], int edges[circleNumber][2], int chosed, double nowLen)
{
    #ifdef DBUGM_BAB
        printf("\n---- BranchAndBound: now: %d, target: %d chosed: %d len: %2.2lf\n", nowC, end, chosed, nowLen);
    #endif
    if(nowLen >= minLen) return;
    if(valid[end] == 0) {
        if(nowLen < minLen) minLen = nowLen;
        return;
    }
    int i, j;
    for(i = 0; i < circleNumber; i++){
        if(valid[i] == 0) continue;
        if(edgeTable[nowC][i].isValid == 0) continue;
        if(nowLen + shortestPath[nowC][i] >= minLen) continue;
        int flag = 1;
        for(j = 0; j < chosed && flag == 1; j++){
            if(FindIntersection(edgeTable[nowC][i].cutLine, edgeTable[edges[j][0]][edges[j][1]].cutLine) == 1) flag = 0;
        }
        if(flag == 0) continue;
        else {
            edges[chosed][0] = nowC;
            edges[chosed][1] = i;
            valid[i] = 0;
            double arc;
            if( nowC == start) arc = 0;
            else arc = CalculateArc(nowC,  edgeTable[edges[chosed-1][0]][edges[chosed-1][1]].cutLine, edgeTable[nowC][i].cutLine);
            BranchAndBound(i, valid, edges, chosed+1, nowLen + edgeTable[nowC][i].cutLine.len + arc);
            valid[i] = 1;
        }
    }
}
void Round(char output[256], double minLen)
{
    sprintf(output, "%.2lf", minLen);
    int len = strlen(output);
    if(output[len-1] == '0'){
        output[len-1] = '\0';
        output[len-2] = '\0';
        output[len-3] = '\0';
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
        int valid[circleNumber];
        for(i = 0; i < circleNumber; i++) valid[i] = 1;
        valid[start] = 0;
        int edges[circleNumber][2];
        BranchAndBound(start, valid, edges, 0, 0);
        if( fabs(minLen - LargeINT) < errorT || fabs(minLen - 0) < errorT ) printf("Case %d: Cannot reach destination shaft\n", caseNumber);
        else {
            char output[256] = {'\0'};
            Round(output, minLen);
            printf("Case %d: length = %s\n", caseNumber, output);
        }
        caseNumber++;
    }
    return 0;
}
