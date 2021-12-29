#pragma once
#include "Game.h"
#include <string>

class GameController {
public:
	void run();
	
private:
	bool isValidInput(const std::string& s);
	std::string getPossibleMoves();
};

