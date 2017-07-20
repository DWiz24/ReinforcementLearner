#ifndef Learner

#define Learner
#include<vector>
void discreteLearn(std::vector<long>(*startingState)(), std::vector<double*>(*possibleActions)(std::vector<long>),std::pair<std::vector<long>, double>(*doAction)(std::vector<long>, long),long trainingRuns, double learningRate, double devaluation, double randomChance);
#endif
