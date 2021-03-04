#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;

using vi = vector<int>;
using vvi = vector<vi>;

using ii = pair<int, int>;
using vii = vector<ii>;
using vvii = vector<vii>;

using ll = long long int;
using vll = vector<ll>;

struct rango{
	int profmin;
	int	idxmin;
};

const int INF = 1000 * 1000 * 1000;
const rango NEUTRO = {INF, -1};

struct SegmentTree {
	int size;
	vector<rango> data; 
	void init(int n) {
		size = 1;
		while (size < n) size <<= 1;
		data.assign(2*size, NEUTRO);
	}
	
	void build(vi const& idxnodos, vi const& profs, int x, int lx, int rx) {
		if (lx + 1 == rx) {
			if (lx < idxnodos.size()) {
				data[x].profmin = profs[lx];
				data[x].idxmin = idxnodos[lx];
			}
		} 
		else {
			int m = (lx + rx) / 2;
			build(idxnodos, profs, 2*x + 1, lx, m);
			build(idxnodos, profs, 2*x + 2, m, rx);
		
			if (data[2 * x + 1].profmin < data[2 * x + 2].profmin) data[x] = data[2 * x + 1];
			else data[x] = data[2 * x + 2];
		}
	}
	
	void build(vi const& idxnodos, vi const& profs) {
		build(idxnodos, profs, 0, 0, size);
	}
	
	rango query(int l, int r, int x, int lx, int rx) {
		//El rango actual est contenido en el rangopreguntado
		if (l <= lx && rx <= r) return data[x];
		//El rango actual est totalmente fuera
		else if (rx <= l || r <= lx) return NEUTRO;
		//El rango est contenido a medias
		else {
			int m = (lx + rx) / 2;
			rango s1 = query(l, r, 2*x + 1, lx, m);
			rango s2 = query(l, r, 2*x + 2, m, rx);
			
			if (s1.profmin < s2.profmin) return s1;
			else return s2;
		}
	}
	
	int query(int l, int r) {
		return query(l, r, 0, 0, size).idxmin;
	}
};

void dfs(int nact, int nant, int profact, vvii const& lady, vi& recorrido, vi& prof, vi & prim) {
	prof.push_back(profact);
	recorrido.push_back(nact);
	prim[nact] = recorrido.size() - 1;

	for (ii arista : lady[nact]) {
		int nsig = arista.first;
		if (nsig == nant) continue;
		
		dfs(nsig, nact, profact + 1, lady, recorrido, prof, prim);
		recorrido.push_back(nact);
		prof.push_back(profact);
	}
}

void dfs_dist(int nact, int nant, ll dact, vvii const& lady, vll& dist) {
	dist[nact] = dact;

	for (ii arista : lady[nact]) {
		int nsig = arista.first;
		int coste = arista.second;

		if (nsig == nant) continue;

		dfs_dist(nsig, nact, dact + coste, lady, dist);
	}
}

bool res() {
	int n;
	cin >> n;
	if (n == 0) return false;

	vvii lady(n);
	for (int i = 0; i < n - 1; ++i) {
		int fin, coste;
		cin >> fin >> coste;

		lady[i + 1].push_back({ fin , coste });
		lady[fin].push_back({ i + 1 , coste });
	}

	vi prof, recorrido, prim(n);
	dfs(0, -1, 0, lady, recorrido, prof, prim);
	SegmentTree st;
	st.init(recorrido.size());
	st.build(recorrido, prof);

	vll dist(n);
	dfs_dist(0, -1, 0, lady, dist);

	int q;
	cin >> q;
	for (int z = 0; z < q; ++z) {
		int ini, fin;
		cin >> ini >> fin;

		int lca = st.query(min(prim[ini],prim[fin]), max(prim[ini], prim[fin]));
		if (z != 0) cout << ' ';
		cout << dist[ini] + dist[fin] - 2 * dist[lca];
		assert(dist[ini] + dist[fin] - 2 * dist[lca] >= 0);
	}
	cout << '\n';

	return true;
}

int main() {
	while (res()) {}
	return 0;
}