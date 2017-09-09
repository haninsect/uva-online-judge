/*1087 - Struts and Springs*/
#include <stdio.h>
#include <stdlib.h>
#define DBUGM
#define MaxWin 105
typedef struct {
    long long int x, y;
    long long int width, height;
    long long int tsw, tsh;
    int vert, hori;
    int top, bottom, left, right;
    int en;
    int ew[MaxWin];
} Window;
typedef struct {
    long long int x, y;
    long long int width, height;
} Output;
Window win[MaxWin];
Output out[MaxWin];
int wn, qn;

void Read()
{
    int i, j;
    for(i = 1; i <= wn; i++){
        win[i].en = 0;
         win[i].tsh = 0,  win[i].tsw = 0;
        scanf("%lld%lld%lld%lld%d%d%d%d%d%d", &win[i].x, &win[i].y, &win[i].width, &win[i].height, &win[i].hori, &win[i].vert, &win[i].top, &win[i].bottom, &win[i].left, &win[i].right );
        if(win[i].vert == 0 && win[i].top == 0 && win[i].bottom == 0) win[i].top = 1;
        if(win[i].hori == 0 && win[i].left == 0 && win[i].right == 0) win[i].right = 1;
    }
    /*Find enclosing win*/
    for(i = 1; i <= wn; i++){
        int flag = 0;
        int ec = 0, eh = 0;
        for(j = 1; j <= wn; j++){
            if(j == i) continue;
            if(win[j].x >= win[i].x || win[j].y >= win[i].y) continue;
            if(win[j].x + win[j].width <= win[i].x + win[i].width || win[j].y + win[j].height <= win[i].y + win[i].height) continue;
            if(win[j].y > eh){
                eh = win[j].y;
                ec = j;
            }
        }
        win[ec].ew[++win[ec].en] = i;
        if(win[i].top == 1) win[i].tsh += win[i].y - win[ec].y;
        if(win[i].vert == 1) win[i].tsh += win[i].height;
        if(win[i].bottom == 1) win[i].tsh += win[ec].height - (win[i].height +  win[i].y - win[ec].y);

        if(win[i].left == 1) win[i].tsw += win[i].x - win[ec].x;
        if(win[i].hori == 1) win[i].tsw += win[i].width;
        if(win[i].right == 1) win[i].tsw += win[ec].width - (win[i].width +  win[i].x - win[ec].x);
    }
}
void Resize(int index, int parent)
{
    int i, j;
    long long int dw, dh;
    dw = (out[parent].width - win[parent].width);
    dh = (out[parent].height - win[parent].height);
    if(win[index].left == 1) {
        out[index].x = out[parent].x + (win[index].x - win[parent].x) + dw*(win[index].x - win[parent].x)/win[index].tsw;
    }
    else {
        out[index].x = out[parent].x + (win[index].x - win[parent].x);
    }

    if(win[index].top == 1) {
        out[index].y = out[parent].y + (win[index].y - win[parent].y) + dh*(win[index].y - win[parent].y)/win[index].tsh;
    }
    else {
        out[index].y = out[parent].y + (win[index].y - win[parent].y);
    }

    if(win[index].hori == 1) {;
        out[index].width = win[index].width + dw*(win[index].width)/win[index].tsw;
    }
    else {
        out[index].width = win[index].width;
    }

    if(win[index].vert == 1) {
        out[index].height = win[index].height + dh*(win[index].height)/win[index].tsh;
    }
    else {
        out[index].height = win[index].height;
    }

    for(i = 1; i <= win[index].en; i++){
        Resize(win[index].ew[i], index);
    }

}
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
	int i, j, k , l;
    int caseNumber = 1;
    while(1){
        #ifndef DBUGM
           printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        scanf("%d%d%lld%lld", &wn, &qn, &win[0].width, &win[0].height);
        if(wn == 0 && qn == 0) break;
        win[0].en = 0, win[0].x = 0, win[0].y = 0;
        Read();
        for(i = 1; i <= qn; i++){
            out[0].x = 0, out[0].y = 0;
            scanf("%lld%lld", &out[0].width, &out[0].height);
            /*Resize*/
            for(j = 1; j <= win[0].en; j++){
                Resize(win[0].ew[j], 0);
            }
            printf("Case %d, resize operation %d:\n", caseNumber, i);
            for(j = 1; j <= wn; j++){
                printf("    Window %d, x = %lld, y = %lld, width = %lld, height = %lld\n", j, out[j].x, out[j].y, out[j].width, out[j].height );
            }
        }

        caseNumber++;
    }

    return 0;
}
