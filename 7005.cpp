#include <cstdio>
typedef struct {
	double value;
	double colSum;
} cell;
cell table[601][10001];
int row, col, size;
double solve(int upr, int lowr)
{	
	int i, j;
	double upper = 5005, lower = 0;
	while(upper - lower > 0.0001){
		double mid = (upper + lower)/2;
		double array[col];
		for(i = 0; i < col; i++) {
			array[i] = table[lowr][i].colSum - table[upr][i].colSum + table[upr][i].value - mid*(lowr - upr + 1);
			if(i != 0) array[i] += array[i-1];
		}
		int st = size/(lowr - upr + 1);
		if(size == 1) { if( (lowr - upr + 1) == 1) st = st-1; }
		else { if(size%(lowr - upr + 1) == 0 ) st = st-1; }
		double min = 0;
		int found = 0;
		for(i = st; i < col; i++) {
			if( array[i] - min >=0) {
				found = 1;
				break;
			}
			if( array[i - st] < min) min = array[i - st];
		}
		if(found == 1) lower = mid;
		else upper = mid;		
	}
	return upper;	
}
int main()
{
	int i, j, k;
	int cn;
	scanf("%d", &cn);
	while(cn--){
		scanf("%d%d%d", &row, &col, &size);
		for(i = 0; i < row; i++){
			for(j = 0; j < col; j++){
				scanf("%lf", &table[i][j].value);
				if(i == 0) table[i][j].colSum = table[i][j].value;
				else table[i][j].colSum = table[i][j].value + table[i-1][j].colSum;
			}
		}
		double ans = 0;
		for(i = 0; i < row; i++){
			for(j = i; j < row; j++){
				double tmp = solve(i, j);
				if(tmp > ans) ans = tmp;
			}
		}
		printf("%.3f\n", ans);
	}
	return 0;
}