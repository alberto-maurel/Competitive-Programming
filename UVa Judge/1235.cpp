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

using iii = pair<int, ii>;
using viii = vector<iii>;

using ll = long long int;
using vc = vector<char>;
using vb = vector<bool>;
using ib = pair<int, bool>;
using vib = vector<ib>;

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

int calculateDistance(string n1, string n2) {
	int dist = 0;
	for (int i = 0; i < 4; ++i) {
		int a = n1[i] - '0';
		int b = n2[i] - '0';
		if (abs(a - b) <= 5) dist += abs(a - b);
		else dist += (10 - abs(a - b));
	}
	return dist;
}

int kruskal(vector<pair<int, pair<string, string>>> & graph, unordered_map<string, int> & biyection) {
	sort(graph.begin(), graph.end());

	ufds u(biyection.size());
	int s = 0;
	int pos = 0;

	while (u.numSets != 1) {
		if (!u.isSameSet(biyection[graph[pos].second.first], biyection[graph[pos].second.second])) {
			u.unionSet(biyection[graph[pos].second.first], biyection[graph[pos].second.second]);
			s += graph[pos].first;
		}
		++pos;
	}
	return s;
}

void resuelveCaso() {
	int n;
	cin >> n;
	vector<string> v;
	unordered_map<string, int> biyection;
	int cont = 0;
	int mdist = 40;
	bool included0000 = false;
	for (int i = 0; i < n; ++i) {
		string aux;
		cin >> aux;
		int dact = calculateDistance("0000", aux);
		if (aux == "0000") included0000 = true;
		if (dact < mdist) mdist = dact;
		if (biyection.find(aux) == biyection.end()) {
			biyection.insert(make_pair(aux, cont));
			++cont;
			v.push_back(aux);
		}
	}

	vector<pair<int, pair<string, string>>> graph;
	for (int i = 0; i < v.size(); ++i) {
		for (int j = i + 1; j < v.size(); ++j) {
			graph.push_back(make_pair(calculateDistance(v[i], v[j]), make_pair(v[i], v[j])));
		}
	}

	if(included0000) cout << kruskal(graph, biyection) << '\n';
	else  cout << mdist + kruskal(graph, biyection) << '\n';
}

int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; ++i) {
		resuelveCaso();
	}
	return 0;
}