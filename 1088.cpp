#include <cstdio>
#include <vector>
#include <algorithm>
#define IFI 1000000000
using namespace std;
typedef struct edge{
	int value;
	int id;
	edge(int a, int b): id(a), value(b){};
} edge;
typedef struct cell{
	vector<int> value;
	vector<int> queue;
	cell(int a){
		int i;
		for(i = 0; i <= a; i++) value.push_back(-1);
		queue.clear();
	};
} cell;
typedef struct{
	vector<edge> eg;
	vector<cell> dp;
} node;
node nds[105];
int size;
int globalt;

void newValue(int nid, int dpid, int maxpl, int maxnl)
{
	if( nds[nid].dp.at(dpid).value.at(maxpl + globalt) == -1){
		nds[nid].dp.at(dpid).queue.push_back(maxpl + globalt);
		nds[nid].dp.at(dpid).value.at(maxpl + globalt) = maxnl;
	}
	else {
		nds[nid].dp.at(dpid).value.at(maxpl + globalt) = min(maxnl, nds[nid].dp.at(dpid).value.at(maxpl + globalt));
	}
}
int ff(int now, int parent)
{
	int i, j, k;
	/* fill all child first */
	for(i = 1; i < nds[now].eg.size(); i++) ff(nds[now].eg.at(i).id, now);	
	
	/*for leafes*/
	if( nds[now].eg.size() == 1) {		
		newValue(now, nds[now].eg.size()-1, 0, 0);
		return 1;
	}
	
	/*fill this node's First child value*/	
	int cell = (60 - nds[now].eg.at(1).value%60)%60;
	int floor = nds[now].eg.at(1).value%60;
	int subt_v, fir_v, sec_v;
	int cid = nds[now].eg.at(1).id;
	
	for(i = 0; i < nds[cid].dp.at(nds[cid].eg.size()-1).queue.size(); i++){
		int qid = nds[cid].dp.at(nds[cid].eg.size()-1).queue.at(i);
		int qpv = qid-globalt;
		int qnv = nds[cid].dp.at(nds[cid].eg.size()-1).value.at(qid);
		
		if(cell <= globalt && cell + qpv <= globalt) {
			newValue(now, 1, max(cell + qpv, cell),  max(qnv-cell, 0));
		}
		if(floor <= globalt && qpv - floor >= -globalt && qnv + floor <= globalt ){
			newValue(now, 1, max(qpv - floor, -floor),  max(qnv + floor, floor));
		}
	}
	
	/* dp for the rest value*/
	/*merge as binary tree*/
	for(i = 2; i < nds[now].eg.size(); i++){ /*Binary tree like*/
		cell = (60 - nds[now].eg.at(i).value%60)%60;
		floor = nds[now].eg.at(i).value%60;
		cid = nds[now].eg.at(i).id;
		
		for(j = 0; j < nds[cid].dp.at(nds[cid].eg.size()-1).queue.size(); j++){ /*The possible maxpv for the subtree_i*/
			int qid = nds[cid].dp.at(nds[cid].eg.size()-1).queue.at(j);
			int qpv = qid-globalt;
			int qnv = nds[cid].dp.at(nds[cid].eg.size()-1).value.at(qid);
			
			for(k = 0; k < nds[now].dp.at(i-1).queue.size(); k++) { /*The possible maxpv for the prev i-1 subtree*/
				int qid2 = nds[now].dp.at(i-1).queue.at(k);
				int qpv2 = qid2-globalt;
				int qnv2 = nds[now].dp.at(i-1).value.at(qid2);
				
				if(cell <= globalt && cell + qpv <= globalt) { /* Deal with edge is pos*/
					int pv_i =  max(cell + qpv, cell), nv_i = max(qnv-cell, 0);
					if(pv_i + qpv2 > globalt || nv_i + qnv2 > globalt) continue;
					newValue(now, i, max(pv_i, qpv2),  max(nv_i, qnv2));
				}
				else break;
			}
			for(k = 0; k < nds[now].dp.at(i-1).queue.size(); k++) { /*The possible maxpv for the prev i-1 subtree*/
				int qid2 = nds[now].dp.at(i-1).queue.at(k);
				int qpv2 = qid2-globalt;
				int qnv2 = nds[now].dp.at(i-1).value.at(qid2);
				
				if(floor <= globalt && qpv - floor >= -globalt && qnv + floor <= globalt ){ /* Deal with edge is neg*/
					int pv_i =  max(qpv - floor, -floor), nv_i = max(qnv + floor, floor);
					if(pv_i + qpv2 > globalt || nv_i + qnv2 > globalt) continue;
					newValue(now, i, max(pv_i, qpv2),  max(nv_i, qnv2));
				}
				else break;
			}			
		}		
	}
	if( nds[now].dp.at(nds[now].eg.size()-1).queue.size() > 0) return 1;
	else return 0;
}
int solve()
{
	int i, j, k;
	/*Construct dp table*/
	for(i = 1; i <= size; i++){
		nds[i].dp.clear();
		nds[i].dp.push_back(cell(globalt*2));
		for(j = 0; j < nds[i].eg.size(); j++){
			nds[i].dp.push_back(cell(globalt*2));
		}
	}
	/*Top down dp with root = 1*/
	if(ff(1, 0) == 1) return 1;
	else return 0;
}
void removep(int now, int parent)
{
	for(int i = 1; i < nds[now].eg.size(); i++){
		if(nds[now].eg.at(i).id == parent) {
			  nds[now].eg.erase (nds[now].eg.begin()+i);
			  nds[now].eg.shrink_to_fit();
			  break;
		}
	}
	for(int i = 1; i < nds[now].eg.size(); i++) removep(nds[now].eg.at(i).id, now);	
	
}
int main()
{
	int i, j, k;
	int cn = 1;
	while(1){
		scanf("%d", &size);
		if(size == 0) break;
		for(i = 0; i <= size; i++){
			nds[i].eg.clear();
			nds[i].eg.push_back(edge(0, -1));			
		}
		for(i = 0; i < size-1; i++){
			int a, b, value;
			scanf("%d%d%d", &a, &b, &value);	
			nds[a].eg.push_back(edge(b, value));
			nds[b].eg.push_back(edge(a, value));		
		}
		/* 1 is root, remove parent edge*/
		removep(1, 0);
		/*exponential up to bound*/
		int upper = 2, lower = 1;
		globalt = upper;
		while(solve() == 0){
			lower = upper;
			upper *= 2;
			globalt = upper;
		}
		/*Binary search*/
		if(lower == 1)  lower = -1;
		while(upper - lower > 1){
			globalt = (upper + lower)/2;
			if( solve() == 1) upper = globalt;
			else lower = globalt;
		}
		printf("Case %d: %d\n", cn, upper);
		cn++;
	}
	return 0;
}