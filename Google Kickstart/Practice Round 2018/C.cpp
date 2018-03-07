//Alberto Maurel
//Solution to Problem C - Kickstart Practice Round 2018

#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <string>
#include <fstream>
using namespace std;

int main() {
	int T, N;

	ifstream input; input.open("CLarge.in");
	ofstream output; output.open("outputLarge.in");

	input >> T;
	for (int i = 1; i <= T; ++i) {
		input >> N;
		map<string, pair<string, bool>> sparseTickets;

		string dest1, dest2;
		for (int j = 0; j < N; ++j) {
			input >> dest1 >> dest2;
			sparseTickets.insert(make_pair(dest1, make_pair(dest2, false)));
		}

		//In this problem we will take advantage of we know that there is only a valid solution. 
		deque<pair<string,string>> solution;

		//We don't know where the itinerary starts. So instead of searching the begin, we start in an arbitrary point. When we find the last
		//flight or a flight we have previously taken, we look for the next flight we haven't already processed. 
		//That lead us to a partial solution, and it's guaranteed that partial solution will end at the begin of the solution we previously had
		//This algorithm conduce us to the solution
		for (auto it = sparseTickets.begin(); it != sparseTickets.end(); ++it) {

			deque<pair<string,string>> partialSolution;
			auto actualIt = it;

			//For each ticket, if we havent already used it, we do
			//We construct all the flights from that one till the first one we have already used (or that is the last one)
			while (actualIt != sparseTickets.end() && !actualIt->second.second) {
				partialSolution.push_back(make_pair(actualIt->first, actualIt->second.first));
				actualIt->second.second = true;
				actualIt = sparseTickets.find(actualIt->second.first);
			}

			//And now we copy the partial solution at the begin of the previous partial solution
			while (partialSolution.size() > 0) {
				solution.push_front(partialSolution[partialSolution.size() - 1]);
				partialSolution.pop_back();
			}
		}

		//And we show the solution
		output << "Case #" << i << ":";
		for (int m = 0; m < solution.size(); ++m) {
			output << ' ' << solution[m].first << '-' << solution[m].second;
		}
		output << '\n';
	}

	return 0;
}