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

std::string GameController::getPossibleMoves(Field field)
{
	return "# pass - skip the turn\n# split a 0 - breed to a cell\n# kill b 9 - kill enemy on a cell\n\n> ";
}

void GameController::changeTurn() {
	if (current_turn == Turn::RED) {
		current_turn = Turn::BLUE;
	} else {
		current_turn = Turn::RED;
	}
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
		if (!ss.eof()) {
			return false;
		}

		return true;
	}
	else if (!move.compare("kill")) {
		ss >> letter;
		
		if (letter < 'a' || letter > 'l') {
			return false;
		}

		ss >> number;

		if (!ss.eof()) {
			return false;
		}

		return true;
	}
	else if (!move.compare("split")) {
		ss >> letter;
		
		ss >> number;
		
		if (!ss.eof()) {
			return false;
		}

		return true;
	}


	return false;
}

void GameController::countAlives(Field field) {
	int red_cnt = 0, blue_cnt = 0;
	for (size_t i = 0; i < Game::HEIGHT * Game::WIDTH; ++i) {
		if (field[i].getState() == Cell::State::BLUE_ALIVE) {
			blue_cnt += 1;
			losses[Turn::BLUE] += 1;
		}
		if (field[i].getState() == Cell::State::RED_ALIVE) {
			red_cnt += 1;
			losses[Turn::RED] += 1;
		}
	}
	alive[Turn::RED] = red_cnt;
	alive[Turn::BLUE] = blue_cnt;
}

bool GameController::allOccupied() {
	if (alive[Turn::BLUE]
		+ alive[Turn::RED]
		+ losses[Turn::BLUE]
		+ losses[Turn::RED] == Game::HEIGHT * Game::WIDTH)
		return true;
	return false;
}

bool GameController::someoneDied() {
	return alive[Turn::BLUE] == 0 || alive[Turn::RED] == 0;
}

void GameController::run() {
	size_t bots_count;

	GameView* gv = new GameView();
	Game* g = new Game();

	std::cout << "How many bots, whould you like to have in a game? (0/1/2)\n";
	std::cin >> bots_count;

	std::cout << "Game will start with " << bots_count << " bots. Type anything to start\n";
	std::cin.ignore(INT_MAX, '\n');
	std::cin.clear();
	std::cin.get();

	std::unordered_map<Turn, Player*> players{
		{Turn::RED,  (bots_count>0?(Player*)(new Bot(Turn::RED)):new Human(Turn::RED) ) },
		{Turn::BLUE, (bots_count>1?(Player*)(new Bot(Turn::BLUE)):new Human(Turn::BLUE) ) } };

	g->setState(Game::State::RED_TURN);

	std::string line = "";

	size_t tick = 0;
	while (tick < MAX_TURNS && !g->isFinished()) {
		++tick;
		bool wrong_input = false;

		Field field = g->getField();

		Player* cur_player = players[current_turn];

		if (bots_count > 0 && current_turn == Turn::RED) {
			cur_player->makeTurn(field, "", 'd', 4);
			g->update(field);
			gv->update(field);
			changeTurn();
			countAlives(field);
			if (someoneDied()) {
				if (alive[Turn::RED] == 0) {
					g->setState(Game::State::BLUE_VICTORY);
				}
				if (alive[Turn::BLUE] == 0) {
					g->setState(Game::State::RED_VICTORY);
				}
			}
			continue;
		}
		if (bots_count > 1 && current_turn == Turn::BLUE) {
			cur_player->makeTurn(field, "", 'd', 4);
			g->update(field);
			gv->update(field);
			changeTurn();
			countAlives(field);
			if (someoneDied()) {
				if (alive[Turn::RED] == 0) {
					g->setState(Game::State::BLUE_VICTORY);
				}
				if (alive[Turn::BLUE] == 0) {
					g->setState(Game::State::RED_VICTORY);
				}
			}
			continue;
		}

		do {
			gv->update(field);
			
			if (wrong_input) {
				std::cout << "Wrong input command\n";
			}
			std::cout << "It's " << (current_turn == Turn::RED ? "Red" : "Blue") << " turn. Possible moves:\n";
			
			std::cout << getPossibleMoves(field);
			
			std::getline(std::cin, line);
			std::cin.clear();

			wrong_input = true;
		} while (!isValidInput(line));

		std::stringstream ss(line);
		std::string cmd;
		char c;
		short d;
		
		ss >> cmd >> c >> d;

		cur_player->makeTurn(field, cmd, c, d);

		line = "";

		g->update(field);
		gv->update(field);
		changeTurn();
		countAlives(field);
		if (someoneDied()) {
			if (alive[Turn::RED] == 0) {
				g->setState(Game::State::BLUE_VICTORY);
			}
			if (alive[Turn::BLUE] == 0) {
				g->setState(Game::State::RED_VICTORY);
			}
		}
	}

	gv->update(g->getField());

	switch (g->getState()) {
	case Game::State::BLUE_VICTORY: {
		std::cout << "Blue Player Wins !!!";
		break;
	}
	case Game::State::RED_VICTORY: {
		std::cout << "Red Player Wins !!!";
		break;
	}
	default: {
		if (alive[Turn::RED] > alive[Turn::BLUE]) {
			std::cout << "Red Player Wins !!!";
		}
		else if (alive[Turn::RED] < alive[Turn::BLUE]) {
			std::cout << "Blue Player Wins !!!";
		}
		else {
			if (losses[Turn::RED] < losses[Turn::BLUE]) {
				std::cout << "Red Player Wins !!!";
			}
			else if (losses[Turn::RED] > losses[Turn::BLUE]) {
				std::cout << "Blue Player Wins !!!";
			}
			else {
				std::cout << "Draw !!!";
			}
		}
		break;
	}

	}
}
