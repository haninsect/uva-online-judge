#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFERSIZE 1024
#define DEBUG_MESSAGE
#define INSTRUCTION 1000
#define DIFINITION 300
#define EXTERNAL 150
#define OBJECT 100
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
        char instructions[INSTRUCTION][20][15] = {'\0'}; /*1000 lines , every line 20 token and every string 15 words*/
        memset(instructions, '\0', sizeof(instructions));

        char buffer[BUFFERSIZE] = {'\0'};
        memset(buffer, '\0', sizeof(buffer));

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
                if(tokenCounter > 20) {
                    printf("Tokens limits\n");
                    exit(1);
                }
            }
            lineCounter++;
            memset(buffer, 0, sizeof(buffer));
        }
        if(lineCounter > INSTRUCTION) {
            printf("Lines limit\n");
            exit(1);
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
        char modulesName[DIFINITION][15] = {'\0'};
        memset(modulesName, '\0', sizeof(modulesName));

        char sortedName[DIFINITION][15] = {'\0'};
        memset(sortedName, '\0', sizeof(sortedName));

        int modulesAddr[DIFINITION] = {0};
        memset(modulesAddr, 0, sizeof(modulesAddr));

        int modulesDup[DIFINITION] = {0};
        memset(modulesAddr, 0, sizeof(modulesAddr));

        int memLocate = 256;
        int mpt = 0;
        for(i = 0; i < lineCounter; i++){
                /*'D'add a module name and addr to array.*/
                /*'C' Update memLocate .*/
            if (strcmp(instructions[i][0], "D") == 0){
                int dup = 0;
                for(j = 0; j < mpt; j++){
                    if(strcmp(modulesName[j] ,instructions[i][1]) == 0){
                        dup = 1;
                        modulesDup[j] = 1;
                        break;
                    }
                }
                if(!dup){
                    strcpy(modulesName[mpt] ,instructions[i][1]);
                    strcpy(sortedName[mpt] ,instructions[i][1]);
                    modulesAddr[mpt] = (int)strtol(instructions[i][2], NULL, 16) + memLocate;
                    mpt++;
                    if(mpt > DIFINITION) {
                        printf("Definition limits\n");
                        exit(1);
                    }
                }
            }
            else if (strcmp(instructions[i][0], "C") == 0){
                memLocate += (int)strtol(instructions[i][1], NULL, 16);
            }

        }
        /*Construct externals' name and addr for every modules*/
        char externalsName[OBJECT][EXTERNAL][15] = {'\0'};
        memset(externalsName, '\0', sizeof(externalsName));

        int externalsAddr[OBJECT][EXTERNAL] = {0};
        memset(externalsAddr, 0, sizeof(externalsAddr));

        int externalsNumber[OBJECT] = {0};
        memset(externalsNumber, 0, sizeof(externalsNumber));

        int modulesNumber = mpt;
        int opt= 0;
        int ept = 0;
        for(i = 0; i < lineCounter; i++){
                /*'D' update module pointer and reset ept*/
                /*'E' add a external name and addr, ept+1*/
            if (strcmp(instructions[i][0], "E") == 0){
                for(j = 0; j < modulesNumber; j++){
                    if(strcmp(modulesName[j], instructions[i][1]) == 0){
                        strcpy(externalsName[opt][ept] ,instructions[i][1]);
                        if(modulesAddr[j] == -1) externalsAddr[opt][ept] = 0;
                        else externalsAddr[opt][ept] = modulesAddr[j];
                        break;
                    }
                }
                if (j == modulesNumber){/*Undefined Symbols*/
                    strcpy(externalsName[opt][ept] ,instructions[i][1]);
                    externalsAddr[opt][ept] = 0;
                    /*Use -1 as the symbol's addr*/
                    strcpy(modulesName[modulesNumber] ,instructions[i][1]);
                    strcpy(sortedName[modulesNumber] ,instructions[i][1]);
                    modulesAddr[modulesNumber] = -1;
                    modulesNumber++;
                    if(modulesNumber > DIFINITION) {
                        printf("Definition limits\n");
                        exit(1);
                    }
                }
                ept++;
            }
            else if (strcmp(instructions[i][0], "Z") == 0){
                externalsNumber[opt] = ept;
                opt++;
                ept = 0;
                if(ept > EXTERNAL) {
                        printf("External limits\n");
                        exit(1);
                    }
                if(opt > EXTERNAL) {
                        printf("External limits\n");
                        exit(1);
                }
            }

        }
        externalsNumber[opt] = ept;
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
        opt = 0;
        for(i = 0; i < lineCounter; i++){
                /*'C' go through the values in the instruction.*/
            if (strcmp(instructions[i][0], "C") == 0){
                int n = (int)strtol(instructions[i][1], NULL, 16);
                /*printf("C: %x\n", n);*/
                for(j = 2; j < n+2; j++){
                    if (strcmp(instructions[i][j], "$")==0 ){
                        j++;
                        int external = (int)strtol(instructions[i][j], NULL, 16);
                        if(external > externalsNumber[opt]) {
                            printf("$ limits\n");
                            exit(1);
                        }
                        checkSum = ((checkSum << 1 & 0x0000ffff) | (checkSum >> 15 & 0xFFFF));
                        checkSum += (externalsAddr[opt][external])/256;
                        checkSum &= 0x0000ffff;
                        checkSum = ((checkSum << 1 & 0x0000ffff) | (checkSum >> 15 & 0xFFFF));
                        checkSum += (externalsAddr[opt][external] %256);
                    }
                    else {
                        checkSum = ((checkSum << 1 & 0x0000ffff) | (checkSum >> 15 & 0xFFFF));
                        checkSum += (int)strtol(instructions[i][j], NULL, 16);
                    }
                    checkSum &= 0x0000ffff;
                    /*printf("%s: %04x\n", instructions[i][j], checkSum);*/
                } /*printf("\n", instructions[i][j]);*/
            }/*End of 'C' instructions*/
             else if (strcmp(instructions[i][0], "Z") == 0){
                opt++;
            }
        }

        #ifndef DEBUG_MESSAGE
        printf("CheckSum: %04X\n", checkSum);
        #endif
        /*Computer checksum, and sorted modulesName array*/
        qsort(sortedName, modulesNumber, sizeof(char)*15, cmp);

        /*Show*/
        /*The undefined symbol's addr is -1 which means ????*/
        if(caseNumber != 1 )printf("\n");
        printf("Case %d: checksum = %04X\n", caseNumber, checkSum);
        printf(" SYMBOL   ADDR\n");
        printf("--------  ----\n");
        for(i = 0; i < modulesNumber; i++){

            for(j = 0; j < modulesNumber; j++){
                if(strcmp(modulesName[j], sortedName[i]) == 0){
                    if(modulesAddr[j]!=-1){
                        printf("%-9s %04X", modulesName[j], modulesAddr[j]);
                        if(modulesDup[j]){
                            printf(" M\n");
                        }
                        else printf("\n");
                    }
                    else
                        printf("%-9s ????\n", modulesName[j]);
                    break;
                }
            }
        }
        caseNumber++;
    }
    return 0;
}

int cmp(void* a, void* b)
{
    return(strcmp((char**)a, (char**)b));
}
