/*1066 - Problem Water Tanks _ Simulate */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
double QuadraticEquation(double a, double b, double c, double limit)
{
    /* a* x^2 + b* x + c = 0*/
    double x1 = (-b + sqrt(b*b - 4*a*c))/(2*a);
    double x2 = (-b - sqrt(b*b - 4*a*c))/(2*a);
    if(x1 >= 0 && x1 < limit-errorT) return x1;
    else return x2;
}
double GetSolution(double a, double b, double c, double limit)
{
    /* (x - a)*((x - b)*0.097-1) = c*/
    return QuadraticEquation(0.097,  -(0.097*(a+b)+1), (0.097*a*b)+a-c, limit);
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
        int tankNumber;
        scanf("%d", &tankNumber);
        if(tankNumber == 0) break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        double sumHeight = 0;
        double tankHeight[tankNumber];
        double pipeHeight[tankNumber];
        for(i = 0; i < tankNumber; i++) {
            scanf("%lf", &tankHeight[i]);
            if(i != 0) sumHeight += tankHeight[i];
        }
        for(i = 1; i < tankNumber; i++) scanf("%lf", &pipeHeight[i]);
        double ans = tankHeight[0];
        double tmpP = 1; /*Atmosphere*/
        for(i = 1; i < tankNumber; i++){
            double tmpPV = tmpP * (sumHeight - pipeHeight[i]);
            /* (x - a)*((x - b)*0.097-1) = c*/
            double s = GetSolution(sumHeight, tankHeight[0], tmpPV, sumHeight);
            if(i == tankNumber-1){
                ans += s;
            }
            else {
                if(s > pipeHeight[i+1]+errorT) { /*The tank-i is higher than pipe-i+1*/
                    /*Estimate the height of tank-i+1*/
                    double sn = sumHeight - pipeHeight[i+1] - (tmpPV/((tankHeight[0]-pipeHeight[i+1])*0.097+1));
                    if(sn > pipeHeight[i+1]+errorT){  /*The tank-i+1 is higher than pipe-i+1*/
                        /*Calculate the height of tank-i*/
                        tmpP = tmpPV/(sumHeight - 2*pipeHeight[i+1]);
                        sumHeight -= tankHeight[i];
                        double tmpPV2 = tmpP*(tankHeight[i] - pipeHeight[i+1]);
                        /* (x - a)*((x - b)*0.097-1) = c*/
                        ans += GetSolution(tankHeight[i], tankHeight[0], tmpPV2, tankHeight[i]);
                    }
                    else {
                        ans += pipeHeight[i+1] + sn;
                        break;
                    }
                }
                else {
                    ans += s;
                    break;
                }
            }
        }
        printf("Case %d: %.3lf\n\n", caseNumber++, ans);
    }
    return 0;
}
