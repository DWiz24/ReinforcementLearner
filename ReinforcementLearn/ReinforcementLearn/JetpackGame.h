#pragma once
void setUpGameWindow();
std::vector<double> newGame();
std::pair<double, std::vector<double>> doAction(std::vector<double> state, long action);
extern long visibleObstacles;