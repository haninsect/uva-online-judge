/*1059 - Jacquard Circuits _ Math*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define errorT 0.00005
#define getCase 100000000
typedef struct {
    int x1, y1;
    int x2, y2;
    int dot;
} segment;

int gcd(int m, int n) {
    while(n != 0) {
        int r = m % n;
        m = n;
        n = r;
    }
    return m;
}

int CheckCollinear(segment line, int  x, int y)
{
    long long int dx1 = line.x2 - line.x1;
    long long int dy1 = line.y2 - line.y1;
    long long int dx2 = x-line.x1;
    long long int dy2 = y-line.y1;
    if( (dx1)*(dy2) == (dy1)*(dx2) ) return 1;
    else return 0;
}
unsigned long long int CalculateAreaX2(int size, segment polygon[size])
{
    #ifdef DBUGM
        printf("---Calculate area---\n");
    #endif
    long long int area = 0;
    int i;
    for(i = 0; i < size; i++){
        long long int x1 = polygon[i].x1;
        long long int x2 = polygon[i].x2;
        long long int y1 = polygon[i].y1;
        long long int y2 = polygon[i].y2;
        area += x1*y2 - x2*y1;
        #ifdef DBUGM
            printf("(%d %d) -> (%d %d) : +%lld  = %lld\n", polygon[i].x1, polygon[i].y1, polygon[i].x2, polygon[i].y2, x1*y2 - x2*y1, area);
        #endif
    }
    if(area < 0) area = -area;
    return (unsigned long long int)area;
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

        int n;
        unsigned long long int level;
        scanf("%d%llu", &n, &level);
        if(n == 0)break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        int fromx = 0, fromy = 0;
        scanf("%d%d", &fromx, &fromy);
        int offsetX = fromx, offsetY = fromy;
        segment lines[n*2];
        int lineNumber = 0;
        for(i = 1; i < n; i++){
            int a, b;
            scanf("%d%d", &a, &b);
            if(a < offsetX) offsetX = a, offsetY = b;
            else if(a == offsetX){
                if(b < offsetY) offsetY = b;
            }
            if(lineNumber != 0){
                if(CheckCollinear(lines[lineNumber-1], a, b)){
                    lines[lineNumber-1].x2 = a;
                    lines[lineNumber-1].y2 = b;
                    fromx = a;
                    fromy = b;
                    continue;
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
        if(CheckCollinear(lines[lineNumber-1], lines[0].x1, lines[0].y1)){
            lines[lineNumber-1].x2 = lines[0].x1;
            lines[lineNumber-1].y2 = lines[0].y1;
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
                if(lines[i].x1 - offsetX != 0) tmpgcd = abs(lines[i].x1 - offsetX);
                else if(lines[i].y1 - offsetY != 0 )tmpgcd = abs(lines[i].y1 - offsetY);
            }
            if(lines[i].x1 - offsetX != 0)tmpgcd = gcd(tmpgcd, abs(lines[i].x1 - offsetX));
            if(lines[i].y1 - offsetY != 0)tmpgcd = gcd(tmpgcd, abs(lines[i].y1 - offsetY));
        }
        #ifdef DBUGM
            printf("GCD: %d\n",tmpgcd);
        #endif
        unsigned long long int dots = 0;
        for(i = 0; i < lineNumber; i++){
            lines[i].x1 = (lines[i].x1 - offsetX)/tmpgcd;
            lines[i].x2 = (lines[i].x2 - offsetX)/tmpgcd;
            lines[i].y1 = (lines[i].y1 - offsetY)/tmpgcd;
            lines[i].y2 = (lines[i].y2 - offsetY)/tmpgcd;
            int dx = abs(lines[i].x2 - lines[i].x1) , dy = abs(lines[i].y2 - lines[i].y1);
            if(dx == 0) lines[i].dot = dy -1;
            else if(dy == 0) lines[i].dot = dx -1;
            else lines[i].dot = gcd(dy, dx)-1;
            dots += (lines[i].dot +1);
        }
        unsigned long long int area0x2 = CalculateAreaX2(lineNumber, lines);
        #ifdef DBUGM
            for(i = 0; i < lineNumber; i++){
                printf("%d %d -> %d %d\n", lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2 );
            }
        #endif
        /*Calculate the level 1~m*/
        printf("Case %d: %llu\n", caseNumber++, (area0x2*((((level*(level+1))/2)*(2*level+1))/3))/2-(dots*level*(level+1))/4+level);
    }
    return 0;
}

