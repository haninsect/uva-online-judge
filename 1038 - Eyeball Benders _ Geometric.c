/*1038 - Eyeball Benders _ Geometric*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.005
typedef struct {
    int isVertical;
    double x[2], y[2];
} line;
typedef struct {
    double offx, offy;
    double magnitude;
    int sl, si;
    int pl, pi;
} rec
void ConstructImage(int size, line image[size]);

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k, l, m, n;
    int caseNumber = 1;
    while(1){

        int m, n;
        scanf("%d%d", &n, &m);
        if(n == 0 && m == 0)break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int isValid = 0;
        line puzzle[m];
        ConstructImage(m, puzzle);
        line solution[n];
        ConstructImage(n, solution);

        /*Chose all point in solution as a reference, match to all points in puzzle to decide offset*/
        for(i = 0; i < n && isValid == 0; i++){
            for(j = 0; j < m && isValid == 0; j++){
                if(solution[i].isVertical != puzzle[j].isVertical) continue;
                for(k = 0; k < 2 && isValid == 0; k++){
                    double offX = solution[i].x[k] - puzzle[j].x[k];
                    double offY = solution[i].y[k] - puzzle[j].y[k];
                    /*Chose a line in solution as a reference, match to all lines in puzzle to decide magnitude */
                    int ln = FindLine(n, solution, solution[i].x[j], solution[i].y[j]);
                    if(ln = -1) exit(1);
                    for(l = 0; l < m && isValid == 0; l++){
                        if(solution[ln].isVertical != puzzle[l].isVertical) continue;
                        double magnitude;
                        if(solution[ln].isVertical == 1) {
                            if((solution[ln].x[0] - solution[i].x[k])*(puzzle[l].x[0] - puzzle[j].x[k]) <= 0) continue;
                            magnitude = (solution[ln].x[0] - solution[i].x[k]) / (puzzle[l].x[0] - puzzle[j].x[k]);
                        }
                        else {
                            if((solution[ln].y[0] - solution[i].y[k])*(puzzle[l].y[0] - puzzle[j].y[k]) <= 0) continue;
                            magnitude = (solution[ln].y[0] - solution[i].y[k]) / (puzzle[l].y[0] - puzzle[j].y[k]);
                        }
                        rec r;
                        r.magnitude = magnitude;
                        r.offx = offX;
                        r.offy = offY;
                        if(Check(m, puzzle, n, solution, offX, offY, magnitude) == 1) isValid = 1;
                    }
                }
            }
        }
        if(isValid) printf("Case %d: valid puzzle\n", caseNumber);
        else printf("Case %d: impossible\n", caseNumber);
        caseNumber++;
    }
    return 0;
}
int Check(int m, line puzzle[m], int n, line solution[n], double offX, double offY, double magnitude)
{
    int i, j, k;
    int valid[m];
    for(i = 0; i < m; i++) valid[i] = 1;

    for(i = 0; i < n; i++){
        for(j = 0; j < m; j++){
            if(valid[j] == 0) continue;
            if(solution[i].isVertical != puzzle[j].isVertical) continue;
            if(solution[i].isVertical == 1) {
                if(fabs(solution[i].x[0] + offX - puzzle[j].x[0]*magnitude) < errorT)
                    if(solution[i].y[0] >= solution[i].y[0] - errorT && solution[i].y[0] <= solution[i].y[1] + errorT)

                if((solution[ln].x[0] - solution[i].x[k])*(puzzle[l].x[0] - puzzle[j].x[k]) <= 0) continue;
                magnitude = (solution[ln].x[0] - solution[i].x[k]) / (puzzle[l].x[0] - puzzle[j].x[k]);
            }
            else {
                if((solution[ln].y[0] - solution[i].y[k])*(puzzle[l].y[0] - puzzle[j].y[k]) <= 0) continue;
                magnitude = (solution[ln].y[0] - solution[i].y[k]) / (puzzle[l].y[0] - puzzle[j].y[k]);
            }
        }
    }
    /*Check there are no extra line int puzzle zone*/
}
int FindLine(int n, line solution[n], double x, double y)
{
    int i;
    for(i = 0; i < n; i++){
        if(solution[i].isVertical == 1){
            if(fabs(x - solution[i].x[0]) < errorT || (y >= solution[i].y[0] - errorT && y <= solution[i].y[1] + errorT)) continue;
            return i;
        }
        else {
            if(fabs(y - solution[i].y[0]) < errorT || (x >= solution[i].x[0] - errorT && x <= solution[i].x[1] + errorT)) continue;
            return i;
        }
    }
    /*To do :
               A*      * * *
                *
                *
     */
    return -1;
}
void ConstructImage(int size, line image[size])
{
    int i;
    for(i = 0; i < size; i++){
        double x1, y1, x2, y2;
        scanf("%lf%lf%lf%lf", &x1, &y1, &x2, &y2);
        if(fabs(x1-x2) < errorT){
            image[i].isVertical = 1;
            image[i].x[0] = x1;
            image[i].x[1] = x1;
            if(y1 > y2){
                image[i].y[1] = y1;
                image[i].y[0] = y2;
            }
            else {
                image[i].y[1] = y2;
                image[i].y[0] = y1;
            }
        }
        else {
            image[i].isVertical = 0;
            image[i].y[0] = y1;
            image[i].y[1] = y1;
            if(x1 > x2){
                image[i].x[1] = x1;
                image[i].x[0] = x2;
            }
            else {
                image[i].x[1] = x2;
                image[i].x[0] = x1;
            }
        }
    }
    return;
}
