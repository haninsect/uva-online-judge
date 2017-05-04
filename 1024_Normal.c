#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEBUG_MESSAGE
#define INSTRUCTIONS 1000
#define DIFINITIONS 300
#define REFERENCES 150
#define OBJECTS 100

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
        /*Get input, store in array*/
        char instructions[INSTRUCTIONS][20][15] = {'\0'}; /*N lines , every line with 20 token and every string 15 words*/
        int lineCounter = 0;
        while(1) {
            char op[2];
            int tokenCounter = 0;
            scanf("%s", &op);
            if(op[0] == '$')break;
            strcpy(instructions[lineCounter][0], op);
            switch(op[0])
            {
            case 'D':
                scanf("%s", &instructions[lineCounter][1]);
                scanf("%s", &instructions[lineCounter][2]);
                break;
            case 'E':
                scanf("%s", &instructions[lineCounter][1]);
                break;
            case 'C':
                scanf("%s", &instructions[lineCounter][1]);
                int c = (int)strtol(instructions[lineCounter][1], NULL, 16);
                for(i = 0; i < c; i++){
                    scanf("%s", &instructions[lineCounter][2+i]);
                }
                break;
            }

            lineCounter++;
        }
        if(lineCounter > INSTRUCTIONS) {
            printf("Lines limit\n");
            exit(1);
        }
        if(lineCounter == 0) break;
        #ifndef DEBUG_MESSAGE
        for(i = 0; i < lineCounter; i++){
            for(j = 0; j < tokensInLine[i]; j++){
                printf("%s ", instructions[i][j]);
            }printf("\n");
        }
        #endif
        
        /*Three steps for 'D', 'E', 'C'.*/
        
        /*Construct externals' name and addr*/
        char externalsName[DIFINITIONS][15] = {'\0'};
        char sortedName[DIFINITIONS][15] = {'\0'};
        int externalsAddr[DIFINITIONS] = {0};
        int externalsDup[DIFINITIONS] = {0};
        int memLocate = 256;
        int ept = 0;
        for(i = 0; i < lineCounter; i++){
                /*'D' add a module name and addr to the array. If duplicate module, set flag.*/
                /*'C' increase memLocate .*/
            if (strcmp(instructions[i][0], "D") == 0){
                int dup = 0;
                for(j = 0; j < ept; j++){
                    if(strcmp(externalsName[j] ,instructions[i][1]) == 0){
                        dup = 1;
                        externalsDup[j] = 1;
                        break;
                    }
                }
                if(!dup){
                    strcpy(externalsName[ept] ,instructions[i][1]);
                    strcpy(sortedName[ept] ,instructions[i][1]);
                    externalsAddr[ept] = (int)strtol(instructions[i][2], NULL, 16) + memLocate;
                    ept++;
                    if(ept > DIFINITIONS) {
                        printf("Definition limits\n");
                        exit(1);
                    }
                }
            }
            else if (strcmp(instructions[i][0], "C") == 0){
                memLocate += (int)strtol(instructions[i][1], NULL, 16);
            }

        }
        /*Construct externals' name and addr for every externals*/
        char referencesName[OBJECTS][REFERENCES][15] = {'\0'};
        int referencesAddr[OBJECTS][REFERENCES] = {0};
        int referencesNumber[OBJECTS] = {0};
        int externalsNumber = ept;
        int opt = 0;
        int rpt = 0;
        for(i = 0; i < lineCounter; i++){
                /*'Z' increase the object pointer and reset the reference pointer.*/
                /*'E' add a external name and addr, rpt+1*/
            if (strcmp(instructions[i][0], "E") == 0){
                for(j = 0; j < externalsNumber; j++){
                    if(strcmp(externalsName[j], instructions[i][1]) == 0){
                        strcpy(referencesName[opt][rpt] ,instructions[i][1]);
                        if(externalsAddr[j] == -1) referencesAddr[opt][rpt] = 0;
                        else referencesAddr[opt][rpt] = externalsAddr[j];
                        break;
                    }
                }
                if (j == externalsNumber){/*Undefined Symbols*/
                    strcpy(referencesName[opt][rpt] ,instructions[i][1]);
                    referencesAddr[opt][rpt] = 0;
                    /*Use -1 as the symbol's addr*/
                    strcpy(externalsName[externalsNumber] ,instructions[i][1]);
                    strcpy(sortedName[externalsNumber] ,instructions[i][1]);
                    externalsAddr[externalsNumber] = -1;
                    externalsNumber++;
                    if(externalsNumber > DIFINITIONS) {
                        printf("Definition limits\n");
                        exit(1);
                    }
                }
                rpt++;
            }
            else if (strcmp(instructions[i][0], "Z") == 0){
                referencesNumber[opt] = rpt;
                opt++;
                rpt = 0;
                if(rpt > REFERENCES) {
                        printf("External limits\n");
                        exit(1);
                    }
                if(opt > REFERENCES) {
                        printf("External limits\n");
                        exit(1);
                }
            }

        }
        referencesNumber[opt] = rpt;
        #ifndef DEBUG_MESSAGE
        for(i = 0; i < externalsNumber; i++){
            printf("%d :%s\n", i, sortedName[i]);
            if(externalsAddr[i] == -1)
                printf("%s \?\?\?\?\n", externalsName[i]);
            else
                printf("%s %4x\n", externalsName[i], externalsAddr[i]);

            for(j = 0; j < referencesNumber[i]; j++){
                printf("    %s, %x\n", referencesName[i][j], referencesAddr[i][j]);
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
                for(j = 2; j < n+2; j++){
                    if (strcmp(instructions[i][j], "$")==0 ){
                        j++;
                        int external = (int)strtol(instructions[i][j], NULL, 16);
                        if(external > referencesNumber[opt]) {
                            printf("$ limits\n");
                            exit(1);
                        }
                        checkSum = ((checkSum << 1 & 0x0000ffff) | (checkSum >> 15 & 0xFFFF));
                        checkSum += (referencesAddr[opt][external])/256;
                        checkSum &= 0x0000ffff;
                        checkSum = ((checkSum << 1 & 0x0000ffff) | (checkSum >> 15 & 0xFFFF));
                        checkSum += (referencesAddr[opt][external] %256);
                    }
                    else {
                        checkSum = ((checkSum << 1 & 0x0000ffff) | (checkSum >> 15 & 0xFFFF));
                        checkSum += (int)strtol(instructions[i][j], NULL, 16);
                    }
                    checkSum &= 0x0000ffff;
                }
            }/*End of 'C' instructions*/
             else if (strcmp(instructions[i][0], "Z") == 0){
                opt++;
            }
        }

        #ifndef DEBUG_MESSAGE
        printf("CheckSum: %04X\n", checkSum);
        #endif
        /*Computer checksum, and sorted externalsName array*/
        qsort(sortedName, externalsNumber, sizeof(char)*15, cmp);

        /*Show*/
        /*The undefined symbol's addr is -1 which means ????*/
        if(caseNumber != 1 )printf("\n");
        printf("Case %d: checksum = %04X\n", caseNumber, checkSum);
        printf(" SYMBOL   ADDR\n");
        printf("--------  ----\n");
        for(i = 0; i < externalsNumber; i++){

            for(j = 0; j < externalsNumber; j++){
                if(strcmp(externalsName[j], sortedName[i]) == 0){
                    if(externalsAddr[j]!=-1){
                        printf("%-9s %04X", externalsName[j], externalsAddr[j]);
                        if(externalsDup[j]){
                            printf(" M\n");
                        }
                        else printf("\n");
                    }
                    else
                        printf("%-9s ????\n", externalsName[j]);
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
