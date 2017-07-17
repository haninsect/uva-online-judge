/*1065 - Raising the Roof _ Slicing */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM
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
    int kind; /*0 left point, 1 intersection, 2 right point*/
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
int svNumber[MaxSlice];

double VecLen(Vec v)
{
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
void Cross(Vec a, Vec b, Vec *c)
{
    c->x = a.y*b.z - a.z*b.y;
    c->y = a.z*b.x - a.x*b.z;
    c->z = a.x*b.y - a.y*b.x;
    if(c->z < 0-errorT){
        c->x *= -1;
        c->y *= -1;
        c->z *= -1;
    }
}
void GetVec(Vertex a, Vertex b, Vec v)
{
    v.x = b.x - a.x;
    v.y = b.x - a.y;
    v.z = b.x - a.z;
}
void FindPlane(Triangle* t)
{
    Vec a, b, c;
    GetVec(vArray[t->vIndex[0]], vArray[t->vIndex[1]], a);
    GetVec(vArray[t->vIndex[0]], vArray[t->vIndex[2]], b);
    Cross(a, b, &c);
    double d = -(c.x*vArray[t->vIndex[0]].x + c.y*vArray[t->vIndex[0]].y + c.z*vArray[t->vIndex[0]].z);
    t->a = c.x;
    t->b = c.y;
    t->c = c.z;
    t->d = d;
    if(fabs(t->c - 0) < errorT ) t->theta = M_PI*90/360;
    t->theta = acos(fabs(t->c)/VecLen(c));

}
void FindLine(Vertex a, Vertex b, Segment* line)
{
    if(a.x - b.x < 0+errorT){
        line->x1 = a.x;
        line->y1 = a.y;
        line->z1 = a.z;
        line->x2 = b.x;
        line->y2 = b.y;
        line->z2 = b.z;
    }
    else {
        line->x1 = b.x;
        line->y1 = b.y;
        line->z1 = b.z;
        line->x2 = a.x;
        line->y2 = a.y;
        line->z2 = a.z;
    }

}
void Read(int caseNumber)
{
    #ifdef DBUGM
        printf("----------- Read-----------\n");
    #endif
    int i, j, k;
    sliceNumber = 0;
    for(i = 0; i < vertexNumber; i++){
        scanf("%lf%lf%lf", &vArray[i].x, &vArray[i].y, &vArray[i].z);
    }
    for(i = 0; i < triangleNumber; i++){
        int a, b, c;
        scanf("%d%d%d", &a, &b, &c);
        tArray[i].vIndex[0] = a-1;
        tArray[i].vIndex[1] = b-1;
        tArray[i].vIndex[2] = c-1;
        FindLine(vArray[tArray[i].vIndex[0]], vArray[tArray[i].vIndex[1]], &tArray[i].lines[0]);
        FindLine(vArray[tArray[i].vIndex[1]], vArray[tArray[i].vIndex[2]], &tArray[i].lines[1]);
        FindLine(vArray[tArray[i].vIndex[2]], vArray[tArray[i].vIndex[0]], &tArray[i].lines[2]);
        FindPlane(&tArray[i]);
    }
}
void ShowLine(Segment edgeA)
{
    printf("Line: (%2.2lf, %2.2lf %2.2lf) -> (%2.2lf, %2.2lf %2.2lf)\n", edgeA.x1, edgeA.y1, edgeA.z1, edgeA.x2, edgeA.y2, edgeA.z2);
}
int FindIntersection(Segment edgeA, Segment edgeB, Vertex *v)
{
    #ifdef DBUGM
        printf("\n---FindIntersection %2.2lf\n", fabs(((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1))));
        ShowLine(edgeA);
        ShowLine(edgeB);
    #endif

     if( fabs(((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1))) < errorT ){
        return 0;
    }

    /*Compute the proportion from edgeA->P1 to the intersection*/
    /*ua = ((x4 - x3)(y1 - y3) - (y4-y3)(x1-x3)) / ((y4 - y3)(x2 - x1) - (x4 - x3)(y2 - y1))*/
    double ppB = ((edgeA.x2 - edgeA.x1)*(edgeB.y1 - edgeA.y1) - (edgeA.y2 - edgeA.y1)*(edgeB.x1 - edgeA.x1)) / ((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1));
    double ppA = ((edgeB.x2 - edgeB.x1)*(edgeA.y1 - edgeB.y1) - (edgeB.y2 - edgeB.y1)*(edgeA.x1 - edgeB.x1)) / ((edgeB.y2 - edgeB.y1)*(edgeA.x2 - edgeA.x1) - (edgeB.x2 - edgeB.x1)*(edgeA.y2 - edgeA.y1));
    #ifdef DBUGM
        printf("ppA, c: %2.2lf %2.2lf\n", ppA, ppB);
    #endif
    if(ppB > 1+errorT || ppB < 0-errorT || ppA > 1+errorT || ppA < 0-errorT) return 0;
    else {
        v->x = (edgeA.x2 - edgeA.x1)*ppA + edgeA.x1;
        v->y = (edgeA.y2 - edgeA.y1)*ppA + edgeA.y1;
        v->z = (edgeA.z2 - edgeA.z1)*ppA + edgeA.z1;
        #ifdef DBUGM
            printf("Intersection: (%2.2lf, %2.2lf %2.2lf)\n", v->x, v->y, v->z);
        #endif
        return 1;
    }
}
int cmp(const void *a, const void *b)
{
    return *(double*)a - *(double*)b;
}
int cmpVertex(const void *a, const void *b)
{
    Vertex *ap = (Vertex*)a, *bp = (Vertex*)b;
    return bp->y - ap->y;
}

void ShowTriangle()
{
    int i, j;
    for(i = 0; i < triangleNumber; i++){
        for(j = 0; j < 3; j++) printf("%d: (%2.2lf %2.2lf %2.2lf)\n", tArray[i].vIndex[j], vArray[tArray[i].vIndex[j]].x, vArray[tArray[i].vIndex[j]].y, vArray[tArray[i].vIndex[j]].z);
        ShowLine(tArray[i].lines[0]);
        ShowLine(tArray[i].lines[1]);
        ShowLine(tArray[i].lines[2]);
    }
}
double CalculateOriginalArea(Segment s1, Segment s2, double height, int triN)
{
    if( fabs(tArray[triN].theta-90) < errorT) return 0;
    double f = sqrt( (s1.x1 - s2.x1)*(s1.x1 - s2.x1) + (s1.y1 - s2.y1)*(s1.y1 - s2.y1) + (s1.z1 - s2.z1)*(s1.z1 - s2.z1));
    double c = sqrt( (s1.x2 - s2.x2)*(s1.x2 - s2.x2) + (s1.y2 - s2.y2)*(s1.y2 - s2.y2) + (s1.z2 - s2.z2)*(s1.z2 - s2.z2));
    return fabs( (f + c * height)/cos(tArray[triN].theta)/2);
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
        #ifdef DBUGM
            printf("Triangles:\n");
            ShowTriangle();
            printf("Start Slice:\n");
        #endif
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
                    if(FindIntersection(tArray[i].lines[0], tArray[j].lines[k], &tmp))sliceX[sliceNumber++] = tmp.x;
                    if(FindIntersection(tArray[i].lines[1], tArray[j].lines[k], &tmp))sliceX[sliceNumber++] = tmp.x;
                    if(FindIntersection(tArray[i].lines[2], tArray[j].lines[k], &tmp))sliceX[sliceNumber++] = tmp.x;
                }
            }
        }
        if(sliceNumber >= MaxVertex*MaxVertex)while(1);
        #ifdef DBUGM
            printf("\n---Slice X:\n");
            for(i = 0; i < sliceNumber; i++) printf("%2.2lf ", sliceX[i]);
            printf("\n");
        #endif
        qsort(sliceX, sliceNumber, sizeof(double), cmp);

        int tmpn = sliceNumber;
        sliceNumber = 0;
        double now = -1;
        for(i = 0; i < tmpn; i++){
            if(fabs(sliceX[i] - now) < errorT ) continue;
            else {
                now = sliceX[i];
                slices[sliceNumber++].x = now;
            }
        }
        if(sliceNumber >= MaxSlice) while(1);
        /*For each slice , sort vertexes on it*/
        for(i = 0; i < sliceNumber; i++){
            svNumber[i] = 0;
            Segment tmpl;
            tmpl.x1 = slices[i].x;
            tmpl.y1 = -200;
            tmpl.x2 = slices[i].x;
            tmpl.y2 = 200;
            tmpl.z1 = 0, tmpl.z2 = 0;
            for(j = 0; j < triangleNumber; j++){
                for(k = 0; k < 3; k++){
                    if(FindIntersection(tmpl, tArray[j].lines[k], &slices[i].v[svNumber[i]])) {
                        slices[i].v[svNumber[i]].triangle = j;
                        if(fabs(slices[i].x - tArray[j].lines[k].x1) < errorT) slices[i].v[svNumber[i]].kind = 0;
                        else if(fabs(slices[i].x - tArray[j].lines[k].x2) < errorT) slices[i].v[svNumber[i]].kind = 2;
                        else slices[i].v[svNumber[i]].kind = 1;
                        svNumber[i]++;
                        if(svNumber[i] >= MaxVertex)while(1);
                    }
                }
            }
            qsort(slices[i].v, svNumber[i], sizeof(Vertex), cmpVertex);
        }
        #ifdef DBUGM
        printf("\n------ Slice Vertex ------\n");
        for(i = 0; i < sliceNumber; i++){
            printf("Slice %d: x = %lf\n", i, slices[i].x);
            for(j = 0; j < svNumber[i]; j++){
                printf("   (%lf %lf %lf) Triangle-k: %d-%d \n", slices[i].v[j].x, slices[i].v[j].y, slices[i].v[j].z, slices[i].v[j].triangle, slices[i].v[j].kind);
            }
        }
        #endif
        /*Between two slices, merge vertexes to get area.*/
            /*When a new vertex(triangle) coming , compare it to the top triangle. only calculate the top one's area*/
            /*When a vertex leave, if it is the top one, extract max to find another top one*/
        double area = 0;
        for(i = 0; i < sliceNumber-1; i++){
            printf("------ Slice %d -> %d ------\n", i, i+1);
            int lp = -1, rp = -1;
            int valid[triangleNumber];
            for(j = 0; j < triangleNumber; j++) valid[j] = 0;
            int top =-1;
            Segment tmps1, tmps2;;
            while(1){
                /*Get new vertex*/
                while(++lp < svNumber[i]){
                    if(slices[i].v[lp].kind != 2) break;
                }
                while(++rp < svNumber[i+1]){
                    if(slices[i+1].v[rp].kind != 0) break;
                }
                printf("left right: %d/%d %d/%d, TL TR: %d %d\n", lp, svNumber[i], rp, svNumber[i+1], slices[i].v[lp].triangle, slices[i+1].v[rp].triangle);
                if(lp == svNumber[i] || rp == svNumber[i+1]) break;
                if(slices[i].v[lp].triangle != slices[i+1].v[rp].triangle) while(1)printf("lp != rp!!!\n"),exit(1);
                if(valid[j] == 1){
                    valid[j] == 0;
                    continue;
                }
                if(top == -1) {
                    top = rp;
                    valid[rp] = 1;
                    FindLine(slices[i].v[lp], slices[i+1].v[rp], &tmps1);
                }
                else if(top == rp){
                    FindLine(slices[i].v[lp], slices[i+1].v[rp], &tmps2);
                    area += CalculateOriginalArea(tmps1, tmps2, slices[i+1].x - slices[i].x, top);
                    /*Extract max*/
                    for(j = 0; j < triangleNumber; j++) if(valid[j] == 1){
                        top = j;
                        break;
                    }
                }
                else {
                    valid[rp] = 1;
                    /*Compare to the top*/
                    top = rp;
                }
            }
        }
        printf("Case %d: %.2lf\n", caseNumber, area);
        caseNumber++;
    }
    return 0;
}
