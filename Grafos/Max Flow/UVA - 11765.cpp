#include <iostream> 
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>
#include <stack>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>
#include <iomanip>
#include <cctype>
#include <climits>

using namespace std;

using vi = vector<int>;
using vvi = vector<vi>;

using ii = pair<int, int>;
using vii = vector<ii>;
using vvii = vector<vii>;

using vb = vector<bool>;

using ll = long long int;

#define mp(a,b) make_pair(a,b)

const int MOD = 1000 * 1000 * 1000 + 7;
const int INF = 1000 * 1000 * 1000;

// Push Relabel with highest label selection rule
// O(V^2 sqrt(E))
typedef ll Flow;
struct Edge {
	int dest, back;
	Flow f, c;
};
struct PushRelabel {
	vector<vector<Edge>> g;
	vector<Flow> ec;
	vector<Edge*> cur;
	vector<vi> hs; vi H;
	PushRelabel(int n) : g(n), ec(n), cur(n), hs(2 * n), H(n) {}
	void add_edge(int s, int t, Flow cap, Flow rcap = 0) {
		if (s == t) return;
		Edge a = { t, g[t].size(), 0, cap };
		Edge b = { s, g[s].size(), 0, rcap };
		g[s].push_back(a);
		g[t].push_back(b);
	}
	void add_flow(Edge& e, Flow f) {
		Edge& back = g[e.dest][e.back];
		if (!ec[e.dest] && f) hs[H[e.dest]].push_back(e.dest);
		e.f += f; e.c -= f; ec[e.dest] += f;
		back.f -= f; back.c += f; ec[back.dest] -= f;
	}
	Flow maxflow(int s, int t) {
		int v = g.size(); H[s] = v; ec[t] = 1;
		vi co(2 * v); co[0] = v - 1;
		for (int i = 0; i < v; ++i) cur[i] = g[i].data();
		for(auto & e: g[s]) add_flow(e, e.c);
		for (int hi = 0;;) {
			while (hs[hi].empty()) if (!hi--) return -ec[s];
			int u = hs[hi].back(); hs[hi].pop_back();
			while (ec[u] > 0) // discharge u
				if (cur[u] == g[u].data() + g[u].size()) {
					H[u] = 1e9;
					for(auto & e: g[u]) if (e.c && H[u] > H[e.dest] + 1)
						H[u] = H[e.dest] + 1, cur[u] = &e;
					if (++co[H[u]], !--co[hi] && hi < v)
						for(int i = 0; i < v; ++i) if (hi < H[i] && H[i] < v)
						--co[H[i]], H[i] = v + 1;
					hi = H[u];
				}
				else if (cur[u]->c && H[u] == H[cur[u]->dest] + 1)
					add_flow(*cur[u], min(ec[u], cur[u]->c));
				else ++cur[u];
		}
	}
};

void res() {
	int n, m;
	cin >> n >> m;

	PushRelabel grafo(n + 2);

	vi c1(n), c2(n);
	for (int& x : c1) cin >> x;
	for (int& x : c2) cin >> x;

	for (int i = 0; i < n; ++i) {
		int l;
		cin >> l;

		if (l == 1) {
			grafo.add_edge(n, i, c1[i]);
			grafo.add_edge(i, n + 1, INF);
		}
		else if (l == -1) {
			grafo.add_edge(n, i, INF);
			grafo.add_edge(i, n + 1, c2[i]);
		}
		else {
			grafo.add_edge(n, i, c1[i]);
			grafo.add_edge(i, n + 1, c2[i]);
		}
	}

	for (int i = 0; i < m; ++i) {
		int p, q, r;
		cin >> p >> q >> r;
		--p; --q;
		grafo.add_edge(p, q, r);
		grafo.add_edge(q, p, r);
	}

	cout << grafo.maxflow(n, n+1) << '\n';
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int t;
	cin >> t;
	for (int nc = 0; nc < t; ++nc) 
		res();

	return 0;
}