#include "stdafx.h"
#include<vector>
#include<SDL.h>
#include<Windows.h>
#undef main

static SDL_Renderer* rend;
static SDL_Window* wind;

long visibleObstacles = 1;
static double ceiling = 500.0;
static double obstacleDensity = 400.0;//lower is more obstacles

static long displayFrequency = 1;
static long numGames = 0;
static long maxGameLength = 4000;
static long currentGameLength = 0;

//state is <y, yVel, timeUntilDownBoost, obstacle1Distance, obstacle1Low, obstacle1High,...>
void setUpGameWindow() {
	SDL_Init(SDL_INIT_EVERYTHING);
	
}
std::vector<double> newGame() {
	numGames++;
	currentGameLength = 0;
	std::vector<double> state;
	state.push_back(ceiling / 2.0); //y
	state.push_back(0.0); //yVel
	state.push_back(0);
	for (long i = 0; i < visibleObstacles; i++) {
		double obstacleBottom = ceiling*(rand() / (double)RAND_MAX)-50;
		double obstacleTop = obstacleBottom + (ceiling - obstacleBottom + 50)*(rand() / (double)RAND_MAX);
		double obstacleDistance = (rand() / (double)RAND_MAX)*obstacleDensity;
		if (i != 0) {
			obstacleDistance += state[state.size() - 3];  //previous obstacle
		}
		state.push_back(obstacleDistance);
		state.push_back(obstacleBottom);
		state.push_back(obstacleTop);
	}

	if (numGames%displayFrequency == 0) {
		wind = SDL_CreateWindow("Jetpack", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			640, 500, SDL_WINDOW_SHOWN);
		rend = SDL_CreateRenderer(wind, -1, 0);
	}
	
	return state;
}

void drawGame(std::vector<double> state) {
	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255); //r,g,b,255
	SDL_RenderClear(rend);
	SDL_Rect agent;
	agent.x = 0;
	agent.y = (ceiling-state[0]) - 6;
	agent.w = 6;
	agent.h = 12;
	SDL_SetRenderDrawColor(rend, 0, 0, 200, 255);
	SDL_RenderFillRect(rend, &agent);
	SDL_SetRenderDrawColor(rend, 200, 0, 0, 255);
	SDL_Rect obstacle;
	obstacle.w = 6;
	for (long i = 3; i < state.size(); i += 3) {
		obstacle.x = state[i] - 3;
		obstacle.y = ceiling - state[i + 2];
		obstacle.h = state[i + 2] - state[i + 1];
		SDL_RenderFillRect(rend, &obstacle);
	}
	SDL_RenderPresent(rend);
	SDL_Delay(70);
}

static double frameRate = 1.0;  //proportional
static double speed = 4.0;
static double gravity = -2.0;
static double jetpackPower = 3.0;
static double downBoostPower = -20.0;
static double downBoostCooldown = 120.0;
//actions:
//0 do nothing
//1 thrust up
//2 downBoost

std::pair<double, std::vector<double>> doAction(std::vector<double> state, long action) {
	if (numGames%displayFrequency == 0) {
		drawGame(state);
	}
	currentGameLength++;
	std::pair<double, std::vector<double>> ret;
	if (currentGameLength >= maxGameLength) {
		ret.first = 0;
		state.clear();
		ret.second = state;
		SDL_DestroyWindow(wind);
		return ret;
	}
	double y = state[0];
	double yVel = state[1];
	double boostCooldown = state[2];

	if (y <= 0 || y >= ceiling) {
		ret.first = -10;
		state.clear();
		ret.second = state;
		SDL_DestroyWindow(wind);
		return ret;
	}

	for (long i = 3; i < 3+3*visibleObstacles; i+=3) { //i is the index of the distance
		state[i] -= speed / frameRate;
		if (state[i] <= 0) {
			if (y >= state[i + 1] && y <= state[i + 2]) { //hit obstacle
				ret.first = -10;
				state.clear();
				ret.second = state;
				SDL_DestroyWindow(wind);
				return ret;
			}
			state.erase(state.begin() + i);
			state.erase(state.begin() + i);
			state.erase(state.begin() + i);
			i -= 3;
			double obstacleBottom = ceiling*(rand() / (double)RAND_MAX) - 50;
			double obstacleTop = obstacleBottom + (ceiling - obstacleBottom + 50)*(rand() / (double)RAND_MAX);
			double obstacleDistance = (rand() / (double)RAND_MAX)*obstacleDensity + state[state.size() - 3];
			state.push_back(obstacleDistance);
			state.push_back(obstacleBottom);
			state.push_back(obstacleTop);
		}
	}
	if (action == 1) {
		yVel += jetpackPower/frameRate;
	}
	else if (action == 2 && boostCooldown <= 0) {
		boostCooldown = downBoostCooldown;
		yVel += downBoostPower;
	}
	boostCooldown -= 1 / frameRate;
	if (boostCooldown < 0) {
		boostCooldown = 0;
	}
	yVel += gravity / frameRate;
	y += yVel / frameRate;
	state[0] = y;
	state[1] = yVel;
	state[2] = boostCooldown;
	ret.first = 1.0;
	ret.second = state;
	return ret;
}
