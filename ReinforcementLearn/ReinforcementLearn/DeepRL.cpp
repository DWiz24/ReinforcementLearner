#include "stdafx.h"
#include <vector>
#include "JetpackGame.h"
#include <random>
#include <iostream>
#include<fstream>
#include<string>

using std::vector;

struct Experience {
	vector<double> state;
	long action;
	double reward;
} typedef Experience;

static long writeFreq = 1000;
static long inputSize = visibleObstacles * 3 + 4;
static vector<long> netStructure = { inputSize, 20,20,3 }; //number of neurons per hidden/output layer
static const long replaySize;
static Experience* ER;
static long ERSlotsFilled = 0;
static long ERNextReplace=0;


vector<double> feedForward(vector<double> input, vector<vector<vector<double>>> &net) {
	for (long layer = 1; layer < netStructure.size(); layer++) {
		vector<double> activations(netStructure[layer]);
		for (long i = 0; i < netStructure[layer]; i++) {
			activations[i] = 0;
			for (long j = 0; j < input.size(); j++) {
				activations[i] += input[j] * net[layer][i][j];
			}
			if (layer != netStructure.size() - 1 && activations[i] < 0) {
				activations[i] = 0;
			}
		}
		input = activations;
	}
	return input;
}
long chooseAction(vector<double> input, vector<vector<vector<double>>> &net, double randChance) {
	if ((rand() / (double)RAND_MAX) < randChance) {
		return rand() % netStructure[netStructure.size()-1];
	} else {
	
		
		vector<double> rewards =feedForward(input, net);
		long bestAction = 0;
		for (long i = 1; i < rewards.size(); i++) {
			if (rewards[i] > rewards[bestAction]) {
				bestAction = i;
			}
		}
		//std::cout << rewards[1]-rewards[0] << std::endl;
		return bestAction;
	}
}

void updateNet(Experience e1, Experience e2, double learnRate, vector<vector<vector<double>>> &net, double discount) {
	vector<double> qNextState = feedForward(e2.state, net);
	long bestA = 0;
	for (long i = 1; i < qNextState.size(); i++) {
		if (qNextState[bestA] < qNextState[i]) {
			bestA = i;
		}
	}
	long zeroes = 0;
	double target = e1.reward + discount*bestA;
	vector<vector<double>> activations; //actual activation of each node (after relu)
	vector<vector<double>> dNode;  //derivative of error with respect to the sum of inputs*weights to a node
	for (long i = 0; i < netStructure.size(); i++) {
		dNode.push_back(vector<double>(netStructure[i],0.0));
		activations.push_back(vector<double>(netStructure[i], 0.0));
	}
	long outputLayer = netStructure.size() - 1;
	activations[0] = e1.state;
	for (long layer = 1; layer < netStructure.size(); layer++) {
		for (long i = 0; i < netStructure[layer]; i++) {
			for (long j = 0; j < netStructure[layer - 1]; j++) {
				activations[layer][i] += activations[layer-1][j] * net[layer][i][j];
			}
			if (activations[layer][i] < 0 && outputLayer!=layer) { //don't relu the outputs
				activations[layer][i] = 0;
				//zeroes++;
			}
		}
	}
	dNode[outputLayer][e1.action] = activations[outputLayer][e1.action] - target; //others are initialized to zero
	for (long layer = outputLayer; layer > 0; layer--) {
		for (long i = 0; i < netStructure[layer]; i++) {
			for (long j = 0; j < netStructure[layer - 1]; j++) {
				if (activations[layer-1][j]>0) {
					dNode[layer - 1][j] += net[layer][i][j] * dNode[layer][i];
					//std::cout << dNode[layer - 1][j] << std::endl;
				}
				double dwij = dNode[layer][i] * activations[layer - 1][j];
				net[layer][i][j] -= dwij*learnRate;
				//std::cout << dwij << std::endl;
			}
		}
	}
	//std::cout << "Initial error "<< dNode[outputLayer][e1.action] << std::endl;
	//std::cout << "After error " << feedForward(e1.state,net)[e1.action]-target << std::endl;
}

void writeArray(vector<vector<vector<double>>> &net, std::string fileName) {
	std::ofstream f;
	f.open("agents\\" + fileName);
	f.precision(15);
	for (long i = 1; i < net.size(); i++) {
		for (long j = 0; j < net[i].size(); j++) {
			for (long k = 0; k < net[i][j].size(); k++) {
				f << net[i][j][k] << std::endl;
			}
		}
	}
	f.close();
}

static const long scoreBufferSize = 500;

void deepRLLearn(vector<vector<vector<double>>> &net, double learnRate, long replaySize, double randChance, double discount) {
	ER = new Experience[replaySize];
	long game = 0;
	long scores[scoreBufferSize];
	for (long i = 0; i < scoreBufferSize; i++) {
		scores[i] = 0;
	}
	long scorePos = 0;
	long totalScore = 0;
	while (true) {
		long score = 0;
		vector<double> state = newGame();
		state.push_back(1.0);
		game++;
		if (game%writeFreq == 0) {
			writeArray(net, "DQN.txt");
		}
		while (state.size() != 1) {
			score++;
			ER[ERNextReplace].state = state;
			long action = chooseAction(state, net, randChance);
			ER[ERNextReplace].action = action;
			state.pop_back();
			std::pair<double, vector<double>> result = doAction(state, action);
			ER[ERNextReplace].reward = result.first;
			state = result.second;
			state.push_back(1.0);
			ERNextReplace++;
			ERSlotsFilled = ERNextReplace > ERSlotsFilled ? ERNextReplace : ERSlotsFilled; //max
			ERNextReplace %= replaySize;

			//pick an experience to replay
			if (ERSlotsFilled > 2) {
				long toReplay = 0;
				while (true) {
					toReplay = rand() % ERSlotsFilled;
					if ((toReplay + 1) % replaySize != ERNextReplace) {
						break;
					}
				}
				updateNet(ER[toReplay], ER[(toReplay + 1) % replaySize], learnRate, net, discount);
				
			}
			
		}
		totalScore -= scores[scorePos];
		totalScore += score;
		scores[scorePos] = score;
		scorePos = (scorePos + 1) % scoreBufferSize;
		if (game%scoreBufferSize == 0) {
			std::cout << totalScore / (double)scoreBufferSize << std::endl;
		}
	}

	delete[] ER;
}

void DNQFile(double learnRate, long replaySize, double randChance, double discount, std::string file) {
	std::ifstream f;
	f.open("agents\\" + file);
	vector<vector<vector<double>>> net(netStructure.size());
	for (long layer = 1; layer < netStructure.size(); layer++) {
		for (long i = 0; i < netStructure[layer]; i++) {
			net[layer].push_back(vector<double>(netStructure[layer-1]));
			for (long j = 0; j < netStructure[layer - 1]; j++) {
				f>>net[layer][i][j];
			}
		}
	}
	deepRLLearn(net, learnRate, replaySize, randChance, discount);
}
void RLNewNet(double learnRate, long replaySize, double randChance, double discount) {
	vector<vector<vector<double>>> net(netStructure.size());
	for (long layer = 1; layer < netStructure.size(); layer++) {
		for (long i = 0; i < netStructure[layer]; i++) {
			net[layer].push_back(vector<double>());
			for (long j = 0; j < netStructure[layer - 1]; j++) {
				net[layer][i].push_back((rand() / (double)RAND_MAX)*0.0001);
			}
		}
	}
	deepRLLearn(net, learnRate, replaySize, randChance, discount);
}
