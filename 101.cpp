#include <cstdio>
#include <vector>
using namespace std;
vector<int> v[50];
	int size;
	int ap = 0, ap2 = 0, bp = 0, bp2 = 0;
void mb(int a, int nt)
{
	//printf("m %d\n", a);
	if(nt == 0){
	for(int k = ap2+1; k < v[ap].size(); k++) {
		v[v[ap].at(k)].push_back(v[ap].at(k));
	}
	while(v[ap].size() != ap2+1) v[ap].pop_back();
		
	}
	else {
		
	for(int k = bp2+1; k < v[bp].size(); k++) {
		v[v[bp].at(k)].push_back(v[bp].at(k));
	}
	while(v[bp].size() != bp2+1) v[bp].pop_back();
	}
	return;
}
void move(int a, int b)
{
	
	for(int k = ap2; k < v[ap].size(); k++) {
		//printf("p %d %d\n", bp, v[ap].at(k));
		v[bp].push_back(v[ap].at(k));
	}
	while(v[ap].size() != ap2) v[ap].pop_back();
	return;
}
int check(int a, int b)
{
	
	for(int i = 0; i < size; i++){
		for(int  j = 0; j < v[i].size(); j++) {
			if(a == v[i].at(j)){
				ap = i, ap2 = j;
			}
			if(b == v[i].at(j)){
				bp = i, bp2 = j;
			}
		}
	}
	if(ap == bp) return 0;
	else return 1;
}
int main()
{
	freopen("in.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);
	int i, j, k;
	scanf("%d", &size);
	for(i = 0; i < size; i++) v[i].clear(), v[i].push_back(i);
	while(1){
		char s[2][100];
		int d[2];
		scanf("%s", s[0]);
		if(s[0][0] == 'q') break;
		else {
			scanf("%d %s %d", &d[0], s[1], &d[1]);
			if(check(d[0], d[1]) == 0)continue;
			
	/*printf("%s %d %s %d\n", s[0], d[0], s[1], d[1]);	
	for(i = 0; i < size; i++){
		printf("%d:", i);
		for(j = 0; j < v[i].size(); j++) {
			printf(" %d", v[i].at(j));
		}
		printf("\n");
	}*/
			if(s[0][0] == 'm') {
			//	printf("get %c\n", s[0][0]);
				mb(d[0], 0);
			}
			//printf("1\n");
			if(s[1][1] == 'n') {
				mb(d[1], 1);
			}
			//printf("2\n");
			move(d[0], d[1]);
			//printf("3\n");
		}
	}
	for(i = 0; i < size; i++){
		printf("%d:", i);
		for(j = 0; j < v[i].size(); j++) {
			printf(" %d", v[i].at(j));
		}
		printf("\n");
	}
	return 0;
}
