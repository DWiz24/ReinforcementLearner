// ReinforcementLearn.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Learner.h"
#include "GridWorld.h"
#include "IceWorld.h"
#include "MazeBot.h"
#include <time.h>

int main()
{
	using namespace std;
	srand(time(0));
	//discreteLearn(gridStartPos, gridListMoves, gridMove, 30, 0.4, 0.98, 0.2);

	//discreteLearn(iceStartPos, iceListMoves, iceMove, 60000, 0.4, 0.99, 0.2);

	discreteLearn(mazeStartPos, mazeListMoves, mazeMove, 60000, 0.2, 0.99, 0.00);
	//discreteLearn(mazeStartPos, mazeListMoves, mazeMove, 60000, 0.2, 0.99, 0.02);
	cout << "hello world" << endl;
	return 0;
}

