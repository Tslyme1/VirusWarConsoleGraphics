#pragma once
#include "Game.h"
#include <unordered_map>

#include <string>

enum class Turn { RED, BLUE };

class GameController {
public:
	void run();
	
private:
	bool someoneDied();
	bool allOccupied();
	void countAlives(Field field);
	bool isValidInput(const std::string& s);
	void changeTurn();
	std::string getPossibleMoves(Field field);

	std::unordered_map<Turn, size_t> alive{ {Turn::RED,  1}, {Turn::BLUE, 1} };
	std::unordered_map<Turn, size_t> losses{ {Turn::RED,  0}, {Turn::BLUE, 0} };
	Turn current_turn = Turn::RED;

	const static size_t MAX_TURNS = 2000;
};

