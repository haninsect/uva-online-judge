#include <cstdio>

int main()
{
    //freopen("in.in", "r", stdin);
    //freopen("out.out", "w", stdout);
    while(1){
        long long int b, p, m;
        if(scanf("%lld%lld%lld", &b, &p, &m) == EOF) break;
        if(p == 0){
            printf("%lld\n", 1%m);
            continue;
        }
        if(b == 0){
            printf("0\n");
            continue;
        }
        long long int pow = b%m;
        long long int ct = 1;
        long long int table[50] = {0};
        table[1] = pow;
        long long int i = 1;
        //printf("%lld %lld %lld\n", b, p, m);
        while(ct*2 < p){
            ct *= 2;
            pow *= pow;
            if(pow >= m) pow = pow%m;
            i++;
            table[i] = pow;
            //printf("%lld: %lld\n", ct, table[i]);
        }
        long long int now = ct;
        while(now != p){
            ct /= 2;
            i--;
            if( now + ct <= p) now += ct, pow *= table[i];
            if( pow >= m ) pow = pow%m;
        }
        printf("%lld\n", pow);
    }
    return 0;
}
