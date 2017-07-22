/*1072 - Huffman Codes _ BAB*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxSize 25
#define MaxNode 2048
typedef struct {
    int isValid;
    int frequency;
} node;
int size;
char input[MaxSize][MaxSize*MaxSize*MaxSize];
node tree[MaxNode];
int maxIndex;
int ans;
void Read()
{
    int i, j;
    /*Initial*/
    for(i = 0; i < MaxSize; i++){
        for(j = 0; j < MaxSize*MaxSize; j++){
            input[i][j] = '\0';
        }
    }
    for(i = 0; i < MaxNode; i++){
        tree[i].isValid  = 0;
        tree[i].frequency = 0;
    }
    maxIndex = 0;
    ans = 0;
    /*Read*/
    for(i = 0; i < size; i++){
        scanf("%s", &input[i]);
        if(strlen(input[i]) >= MaxSize*MaxSize) exit(1);
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
int CheckValid(int nowIndex, int value)
{
    if(nowIndex == 1) return 1;
    int level = -1;
    int i, p = 1;
    for(i = 1; i < 32; i++){
        if(p*2 -1 >= nowIndex){
            level = i;
            break;
        }
        else p *= 2;
    }
    if(level == -1) exit(1);
    /*Find the previous level's minimum*/
    for(i = p*2 -1; i >= p; i-- ){
        if(tree[i].isValid){
            if( tree[i].frequency < value) return 0;
            break;
        }
    }
    /*Compared to the same level*/
    if( nowIndex != p){
        int tmp = -1;
        for(i = nowIndex-1; i >= p; i-- ){
            if(tree[i*2+1].isValid){
                if( tree[i*2+1].frequency < value) return 0;
                else return 1;
            }
            else if(tree[i*2].isValid){
                if( tree[i*2].frequency < value) return 0;
                else return 1;
            }
        }
        if(tmp == -1) return 1;
    }
    return 1;

}
void ShowTree()
{
    int i, j, p = 1;
    for(i = 0; i < 32; i++){
        int flag = 0;
        for(j = 2*p-1; j >= p; j--){
            printf("%3d ", tree[j].frequency);
            if(j >= maxIndex) flag = 1;
        }
        printf("\n");
        if(flag == 1) break;
        p*= 2;
    }
    printf("\n");
}
void BranchAndBound(int nowIndex)
{
    #ifdef DBUGM
        printf("\n--- New Branch: %d/%d: %d ---\n", nowIndex, maxIndex, tree[nowIndex].frequency);
    #endif
    if(nowIndex >= maxIndex){
        ans++;
        #ifdef DBUGM
            printf("New ans: %d:\n", ans);
            ShowTree();
        #endif
        if(ans >= 1000000000) while(1);
        return;
    }
    if(tree[nowIndex].isValid == 0){
        BranchAndBound(nowIndex+1);
        return;
    }
    /*End of recursively call*/

    /*Find all possible combination of two children*/
        /*Check that the values in the same level are increasing*/
        /*Check that all the values is smaller than the previous level*/
        /*Recursively call the next index*/
    int i, j;
    int c = 0;
    if(tree[nowIndex*2].isValid == 1) c++;
    if(tree[nowIndex*2+1].isValid == 1) c++;
    if(c == 0) {
        BranchAndBound(nowIndex+1);
        return;
    }
    else if(c == 1){
        printf("Not Huffman tree!\n");
        exit(1);
    }
    else {
        for(i = 1; i <= tree[nowIndex].frequency/2; i++){
            tree[nowIndex*2 +1].frequency = i;
            tree[nowIndex*2].frequency = tree[nowIndex].frequency-i;
            if(CheckValid(nowIndex, tree[nowIndex].frequency-i) == 1) {
                BranchAndBound(nowIndex+1);
            }
            tree[nowIndex*2 +1].frequency = 0;
            tree[nowIndex*2].frequency = 0;
        }
    }
}
int ConstructTree()
{
    /*according input ex: 010101, find the index in tree, set flag 1*/
    /*The index is
                1
        3               2
    7       6       5       4
            .
            .
            .
    */
    int i, j;
    tree[1].isValid = 1;
    for(i = 0; i < size; i++){
        int node = 1;
        if(strlen(input[i]) >= 10) return 0;
        for(j = 0; j < strlen(input[i]); j++){
            if(input[i][j] == '0') node = 2*node +1;
            else node = 2*node;
            tree[node].isValid = 1;
        }
        if(node > maxIndex) maxIndex = node;
    }
    tree[1].frequency = 100;
    return 1;
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
        scanf("%d", &size);
        if(size == 0) break;
        Read();
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        /*Construct tree*/
        int valid = ConstructTree();
        /*Branch And Bound*/
        /*Start from root*/
        if(valid) BranchAndBound(1);
        printf("Case %d: %d\n", caseNumber, ans);
        caseNumber++;
    }
    return 0;
}
