#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <functional>
#include <algorithm>
#include <climits>
#include <sstream>

using namespace std;

using ii = pair<int, int>;
using vi = vector<int>;
using vii = vector<ii>;
using vvi = vector<vi>;

using ll = long long int;
using vc = vector<char>;
using vb = vector<bool>;
using ib = pair<int, bool>;
using vib = vector<ib>;

//Exec time: 0.000s

//Comments: a dijkstra problem. Notice that is not a one dimensional usual dijkstra, is a "states-dijkstra", 
//			where you have to mark a state (the time when you arrive to a floor and the last elevator you have
//			taken), because arriving to a floor that is not the desired one in less time does not mean that 
//			is the best partial result to reach the last floor (maybe you prefer to arrive 30s later but not
//			change of elevator, saving 30s)

struct tFloor {
	int fFloor; //Following floor
	int time;
	int elevator;
};

struct tState {
	int floor,
		celev, //current elevator
		time;
};

struct comp {
	bool operator()(tState const& t1, tState const& t2) {
		return t1.time < t2.time;
	}
};

void dijkstra(vector<vector<tFloor>> const& graph, vvi & sol, int nelev, int destfloor) {
	vvi mark(nelev, vi(100, INT_MAX));
	for (int i = 0; i < nelev; ++i) {
		mark[i][0] = 0;
	}

	priority_queue<tState, vector<tState>, comp> pq;

	for (int i = 0; i < nelev; ++i) {
		tState aux = { 0, i, 0 };
		pq.push(aux);
	}

	//Dijkstra
	while (!pq.empty()) {
		tState front = pq.top(); pq.pop();
		
		for (int i = 0; i < graph[front.floor].size(); ++i) {
			tState nstate;
			nstate.time = front.time + graph[front.floor][i].time;
			if (front.celev != graph[front.floor][i].elevator) nstate.time += 60;
			nstate.floor = graph[front.floor][i].fFloor;
			nstate.celev = graph[front.floor][i].elevator;

			//We mark the state
			if (nstate.time < mark[nstate.celev][nstate.floor]) {
				mark[nstate.celev][nstate.floor] = nstate.time;
				pq.push(nstate);
			}
		}
	}
	sol = mark;
}

bool resuelveCaso() {
	int n, k;
	cin >> n;
	if (!cin) return false;
	cin >> k;

	vi times(n);
	for (int i = 0; i < n; ++i) {
		cin >> times[i];
	}

	vvi elevators(n, vi());

	string lact;
	getline(cin, lact);
	for (int i = 0; i < n; ++i) {
		getline(cin, lact);
		stringstream ss(lact);
		int pact;
		while (ss >> pact) {
			elevators[i].push_back(pact);
		}
	}

	vector<vector<tFloor>> graph(100);
	//Building the graph
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < elevators[i].size() - 1; ++j) {
			graph[elevators[i][j]].push_back({ elevators[i][j + 1], (elevators[i][j + 1] - elevators[i][j]) * times[i], i});
			graph[elevators[i][j + 1]].push_back({ elevators[i][j], (elevators[i][j + 1] - elevators[i][j]) * times[i], i });
		}
	}

	//We call Dijkstra
	vvi sol;
	dijkstra(graph, sol, n, k);

	int bestsol = INT_MAX;
	for (int i = 0; i < n; ++i) {
		if (sol[i][k] < bestsol) bestsol = sol[i][k];
	}

	if (bestsol == INT_MAX) cout << "IMPOSSIBLE\n";
	else cout << bestsol << '\n';

	return true;
}

int main() {
	while (resuelveCaso()) {}
	return 0;
}