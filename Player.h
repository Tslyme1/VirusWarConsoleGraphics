#pragma once
#include "Game.h"

class Player {
public:
	virtual void makeTurn(Field field, const std::string cmd, char c, short d) = 0;
	Team getTeam() { return team; }

	bool haveTeammate(Field field, short x, short y) {
		for (size_t i = 0; i < 8; ++i) {
			short dx = MOVES[i][0], dy = MOVES[i][1];
			if (x + dx >= 0 && x + dx < Game::WIDTH
				&& y + dy >= 0 && y + dy < Game::HEIGHT) {
				
				auto info = field[(y + dy) * Game::WIDTH + x + dx].getInfo();

				if (info.first == Cell::State::ALIVE
					&& info.second == team) {
					return true;
				}
			}
		}
		return false;
	}

	const short MOVES[8][2] = { 
		{ 1,  1}, 
		{ 1,  0}, 
		{ 1, -1},						
		{ 0,  1},
		{ 0, -1},
		{-1,  1}, 
		{-1,  0}, 
		{-1, -1} };

protected:
	Team team;
};

