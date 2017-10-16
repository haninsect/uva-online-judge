#include <cstdio>


long long int GCD(int a, int b)
{
    //printf("%d %d\n", a, b);
    if(a > b) return GCD(b, a);
    else {
        if(b%a == 0) return a;
        else return GCD(b%a, a);
    }
}
int main()
{
    while(1){
        int size;
        scanf("%d", &size);
        if(size == 0) break;
        long long int ans = 0;
        for(int i = 1; i < size; i++){
            for(int j = i+1; j <= size; j++){
                ans += GCD(i, j);
            }
        }
        printf("%lld\n", ans);
    }
    return 0;
}
