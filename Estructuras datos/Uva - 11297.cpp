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

#define mp(a,b) make_pair(a,b)

const int MOD = 1000 * 1000 * 1000 + 7;
const int INF = 1000 * 1000 * 1000;

const ii NEUTRO = { INT_MAX, INT_MIN };

struct SegmentTree2D {
	int sizef;
	int sizec;
	vector<vector<ii>> data; //(mínimo, máximo)

	void init(int f, int c) {
		sizef = 1;
		while (sizef < f) sizef <<= 1;
		data.resize(2 * sizef);

		sizec = 1;
		while (sizec < c) sizec <<= 1;
		
		for (int i = 0; i < 2*sizef; ++i) {
			data[i].resize(2 * sizec, NEUTRO);
		}
	}

	//ACTUALIZAR SEGUN LA OPERACION
	ii combinar(ii const& a, ii const& b) {
		int m = min(a.first, b.first);
		int M = max(a.second, b.second);
		return { m,M };
	}

	
	void build_x(vvi const& v, int y, int ly, int ry, int x, int lx, int rx) {
		if (lx + 1 == rx) {
			if (lx < v[0].size())
				if (ly + 1 == ry) data[y][x] = { v[ly][lx], v[ly][lx] }; //MODIFICAR
				else data[y][x] = combinar(data[2 * y + 1][x], data[2 * y + 2][x]);
		}

		else {
			int mx = (lx + rx) / 2;
			build_x(v, y, ly, ry, 2 * x + 1, lx, mx);
			build_x(v, y, ly, ry, 2 * x + 2, mx, rx);
			
			data[y][x] = combinar(data[y][2 * x + 1], data[y][2 * x + 2]);
		}
	}

	void build_y(vvi const& v, int y, int ly, int ry) {
		if (ly + 1 == ry) {
			if (ly < v.size())
				build_x(v, y, ly, ry, 0, 0, sizec);
		}

		else {
			int my = (ly + ry) / 2;
			build_y(v, 2 * y + 1, ly, my);
			build_y(v, 2 * y + 2, my, ry);

			build_x(v, y, ly, ry, 0, 0, sizec);
		}
	}

	void build(vvi const& v) {
		build_y(v, 0, 0, sizef);
	}
	

	ii query_x(int x1, int y1, int x2, int y2, int y, int ly, int ry, int x, int lx, int rx) {
		if (x1 <= lx && rx <= x2)
			return data[y][x];
		else if (rx <= x1 || x2 <= lx) return NEUTRO;
		else {
			int mx = (lx + rx) / 2;
			ii s1 = query_x(x1, y1, x2, y2, y, ly, ry, 2 * x + 1, lx, mx);
			ii s2 = query_x(x1, y1, x2, y2, y, ly, ry, 2 * x + 2, mx, rx);

			return combinar(s1, s2);
		}
	}

	ii query_y(int x1, int y1, int x2, int y2, int y, int ly, int ry) {
		if (y1 <= ly && ry <= y2)
			return query_x(x1, y1, x2, y2, y, ly, ry, 0, 0, sizec);
		else if (ry <= y1 || y2 <= ly) return NEUTRO;
		else {
			int my = (ly + ry) / 2;
			ii s1 = query_y(x1, y1, x2, y2, 2 * y + 1, ly, my);
			ii s2 = query_y(x1, y1, x2, y2, 2 * y + 2, my, ry);

			return combinar(s1, s2);
		}
	}

	ii query(int x1, int y1, int x2, int y2) {
		return query_y(x1, y1, x2, y2, 0, 0, sizef);
	}

	void update_x(int px, int py, int val, int y, int ly, int ry, int x, int lx, int rx) {
		if (lx + 1 == rx) {
			if (ly + 1 == ry)
				data[y][x] = { val, val };
			else
				data[y][x] = combinar(data[2 * y + 1][x], data[2 * y + 2][x]);
		}
		else {
			int mx = (lx + rx) / 2;
			if (px < mx) update_x(px, py, val, y, ly, ry, 2 * x + 1, lx, mx);
			else         update_x(px, py, val, y, ly, ry, 2 * x + 2, mx, rx);

			data[y][x] = combinar(data[y][2 * x + 1], data[y][2 * x + 2]);
		}
	}

	void update_y(int px, int py, int val, int y, int ly, int ry) {
		if (ly + 1 == ry)
			update_x(px, py, val, y, ly, ry, 0, 0, sizec);
		
		else {
			int my = (ly + ry) / 2;
			if (py < my) update_y(px, py, val, 2 * y + 1, ly, my);
			else         update_y(px, py, val, 2 * y + 2, my, ry);

			update_x(px, py, val, y, ly, ry, 0, 0, sizec);
		}
	}

	void update(int px, int py, int val) {
		update_y(px, py, val, 0, 0, sizef);
	}
};

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	
	int n;
	cin >> n;

	vvi v(n, vi(n));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			cin >> v[i][j];
		}
	}

	SegmentTree2D st;
	st.init(n, n);

	st.build(v);

	int q;
	cin >> q;
	for (int i = 0; i < q; ++i) {
		char op;
		cin >> op;
		if (op == 'q') {
			int x1, x2, y1, y2;
			cin >> y1 >> x1 >> y2 >> x2;
			--x1; --x2; --y1; --y2;
			
			ii res = st.query(x1, y1, x2 + 1, y2 + 1);
			cout << res.second << ' ' << res.first << '\n';
		}
		else {
			int x, y, v;
			cin >> x >> y >> v;
			--x; --y;

			st.update(y, x, v);
		}
	}

	return 0;
}