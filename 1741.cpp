#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstdlib>
using namespace std;
typedef struct {
    vector<int> child;
    long long int weight;
    int countdown;
    long long int minchild;
    int parent;
    int ans;
    char name[20];
} node;
node nds[500010*2];
typedef struct river{
    long long int len;
    int id;
    river(long long int a, int b){
        len = a;
        id = b;
    }
} river;
vector<river> rvs;
struct state
{
    int id;
    long long int value;
    state(int a, long long int b): id(a), value(b){};
    bool operator < (const state& rhs) const
    {
        return value >= rhs.value;
    }
};
priority_queue<state> q;
int ns;

void BFS()
{

    vector<int> tq;
    vector<long long int> tlen;
    tq.push_back(0);
    tlen.push_back(0);
    int pt = 0;
    while(pt < tq.size()){
        int now = tq[pt];
        if(nds[now].countdown == 0){
            rvs.push_back(river(tlen[pt], now));
        }
        else {
            for(int i = 0; i < nds[now].child.size(); i++){
                tq.push_back(nds[now].child[i]);
                tlen.push_back(tlen[pt]+nds[nds[now].child[i]].weight);
            }
        }
        pt++;
    }
}
bool cmp(const river &a, const river &b)
{
    return a.len < b.len;
}
int main()
{
    int i, j, k;
    while(1){
        int a, b;
        if(scanf("%d%d", &a, &b) == EOF) break;
        ns = a+b+1;
        rvs.clear();
        q = priority_queue <state>();
        for(i = 0; i < ns; i++) nds[i].child.clear(), nds[i].parent = -1, nds[i].countdown = 0, nds[i].minchild = (long long int)1000000000*(long long int)1000000000;
        for(i = 1; i <= a; i++){
            int to;
            scanf("%s%d%lld", nds[b+i].name, &to, &nds[b+i].weight);
            nds[to].child.push_back(b+i);
            nds[to].countdown++;
            nds[b+i].parent = to;
        }
        for(i = 1; i <= b; i++){
            int to;
            scanf("%d%lld", &to, &nds[i].weight);
            nds[to].child.push_back(i);
            nds[to].countdown++;
            nds[i].parent = to;
            nds[i].name[0] = 'i';
            nds[i].name[1] = '\0';
        }

        /*BFS*/
        BFS();
        /*L1~Ln*/
        sort(rvs.begin(), rvs.end(), cmp);
        for(i = b+1; i < ns; i++){
            q.push(state(i, nds[i].weight));

        }
        for(i = 0; i < rvs.size(); i++){
            while(q.size() > 0){
                if(q.top().value > rvs[i].len) break;
                int nid = q.top().id;
                if(q.top().value < nds[nds[nid].parent].minchild )nds[nds[nid].parent].minchild = q.top().value;
                nds[nds[nid].parent].countdown--;
                q.pop();

                if(nds[nds[nid].parent].countdown == 0){
                    if(nds[nid].parent == 0) break;
                    q.push(state(nds[nid].parent, nds[nds[nid].parent].weight + nds[nds[nid].parent].minchild));
                }
            }
            nds[rvs[i].id].ans = q.size();
        }
        for( i = b+1; i < ns; i++) printf("%s %d\n", nds[i].name, nds[i].ans+1);

    }
    return 0;
}
