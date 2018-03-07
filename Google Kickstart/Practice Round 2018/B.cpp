//Alberto Maurel
//Solution to Problem C - Kickstart Practice Round 2018

//The idea is to find first the minimum n where Sn has k or more characters (S(n + i) will obviously contain also the character k and it would be
//a waste of time)
//The length of S(n) is the n-th Mersenne number (we can calculate it by using the formula 2^n - 1). If you don't see the formula, you can use 
//oeis.org to find it.
//Using that formula, we find the smallest n we need (n = log2(k + 1)) and then we let our function make the magic

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <algorithm>
using namespace std;

//Recursive function to find the solution. O(log n)
int findGogol(unsigned long long int k, unsigned long long int stringLength) {
	//If we have only one character we are at S1 and the character is 0
	if (stringLength == 1) return 0;
	else {
		//The character we are looking for is the central character, so it will obviously be a 0
		if (k - 1 == (stringLength - 1) / 2) return 0;
		//The k-th character is at the left part of S(i), so it will be the same character as the k-th character of S(i - 1) 
		else if (k - 1 < (stringLength - 1)/ 2) return findGogol(k, (stringLength - 1)/ 2);
		//The k-th character is at the right part of S(i), so it will be the opposite character (we switch the string)
		//as the (stringLength - k + 1)-th character of S(i - 1) (we reverse the string, like we were looking it through a mirror)
		else return 1 - findGogol(stringLength - k + 1, (stringLength - 1)/ 2);
	}
}

int main() {
	int T;
	ifstream input; input.open("BLarge.in");
	ofstream output; output.open("OutputLarge.txt");

	input >> T;
	for (int i = 1; i <= T; ++i) {
		unsigned long long int k;
		input >> k;
		output << "Case #" << i << ": ";
		
		//We find the lenght of the first string as explained at the top part
		//VERY IMPORTANT:  we need to use ceil(log2(k + 1)) + 1 instead of ceil(log2(k + 1)) because for the biggest numbers we don't have 
		//precission enough, and the second code may lead to a Mersenne number smaller than k, resulting in an undesired WA
		unsigned long long int aux = ceil(log2(k + 1)) + 1;

		//Next Mersenne Number
		unsigned long long firstStringLength = pow(2, aux);
		--firstStringLength;
		output << findGogol(k, firstStringLength) << '\n';
	}
	return 0;
}