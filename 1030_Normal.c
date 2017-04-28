/*UVa 1030 - Image Is Everything */
#include <stdio.h>
#define DM
int UpdateCubd(int size, char cube[size][size][size], char color, int direction, int x, int y);
int main()
{
    #ifndef DM
    freopen("input.in", "r", stdin);
    freopen("output.out", "w", stdout);
    #endif
    while(1){
        int size;
        scanf("%d", &size);
        if(size == 0) break;

        int i,j,k;
        char cube[size][size][size];
        for(i = 0; i < size; i++){
            for(j = 0; j < size; j++){
                for(k = 0; k < size; k++){
                    cube[i][j][k] = '0'; /* 0 means white*/
                }
            }
        }
        /*Get input*/
        char image[6][size][size]; /* front, left, back, right, top, bottom*/
        for(i = 0; i < size; i++){
            for(j = 0; j < 6; j++){
                for(k = 0; k < size; k++){
                    char c;
                    c = getchar();
                    while(c == ' ' || c == '\n'){
                        c = getchar();
                    }
                    image[j][i][k] = c;

                }
            }
        }

        #ifndef DM
        for(i = 0; i < 6; i++){
            printf("Direction %d\n", i);
            for(j = 0; j < size; j++){
                for(k = 0; k < size; k++){
                    printf("%c ", image[i][j][k]);
                }printf("\n");
            }
        }
        #endif
        int flag = 1;
        while(flag){
            flag = 0;
            for(i = 0; i < 6; i++){
                for(j = 0; j < size; j++){
                    for(k = 0; k < size; k++){
                        if(UpdateCubd(size, cube, image[i][j][k], i, j, k)) flag = 1;
                    }
                }

            }

        }

        #ifndef DM
        printf("---Tmp---\n");
        for(i = 0; i < size; i++){
            printf("n %d:\n", i);
            for(j = 0; j < size; j++){
                for(k = 0; k < size; k++){
                    printf("%c ", cube[i][j][k]);
                }printf("\n");
            }
        }
        #endif
        /*Calculate result*/
        int weight = 0;
        for(i = 0; i < size; i++){
            for(j = 0; j < size; j++){
                for(k = 0; k < size; k++){
                    if(cube[i][j][k] != '.')weight++;
                }
            }
        }
        printf("Maximum weight: %d gram(s)\n", weight);

    }
    return 0;
}

int UpdateCubd(int size, char cube[size][size][size], char color, int direction, int x, int y)
{
    switch(direction) {/* front, left, back, right, top, bottom*/
        int flag = 1;
        int i , j, k;
        case 0:/*front*/
            flag = 1;
            i = 0, j = x, k = y;
            while(i < size){
                if(color == '.'){
                    if(cube[i][j][k] != color)flag = 0;
                    cube[i][j][k] = '.';
                    i++;
                    continue;
                }
                else {
                    if(cube[i][j][k] != color){
                        if(cube[i][j][k] == '0'){
                            cube[i][j][k] = color;
                            flag = 0;
                            break;
                        }
                        else{
                            cube[i][j][k] = '.';
                            i++;
                            continue;
                        }
                    }
                    else{
                        break;
                    }
                }
            }
            if(flag) return 0; /*Not change*/
            else return 1;
            break;
        case 1:/*left*/
            flag = 1;
            i = size - 1 - y, j = x, k = 0;
            while(k < size){
                if(color == '.'){
                    if(cube[i][j][k] != color)flag = 0;
                    cube[i][j][k] = '.';
                    k++;
                    continue;
                }
                else {
                    if(cube[i][j][k] != color){
                        if(cube[i][j][k] == '0'){
                            cube[i][j][k] = color;
                            flag = 0;
                            break;
                        }
                        else{
                            cube[i][j][k] = '.';
                            k++;
                            continue;
                        }
                    }
                    else{
                        break;
                    }
                }
            }
            if(flag) return 0; /*Not change*/
            else return 1;
            break;
        case 2:/*back*/
            flag = 1;
            i = size - 1, j =x, k = size - 1 - y;
            while(i > 0){
                if(color == '.'){
                    if(cube[i][j][k] != color)flag = 0;
                    cube[i][j][k] = '.';
                    i--;
                    continue;
                }
                else {
                    if(cube[i][j][k] != color){
                        if(cube[i][j][k] == '0'){
                            cube[i][j][k] = color;
                            flag = 0;
                            break;
                        }
                        else{
                            cube[i][j][k] = '.';
                            i--;
                            continue;
                        }
                    }
                    else{
                        break;
                    }
                }
            }
            if(flag) return 0; /*Not change*/
            else return 1;

            break;
        case 3:/*right*/
            flag = 1;
            i = y , j = x, k = size - 1;
            while(k > 0){
                if(color == '.'){
                    if(cube[i][j][k] != color)flag = 0;
                    cube[i][j][k] = '.';
                    k--;
                    continue;
                }
                else {
                    if(cube[i][j][k] != color){
                        if(cube[i][j][k] == '0'){
                            cube[i][j][k] = color;
                            flag = 0;
                            break;
                        }
                        else{
                            cube[i][j][k] = '.';
                            k--;
                            continue;
                        }
                    }
                    else{
                        break;
                    }
                }
            }
            if(flag) return 0; /*Not change*/
            else return 1;
            break;
        case 4:/*top*/
            flag = 1;
            i = size - 1 - x, j = 0, k = y;
            while(j < size){
                if(color == '.'){
                    if(cube[i][j][k] != color)flag = 0;
                    cube[i][j][k] = '.';
                    j++;
                    continue;
                }
                else {
                    if(cube[i][j][k] != color){
                        if(cube[i][j][k] == '0'){
                            cube[i][j][k] = color;
                            flag = 0;
                            break;
                        }
                        else{
                            cube[i][j][k] = '.';
                            j++;
                            continue;
                        }
                    }
                    else{
                        break;
                    }
                }
            }
            if(flag) return 0; /*Not change*/
            else return 1;
            break;
        case 5:/*bottom*/

            flag = 1;
            i = x, j = size - 1, k = y;
            while(j > 0){
                if(color == '.'){
                    if(cube[i][j][k] != color)flag = 0;
                    cube[i][j][k] = '.';
                    j--;
                    continue;
                }
                else {
                    if(cube[i][j][k] != color){
                        if(cube[i][j][k] == '0'){
                            cube[i][j][k] = color;
                            flag = 0;
                            break;
                        }
                        else{
                            cube[i][j][k] = '.';
                            j--;
                            continue;
                        }
                    }
                    else{
                        break;
                    }
                }
            }
            if(flag) return 0; /*Not change*/
            else return 1;
            break;
    }
}
