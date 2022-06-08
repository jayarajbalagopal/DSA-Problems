#include <iostream>
#include <vector>

using namespace std;

void viewgrid(vector<vector<char>> grid, int n, int m){
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++)
			cout << grid[i][j] << " ";
		cout << endl;
	}
}

vector<pair<int,int>> update_rider(vector<vector<char>>& grid, vector<pair<int,int>>& riders, int n, int m){
	vector<pair<int,int>> new_rider_positions;
	for(auto pos: riders){
		int i = pos.first;
		int j = pos.second;
		if(i-1>=0){
			if(grid[i-1][j] != '#'){
				grid[i-1][j] = 'R';
				new_rider_positions.push_back({i-1,j});
			}
		}
		if(i+1<n){
			if(grid[i+1][j] != '#'){
				grid[i+1][j] = 'R';
				new_rider_positions.push_back({i+1,j});
			}
		}
		if(j-1>=0){
			if(grid[i][j-1] != '#'){
				grid[i][j-1] = 'R';
				new_rider_positions.push_back({i,j-1});
			}
		}
		if(j+1<m){
			if(grid[i][j+1] != '#'){
				grid[i][j+1] = 'R';
				new_rider_positions.push_back({i,j+1});
			}
		}
	}
	return new_rider_positions;
}

bool findpath(vector<vector<char>> grid, vector<pair<int,int>> riders, vector<vector<bool>> visited, int pathlength, int* min_path, int i, int j, int n, int m){
	if(i == 1 && j == 0){
		if(pathlength < *min_path)
			*min_path = pathlength;
		return true;
	}
	visited[i][j] = true;
	vector<vector<char>> new_grid(grid);
	vector<pair<int,int>> new_riders = update_rider(new_grid, riders, n, m);
	
	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;

	if(i-1>=0){
		if(!visited[i-1][j] && grid[i-1][j] == '.'){
			up = findpath(new_grid, new_riders, visited, pathlength+1, min_path, i-1, j, n ,m);
		}
		else{
			up = false;
		}
	}
	if(i+1<n){
		if(!visited[i+1][j] && grid[i+1][j] == '.'){
			down = findpath(new_grid, new_riders, visited, pathlength+1, min_path, i+1, j, n ,m);
		}
		else{
			down = false;
		}
	}
	if(j-1>=0){
		if(!visited[i][j-1] && grid[i][j-1] == '.'){
			left = findpath(new_grid, new_riders, visited, pathlength+1, min_path, i, j-1, n ,m);
		}
		else{
			left = false;
		}
	}
	if(j+1<m){
		if(!visited[i][j+1] && grid[i][j+1] == '.'){
			right = findpath(new_grid, new_riders, visited, pathlength+1, min_path, i, j+1, n ,m);
		}
		else{
			right = false;
		}
	}
	return (up || down || left || right);
}

int main(){
	int n,m;
	cin >> n >> m;
	vector<vector<char>> grid(n, vector<char>(m));
	vector<pair<int,int>> riders;
	int start_i, start_j;
	for(int i=0;i<n;i++){
		for(int j=0;j<m;j++){
			cin >> grid[i][j];
			if(grid[i][j] == 'R')
				riders.push_back({i,j});
			if(grid[i][j] == 'A'){
				start_i = i;
				start_j = j;
			}
		}
	}

	vector<vector<bool>> visited(n, vector<bool>(m, false));
	int min_path = INT_MAX;
	if(findpath(grid, riders,visited, 0, &min_path, start_i, start_j, n, m)){
		cout << "Yes" << endl;
		cout << min_path << endl;
	}
	else{
		cout << "No" << endl;
	}
	return 0;
}