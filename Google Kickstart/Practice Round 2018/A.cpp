//Alberto Maurel
//Solution to Problem A - Kickstart Practice Round 2018

//In this problem we have to precalculate for each city how many buses go through each city before answering the queries
//As far as the amount of cities is small (even in the large dataset), the best option is to store an array with a city in each position
//and using this naive but faster implementation.


//Note: for bigger datasets we should store this in intervals of neightbour cities with the same amount of buses, and do a binary search for
//each query. In addition, we should process the beginnings and ends of the buses, not the cities one by one (having several cities where we 
//don't do anything), as we do in this implementation.

#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <fstream>
#include <functional>
using namespace std;

const int SIZE = 5010;

int main() {
	int N, T;

	ifstream input;
	input.open("ALarge.in");
	ofstream output;
	output.open("outputLarge.txt");

	input >> T;
	for (int i = 1; i <= T; ++i) {
		input >> N;
		vector<pair<int,int>> stops;
		int p1, p2;
		for (int j = 0; j < N; ++j) {
			input >> p1 >> p2;
			stops.push_back(make_pair(p1, p2));
		}
		sort(stops.begin(), stops.end());
		
		
		//Preprocessing
		vector<int> sol(SIZE, 0);
		int pos = 0;
		priority_queue<int, vector<int>, greater<int>> pq;

		//For each city 
		for (int k = 0; k < SIZE; ++k) {
			//First we calculate if some bus line starts at that city
			while (pos< stops.size() && stops[pos].first <= k) {
				pq.push(stops[pos].second);
				++pos;
			}

			//And then if some bus line ends

			//Into the priority queue we only store the cities where a bus line ends. Obviously, if a city is into the priority queue is because
			//there is a line that goes throught the current city, so the amount of bus lines is the same as pq.size() 
			while (!pq.empty() && pq.top() < k) pq.pop();

			//And we store how many buses go through that city
			sol[k] = pq.size();
		}

		int P, aux;
		input >> P;
		
		//And we print the answer for each query
		output << "Case #" << i << ':';
		for (int m = 0; m < P; ++m) {
			input >> aux;
			output<< ' ' << sol[aux];
		}
		output<< '\n';
	}
	output.close();
	return 0;
}