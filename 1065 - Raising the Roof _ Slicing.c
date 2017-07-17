/*1065 - Raising the Roof _ Slicing */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define MaxVertex 3000
#define MaxSlice 10000
#define errorT 0.00005
#define GetCase 500000
#define M_PI 3.14159265358979323846
int vertexNumber = 0, triangleNumber = 0;
typedef struct {
    double x, y, z;
} Vec;
typedef struct {
    double x, y, z;
    int triangle;
} Vertex;
typedef struct {
    double x1, x2;
    double y1, y2;
    double z1, z2;
} Segment;
typedef struct {
    int vIndex[3];
    Segment lines[3];
    /*ax + by + cz + d = 0*/
    double a, b, c, d;
    double theta;
} Triangle;
typedef struct {
    double x;
    Vertex v[MaxVertex];
} Slice;
Vertex vArray[300];
Triangle tArray[1000];

double sliceX[MaxVertex*MaxVertex];
int sliceNumber = 0;
Slice slices[MaxSlice];
double VecLen(Vec v)
{
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
void Cross(Vec a, Vec b, Vec c)
{
    c.x = a.y*b.z - a.z*b.y;
    c.y = a.z*b.x - a.x*b.z;
    c.z = a.x*b.y - a.y*b.x;
    if(c.z < 0-errorT){
        c.x *= -1;
        c.y *= -1;
        c.z *= -1;
    }
}
void GetVec(Vertex a, Vertex b, Vec v)
{
    v.x = b.x - a.x;
    v.y = b.x - a.y;
    v.z = b.x - a.z;
}
void FindPlane(Triangle t)
{
    Vec a, b, c;
    GetVec(vArray[t.vIndex[0]], vArray[t.vIndex[1]], a);
    GetVec(vArray[t.vIndex[0]], vArray[t.vIndex[2]], b);
    Cross(a, b, c);
    double d = -(c.x*vArray[t.vIndex[0]].x + c.y*vArray[t.vIndex[0]].y + c.z*vArray[t.vIndex[0]].z);
    t.a = c.x;
    t.b = c.y;
    t.c = c.z;
    t.d = d;
    if(fabs(t.c - 0) < errorT ) t.theta = M_PI*90/360;
    t.theta = acos(fabs(t.c)/VecLen(c));

}
void FindLine(Vertex a, Vertex b, Segment line)
{
    line.x1 = a.x;
    line.y1 = a.y;
    line.z1 = a.z;
    line.x2 = b.x;
    line.y2 = b.y;
    line.z2 = b.z;
}
void Read(int caseNumber)
{
    #ifdef DBUGM
        printf("----------- Read-----------\n");
    #endif
    int i, j, k;
    sliceNumber = 0;
    for(i = 0; i < vertexNumber; i++){
        scanf("%d%d%d", &vArray[i].x, &vArray[i].y, &vArray[i].z);
    }
    for(i = 0; i < triangleNumber; i++){
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        tArray[i].vIndex[0] = a-1;
        tArray[i].vIndex[1] = b-1;
        tArray[i].vIndex[2] = c-1;
        FindLine(vArray[tArray[i].vIndex[0]], vArray[tArray[i].vIndex[1]], tArray[i].lines[0]);
        FindLine(vArray[tArray[i].vIndex[1]], vArray[tArray[i].vIndex[2]], tArray[i].lines[1]);
        FindLine(vArray[tArray[i].vIndex[2]], vArray[tArray[i].vIndex[0]], tArray[i].lines[2]);
        FindPlane(tArray[i]);
    }
}
int FindIntersection(Segment edgeA, Segment edgeB, Vertex v)
{

    /*Compute the proportion from edgeA->P1 to the intersection*/
    /*ua = ((x4 - x3)(y1 - y3) - (y4-y3)(x1-x3)) / ((y4 - y3)(x2 - x1) - (x4 - x3)(y2 - y1))*/
    double c = (double)((edgeA.x2 - edgeA.x1)*(edgeB.y1 - edgeA.y1) - (edgeA.y2 - edgeA.y1)*(edgeB.x1 - edgeA.x1)) / (double)((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1));
    double r = (double)((edgeB.x2 - edgeB.x1)*(edgeA.y1 - edgeB.y1) - (edgeB.y2 - edgeB.y1)*(edgeA.x1 - edgeB.x1)) / (double)((edgeB.y2 - edgeB.y1)*(edgeA.x2 - edgeA.x1) - (edgeB.x2 - edgeB.x1)*(edgeA.y2 - edgeA.y1));
    #ifdef DBUGM
       printf("r, c: %lf %lf\n", r, c);
    #endif
    if(c > 1-errorT || c < 0+errorT || r > 1-errorT || r < 0+errorT) return 0;
    else {
        v.x = (edgeA.x2 - edgeA.x1)*r + edgeA.x1;
        return 1;
    }
}
int cmp(const void *a, const void *b)
{
    return *(double*)a - *(double*)b;
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
        scanf("%d%d", &vertexNumber, &triangleNumber);
        if(caseNumber == GetCase) printf("%d %d %d\n", vertexNumber, triangleNumber);
        if(vertexNumber == 0) break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        Read(caseNumber);
        /*Find Slices*/
        for(i = 0; i < triangleNumber; i++){
            /*3 edge*/
            for(j = 0; j < 3; j++){
                sliceX[sliceNumber++] = vArray[tArray[i].vIndex[j]].x;
            }
            /*The intersection with other edge*/
            for(j = 0; j < triangleNumber; j++){
                if(i == j) continue;
                for(k = 0; k < 3; k++){
                    Vertex tmp;
                    if(FindIntersection(tArray[i].lines[0], tArray[j].lines[k], tmp))sliceX[sliceNumber++] = tmp.x;
                    if(FindIntersection(tArray[i].lines[1], tArray[j].lines[k], tmp))sliceX[sliceNumber++] = tmp.x;
                    if(FindIntersection(tArray[i].lines[2], tArray[j].lines[k], tmp))sliceX[sliceNumber++] = tmp.x;
                }
            }
        }
        qsort(sliceX, sliceNumber, sizeof(double), cmp);
        int tmp = sliceNumber;
        sliceNumber = 0;
        double now = -1;
        for(i = 0; i < tmp; i++){
            if(fabs(sliceX[i] - now) < errorT ) continue;
            else {
                now = sliceX[i];
                slices[sliceNumber++].x = now;
            }
        }
        if(sliceNumber >= MaxSlice) while(1);
        /*For each slice , sort vertexes on it*/
        /*Between two slices, merge vertexes to get area.*/
            /*When a new vertex(triangle) coming , compare it to the top triangle. only calculate the top one's area*/
            /*When a vertex leave, if it is the top one, extract max to find another top one*/
        double area = 0;
        printf("Case %d: %.2lf\n", caseNumber, area);
        caseNumber++;
    }
    return 0;
}
