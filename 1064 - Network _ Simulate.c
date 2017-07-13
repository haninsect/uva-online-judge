/*1064 - Network _ Simulate */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define MaxSize 64*1000+1
#define errorT 0.00005
typedef struct {
    int message;
    int from, to;
} packet;
int mFlag[5][MaxSize];
int minBuffer;
int message, pNumber;
int mSize[5];
packet packets[1000];

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){
        scanf("%d%d", &message, &pNumber);
        if(message == 0) break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int pArray[message];
        for(i = 0; i < message; i++) scanf("%d", &mSize[i]), pArray[i] = i;
        for(i = 0; i < pNumber; i++){
            scanf("%d%d%d", &packets[i].message, &packets[i].from, &packets[i].to);
            packets[i].message--;
        }
        minBuffer = LargeINT;
        Permutation(message, pArray, 0, message-1);
        /*if(caseNumber > 1) printf("\n");*/
        printf("Case %d: %d\n\n", caseNumber++, minBuffer);

    }
    return 0;
}
int Simulate(int size, int order[size])
{
    int i;
    int mp[message];
    for(i = 0; i < message; i++) mp[i] = 1;
    int now = 0, tmpBuffer = 0, max = 0;
    for(i = 0; i < pNumber && now < message; i++){
        if(packets[i].message == order[now] && packets[i].from == mp[order[now]]){ /*Build*/
            mp[order[now]] = packets[i].to +1;
            if(mp[order[now]] > mSize[order[now]]) now++;
            if(now >= message) break;

            while(mFlag[order[now]][ mp[order[now]] ] != 0){ /*The next packet is in the buffer*/
                tmpBuffer -= mFlag[order[now]][ mp[order[now]] ] - mp[order[now]] +1;
                mp[order[now]] = mFlag[order[now]][ mp[order[now]] ]+1;
                if(mp[order[now]] > mSize[order[now]]) now++;
                if(now >= message) break;
            }
        }
        else { /*Store into buffer*/
            mFlag[packets[i].message][packets[i].from] = packets[i].to;
            tmpBuffer += packets[i].to - packets[i].from + 1;
            if(tmpBuffer > max) max = tmpBuffer;
        }
    }
    return max;
}
void Permutation(int size, int data[size], int a, int b)
{
    int i, j;
	if(a == b)
	{
	    #ifdef DBUGM
           printf("Find Permutation:%d\n", size);
           for(i = 0; i < size; i++) printf("%d ", data[i]);
           printf("\n");
        #endif
	    for(i = 0; i < 5; i++)
            for(j = 0; j < MaxSize; j++)
                mFlag[i][j] = 0;
        int tmpAns = Simulate(size, data);
	    if(tmpAns < minBuffer) minBuffer = tmpAns;
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
