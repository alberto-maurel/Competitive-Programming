#include <iostream>
#include <algorithm>
#include <cstdio>
#include <vector>
//#include <cstring>
#include <string>

using namespace std;

using vi = vector<int>;

typedef pair<int, int> ii;

string T; int n;  // input string and length

void countingSort(int k, vector<int> & RA, vector<int> & SA) { // O(n)
	vector<int> tempSA(n);

	int i, sum, maxi = max(300, n); // up to 255 ASCII chars or length of n
	vector<int> c(maxi, 0);
	//memset(c, 0, sizeof c);
	for (i = 0; i < n; i++)
		c[i + k < n ? RA[i + k] : 0]++;

	for (i = sum = 0; i < maxi; i++) {
		int t = c[i]; c[i] = sum; sum += t;
	}

	for (i = 0; i < n; i++)
		tempSA[c[SA[i] + k < n ? RA[SA[i] + k] : 0]++] = SA[i];

	for (i = 0; i < n; i++)
		SA[i] = tempSA[i];
}

// Builds Suffix Array for string T. T should end with sth like $

void constructSA(vector<int> & RA, vector<int> & SA) { // O(n log n), can go up to 100000 characters
	vector<int> tempRA(n);
	
	int i, k, r;
	for (i = 0; i < n; i++) RA[i] = T[i];
	for (i = 0; i < n; i++) SA[i] = i;
	for (k = 1; k < n; k <<= 1) {
		countingSort(k, RA, SA);
		countingSort(0, RA, SA);
		tempRA[SA[0]] = r = 0;

		for (i = 1; i < n; i++)
			tempRA[SA[i]] = (RA[SA[i]] == RA[SA[i - 1]] && RA[SA[i] + k] == RA[SA[i - 1] + k]) ? r : ++r;
		for (i = 0; i < n; i++)
			RA[i] = tempRA[i];
		if (RA[SA[n - 1]] == n - 1)
			break; // optimization
	}
}

// Longest Common Prefix: Para cada i, halla lalongitud del prefijo mas largo
// que el sufijo i de nuestra palabra comparte conalgun otro sufijo, y lo guarda en LCP[i]
// Para hacerlo en O(n), usa el orden inicial de los sufijos, no el del suffix array,
// y de ahi surgen los arrays auxiliares Phi y PLCP

vi computeLCP(vector<int> const& SA) { // O(n)
	vector<int> Phi(n), PLCP(n), LCP(n);
	int i, L;
	Phi[SA[0]] = -1;
	for (i = 1; i < n; i++)
		Phi[SA[i]] = SA[i - 1];
	for (i = L = 0; i < n; i++) {
		if (Phi[i] == -1) {
			PLCP[i] = 0;
			continue;
		}

		while (T[i + L] == T[Phi[i] + L])
			L++;

		PLCP[i] = L;
		L = max(L - 1, 0);
	}

	for (i = 0; i < n; i++)
		LCP[i] = PLCP[SA[i]];

	return LCP;
}


/*
// Longest Repeated Substring: por la definicion de LCP, es el valor maximo de LC
ii LRS() { // O(n), returns a pair (the LRS length and its index in the SA)
	int i, idx = 0, maxLCP = -1;
	for (i = 1; i < n; i++)
		if (LCP[i] > maxLCP)
			maxLCP = LCP[i], idx = i;
	return ii(maxLCP, idx);
}

char P[MAX_N]; //pattern
int m; //length of pattern// Compara T a partir del indice id con P
int comp(int id) {
	for (int i = 0; i < m; ++i) {
		if (id + i >= n || T[id + i] < P[i]) return -1; //P mayor
		if (T[id + i] > P[i]) return 1; // P menor
	}
	return 0;
}

// Devuelve dos extremos (l, u): todos los S[i], i en [l, u]
// son indices donde aparece el patron P
ii stringMatching() { // O(m log n)
	int lo = 0, hi = n - 1, mid = lo;
	while (lo < hi) { // binary search lower bound
		mid = (lo + hi) / 2;
		int res = strncmp(T + SA[mid], P, m);
		// int res = comp(SA[mid]) if working with strings
		if (res >= 0) hi = mid;
		else          lo = mid + 1;
	}

	if (strncmp(T + SA[lo], P, m) != 0) return ii(-1, -1); // if not found
	ii ans; ans.first = lo;lo = 0; hi = n - 1; mid = lo;
	while (lo < hi) { // if lower bound is found, binarysearch upper bound
		mid = (lo + hi) / 2;
		int res = strncmp(T + SA[mid], P, m);
		// int res = comp(SA[mid]) if working with strings
		if (res > 0) hi = mid;
		else         lo = mid + 1;
	}

	if (strncmp(T + SA[hi], P, m) != 0) hi--;// special case
	ans.second = hi;
	return ans;
} // return (lowerbound, upperbound)

int owner(int idx) { return (idx < n - m - 1) ? 1 : 2;}

// Longest Common Subsequence (entre 2 strings): n = T1.size(), m = T2.size()
// Primero las concatenamos: T = T1$T2#. Construimos elsuffix array de esta string.
// Llamamos al LCP, y despues a esto -> coste total O(nlog n)

ii LCS() { // O(n), returns  (l: the LCS length, i: itsindex in the SA), i.e.,
		   // solution = T[SA[i]] T[SA[i] + 1] ... T[SA[i] + l- 1]
	int i, idx = 0, maxLCP = -1;
	for (i = 1; i < n; i++)
		if (owner(SA[i]) != owner(SA[i - 1]) && LCP[i] >maxLCP)
			maxLCP = LCP[i], idx = i;
	return ii(maxLCP, idx);
}
*/

int main() {
	int nc;
	cin >> nc;
	for (int z = 0; z < nc; ++z) {
		string s;
		cin >> s;

		s.push_back('$');

		T = s;
		n = s.size();

		vi RA(n), SA(n);

		constructSA(RA, SA);
		vi LCP = computeLCP(SA);

		int midx = 0, mlen = 0, mreps = 0;
		int idxact = 0, repsact = 0;

		for (int i = 1; i < LCP.size(); ++i) {
			if (LCP[i] > mlen) {
				midx = i; mlen = LCP[i]; mreps = 2;
				idxact = i - 1; repsact = 2;
			}
			else if (LCP[i] == mlen) {
				if (i - idxact == repsact) {
					++repsact;

					if (repsact > mreps) {
						mreps = repsact;
						midx = idxact;
					}
				}
			}
			else {
				repsact = 0;
				idxact = i;
			}
		}

		if (mlen == 0) cout << "No repetitions found!\n";
		else {
			for (int i = SA[midx]; i < SA[midx] + mlen; ++i) {
				cout << s[i];
			}
			cout << ' ' << mreps << '\n';
		}
	}

	return 0;
}