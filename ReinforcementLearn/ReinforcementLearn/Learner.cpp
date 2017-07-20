#include "stdafx.h"
#include <iostream>
#include "Learner.h"

//I'm trying to seperate the learning code from code that only applies to GridWorld or IceWorld
//I also want to play with function pointers

//A state is a vector of longs


//startingState() returns the a start state for the agent
//possibleActions() returns a list of pointers to the q-values of possible actions in the lookup table
//doAction() takes the current state and the index of the chosen action from possibleActions(),
//   returns the reward and the new state.  New state is an empty vector if the goal was reached
//trainingRuns is the number of times the agent goes through the maze
//learningRate is  (0,1) higher is faster learning
//devaluation is [0,1], how much you discount future rewards. 1 is no discount
//randomChance is the probability tht the agent makes a random choice
void discreteLearn(std::vector<long>(*startingState)(),
	std::vector<double*>(*possibleActions)(std::vector<long>),
	std::pair<std::vector<long>,double> (*doAction)(std::vector<long>, long),
	long trainingRuns, double learningRate, double devaluation, double randomChance) {

	using std::vector;
	for (long runsComplete = 0; runsComplete < trainingRuns; ++runsComplete) {
		vector<long> currentState=startingState();
		vector<double*> actions = possibleActions(currentState);
		long chosenAction = 0;
		double bestQ = *actions[0];
		for (long i = 1; i < actions.size(); ++i) {
			if (*actions[i] > bestQ) {
				bestQ = *actions[i];
				chosenAction = i;
			}
		}
		while (true) {
			//I assume there is always an available action
			double r = (double)rand() / RAND_MAX;
			if (r < randomChance) {
				chosenAction = rand() % actions.size();
			}
			std::pair<vector<long>, double> result = doAction(currentState, chosenAction);
			vector<long> nextState = result.first;
			double rewardGotten = result.second;
			if (nextState.size()==0) {
				*actions[chosenAction]=*actions[chosenAction] + learningRate*(rewardGotten - *actions[chosenAction]);
				break;
			}
			vector<double*> nextActions = possibleActions(nextState);
			long nextChosenAction = 0;
			bestQ = *nextActions[0];
			for (long i = 1; i < nextActions.size(); ++i) {
				if (*nextActions[i] > bestQ) {
					bestQ = *nextActions[i];
					nextChosenAction = i;
				}
			}
			*actions[chosenAction] = *actions[chosenAction] + learningRate*(rewardGotten + devaluation*bestQ - *actions[chosenAction]);
			chosenAction = nextChosenAction;
			actions = nextActions;
			currentState = nextState;
		}
	}
}