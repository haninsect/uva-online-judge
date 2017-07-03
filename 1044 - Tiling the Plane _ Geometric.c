/*1044 - Tiling the Plane _ Geometric*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define MaxSize 2000
int Direction(char d);
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i, j, k, l, m, n;
    int caseNumber = 1;
    while(1){

        int n;
        scanf("%d", &n);
        if(n == 0)break;

        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int side[MaxSize];
        int sp = 0;
        for(i = 0; i < n; i++){
            char dir = getchar();
            while( dir == '\n' || dir == ' ' )dir = getchar();
            int len;
            scanf("%d", &len);
            int di = Direction(dir);
            for(j = 0; j < len; j++){
                side[sp++] = di;
            }
        }
        if(sp >= MaxSize)exit(1);
        /*ShowSide(sp, side);*/
        int isPossible = 0;
        for(i = 0; i < sp-5 && isPossible == 0; i++){
            for(j = i+1; j < sp-4 && isPossible == 0; j++){
                for(k = j+1; k < sp-3 && isPossible == 0; k++){
                    for(l = k+1; l < sp-2 && isPossible == 0; l++){
                        for(m = l+1; m < sp-1 && isPossible == 0; m++){
                            for(n = m+1; n < sp && isPossible == 0; n++){
                                isPossible = Check(sp, side, i, j, k, l, m, n);
                            }
                        }
                    }
                }
            }
        }
        printf("Polygon %d: ", caseNumber);
        if(isPossible) printf("Possible\n");
        else printf("Impossible\n");
        caseNumber++;
    }
    return 0;
}
int Check(int size, int side[MaxSize], int a, int b, int c, int d, int e, int f)
{
    int i, j;
    if(b-a != e-d) return 0;
    for(i = 0; i < b-a; i++){
        if(side[a+i] != -side[(e-1-i + size)%size]) return 0;
    }
    if(c-b != f-e) return 0;
    for(i = 0; i < c-b; i++){
        if(side[b+i] != -side[(f-1-i + size)%size]) return 0;
    }
    if(d-c != abs(a+size-f)) return 0;
    for(i = 0; i < d-c; i++){
        if(side[c+i] != -side[(a-1-i + size)%size]) return 0;
    }
    /*
    for(i = 0; i < size; i++) {
        if(i == a || i == b || i ==c || i ==d || i ==e || i ==f) printf("%2d ", i);
        else printf("   ");
    }
    printf("\n");
    */
    return 1;
}
int Direction(char d)
{
    switch(d){
        case 'N':
            return 1;
            break;
        case 'E':
            return 2;
            break;
        case 'S':
            return -1;
            break;
        case 'W':
            return -2;
            break;
        default:
            exit(1);
            break;
    }
}
void ShowSide(int sp, int side[MaxSize])
{
    int i;
    for(i = 0; i < sp; i++) printf("%2d ", i);
    printf("\n");
    for(i = 0; i < sp; i++){
        if(side[i] == 1) printf(" N ");
        else if(side[i] == 2) printf(" E ");
        else if(side[i] == -1) printf(" S ");
        else if(side[i] == -2) printf(" W ");
    }
    printf("\n");
}
