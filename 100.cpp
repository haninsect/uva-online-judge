#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
int solve(long long int a)
{
	if(a == 1) return 1;
	else {
		if(a%2 == 0) return solve(a/2)+1;
		else return solve(a*3+1)+1;
	}
}
int main()
{
	int i, j, k;
	int a, b;
	while(scanf("%d%d", &a, &b) != EOF){
		int ans = 0;
		for(i = min(a, b); i <= max(a, b); i++) {
			int t = solve(i);
			if(t > ans) ans = t;
		}
		printf("%d %d %d\n", a, b, ans);
	}
	return 0;
}
