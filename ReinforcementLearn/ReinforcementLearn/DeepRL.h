#pragma once
#include<vector>
void RLNewNet(double learnRate, long replaySize, double randChance, double discount);
void DNQFile(double learnRate, long replaySize, double randChance, double discount, std::string file);
void deepRLLearn(std::vector<std::vector<std::vector<double>>> &net, double learnRate, long replaySize, double randChance, double discount);