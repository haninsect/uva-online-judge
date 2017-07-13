/*1060 - Collecting Luggage _ Geometrix */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define DBUGM1
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
double Dijkstra(int from, int to);
double CheckValid(segment checkline);
void AddPoint(point p, int index, int at);
void ConstructGraph();
double CalculateArea(int size, segment lines[size]);
int CheckIntersect(segment edgeA, segment edgeB, segment edge0);
void ReadInput();

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
        oArea = CalculateArea(eNumber, lines);
        ConstructGraph();
        ShowGraph();
        AddPoint(people, 0, -eNumber);
        ShowGraph();

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
            ShowGraph();
            double ans = Dijkstra(0, gSize-1);
            printf("1: %lf %lf\n", mid, ans);
            printf("2: %lf %lf\n", target.x, target.y);
            /*break;*/
            ans = ans/speedp;
            printf("ANS: %lf\n", ans);
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
            double valid = CheckValid(tmp);
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
        adjMatrix[i+1][index] = 0;
        adjMatrix[index][i+1] = 0;
    }
    /*Add edge*/
    for(i = 0; i < eNumber; i++){
        segment tmp;
        tmp.x1 = lines[i].x1;
        tmp.y1 = lines[i].y1;
        tmp.x2 = p.x;
        tmp.y2 = p.y;
        double valid = CheckValid(tmp);
            printf("%04.2lf %04.2lf -> %04.2lf %04.2lf\n", tmp.x1, tmp.y1, tmp.x2, tmp.y2);
            printf("%d %d: %lf\n\n", index, i, valid);
        adjMatrix[i+1][index] = valid;
        adjMatrix[index][i+1] = valid;
    }
}

double CheckValid(segment checkline)
{
    #ifdef DBUGM
        printf("---CheckValid---\n");
    #endif
    /*Should return length*/
    /*All Overlap -> valid*/
    /*Crossing edge -> fail*/
    int i, j;
    int isIntersection = 0;
    double len = sqrt((checkline.y2 - checkline.y1)*(checkline.y2 - checkline.y1) + (checkline.x2 - checkline.x1)*(checkline.x2 - checkline.x1));
    for(i = 0; i < eNumber && isIntersection == 0; i++){
        isIntersection = CheckIntersect(lines[i], checkline, lines[(i-1+eNumber)%eNumber]);
    }
    #ifdef DBUGM
       printf("isIntersection: %d\n", isIntersection);
    #endif
    if(isIntersection == 1) return 0; /*Crossing edge*/
    else if(isIntersection == -1) return len; /*All overlap*/
    else {
        /*The area increase -> fail*/
        if(CheckAllInside(checkline) == 1) return 0;
        else return len;
    }
}
int CheckAllInside(segment checkline)
{
    int i = 0, j;
    point tmp1;
    tmp1.x = checkline.x1;
    tmp1.y = checkline.y1;
    point tmp2;
    tmp2.x = checkline.x2;
    tmp2.y = checkline.y2;

    while(CheckOnSide(lines[i], tmp1) == 0) i++;
    segment part[MaxSize];
    int partNumber = 0;
    part[partNumber].x1 = tmp1.x;
    part[partNumber].y1 = tmp1.y;
    part[partNumber].x2 = lines[i].x2;
    part[partNumber++].y2 = lines[i].y2;
    i = (i+1)%eNumber;
    while(CheckOnSide(lines[i], tmp2) == 0){
        part[partNumber].x1 = part[partNumber-1].x2;
        part[partNumber].y1 = part[partNumber-1].y2;
        part[partNumber].x2 = lines[i].x2;
        part[partNumber++].y2 = lines[i].y2;
        i = (i+1)%eNumber;
    }
    part[partNumber].x1 = part[partNumber-1].x2;
    part[partNumber].y1 = part[partNumber-1].y2;
    part[partNumber].x2 = tmp2.x;
    part[partNumber++].y2 = tmp2.y;
    part[partNumber].x1 = part[partNumber-1].x2;
    part[partNumber].y1 = part[partNumber-1].y2;
    part[partNumber].x2 = tmp1.x;
    part[partNumber++].y2 = tmp1.y;
    double partA = CalculateArea(partNumber, part);

    /*Part B*/
    while(CheckOnSide(lines[i], tmp1) == 0) i = (i+1)%eNumber;
    segment partb[MaxSize];
    partNumber = 0;
    partb[partNumber].x1 = tmp2.x;
    partb[partNumber].y1 = tmp2.y;
    partb[partNumber].x2 = lines[i].x2;
    partb[partNumber++].y2 = lines[i].y2;
    i = (i+1)%eNumber;
    while(CheckOnSide(lines[i], tmp1) == 0){
        partb[partNumber].x1 = partb[partNumber-1].x2;
        partb[partNumber].y1 = partb[partNumber-1].y2;
        partb[partNumber].x2 = lines[i].x2;
        partb[partNumber++].y2 = lines[i].y2;
        i = (i+1)%eNumber;
    }
    partb[partNumber].x1 = partb[partNumber-1].x2;
    partb[partNumber].y1 = partb[partNumber-1].y2;
    partb[partNumber].x2 = tmp1.x;
    partb[partNumber++].y2 = tmp1.y;
    partb[partNumber].x1 = part[partNumber-1].x2;
    partb[partNumber].y1 = part[partNumber-1].y2;
    partb[partNumber].x2 = tmp2.x;
    partb[partNumber++].y2 = tmp2.y;
    double partB = CalculateArea(partNumber, part);
    printf("%lf + %lf > %lf : %d", partA, partB, partA + partB, partA + partB > oArea-errorT);
    if(partA + partB > oArea+errorT) return 0;
    else return 1;

}
int CheckOnSide(segment line, point pt)
{
    double l1 = sqrt((line.x1-pt.x)*(line.x1-pt.x) + (line.y1-pt.y)*(line.y1-pt.y));
    double l2 = sqrt((line.x2-pt.x)*(line.x2-pt.x) + (line.y2-pt.y)*(line.y2-pt.y));
    double l3 = sqrt((line.x2-line.x1)*(line.x2-line.x1) + (line.y2-line.y1)*(line.y2-line.y1));
    if(fabs(l1 + l2 - l3) < errorT) return 1;
}
int CheckIntersect(segment edgeA, segment edgeB, segment edge0)
{
    if( fabs((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1)) < errorT ){
        if( ((fabs(edgeA.y2 - edgeB.y2) < errorT) && (fabs(edgeA.x2 - edgeB.x2) < errorT)) || ( (fabs(edgeA.y1 - edgeB.y1) < errorT) && (fabs(edgeA.x1 - edgeB.x1) < errorT)) ){
            return -1;
        }
        if( ((fabs(edgeA.y2 - edgeB.y1) < errorT) && (fabs(edgeA.x2 - edgeB.x1) < errorT)) || ( (fabs(edgeA.y1 - edgeB.y2) < errorT) && (fabs(edgeA.x1 - edgeB.x2) < errorT)) ){
            return -1;
        }
    }
    /*Compute the proportion from edgeA->P1 to the intersection*/
    /*ua = ((x4 - x3)(y1 - y3) - (y4-y3)(x1-x3)) / ((y4 - y3)(x2 - x1) - (x4 - x3)(y2 - y1))*/
    double c = (double)((edgeA.x2 - edgeA.x1)*(edgeB.y1 - edgeA.y1) - (edgeA.y2 - edgeA.y1)*(edgeB.x1 - edgeA.x1)) / (double)((edgeA.y2 - edgeA.y1)*(edgeB.x2 - edgeB.x1) - (edgeA.x2 - edgeA.x1)*(edgeB.y2 - edgeB.y1));
    double r = (double)((edgeB.x2 - edgeB.x1)*(edgeA.y1 - edgeB.y1) - (edgeB.y2 - edgeB.y1)*(edgeA.x1 - edgeB.x1)) / (double)((edgeB.y2 - edgeB.y1)*(edgeA.x2 - edgeA.x1) - (edgeB.x2 - edgeB.x1)*(edgeA.y2 - edgeA.y1));
    //printf("r, c: %lf %lf\n", r, c);
    if(fabs(r - 0) < errorT && (c < 1-errorT && c > 0+errorT)){
        /*Check if crossing on corner*/
        double a2 = -(edgeB.y2 - edgeB.y1), b2 = edgeB.x2 - edgeB.x1, c2 = (edgeB.y2 - edgeB.y1)*edgeB.x1 - (edgeB.x2 - edgeB.x1)*edgeB.y1;
        double sign1, sign2;
         // printf("Line B: %dx + %dy + %d = 0\n", a2, b2, c2);
         sign1  = a2*edgeA.x2 + b2*edgeA.y2 + c2;
         sign2  = a2*edge0.x1 + b2*edge0.y1 + c2;
         //printf("S: %lf %lf\n", sign1, sign2);
         if(sign1 * sign2 >= 0-errorT) return 0;
         else return 1;

    }
    if(c > 1-errorT || c < 0+errorT || r > 1-errorT || r < 0+errorT) return 0;
    else return 1;
}

double CalculateArea(int size, segment polygon[MaxSize])
{
    double area = 0;
    int i;
    for(i = 0; i < size; i++){
        area += polygon[i].x1*polygon[i].y2 - polygon[i].x2*polygon[i].y1;
    }
    return abs(area)/2;
}

double Dijkstra(int from, int to)
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
            if(adjMatrix[deQ][i] > 0+errorT && valid[i] == 1){
                if(key[deQ] + adjMatrix[deQ][i] < key[i]) {
                        printf("%lf + %lf = %lf\n", key[deQ], adjMatrix[deQ][i], key[deQ] + adjMatrix[deQ][i]);
                    key[i] = key[deQ] + adjMatrix[deQ][i];
                    parent[i] = deQ;
                }
            }
        }

        for(i = 0;  i < gSize; i++) printf("%lf ", key[i]);
        printf("\n");
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
    scanf("%lf%lf", &people.x, &people.y);
    scanf("%lf%lf", &speedb, &speedp);
    speedb /= 60;
    speedp /= 60;
}

void ShowGraph()
{
    int i, j;
    for(i = 0; i < gSize; i++){
        for(j = 0; j < gSize; j++){
            printf("%05.2lf ", adjMatrix[i][j]);
        }
        printf("\n");
    }
        printf("\n");
}
