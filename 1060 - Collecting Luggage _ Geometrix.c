/*1060 - Collecting Luggage _ Geometrix */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM
#define LargeINT 1000000000
#define MaxSize 101
#define errorT 0.00005
typedef struct {
    double x1, y1;
    double x2, y2;
} segment;
typedef struct {
    double x, y;
} point;
/*Global*/
void ToString(double time, char string[10]);
int Dijkstra(int from, int to);
double CheckValid(segment checkline, int a, int b);
void AddPoint(point p, int index, int at);
void ConstructGraph();
double CalculateArea(int size, segment lines[size]);
double CalculatePolygon();
int CheckIntersect(segment edgeA, segment edgeB);

double oArea;
int eNumber;
int gSize;
segment lines[MaxSize];
point people;
double speedb, speedp;
double len[MaxSize];
double adjMatrix[MaxSize+2][MaxSize+2];

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){
        scanf("%d", &eNumber);
        if(eNumber == 0) break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        ReadInput();
        /*Construct Graph*/
        gSize = eNumber+2;
        oArea = CalculatePolygon();
        ConstructGraph();
        AddPoint(people, 0, -eNumber);

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
            AddPoint(target, gSize-1, -i);
            double ans = Dijkstra(0, gSize-1);
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
void ConstructGraph()
{
    #ifdef DBUGM
        printf("---ConstructGraph---\n");
    #endif
    /*For all the vertex in on polygon*/
    int i, j;
    for(i = 0; i < eNumber-1; i++ ){
        for(j = i+1; j < eNumber; j++){
            segment tmp;
            tmp.x1 = lines[i].x1;
            tmp.y1 = lines[i].y1;
            tmp.x2 = lines[j].x1;
            tmp.y2 = lines[j].y1;
            double valid = CheckValid(tmp, i, j);
            adjMatrix[i+1][j+1] = valid;
            adjMatrix[j+1][i+1] = valid;
        }
    }

}
void AddPoint(point p, int index, int at)
{
    #ifdef DBUGM
        printf("---AddPoint %d---\n", at);
    #endif
    /*Flush*/
    int i;
    for(i = 0; i < eNumber; i++){
        adjMatrix[i][index] = 0;
        adjMatrix[index][i+1] = 0;
    }
    /*Add edge*/
    for(i = 0; i < eNumber; i++){
        segment tmp;
        tmp.x1 = lines[i].x1;
        tmp.y1 = lines[i].y1;
        tmp.x2 = p.x;
        tmp.y2 = p.y;
        double valid = CheckValid(tmp, i, at);
        adjMatrix[i+1][index] = valid;
        adjMatrix[index][i+1] = valid;
    }
}

double CheckValid(segment checkline, int from, int to)
{
    #ifdef DBUGM
        printf("---CheckValid %d %d---\n", from, to);
    #endif
    /*Should return length*/
    /*All Overlap -> valid*/
    /*Crossing edge -> fail*/
    int i, j;
    int isIntersection = 0;
    double len = sqrt((checkline.y2 - checkline.y1)*(checkline.y2 - checkline.y1) + (checkline.x2 - checkline.x1)*(checkline.x2 - checkline.x1));
    for(i = 0; i < eNumber && isIntersection == 0; i++){
        isIntersection = CheckIntersect(lines[i], checkline);
    }
    #ifdef DBUGM
       printf("isIntersection: %d\n", isIntersection);
    #endif

    if(isIntersection == 1) return 0; /*Crossing edge*/
    else if(isIntersection == -1) return len; /*All overlap*/
    else {
        if(to = -eNumber) return len; /*Source*/
        else {
            /*The area increase -> fail*/
            double aArea, bArea;
            if(to > 0){
                segment apart[abs(from-to) +1];
                for(i = 0; i < abs(from-to); i++){
                    apart[i].x1 = lines[i + from].x1;
                    apart[i].x2 = lines[i + from].x2;
                    apart[i].y1 = lines[i + from].y1;
                    apart[i].y2 = lines[i + from].y2;
                }
                apart[i].x1 = checkline.x2;
                apart[i].x2 = checkline.y2;
                apart[i].y1 = checkline.x1;
                apart[i].y2 = checkline.y1;
                aArea = CalculateArea(abs(from-to) +1, apart);

                segment bpart[eNumber - abs(from-to) +1];
                for(i = 0; i < eNumber - abs(from-to); i++){
                    bpart[i].x1 = lines[(i + b)%eNumber].x1;
                    bpart[i].x2 = lines[(i + b)%eNumber].x2;
                    bpart[i].y1 = lines[(i + b)%eNumber].y1;
                    bpart[i].y2 = lines[(i + b)%eNumber].y2;
                }
                bpart[i].x1 = checkline.x1;
                bpart[i].x2 = checkline.y1;
                bpart[i].y1 = checkline.x2;
                bpart[i].y2 = checkline.y2;
                bArea = CalculateArea(eNumber - abs(from-to) +1, bpart);

            }
            else { /*Target which is on the side*/
                b = -b;
                segment apart[abs(from-to) +1 +1];
                for(i = 0; i < abs(from-to); i++){
                    apart[i].x1 = lines[i + a].x1;
                    apart[i].x2 = lines[i + a].x2;
                    apart[i].y1 = lines[i + a].y1;
                    apart[i].y2 = lines[i + a].y2;
                }
                apart[i].x1 = lines[b].x1;
                apart[i].y1 = lines[b].x2;
                apart[i].x2 = checkline.x2;
                apart[i].y2 = checkline.y2;
                apart[i+1].x1 = checkline.x2;
                apart[i+1].y1 = checkline.y2;
                apart[i+1].x2 = checkline.x1;
                apart[i+1].y2 = checkline.y1;
                aArea = CalculateArea(a - b +1, apart);

                segment bpart[eNumber - (a - b) +1];
                for(i = 0; i < eNumber - (a - b); i++){
                    bpart[i].x1 = lines[(i + b)%eNumber].x1;
                    bpart[i].x2 = lines[(i + b)%eNumber].x2;
                    bpart[i].y1 = lines[(i + b)%eNumber].y1;
                    bpart[i].y2 = lines[(i + b)%eNumber].y2;
                    if(i == 0){
                        bpart[i].x1 = checkline.x2;
                        bpart[i].y1 = checkline.y2;
                    }
                }
                bpart[i].x1 = checkline.x1;
                bpart[i].y1 = checkline.y1;
                bpart[i].x2 = checkline.x2;
                bpart[i].y2 = checkline.y2;
                bArea = CalculateArea(eNumber - (a - b) +1, bpart);
            }
            if(fabs(aArea + bArea - oArea) < errorT) return 0;
            else return len;
        }

    }


}
int CheckIntersect(segment edgeA, segment edgeB)
{
    if( fabs((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1)) < errorT ){
        if( (fabs(edgeA.y2 - edgeB.y2) < errorT) && (fabs(edgeA.x2 - edgeB.x2) < errorT)) || ( (fabs(edgeA.y1 - edgeB.y1) < errorT) && (fabs(edgeA.x1 - edgeB.x1) < errorT)) ){
            return -1;
        }
    }
    /*Compute the proportion from edgeA->P1 to the intersection*/
    /*ua = ((x4 - x3)(y1 - y3) - (y4-y3)(x1-x3)) / ((y4 - y3)(x2 - x1) - (x4 - x3)(y2 - y1))*/
    double c = (double)((edgeA.x2 - edgeA.x1)*(edgeB.y1 - edgeA.y1) - (edgeA.y2 - edgeA.y1)*(edgeB.x1 - edgeA.x1)) / (double)((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1));
    double r = (double)((edgeB.x2 - edgeB.x1)*(edgeA.y1 - edgeB.y1) - (edgeB.y2 - edgeB.y1)*(edgeA.x1 - edgeB.x1)) / (double)((edgeB.y2 - edgeB.y1)*(edgeA.x2 - edgeA.x1) - (edgeB.x2 - edgeB.x1)*(edgeA.y2 - edgeA.y1));
    if(c >= 1 || c <= 0 || r >= 1 || r <= 0) return 0;
    else return 1;
}
double CalculatePolygon()
{
    double area = 0;
    int i;
    for(i = 0; i < eNumber+1; i++){
        area += lines[i].x1*lines[i].y2 - lines[i].x2*lines[i].y1;
    }
    return abs(area)/2;
}
double CalculateArea(int size, segment polygon[size])
{
    double area = 0;
    int i;
    for(i = 0; i < size; i++){
        area += polygon[i].x1*polygon[i].y2 - polygon[i].x2*polygon[i].y1;
    }
    return abs(area)/2;
}

int Dijkstra(int from, int to)
{
    int i, j;
    int valid[gSize], parent[gSize];
    double key[gSize];
    for(i = 0; i < gSize; i++) valid[i] = 1, parent[i] = -1, key[i] = LargeINT;
    key[from] = 0;
    while(1){
        int deQ = -1;
        double min = LargeINT;
        for(i = 0; i < gSize; i++){
            if(key[i] < min && valid[i] == 1){
                min = key[i];
                deQ = i;
            }
        }
        if(deQ == -1) break;
        valid[deQ] = 0;
        for(i = 0; i < gSize; i++){
            if(adjMatrix[deQ][i] > errorT && valid[i] == 1){
                if(key[deQ] + adjMatrix[deQ][i] < key[i]) {
                    key[i] = key[deQ] + adjMatrix[deQ][i];
                    parent[i] = deQ;
                }
            }
        }
    }
    /*Return length*/
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

void ReadInput()
{
    int i;
    int fromx = 0, fromy = 0;
    scanf("%d%d", &fromx, &fromy);
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
    scanf("%d%d", &people.x, &people.y);
    scanf("%lf%lf", &speedb, &speedp);
}
