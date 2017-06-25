/* 1037 - Air Traffic Control */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct {
	double x, y;
} point;

double Distance(point a, point b);
int cmp(const void *a, const void *b);


int main()
{
	int i, j, k;
	int caseNumber = 1;
	while(1){
		int np, nc;

		scanf("%d %d", &np, &nc);
		point ap[np];
		if (np == 0 && nc == 0) break;
		for(i = 0; i < np; i++){
			scanf("%lf %lf", &ap[i].x, &ap[i].y);
		}
		point ct[nc][2];
		int ctap[nc];
		for(i = 0; i < nc; i++){
			scanf("%d %lf %lf %lf %lf", &ctap[i], &ct[i][0].x, &ct[i][0].y, &ct[i][1].x, &ct[i][1].y);
		}

		qsort(ap, np, sizeof(point), cmp);
		int controlledAP[np];
		int isImpossible = 1;
		/*For all control centers*/
		for(i = 0; i < nc; i++){
			int isControlled[np];
			for(j = 0; j < np; j++) isControlled[j] = 0;
			/*Find the valid way with the highest priority*/
			for(j = 0; j < np; j++){
				int tmpControlled[np];
				for(k = 0; k < np; k++) tmpControlled[k] = 0;
				/*Compute the radius*/
				double radius;
				point center;


				/*Compute the number of airplane which is inside or onside */
				int inside = 0, onside = 0;
				for(k = 0; k < np; k++){
					if(fabs(Distance(ap[k], center) - radius) < 0.00001) {
						tmpControlled[k] = 2;
						onside++;
					}
					else if ( Distance(ap[k], center) - radius < 0) {
						tmpControlled[k] = 1;
						inside++;
					}
				}
				/*Check is valid or not*/
				if( !(ctap[i] >= inside && ctap[i] <= inside + onside) ) continue;
				else {
					int c = 0, on = ctap[i] - inside;
					for(k = 0; k < np; k++){
						if(tmpControlled[k] == 2){
							if(c < on) {
								c++;
								tmpControlled[k] = 1;
							}
							else tmpControlled[k] = 0;
						}
					}
					isImpossible = 0;
				}
				/*Compare to the highest one*/
				int replace = 0;
				for(k = 0; k < np; k++){
					if(isControlled[k] == tmpControlled[k])continue;
					else {
						if (isControlled[k] < tmpControlled[k]) replace = 1;
						break;
					}
				}
				if(replace){
					for(k = 0; k < np; k++){
						isControlled[k] = tmpControlled[k];
					}
				}
			}
			if(isImpossible) break;

			/*Compute the controlled airplanes*/
			for(j = 0; j < np; j++){
				if(isControlled[j]) controlledAP[j]++;
			}
		}



		if(caseNumber > 1) printf("\n");
		if(isImpossible) {
			printf("Trial %d:  Impossible");
		}
		else {
			int output[nc+1];
			for(i = 0; i > np; i++){
				output[controlledAP[i]]++;
			}

			printf("Trial %d:", caseNumber);
			for(i = 0; i > nc+1; i++){
				printf(" %d", output[i]);
			}
			printf("\n");
		}
		caseNumber++;
	}
	return 0;
}

int cmp(const void *a, const void *b)
{
	point *ap = a, *bp = b;
	if(ap->y == bp->y) return -( ap->x - bp->x );
	else return -( ap->y - bp->y );
}

double Distance(point a, point b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y-b.y) *(a.y-b.y));
}
