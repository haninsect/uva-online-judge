/*1053 - Building a Clock _ Simulate*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.005
#define AnsSize 100000
int minutes[AnsSize][12];
int mNumber = 0;
int hours[AnsSize][12];
int hNumber = 0;
char geers[7] = {'\0'};
int teeth[7] = {0};
int rate;
void GetStringFormat(int minutes[12], int hours[12], char alph[12]);
int CompareAlph(int bMinute[12], int bHour[12], int minutes[12], int hours[12]);
void Swap(int *a, int *b);
void Permutation(int size, int data[size], int a, int b);
int powInt(int a, int b);

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    int mask[7] = {0};
    while(1){

        int gNumber;
        scanf("%d", &gNumber);
        if(gNumber == 0)break;
        scanf("%d", &rate);
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        /*Initialize*/
        int Impossible = 1;
        for(i = 0; i < 7; i++) geers[i] = '\0', teeth[i] = 0;
        for(i = 0; i < AnsSize; i++)
            for(j = 0; j < 12; j++)
                minutes[i][j] = -2, hours[i][j] = -2;
        mNumber = 0;
        hNumber = 0;

        for(i = 0; i < gNumber; i++){
            scanf("%s%d", &geers[i], &teeth[i]);
        }
        /*For all possible combination*/
        for(i = 1; i < powInt(2, gNumber); i++){
            int queue[14] = {0};
            int qp = 0;
            for(j = 0; j < gNumber; j++){
                if((i&mask[j]) > 0) {
                    queue[qp++] = j;
                }
            }
            for(j = qp; j < qp*2-1; j++){/*Add '-' */
                queue[j] = -1;
            }
            qp = qp*2-1;
            /*For all possible arrangement*/
            Permutation(qp, queue, 0, qp-1);
        }
        /*Merge minutes and hours, find the best answer*/
        int minShafts = LargeINT, minGeers = LargeINT;
        int mIndex = -1, hIndex = -1;
        char alph[12] = {'\0'};
        printf("%d %d\n", mNumber, hNumber);
        for(i = 0; i < mNumber; i++){
            for(j = 0; j < hNumber; j++){
                int tmpShafts = 1, tmpGeers = 0;
                int separate = 0;
                int valid = 1;
                int used[7] = {0};
                for(k = 0; (minutes[i][k] != -2 || hours[j][k] != -2) && valid == 1; k++){
                    if(separate == 0){ /*Still have common part*/
                        if(minutes[i][k] != hours[j][k]){/*separate here*/
                            separate = 1;
                            if(minutes[i][k] >= 0){
                                if(used[minutes[i][k]]) valid = 0;
                                else {
                                    used[minutes[i][k]] = 1;
                                    tmpGeers++;
                                }
                            }
                            else if (minutes[i][k] == -1){
                                tmpShafts++;
                            }
                            if(hours[j][k] >= 0){
                                if(used[hours[j][k]]) valid = 0;
                                else {
                                    used[hours[j][k]] = 1;
                                    tmpGeers++;
                                }
                            }
                            else if (hours[j][k] == -1){
                                tmpShafts++;
                            }
                        }
                        else {/*Same again*/
                            if(minutes[i][k] >= 0){
                                if(used[minutes[i][k]]) valid = 0;
                                else {
                                    used[minutes[i][k]] = 1;
                                    tmpGeers++;
                                }
                            }
                            else if (minutes[i][k] == -1){
                                tmpShafts++;
                            }
                        }
                    }
                    else { /*Have Separated*/
                        if(minutes[i][k] >= 0){
                            if(used[minutes[i][k]]) valid = 0;
                            else {
                                used[minutes[i][k]] = 1;
                                tmpGeers++;
                            }
                        }
                        else if (minutes[i][k] == -1){
                            tmpShafts++;
                        }
                        if(hours[j][k] >= 0){
                            if(used[hours[j][k]]) valid = 0;
                            else {
                                used[hours[j][k]] = 1;
                                tmpGeers++;
                            }
                        }
                        else if (hours[j][k] == -1){
                            tmpShafts++;
                        }
                    }
                }
                if( valid ){/*Compare the answer with the best one*/
                    Impossible = 0;
                    int replace = 0;
                    char tmpAlph[12] = {'\0'};
                    if(tmpShafts < minShafts) replace = 1;
                    else if(tmpShafts == minShafts){
                        if(tmpGeers < minGeers) replace = 1;
                        else if(tmpGeers == minGeers){
                            if(CompareAlph(minutes[mIndex], hours[hIndex], minutes[i], hours[j])) replace = 1;
                        }
                    }
                    if(replace){
                        minShafts = tmpShafts;
                        minGeers = tmpGeers;
                        mIndex = i;
                        hIndex = j;
                    }
                }

            }
        }


        printf("Trial %d", caseNumber);
        if(Impossible == 0){
            printf("\nMinutes: *");
            for(i = 0; minutes[mIndex][i] != -2; i++){
                printf("%c", minutes[mIndex][i]);
            }
            printf("\nHours: *");
            for(i = 0;  hours[hIndex][i] != -2; i++){
                printf("%c", hours[hIndex][i]);
            }
            printf("\n");
        }
        printf(" IS IMPOSSIBLE\n");
        caseNumber++;

    }
    return 0;
}

int powInt(int a, int b)
{
    int i, r = 1;
    for(i = 0; i < b; i++) r *= a;
    return r;
}

void Permutation(int size, int data[size], int a, int b)
{
    int i, j;
	if(a == b)
	{
	    #ifdef DBUGM
           printf("Find Permutation:%d\n", size);
            for(i = 0; i < size; i++) {
                if(data[i] == -1) printf("-");
                else printf("%c", geers[data[i]]);
            }printf("\n");
        #endif

	    /*Minute*/
	    int dc = 0, nc = 0, start = 0;
	    int tmpRate = -rate, tmpTeeth = 0;
	    if(rate == 24) mNumber++;
	    else {
            while(data[start] == -1) start++;
            tmpTeeth = tmpRate*teeth[data[start]];
            for(i = start; i < size; i++) {

                if(data[i] == -1) dc++, nc = 0;
                else nc++, dc = 0;
                if(dc >= 2) break;
                if(nc >= 3) break;
                if(nc == 1) tmpRate = -tmpTeeth/teeth[data[start]];
                if(nc != 0) tmpTeeth = tmpRate*teeth[data[start]];
                if(tmpRate == 24){
                    for(j = start; j <= i; j++){
                        minutes[mNumber][j-start] = data[j];
                    }
                    mNumber++;
                    break;
                }
            }
	    }
		/*Hour*/
		dc = 0;
		nc = 0;
		start = 0;
	    tmpRate = -rate;
	    if(rate == 2) hNumber++;
	    else {
            while(data[start] == -1) start++;
            tmpTeeth = tmpRate*teeth[data[start]];
            for(i = start; i < size; i++) {
                if(data[i] == -1) dc++, nc = 0;
                else nc++, dc = 0;
                if(dc >= 2) break;
                if(nc >= 3) break;
                if(nc == 1) tmpRate = -tmpTeeth/teeth[data[start]];
                if(nc != 0) tmpTeeth = tmpRate*teeth[data[start]];
                if(tmpRate == 2){
                    for(j = start; j <= i; j++){
                        hours[hNumber][j-start] = data[j];
                    }
                    hNumber++;
                    break;
                }
            }
	    }
	}
	else
	{
	    int dc = 0, nc = 0, start = 0;
        while(data[start] == -1) start++;
        for(i = start; i < a; i++) {
            if(data[i] == -1) dc++, nc = 0;
            else nc++, dc = 0;
            if(dc >= 2) return;
            if(nc >= 3) return;
        }


		for(i = a; i < size; i++)
		{
		  Swap(&data[a], &data[i]);
		  Permutation(size, data, a+1, b);
		  Swap(&data[a], &data[i]);
		}
	}
}
void Swap(int *a, int *b)
{
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
int CompareAlph(int bMinute[12], int bHour[12], int minutes[12], int hours[12])
{
    int i, j;
    char bAlph[12] = {'\0'};
    GetStringFormat(bMinute, bHour, bAlph);
    char alph[12] = {'\0'};
    GetStringFormat(minutes, hours, alph);
    return strcmp(alph, bAlph);
}
void GetStringFormat(int minutes[12], int hours[12], char alph[12])
{
    int i, j;
    int sp = 0;
    for(i = 0; minutes[i] != -2; i++){
        if(minutes[i] == -1) alph[sp++] = '-';
        else alph[sp++] = geers[minutes[i]];
    }
    for(i = 0; hours[i] != -2; i++){
        if(hours[i] == -1) alph[sp++] = '-';
        else alph[sp++] = geers[hours[i]];
    }
}
