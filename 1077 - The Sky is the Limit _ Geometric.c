/*1077 - The Sky is the Limit _ Geometric*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxSize 105

typedef struct {
    double x1, x2;
    double y1, y2;
    double len;
} Segment;
typedef struct {
    Segment line[2];
} Triangle;
typedef struct {
    double ep;
    int isLeft;
} EndPoint;
typedef struct {
    double l, r;
} Interval;
Triangle tArray[MaxSize];
int size;
void ShowEdge(Segment edgeA)
{
    printf("(%.2lf, %.2lf) -> (%.2lf, %.2lf)\n", edgeA.x1, edgeA.y1, edgeA.x2, edgeA.y2);
}
int cmp(const void* a, const void *b)
{
    EndPoint *ap = (EndPoint*)a;
    EndPoint *bp = (EndPoint*)b;
    if(fabs(ap->ep - bp->ep) < errorT) return 0;
    else if(ap->ep > bp->ep) return 1;
    else return -1;
}
int FindIntersection(Segment edgeA, Segment edgeB, double *proportion)
{
    #ifdef DBUGM
        printf("\n---FindIntersection %2.2lf\n", fabs(((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1))));
        ShowEdge(edgeA);
        ShowEdge(edgeB);
    #endif

     if( fabs((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1)) < errorT ){
        if(edgeA.x1 == edgeB.x1 && edgeA.y1 == edgeB.y1)
            return -1;
        if(edgeA.x2 == edgeB.x2 && edgeA.y2 == edgeB.y2)
            return -1;
        return 0;
    }

    /*Compute the proportion from edgeA->P1 to the intersection*/
    /*ua = ((x4 - x3)(y1 - y3) - (y4-y3)(x1-x3)) / ((y4 - y3)(x2 - x1) - (x4 - x3)(y2 - y1))*/
    double ppB = ((edgeA.x2 - edgeA.x1)*(edgeB.y1 - edgeA.y1) - (edgeA.y2 - edgeA.y1)*(edgeB.x1 - edgeA.x1)) / ((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1));
    double ppA = ((edgeB.x2 - edgeB.x1)*(edgeA.y1 - edgeB.y1) - (edgeB.y2 - edgeB.y1)*(edgeA.x1 - edgeB.x1)) / ((edgeB.y2 - edgeB.y1)*(edgeA.x2 - edgeA.x1) - (edgeB.x2 - edgeB.x1)*(edgeA.y2 - edgeA.y1));
    #ifdef DBUGM
        printf("ppA, ppB: %2.2lf %2.2lf\n", ppA, ppB);
    #endif
    if(ppB > 1+errorT || ppB < 0-errorT || ppA > 1+errorT || ppA < 0-errorT) {
        return 0;
    }
    else {
        *proportion = ppA;
        return 1;
    }
}
void Read()
{
    int i;
    for(i = 0; i < size; i ++){
        double a, h, b;
        scanf("%lf%lf%lf", &a, &h, &b);
        tArray[i].line[0].x1 = a- b/2;
        tArray[i].line[0].y1 = 0;
        tArray[i].line[0].x2 = a;
        tArray[i].line[0].y2 = h;
        tArray[i].line[0].len = sqrt(b*b/2/2 + h*h);

        tArray[i].line[1].x1 = a;
        tArray[i].line[1].y1 = h;
        tArray[i].line[1].x2 = a+b/2;
        tArray[i].line[1].y2 = 0;
        tArray[i].line[1].len = sqrt(b*b/2/2 + h*h);
    }

}
int CoverCheck(int i, int j)
{
    if(tArray[i].line[0].x1 <= tArray[j].line[0].x1-errorT && tArray[i].line[1].x2 >= tArray[j].line[1].x2+errorT && tArray[i].line[0].y2 >= tArray[j].line[0].y2+errorT){
        double lx = tArray[j].line[0].y2/tArray[i].line[0].y2 * (tArray[i].line[0].x2 - tArray[i].line[0].x1) + tArray[i].line[0].x1;
        double rx = tArray[i].line[1].x2 - tArray[j].line[0].y2/tArray[i].line[0].y2 * (tArray[i].line[0].x2 - tArray[i].line[0].x1);
        if( lx-errorT <= tArray[j].line[0].x2 && rx+errorT >= tArray[j].line[0].x2)
            return 1;
        else
            return 0;
    }
    return 0;
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
        scanf("%d", &size);
        if(size == 0) break;
        Read();
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        /*Preprocessing, remove the useless triangles*/
        int duplicate[MaxSize] = {0};
        for(i = 0; i < size-1; i++){
            if(duplicate[i] == 1) continue;
            for(j = i+1; j < size; j++){
                if(duplicate[j] == 1) continue;
                /*Cover checking*/
                if(CoverCheck(i, j)) {
                    duplicate[j] = 1;
                    continue;
                }
                if(CoverCheck(j, i)) {
                    duplicate[i] = 1;
                    break;
                }
                /*Overlap checking*/
                double a;
                int lf = FindIntersection(tArray[i].line[0], tArray[j].line[0], &a);
                int rt = FindIntersection(tArray[i].line[1], tArray[j].line[1], &a);
                if(lf < 0 && rt < 0) {
                    duplicate[j] = 1;
                    continue;
                }
                else if( (lf < 0 && rt >= 0) || (lf >= 0 && rt < 0)) {
                    if (tArray[i].line[0].y2 < tArray[j].line[0].y2) {
                        duplicate[i] = 1;
                        break;
                    }
                    else {
                        duplicate[j] = 1;
                        continue;
                    }
                }
            }
        }
        #ifdef DBUGM
            printf("Duplicate:\n", caseNumber);
            for(i = 0; i < size; i++) printf("dup %d: %d\n", i, duplicate[i]);
        #endif

        double len = 0;
        for(i = 0; i < size; i++){
            if(duplicate[i] == 1) continue;
            for(k = 0; k < 2; k++){
                EndPoint epts[MaxSize*2];
                int itn = 0;
                for(j = 0; j < size; j++){
                    if(i == j) continue;
                    if(duplicate[j] == 1) continue;
                    /*Find cut intervals*/
                    epts[itn*2].isLeft = 1;
                    epts[itn*2+1].isLeft = 0;
                    int lt = FindIntersection(tArray[i].line[k], tArray[j].line[0], &epts[itn*2].ep);
                    int rt = FindIntersection(tArray[i].line[k], tArray[j].line[1], &epts[itn*2 +1].ep);
                    if( lt == 0 && rt == 0 ) continue;
                    else if(lt == 0 && rt > 0){
                        if(k == 0){
                            epts[itn*2].ep = 0;
                            itn++;
                            continue;
                        }
                        else {
                            if(tArray[i].line[1].x2 > tArray[j].line[1].x2){
                                epts[itn*2].ep = 0;
                                itn++;
                                continue;
                            }
                            else {
                                epts[itn*2].ep = epts[itn*2 +1].ep;
                                epts[itn*2+1].ep = 1;
                                itn++;
                                continue;
                            }
                        }
                    }
                    else if(lt > 0 && rt == 0){
                        if(k == 1){
                            epts[itn*2 +1].ep = 1;
                            itn++;
                            continue;
                        }
                        else {
                            if(tArray[i].line[0].x1 < tArray[j].line[0].x1){
                                epts[itn*2 +1].ep = 1;
                                itn++;
                                continue;
                            }
                            else {
                                epts[itn*2 +1].ep = epts[itn*2].ep;
                                epts[itn*2].ep = 0;
                                itn++;
                                continue;
                            }
                        }
                    }
                    else if(lt > 0 && rt > 0){
                        itn++;
                        continue;
                    }
                    else {
                        printf("Undefined relation\n");
                        exit(1);
                    }
                }
                /*Union intervals*/
                qsort(epts, itn*2, sizeof(EndPoint), cmp);
                double cPart = 0;
                int counter = 0;
                double start = 0;
                for(j = 0; j < itn*2; j++){
                    if(epts[j].isLeft){
                        if(counter == 0) start = epts[j].ep;
                        counter++;
                    }
                    else {
                        if(counter == 1) cPart += epts[j].ep- start;
                        counter--;
                    }
                }
                len += (1-cPart)*tArray[i].line[k].len;
            }
        }
        printf("Case %d: %.0f\n\n", caseNumber, len);
        caseNumber++;
    }
    return 0;
}
