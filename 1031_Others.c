/*1031 - Insecure in Prague*/
#include <stdio.h>
#include <string.h>
//#define DM
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		freopen("output.out", "w", stdout);
	#endif
    int i,j,k,l;
    int caseNumber = 1;
    while(1){
        char InputString[50];
        scanf("%s", &InputString);
        int InputLen =  strlen(InputString);
        if(strcmp(InputString, "X") == 0) break;
        int alphabetNumber[26] = {0};
        for(i = 0; i < InputLen; i++){
            alphabetNumber[InputString[i] - 'A']++;
        }
        /*five loop to find all possible*/
        int ansNumber = 0;
        char ansString[InputLen/2];
        for(i = 0; i < InputLen/2; i++) ansString[i] = '\0';

        int len;
        for(len = InputLen/2; len > 0 && ansNumber < 1; len--){
            #ifndef DM
            printf("Start len %d:\n", len);
            #endif
            int s;
            for(s = 0; s < InputLen && ansNumber < 2; s++){
                for(i = 0; i < InputLen-1 && ansNumber < 2; i++){
                    int queue[InputLen];
                    int uqueue[InputLen];
                    int qn = InputLen;
                    int alphabetFilled[26] = {0};
                    char tmpAns[len+1];
                    for(j = 0; j < InputLen; j++) queue[j] = j;
                    for(j = 0; j < InputLen; j++) uqueue[j] = j;
                    for(j = 0; j < 26; j++) alphabetFilled[j] = 0;
                    for(j = 0; j < len; j++) tmpAns[j] = '\0';
                    /*Two times fill */
                    /*Find first letter*/
                    /*Find all letter lest*/
                        /*If one alphabet is less than 1/2*total, then cut*/
                    int lpt = queue[s];
                    qn--;
                    for(l = s; l < qn; l++) uqueue[l] = uqueue[l+1];
                    for(l = 0; l < qn; l++) queue[l] = uqueue[(l+s)%qn];
                    int c = 0;
                    int wrong = 0;
                    while(c < len){
                        tmpAns[c] = InputString[lpt];
                        alphabetFilled[tmpAns[c] - 'A']++;
                        if(alphabetFilled[tmpAns[c] - 'A'] > alphabetNumber[tmpAns[c] - 'A']/2){
                            tmpAns[c+1] = '\0';
                            printf("    Wrong: %s for s,i (%d, %d)\n", tmpAns, s, i);
                            wrong = 1;
                            break;
                        }
                        c++;
                        if( c == len) break;
                        lpt = queue[i%qn];
                        printf("%d : ", i%qn);
                        for(l = 0; l < qn; l++) printf("%d ", queue[l]);
                        for(l = i%qn; l < qn-1; l++) uqueue[l] = uqueue[l+1];
                        for(l = 0; l < qn-1; l++) queue[l] = uqueue[(l+i%qn)%qn];
                        qn--;
                    }
                    if(wrong) continue;
                    tmpAns[c] = '\0';
                    #ifndef DM
                    printf("    chose %s for s,i (%d, %d)\n", tmpAns, s, i);
                    #endif
                    /*Second times*/
                    int t;
                    for(t = 0; t < InputLen && ansNumber < 2; t++){
                        for( j = i+1; j < InputLen && ansNumber < 2; j++){
                            int tmpQueue[InputLen];
                            int tmpLpt = lpt;
                            int tmpQn = qn;
                            for(l = 0; l < qn; l++) tmpQueue[l] = queue[l];
                            wrong = 0;
                            tmpLpt = tmpQueue[t%tmpQn];
                            for(l = t%tmpQn; l < tmpQn-1; l++) tmpQueue[l] = tmpQueue[l+1];
                            tmpQn--;
                            c = 0;
                            #ifndef DM
                            printf("        check ");
                            #endif
                            while(c < len){
                                if(tmpAns[c] != InputString[tmpLpt]){
                                    #ifndef DM
                                    printf(" Wrong char: %c should be %c. t,j (%d, %d)", InputString[tmpLpt], tmpAns[c], t, j);
                                    #endif
                                    wrong = 1;
                                    break;
                                }
                                #ifndef DM
                                printf("%c", InputString[tmpLpt]);
                                #endif
                                c++;
                                if( c == len) break;
                                tmpLpt = tmpQueue[i%tmpQn];
                                for(l = i%tmpQn; l < tmpQn-1; l++) tmpQueue[l] = tmpQueue[l+1];
                                tmpQn--;
                            }
                            #ifndef DM
                            printf("\n          result: %d\n", wrong);
                            #endif
                            if(wrong) continue;
                            else {
                                if(strcasecmp(ansString, tmpAns) == 0) continue;
                                strcpy(ansString, tmpAns);
                                #ifndef DM
                                printf("    Find ans: %s\n", ansString);
                                #endif
                                ansNumber++;
                            }
                        }
                    }


                }/*End of i slots.*/
            }/*End of a case.*/
        }/*End of find ans.*/







        if(ansNumber == 1)
            printf("Code %d: %s\n", caseNumber, ansString);
        else
            printf("Code %d: Codeword not unique\n", caseNumber);

        caseNumber++;

    }
    return 0;
}


