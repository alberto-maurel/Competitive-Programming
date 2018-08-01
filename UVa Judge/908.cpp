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

//Comments: a super easy MST problem solved with Kruskal algorithm

//ufds implementation taken from David Pérez, Pablo Hidalgo and Mingxiao Guo
//ACM-ICPC SWERC 2017 dossier
struct ufds {
	vi p, r, n;
	int numSets;
	ufds(int N) {
		p.assign(N, 0); //Marca el conjunto al que pertenece
		r.assign(N, 0);
		n.assign(N, 1); //indica el tamaño del conjunto
		numSets = N;
		for (int i = 0; i < N; i++) p[i] = i;
	}
	int findSet(int i) { // finds and path compresses if possible
		return (p[i] == i) ? i : (p[i] = findSet(p[i]));
	}
	bool isSameSet(int i, int j) {
		return findSet(i) == findSet(j);
	}
	void unionSet(int i, int j) {
		int x = findSet(i), y = findSet(j);
		if (x != y) {
			if (r[x] > r[y]) {
				p[y] = x;
				n[x] += n[y];
			}
			else {
				p[x] = y;
				n[y] += n[x];
				if (r[x] == r[y]) r[y]++;
			} numSets--;
		}
	}
	int sizeSet(int i) { // returns size of the set to which element i belongs
		return n[findSet(i)];
	}
};

struct tState {
	int c1,
		c2,
		cost;
};

struct comp {
	bool operator()(tState const& t1, tState const& t2) {
		return t1.cost < t2.cost;
	}
} cmp;

int kruskal(vector<tState> & v, int n) {
	ufds computers(n);
	int sol = 0;

	sort(v.begin(), v.end(), cmp);
	int pos = 0;

	while (pos < v.size() && computers.numSets != 1) {
		if (!computers.isSameSet(v[pos].c1 - 1, v[pos].c2 - 1)) {
			sol += v[pos].cost;
			computers.unionSet(v[pos].c1 - 1, v[pos].c2 - 1);
		}
		++pos;
	}
	return sol;
}

bool resuelveCaso(int & c) {
	int n;
	cin >> n;
	if (!cin) return false;
	int s1 = 0, s2;
	vector<tState> v;
	for (int i = 0; i < n - 1; ++i) {
		tState aux;
		cin >> aux.c1 >> aux.c2 >> aux.cost;
		v.push_back(aux);
		s1 += aux.cost;
	}
	
	int k;
	cin >> k;
	for (int i = 0; i < k; ++i) {
		tState aux;
		cin >> aux.c1 >> aux.c2 >> aux.cost;
		v.push_back(aux);
	}

	int m;
	cin >> m;
	for (int i = 0; i < m; ++i) {
		tState aux;
		cin >> aux.c1 >> aux.c2 >> aux.cost;
	}

	s2 = kruskal(v, n);

	if (c != 0) cout << '\n';
	cout << s1 << '\n';
	cout << s2 << '\n';
	++c;
	return true;
}

int main() {
	int c = 0;
	while (resuelveCaso(c)) {}
	return 0;
}