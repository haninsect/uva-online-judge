/*1035 - Tree-Lined Streets*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DBUGM
typedef struct {
    long long int x1, y1, x2, y2;
} edge;
int cmp(const void* a, const void* b);
double FindIntersection(edge* edgeA, edge* edgeB);
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
	int i, j, k , l;
    int caseNumber = 1;
    while(1){
        int n;
        scanf("%d", &n);
        if(n == 0) break;

        edge edges[n];
        for(i = 0; i < n; i++){
            scanf("%lld%lld%lld%lld", &edges[i].x1, &edges[i].y1, &edges[i].x2, &edges[i].y2);
        }
        /*for n -> 1,  find the intersection. Sort intersections. compute trees.*/
        int trees = 0;
        for(i = 0; i < n; i++){
            double intersections[n];
            int intsecNumber = 0;
            for(j = 0; j < n; j++){
                if(i == j) continue;
                if((intersections[intsecNumber] = FindIntersection(&edges[i], &edges[j])) > 0.000001 ) intsecNumber++;
            }
            double edgeLen;
            edgeLen = sqrt((edges[i].y2 - edges[i].y1)*(edges[i].y2 - edges[i].y1) + (edges[i].x2 - edges[i].x1)*(edges[i].x2 - edges[i].x1));

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
            if((int)((edgeLen*intersections[0] - 25)) >= 0)trees += (int)((edgeLen*intersections[0] - 25)/50 + 1);

            #ifndef DBUGM
                printf("Trees: %d\n", trees);
            #endif
            if((int)((edgeLen - edgeLen*intersections[intsecNumber-1] - 25)) >= 0)trees += (int)((edgeLen - edgeLen*intersections[intsecNumber-1] - 25)/50 + 1);

            /*Compute the rest*/
            for(j = 1; j < intsecNumber; j++){
                #ifndef DBUGM
                    printf("      Trees: %d\n", trees);
                #endif
                if((int)((edgeLen*intersections[j] - edgeLen*intersections[j-1] - 50)) >= 0)trees += (int)((edgeLen*intersections[j] - edgeLen*intersections[j-1] - 50)/50 + 1);
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


    #ifndef DBUGM
        printf("Line A: %dx + %dy + %d = 0\n", a1, b1, c1);
        printf("Line B: %dx + %dy + %d = 0\n", a2, b2, c2);
    #endif
    /*Compute the proportion from edgeA->P1 to the intersection*/
    /*ua = ((x4 - x3)(y1 - y3) - (y4-y3)(x1-x3)) / ((y4 - y3)(x2 - x1) - (x4 - x3)(y2 - y1))*/
    double c = (double)((edgeA->x2 - edgeA->x1)*(edgeB->y1 - edgeA->y1) - (edgeA->y2 - edgeA->y1)*(edgeB->x1 - edgeA->x1)) / (double)((edgeA->y2 - edgeA->y1)*(edgeB->x2 - edgeB->x1) - (edgeA->x2 - edgeA->x1)*(edgeB->y2 - edgeB->y1));
    double r = (double)((edgeB->x2 - edgeB->x1)*(edgeA->y1 - edgeB->y1) - (edgeB->y2 - edgeB->y1)*(edgeA->x1 - edgeB->x1)) / (double)((edgeB->y2 - edgeB->y1)*(edgeA->x2 - edgeA->x1) - (edgeB->x2 - edgeB->x1)*(edgeA->y2 - edgeA->y1));
    if(c >= 1 || c <= 0 || r >= 1 || r <= 0) return 0.0;
    else return r;
}

int cmp(const void* a, const void* b)
{
    return *(double*)a > *(double*)b;
}
