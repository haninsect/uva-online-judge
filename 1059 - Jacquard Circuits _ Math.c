/*1059 - Jacquard Circuits _ Math*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM
#define LargeINT 1000000000
#define errorT 0.00005
typedef struct {
    int x1, y1;
    int x2, y2;
} segment;
int gcd(int m, int n) ;
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){

        int n, level;
        scanf("%d%d", &n, &level);
        if(n == 0)break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int fromx = 0, fromy = 0;
        scanf("%d%d", &fromx, &fromy);
        int offsetX = fromx, offsetY = fromy;
        segment lines[n+1];
        int lineNumber = 0;
        for(i = 1; i < n; i++){
            int a, b;
            scanf("%d%d", &a, &b);
            if(a < offsetX) offsetX = a, offsetY = b;
            else if(a == offsetX){
                if(b < offsetY) offsetY = b;
            }
            if(lineNumber != 0){
                if(CheckCollinear()){
                    lines[lineNumber-1].x2 = a;
                    lines[lineNumber-1].y2 = b;
                }
            }
            lines[lineNumber].x1 = fromx;
            lines[lineNumber].y1 = fromy;
            lines[lineNumber].x2 = a;
            lines[lineNumber].y2 = b;
            fromx = a;
            fromy = b;
            lineNumber++;
        }
        if(CheckCollinear()){
            lines[lineNumber-1].x2 = 0;
            lines[lineNumber-1].y2 = 0;
        }
        else {
            lines[lineNumber].x1 = fromx;
            lines[lineNumber].y1 = fromy;
            lines[lineNumber].x2 = lines[0].x1;
            lines[lineNumber].y2 = lines[0].y1;
            lineNumber++;
        }
        #ifdef DBUGM
            for(i = 0; i < lineNumber; i++){
                printf("%d %d -> %d %d\n", lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2 );
            }
        #endif
        /*Find the smallest one*/
        int tmpgcd = 0;
        for(i = 0; i < lineNumber; i++){
            if(tmpgcd == 0) {
                if(lines[i].x1 != 0) tmpgcd = gcd(lines[i].x1 - offsetX, lines[i].x1 - offsetX);
                else if(lines[i].y1 != 0 )tmpgcd = gcd(lines[i].y1 - offsetY, lines[i].y1 - offsetY);
            }
            if(lines[i].x1 != 0)tmpgcd = gcd(tmpgcd, lines[i].x1 - offsetX);
            if(lines[i].y1 != 0)tmpgcd = gcd(tmpgcd, lines[i].y1 - offsetY);
        }
        printf("%d\n", tmpgcd);
        /*Calculate the level 1~m*/
        unsigned long long int ans = 0;


        printf("Case %d: %llu\n", caseNumber++, ans);
    }
    return 0;
}
int gcd(int m, int n) {
    printf("!%d %d\n", m, n);
    while(n != 0) {
        int r = m % n;
        m = n;
        n = r;
    }
    return m;
}

int CheckCollinear()
{
    return 0;
}
