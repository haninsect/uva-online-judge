/*1046 - Workshops_Greedy*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define DBUGM5
#define LargeINT 1000000000
int cmp(const void *a, const void *b);
typedef struct{
    int duration, capacity;
} room;
typedef struct{
    int pNumber;
    int time;
} people;

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){
        int p, r;
        scanf("%d", &p);
        if(p == 0)break;
        #ifdef DBUGM
           printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        people peoples[p];
        for(i = 0; i < p; i++){
            scanf("%d %d", &peoples[i].pNumber, &peoples[i].time);
        }
        scanf("%d", &r);
        room rooms[r];
        for(i = 0; i < r; i++){
            int h, m;
            scanf("%d %d:%d", &rooms[i].capacity, &h, &m);
            rooms[i].duration = (h-14)*60 + m;
        }
        qsort(peoples, p, sizeof(people), cmp);
        int restP = p, restN = 0;
        for(i = 0; i < p; i++) restN += peoples[i].pNumber;
        int validR[r];
        for(i = 0; i < r; i++) validR[i] = 1;

        /*Start Greedy*/
        for(i = 0; i < p; i++){
            int chooseR = -1, minT = LargeINT;
            for(j = 0; j < r; j++){
                if(validR[j] == 0)continue;
                if(peoples[i].pNumber > rooms[j].capacity || peoples[i].time > rooms[j].duration) continue;
                if(minT > rooms[j].duration) minT = rooms[j].duration, chooseR = j;
            }
            if(chooseR != -1){
                validR[chooseR] = 0;
                restP -= 1;
                restN -= peoples[i].pNumber;
            }
        }
        printf("Trial %d: %d %d\n\n", caseNumber, restP, restN);
        caseNumber++;
    }
    return 0;
}
int cmp(const void *a, const void *b)
{
    people *pa = a, *pb = b;
    if(pa->pNumber == pb->pNumber){
        return  pb->time - pa->time;
    }
    else return pb->pNumber - pa->pNumber;
}
