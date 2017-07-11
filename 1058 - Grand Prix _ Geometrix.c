/*1058 - Grand Prix _ Geometrix*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define MaxSize 100
#define errorT 0.00005
#define M_PI 3.14159265358979323846
typedef struct {
    double x1, y1;
    double x2, y2;
} segment;
typedef struct {
    double left, right;
} interval;

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){

        int n, t;
        scanf("%d%d", &n, &t);
        if(n == 0)break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int fromx = 0, fromy = 0;
        segment lines[n];
        for(i = 0; i < n; i++){
            int a, b;
            scanf("%d%d", &a, &b);
            lines[i].x1 = fromx;
            lines[i].y1 = fromy;
            lines[i].x2 = a;
            lines[i].y2 = b;
            fromx = a;
            fromy = b;
        }
        if(t == 0) {
            printf("Case %d: Acceptable as proposed\n\n", caseNumber++);
            continue;
        }
        /*For all line segment, find the interval of available theta*/
        interval clockwise[n];
        interval counterclock[n];
        for(i = 0; i < n; i++){
            double dx = lines[i].x2 - lines[i].x1;
            double dy = lines[i].y2 - lines[i].y1;
            if(fabs(dx -0) < errorT) {
                if(dy > 0) {
                    clockwise[i].left = 0;
                    clockwise[i].right = 180;
                    counterclock[i].left = 0;
                    counterclock[i].right = 0;
                }
                else {
                    clockwise[i].left = 0;
                    clockwise[i].right = 0;
                    counterclock[i].left = 0;
                    counterclock[i].right = 180;
                }
            }
            else {
                double theta = atan2(dy, dx);
                theta = theta*180.0/M_PI;
                AddInterval(theta, i, n, clockwise, counterclock);
            }
        }
        /*Find Intersection of intervals*/
        /*ClockWise*/
        double ans = LargeINT;
        double maxl = 0, minr = 180;
        int flag = 0;
        for(i = 0; i < n; i++){
            #ifdef DBUGM
                printf("1 [%lf, %lf]\n", clockwise[i].left, clockwise[i].right);
            #endif
            if(maxl < clockwise[i].left) maxl = clockwise[i].left;
            if(minr > clockwise[i].right) minr = clockwise[i].right;
        }
        #ifdef DBUGM
             printf("%lf, %lf\n", maxl, minr);
        #endif

        if(maxl < minr || fabs(maxl - minr) < errorT) ans = maxl;
        /*CounterClockWise*/
        maxl = 0;
        minr = 180;
        for(i = 0; i < n; i++){
            #ifdef DBUGM
                printf("2 [%lf, %lf]\n", counterclock[i].left, counterclock[i].right);
            #endif
            if(maxl < counterclock[i].left) maxl = counterclock[i].left;
            if(minr > counterclock[i].right) minr = counterclock[i].right;
        }
        #ifdef DBUGM
             printf("%lf, %lf\n", maxl, minr);
        #endif
        if((maxl < minr || fabs(maxl - minr) < errorT) && maxl < ans) ans = maxl, flag = 1;





        if(fabs(ans - LargeINT) < errorT) printf("Case %d: Unacceptable\n", caseNumber++);
        else if(fabs(ans - 0) < errorT) printf("Case %d: Acceptable as proposed\n", caseNumber++);
        else {
            if(flag == 1) printf("Case %d: Acceptable after counterclockwise rotation of %.2lf degrees\n", caseNumber++, ans);
            else printf("Case %d: Acceptable after clockwise rotation of %.2lf degrees\n", caseNumber++, ans);
        }
        printf("\n");
    }
    return 0;
}
void AddInterval(double theta, int i, int n, interval clockwise[n], interval counterclock[n])
{
    if(theta > 90){
        clockwise[i].left = theta - 90;
        clockwise[i].right = 180;
        counterclock[i].left = 270 - theta;
        counterclock[i].right = 180;
    }
    else if(theta > 0 && theta < 90){
        clockwise[i].left = 0;
        clockwise[i].right = 90 + theta;
        counterclock[i].left = 0;
        counterclock[i].right = 90 - theta;
    }
    else if(theta < 0 && theta > -90){
        clockwise[i].left = 0;
        clockwise[i].right = 90 + theta;
        counterclock[i].left = 0;
        counterclock[i].right = 90 - theta;
    }
    else if(theta < -90){
        clockwise[i].left = 270 + theta;
        clockwise[i].right = 180;
        counterclock[i].left = -90 - theta;
        counterclock[i].right = 180;
    }
    else if(fabs(theta - 0) < errorT){
        clockwise[i].left = 0;
        clockwise[i].right = 90;
        counterclock[i].left = 0;
        counterclock[i].right = 90;
    }
    else exit(1);
}


