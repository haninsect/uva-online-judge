/*1069 - Always an integer _Induction */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#define DBUGM1
#define DBUGM_Induction1
#define DBUGM_ShowPolynomial1
#define LargeINT 1000000000
#define errorT 0.000001
#define M_PI 3.14159265358979323846
#define MaxSize 100*100
#define MaxDegree 105
char input[MaxSize];
long long int coefficient[MaxDegree];
long long int Divisor;
int degree;
void ShowPolynomial(int dg, long long int polynomial[MaxDegree])
{
    int i;
    printf("%d:(", dg);
    if(dg > 1){
        if(polynomial[dg] == 1) printf("n^%d", dg);
        else if(polynomial[dg] == -1) printf("-n^%d", dg);
        else printf("%lldn^%d", polynomial[dg], dg);
    }
    for(i = dg-1; i >= 2; i--){
        if(polynomial[i] == 0) continue;
        if (polynomial[i] > 0) printf("+");
        if(polynomial[i] == 1) printf("n^%d", i);
        else if(polynomial[i] == -1) printf("-n^%d", i);
        else printf("%lldn^%d", polynomial[i], i);
    }
    if(polynomial[1] != 0){
        if(dg != 1){
            if (polynomial[1] > 0 ) printf("+");
        }
        if(polynomial[1] == 1) printf("n");
        else if(polynomial[1] == -1) printf("-n");
        else printf("%lldn", polynomial[1]);
    }
    if(polynomial[0] != 0){
        if(dg != 0){
            if (polynomial[0] > 0 ) printf("+");
        }
        printf("%lld", polynomial[0]);
    }
    printf(")\n");
}
long long int GetDecimal(int *nowPointer)
{
    #ifdef DBUGM
        printf("GetDecimal: input[%d]: %c\n", *nowPointer, input[*nowPointer] );
    #endif
    int i;
    long long int sign = 1;
    if(!isdigit(input[*nowPointer])){
         if(input[*nowPointer] == 'n') return 1;
         if(input[*nowPointer] == ')') return 0;
         if(input[*nowPointer] == '-') sign = -1;
         (*nowPointer)++;
    }
    if(input[*nowPointer] == 'n') return 1*sign;

    long long int r = 0;
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
        long long int coe = GetDecimal(&np);
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
void MultiplePolynomail(int *dg1, long long int p1[MaxDegree], int dg2, long long int p2[MaxDegree])
{
    #ifdef DBUGM_ShowPolynomial
        printf("--- Multiple ---\n");
        ShowPolynomial(*dg1, p1);
        ShowPolynomial(dg2, p2);
    #endif
    int i, j;
    long long int tmp[MaxDegree] = {0};
    for(i = 0; i <= *dg1; i++){
        for(j = 0; j <= dg2; j++){
            tmp[i+j] = tmp[i+j]%Divisor + (p1[i]*p2[j])%Divisor;
        }
    }
    for(i = 0; i <= *dg1+dg2; i++){
        if(tmp[i] != 0 && i >*dg1) *dg1 = i;
        p1[i] = tmp[i]%Divisor;
    }
    #ifdef DBUGM_ShowPolynomial
        printf("- Done -\n");
        ShowPolynomial(*dg1, p1);
    #endif
}
int Induction(int dg, long long int polynomial[MaxDegree])
{
    #ifdef DBUGM_Induction
        printf("--- Start Induction %d %lld---\n", dg, Divisor);
        ShowPolynomial(dg, polynomial);
    #endif
    int i, j;
    /*Basis: n = 1*/
    long long int fx = 0;
    for(i = 0; i <= dg; i++){
        polynomial[i] = polynomial[i]%Divisor; /*In case of overflow, all coefficient mod divisor*/
        fx = fx%Divisor + polynomial[i];
    }
    if(fx % Divisor != 0) return 0;
    if(dg == 0 && polynomial[0] == 0) return 1;
    #ifdef DBUGM_Induction
        printf("f(1): %lld\n", fx);
    #endif

    /*For degree 0~dg*/
        /*Assume n = k*/
        /*Imply n = k +1*/
    long long int k1Polynomail[MaxDegree] = {0};
    int k1dg = 0;
    k1Polynomail[0] = 1;
    long long int result[MaxDegree] = {0};
    int rtdg = 0;
    long long int xp1[MaxDegree] = {0};
    xp1[0] = 1;
    xp1[1] = 1;
    for(i = 1; i <= dg; i++){
        MultiplePolynomail(&k1dg, k1Polynomail, 1, xp1);
        /*(k+1)^i - (k)*/
        for(j = 0; j < k1dg; j++){
            if(polynomial[i] == 0) break;
            result[j] = result[j]%Divisor + k1Polynomail[j]*polynomial[i];
        }
        #ifdef DBUGM_ShowPolynomial
            printf("- Tmp Result: (x+1)^%d/%d\n", i, dg);
            ShowPolynomial(k1dg, result);
        #endif
    }

    int flag = 1;
    for(i = 0; i < dg; i++){
        if(result[i] != 0 && i > rtdg) rtdg = i;
        result[i] = result[i]%Divisor;
        if(result[i] != 0) flag = 0;
    }
    #ifdef DBUGM_Induction
        printf("--- result %d ---\n", rtdg);
        ShowPolynomial(rtdg, result);
    #endif
    if(flag == 1) return 1;
    else Induction(rtdg, result);

}
int main()
{
    #ifndef ONLINE_JUDGE
		freopen("input.in", "r", stdin);
        freopen("output.out", "w", stdout);
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
