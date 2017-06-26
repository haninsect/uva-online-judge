/*1039 - Simplified GSM Network _BAndB*/
#include <stdio.h>
#include <math.h>
typedef struct {
    double x, y;
} point;
int cutLine(point* start, point* end, point a, point b);
void ShowRoads(int c, int roads[c][c]);
const int inf = 1000000000;
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k, l;
    int caseNumber = 1;
    while(1){
        int bts, c, r, q;
        scanf("%d %d %d %d", &bts, &c, &r, &q);
        if(bts == 0 && c == 0 && r == 0 && q == 0) break;

        point towers[bts];
        for(i = 0; i < bts; i++) scanf("%lf %lf", &towers[i].x, &towers[i].y);
        point cities[c];
        for(i = 0; i < c; i++) scanf("%lf %lf", &cities[i].x, &cities[i].y);
        int roads[c][c];
        for(i = 0; i < c; i++)
            for(j = 0; j < c; j++) roads[i][j] = inf;
        for(i = 0; i < r; i++) {
            int a, b;
            scanf("%d %d", &a, &b);
            a--;
            b--;
            roads[a][b] = 0;
        }
        /*Calculate weight*/
        for(i = 0; i < c; i++){
            for(j = 0; j < c; j++){
                if(roads[i][j] == inf) continue;
                for(k = 0; k < bts; k++){
                    int isOccupying = 1;
                    point start, end;
                    start.x = cities[i].x;
                    start.y = cities[i].y;
                    end.x = cities[j].x;
                    end.y = cities[j].y;
                    for(l = 0; l < bts && isOccupying == 1; l++){
                        if(k == l) continue;
                        /*At first tower[k] dominate whole roads[i][j]].*/
                        /*For every other towers, cut the road. If tower[k] still have some part of roads[i][j]. edge length++*/
                        isOccupying = cutLine(&start, &end, towers[k], towers[l]);
                        if(isOccupying == 0) break;
                    }
                    if(isOccupying) roads[i][j]++;
                }
                roads[i][j]--;
            }
        }
        /*ShowRoads(c, roads);*/
        /*Calculate Shortest path*/
        for(i = 0; i < c; i++){
            for(j = 0; j < c; j++){
                for(k = 0; k < c; k++){
                    if (roads[i][k] + roads[k][j] < roads[i][j])
                        roads[i][j] = roads[i][k] + roads[k][j];
                }
            }
        }

        /*Get input Q and print result*/
        printf("Case %d:\n", caseNumber);
        for(i = 0; i < q; i++){
            int a, b;
            scanf("%d %d", &a, &b);
            a--;
            b--;
            if(roads[a][b] == inf) printf("Impossible\n");
            else printf("%d\n", roads[a][b]);
        }
        caseNumber++;


    }

    return 0;
}

int cutLine(point* start, point* end, point bts0, point bts1)
{
    /*If line are all locate on bts1's range, return 0*/
    /*Find perpendicular bisector a1*x + b1*y + c1 = 0*/
    double a1, b1, c1;
    a1 = bts0.x - bts1.x;
    b1 = bts0.y - bts1.y;
    c1 = (bts1.y*bts1.y + bts1.x*bts1.x - bts0.x*bts0.x - bts0.y*bts0.y)/2;

    int isCrossing = 0;
    double signS = a1*start->x + b1*start->y + c1;
    double signE = a1*end->x + b1*end->y + c1;
    if(signS*signE < 0) isCrossing = 1;

    double sign0 = a1*bts0.x + b1*bts0.y + c1;
    if(isCrossing == 1){
        /*Find intersection point*/
        /*start->End: -(y2 - y1)x + (x2-x1)y + (y2-y1)x1 - (x2 - x1) y1 = 0*/
        point interP;
        double  a2 = -(end->y - start->y), b2 = end->x - start->x, c2 = (end->y - start->y)*start->x - (end->x - start->x)*start->y;
        interP.x = (b2*c1 - b1*c2)/(b1*a2 - b2*a1);
        interP.y = (c2*a1 - c1*a2)/(b1*a2 - b2*a1);
        if(signS*sign0 > 0) {
            end->x = interP.x;
            end->y = interP.y;
        }
        else {
            start->x = interP.x;
            start->y = interP.y;
        }
        return 1;
    }
    else {
        double signT = signS;
        if(fabs(signT - 0.0) <0.0001 ) signT = signE;
        if(sign0*signT > 0) return 1;
        else return 0;
    }
}

void ShowRoads(int c, int roads[c][c])
{
    int i, j;
    for(i = 0; i < c; i++){
        for(j = 0; j < c; j++){
            if(roads[i][j] == inf)printf("%2d ", 0);
            else printf("%2d ",roads[i][j]);
        }printf("\n");
    }
}
