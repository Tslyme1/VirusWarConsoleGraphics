#pragma once
#include "Game.h"
#include "GameController.h"

class Player {
public:
	virtual void makeTurn(Field field, const std::string cmd, char c, short d) = 0;
	void setTurn(Turn t) { my_turn = t; }
	Turn getTurn() { return my_turn; }

	bool canSplit(Field field, short x, short y) {
		for (size_t i = 0; i < 8; ++i) {
			short dx = MOVES[i][0], dy = MOVES[i][1];
			if (x + dx >= 0 && x + dx < Game::WIDTH
				&& y + dy >= 0 && y + dy < Game::HEIGHT) {
				if (field[(y + dy) * Game::WIDTH + x + dx].getState() == Cell::State::BLUE_ALIVE
					&& getTurn() == Turn::BLUE) {
					return true;
				}
				if (field[(y + dy) * Game::WIDTH + x + dx].getState() == Cell::State::RED_ALIVE
					&& getTurn() == Turn::RED) {
					return true;
				}
			}
		}
		return false;
	}


	const short MOVES[8][2] = { { 1, 1}, { 1, 0}, { 1, -1},
								{ 0, 1},		  { 0, -1},
								{-1, 1}, {-1, 0}, {-1, -1} };
protected:
	Turn my_turn;
};

