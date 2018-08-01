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

//Exec time: 0.040s

//Comments: a super easy MST problem solved with Kruskal algorithm. Solution adapted
//			from problem 908 changing the input 

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
		if (!computers.isSameSet(v[pos].c1, v[pos].c2)) {
			sol += v[pos].cost;
			computers.unionSet(v[pos].c1, v[pos].c2);
		}
		++pos;
	}
	return sol;
}

void resuelveCaso(int c) {
	int m, n;
	cin >> m >> n;
	int city = 0;
	unordered_map<string, int> biyection;

	vector<tState> v;
	for (int i = 0; i < n; ++i) {

		string n1, n2;
		cin >> n1 >> n2;
		int p1, p2;

		auto it1 = biyection.find(n1);
		if (it1 == biyection.end()) {
			biyection.insert(make_pair(n1, city));
			p1 = city;
			++city;
		}
		else {
			p1 = it1->second;
		}

		auto it2 = biyection.find(n2);
		if (it2 == biyection.end()) {
			biyection.insert(make_pair(n2, city));
			p2 = city;
			++city;
		}
		else {
			p2 = it2->second;
		}

		tState aux;
		aux.c1 = p1; aux.c2 = p2;
		cin >> aux.cost;
		v.push_back(aux);
	}

	int sol = kruskal(v, n);

	if (c != 0) cout << '\n';
	cout << sol << '\n';
}

int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; ++i) {
		resuelveCaso(i);
	}
	return 0;
}