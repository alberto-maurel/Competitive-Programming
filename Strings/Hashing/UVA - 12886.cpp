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
using vll = vector<ll>;

#define mp(a,b) make_pair(a,b)

//Primos grandes
const ll MOD1 = 1000000007;
const ll MOD2 = 999999937;

//Primos lo suficientemente grandes para que cubran todas las 
// letras del alfabeto (si solo usamos minúsculas podemos coger
// p1 = 31 y p2 = 37)
ll p1 = 89, p2 = 97;

vll p1pot, p2pot;       //Potencias de p1 y p2
vll p1potinv, p2potinv; //Inversas modulares de las potencias

//Algoritmo EEAA para calcular el inverso modular
ll eea(ll a, ll b, ll& x, ll& y) {
	ll xx = y = 0, yy = x = 1;
	while (b) {
		ll q = a / b, t = b; b = a % b; a = t;
		t = xx; xx = x - q * xx; x = t;
		t = yy; yy = y - q * yy; y = t;
	}
	return a;
}

//Función para precalcular las potencias y los inversos
//IMPORTANTE: intentar precalcular todo antes de correr el algoritmo,
//            parece que el inverso modular tarda
void precalc() {
	ll p1pow = 1, p2pow = 1;

	for (int i = 0; i < 2010; ++i) {
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

	//Llamar con la string que queremos hashear para calcular los hashes acumulados
	//Para columnas
	void prec_hash(vector<string> const& s, int col) {
		ll p1acum = 0, p2acum = 0;

		for (int i = 0; i < s.size(); ++i) {
			p1acum = (p1acum + (s[i][col] - 'a' + 1) * p1pot[i]) % MOD1;
			p2acum = (p2acum + (s[i][col] - 'a' + 1) * p2pot[i]) % MOD2;

			prec_h1.push_back(p1acum);
			prec_h2.push_back(p2acum);
		}
	}
	
	//Para calcular el hash de s[ini...fin]
	pair<ll, ll> poly_hash(int ini, int fin) {
		ll res1 = (prec_h1[fin] - (ini > 0 ? prec_h1[ini - 1] : 0) + MOD1) % MOD1;
		ll res2 = (prec_h2[fin] - (ini > 0 ? prec_h2[ini - 1] : 0) + MOD2) % MOD2;

		res1 = (((res1 * p1potinv[ini]) % MOD1) + MOD1) % MOD1;
		res2 = (((res2 * p2potinv[ini]) % MOD2) + MOD2) % MOD2;

		return { res1, res2 };
	}
};

using llll = pair<ll, ll>;
using vllll = vector<llll>;
using vvllll = vector<vllll>;

int sol = 0;

vllll t, p; vi b; int n, m; // n=t.size, m=p.size
void kmpPreprocess() {

	b.resize(n + m + 1);
	int i = 0, j = -1; b[0] = -1;
	
	while (i < m) {
		while (j >= 0 && p[i] != p[j]) 
			j = b[j];
		b[++i]=++j;
	}
}

void kmpSearch() {
	int i = 0, j = 0;
	while (i<n) {
		while (j >= 0 && t[i] != p[j]) j = b[j];
		i++; j++;
		
		if (j == m) {
			++sol;
			j = b[j];
		}
	}
}

bool res() {
	precalc();
	sol = 0;

	int hp, wp, hm, wm;
	cin >> hp >> wp >> hm >> wm;
	if (!cin) return false;

	vector<string> copias(hp);
	vector<string> orig(hm);

	for (int i = 0; i < hp; ++i) cin >> copias[i];
	for (int i = 0; i < hm; ++i) cin >> orig[i];

	vector<PolyHash> hashes(wm);
	for (int i = 0; i < wm; ++i) {
		hashes[i].prec_hash(orig, i);
	}

	vector<PolyHash> hashesCop(wp);
	for (int i = 0; i < wp; ++i) {
		hashesCop[i].prec_hash(copias, i);
	}

	vllll nuevaCopia(wp);
	vvllll nuevoCuadro(hm - hp + 1, vllll(wm));

	for (int i = 0; i < hm - hp + 1; ++i) {
		for (int j = 0; j < wm; ++j) {
			nuevoCuadro[i][j] = hashes[j].poly_hash(i, i + hp - 1);
		}
	}

	for (int j = 0; j < wp; ++j) {
		nuevaCopia[j] = hashesCop[j].poly_hash(0, hp - 1);
	}

	p = nuevaCopia;
	m = p.size();

	for (int i = 0; i < nuevoCuadro.size(); ++i) {
		t = nuevoCuadro[i];
		n = t.size();

		kmpPreprocess();
		kmpSearch();
	}


	cout << sol << '\n';
	return true;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	while (res());
	return 0;
}