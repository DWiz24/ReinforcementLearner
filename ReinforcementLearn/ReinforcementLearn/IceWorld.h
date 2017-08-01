#ifndef IceWorld

#include<vector>
std::vector<long> iceStartPos();
std::vector<double*> iceListMoves(std::vector<long> pos);
std::pair<std::vector<long>, double> iceMove(std::vector<long> pos, long choice);

#define IceWorld
#endif
