#ifndef GridWorld

#include<vector>
std::vector<long> gridStartPos();
std::vector<double*> gridListMoves(std::vector<long> pos);
std::pair<std::vector<long>, double> gridMove(std::vector<long> pos, long choice);
#define GridWorld
#endif
