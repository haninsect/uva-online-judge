/*12086 - Potentiometers _ SegmentTree*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define MaxSize 800000
#define ArraySize 300020
typedef struct {
    int leftP, rightP;
    int sum;
} node;
int array[ArraySize];
node sTree[MaxSize];
int powInt(int a, int p);
int SetValue(int x, int v, int tSize, node tree[tSize], int index);
int SumInterval(int l, int r, int tSize, node tree[tSize], int index);
void BuildSegmentTree(int n, int array[n], int tSize, node tree[tSize]);

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){
        int n;
        scanf("%d", &n);
        if(n == 0)break;

        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int tSize = 1;
        while(1){
            tSize *= 2;
            if(tSize >= n) {
                tSize = tSize*2 -1;
                break;
            }
        }

        for(i = 0; i < MaxSize; i++){
            sTree[i].leftP = i;
            sTree[i].rightP = i;
            sTree[i].sum = 0;
        }
        for(i = 0; i < n; i++) scanf("%d", &array[i]);
        BuildSegmentTree(n, array, tSize, sTree);
        #ifdef DBUGM
            printf("n: %d, Tree Size: %d\n", n, tSize);
            ShowTree(tSize, sTree);
        #endif
        if(caseNumber > 1) printf("\n");
        printf("Case %d:\n", caseNumber);
        while(1){
            char inst[5] = {'\0'};
            scanf("%s", &inst);
            int a, b;
            if(inst[0] == 'E')break;
            else {
                scanf("%d %d", &a, &b);
                if( inst[0] == 'S'){
                    SetValue(a-1, b, tSize, sTree, 0);
                }
                else if(inst[0] == 'M'){
                    printf("%d\n", SumInterval(a-1, b-1, tSize, sTree, 0));
                }
                /*else exit(1);*/
            }
        }
        caseNumber++;

    }
    return 0;
}
int SetValue(int x, int v, int tSize, node tree[tSize], int index)
{
    #ifdef DBUGM
       printf("Traveling %d:\n  Set %d to value %d\n", index, x, v);
    #endif
    int diff;
    if(x == tree[index].leftP && x == tree[index].rightP) {
        diff = v - tree[index].sum;
        tree[index].sum = v;
        return diff;
    }

    if(x <= (tree[index].rightP - tree[index].leftP)/2 + tree[index].leftP) {
        diff = SetValue(x, v, tSize, tree, index*2 + 1);
    }
    else {
        diff = SetValue(x, v, tSize, tree, index*2 + 2);
    }
    tree[index].sum += diff;
    return diff;


}
int SumInterval(int l, int r, int tSize, node tree[tSize], int index)
{
    #ifdef DBUGM
       printf("Traveling %d:\n  Measure Interval: [%d, %d]\n", index, l, r);
    #endif
    if(l == tree[index].leftP && r == tree[index].rightP) return tree[index].sum;
    int tmpSum = 0;
    if(l <= (tree[index].rightP - tree[index].leftP)/2 + tree[index].leftP) {
        if( r >= (tree[index].rightP - tree[index].leftP)/2 + tree[index].leftP +1){
            tmpSum +=  SumInterval(l, (tree[index].rightP - tree[index].leftP)/2 + tree[index].leftP, tSize, tree, index*2+1);
            tmpSum +=  SumInterval((tree[index].rightP - tree[index].leftP)/2 + tree[index].leftP +1, r, tSize, tree, index*2+2);
        }
        else tmpSum +=  SumInterval(l, r, tSize, tree, index*2+1);
    }
    else tmpSum +=  SumInterval(l, r, tSize, tree, index*2+2);

    return tmpSum;
}
void BuildSegmentTree(int n, int array[n], int tSize, node tree[tSize])
{
    int i, j;
    int Leaf = 1;
    while(1){
        Leaf *= 2;
        if(Leaf >= n) {
            Leaf = Leaf-1;
            break;
        }
    }
    #ifdef DBUGM
        printf("NonLeaf: %d\n", Leaf);
    #endif
    /*leaf*/
    for(i = Leaf; i < tSize; i++){
        tree[i].leftP = i - Leaf;
        tree[i].rightP = i - Leaf;
        if(i - Leaf < n) tree[i].sum = array[i - Leaf];
        else tree[i].sum = 0;
    }
    for(i = Leaf-1; i >= 0; i--){
        tree[i].leftP = tree[i*2 + 1].leftP;
        tree[i].rightP = tree[i*2 + 2].rightP;
        tree[i].sum = tree[i*2 + 1].sum + tree[i*2 + 2].sum;
    }
}
void ShowTree(int tSize, node tree[tSize])
{
    int i, j;
    for(i = 0; i < tSize; i++){
        printf("%d: [%d, %d] = %d\n", i, tree[i].leftP, tree[i].rightP, tree[i].sum);
    }
}
int powInt(int a, int p)
{
    int i;
    int r = 1;
    for(i = 0; i < p; i++){
        r *= a;
    }
    return r;
}
