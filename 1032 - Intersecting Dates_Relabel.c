/*1032 - Intersecting Dates*/
#include <stdio.h>
#include <stdlib.h>
#define DBUGM
typedef struct {
    int isValid;
    int date;
} label;

typedef struct {
    int date;
    int interval;
    int isRightPoint;
} point;

int cmp(const void*, const void*);
int NextDate(int x);
int PrevDate(int x);
char* StringDate(int date);

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int caseNumber = 1;
    while(1){
        int oldDate, newDate;
        scanf("%d%d", &oldDate, &newDate);
        if(oldDate == 0 && newDate == 0) break;

        int i, j;
        int intervals[oldDate + newDate][2];
        int pointNumber = 2*(oldDate + newDate);
        point points[pointNumber];
        for (i = 0; i < oldDate + newDate; i++){
            scanf("%d%d", &intervals[i][0], &intervals[i][1]);
            if(intervals[i][0] > intervals[i][1])exit(1);
            points[2*i].date = intervals[i][0];
            points[2*i].interval = i;
            points[2*i].isRightPoint = 0;
            points[2*i + 1].date = intervals[i][1];
            points[2*i + 1].interval = i;
            points[2*i + 1].isRightPoint = 1;
        }
        qsort(points, pointNumber, sizeof(point), cmp);
        #ifndef DBUGM
            printf("Sorted: \n");
            for(i = 0; i < 2*(oldDate + newDate); i++){
                printf("    %d interval[%d][%d]\n", points[i].date, points[i].interval, points[i].isRightPoint);
            }
        #endif
        int labelNumber = -1;
        int labeledInterval[pointNumber-1];
        for(i = 0; i < pointNumber-1; i++) labeledInterval[i] = 0;

        label labeledPoint[pointNumber];
        for(i = 0; i < pointNumber; i++){
            if(i == 0 || points[i].date != points[i-1].date){
                labelNumber++;
                labeledPoint[labelNumber].isValid = 0;
                labeledPoint[labelNumber].date = points[i].date;
            }
            intervals[points[i].interval][points[i].isRightPoint] = labelNumber;

        }
        labelNumber++;
        #ifndef DBUGM
            printf("Sorted: \n");
            for(i = 0; i < (oldDate + newDate); i++){
                printf("    %d interval[%d][%d]\n", intervals[i][0], i, 0);
                printf("    %d interval[%d][%d]\n", intervals[i][1], i, 1);
            }
        #endif
        for(i = (oldDate + newDate) -1; i >= 0 ; i--){
            int lp = intervals[i][0], rp = intervals[i][1];

            if (i >= oldDate) {
                for(j = lp; j <= rp; j++){

                    labeledPoint[j].isValid = 1;
                    if(j != rp) labeledInterval[j] = 1;
                }
            }
            else {
                for(j = lp; j <= rp; j++){
                    labeledPoint[j].isValid = 0;
                    if(j != rp) labeledInterval[j] = 0;
                }
            }
        }
        #ifndef DBUGM
            printf("Labeled Data: \n");
            for(i = 0; i < labelNumber; i++){
                printf("%d  ", labeledPoint[i].date);
            }printf("\n    ");
            for(i = 0; i < labelNumber; i++){
                printf("%d         ", i);
            }printf("\n    ");
            for(i = 0; i < labelNumber; i++){
                printf("%d         ", labeledPoint[i].isValid);
            }printf("\n         ");
            for(i = 0; i < labelNumber-1; i++){
                printf("%d         ", labeledInterval[i]);
            }printf("\n");
        #endif
        for(i = 0; i < labelNumber-1; i++){
            if(labeledPoint[i].date == PrevDate(labeledPoint[i+1].date) ) {
                if(labeledPoint[i].isValid && labeledPoint[i+1].isValid )labeledInterval[i] = 1;
                else labeledInterval[i] = 0;
            }
        }
        /*Print Result*/
        if( caseNumber > 1)printf("\n");
        printf("Case %d:\n", caseNumber);
        int isUpToDate = 1;
        int isPrint = 0;
        int pDate = 0;
        for(i = 0; i < labelNumber; i++){
            if (i == labelNumber-1){
                if (isPrint) {
                    if (labeledPoint[i].isValid){
                        if(pDate == labeledPoint[i].date) printf("\n");
                        else printf(" to %s\n", StringDate(labeledPoint[i].date));
                        isUpToDate = 0;
                    }
                    else {
                        if(pDate == PrevDate(labeledPoint[i].date)) printf("\n");
                        else printf(" to %s\n", StringDate(PrevDate(labeledPoint[i].date) ));
                        isUpToDate = 0;
                    }
                }
                else {
                    if (labeledPoint[i].isValid){
                        printf("    %s\n", StringDate(labeledPoint[i].date));
                        isUpToDate = 0;
                    }
                }

                continue;
            }
            if (labeledInterval[i]) {
                if (isPrint) {
                    if (labeledPoint[i].isValid){
                        continue;
                    }
                    else {
                        if(pDate == PrevDate(labeledPoint[i].date)) printf("\n");
                        else printf(" to %s\n", StringDate(PrevDate(labeledPoint[i].date) ));
                        printf("    %s", StringDate(NextDate(labeledPoint[i].date)));
                        pDate = NextDate(labeledPoint[i].date);
                        isUpToDate = 0;
                    }
                }
                else {
                    if (labeledPoint[i].isValid){
                        printf("    %s", StringDate(labeledPoint[i].date));
                        pDate = labeledPoint[i].date;
                        isUpToDate = 0;
                        isPrint = 1;
                    }
                    else {
                        printf("    %s", StringDate(NextDate(labeledPoint[i].date)));
                        pDate = NextDate(labeledPoint[i].date);
                        isUpToDate = 0;
                        isPrint = 1;
                    }
                }
            }
            else {
                if (isPrint) {
                    if (labeledPoint[i].isValid){
                        if(pDate == labeledPoint[i].date) printf("\n");
                        else printf(" to %s\n", StringDate(labeledPoint[i].date));
                        isUpToDate = 0;
                        isPrint = 0;
                    }
                    else {
                        if(pDate == PrevDate(labeledPoint[i].date)) printf("\n");
                        else printf(" to %s\n", StringDate(PrevDate(labeledPoint[i].date) ));
                        isUpToDate = 0;
                        isPrint = 0;
                    }
                }
                else {
                    if (labeledPoint[i].isValid){
                        printf("    %s\n", StringDate(labeledPoint[i].date));
                        isUpToDate = 0;
                    }
                }
            }
        }
        if (isUpToDate) printf("    No additional quotes are required.\n");
        caseNumber++;

    }
    return 0;
}

int cmp(const void* a, const void* b)
{
    point* pa = (point*) a;
    point* pb = (point*) b;
    return pa->date - pb->date;
}

int PrevDate(int x) {
	int mday[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	int yy = x/10000, mm = x%10000/100, dd = x%100;
	if ((yy%4 == 0 && yy%100 != 0) || yy%400 == 0)
		mday[2] = 29;
	if (mm == 1 && dd == 1) {
		yy--, mm = 12, dd = 31;
	} else {
		dd--;
		if (dd < 1)			mm--, dd = mday[mm];
	}
	return yy * 10000 + mm * 100 + dd;
}
int NextDate(int x) {
	int mday[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	int yy = x/10000, mm = x%10000/100, dd = x%100;
	if ((yy%4 == 0 && yy%100 != 0) || yy%400 == 0)
		mday[2] = 29;
	dd++;
	if (dd > mday[mm])	dd = 1, mm++;
	if (mm == 13)		mm = 1, yy++;
	return yy * 10000 + mm * 100 + dd;
}

char* StringDate(int date) {
	int yy = date/10000, mm = date%10000/100, dd = date%100;
	char *sdate = malloc(sizeof(char)*50);
	sprintf(sdate, "%d/%d/%d", mm, dd, yy);
	return sdate;
}
