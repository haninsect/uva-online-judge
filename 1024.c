#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFERSIZE 1024
#define DEBUG_MESSAGE

int cmp(void* a, void* b);
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i,j,k;
    int caseNumber = 1;
    while(1){
        /*Get input, store in arrays*/
        char instructions[50][20][20] = {'\0'}; /*1000 lines , every line 20 token and every string 20 words*/
        char buffer[BUFFERSIZE] = {'\0'};
        int lineCounter = 0;
        while( fgets(buffer, BUFFERSIZE , stdin) ) {
            if(buffer[0] == '$')break;
            int ln = strlen(buffer)-1;
            if (buffer[ln] == '\n')
                buffer[ln] = '\0';
            char *token;
            int tokenCounter = 0;
            for(token = strtok(buffer, " "); token != NULL; token = strtok(NULL, " ")) {
                strcpy(instructions[lineCounter][tokenCounter], token);
                tokenCounter++;
            }
            lineCounter++;
        }
        if(lineCounter == 0) break;
        #ifndef DEBUG_MESSAGE
        /*for(i = 0; i < lineCounter; i++){
            for(j = 0; j < tokensInLine[i]; j++){
                printf("%s ", instructions[i][j]);
            }printf("\n");
        }*/
        #endif
        /*Construct modules' name and addr*/
        char modulesName[50][15] = {'\0'};
        char sortedName[50][15] = {'\0'};
        int modulesAddr[50] = {0};
        int memLocate = 256;
        int mpt = -1;
        for(i = 0; i < lineCounter; i++){
                /*'D'add a module name and addr to array.*/
                /*'C' Update memLocate .*/
            if (strcmp(instructions[i][0], "D") == 0){
                mpt++;
                strcpy(modulesName[mpt] ,instructions[i][1]);
                strcpy(sortedName[mpt] ,instructions[i][1]);
                modulesAddr[mpt] = (int)strtol(instructions[i][2], NULL, 16) + memLocate;
            }
            else if (strcmp(instructions[i][0], "C") == 0){
                memLocate += (int)strtol(instructions[i][1], NULL, 16);
            }

        }
        /*Construct externals' name and addr for every modules*/
        char externalsName[50][50][15] = {'\0'};
        int externalsAddr[50][50] = {0};
        int externalsNumber[50] = {0};
        int modulesNumber = mpt+1;
        mpt = -1;
        int ept = 0;
        for(i = 0; i < lineCounter; i++){
                /*'D' update module pointer and reset ept*/
                /*'E' add a external name and addr, ept+1*/
            if (strcmp(instructions[i][0], "D") == 0){
                if(mpt >= 0) externalsNumber[mpt] = ept;
                mpt++;
                ept = 0;
            }
            else if (strcmp(instructions[i][0], "E") == 0){
                for(j = 0; j < modulesNumber; j++){
                    if(strcmp(modulesName[j], instructions[i][1]) == 0){
                        strcpy(externalsName[mpt][ept] ,instructions[i][1]);
                        externalsAddr[mpt][ept] = modulesAddr[j];
                        break;
                    }
                }
                if (j == modulesNumber){/*Undefined Symbols*/
                    strcpy(externalsName[mpt][ept] ,instructions[i][1]);
                    externalsAddr[mpt][ept] = 0;
                    /*Use -1 as the symbol's addr*/
                    strcpy(modulesName[modulesNumber] ,instructions[i][1]);
                    strcpy(sortedName[modulesNumber] ,instructions[i][1]);
                    modulesAddr[modulesNumber] = -1;
                    modulesNumber++;
                }
                ept++;
            }

        }
        externalsNumber[mpt] = ept;
        #ifndef DEBUG_MESSAGE
        for(i = 0; i < modulesNumber; i++){
            printf("%d :%s\n", i, sortedName[i]);
            if(modulesAddr[i] == -1)
                printf("%s \?\?\?\?\n", modulesName[i]);
            else
                printf("%s %4x\n", modulesName[i], modulesAddr[i]);

            for(j = 0; j < externalsNumber[i]; j++){
                printf("    %s, %x\n", externalsName[i][j], externalsAddr[i][j]);
            }
        }
        #endif
        /*Generate checksum array*/
        int checkSum = 0;
        mpt = -1;
        for(i = 0; i < lineCounter; i++){
                /*'D' update module pointer*/
                /*'C' go through the values in the instruction.*/
            if (strcmp(instructions[i][0], "D") == 0){
                mpt++;
            }
            else if (strcmp(instructions[i][0], "C") == 0){
                int n = (int)strtol(instructions[i][1], NULL, 16);
                /*printf("C: %x\n", n);*/
                for(j = 2; j < n+2; j++){
                    if (strcmp(instructions[i][j], "$")==0 ){
                        j++;
                        int external = (int)strtol(instructions[i][j], NULL, 16);
                        checkSum = ((checkSum << 1) | (checkSum >> 15)) & 0xFFFF;
                        checkSum += (externalsAddr[mpt][external])/256;
                        checkSum &= 0x0000ffff;
                        checkSum = ((checkSum << 1) | (checkSum >> 15)) & 0xFFFF;
                        checkSum += (externalsAddr[mpt][external] %256);
                    }
                    else {
                        checkSum = ((checkSum << 1) | (checkSum >> 15)) & 0xFFFF;
                        checkSum += (int)strtol(instructions[i][j], NULL, 16);
                    }
                    checkSum &= 0x0000ffff;
                    /*printf("%s: %04x\n", instructions[i][j], checkSum);*/
                } /*printf("\n", instructions[i][j]);*/
            }/*End of 'C' instructions*/
        }

        #ifndef DEBUG_MESSAGE
        printf("CheckSum: %04X\n", checkSum);
        #endif
        /*Computer checksum, and sorted modulesName array*/
        qsort(sortedName, modulesNumber, sizeof(char)*10, cmp);

        /*Show*/
        /*The undefined symbol's addr is -1 which means ????*/
        if(caseNumber != 1 )printf("\n");
        printf("Case %d: checksum = %04X\n", caseNumber, checkSum);
        printf(" SYMBOL   ADDR\n");
        printf("--------  ----\n");
        for(i = 0; i < modulesNumber; i++){
            if(i != 0) if(strcmp(sortedName[i], sortedName[i-1]) == 0) continue;

            for(j = 0; j < modulesNumber; j++){
                if(strcmp(modulesName[j], sortedName[i]) == 0){
                    if(modulesAddr[j]!=-1)
                        printf("%-9s %04X", modulesName[j], modulesAddr[j]);
                    else
                        printf("%-9s ????", modulesName[j]);
                    break;
                }
            }

            if(strcmp(sortedName[i], sortedName[i+1]) == 0){
                printf(" M\n");
            }
            else   printf("\n");
        }
        caseNumber++;
    }
    return 0;
}

int cmp(void* a, void* b)
{
    return(strcmp((char**)a, (char**)b));
}
