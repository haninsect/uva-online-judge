#include <stdio.h>
#define InputLen 40
#define CaseNumber 100
int main()
{
    freopen("input.in", "w", stdout);
    srand(time(NULL));
    int c = 0;
    while(c < CaseNumber){
        int i;
        for(i = 0; i < InputLen; i++){
            printf("%c", 'A' + rand()%26);
        }
        printf("\n");
        c++;
    }
    printf("X\n");

}
