#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using ll = long long int;

using vi = vector<ll>;
using vvi = vector<vi>;

using ii = pair<int, int>;
using vii = vector<ii>;
using vvii = vector<vii>;

struct nums {
	int nzeros,
		npos,
		nneg;
};

const int INF = 1000 * 1000 * 1000;

nums comb(nums const& n1, nums const& n2) {
	return { n1.nzeros + n2.nzeros, n1.npos + n2.npos, n1.nneg + n2.nneg };
}

struct SegmentTree {
	int size;
	vi data;

	void init(int n) {
		size = 1;
		while (size < n) size *= 2;
		data.assign(2 * size, INF); // Asignamos el elemento neutro para la operación
	}
	
	void build(vi const& profs, int x, int lx, int rx) {
		if (lx + 1 == rx) {
			if (lx < profs.size()) {
				data[x] = profs[lx];
			}
			return;
		}

		int m = (lx + rx) / 2;
		build(profs, 2 * x + 1, lx, m);
		build(profs, 2 * x + 2, m, rx);

		data[x] = min(data[2 * x + 1], data[2 * x + 2]); //Operación
	}

	void build(vi const& profs) {
		build(profs, 0, 0, size);
	}
	

	int query(int l, int r, int x, int lx, int rx) {
		if (l <= lx && rx <= r) return data[x];
		else if (rx <= l || r <= lx) return INF; //Elemento neutro
		else {
			int m = (lx + rx) / 2;
			int s1 = query(l, r, 2 * x + 1, lx, m);
			int s2 = query(l, r, 2 * x + 2, m, rx);

			return min(s1, s2);  //Operacion
		}
	}

	int query(int l, int r) {
		return query(l, r, 0, 0, size);
	}
};

void dfs(int nact, int nant, int prof, vvii const& lAdy, vi & primeraVez, vi & profNodos, vi & ordenNodos) {
	if (primeraVez[nact] == -1) primeraVez[nact] = ordenNodos.size();
	ordenNodos.push_back(nact);
	profNodos.push_back(prof);

	for (ii arista : lAdy[nact]) {
		int nsig = arista.first;

		if (nsig == nant) continue;
		
		dfs(nsig, nact, prof + 1, lAdy, primeraVez, profNodos, ordenNodos);
		ordenNodos.push_back(nact);
		profNodos.push_back(prof);
	}
}

void dfs2(int nact, int nant, vvii const& lAdy, vi & parent, vi & dist) {
	for (ii arista : lAdy[nact]) {
		int nsig = arista.first;
		int coste = arista.second;

		if (nsig == nant) continue;

		parent[nsig] = nact;
		dist[nsig] = coste;

		dfs2(nsig, nact, lAdy, parent, dist);
	}
}

bool res() {
	int n;
	cin >> n;
	if (n == 0) return false;

	vvii lAdy(n);
	for (int i = 0; i < n - 1; ++i) {
		int fin, coste;
		cin >> fin >> coste;
		lAdy[i + 1].push_back({ fin, coste });
		lAdy[fin].push_back({ i + 1, coste });
	}

	//Recorrido de euler
	vi primeraVez(n, -1);
	vi profNodos, ordenNodos;
	dfs(0, -1, 0, lAdy, primeraVez, profNodos, ordenNodos);

	SegmentTree st;
	st.init(profNodos.size());
	st.build(profNodos);

	//Sparse table
	vi parent(n, -1), dist(n, 0);
	dfs2(0, -1, lAdy, parent, dist);

	int altura = 1, altura2 = 1;
	while (altura2 < n) {
		altura2 *= 2;
		++altura;
	}

	vvi distSparseTable(altura, vi(n, 0));
	vvi parentSparseTable(altura, vi(n, -1));

	for (int h = 0; h < altura; ++h) {
		for (int i = 0; i < n; ++i) {
			//Caso base, distancia al padre
			if (h == 0) {
				distSparseTable[h][i] = dist[i];
				parentSparseTable[h][i] = parent[i];
			}

			//Caso recursivo
			else {
				int p = parentSparseTable[h - 1][i];
				if (p != -1) {
					parentSparseTable[h][i] = parentSparseTable[h - 1][p];
					distSparseTable[h][i] = distSparseTable[h - 1][i] + distSparseTable[h - 1][p];
				}
			}
		}
	}

	int q;
	cin >> q;
	for (int i = 0; i < q; ++i) {
		int a, b;
		cin >> a >> b;

		//Calcular profundidad LCA(a,b)
		int profLCA = st.query(min(primeraVez[a], primeraVez[b]), max(primeraVez[a], primeraVez[b]) + 1);

		//Calcular distancia(a, LCA(a,b)) y distancia(LCA(a,b), b)
		int profa = profNodos[primeraVez[a]];
		int profb = profNodos[primeraVez[b]];

		int ha = profa - profLCA;
		int hb = profb - profLCA;

		ll sol = 0;
		int hact = 0;
		int hact2 = 1;
		while (ha > 0) {
			if ((ha & hact2) != 0) {
				sol += distSparseTable[hact][a];

				ha -= hact2;
				a = parentSparseTable[hact][a];
			}
			hact2 *= 2;
			++hact;
		}

		hact = 0;
		hact2 = 1;
		while (hb > 0) {
			if ((hb & hact2) != 0) {
				sol += distSparseTable[hact][b];

				hb -= hact2;
				b = parentSparseTable[hact][b];
			}
			hact2 *= 2;
			++hact;
		}

		if (i != 0) cout << ' ';
		cout << sol;
	}

	cout << '\n';
	return true;
}

int main() {
	while (res()) {}
	return 0;
}