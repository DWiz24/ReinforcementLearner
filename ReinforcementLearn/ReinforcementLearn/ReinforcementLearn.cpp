// ReinforcementLearn.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Learner.h"
#include "GridWorld.h"
#include "IceWorld.h"


int main()
{
	using namespace std;
	//discreteLearn(gridStartPos, gridListMoves, gridMove, 30, 0.4, 0.98, 0.2);

	discreteLearn(iceStartPos, iceListMoves, iceMove, 60000, 0.4, 0.99, 0.2);
	cout << "hello world" << endl;
	return 0;
}

