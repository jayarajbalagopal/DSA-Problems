// Find count of distinct elements in every sub-array of size k using unordered_map

#include <iostream>
#include <vector>
#include "unordered_map.cpp"

using namespace std;

vector<int> solve(vector<int> input, int n, int k){
	if(k>n)
		return vector<int>{-1};
	vector<int> result;
	unordered_map<int, int> count;
	int distinct = 0;
	for(int i=0;i<k;i++){
		if(count[input[i]] == 0){
			count.insert(input[i], 1);
			distinct++;
		}
		else{
			count.insert(input[i], count[input[i]]+1);
		}
	}
	result.push_back(distinct);
	for(int i=k;i<n;i++){
		count.insert(input[i-k], count[input[i-k]]-1);
		if(count[input[i-k]] == 0)
			distinct--;
		if(count[input[i]] == 0 ){
			count.insert(input[i], 1);
			distinct++;
		}
		else{
			count.insert(input[i], count[input[i]]+1);
		}
		result.push_back(distinct);
	}
	return result;
}

int main(){
	int n, k;
	cin >> n >> k;
	vector<int> input(n);
	for(int i=0;i<n;i++)
		cin >> input[i];
	vector<int> res = solve(input, n, k);
	for(auto r: res)
		cout << r << " ";
	cout << endl;
	return 0;
}
