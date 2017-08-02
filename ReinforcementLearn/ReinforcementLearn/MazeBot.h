#pragma once

#include<vector>
std::vector<long> mazeStartPos();
std::vector<double*> mazeListMoves(std::vector<long> pos);
std::pair<std::vector<long>, double> mazeMove(std::vector<long> pos, long choice);

