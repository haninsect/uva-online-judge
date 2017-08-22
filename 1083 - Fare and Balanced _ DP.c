#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxSize 50005
int size;
typedef struct {
    int ei;
    int el;
    int to;
} edge;
typedef struct {
    int isBalanced;
    int isIncrease;
    int len;
} status;
typedef struct {
    int en;
    edge *es;
} node;
typedef struct {
    int ei;
    int increase;
} answer;
int an;
int tn;
answer output[MaxSize];
node ns[MaxSize];
status sts[MaxSize];
int fail;
int indegree[MaxSize];
int lp;
void Read()
{
    int i, j;
    an = 0, fail = 0, lp = 0;
    for(i = 1; i < MaxSize; i++) {
        ns[i].en = 0;
        sts[i].isBalanced = 0;
        sts[i].isIncrease = 0;
        sts[i].len = 0;
        indegree[i] = 0;
    }
    int fx[MaxSize] = {0}, to[MaxSize] = {0}, len[MaxSize] = {0};
    int count[MaxSize] = {0};
    for(i = 1; i <= tn; i++) {
        scanf("%d%d%d", &fx[i], &to[i], &len[i]);
        count[fx[i]]++;
        indegree[to[i]]++;
    }
    for(i = 1; i <= size; i++){
        ns[i].es = (edge*) malloc(sizeof(edge)*(count[i]+1));
        /*printf("%d %d\n", i, count[i]+1);*/
    }
    for(i = 1; i <= tn; i++) {
        /*printf("%d\n", fx[i]);*/
        ns[fx[i]].en++;
        ns[fx[i]].es[ns[fx[i]].en].ei = i;
        ns[fx[i]].es[ns[fx[i]].en].to = to[i];
        ns[fx[i]].es[ns[fx[i]].en].el = len[i];
        /*printf("%d\n", i);*/
    }
    /*printf("??");*/
}
void Balance(int index, int tmpl)
{
    if(sts[index].isBalanced == 1) return;
    int value = 0, i, flag = 0;
    for(i = 1; i <= ns[index].en; i++){
        Balance(ns[index].es[i].to, tmpl + ns[index].es[i].el);
        if(fail == 1) return;
        if(ns[index].es[i].el + sts[ns[index].es[i].to].len != value) {
            if(value != 0) flag = 1;
            value = ns[index].es[i].el + sts[ns[index].es[i].to].len;
        }
        if(sts[ns[index].es[i].to].isIncrease) sts[index].isIncrease = 1;
    }
    if(flag == 1){
            /*printf("need balance: %d %d\n", index, tmpl);*/
        for(i = 1; i <= ns[index].en; i++){
            /*printf("%d %d %d\n", ns[index].es[i].el, sts[ns[index].es[i].to].len, tmpl);*/
            if(ns[index].es[i].el + sts[ns[index].es[i].to].len + tmpl != lp){
                if(sts[ns[index].es[i].to].isIncrease == 1) {
                    fail = 1;
                    return;
                }
                else {
                    output[++an].ei = ns[index].es[i].ei;
                    output[an].increase = lp - (ns[index].es[i].el + sts[ns[index].es[i].to].len + tmpl);
                }
            }
        }
        sts[index].isBalanced = 1;
        sts[index].isIncrease = 1;
        sts[index].len = lp - tmpl;
    }
    else {
        sts[index].isBalanced = 1;
        sts[index].len = value;
    }
    return;
}
int FindLong()
{
    int i, j;
    int count[MaxSize] = {0};
    int queue[MaxSize] = {0}, qp = 1, qs = 0, key[MaxSize] = {0};
    queue[++qs] = 1;
    for(i = 1; i <= size; i++){
        if(qp > qs) while(1);
        for(j = 1; j <= ns[queue[qp]].en; j++){
            count[ns[queue[qp]].es[j].to]++;
            if(key[ns[queue[qp]].es[j].to] < key[queue[qp]] + ns[queue[qp]].es[j].el) key[ns[queue[qp]].es[j].to] = key[queue[qp]] + ns[queue[qp]].es[j].el;
            if(count[ns[queue[qp]].es[j].to] == indegree[ns[queue[qp]].es[j].to]){
                queue[++qs] = ns[queue[qp]].es[j].to;
            }
        }
        qp++;
    }
    return key[size];
}
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){
        scanf("%d%d", &size, &tn);
        if(size == 0) break;
        Read();
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        lp = FindLong();
        Balance(1, 0);
        if(fail == 1) printf("Case %d: No solution\n", caseNumber);
        else {
            printf("Case %d: %d %d\n", caseNumber, an, sts[1].len);
            for(i = 1; i <= an; i++) printf("%d %d\n", output[i].ei, output[i].increase);
        }
        caseNumber++;
    }
    return 0;
}
