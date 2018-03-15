//Alberto Maurel
//UVa 13207 AirCraft: Monster
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include <set>
#include <algorithm>
using namespace std;

//A good example of branch and bound problem
//As far as we can have up to 30 missions, if we try a brute force solution we get a huge time limit. Even if we use BFS or DFS we get tle (at least the versions I
//tried) 
//The idea for each quest decide whether if we want to take it or not. So in a raw brute force solution we would have up to 2^30 posibilities. To prune some branches
//we precompute for each quest how many points could we get if we take all the missions from that point. That is an upper bound for our actual points, so if 
//in one of the three categories we cannot achieve the desired amount, we stop that recursion

//To improve the performance of our algorithm, we also sort the quests using a heuristic. It's clear that if we decline to take the quests that give us the biggest
//amount of points, probably we couldnt reach the point's goal. So it's better to explore that nodes earlier, because if we explore first the ones that give us less
//points, probably we will prune the branch lower (and in every branch and bound problem the goal is to prune asap)

//Exec time: 0,000s

struct triple {
	int p1, p2, p3;
};

//Instead of sorting by the score of one characteristic choosen arbitrarily, we sort by the "mean" of the three
bool operator<(triple const& t1, triple const& t2) {
	int sumT1 = t1.p1 + t1.p2 + t1.p3;
	int sumT2 = t2.p1 + t2.p2 + t2.p3;
	return sumT1 > sumT2;
}

bool branchAndBound(vector<triple> const& points, vector<vector<int>> const& marks, int actualQuest, int p1, int p2, int p3, int a, int b, int c) {
	//If we have gone through all the quests and we haven't get the points needed
	if (actualQuest == points.size()) {
		return false;
	}
	else {
		//If we haven't already surpassed the needed points in any of the three categories
		if (p1 + marks[actualQuest][0] >= a &&  p2 + marks[actualQuest][1] >= b && p3 + marks[actualQuest][2] >= c) {
			bool found1 = false;
			if (p1 + points[actualQuest].p1 == a && p2 + points[actualQuest].p2 == b && p3 + points[actualQuest].p3 == c) {
				found1 = true;
			}
			else if (p1 + points[actualQuest].p1 <= a && p2 + points[actualQuest].p2 <= b && p3 + points[actualQuest].p3 <= c) {
				found1 = branchAndBound(points, marks, actualQuest + 1, p1 + points[actualQuest].p1, p2 + points[actualQuest].p2, p3 + points[actualQuest].p3, a, b, c);
			}

			bool found2 = false;
			if (!found1) found2 = branchAndBound(points, marks, actualQuest + 1, p1, p2, p3, a, b, c);
			return (found1 || found2);
		}
		return false;
	}
}


int main() {
	int T;
	cin >> T;
	for (int i = 0; i < T; ++i) {
		int a, b, c, m;
		cin >> a >> b >> c >> m;

		vector<triple> points;
		string aux;
		int n1, n2, n3;
		for (int j = 0; j < m; ++j) {
			cin >> aux >> n1 >> n2 >> n3;
			points.push_back({ n1,n2,n3 });
		}

		sort(points.begin(), points.end());

		//Now we create the marks
		vector<vector<int>> marks (m, vector<int>(3,0));

		marks[m - 1][0] = points[m - 1].p1;
		marks[m - 1][1] = points[m - 1].p2;
		marks[m - 1][2] = points[m - 1].p3;
		for (int i = m - 2; i >= 0; --i) {
			//We chose that quantity because is bigger than the maximum amount of points that can be asked, but at the same time avoids overflow
			if(marks[i + 1][0] < 50000000) marks[i][0] = points[i].p1 + marks[i + 1][0];
			else marks[i][0] = marks[i + 1][0];

			if (marks[i + 1][1] < 50000000) marks[i][1] = points[i].p2 + marks[i + 1][1];
			else marks[i][1] = marks[i + 1][1];

			if(marks[i + 1][2] < 50000000) marks[i][2] = points[i].p3 + marks[i + 1][2];
			else marks[i][2] = marks[i + 1][2];
		}

		if (branchAndBound(points, marks, 0, 0 ,0 ,0, a, b, c)) cout << "POSSIBLE\n";
		else cout << "IMPOSSIBLE\n";
	}
	return 0;
}
