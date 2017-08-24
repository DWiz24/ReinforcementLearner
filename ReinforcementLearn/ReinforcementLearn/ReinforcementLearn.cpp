// ReinforcementLearn.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Learner.h"
#include "GridWorld.h"
#include "IceWorld.h"
#include "MazeBot.h"
#include <time.h>
#include "JetpackGame.h"
#include "EvoSystems.h"
#include "DeepRL.h"


int main()
{
	using namespace std;
	srand(time(0));
	
	//discreteLearn(gridStartPos, gridListMoves, gridMove, 30, 0.4, 0.98, 0.2);

	//discreteLearn(iceStartPos, iceListMoves, iceMove, 60000, 0.4, 0.99, 0.2);

	//discreteLearn(mazeStartPos, mazeListMoves, mazeMove, 60000, 0.2, 0.99, 0.01);

	setUpGameWindow();
	//void ESLearn(long iterations, long population, double stdev, double learnRate)
	//ESNewNet(500000, 50, 0.002, 0.0004);
	//ESLearn(500000, 40, 0.002, 0.0004, loadFile("1267.txt"));

	//ESLearn(1, 1, 0, 0, loadFile("best_ES.txt")); //the update algorithm breaks when it can't learn and has zero stdev, so just observe for a generation;

	//RLNewNet(double learnRate, long replaySize, double randChance, double discount)
	//RLNewNet(     0.0001,              500,           0.05,              0.95);

	//void DNQFile(double learnRate, long replaySize, double randChance, double discount, std::string file);
	DNQFile(           0.0001,       500,               0.05,              0.95,         "DQN.txt");
	cout << "hello world" << endl;
	return 0;
}

