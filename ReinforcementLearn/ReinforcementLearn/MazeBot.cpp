#include "stdafx.h"
#include<vector>
#include <algorithm>
#include<iostream>
#include<string>

static long printFreq=1;
static long runs = 0;

static const long rows = 13;
static const long cols = 9;
static long startRow = 5;
static long startCol = 3;
static long agentR;
static long agentC;

//I'll implement these if needed
static long runDuration;
static long runTimeout = 500;
static double timeoutPenalty = -100;

static double movePenalty = -0.1;
static double visitBonus = 2.0;
static long maze[rows][cols];
static long visited[rows][cols];
//0 is wall, 1 is default, 1-3 is marked

long rDirs[4] = { -1,1,0,0 };
long cDirs[4] = { 0,0,-1,1 };

//state is <paint on my space, up, down, left ,right>
//array is [current paint][up paint][down paint][left paint][right paint][color leaving behind][direction]
static double qTable[4][4][4][4][4][4][4] = {};

void DFS(long r, long c) {
	using std::vector;
	vector<long> moves = { 0,1,2,3 };
	std::random_shuffle(moves.begin(), moves.end());
	for (long i = 0; i < 4; i++) {
		long newR = r + 2*rDirs[moves[i]];
		long newC = c + 2*cDirs[moves[i]];
		if (newR >= 0 && newR < rows && newC >= 0 && newC < cols && maze[newR][newC] == 0) {
			maze[newR][newC] = 1;
			maze[r + rDirs[moves[i]]][c+cDirs[moves[i]]] = 1;
			DFS(newR, newC);
		}
	}
}

void remakeMaze() {
	for (long i = 0; i < rows; i++) {
		for (long k = 0; k < cols; k++) {
			maze[i][k] = 0;
			visited[i][k] = 0;
		}
	}
	maze[startRow][startCol] = 1;
	DFS(startRow, startCol);
}
std::vector<long> mazeStartPos() {
	remakeMaze();
	std::vector<long> start;
	start.push_back(1);
	for (long i = 0; i < 4; i++) {
		start.push_back(maze[startRow+rDirs[i]][startCol+cDirs[i]]);
	}
	agentR = startRow;
	agentC = startCol;
	runDuration = 0;
	runs++;
	if (runs%printFreq == 0) {
		std::cout << "Starting Run" << std::endl;
	}
	return start;
}
std::vector<double*> mazeListMoves(std::vector<long> pos) {
	std::vector<double*> possibleMoves;
	for (long color = 1; color < 4; color++) {
		for (long dir = 0; dir < 4; dir++) {
			possibleMoves.push_back(&qTable[pos[0]][pos[1]][pos[2]][pos[3]][pos[4]][color][dir]);
		}
	}
	return possibleMoves;
}
void printMaze() {
	using namespace std;
	char symbols[4] = { 'x','.','*','-' };
	char agentSymbols[4] = { '@','@','@','@' }; //agent symbol depends on what is under it
	for (long i = 0; i < rows; i++) {
		string str("");
		for (long k = 0; k < cols; k++) {
			if (i == agentR && k == agentC) {
				str += agentSymbols[maze[i][k]];
			}
			else {
				str += symbols[maze[i][k]];
			}
		}
		cout << str << endl;
	}
	cout << endl;
}
std::pair<std::vector<long>, double> mazeMove(std::vector<long> pos, long choice) {
	if (runs%printFreq == 0) {
		printMaze();
	}
	
	using std::vector;
	vector<long> newState;
	double reward = movePenalty;
	long chosenDir = choice % 4;
	long chosenColor = choice / 4 + 1;
	long newR = agentR + rDirs[chosenDir];
	long newC = agentC + cDirs[chosenDir];
	if (maze[newR][newC] == 0) {
		reward -= 100;
		newState.push_back(pos[0]);
		newState.push_back(pos[1]);
		newState.push_back(pos[2]);
		newState.push_back(pos[3]);
		newState.push_back(pos[4]);
	}
	else {
		
		if (!visited[newR][newC]) {
			reward += visitBonus;
			visited[newR][newC] = 1;
		}
		maze[agentR][agentC] = chosenColor;
		agentR = newR;
		agentC = newC;
		newState.push_back(maze[newR][newC]);
		for (long dir = 0; dir < 4; dir++) {
			newState.push_back(maze[newR + rDirs[dir]][newC + cDirs[dir]]);
		}
		if (newR == startRow && newC == startCol) {
			newState.clear();
		}
		
	}
	std::pair<std::vector<long>, double> ret;
	ret.first = newState;
	ret.second = reward;
	return ret;
}

