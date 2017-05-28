#include <stdio.h>
#include <time.h>
int main()
{
    srand(time(NULL));   // should only be called once
    int r = rand();
    int i, j;
    freopen("input.in", "w", stdout);

    int mday[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int n = 100;
    while(n--){
        int oldD = 0;
        int newD = 0;
        while(oldD == 0 && newD == 0){
            oldD = rand()%100;
            newD = rand()%100;
        }
        printf("%d %d\n", oldD, newD);
        for(i = 0; i < oldD; i++){
            int yy = (rand()%400) + 1700;
            int mm = (rand()%12)+1;
            int dd;
            if(mm == 2){
                if ((yy%4 == 0 && yy%100 != 0) || yy%400 == 0) dd = (rand()%29) + 1;
                else dd = (rand()%28) + 1;
            }
            else dd = (rand()%mday[mm]) + 1;
            int a = yy * 10000 + mm * 100 + dd;

            yy = (rand()%400) + 1700;
            mm = (rand()%12)+1;
            if(mm == 2){
                if ((yy%4 == 0 && yy%100 != 0) || yy%400 == 0) dd = (rand()%29) + 1;
                else dd = (rand()%28) + 1;
            }
            else dd = (rand()%mday[mm]) + 1;
            int b = yy * 10000 + mm * 100 + dd;
            if(a > b) i--;
            else printf("%d %d\n", a, b);
        }

        for(i = 0; i < newD; i++){
            int yy = (rand()%400) + 1700;
            int mm = (rand()%12)+1;
            int dd;
            if(mm == 2){
                if ((yy%4 == 0 && yy%100 != 0) || yy%400 == 0) dd = (rand()%29) + 1;
                else dd = (rand()%28) + 1;
            }
            else dd = (rand()%mday[mm]) + 1;
            int a = yy * 10000 + mm * 100 + dd;

            yy = (rand()%400) + 1700;
            mm = (rand()%12)+1;
            if(mm == 2){
                if ((yy%4 == 0 && yy%100 != 0) || yy%400 == 0) dd = (rand()%29) + 1;
                else dd = (rand()%28) + 1;
            }
            else dd = (rand()%mday[mm]) + 1;
            int b = yy * 10000 + mm * 100 + dd;
            if(a > b) i--;
            else printf("%d %d\n", a, b);
        }
    }
    printf("0 0\n");
}
