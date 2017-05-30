/*1035 - Tree-Lined Streets*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DBUGM
typedef struct {
    int x1, y1, x2, y2;
} edge;
int cmp(const void* a, const void* b);
double FindIntersection(edge* edgeA, edge* edgeB);
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
	int i, j, k , l;
    int caseNumber = 1;
    while(1){
        #ifndef DBUGM
           printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int n;
        scanf("%d", &n);
        if(n == 0) break;

        edge edges[n];
        for(i = 0; i < n; i++){
            scanf("%d %d %d %d", &edges[i].x1, &edges[i].y1, &edges[i].x2, &edges[i].y2);
        }
        /*for n -> 1,  find the intersection. Sort intersections. compute trees.*/
        int trees = 0;
        for(i = 0; i < n; i++){
            double intersections[n];
            int intsecNumber = 0;
            for(j = 0; j < n; j++){
                if(i == j) continue;
                if((intersections[intsecNumber] = FindIntersection(&edges[i], &edges[j])) > 0.001 ) intsecNumber++;
            }
            double edgeLen;
            if(edges[i].x2 - edges[i].x1 == 0) edgeLen = abs(edges[i].y2 - edges[i].y1);
            else edgeLen = abs(edges[i].x2 - edges[i].x1) / cos(atan( (double)(edges[i].y2 - edges[i].y1) / (double)(edges[i].x2 - edges[i].x1) ));
            #ifndef DBUGM
                printf("edgeLen: %lf\n", edgeLen);
            #endif
            if(intsecNumber == 0){
                trees += (int)(edgeLen/50 + 1);
                continue;
            }

            qsort(intersections, intsecNumber, sizeof(double), cmp);
            #ifndef DBUGM
            for(j = 0; j < intsecNumber; j++){
                printf("%lf:(%lf, %lf) \n", intersections[j], edges[i].x1+intersections[j]*(edges[i].x2 - edges[i].x1), edges[i].y1+intersections[j]*(edges[i].y2 - edges[i].y1));
            }
            #endif
            /*Compute the first and last one*/
            #ifndef DBUGM
                printf("Trees: %d\n", trees);
            #endif
            if((int)((edgeLen*intersections[0] - 25)/50 + 1) > 0)trees += (int)((edgeLen*intersections[0] - 25)/50 + 1);

            #ifndef DBUGM
                printf("Trees: %d\n", trees);
            #endif
            if((int)((edgeLen - edgeLen*intersections[intsecNumber-1] - 25)/50 + 1) > 0)trees += (int)((edgeLen - edgeLen*intersections[intsecNumber-1] - 25)/50 + 1);

            /*Compute the rest*/
            for(j = 1; j < intsecNumber; j++){
                #ifndef DBUGM
                    printf("      Trees: %d\n", trees);
                #endif

                if((int)((edgeLen*intersections[j] - edgeLen*intersections[j-1] - 50)/50 + 1) > 0)trees += (int)((edgeLen*intersections[j] - edgeLen*intersections[j-1] - 50)/50 + 1);
            }

        }
        printf("Map %d\n", caseNumber);
        printf("Trees = %d\n", trees);
        caseNumber++;
    }

    return 0;
}

double FindIntersection(edge* edgeA, edge* edgeB)
{
    /*no intersection return 0*/
    /*-(y2 - y1)x + (x2-x1)y + (y2-y1)x1 - (x2 - x1) y1 = 0*/
    int a1 = -(edgeA->y2 - edgeA->y1), b1 = edgeA->x2 - edgeA->x1, c1 = (edgeA->y2 - edgeA->y1)*edgeA->x1 - (edgeA->x2 - edgeA->x1)*edgeA->y1;
    int a2 = -(edgeB->y2 - edgeB->y1), b2 = edgeB->x2 - edgeB->x1, c2 = (edgeB->y2 - edgeB->y1)*edgeB->x1 - (edgeB->x2 - edgeB->x1)*edgeB->y1;
    int sign1, sign2;
    #ifndef DBUGM
       // printf("Line A: %dx + %dy + %d = 0\n", a1, b1, c1);
       // printf("Line B: %dx + %dy + %d = 0\n", a2, b2, c2);
    #endif
    sign1  = a1*edgeB->x1 + b1*edgeB->y1 + c1;
    sign2  = a1*edgeB->x2 + b1*edgeB->y2 + c1;
    if(sign1 * sign2 >= 0) return 0.0;
    sign1  = a2*edgeA->x1 + b2*edgeA->y1 + c2;
    sign2  = a2*edgeA->x2 + b2*edgeA->y2 + c2;
    if(sign1 * sign2 >= 0) return 0.0;


    /*Compute length edgeA->P1 to intersection*/
    /*ua = ((x4 - x3)(y1 - y3) - (y4-y3)(x1-x3)) / ((y4 - y3)(x2 - x1) - (x4 - x3)(y2 - y1))*/


    return (double)((edgeB->x2 - edgeB->x1)*(edgeA->y1 - edgeB->y1) - (edgeB->y2 - edgeB->y1)*(edgeA->x1 - edgeB->x1)) / (double)((edgeB->y2 - edgeB->y1)*(edgeA->x2 - edgeA->x1) - (edgeB->x2 - edgeB->x1)*(edgeA->y2 - edgeA->y1));
}

int cmp(const void* a, const void* b)
{
    return *(double*)a > *(double*)b;
}
