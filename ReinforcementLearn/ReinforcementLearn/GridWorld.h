#ifndef GridWorld

#include<vector>
std::vector<long> startPos();
std::vector<double*> listMoves(std::vector<long> pos);
std::pair<std::vector<long>, double> move(std::vector<long> pos, long choice);
#define GridWorld
#endif
