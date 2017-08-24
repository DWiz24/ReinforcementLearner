#include "stdafx.h"
#include <vector>
#include "JetpackGame.h"
#include <random>
#include <iostream>
#include <time.h>
#include<fstream>
#include<string>
#include "EvoSystems.h"

using std::vector;
static long inputSize = visibleObstacles * 3 + 4;
static vector<long> netStructure = {inputSize, 20,20,3}; //number of neurons per hidden/output layer
static long numWeights=0; //calculated later
static long trialsPerAgent = 8;
static long printReward = 20;
static long writeFrequency = 20;


//nets are represented as net[layer-1][neuron in layer][neuron from previous layer]=weight
double* loadFile(std::string file) {
	std::ifstream f("agents\\"+file);
	long weights;
	f >> weights;
	//std::cout << weights << std::endl;
	double* arr = new double[weights];
	for (long i = 0; i < weights; i++) {
		f >> arr[i];
		//std::cout << d << std::endl;
	}
	return arr;
}
vector<double> feedForward(vector<vector<vector<double>>> net, vector<double> input) {
	//input is the activation the the prev layer
	//for (long i = 0; i < input.size(); i++) {
	//	std::cout << input[i] << " ";
	//}
	//std::cout << std::endl;
	for (long layer = 1; layer < netStructure.size(); layer++) {
		vector<double> activations;
		for (long i = 0; i < netStructure[layer]; i++) {
			activations.push_back(0.0);
			for (long j = 0; j < input.size(); j++) {
				activations[i] += input[j] * net[layer-1][i][j];
			}
			if (layer != netStructure.size() - 1 && activations[i] < 0) {
				activations[i] = 0;
			}
		}
		input = activations;
		
	}
	//std::cout << input[0] << " " << input[1] << " " << input[2] << std::endl<<std::endl;
	return input;
}

double runTrial(double* weights) {
	long currentWeight = 0;
	vector<vector<vector<double>>> net(netStructure.size() - 1);
	for (long layer = 1; layer < netStructure.size(); layer++) {
		for (long i = 0; i < netStructure[layer]; i++) {
			net[layer - 1].push_back(vector<double>());
			for (long j = 0; j < netStructure[layer - 1]; j++) {
				net[layer - 1][i].push_back(weights[currentWeight]);
				currentWeight++;
			}
		}
	}

	double totalReward = 0;
	for (long trial = 0; trial < trialsPerAgent; trial++) {
		vector<double> state = newGame();

		while (state.size() != 0) {
			state.push_back(1);
			vector<double> rewards = feedForward(net, state);
			double* probs = new double[rewards.size()];
			double total = 0;
			long bestAction = rewards.size() - 1;
			for (long i = 0; i < rewards.size(); i++) {
				probs[i] = exp(rewards[i]);
				total += probs[i];
				//std::cout << rewards[i] << std::endl;
			}
			double randNum = (rand() /(double) RAND_MAX)*total;
			for (long i = 0; i < rewards.size()-1; i++) {
				if (randNum < probs[i]) {
					bestAction = i;
					break;
				}
				else {
					randNum -= probs[i];
				}
			}
			delete[] probs;
			state.pop_back();
			std::pair<double, vector<double>> result = doAction(state, bestAction);
			//std::cout << bestAction << std::endl;
			totalReward += result.first;
			state = result.second;
		}
	}
	return totalReward;
}

void writeArray(double* net, long numWeights, std::string fileName) {
	std::ofstream f;
	f.open("agents\\" + fileName);
	f << numWeights << std::endl;
	f.precision(15);
	for (long i = 0; i < numWeights; i++) {
		f << net[i] << std::endl;
	}
	f.close();
}


void ESNewNet(long iterations, long population, double stdev, double learnRate) {
	for (long i = 1; i < netStructure.size(); i++) {
		numWeights += netStructure[i] * netStructure[i - 1];
	}

	double* net = new double[numWeights];
	for (long i = 0; i < numWeights; i++) {
		net[i] = (rand() / (double)RAND_MAX) / 25.0;
	}

	ESLearn(iterations, population, stdev, learnRate,net);
}
void ESLearn(long iterations, long population, double stdev, double learnRate, double* net) {
	for (long i = 1; i < netStructure.size(); i++) {
		numWeights += netStructure[i] * netStructure[i - 1];
	}
	double** weightZScores = new double*[population];//by [trial #][weight #]
	for (long i = 0; i < population; i++) {
		weightZScores[i] = new double[numWeights];
	}

	double* rewardTable = new double[population];
	double* currentWeights = new double[numWeights];
	std::default_random_engine rng(time(0));
	std::normal_distribution<double> distribution(0, 1);

	for (long generation = 0; generation < iterations; generation++) {
		double meanReward = 0;
		for (long i = 0; i < population; i++) {
			for (long j = 0; j < numWeights; j++) {
				weightZScores[i][j] = distribution(rng);
				currentWeights[j] = net[j] + stdev*weightZScores[i][j];
			}
			rewardTable[i]=runTrial(currentWeights);
			meanReward += rewardTable[i];
		}
		meanReward /= population;
		if (generation%printReward == 0) {
			std::cout << "Generation " << generation << " average reward " << meanReward << std::endl;
		}
		double rewardStdev = 0;
		for (long i = 0; i < population; i++) {
			rewardStdev += (rewardTable[i]-meanReward)*(rewardTable[i] - meanReward);
		}
		rewardStdev = sqrt(rewardStdev);
		for (long i = 0; i < population; i++) {
			rewardTable[i] = (rewardTable[i] - meanReward) / rewardStdev;
		}
		for (long i = 0; i < numWeights; i++) {
			double slope = 0;
			for (long j = 0; j < population; j++) {
				slope += rewardTable[j] * weightZScores[j][i];
			}
			net[i] += (slope*learnRate) / (population*stdev);
		}
		//std::cout << generation << std::endl;
		if ((generation) % writeFrequency == 0) {
			//std::cout << "writing" << std::endl;
			writeArray(net, numWeights, "ES "+std::to_string(meanReward)+".txt");
		}
	}
	delete[] net;
	delete[] rewardTable;
	delete[] currentWeights;
	for (long i = 0; i < population; i++) {
		delete[] weightZScores[i];
	}
	delete[] weightZScores;
}