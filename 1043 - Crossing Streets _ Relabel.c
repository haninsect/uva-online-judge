/*1043 - Crossing Streets _ Relabel*/
#include <stdio.h>
#include <stdlib.h>
int cmp(const void *a, const void *b);
typedef struct {
    int x1, y1, x2, y2;
} line;
typedef struct {
    int x, y;
} point;
const int dx[4] = {-1, 0, 1, 0};
const int dy[4] = {0, 1, 0, -1};
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k, l;
    int caseNumber = 1;
    while(1){
        int answer = 0;
        int lineNumber;
        scanf("%d", &lineNumber);
        if(lineNumber == 0)break;

        int sortedX[lineNumber*2], sortedY[lineNumber*2];
        line lines[lineNumber];
        for(i = 0; i < lineNumber; i++){
            scanf("%d %d %d %d", &lines[i].x1, &lines[i].y1, &lines[i].x2, &lines[i].y2);
            sortedX[i*2] = lines[i].x1;
            sortedX[i*2 + 1] = lines[i].x2;
            sortedY[i*2] = lines[i].y1;
            sortedY[i*2 + 1] = lines[i].y2;
        }
        int startX, startY;
        int endX, endY;
        scanf("%d %d %d %d", &startX, &startY, &endX, &endY);
        if(lineNumber <= 3){
            printf("City %d\n", caseNumber);
            printf("Peter has to cross %d streets\n", 0);
            caseNumber++;
            continue;
        }
        /*Relabel*/
        qsort(sortedX, lineNumber*2, sizeof(int), cmp);
        qsort(sortedY, lineNumber*2, sizeof(int), cmp);
        int tmp[lineNumber*2];
        int xn = 0;
        for(i = 0; i < lineNumber*2; i++) tmp[i] = sortedX[i];
        for(i = 0; i < lineNumber*2; i++){
            if(i < lineNumber*2-1)
                if(tmp[i] == tmp[i+1])continue;
            sortedX[xn++] = tmp[i];
        }
        int yn = 0;
        for(i = 0; i < lineNumber*2; i++) tmp[i] = sortedY[i];
        for(i = 0; i < lineNumber*2; i++){
            if(i < lineNumber*2-1)
                if(tmp[i] == tmp[i+1]) continue;
            sortedY[yn++] = tmp[i];
        }
        for(i = 0; i < lineNumber; i++){
            for(j = 0; j < xn; j++){
                if(lines[i].x1 == sortedX[j]) lines[i].x1 = j*2;
                if(lines[i].x2 == sortedX[j]) lines[i].x2 = j*2;
            }
            for(j = 0; j < yn; j++){
                if(lines[i].y1 == sortedY[j]) lines[i].y1 = j*2;
                if(lines[i].y2 == sortedY[j]) lines[i].y2 = j*2;
            }
        }

        /*Tabular*/
        int row = xn*2, col = yn*2;
        int table[row][col];
        for(i = 0; i < row; i++)
            for(j = 0; j < col; j++) table[i][j] = 0;
            /*Fill in lines*/
        for(i = 0; i < lineNumber; i++){
            if(lines[i].x1 == lines[i].x2){
                for(j = min(lines[i].y1, lines[i].y2); j <= max(lines[i].y1, lines[i].y2); j++) table[lines[i].x1][j] = 1;
            }
            else{
                for(j = min(lines[i].x1, lines[i].x2); j <= max(lines[i].x1, lines[i].x2); j++) table[j][lines[i].y1] = 1;
            }
        }
            /*set -1 to intersection*/
        for(i = 0; i < row; i++){
            for(j = 0; j < col; j++) {
                if(table[i][j] == 0)continue;
                int v = 0;
                if(i > 0) if(table[i-1][j] == 1) v = 1;
                if(i < row-1) if(table[i+1][j] == 1) v = 1;
                if(v == 0) continue;

                if(j > 0) if(table[i][j-1] == 1) table[i][j] = -1;
                if(j < col-1) if(table[i][j+1] == 1) table[i][j] = -1;
            }
        }
            /*Find start and end*/
        int rsx = row-1, rsy = col-1, rex = row-1, rey = col-1;
        for(j = 0; j < xn-1; j++){
            if(startX >= sortedX[j] && startX <= sortedX[j+1]) rsx = j*2+1;
            if(endX >= sortedX[j] && endX <= sortedX[j+1]) rex = j*2+1;
        }
        for(j = 0; j < yn-1; j++){
            if(startY >= sortedY[j] && startY <= sortedY[j+1]) rsy  = j*2+1;
            if(endY >= sortedY[j] && endY <= sortedY[j+1]) rey = j*2+1;
        }
        /*BFS*/
        /*
        for(i = 0; i < row; i++){
            for(j = 0; j < col; j++) {
                    printf("%2d ", table[i][j]);
            }
            printf("\n");
        }
        printf("Start: %d %d\n", rsx, rsy);
        printf("End: %d %d\n", rex, rey);
        */
        int cross[row][col];
        for(i = 0; i < row; i++)
            for(j = 0; j < col; j++) cross[i][j] = 1000000000;
        point points[row*col];

        int q = 0, qp = 0;
        points[q].x = rsx;
        points[q].y = rsy;
        q++;
        cross[rsx][rsy] = 0;
        while(qp < q){
            /*For direction: LEFT UP RIGHT DOWN*/
            for(i = 0; i < 4; i++){
                if(points[qp].x + dx[i] > 0 && points[qp].x + dx[i] < row && points[qp].y + dy[i] > 0 && points[qp].y + dy[i] < col){
                    if(table[points[qp].x + dx[i]][points[qp].y + dy[i]] == 0){
                        if(cross[points[qp].x][points[qp].y] < cross[points[qp].x + dx[i]][points[qp].y + dy[i]]){
                            cross[points[qp].x + dx[i]][points[qp].y + dy[i]] = cross[points[qp].x][points[qp].y];
                            points[q].x = points[qp].x + dx[i];
                            points[q].y = points[qp].y + dy[i];
                            q++;
                        }
                    }
                    else if(table[points[qp].x + dx[i]][points[qp].y + dy[i]] == 1){
                        if(cross[points[qp].x][points[qp].y]+1 < cross[points[qp].x + dx[i]][points[qp].y + dy[i]]){
                            cross[points[qp].x + dx[i]][points[qp].y + dy[i]] = cross[points[qp].x][points[qp].y]+1;
                            points[q].x = points[qp].x + dx[i];
                            points[q].y = points[qp].y + dy[i];
                            q++;
                        }
                    }
                }
            }
            qp++;
        }

        printf("City %d\n", caseNumber);
        printf("Peter has to cross %d streets\n", cross[rex][rey]);
        caseNumber++;
    }

    return 0;
}

int cmp(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

int max(int a, int b)
{
    if(a >= b) return a;
    else return b;
}
int min(int a, int b)
{
    if(a >= b) return b;
    else return a;
}
