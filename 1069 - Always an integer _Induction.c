/*1069 - Always an integer _Induction */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#define DBUGM1
#define DBUGM_ShowPolynomial
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxSize 100*100
#define MaxDegree 105
char input[MaxSize];
int coefficient[MaxDegree];
int Divisor;
int degree;
void ShowPolynomial(int dg, int polynomial[MaxDegree])
{
    int i;
    printf("(");
    if(dg > 1){
        if(polynomial[dg] == 1) printf("n^%d", dg);
        else if(polynomial[dg] == -1) printf("-n^%d", dg);
        else printf("%dn^%d", polynomial[dg], dg);
    }
    for(i = dg-1; i >= 2; i--){
        if(polynomial[i] == 0) continue;
        if (polynomial[i] > 0) printf("+");
        if(polynomial[dg] == 1) printf("n^%d", i);
        else if(polynomial[dg] == -1) printf("-n^%d", i);
        else printf("%dn^%d", polynomial[i], i);
    }
    if(polynomial[1] != 0){
        if(dg != 1){
            if (polynomial[1] > 0 ) printf("+");
        }
        if(polynomial[1] == 1) printf("n");
        else if(polynomial[1] == -1) printf("-n");
        else printf("%dn", polynomial[1]);
    }
    if(polynomial[0] != 0){
        if(dg != 0){
            if (polynomial[0] > 0 ) printf("+");
        }
        printf("%d", polynomial[0]);
    }
    printf(")/%d\n", Divisor);
}
int GetDecimal(int *nowPointer)
{
    #ifdef DBUGM
        printf("GetDecimal: input[%d]: %c\n", *nowPointer, input[*nowPointer] );
    #endif
    int i;
    int sign = 1;
    if(!isdigit(input[*nowPointer])){
         if(input[*nowPointer] == 'n') return 1;
         if(input[*nowPointer] == ')') return 0;
         if(input[*nowPointer] == '-') sign = -1;
         (*nowPointer)++;
    }
    if(input[*nowPointer] == 'n') return 1*sign;

    int r = 0;
    int digits = 0;
    while(isdigit(input[*nowPointer])){
        r = r*10 + (input[*nowPointer]-'0');
        (*nowPointer)++;
    }
    #ifdef DBUGM
        printf("return: %d\n", r*sign);
    #endif
    return r*sign;
}
void Read()
{
    int i, j;
    for(i = 0; i < MaxDegree; i++) coefficient[i] = 0;
    int np = 1;
    degree = 0;
    while(np < strlen(input)){
        int coe = GetDecimal(&np);
        if(input[np] == ')'){
            coefficient[0] = coe;
            np += 2;
            Divisor = GetDecimal(&np);
            break;
        }
        np++;
        int dg = 1;
        if(input[np] == '^') np++, dg = GetDecimal(&np);
        if(degree < dg) degree = dg;
        #ifdef DBUGM
            printf("Read Degree: %d, Coefficient: %d\n", dg, coe);
        #endif
        coefficient[dg] = coe;
    }
    #ifdef DBUGM_ShowPolynomial
        printf("Input degree: %d, divisor: %d\n%s\n", degree, Divisor, input);
        ShowPolynomial(degree, coefficient);
    #endif
}
int Induction(int dg, int polynomial[MaxDegree])
{
    return 0;
}
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
		//freopen("output.out", "w", stdout);
	#endif
    int i, j, k;
    int caseNumber = 1;
    while(1){
        for(i = 0; i < MaxSize; i++) input[i] = '\0';
        scanf("%s", &input);
        if(input[0] == '.') break;
        #ifdef DBUGM
            printf("----------- Debug Message %d-----------\n", caseNumber);
        #endif
        Read();
        int divisible = Induction(degree, coefficient);
        if(divisible) printf("Case %d: Always an integer\n", caseNumber);
        else printf("Case %d: Not always an integer\n", caseNumber);
        caseNumber++;
    }
    return 0;
}
