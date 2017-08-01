#include "stdafx.h"
#include<vector>
#include <iostream>
#include <string>
static long printFreq = 10;
static long runCount = 0;
static long goalRow = 0;
static long goalCol = 5;
static double movePenalty = -0.01;
static char openSpaces[7][6] = {
	{1,1,1,1,0,1},
	{1,1,1,0,0,1},
	{0,0,1,1,0,1},
	{1,1,1,1,0,1},
	{1,0,1,0,1,1},
	{1,1,1,1,0,1},
	{1,1,1,1,1,1}
};
static double rewards[7][6] = {
	{0,0,0,0,0,10},
	{0,0,0,0,0,0},
	{ 0,0,0,0,0,0 },
	{ 0,0,0,0,0,0 },
	{ 0,0,0,0,0,0 },
	{ 0,0,0,0,0,0 },
	{ 0,0,0,0,0,0 }
};

static long xDirs[4] = { -1,1,0,0 };
static long yDirs[4] = { 0,0,-1,1 };
static double qTable[7][6][4] = {}; //left=0, right=1, up=2, down=3
//initialize everything to 0

std::vector<long> gridStartPos() {
	using std::vector;
	vector<long> start;
	start.push_back(0); //start row
	start.push_back(0); //start col
	runCount++;
	if (runCount%printFreq == 0) {
		std::cout << "Starting Run" << std::endl;
	}
	return start;
}

std::vector<double*> gridListMoves(std::vector<long> pos) {
	using std::vector;
	//pos[1] is row, [0] is col
	vector<double*> possible;
	for (long i = 0; i < 4; i++) {
		long r = pos[0] + xDirs[i];
		long c = pos[1] + yDirs[i];
		if (r>= 0 && r < 6 && c >= 0 && c < 7 && openSpaces[c][r]!=0) {
			possible.push_back(&qTable[pos[1]][pos[0]][i]);
		}
	}
	return possible;
}

void printBoard(long row, long col) {
	using namespace std;
	for (long i = 0; i < 7; i++) {
		string str("");
		for (long k = 0; k < 6; k++) {
			string c = ".";
			if (!openSpaces[i][k]) {
				c = "x";
			}
			else if (i == row && k == col) {
				c = "A";
			}
			else if (i == goalRow && k == goalCol) {
				c = "G";
			}
			str += c + " ";
		}
		cout << str << endl;
		
	}
	cout << endl;
}
std::pair<std::vector<long>, double> gridMove(std::vector<long> pos, long choice) {
	long dir = 0;
	long foundMoves = 0;
	for (long i = 0; i<4; i++) {
		long r = pos[0] + xDirs[i];
		long c = pos[1] + yDirs[i];
		if (r >= 0 && r < 6 && c >= 0 && c < 7 && openSpaces[c][r]!=0) {
			if (foundMoves == choice) {
				dir = i;
				break;
			}
			foundMoves++;
		}
	}
	pos[1] += yDirs[dir];
	pos[0] += xDirs[dir];
	if (runCount % printFreq == 0) {
		printBoard(pos[1],pos[0]);
	}
	std::pair<std::vector<long>, double> ret;
	ret.second = rewards[pos[1]][pos[0]] + movePenalty;
	if (pos[1] == goalRow && pos[0] == goalCol) {
		std::vector<long> empty;
		ret.first = empty;
		return ret;
	}
	ret.first = pos;
	return ret;
}
