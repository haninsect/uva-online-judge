#include <stdio.h>
#define LineLimit 500
#define DLimits 100
#define ELimits 100
int main()
{
    freopen("input.in", "w", stdout);
    srand(time(NULL));
    int i,j,k;

    int line = 500;
    char dName[100][10] = {'\0'};
    int dNumber = 0;
    int oNumber = 0;
    int eNumber = 0;
    int c = 0;
    while(c < line){
        int exitFlag = 0;
        int op = (rand()%4);
        switch(op){
            case 0:
                if(dNumber >= DLimits){
                    exitFlag = 1;
                    break;
                }
                printf("D ");
                int symbolLen = (rand()%8)+1;
                for(i = 0; i < symbolLen; i++){
                    char c = 'A' + (rand()%26);
                    dName[dNumber][i] = c;
                    printf("%c", c);
                }
                dName[dNumber][i] = '\0';
                printf(" %X\n", (rand()%256));
                dNumber++;
                break;
            case 1:
                if(eNumber > ELimits) {
                    exitFlag = 1;
                    break;
                }
                printf("E ");
                int isUndefined = rand()%2;
                if(isUndefined || dNumber <= 0){
                    int symbolLen = (rand()%8)+1;
                    for(i = 0; i < symbolLen; i++){
                        char c = 'A' + (rand()%26);
                        printf("%c", c);
                    }
                    printf("\n");
                }
                else {
                    int d = rand()%dNumber;
                    printf("%s\n", dName[d]);
                }
                eNumber++;
                break;
            case 2:
                printf("C ");
                int c = (rand()%16)+1;
                printf("%X", c);
                for(i = 0; i < c; i++){
                    int isDollar = rand()%2;
                    if(isDollar){
                        if( i >= c-1 || eNumber <= 0)printf(" %X", rand()%256);
                        else{
                            printf(" $");
                            int e = rand()%eNumber;
                            printf(" %d", e);
                            i++;
                        }
                    }
                    else{
                        printf(" %X", rand()%256);
                    }
                }
                printf("\n");
                break;
            case 3:
                printf("Z\n");
                oNumber++;
                eNumber = 0;
                break;

        }
        if(exitFlag)break;
        c++;
    }
    printf("$\n$");
}
