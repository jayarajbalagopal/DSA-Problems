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

string lex(string inp){
	string result;
	string concatenated_inp = inp+inp;
	int concat_len = concatenated_inp.size();
	vector<int> SA = build_suffix_array(concatenated_inp);
	int index;
	for(int i=0;i<SA.size();i++){
		if(SA[i] <= inp.size()-1 && SA[i]>=0){
			index = SA[i];
			break;
		}
	}
	for(int i=index;i<index+inp.size();i++)
		result += concatenated_inp[i];
	return result;
}

int main(){
	string inp;
	cin >> inp;
	cout << lex(inp);
	return 0;
}