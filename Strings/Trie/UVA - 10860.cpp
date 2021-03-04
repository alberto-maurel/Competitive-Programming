#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

//Esto originalmente lo saqué de Geeks for Geeks:
//https://www.geeksforgeeks.org/trie-insert-and-search/

/* USO
Primero, fijar el numero de letras del alfabeto, y la primera letra del alfabeto.
Después se puede crear el trie : TrieNode * root = getNode()
Y a partir de ahí : insertar, borrar, buscar, moverse por el ́arbol...
*/

//La raíz del árbol es una especie de nodo Dummy, la información comienza en sus hijos

const int ALPHABET_SIZE = 100; //NUMERO DE LETRAS DEL ALFABETO
const char FIRST_LETTER = 33; //PRIMERA LETRA DEL ALFABETO

const int INF = 1000 * 1000 * 1000;

//Nodo del trie
struct TrieNode {
	TrieNode* children[ALPHABET_SIZE];
	bool fin; //True si es el final de una palabra
};

TrieNode*getNode() { // Returns new trie (all NULLs)
	TrieNode*ptr = new TrieNode;
	ptr->fin = false;

	for (int i = 0; i < ALPHABET_SIZE; i++)
		ptr->children[i] = NULL;
	return ptr;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leafnode
void insert(TrieNode*root, string key) { // O(key.size())
	TrieNode*ptr = root;
	for (int i = 0; i < key.length(); i++) {
		int index = key[i] - FIRST_LETTER;
		if (!ptr->children[index])
			ptr->children[index] = getNode();
		ptr = ptr->children[index];
	}
	
	ptr->fin = true; // mark as leaf
}

// Returns true if key presents in trie, else false
bool search(TrieNode*root, string key) { // O(key.size())
	TrieNode*ptr = root;
	for (int i = 0; i < key.length(); i++) {
		int index = key[i] - FIRST_LETTER;
		if (!ptr->children[index])
			return false;
		ptr = ptr->children[index];
	}
	
	return (ptr != NULL && ptr->fin);
}

//LA PARTE DEL REMOVE NO LA HE PROBADO

bool isEmpty(TrieNode*root) {
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (root->children[i]) return false;
	return true;
}

// Recursive function to delete a key from given Trie
TrieNode*remove(TrieNode*root, string key, int depth= 0) { // O(key.size())
	if (!root) return NULL; // empty trie
	if (depth == key.size()) { // Last character
		root->fin = false; // no more end ofword
		if (isEmpty(root)) { // if not prefix: delete
			delete (root);
			root = NULL;
		}
		return root;
	}
	int index = key[depth] - FIRST_LETTER; 
	root->children[index] = remove(root->children[index], key, depth + 1);
	if (isEmpty(root) && root->fin == false) {
		delete (root); 
		root = NULL; // recursive delete
	}
	return root;
}

int dp(int nact, string const & s, TrieNode* root, vector<int> & marcaje) {
	if (nact == s.size())
		return 0;
	else if (marcaje[nact] != -1) 
		return marcaje[nact];
	else {
		TrieNode* trie = root;
		int sol = INF;

		for (int i = 0; nact + i < s.size(); ++i) {
			int index = s[nact + i] - FIRST_LETTER;

			//No hay hijo por el que seguir
			if (!trie->children[index])
				break;

			trie = trie->children[index];

			if (trie->fin) {
				sol = min(sol, 1 + dp(nact + i + 1, s, root, marcaje));
			}
		}

		marcaje[nact] = sol;
		return sol;
	}
}

void res(int nc) {
	string s;
	getline(cin, s);

	int elems;
	cin >> elems;

	string aux;
	getline(cin, aux);

	TrieNode* root = getNode();

	for (int i = 0; i < elems; ++i) {
		string pal;
		getline(cin, pal);

		insert(root, pal);
		reverse(pal.begin(), pal.end());

		insert(root, pal);
	}


	cout << "Set " << nc << ": ";
	vector<int> marcaje(s.size(), -1);
	int sol = dp(0, s, root, marcaje);

	if (sol != INF) cout << sol << ".\n";
	else cout << "Not possible.\n";
}

int main() {
	int nc;
	cin >> nc;

	string aux;
	getline(cin, aux);

	for (int z = 0; z < nc; ++z) {
		res(z + 1);
	}

	return 0;
}