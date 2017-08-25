/*1086 - The Ministers' Major Mess _ 2SAT */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxPeople 505
#define MaxBill 210
int bills, ps;
typedef struct {
    int vn;
    int bn[5];
    int vote[5];
} people;
people pArray[MaxPeople];
int graph[MaxBill][MaxBill];
int FTime[MaxBill*2];
void Read()
{
    int i, j, k;
    for(i = 1; i < MaxBill; i++){
        for(j = 1; j < MaxBill; j++) graph[i][j] = 0;
    }
    for(i = 1; i <= ps; i++){
        scanf("%d", &pArray[i].vn);
        for(j = 1; j <= pArray[i].vn; j++){
            char c;
            scanf("%d %c", &pArray[i].bn[j], &c);
            if(c == 'y') pArray[i].vote[j] = 1;
            else pArray[i].vote[j] = 0;
        }
    }
}
void ConstructGraph()
{
    int i, j, k;
    for(i = 1; i <= ps; i++){
        if(pArray[i].vn <= 2){
            for(j = 1; j <= pArray[i].vn; j++){
                int from = pArray[i].bn[j] + bills*(!pArray[i].vote[j]);
                int to = pArray[i].bn[j] + bills*(pArray[i].vote[j]);
                graph[from][to] = 1;
            }
        }
        else {
            for(j = 1; j <= pArray[i].vn; j++){
                for(k = 1; k <= pArray[i].vn; k++){
                    if(j == k) continue;
                    int from = pArray[i].bn[j] + bills*(!pArray[i].vote[j]);
                    int to = pArray[i].bn[k] + bills*(pArray[i].vote[k]);
                    graph[from][to] = 1;
                }
            }
        }
    }
}
int DFS(int now, int time, int visit[MaxBill])
{
    int i;
    visit[now] = 1;
    for(i = 1; i <= bills*2; i++){
        if(graph[now][i] == 0 || visit[i] == 1) continue;
        time = DFS(i, time+1, visit);
    }
    FTime[time+1] = now;
    return time+1;
}
int Check(int now, int member[MaxBill], int visit[MaxBill])
{
    int i;
    visit[now] = 1;
    member[now] = 1;
    if(now > bills) {if(member[now-bills]) return 0;}
    else {if(member[now+bills]) return 0;}
    for(i = 1; i <= bills*2; i++){
        if(graph[i][now] == 0 || visit[i] == 1) continue;
        if(Check(i, member, visit) == 0) return 0;
    }
    return 1;
}
int SSCCheck()
{
    int i, j;
    for(i = 1; i < MaxBill*2; i++) FTime[i] = 0;
    int visit[MaxBill] = {0}, nowTime = 0;
    for(i = 1; i <= bills*2; i++){
        if(visit[i] == 0) nowTime = DFS(i, nowTime+1, visit);
    }
    for(i = 1; i < MaxBill; i++) visit[i] = 0;
    int valid = 1;
    for(i = bills*4; i >= 1 && valid == 1; i--){
        if(FTime[i] == 0 || visit[FTime[i]] != 0) continue;
        int member[MaxBill] = {0};
        if(Check(FTime[i], member, visit) == 0) valid = 0;
    }
    return valid;
}
int Solve()
{
    return SSCCheck();
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
        scanf("%d%d", &bills, &ps);
        if(bills == 0 && ps == 0) break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        Read();
        ConstructGraph();
        /*for(i = 1; i <= bills*2; i++){
            for(j = 1; j <= bills*2; j++) printf("%d ", graph[i][j]);
            printf("\n");
        }*/
        if(Solve()){
            /*Find each bill's value*/
            char result[5] = {"-yn?"};
            printf("Case %d: ", caseNumber);
            for(i = 1; i <= bills; i++){
                int ans = 0;
                int tmp = graph[i][i+bills];
                graph[i][i+bills] = 1; /* Xi' -> Xi*/
                if(Solve()) ans += 1;
                graph[i][i+bills] = tmp;
                tmp = graph[i+bills][i];
                graph[i+bills][i] = 1; /* Xi -> Xi'*/
                if(Solve()) ans += 2;
                graph[i+bills][i] = tmp;
                printf("%c", result[ans]);
            }
            printf("\n");
        }
        else printf("Case %d: impossible\n", caseNumber);

        caseNumber++;
    }
    return 0;
}
