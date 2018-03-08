//Alberto Maurel
//UVa 10503 - The dominoes solitaire
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

//Branch and bound problem

bool solve(vector<vector<int>> & kindOfAvailablePieces, int rightSideOfActualPiece, int nSpaces, int actualPosition, int leftSideOfLastPiece) {
	//If we have reached the end in a valid state we already have the solution
	if (actualPosition == nSpaces) {
		if (leftSideOfLastPiece == rightSideOfActualPiece) return true;
		else return false;
	}
	else {
		//If not we try each piece with the same left side as the right side of the previous one
		for (int i = 0; i <= 6; ++i) {
			if (kindOfAvailablePieces[rightSideOfActualPiece][i] > 0) {
				//We decrement the piece in both places (we placed it twice)
				--kindOfAvailablePieces[rightSideOfActualPiece][i];
				--kindOfAvailablePieces[i][rightSideOfActualPiece];
				if (solve(kindOfAvailablePieces, i, nSpaces, actualPosition + 1, leftSideOfLastPiece)) {
					goto finish;
				}
				//And we reset back to the starting state if the one we tried was not valid
				++kindOfAvailablePieces[rightSideOfActualPiece][i];
				++kindOfAvailablePieces[i][rightSideOfActualPiece];
			}
		}
		return false;
	}	
finish:
	return true;
}

int main() {
	int numberOfSpaces, nPieces;
	cin >> numberOfSpaces; 
	while (numberOfSpaces != 0) {
		//We store each piece twice: like if it starts with one side or if it starts with the other
		vector<vector<int>> kindOfAvailablePieces(7,vector<int>(7,0));
		int begin, end;
		int leftBeginning, rightBeginning, rightEnding, leftEnding;
		cin >> nPieces >> leftBeginning >> rightBeginning >> leftEnding >> rightEnding;
		for (int i = 0; i < nPieces; ++i) {
			cin >> begin >> end;
			++kindOfAvailablePieces[begin][end];
			++kindOfAvailablePieces[end][begin];
		}

		if (solve(kindOfAvailablePieces, rightBeginning, numberOfSpaces, 0, leftEnding)) cout << "YES\n";
		else cout << "NO\n";
		cin >> numberOfSpaces;
	}
	return 0;
}