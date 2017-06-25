/* 1037 - Air Traffic Control */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct {
	double x, y;
} point;

double Distance(point a, point b);
int cmp(const void *a, const void *b);
const double errorT = 0.00001;

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
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
        for(j = 0; j < np; j++) controlledAP[j] = 0;
		int isImpossible = 1;
		/*For all control centers*/
		for(i = 0; i < nc; i++){
            isImpossible = 1;
            double smallR = 0;
			int isControlled[np];
			for(j = 0; j < np; j++) isControlled[j] = 0;
			/*Find the valid way with the highest priority*/
			for(j = 0; j < np; j++){
				int tmpControlled[np];
				for(k = 0; k < np; k++) tmpControlled[k] = 0;
				/*Compute the radius*/
				double radius = 0;
				point center;
                double a1, b1, c1;
                a1 = ct[i][1].x - ct[i][0].x;
                b1 = ct[i][1].y - ct[i][0].y;
                c1 = (ct[i][0].y*ct[i][0].y + ct[i][0].x*ct[i][0].x - ct[i][1].x*ct[i][1].x - ct[i][1].y*ct[i][1].y)/2;

                double a2, b2, c2;
                a2 = ct[i][1].x - ap[j].x;
                b2 = ct[i][1].y - ap[j].y;
                c2 = (ap[j].y*ap[j].y + ap[j].x*ap[j].x - ct[i][1].x*ct[i][1].x - ct[i][1].y*ct[i][1].y )/2;

                center.x = (b2*c1 - b1*c2)/(b1*a2 - b2*a1);
                center.y = (c2*a1 - c1*a2)/(b1*a2 - b2*a1);
                radius = Distance(center, ap[j]);
                /*printf("\t%d %d -> %lf %lf\n", i, j, center.x, center.y);
                printf("\t%d %d -> %lf %lf %lf\n", i, j, Distance(center, ct[i][0]), Distance(center, ct[i][1]), Distance(center, ap[j]));*/
                if(smallR > 0)
                    if(radius - smallR > errorT ) continue;

				/*Compute the number of airplane which is inside or onside */
				int inside = 0, onside = 0;
				for(k = 0; k < np; k++){
                        /*printf("d: %lf\n", Distance(ap[k], center));*/
					if(fabs(Distance(ap[k], center) - radius) < errorT) {
						tmpControlled[k] = 2;
						onside++;
					}
					else if ( Distance(ap[k], center) - radius < 0) {
						tmpControlled[k] = 1;
						inside++;
					}
				}
				/*Check is valid or not*/
				if( !(ctap[i] >= inside && ctap[i] <= inside + onside) ) {
                       /* printf("(%d %d)fail: %d, %d %d\n", i, j, ctap[i], inside, onside);*/
                    continue;
				}
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
				if(smallR > 0){
                    if( fabs(smallR - radius < errorT) ){
                        for(k = 0; k < np; k++){
                            if(isControlled[k] == tmpControlled[k])continue;
                            else {
                                if (isControlled[k] < tmpControlled[k]) replace = 1;
                                break;
                            }
                        }

                    }
                    else replace = 1;
				}
				else replace = 1;
				if(replace){
                    for(k = 0; k < np; k++){
                        isControlled[k] = tmpControlled[k];
                    }
                    smallR = radius;
                }


			}
			if(isImpossible) break;

			/*Compute the controlled airplanes*/
			for(j = 0; j < np; j++){
				if(isControlled[j]) controlledAP[j]++;
			}
			/*Start New control*/
			/*printf("R: %lf\n", smallR);
			for(j = 0; j < np; j++) printf("%d: %d\n", j, isControlled[j]);*/
		}

		if(isImpossible) {
			printf("Trial %d:  Impossible\n", caseNumber);
		}
		else {
			int output[nc+1];
			for(j = 0; j < np; j++) output[j] = 0;
			for(i = 0; i < np; i++){
				/*printf(" %d\n", controlledAP[i]);*/
				output[controlledAP[i]]++;
			}

			printf("Trial %d:  ", caseNumber);
			for(i = 0; i < nc+1; i++){
				printf("%d  ", output[i]);
			}
			printf("\n");
		}
		printf("\n");
		caseNumber++;
	}
	return 0;
}

int cmp(const void *a, const void *b)
{
	point *ap = a, *bp = b;
	if(fabs(ap->y - bp->y) < errorT) return -( ap->x - bp->x );
	else return -( ap->y - bp->y );
}

double Distance(point a, point b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y-b.y) *(a.y-b.y));
}
