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

//Exec time: 0.030s

//Comments: BFS problem. 

struct tState {
	int n,
		TTL;
};

bool resuelveCaso(int & nc) {
	int n;
	cin >> n;
	if (n == 0) return false;

	unordered_map<int, int> bijection;

	vvi graph(40, vi());
	int nact = 0;

	for (int i = 0; i < n; ++i) {
		int p1, p2, c1, c2;
		cin >> p1 >> p2;

		auto it1 = bijection.find(p1);
		if (it1 == bijection.end()) {
			bijection.insert(make_pair(p1, nact));
			c1 = nact;
			++nact;
		}
		else {
			c1 = it1->second;
		}

		auto it2 = bijection.find(p2);
		if (it2 == bijection.end()) {
			bijection.insert(make_pair(p2, nact));
			c2 = nact;
			++nact;
		}
		else {
			c2 = it2->second;
		}

		graph[c1].push_back(c2);
		graph[c2].push_back(c1);
	}

	int ini, TTLini;
	int nnodes = bijection.size();

	cin >> ini >> TTLini;
	while (ini != 0 || TTLini != 0) {
		vector<bool> mark(40, false);
		queue<tState> q;

		q.push({ bijection[ini] , TTLini });
		mark[bijection[ini]] = true;

		while (!q.empty()) {
			tState front = q.front();
			q.pop();
			--nnodes;
			
			if (front.TTL > 0) {
				for (int i = 0; i < graph[front.n].size(); ++i) {
					if (!mark[graph[front.n][i]]) {
						q.push({ graph[front.n][i], front.TTL - 1 });
						mark[graph[front.n][i]] = true;
					}
				}
			}
		}
		cout << "Case " << nc << ": " << nnodes << " nodes not reachable from node " << ini << " with TTL = " << TTLini << ".\n";
		++nc;
		nnodes = bijection.size();
		cin >> ini >> TTLini;
	}

	return true;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	char buffer[1000000];
	cout.rdbuf()->pubsetbuf(buffer, 1000000);
	int nc = 1;
	while (resuelveCaso(nc)) {}
	return 0;
}