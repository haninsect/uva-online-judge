/* 1080 - My Bad _ SImulate */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxSize 1024
typedef struct {
    int value;
    int type; /*0 And 1 or 2 xor 3 not*/
    int failure; /*0 no fault 1 invert 2 stuck 1 3 stuck 0*/

    int is[2][2];

} Gate;
int iss, gn, osn;
int isignals[MaxSize][8];
Gate gates[20];
int osignals[MaxSize][20];
int os[20];
int isn;
void Read()
{
    int i, j;
    for(i = 0; i < gn; i++){
        gates[i].failure = 0;
        gates[i].value = 0;
        gates[i].is[1][0] = 0;
        gates[i].is[1][1] = 0;
        char it[5], is1[5], is2[5] = {'\0'};
        scanf("%s%s", &it, &is1);
        if(it[0] == 'a') gates[i].type = 0;
        else if(it[0] == 'o') gates[i].type = 1;
        else if(it[0] == 'x') gates[i].type = 2;
        else if(it[0] == 'n') gates[i].type = 3;

        if(is1[0] == 'i') gates[i].is[0][0] = 0;
        else if(is1[0] == 'g') gates[i].is[0][0] = 1;
        if(strlen(is1) == 3){
            gates[i].is[0][1] = 10 + is1[2] - '1';
        }
        else gates[i].is[0][1] = is1[1] - '1';

        if(gates[i].type != 3) {
            scanf("%s", &is2);
            if(is2[0] == 'i') gates[i].is[1][0] = 0;
            else if(is2[0] == 'g') gates[i].is[1][0] = 1;
            if(strlen(is2) == 3){
                gates[i].is[1][1] = 10 + is2[2] - '1';
            }
            else gates[i].is[1][1] = is2[1] - '1';
        }
    }
    for(i = 0; i < osn; i++){
        scanf("%d", &os[i]);
        os[i]--;
    }
    scanf("%d", &isn);
    for(i = 0; i < isn; i++){
        for(j = 0; j < iss; j++){
            scanf("%d", &isignals[i][j]);
        }
        for(j = 0; j < osn; j++){
            scanf("%d", &osignals[i][j]);
        }
    }
}
int FindValue(int type, int src, int index, int src2, int index2, int signal)
{
    int i1, i2;
    if(src == 1) {
        if(gates[index].value == -1) return -1;
        i1 = gates[index].value;
    }
    if(src2 == 1) {
        if(gates[index2].value == -1) return -1;
        i2 = gates[index2].value;
    }
    if(src == 0) i1 = isignals[signal][index];
    if(src2 == 0) i2 = isignals[signal][index2];
    /*printf("i1, i2: %d %d\n", i1, i2);*/
    switch (type){
    case 0:
        return i1&i2;
        break;
    case 1:
        return i1|i2;
        break;
    case 2:
        if(i1 == i2) return 0;
        else return 1;
        break;
    case 3:
        return !i1;
        break;
    }
}
int Simulate()
{
    int i, j;
    for(i = 0; i < gn; i++){
        gates[i].value = -1;
    }

    for(i = 0; i < isn; i++){
        int done = 0;
        while(done != 1){
            done = 1;
            for(j = 0; j < gn; j++){
                /*printf("%d: %d\n", j, gates[j].failure);*/
                int ivt = 0;
                if(gates[j].failure == 1) ivt = 1;
                else if(gates[j].failure == 2) {
                    if(gates[j].value != 1)done = 0;
                    gates[j].value = 1;
                    continue;
                }
                else if(gates[j].failure == 3) {
                    if(gates[j].value != 0)done = 0;
                    gates[j].value = 0;
                    continue;
                }
                int tmp = FindValue(gates[j].type, gates[j].is[0][0], gates[j].is[0][1], gates[j].is[1][0], gates[j].is[1][1], i);
                if(tmp == -1){
                    continue;
                }
                if(ivt == 1) tmp = !tmp;
                if(gates[j].value != tmp) {
                    done = 0;
                    gates[j].value = tmp;
                }
                /*printf("%d, %d: %d\n", i, j, gates[j].value);*/
            }
            /*printf("%d Done: %d\n", gn, done);*/
        }
        for(j = 0; j < osn; j++){
           /* printf("%d: Gate %d: %d/%d\n", i, j, gates[os[j]].value, osignals[i][j]);*/
            if(osignals[i][j] != gates[os[j]].value) return 0;
        }
    }
    return 1;

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
        scanf("%d%d%d", &iss, &gn, &osn);
        if(iss == 0) break;
        Read();
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int count = 0, ansi, ansj;
        if(Simulate()){
            printf("Case %d: No faults detected\n", caseNumber);
            caseNumber++;
            continue;
        }
        for(i = 0; i < gn; i++){
            for(j = 0; j < 3; j++){
                gates[i].failure = j+1;
                /*Simulate*/
                /*rintf("\n--Simulate %d %d\n", i, j);*/
                if(Simulate()){
                    ansi = i;
                    ansj = j+1;
                    count++;
                }
            /*printf("Count: %d\n", count);*/
            }
            gates[i].failure = 0;
        }
        /*printf("Count: %d\n", count);*/
        if(count != 1 ) printf("Case %d: Unable to totally classify the failure\n", caseNumber);
        else {
            printf("Case %d: Gate %d is failing; ", caseNumber, ansi+1);
            if(ansj == 1) printf("output inverted\n");
            else if(ansj == 2) printf("output stuck at 1\n");
            else if(ansj == 3) printf("output stuck at 0\n");
        }
        caseNumber++;
    }
    return 0;
}
