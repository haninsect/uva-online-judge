/*1060 - Collecting Luggage _ Geometrix */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
#define LargeINT 1000000000
#define MaxSize 350
#define errorT 0.00005
typedef struct {
    double x1, y1;
    double x2, y2;
} segment;
typedef struct {
    double x, y;
} point;
void ToString(double time, char string[10]);
int Dijkstra(int from, int to, int n, double adjMatrix[n][n]);
double CheckValid(int size, segment lines[size], segment checkp);
void AddPoint(int gSize, segment lines[gSize-2], double adjMatrix[gSize][gSize], point p, int index);
void ConstructGraph(int gSize, segment lines[gSize-2], double adjMatrix[gSize][gSize]);
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){
        int eNumber;
        scanf("%d", &eNumber);
        if(eNumber == 0) break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        segment lines[eNumber];
        int fromx = 0, fromy = 0;
        scanf("%d%d", &fromx, &fromy);
        double len[eNumber];
        for(i = 1; i < eNumber; i++){
            int a, b;
            scanf("%d%d", &a, &b);
            lines[i-1].x1 = fromx;
            lines[i-1].y1 = fromy;
            lines[i-1].x2 = a;
            lines[i-1].y2 = b;
            len[i] = sqrt((lines[i-1].y2 - lines[i-1].y1)*(lines[i-1].y2 - lines[i-1].y1) + (lines[i-1].x2 - lines[i-1].x1)*(lines[i-1].x2 - lines[i-1].x1)) + len[i-1];
            fromx = a;
            fromy = b;
        }
        lines[i-1].x1 = fromx;
        lines[i-1].y1 = fromy;
        lines[i-1].x2 = lines[0].x1;
        lines[i-1].y2 = lines[0].y1;
        len[0] = sqrt((lines[i-1].y2 - lines[i-1].y1)*(lines[i-1].y2 - lines[i-1].y1) + (lines[i-1].x2 - lines[i-1].x1)*(lines[i-1].x2 - lines[i-1].x1)) + len[i-1];
        point people;
        double speedb, speedp;
        scanf("%d%d", &people.x, &people.y);
        scanf("%lf%lf", &speedb, &speedp);
        /*Construct Graph*/
        int gSize = eNumber+2;
        double adjMatrix[gSize][gSize];
        ConstructGraph(gSize, lines, adjMatrix);
        AddPoint(gSize, lines, adjMatrix, people, 0);
        /*Binary search possible t. For all t, there is a target on polygon. Edit graph with the target. Run Dijkstra to calculate time.*/
        double hb = 10000000, lb = 0;
        while(hb - lb > 0.1){
            double mid = (hb + lb)/2;
            double tmpl = mid*speedb;
            while(tmpl > len[0]) tmpl -= len[0];
            for(i = 0; i < eNumber-1; i++){
                if(tmpl < len[i+1]) break;
            }
            point target;
            double factor = (tmpl - len[i])/(len[(i+1)%eNumber] - len[i]);
            if(i == 0) factor = (tmpl / (len[(i+1)%eNumber]));
            target.x = (lines[i].x2 - lines[i].x1)*( factor ) + lines[i].x1;
            target.y = (lines[i].y2 - lines[i].y1)*( factor ) + lines[i].y1;
            AddPoint(gSize, lines, adjMatrix, target, gSize-1);
            double ans = Dijkstra(0, gSize-1, gSize, adjMatrix);
            ans = ans/speedp;
            if(ans > mid) lb = mid;
            else if(fabs(ans - mid) < errorT) hb = mid;
            else hb = mid;
        }
        //printf("%lf %lf\n", hb, lb);
        char TimeString[10] = {'\0'};
        ToString(hb, TimeString);


        printf("Case %d: Time = %s\n", caseNumber, TimeString);
        caseNumber++;
    }
    return 0;
}
void ConstructGraph(int gSize, segment lines[gSize-2], double adjMatrix[gSize][gSize])
{
    int i, j;
    for(i = 0; i < gSize-2-1; i++ ){
        for(j = i+1; j < gSize-2; j++){
            segment tmp;
            tmp.x1 = lines[i].x1;
            tmp.y1 = lines[i].y1;
            tmp.x2 = lines[j].x1;
            tmp.y2 = lines[j].y1;
            double valid = CheckValid(gSize-2, lines, tmp);
            adjMatrix[i+1][j+1] = valid;
            adjMatrix[j+1][i+1] = valid;
        }
    }

}
void AddPoint(int gSize, segment lines[gSize-2], double adjMatrix[gSize][gSize], point p, int index)
{
    int i;
    for(i = 0; i < gSize-2; i++){
        segment tmp;
        tmp.x1 = lines[i].x1;
        tmp.y1 = lines[i].y1;
        tmp.x2 = p.x;
        tmp.y2 = p.y;
        double valid = CheckValid(gSize-2, lines, tmp);
        adjMatrix[i+1][index] = valid;
        adjMatrix[index][i+1] = valid;
    }
}

double CheckValid(int size, segment lines[size], segment checkline)
{
    double len = sqrt((checkline.y2 - checkline.y1)*(checkline.y2 - checkline.y1) + (checkline.x2 - checkline.x1)*(checkline.x2 - checkline.x1));
    return len;
}
int Dijkstra(int from, int to, int n, double adjMatrix[n][n])
{
    int i, j;
    int valid[n], parent[n];
    double key[n];
    for(i = 0; i < n; i++) valid[i] = 1, parent[i] = -1, key[i] = LargeINT;
    key[from] = 0;
    while(1){
        int deQ = -1;
        double min = LargeINT;
        for(i = 0; i < n; i++){
            if(key[i] < min && valid[i] == 1){
                min = key[i];
                deQ = i;
            }
        }
        if(deQ == -1) break;
        valid[deQ] = 0;
        for(i = 0; i < n; i++){
            if(adjMatrix[deQ][i] > 0 && valid[i] == 1){
                if(key[deQ] + adjMatrix[deQ][i] < key[i]) {
                    key[i] = key[deQ] + adjMatrix[deQ][i];
                    parent[i] = deQ;
                }
            }
        }
    }
    /*Set the node on path been used*/
    //printf("dij: %lf\n", key[to]);
    return key[to];
}

void ToString(double time, char string[10])
{
    int m = 0;
    while(time - 60 > errorT){
        time -= 60;
        m++;
    }
    sprintf(string, "%d:%02.0lf", m, time);
}
