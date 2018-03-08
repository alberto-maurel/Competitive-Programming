//Alberto Maurel
//UVa Judge 11310 - Delivery Debacle
#include <iostream>
#include <vector>
using namespace std;

//An easy dynamic programming problem

const long long int EMPTY = 0;

long long int solve(vector<long long int> & dynamic, int actualNumber) {
	if (actualNumber < 1) {
		return 1;
	}
	//If the length of the remaining box if is only one, we can only place the cakes in one way (2 small ones)
	else if (actualNumber == 1) {
		dynamic[1] = 1;
		return 1;
	}
	//If the length of the remaining box if is only one, we can only place the cakes in five ways (look at the statement)
	else if (actualNumber == 2) {
		dynamic[2] = 5;
		return 5;
	}
	//If we have precalculated the ways of placing cakes for that lenght, we just have to use that previous solution
	else if (dynamic[actualNumber] != EMPTY) {
		return dynamic[actualNumber];
	}
	//If we are in any other case, we are sure we have a lenght of 3 or more. We have three posibilities:
	//1 - If we place 2 small cakes in the first empty column, there are solve(actualNumber - 1) posibilities for placing the remaining cakes
	//2 - If we place a big cake and we fill the remaining cell with a small cake, there are 4 * solve(actualNumber - 2) (4 rotations) posibilities
	//3 - If we place 2 big cakes, there are 2 * solve(actualNumber - 2) posibilities
	//We don't have to concern about counting any case twice, because obviously this combinations don't overlap 
	else {
		long long int sol = solve(dynamic, actualNumber - 1);
		sol += 4 * solve(dynamic, actualNumber - 2);
		sol += 2 * solve(dynamic, actualNumber - 3);
		dynamic[actualNumber] = sol;
		return sol;
	}
}

int main() {
	int nCases;
	int lenght;
	cin >> nCases;
	vector<long long int> dynamic(45, EMPTY);
	solve(dynamic, 40);
	for (int i = 0; i < nCases; ++i) {
		cin >> lenght;
		cout << dynamic[lenght] << '\n';
	}

	return 0;
}