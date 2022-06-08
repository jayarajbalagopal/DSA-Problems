#include <iostream>
#include <vector>

using namespace std;

void relax_edge(int start, int end, int weight, vector<int>& min_distance, vector<int>& previous_node){
    if(min_distance[end] > min_distance[start]+weight){
        min_distance[end] = min_distance[start]+weight;
        previous_node[end] = start;
    }
    else if(min_distance[end] == min_distance[start]+weight){
    	if(start < previous_node[end])
    		previous_node[end] = start;
    }
}

int extract_min(vector<int>& min_distance,vector<bool>& visited){
    int min = INT_MAX;
    int min_node = -1;
    int v = min_distance.size();
    for(int i=0;i<v;i++){
        if(!visited[i]){
            if(min_distance[i]<min){
                min = min_distance[i];
                min_node = i;
            }
        }
    }
    return min_node;
}

vector<vector<int>> identify_shorted_path(int start, vector<int> previous_node){
	int v = previous_node.size();
	vector<vector<int>> result;
	for(int i=0;i<v;i++){
		vector<int> path;
		int current_node = i;
		while(current_node != start){
			path.push_back(current_node);
			current_node = previous_node[current_node];
		}
		path.push_back(start);
		result.push_back(path);
	}
	return result;
}

vector<pair<vector<int>, int>> dijkstra(int v, int s, vector<vector<int>> adj[])
{ 
    vector<bool> visited(v, false);
    vector<int> min_distance(v, INT_MAX);
    vector<int> previous_node(v, -1);
    
    min_distance[s] = 0;
    previous_node[s] = s;
    int current_node = s;  

    while(current_node != -1){
        visited[current_node] = true;
        for(int i=0;i<adj[current_node].size();i++)
            relax_edge(current_node, adj[current_node][i][0], adj[current_node][i][1], min_distance, previous_node);
        current_node = extract_min(min_distance, visited);
    }

    vector<vector<int>> path = identify_shorted_path(s, previous_node);
    vector<pair<vector<int>, int>> path_with_weights;
    for(int i=0;i<v;i++)
    	if(i!=s)
    		path_with_weights.push_back({path[i], min_distance[i]});
    return path_with_weights;
}

void viewpath(vector<pair<vector<int>, int>> path_with_weights){
	for(auto node_path: path_with_weights){
		cout << "(" << node_path.second << ") : ";
		for(auto node: node_path.first)
			cout << node << " ";
		cout << endl;
	}
}

bool customcompare(const pair<vector<int>, int> a, const pair<vector<int>, int> b){
	if(a.second == b.second)
		return a.first < b.first;
	else
		return a.second < b.second;
}

void deleteduplicates(vector<pair<vector<int>, int>>& path_with_weights){
	vector<pair<vector<int>, int>> result;
	for(int i=0;i<path_with_weights.size();i++){
		if(path_with_weights[i].second != path_with_weights[i-1].second)
			result.push_back(path_with_weights[i]);
	}
	path_with_weights = result;
}

int main(){
	int n, m;
	cin >> n >> m;
	vector<vector<int>> adj[n];
	for(int i=0;i<m;i++){
		int start;
		int end;
		int weight;
		cin >> start >> end >> weight;
		adj[start].push_back(vector<int>{end, weight});
		adj[end].push_back(vector<int>{start, weight});
	}
	int k;
	cin >> k;
	vector<pair<vector<int>, int>> allpairpaths;
	for(int i=0;i<n;i++){
		vector<pair<vector<int>, int>> temp = dijkstra(n, i, adj);
		allpairpaths.insert(allpairpaths.end(), temp.begin(), temp.end());
	}
	sort(allpairpaths.begin(), allpairpaths.end(), customcompare);
	deleteduplicates(allpairpaths);
	if(k>allpairpaths.size())
		cout << "k exceeds available paths" << endl;
	else{
		for(int i=0;i<k;i++){
			for(auto node: allpairpaths[i].first){
				cout << node << " ";
			}
			cout << endl;
		}
	}
	return 0;
}