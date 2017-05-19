/*1033 - Merging Maps*/
#include <stdio.h>
//#define Show_DM

#define MAX_MAP 50
#define MAX_ROW 100
#define MAX_COL 100
#define MAX_MATCH 2500
typedef struct{
    int isValid;
    int r, c;
} mapInfor;

typedef struct{
    int score;
    int mapA, mapB;
    int offsetR, offsetC;
} match;

int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif

    int caseNumber = 1;
    int i,j,k;
    while(1){
        int mapNumber;
        scanf("%d", &mapNumber);
        if (mapNumber == 0) break;

        char maps[MAX_MAP][MAX_ROW][MAX_COL];
        mapInfor mapIn[MAX_MAP];
        for (i = 0; i < MAX_MAP; i++) {
            mapIn[i].isValid = 0;
            mapIn[i].r = 0;
            mapIn[i].c = 0;
            for(j = 0; j < MAX_ROW; j++) {
                for(k = 0; k < MAX_COL; k++){
                    maps[i][j][k] = '-';
                }
            }
        }

        for (i = 0; i < mapNumber; i++) {
            int r, c;
            scanf("%d %d", &r, &c);
            mapIn[i].isValid = 1;
            mapIn[i].r = r;
            mapIn[i].c = c;
            for(j = 0; j < r; j++) {
                for(k = 0; k < c; k++){
                    char c;
                    while((c = getchar()) == '\n');
                    maps[i][j][k] = c;
                }
            }
        }
        #ifndef Show_DM
        for (i = 0; i < mapNumber; i++) {
            printf("MAP %d:\n", i);
            for(j = 0; j < mapIn[i].r; j++) {
                printf("    ");
                for(k = 0; k < mapIn[i].c; k++){
                    printf("%c", maps[i][j][k]);
                }
                printf("\n");
            }
            printf("\n");
        }
        #endif
        /*First Round*/
            /*Calculate match-scores for every combination*/
            /*Pop the highest score match*/
            /*Merge tow map and Refresh map information*/
        int matchNumber = 0;
        match matches[MAX_MATCH];
        for(i = 0; i < MAX_MATCH; i++){
            matches[i].score = 0;
        }
        for (i = 0; i < mapNumber; i++) {
            for(j = i+1; j < mapNumber; j++) {
                CalculateScore(matchNumber, matches, maps, mapIn, i, j);
                matchNumber++;
            }
        }

        int maxMatch = -1;
        maxMatch = GetHighestMatch(matchNumber, matches);
        #ifndef Show_DM
        printf("First Round:\n");
        for(i = 0; i < matchNumber; i++){
            printf("match %d:\n", i);
            printf("     score: %d\n", matches[i].score);
            printf("     mapA: %d, mapB: %d\n", matches[i].mapA, matches[i].mapB);
            printf("     offR: %d, offC: %d\n", matches[i].offsetR, matches[i].offsetC);
        }
        for (i = 0; i < mapNumber; i++) {
            printf("MAP %d:\n", i);
            for(j = 0; j < mapIn[i].r; j++) {
                printf("    ");
                for(k = 0; k < mapIn[i].c; k++){
                    printf("%c", maps[i][j][k]);
                }
                printf("\n");
            }
            printf("\n");
        }
        #endif
        if(maxMatch != -1) MergeMaps(maxMatch, matchNumber, matches, maps, mapIn);
        mapNumber++;

        /*The rest round*/
        while(maxMatch != -1){
            for (i = 0; i < mapNumber-1; i++) {
                if (mapIn[i].isValid == 0) continue;
                CalculateScore(matchNumber, matches, maps, mapIn, i, mapNumber-1);
                matchNumber++;
            }

            maxMatch = GetHighestMatch(matchNumber, matches);
            if(maxMatch == -1) break;
            MergeMaps(maxMatch, matchNumber, matches, mapNumber, maps, mapIn);
            mapNumber++;
        }

        /*Show the remaining maps.*/
        if(caseNumber > 1) printf("\n");
        printf("Case %d\n", caseNumber);
        int c = 1;
        for(i = 0; i < mapNumber; i++) {
            if(mapIn[i].isValid){
                if(c > 1) printf("\n");
                printf("    MAP %d:\n");

                printf("    +");
                for(k = 0; k < mapIn[i].c; k++){
                    printf("-");
                }
                printf("+\n");

                for(j = 0; j < mapIn[i].r; j++) {
                    printf("    |");
                    for(k = 0; k < mapIn[i].c; k++){
                        printf("%c", maps[i][j][k]);
                    }
                    printf("|\n");
                }

                printf("    +");
                for(k = 0; k < mapIn[i].c; k++){
                    printf("-");
                }
                printf("+\n");

                c++;
            }
        }
        caseNumber++;
    }
}

void CalculateScore(int matchNumber, match matches[MAX_MATCH], char maps[MAX_MAP][MAX_ROW][MAX_COL], mapInfor mapIn[MAX_MAP], int mapA, int mapB)
{
    int i, j, k, l;
    /*For r -> 1*/
        /*For c -> 1*/
            /* Compute score */
            /* Assign the highest into matchNumber*/
    matches[matchNumber].mapA = mapA;
    matches[matchNumber].mapB = mapB;

    for(i = mapIn[mapA].r -1; i > -mapIn[mapB].r; i--){
        for(j = mapIn[mapA].c -1; j > -mapIn[mapB].c; j--){
            int score = 0;
            int isValid = 1;
            for(k = 0; k < mapIn[mapB].r && isValid; k++){
                if(i + k < 0) continue;
                if(i + k >= mapIn[mapA].r) break;

                for(l = 0; l < mapIn[mapB].c && isValid; l++){
                    if(j + l < 0) continue;
                    if(j + l >= mapIn[mapA].c) break;
                    if(maps[mapA][i+k][j+l] != '-' && maps[mapA][i+k][j+l] == maps[mapB][k][l]) score++;
                    if(maps[mapA][i+k][j+l] != '-' && maps[mapB][k][l] != '-' && maps[mapA][i+k][j+l] != maps[mapB][k][l]) isValid = 0;
                }
            }
            if(score >= matches[matchNumber].score && isValid){
                matches[matchNumber].score = score;
                matches[matchNumber].offsetR = i;
                matches[matchNumber].offsetC = j;
            }
        }
    }


}

int GetHighestMatch(int matchNumber, match matches[MAX_MATCH])
{
    int i;
    /*scan the array, return the max one*/
    int maxMatch = -1;
    int maxScore = 0;
    for(i = 0; i < matchNumber; i++){
        if( matches[i].score > maxScore){
            maxMatch = i;
            break;
        }
    }
    /*If no matches can find, return -1*/
    return maxMatch;
}

void MergeMaps(int maxMatch, int matchNumber, match matches[MAX_MATCH], int mapNumber, char maps[MAX_MAP][MAX_ROW][MAX_COL], mapInfor mapIn[MAX_MAP])
{
    int i, j, k;
    /*Compute the new row and col*/
    int offa_r, offa_c, offb_r, offb_c;
    if(matches[maxMatch].offsetR >= 0){
        if(mapIn[matches[maxMatch].mapB].r + matches[maxMatch].offsetR >= mapIn[matches[maxMatch].mapA].r) mapIn[mapNumber].r = mapIn[matches[maxMatch].mapB].r + matches[maxMatch].offsetR;
        else mapIn[mapNumber].r = mapIn[matches[maxMatch].mapA].r;
        offa_r = 0;
        offb_r = matches[maxMatch].offsetR;
    }
    else {
        if(mapIn[matches[maxMatch].mapB].r >= mapIn[matches[maxMatch].mapA].r + (-matches[maxMatch].offsetR)) mapIn[mapNumber].r = mapIn[matches[maxMatch].mapB].r;
        else mapIn[mapNumber].r = mapIn[matches[maxMatch].mapA].r + (-matches[maxMatch].offsetR);
        offa_r = (-matches[maxMatch].offsetR);
        offb_r = 0;
    }
    if(matches[maxMatch].offsetC >= 0){
        if(mapIn[matches[maxMatch].mapB].c + matches[maxMatch].offsetC >= mapIn[matches[maxMatch].mapA].c) mapIn[mapNumber].c = mapIn[matches[maxMatch].mapB].c + matches[maxMatch].offsetC;
        else mapIn[mapNumber].r = mapIn[matches[maxMatch].mapA].c;
        offa_c = 0;
        offb_c = matches[maxMatch].offsetC;
    }
    else {
        if(mapIn[matches[maxMatch].mapB].c >= mapIn[matches[maxMatch].mapA].c + (-matches[maxMatch].offsetC)) mapIn[mapNumber].c = mapIn[matches[maxMatch].mapB].c;
        else mapIn[mapNumber].c = mapIn[matches[maxMatch].mapA].c + (-matches[maxMatch].offsetC);
        offa_c = (-matches[maxMatch].offsetC);
        offb_c = 0;
    }
    /*Assign data into the new map*/
    mapIn[mapNumber].isValid = 1;
    for(i = 0; i < mapIn[matches[maxMatch].mapA].r; i++){
        for(j = 0; j < mapIn[matches[maxMatch].mapA].c; j++){
            maps[mapNumber][i + offa_r][j + offa_c] = maps[matches[maxMatch].mapA][i][j];
        }
    }
    for(i = 0; i < mapIn[matches[maxMatch].mapB].r; i++){
        for(j = 0; j < mapIn[matches[maxMatch].mapB].c; j++){
            maps[mapNumber][i + offb_r][j + offb_c] = maps[matches[maxMatch].mapB][i][j];
        }
    }
    /*UnValid the map being merged and the corresponding matches*/
    mapIn[matches[maxMatch].mapA].isValid = 0;
    mapIn[matches[maxMatch].mapB].isValid = 0;
    for(i = 0; i < matchNumber; i++){
        if(matches[i].mapA == matches[maxMatch].mapA || matches[i].mapA == matches[maxMatch].mapB) matches[i].score = 0;
        if(matches[i].mapB == matches[maxMatch].mapA || matches[i].mapB == matches[maxMatch].mapB) matches[i].score = 0;

    }
}
