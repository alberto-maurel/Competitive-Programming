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
#include <complex>
#include <fstream>

using namespace std;

const double EPS = 1e-1;

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

// sign = 1 -> forward transform, sign = -1 -> reverse
void fft(vector<complex<double>>& a, int sign = 1) {
	int n = a.size(); // n should be a power of two
	float theta = 8 * sign * atan(1.0) / n;
	for (int i = 0, j = 1; j < n - 1; ++j) {
		for (int k = n >> 1; k > (i ^= k); k >>= 1);
		if (j < i) swap(a[i], a[j]);
	}
	for (int m, mh = 1; (m = mh << 1) <= n; mh = m) {
		int irev = 0;
		for (int i = 0; i < n; i += m) {
			complex<double>  w = exp(complex<double>(0, theta * irev));
			for (int k = n >> 2; k > (irev ^= k); k >>= 1);
			for (int j = i; j < mh + i; ++j) {
				int k = j + mh;
				complex<double>  x = a[j] - a[k];
				a[j] += a[k];
				a[k] = w * x;
			}
		}
	}
	if (sign == -1) {
		for (int i = 0; i < n; i++)
			a[i] /= n;
	}
	return;
}

vector<char> letras = { 'A', 'C', 'T', 'G' };

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	
	string s, t;
	cin >> s >> t;

	reverse(t.begin(), t.end());

	

	int tam = 1;
	while (tam < s.size() + t.size())
		tam <<= 1;

	vector<int> hamming_dis(s.size() + t.size(), 0);
	
	for (int i = 0; i < letras.size(); ++i) {
		vector<complex<double>> vs(tam, {0,0});
		vector<complex<double>> vt(tam, {0,0});

		for (int j = 0; j < s.size(); ++j) 
			if (s[j] == letras[i]) 
				vs[j] = { 1,0 };

		for (int j = 0; j < t.size(); ++j)
			if (t[j] == letras[i])
				vt[j] = { 1,0 };

		fft(vs, 1);
		fft(vt, 1);
		for (int j = 0; j < vs.size(); ++j) {
			vs[j] = vs[j] * vt[j];
		}
		fft(vs, -1);

		for (int j = 0; j < hamming_dis.size(); ++j) {
			hamming_dis[j] += (int) round(vs[j].real());
		}
	}

	int res = t.size();
	for (int i = t.size() - 1; i < s.size(); ++i) {
		res = min(res, (int) t.size() - hamming_dis[i]);
	}
	cout << res << '\n';

	return 0;
}