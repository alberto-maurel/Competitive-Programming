#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

//This problem is easy to solve using a BFS. The solely point we have to take care about is how to mark the BFS
//Obviously if we reach a cell in a state where we have made more movements and we are in a longer streak of obstacles is worse than 
//other state with less movements. Is only admisible a new state with more movements and a shorter streak of obstacles (because we may
//be impossible to reach the final state in the first situation, or even have a longer path in the rest of the maze due to we cannot go 
//through any obstacle)
//Because in a BFS we process every single valid state with n movements away from the beginning before processing any state n + 1 movements
//away from the beginning, we just store as mark the smallest streak of obstacles (it's guaranteed that no shorter path will reach a point 
//once any path has reached that point)

struct tState {
	int x, y;
	int obstacles;
	int nMov;
};

int dr[] = { -1,0,1,0 };
int dc[] = { 0,1,0,-1 };

int main() {
	int nCases;
	cin >> nCases;

	int m, n, K;
	for (int T = 0; T < nCases; ++T) {
		cin >> m >> n >> K;
		vector<vector<bool>> mazeMap(m, vector<bool>(n)); //False if there is any obstacle in that cell,true if not
		vector<vector<int>> marks(m, vector<int>(n, INT_MAX)); //Stores the smallest streak of obstacles we have reached that cell with 
		bool aux;
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				cin >> aux;
				if (aux == 0) mazeMap[i][j] = true;
				else mazeMap[i][j] = false;
			}
		}

		tState actualState;
		queue<tState> bfs;
		//First state
		bfs.push({ 0,0,0,0 });
		
		while (!bfs.empty()){
			actualState = bfs.front();  bfs.pop();
			//We have reached the end
			if (actualState.x == n - 1 && actualState.y == m - 1) goto finish;
			//For each direction
			for (int k = 0; k < 4; ++k) {
				//We chech we don't fall outside the maze
				if (actualState.y + dr[k] >= 0 && actualState.y + dr[k] < mazeMap.size() && actualState.x + dc[k] >= 0 && actualState.x + dc[k] < mazeMap[0].size()) {
					
					//If there isn't an obstacle in that cell we only have to check if there hasn't been any previous state in that cell
					if (mazeMap[actualState.y + dr[k]][actualState.x + dc[k]] == true && marks[actualState.y + dr[k]][actualState.x + dc[k]] > 0) {
						bfs.push({ actualState.x + dc[k], actualState.y + dr[k], 0, actualState.nMov + 1 });
						marks[actualState.y + dr[k]][actualState.x + dc[k]] = 0;
					}

					//If there is an obstacle in that point but we already have enought turbo left
					//We also check that no other state with a smaller obstacles' streak has been in that cell before
					else if (actualState.obstacles + 1 <= K && marks[actualState.y + dr[k]][actualState.x + dc[k]] > actualState.obstacles + 1) {
						bfs.push({ actualState.x + dc[k], actualState.y + dr[k], actualState.obstacles + 1 ,actualState.nMov + 1 });
						marks[actualState.y + dr[k]][actualState.x + dc[k]] = actualState.obstacles + 1;
					}
				}
			}
		}
	finish:

		if (actualState.x == n - 1 && actualState.y == m - 1) cout << actualState.nMov;
		else cout << "-1";
		cout << '\n';
	}

	return 0;
}