/*1079 - A Careful Approach _ BS*/
#include <stdio.h>
#include <stdlib.h>
#define DBUGM
#define MaxSize 10
int size;
typedef struct {
    int lp, rp;
} interval;
interval its[MaxSize];
int flag = 0;
int mid;
void Read()
{
    int i, j;
    for(i = 1; i <= size; i++){
        scanf("%d%d", &its[i].lp, &its[i].rp);
        its[i].lp *= 60*10;
        its[i].rp *= 60*10;
    }

}
void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
void permute(int index, int n, int a[n+1])
{
    int i, j;
    if(index == n){
        /*Do SomeThing*/
        int nowp = its[a[1]].lp;
        for(i = 2; i <= n; i++){
            if(mid+nowp > its[a[i]].rp)return;
            if(mid+nowp <= its[a[i]].lp) {
                nowp = its[a[i]].lp;
            }
            else nowp = nowp + mid;
        }
        flag = 1;
    }
    else {
        for(i = index; i <= n; i++){
            swap(&a[index], &a[i]);
            permute(index+1, n, a);
            swap(&a[index], &a[i]);
            if(flag == 1) return;
        }
    }
}

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
	int i, j, k , l;
    int caseNumber = 1;

    while(1){
        #ifndef DBUGM
           printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        scanf("%d", &size);
        if(size == 0) break;
        Read();
        int a[size+1];
        flag = 0;
        for(i = 1; i <= size; i++) a[i] = i;
        /*Binary Search*/
        int upper = 1440*60*10, lower = 0;
        while(upper - lower > 1){
            mid = (upper + lower)/2;
            permute(1, size, a);
            if(flag == 0) upper = mid;
            else lower = mid;
            /*printf("%d: %d\n", mid, flag);*/
            flag = 0;
            for(i = 1; i <= size; i++) a[i] = i;
        }
        printf("Case %d: %d:%02d\n", caseNumber, (lower+5)/600, (lower+5 - (lower+5)/600*600)/10);
        caseNumber++;
    }
    return 0;
}
