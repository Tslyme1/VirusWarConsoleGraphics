#include "Human.h"
#include "Bot.h"
#include "GameController.h"
#include "GameView.h"
#include "Game.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>

std::string GameController::getPossibleMoves()
{
	return "# pass - skip the turn\n# split a 0 - breed to a cell\n# kill b 9 - kill enemy on a cell\n\n> ";
}

bool GameController::isValidInput(const std::string& s) {
	if (!s.compare("")) return false;

	std::stringstream ss(s);

	std::string move, dump;
	char letter;
	short number;

	ss >> move;
	transform(move.begin(), move.end(), move.begin(), std::tolower);

	if (!move.compare("pass")) {
		if (!ss.eof()) { return false; }

		return true;
	}
	else if (!move.compare("kill")) {
		ss >> letter;
		if (letter < 'a' || letter > 'l') { return false; }

		ss >> number;
		if (!ss.eof()) { return false; }

		return true;
	}
	else if (!move.compare("split")) {
		ss >> letter;
		ss >> number;
		
		if (!ss.eof()) { return false; }

		return true;
	}

	return false;
}

void GameController::run() {
	size_t bots_count;

	GameView* gv = new GameView();
	Game* g = new Game();

	while (!g->isFinished()) {
		gv->update(g->getField());

		std::string cmd = "pass";
		char c  = 'a';
		short d = 0;

		if (!g->isBot(g->getCurrentTeam())) {
			std::string line = "";
			bool wrong_input = false;

			do {
				gv->clearText();

				if (wrong_input) {
					std::cout << "Wrong input command\n";
				}
				std::cout << "It's " << (g->getCurrentTeam() == Team::RED ? "Red" : "Blue") << " turn. Possible moves:\n";

				std::cout << getPossibleMoves();

				std::getline(std::cin, line);
				std::cin.clear();

				wrong_input = true;
			} while (!isValidInput(line));

			std::stringstream ss(line);

			ss >> cmd >> c >> d;
		}

		g->update(cmd, c, d);
	}

	gv->update(g->getField());

	switch (g->getCurrentTeam()) {
	case Team::BLUE: {
		std::cout << "Blue Player Wins !!!";
		break;
	}
	case Team::RED: {
		std::cout << "Red Player Wins !!!";
		break;
	}
	case Team::NONE: {
		std::cout << "Draw !!!";
		break;
	}
	default: {
		std::cout << "Wait, what ?";
		break;
	}
	}
}
