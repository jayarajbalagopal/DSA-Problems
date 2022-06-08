#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct suffix{
	int index;
	int place_value[2];
	int rank=-1;
};

bool comparator(const suffix &a, const suffix &b){
	if(a.place_value[0] == b.place_value[0])
		return (a.place_value[1]<b.place_value[1]);
	else
		return (a.place_value[0]<b.place_value[0]);
}

void view_suffixes(vector<suffix> suffix_ranking){
	for(int i=0;i<suffix_ranking.size();i++){
		cout << suffix_ranking[i].index << " " << suffix_ranking[i].place_value[0] << " " << suffix_ranking[i].place_value[1] << " : " << suffix_ranking[i].rank << endl; 
	}
}

vector<int> build_suffix_array(string inp){
	inp = inp;
	int n = inp.size();
	vector<suffix> suffix_ranking(n);
	for(int i=0;i<n;i++){
		suffix_ranking[i].index = i;
		suffix_ranking[i].place_value[0] = inp[i];
		suffix_ranking[i].place_value[1] = (i+1)<n?inp[i+1]:-1;
	}
	sort(suffix_ranking.begin(), suffix_ranking.end(), comparator);
	int rank = 0;
	vector<int> current_index_ranking(n);
	for(int i=0;i<n;i++){
		if(i==0){
			suffix_ranking[i].rank=0;
		}
		else if((suffix_ranking[i].place_value[0] == suffix_ranking[i-1].place_value[0]) && (suffix_ranking[i].place_value[1] == suffix_ranking[i-1].place_value[1])){
			suffix_ranking[i].rank = rank;
		}
		else{
			rank++;
			suffix_ranking[i].rank = rank;
		}
		current_index_ranking[suffix_ranking[i].index] = suffix_ranking[i].rank;
	}
	for(int k=2;k<n;k*=2){
		for(int i=0;i<n;i++){
			int next = suffix_ranking[i].index+k;
			suffix_ranking[i].place_value[0] = suffix_ranking[i].rank;
			suffix_ranking[i].place_value[1] = (next<n)?current_index_ranking[next]:-1;
		}
		sort(suffix_ranking.begin(), suffix_ranking.end(), comparator);
		rank = 0;
		for(int i=0;i<n;i++){
			if(i==0){
				suffix_ranking[i].rank=0;
			}
			else if((suffix_ranking[i].place_value[0] == suffix_ranking[i-1].place_value[0]) && (suffix_ranking[i].place_value[1] == suffix_ranking[i-1].place_value[1])){
				suffix_ranking[i].rank = rank;
			}
			else{
				rank++;
				suffix_ranking[i].rank = rank;
			}
			current_index_ranking[suffix_ranking[i].index] = suffix_ranking[i].rank;
		}
	}
	vector<int> result;
	for(int i=0;i<n;i++)
		result.push_back(suffix_ranking[i].index);
	return result;
}

vector<int> kasai(vector<int> SA, string inp){
	int n = SA.size();
	vector<int> lcp(n);
	
	vector<int> index_to_rank_map(n);
	for(int i=0;i<n;i++){
		index_to_rank_map[SA[i]] = i;
	}

	int ml = 0;
	for(int i=0;i<n;i++){
		if(index_to_rank_map[i]>0){
			int prev_suffix = SA[index_to_rank_map[i]-1];
			while(inp[i+ml] == inp[prev_suffix+ml]) ml++;
			lcp[index_to_rank_map[i]] = ml;
			if(ml>0)ml--;
		}
	}
	return lcp;
}

int lcp_range_minimum(int i, int j, vector<int> lcp){
	int m = INT_MAX;
	int start = min(i,j);
	int end = max(i,j);
	for(int k=start+1;k<=end;k++)
		m = min(lcp[k],m);
	return m;
}

string solve(string inp){
	string rev = inp;
	int n = inp.size();
	reverse(rev.begin(), rev.end());
	string new_string = inp + "$" + rev;
	int m = new_string.size();
	vector<int> SA = build_suffix_array(new_string);
	vector<int> lcp = kasai(SA, new_string);

	int res = 0;
	int index = -1;
	for(int i=1;i<m;i++){
		if((SA[i-1] < n && SA[i] > n) || (SA[i-1] > n && SA[i] < n)){
			if(lcp[i] > res){
				res = lcp[i];
				index = min(SA[i], SA[i-1]);
			}
		}
	}

	string res_s;
	for(int i=index;i<index+res;i++)
		res_s += inp[i];
	return res_s;
}

int main(){
	string inp;
	cin >> inp;
	cout << solve(inp);
	return 0;
}