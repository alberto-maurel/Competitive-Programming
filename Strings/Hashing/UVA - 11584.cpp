#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <map>

using namespace std;

using vi = vector<int>;

using ll = long long int;
using vll = vector<ll>;

const ll MOD1 = 1000000007;
const ll MOD2 = 999999937;

ll p1 = 89, p2 = 97;

vll p1pot;
vll p2pot;
vll p1potinv;
vll p2potinv;

ll eea(ll a, ll b, ll& x, ll& y) {
	ll xx = y = 0, yy = x = 1;
	while (b) {
		ll q = a / b, t = b; b = a % b; a = t;
		t = xx; xx = x - q * xx; x = t;
		t = yy; yy = y - q * yy; y = t;
	}
	return a;
}

void precalc() {
	ll p1pow = 1, p2pow = 1;

	for (int i = 0; i < 1010; ++i) {
		ll inv1, aux1, inv2, aux2;
		eea(p1pow, MOD1, inv1, aux1);
		eea(p2pow, MOD2, inv2, aux2);

		p1pot.push_back(p1pow);
		p2pot.push_back(p2pow);
		p1potinv.push_back(inv1);
		p2potinv.push_back(inv2);

		p1pow = (p1pow * p1) % MOD1;
		p2pow = (p2pow * p2) % MOD2;
	}
}

struct PolyHash {
	vll prec_h1;
	vll prec_h2;

	void prec_hash(string const& s) {
		ll p1acum = 0, p2acum = 0;

		for (int i = 0; i < s.size(); ++i) {
			p1acum = (p1acum + (s[i] - 'a' + 1) * p1pot[i]) % MOD1;
			p2acum = (p2acum + (s[i] - 'a' + 1) * p2pot[i]) % MOD2;

			prec_h1.push_back(p1acum);
			prec_h2.push_back(p2acum);
		}
	}

	pair<ll, ll> poly_hash(int ini, int fin) {
		ll res1 = (prec_h1[fin] - (ini > 0 ? prec_h1[ini - 1] : 0) + MOD1) % MOD1;
		ll res2 = (prec_h2[fin] - (ini > 0 ? prec_h2[ini - 1] : 0) + MOD2) % MOD2;

		res1 = (((res1 * p1potinv[ini]) % MOD1) + MOD1) % MOD1;
		res2 = (((res2 * p2potinv[ini]) % MOD2) + MOD2) % MOD2;

		return { res1, res2 };
	}
};

int dp(PolyHash& hashs, PolyHash& hashr, int len) {
	vi marcaje(len + 1, 10000);
	marcaje[0] = 0;


	for (int i = 0; i < len; ++i) {
		for (int j = i; j < len; ++j) {
			if (hashs.poly_hash(i, j) == hashr.poly_hash(len - 1 - j, len - 1 - i)) {
				marcaje[j + 1] = min(marcaje[j + 1], marcaje[i] + 1);
			}
		}
	}

	return marcaje[len];
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	precalc();

	int nc;
	cin >> nc;

	for (int z = 0; z < nc; ++z) {
		string s;
		cin >> s;

		PolyHash hashs;
		hashs.prec_hash(s);

		string srev = s;
		reverse(srev.begin(), srev.end());
		PolyHash hashr;
		hashr.prec_hash(srev);

		vi minpal(s.size(), -1);
		cout << dp(hashs, hashr, s.size()) << '\n';
	}

	return 0;
}