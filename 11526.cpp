#include <cstdio>

long long H(long long n){
    long long res = 0;
    for( long long i = 1; i <= n; i=i+1 ){
        //printf("%d\n", i);
        if(n/i == n/(i+1)){
            if(((n/i)-1) == 0){
                res += (n/i)*(n-i+1);
                i = n;
            }
            else {
                res += ((n/(n/i))-i+1) * (n/i);
                i = (n/(n/i));
            }
        }
        else res = (res + n/i);
    }
    return res;
}
int main()
{
    //freopen("in.in", "r", stdin);
    //freopen("out.out", "w", stdout);
    int cn;
    scanf("%d", &cn);
    while(cn--){
        long long n;
        scanf("%lld", &n);
        printf("%lld\n", H(n));
    }
    return 0;
}
