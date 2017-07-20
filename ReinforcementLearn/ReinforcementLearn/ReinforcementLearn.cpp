// ReinforcementLearn.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Learner.h"
#include "GridWorld.h"



int main()
{
	using namespace std;
	discreteLearn(startPos, listMoves, move, 10000, 0.4, 0.98, 0.2);
	cout << "hello world" << endl;
	return 0;
}

