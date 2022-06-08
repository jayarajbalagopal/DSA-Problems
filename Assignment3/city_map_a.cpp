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
		if(i!=start){
			vector<int> path;
			int current_node = i;
			while(current_node != start){
				path.push_back(current_node);
				current_node = previous_node[current_node];
			}
			path.push_back(start);
			result.push_back(path);
		}
	}
	return result;
}

vector<vector<int>> dijkstra(int v, int s, vector<vector<int>> adj[])
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
    return path;
}

void viewpath(vector<vector<int>> path){
	for(auto node_path: path){
		for(auto node: node_path)
			cout << node << " ";
		cout << endl;
	}
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
	vector<vector<int>> path = dijkstra(n, k, adj);
	viewpath(path);
	return 0;
}