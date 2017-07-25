/*1074 - Net Loss _ Math */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000000000000000001
#define M_PI 3.14159265358979323846
#define MaxSize 30
int size;

void Multiple(double p1[MaxSize], double p2[MaxSize], double result[MaxSize])
{
    int i, j;
    for(i = 0; i < MaxSize; i++){
        if(fabs(p1[i]- 0) < errorT) continue;
        for(j = 0; j < MaxSize; j++){
            if(fabs(p2[j]- 0) < errorT) continue;
            result[i+j] += p1[i]*p2[j];
        }
    }
}
double Integral(double p1[MaxSize], double lower, double upper)
{
    double tmp[MaxSize] = {0};
    int i;
    for(i = 0; i < MaxSize; i++){
        if(fabs(p1[i]- 0) < errorT) continue;
        if(i == 0) tmp[i+1] = p1[i];
        else tmp[i+1] = p1[i]/(i+1);
    }
    double tmpU = 1, tmpL = 1, result = 0;
    for(i = 1; i < MaxSize-1; i++){
        tmpU *= upper;
        tmpL *= lower;
        result += (tmpU - tmpL)*tmp[i];
    }
    return result;
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
        scanf("%d", &size);
        if(size == 0) break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        double polynomail[MaxSize] = {0};
        for(i = size; i >= 0; i--) scanf("%lf", &polynomail[i]);
        double xMinusC[MaxSize] = {0};
        double c;
        scanf("%lf", &c);
        xMinusC[0] = -c;
        xMinusC[1] = 1;
        double pSquare[MaxSize] = {0};
        Multiple(polynomail, polynomail, pSquare);
        double xmcSquare[MaxSize] = {0};
        Multiple(xMinusC, xMinusC, xmcSquare);
        double pMxmc[MaxSize] = {0};
        Multiple(polynomail, xMinusC, pMxmc);
        double A = Integral(polynomail, -1, c), B = Integral(xMinusC, -1, c), C = Integral(pMxmc, -1, c), D = Integral(xmcSquare, -1, c);
        double E = Integral(polynomail, c, 1), F = Integral(xMinusC, c, 1), G = Integral(pMxmc, c, 1), H = Integral(xmcSquare, c, 1);
        double y0 = 2*(A + E - C*B/D - G*F/H);
        double m1 = (C-y0*B)/D, m2 = (G-y0*F)/H;
        printf("Case %d: %.3lf %.3lf %.3lf %.3lf\n", caseNumber, m1, -m1*c+y0 , m2, -m2*c+y0);
        caseNumber++;
    }
    return 0;
}
