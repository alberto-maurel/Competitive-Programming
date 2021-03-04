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

vll prec_h1;
vll prec_h2;
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

void prec_hash(string const& s) {
	prec_h1.clear(); prec_h2.clear();
	p1pot.clear(); p2pot.clear();
	p1potinv.clear(); p2potinv.clear();

	ll p1 = 31, p2 = 37;
	ll p1pow = 1, p2pow = 1;
	ll p1acum = 0, p2acum = 0;

	for (int i = 0; i < s.size(); ++i) {
		p1acum = (p1acum + (s[i] - 'a' + 1) * p1pow) % MOD1;
		p2acum = (p2acum + (s[i] - 'a' + 1) * p2pow) % MOD2;

		ll inv1, aux1, inv2, aux2;
		eea(p1pow, MOD1, inv1, aux1);
		eea(p2pow, MOD2, inv2, aux2);

		prec_h1.push_back(p1acum);
		prec_h2.push_back(p2acum);
		p1pot.push_back(p1pow);
		p2pot.push_back(p2pow);
		p1potinv.push_back(inv1);
		p2potinv.push_back(inv2);

		p1pow = (p1pow * p1) % MOD1;
		p2pow = (p2pow * p2) % MOD2;
	}
}

pair<ll, ll> poly_hash(int ini, int fin) {
	ll res1 = (prec_h1[fin] - (ini > 0 ? prec_h1[ini - 1] : 0) + MOD1) % MOD1;
	ll res2 = (prec_h2[fin] - (ini > 0 ? prec_h2[ini - 1] : 0) + MOD2) % MOD2;

	assert(res1 >= 0);
	assert(res2 >= 0);

	res1 = (((res1 * p1potinv[ini]) % MOD1) + MOD1) % MOD1;
	res2 = (((res2 * p2potinv[ini]) % MOD2) + MOD2) % MOD2;

	assert(res1 >= 0);
	assert(res2 >= 0);

	return { res1, res2 };
}

bool res() {
	string s = "";
	string lact;
	getline(cin, lact);
	if (!cin) return false;

	for (char a : lact) {
		if (a != ' ') 
			s.push_back(a);
	}

	prec_hash(s);

	for (int d = 0; d < s.size(); ++d) {
		map<pair<ll, ll>, int> biy;
		int maxreps = 0;

		for (int i = 0; i < s.size(); ++i) {
			int j = i + d;
			if (j >= s.size())
				break;

			auto hash = poly_hash(i, j);

			if (biy.find(hash) == biy.end())
				biy.insert({hash, 0});
			++biy[hash];

			maxreps = max(maxreps, biy[hash]);
		}
		if (maxreps < 2) {
			cout << '\n';
			break;
		}
		else cout << maxreps << '\n';
	}
	
	return true;
}

int main() {
	while (res()) {}
	return 0;
}