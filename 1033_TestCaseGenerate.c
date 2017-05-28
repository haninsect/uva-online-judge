
#include <stdio.h>
#include <time.h>
int main()
{
    srand(time(NULL));   // should only be called once
    int r = rand();

    freopen("input.in", "w", stdout);
    int n = 1000; //case number
    while(n--){
        int mapNumber = rand()%8 + 2;
        printf("%d\n", mapNumber);
        int l;

        for(l = 0; l < mapNumber; l++){

            int row = rand()%10 + 1;
            int col = rand()%10 + 1;
            printf("%d %d\n", row, col);

            int i, j;
            for(i = 0; i < row; i++){
                for(j = 0; j < col; j++){
                    int m = rand()%40;
                    if(m >= 26) printf("-");
                    else printf("%c", 'A' + m);
                }
                printf("\n");
            }
        }
    }
    printf("0\n");
}
