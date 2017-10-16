#include <cstdio>
#include <cstring>
#include <vector>

int prime[100005] = {0};
std::vector<int> ps;
int main()
{
    //freopen("in.in", "r", stdin);
    //freopen("out.out", "w", stdout);
    int i, j, k;
    prime[1] = 0, prime[0] = 0;
    for(i = 2; i <= 100000; i++){
        if(prime[i] == 0){
            prime[i] = 1;
            for(j = i; j <= 100000; j++){
                //printf("%d %d\n", i, j);
                if((long long int)i*(long long int)j > 100000) break;
                prime[i*j] = 2;
            }
        }
    }
    for(i = 100000; i >=2; i--){
        if(prime[i] == 1) ps.push_back(i);
    }
    while(1){
        char buffer[300] = "\0";
        scanf("%s", buffer);
        if(buffer[0] == '0' && strlen(buffer) == 1) break;
        //printf("%d\n", ps.size());
        for(i = 0; i < ps.size(); i++){
            int tmp = ps.at(i);
            int ff1 = 0;
            for(j = 0; j < strlen(buffer); j++){
                int bf = 0, ff = 1;
                char ck[10] = {'\0'};
                sprintf(ck, "%d", tmp);
                for(k = 0; k < strlen(ck); k++){
                    if(j+k >= strlen(buffer)) {
                        bf = 1;
                        break;
                    }
                    if(buffer[j+k] != ck[k]) {
                        ff = 0;
                        break;
                    }
                }
                if(bf == 1) break;
                if(ff == 1) {
                    printf("%d\n", tmp);
                    ff1 = 1;
                    break;
                }
            }
            if(ff1 == 1) break;
        }
    }
    return 0;
}
