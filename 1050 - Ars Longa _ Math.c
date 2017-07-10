/*1050 - Ars Longa _ Math*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define MaxSize 350
#define errorT 0.00005
typedef struct {
    double x, y, z;
} Vec;
int ball, rod;
Vec balls[MaxSize];
double equation[MaxSize][MaxSize];

Vec* VecDiff(Vec a, Vec b);
void SwapRow(int a, int b);
int Gauss(int totalEquation);
void PrintEquation(int ep);

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){

        scanf("%d%d", &ball, &rod);
        if(ball == 0)break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        /*Set equation*/
        for(i = 0; i < ball; i++){
            scanf("%lf%lf%lf", &balls[i].x, &balls[i].y, &balls[i].z);
        }
        int ep = 0;
        for(i = 0; i < MaxSize; i++)
            for(j = 0; j < MaxSize; j++)
                equation[i][j] = 0.0; /*[Row][variable]*/
        int ballToEq[ball];
        for(i = 0; i < ball; i++) ballToEq[i] = -1;
        for(i = 0; i < rod; i++){
            int a, b;
            scanf("%d%d", &a, &b);
            a--;
            b--;
            if(fabs(balls[a].z - 0) > errorT){
                if(ballToEq[a] == -1) ballToEq[a] = ep++;
                Vec *force = VecDiff(balls[a], balls[b]);
                equation[ballToEq[a]*3+0][i] = force->x;
                equation[ballToEq[a]*3+1][i] = force->y;
                equation[ballToEq[a]*3+2][i] = force->z;
                equation[ballToEq[a]*3+2][rod] = 1;
                free(force);
            }
            if(fabs(balls[b].z - 0) > errorT){
                if(ballToEq[b] == -1) ballToEq[b] = ep++;
                Vec *force2 = VecDiff(balls[b], balls[a]);
                equation[ballToEq[b]*3+0][i] = force2->x;
                equation[ballToEq[b]*3+1][i] = force2->y;
                equation[ballToEq[b]*3+2][i] = force2->z;
                equation[ballToEq[b]*3+2][rod] = 1;
                free(force2);
            }

        }
        /*PrintEquation(ep*3);*/
        /*Gauss elimination*/
        int result = Gauss(ep*3);
        if (result == 0) printf("Sculpture %d: NON-STATIC\n", caseNumber);
        else if (result == 1) printf("Sculpture %d: UNSTABLE\n", caseNumber);
        else printf("Sculpture %d: STABLE\n", caseNumber);
        caseNumber++;
    }
    return 0;
}

int Gauss(int totalEquation)
{
    int i, j, k;
    int valSol = 0;
    for(i = 0; i < rod; i++){
        /*Chose a not 0 pivot*/
        for(j = valSol; j < totalEquation; j++){
            if(fabs(equation[j][i] - 0) > errorT) break;
        }
        if(j == totalEquation) continue; /*All coefficient is zero*/
        /*Swap the not 0 equation to the top*/
        SwapRow(valSol, j);
        /*Make all other down row 0 in this rod*/
        for(j = valSol+1; j < totalEquation; j++){
            double mul = equation[j][i]/equation[valSol][i];
            for(k = i; k < rod+1; k++){
                equation[j][k] -= mul*equation[valSol][k];
            }
        }
        valSol++;
    }
    for(i = valSol; i < totalEquation; i++) if(fabs(equation[i][rod] - 0) > errorT) return 0;
    /*printf("%d/%d\n", valSol, totalEquation);*/
    if(valSol < totalEquation) return 1;
    else return 2;
    /*Check result
    1. No solution: There are non zero value in the rest row of the (rod+1)th col.
    2. Infinite solution: The number of variable solved is less than total variable.
    3. Unique solution: The number of variable solved is not less than total variable.?????
    */
}

void SwapRow(int a, int b)
{
    int i, j;
    for(i = 0; i < rod+1; i++){
        double tmp =  equation[a][i];
        equation[a][i] = equation[b][i];
        equation[b][i] = tmp;
    }
}
Vec* VecDiff(Vec a, Vec b)
{
    Vec *v = malloc(sizeof(Vec));
    v->x = a.x - b.x;
    v->y = a.y - b.y;
    v->z = a.z - b.z;
    return v;
}

void PrintEquation(int ep)
{
    int i, j;
    for(i = 0; i < ep; i++){
        for(j = 0; j < rod+1; j++){
            printf("%3.2lf ", equation[i][j]);
        }
        printf("\n");
    }
}
