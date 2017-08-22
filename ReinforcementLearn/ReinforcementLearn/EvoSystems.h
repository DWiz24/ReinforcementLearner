#pragma once
void ESNewNet(long iterations, long population, double stdev, double learnRate);
void ESLearn(long iterations, long population, double stdev, double learnRate, double* initial);
double* loadFile(std::string fileName);