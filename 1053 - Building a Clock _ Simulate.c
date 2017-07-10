/*1053 - Building a Clock _ Simulate*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.00005
#define AnsSize 100000
#define AnsLen 15
int minutes[AnsSize][AnsLen];
int mNumber = 0;
int hours[AnsSize][AnsLen];
int hNumber = 0;
char geers[7] = {'\0'};
double teeth[7] = {0.0};
double rate;
void GetStringFormat(int minutes[AnsLen], int hours[AnsLen], char alph[AnsLen]);
int CompareAlph(int bMinute[AnsLen], int bHour[AnsLen], int minutes[AnsLen], int hours[AnsLen]);
void Swap(int *a, int *b);
void Permutation(int size, int data[size], int a, int b);
int powInt(int a, int b);
void Check(int size, int data[AnsLen]);

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    int mask[7] = {0};
    for(i = 0; i < 7; i++) mask[i] = powInt(2, i);
    while(1){

        int gNumber;
        scanf("%d", &gNumber);
        if(gNumber == 0)break;
        scanf("%lf", &rate);
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        /*Initialize*/
        for(i = 0; i < 7; i++) geers[i] = '\0', teeth[i] = 0.0;
        for(i = 0; i < AnsSize; i++)
            for(j = 0; j < AnsLen; j++)
                minutes[i][j] = -2, hours[i][j] = -2;
        mNumber = 0;
        hNumber = 0;

        for(i = 0; i < gNumber; i++){
            scanf("%s%lf", &geers[i], &teeth[i]);
        }
        /*For all possible combination*/
        /*1 shaft*/
        if(rate == 24) mNumber++;
        if(rate == 2) hNumber++;
        /*2~6 shafts*/
        for(i = 1; i < powInt(2, gNumber); i++){
            int queue[AnsLen] = {0};
            int qp = 0;
            for(j = 0; j < gNumber; j++){
                if((i&mask[j]) > 0) {
                    queue[qp++] = j;
                }
            }
            /*For all possible arrangement*/
            if(qp > 1) Permutation(qp, queue, 0, qp-1);
        }
        /*Merge minutes and hours, find the best answer*/
        int minShafts = LargeINT, minGeers = LargeINT;
        int mIndex = -1, hIndex = -1;
        #ifdef DBUGM
            printf("mNumber: %d, hNumber: %d\n", mNumber, hNumber);
        #endif
        int Impossible = 1;
        for(i = 0; i < mNumber; i++){
            for(j = 0; j < hNumber; j++){
                int tmpShafts, tmpGeers;
                int valid = 0;
                valid = Merge(i, j, &tmpGeers, &tmpShafts);
                /*Compare the answer with the best one*/
                if( valid ){
                    Impossible = 0;
                    int replace = 0;
                    if(tmpShafts < minShafts) replace = 1;
                    else if(tmpShafts == minShafts){
                        if(tmpGeers < minGeers) replace = 1;
                        else if(tmpGeers == minGeers){
                            if(CompareAlph(minutes[mIndex], hours[hIndex], minutes[i], hours[j]) == 1) replace = 1;
                        }
                    }
                    if(replace){
                        minShafts = tmpShafts;
                        minGeers = tmpGeers;
                        mIndex = i;
                        hIndex = j;
                        /*printf("%d %d\n", tmpShafts, tmpGeers);*/
                    }
                }

            }
        }

        if(caseNumber > 1) printf("\n");
        printf("Trial %d", caseNumber);
        if(Impossible == 0){
            printf("\nMinutes: *");
            for(i = 0; minutes[mIndex][i] != -2; i++){
                if(minutes[mIndex][i] == -1) printf("-");
                else printf("%c", geers[minutes[mIndex][i]]);
            }
            printf("\nHours:   *");
            for(i = 0;  hours[hIndex][i] != -2; i++){
                if(hours[hIndex][i] == -1) printf("-");
                else printf("%c", geers[hours[hIndex][i]]);
            }
            printf("\n");
        }
        else printf(" IS IMPOSSIBLE\n");
        caseNumber++;

    }
    return 0;
}
int Merge(int i, int j, int* Geers, int* Shafts)
{
    int k;
    int tmpGeers = 0, tmpShafts = 1;
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
    *Geers = tmpGeers;
    *Shafts = tmpShafts;
    return valid;
}
int powInt(int a, int b)
{
    int i, r = 1;
    for(i = 0; i < b; i++) r *= a;
    return r;
}

void Permutation(int size, int data[AnsLen], int a, int b)
{
    int i, j;
	if(a == b)
	{
	    #ifdef DBUGM
           printf("Find Permutation:%d\n", size);
            for(i = 0; i < size; i++) {
                printf("%c", geers[data[i]]);
            }printf("\n");
        #endif
	    Check(size, data);
	}
	else
	{
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
int CompareAlph(int bMinute[AnsLen], int bHour[AnsLen], int minutes[AnsLen], int hours[AnsLen])
{
    int i, j;
    char bAlph[AnsLen] = {'\0'};
    GetStringFormat(bMinute, bHour, bAlph);
    char alph[AnsLen] = {'\0'};
    GetStringFormat(minutes, hours, alph);
    #ifdef DBUGM
       printf("Compare answer: \n")
       printf("%s  v.s.  %s  Replace?: %d\n", bAlph, alph, strcmp(bAlph, alph));
    #endif

    return strcmp(bAlph, alph);
}
void GetStringFormat(int minutes[AnsLen], int hours[AnsLen], char alph[AnsLen])
{
    int i, j;
    int sp = 0;
    for(i = 0; minutes[i] != -2; i++){
        if(minutes[i] == -1) continue;
        else alph[sp++] = geers[minutes[i]];
    }
    for(i = 0; hours[i] != -2; i++){
        if(hours[i] == -1) continue;
        else alph[sp++] = geers[hours[i]];
    }
}
void Check(int size, int data[AnsLen])
{
    #ifdef DBUGM
       printf("Start Check: %d\nmNumber: %d, hNumber: %d\n", size, mNumber, hNumber);
    #endif
    int i, j;
    switch(size){
    case 2:
        /*A-B*/
        if (fabs(-rate*teeth[data[0]]/teeth[data[1]] - 24) < errorT){
            minutes[mNumber][0] = data[0];
            minutes[mNumber][1] = -1;
            minutes[mNumber][2] = data[1];
            mNumber++;
        }
        else if (fabs(-rate*teeth[data[0]]/teeth[data[1]] - 2) < errorT){
            hours[hNumber][0] = data[0];
            hours[hNumber][1] = -1;
            hours[hNumber][2] = data[1];
            hNumber++;
        }
        break;
    case 3:
        /*A-B-C*/
        if (fabs(rate*teeth[data[0]]/teeth[data[2]] - 24) < errorT){
            minutes[mNumber][0] = data[0];
            minutes[mNumber][1] = -1;
            minutes[mNumber][2] = data[1];
            minutes[mNumber][3] = -1;
            minutes[mNumber][4] = data[2];
            mNumber++;
        }
        else if (fabs(rate*teeth[data[0]]/teeth[data[2]] - 2) < errorT){
            hours[hNumber][0] = data[0];
            hours[hNumber][1] = -1;
            hours[hNumber][2] = data[1];
            hours[hNumber][3] = -1;
            hours[hNumber][4] = data[2];
            hNumber++;
        }
        break;
    case 4:
        /*A-B-C-D*/
        if (fabs(-rate*teeth[data[0]]/teeth[data[3]] - 24) < errorT){
            minutes[mNumber][0] = data[0];
            minutes[mNumber][1] = -1;
            minutes[mNumber][2] = data[1];
            minutes[mNumber][3] = -1;
            minutes[mNumber][4] = data[2];
            minutes[mNumber][5] = -1;
            minutes[mNumber][6] = data[3];
            mNumber++;
        }
        else if (fabs(-rate*teeth[data[0]]/teeth[data[3]] - 2) < errorT){
            hours[hNumber][0] = data[0];
            hours[hNumber][1] = -1;
            hours[hNumber][2] = data[1];
            hours[hNumber][3] = -1;
            hours[hNumber][4] = data[2];
            hours[hNumber][5] = -1;
            hours[hNumber][6] = data[3];
            hNumber++;
        }
        /*A-BC-D*/
        if (fabs(rate*teeth[data[0]]*teeth[data[2]]/(teeth[data[1]]*teeth[data[3]]) - 24) < errorT){
            minutes[mNumber][0] = data[0];
            minutes[mNumber][1] = -1;
            minutes[mNumber][2] = data[1];
            minutes[mNumber][3] = data[2];
            minutes[mNumber][4] = -1;
            minutes[mNumber][5] = data[3];
            mNumber++;
        }
        else if (fabs(rate*teeth[data[0]]*teeth[data[2]]/(teeth[data[1]]*teeth[data[3]]) - 2) < errorT){
            hours[hNumber][0] = data[0];
            hours[hNumber][1] = -1;
            hours[hNumber][2] = data[1];
            hours[hNumber][3] = data[2];
            hours[hNumber][4] = -1;
            hours[hNumber][5] = data[3];
            hNumber++;
        }
        break;
    case 5:
        /*A-B-C-D-E*/
        if (fabs(rate*teeth[data[0]]/teeth[data[4]] - 24) < errorT){
            minutes[mNumber][0] = data[0];
            minutes[mNumber][1] = -1;
            minutes[mNumber][2] = data[1];
            minutes[mNumber][3] = -1;
            minutes[mNumber][4] = data[2];
            minutes[mNumber][5] = -1;
            minutes[mNumber][6] = data[3];
            minutes[mNumber][7] = -1;
            minutes[mNumber][8] = data[4];
            mNumber++;
        }
        else if (fabs(rate*teeth[data[0]]/teeth[data[4]] - 2) < errorT){
            hours[hNumber][0] = data[0];
            hours[hNumber][1] = -1;
            hours[hNumber][2] = data[1];
            hours[hNumber][3] = -1;
            hours[hNumber][4] = data[2];
            hours[hNumber][5] = -1;
            hours[hNumber][6] = data[3];
            hours[hNumber][7] = -1;
            hours[hNumber][8] = data[4];
            hNumber++;
        }
        /*A-BC-D-E*/
        if (fabs(-rate*teeth[data[0]]*teeth[data[2]]/(teeth[data[1]]*teeth[data[4]]) - 24) < errorT){
            minutes[mNumber][0] = data[0];
            minutes[mNumber][1] = -1;
            minutes[mNumber][2] = data[1];
            minutes[mNumber][3] = data[2];
            minutes[mNumber][4] = -1;
            minutes[mNumber][5] = data[3];
            minutes[mNumber][6] = -1;
            minutes[mNumber][7] = data[4];
            mNumber++;
        }
        else if (fabs(-rate*teeth[data[0]]*teeth[data[2]]/(teeth[data[1]]*teeth[data[4]]) - 2) < errorT){
            hours[hNumber][0] = data[0];
            hours[hNumber][1] = -1;
            hours[hNumber][2] = data[1];
            hours[hNumber][3] = data[2];
            hours[hNumber][4] = -1;
            hours[hNumber][5] = data[3];
            hours[hNumber][6] = -1;
            hours[hNumber][7] = data[4];
            hNumber++;
        }
        /*A-B-CD-E*/
        if (fabs(-rate*teeth[data[0]]*teeth[data[3]]/(teeth[data[2]]*teeth[data[4]]) - 24) < errorT){
            minutes[mNumber][0] = data[0];
            minutes[mNumber][1] = -1;
            minutes[mNumber][2] = data[1];
            minutes[mNumber][3] = -1;
            minutes[mNumber][4] = data[2];
            minutes[mNumber][5] = data[3];
            minutes[mNumber][6] = -1;
            minutes[mNumber][7] = data[4];
            mNumber++;
        }
        else if (fabs(-rate*teeth[data[0]]*teeth[data[3]]/(teeth[data[2]]*teeth[data[4]]) - 2) < errorT){
            hours[hNumber][0] = data[0];
            hours[hNumber][1] = -1;
            hours[hNumber][2] = data[1];
            hours[hNumber][3] = -1;
            hours[hNumber][4] = data[2];
            hours[hNumber][5] = data[3];
            hours[hNumber][6] = -1;
            hours[hNumber][7] = data[4];
            hNumber++;
        }
        break;
    case 6:
        /*A-B-C-D-E-F*/
        if (fabs(-rate*teeth[data[0]]/teeth[data[5]] - 24) < errorT){
            minutes[mNumber][0] = data[0];
            minutes[mNumber][1] = -1;
            minutes[mNumber][2] = data[1];
            minutes[mNumber][3] = -1;
            minutes[mNumber][4] = data[2];
            minutes[mNumber][5] = -1;
            minutes[mNumber][6] = data[3];
            minutes[mNumber][7] = -1;
            minutes[mNumber][8] = data[4];
            minutes[mNumber][9] = -1;
            minutes[mNumber][10] = data[5];
            mNumber++;
        }
        else if (fabs(-rate*teeth[data[0]]/teeth[data[5]] - 2) < errorT){
            hours[hNumber][0] = data[0];
            hours[hNumber][1] = -1;
            hours[hNumber][2] = data[1];
            hours[hNumber][3] = -1;
            hours[hNumber][4] = data[2];
            hours[hNumber][5] = -1;
            hours[hNumber][6] = data[3];
            hours[hNumber][7] = -1;
            hours[hNumber][8] = data[4];
            hours[hNumber][9] = -1;
            hours[hNumber][10] = data[5];
            hNumber++;
        }
        /*A-BC-D-E-F*/
        if (fabs(rate*teeth[data[0]]*teeth[data[2]]/(teeth[data[1]]*teeth[data[5]]) - 24) < errorT){
            minutes[mNumber][0] = data[0];
            minutes[mNumber][1] = -1;
            minutes[mNumber][2] = data[1];
            minutes[mNumber][3] = data[2];
            minutes[mNumber][4] = -1;
            minutes[mNumber][5] = data[3];
            minutes[mNumber][6] = -1;
            minutes[mNumber][7] = data[4];
            minutes[mNumber][8] = -1;
            minutes[mNumber][9] = data[5];
            mNumber++;
        }
        else if (fabs(rate*teeth[data[0]]*teeth[data[2]]/(teeth[data[1]]*teeth[data[5]]) - 2) < errorT){
            hours[hNumber][0] = data[0];
            hours[hNumber][1] = -1;
            hours[hNumber][2] = data[1];
            hours[hNumber][3] = data[2];
            hours[hNumber][4] = -1;
            hours[hNumber][5] = data[3];
            hours[hNumber][6] = -1;
            hours[hNumber][7] = data[4];
            hours[hNumber][8] = -1;
            hours[hNumber][9] = data[5];
            hNumber++;
        }
        /*A-B-CD-E-F*/
        if (fabs(rate*teeth[data[0]]*teeth[data[3]]/(teeth[data[2]]*teeth[data[5]]) - 24) < errorT){
            minutes[mNumber][0] = data[0];
            minutes[mNumber][1] = -1;
            minutes[mNumber][2] = data[1];
            minutes[mNumber][3] = -1;
            minutes[mNumber][4] = data[2];
            minutes[mNumber][5] = data[3];
            minutes[mNumber][6] = -1;
            minutes[mNumber][7] = data[4];
            minutes[mNumber][8] = -1;
            minutes[mNumber][9] = data[5];
            mNumber++;
        }
        else if (fabs(rate*teeth[data[0]]*teeth[data[3]]/(teeth[data[2]]*teeth[data[5]]) - 2) < errorT){
            hours[hNumber][0] = data[0];
            hours[hNumber][1] = -1;
            hours[hNumber][2] = data[1];
            hours[hNumber][3] = -1;
            hours[hNumber][4] = data[2];
            hours[hNumber][5] = data[3];
            hours[hNumber][6] = -1;
            hours[hNumber][7] = data[4];
            hours[hNumber][8] = -1;
            hours[hNumber][9] = data[5];
            hNumber++;
        }
        /*A-B-C-DE-F*/
        if (fabs(rate*teeth[data[0]]*teeth[data[4]]/(teeth[data[3]]*teeth[data[5]]) - 24) < errorT){
            minutes[mNumber][0] = data[0];
            minutes[mNumber][1] = -1;
            minutes[mNumber][2] = data[1];
            minutes[mNumber][3] = -1;
            minutes[mNumber][4] = data[2];
            minutes[mNumber][5] = -1;
            minutes[mNumber][6] = data[3];
            minutes[mNumber][7] = data[4];
            minutes[mNumber][8] = -1;
            minutes[mNumber][9] = data[5];
            mNumber++;
        }
        else if (fabs(rate*teeth[data[0]]*teeth[data[4]]/(teeth[data[3]]*teeth[data[5]]) - 2) < errorT){
            hours[hNumber][0] = data[0];
            hours[hNumber][1] = -1;
            hours[hNumber][2] = data[1];
            hours[hNumber][3] = -1;
            hours[hNumber][4] = data[2];
            hours[hNumber][5] = -1;
            hours[hNumber][6] = data[3];
            hours[hNumber][7] = data[4];
            hours[hNumber][8] = -1;
            hours[hNumber][9] = data[5];
            hNumber++;
        }
        /*A-BC-DE-F*/
        if (fabs(-rate*teeth[data[0]]*teeth[data[2]]*teeth[data[4]]/(teeth[data[1]]*teeth[data[3]]*teeth[data[5]]) - 24) < errorT){
            minutes[mNumber][0] = data[0];
            minutes[mNumber][1] = -1;
            minutes[mNumber][2] = data[1];
            minutes[mNumber][3] = data[2];
            minutes[mNumber][4] = -1;
            minutes[mNumber][5] = data[3];
            minutes[mNumber][6] = data[4];
            minutes[mNumber][7] = -1;
            minutes[mNumber][8] = data[5];
            mNumber++;
        }
        else if (fabs(-rate*teeth[data[0]]*teeth[data[2]]*teeth[data[4]]/(teeth[data[1]]*teeth[data[3]]*teeth[data[5]]) - 2) < errorT){
            hours[hNumber][0] = data[0];
            hours[hNumber][1] = -1;
            hours[hNumber][2] = data[1];
            hours[hNumber][3] = data[2];
            hours[hNumber][4] = -1;
            hours[hNumber][5] = data[3];
            hours[hNumber][6] = data[4];
            hours[hNumber][7] = -1;
            hours[hNumber][8] = data[5];
            hNumber++;
        }
        break;
    default:
        exit(1);
        break;
    }
    #ifdef DBUGM
        printf("Check done\n");
    #endif
    return;
}
