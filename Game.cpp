#include "Game.h"
#include "Human.h"
#include "Bot.h"

#include <iostream>
#include <ctime>
#include <cstdlib>

Game::Game()
: field(new Cell[WIDTH * HEIGHT]), 
  losses({ {Team::RED,  0},		{Team::BLUE, 0} }),
  alive( { {Team::RED,  1},		{Team::BLUE, 1} }),
  is_bot({ {Team::RED,  false}, {Team::BLUE, false} }) {
	initializeField();

	std::srand(std::time(nullptr));

	std::cout << "How many bots, whould you like to have in a game? (0/1/2)\n";
	std::cin >> bots_count;
	bots_count %= 3;

	std::cout << "Game will start with " << bots_count << " bots. Type anything to start\n";
	std::cin.ignore(INT_MAX, '\n'); 
	std::cin.clear();
	std::cin.get();

	players = { {Team::RED, nullptr },
			{ Team::BLUE, nullptr } };

	Team team_rnd = (std::rand() % 2) ? Team::BLUE : Team::RED;
	Team team_enm = team_rnd == Team::BLUE ? Team::RED : Team::BLUE;
	switch (bots_count) {
	case 0: {
		players[team_rnd] = new Human(team_rnd);
		players[team_enm] = new Human(team_enm);
		break;
	}
	case 1: {
		is_bot[team_rnd] = true;
		players[team_rnd] = new Bot(team_rnd);
		players[team_enm] = new Human(team_enm);
		break;
	}
	case 2: {
		is_bot[team_rnd] = true;
		is_bot[team_enm] = true;
		players[team_rnd] = new Bot(team_rnd);
		players[team_enm] = new Bot(team_enm);
		break;
	}
	default:
		break;
	}
}

void Game::initializeField() {
	field[0] = Cell(Cell::State::ALIVE, Team::RED);
	field[HEIGHT * WIDTH - 1] = Cell(Cell::State::ALIVE, Team::BLUE);
};

void Game::update(const std::string cmd, char c, short d) {
	turn_count += 1;

	Player* current_player = players[current_turn];

	current_player->makeTurn(this->field, cmd, c, d);

	updateAlives();
	if (someoneDied()) {
		game_state = State::VICTORY;
		return;
	}
	if (allOccupied()) {
		if (alive[Team::RED] < alive[Team::BLUE]) {
			changeTurn();
		}
		else if(alive[Team::RED] > alive[Team::BLUE]) {
			if (losses[Team::RED] > losses[Team::BLUE]) {
				changeTurn();
			}
			else {
				current_turn = Team::NONE;
			}
		}
		else {
			current_turn = Team::NONE;
		}
		game_state = State::VICTORY;
		return;
	}

	changeTurn();
}

void Game::changeTurn() {
	current_turn = current_turn == Team::BLUE ? Team::RED : Team::BLUE;
}

void Game::updateAlives() {
	std::unordered_map<Team, size_t> cnt{ {Team::RED, 0}, {Team::BLUE, 0} };
	for (size_t i = 0; i < Game::HEIGHT * Game::WIDTH; ++i) {
		auto info = field[i].getInfo();
		if (info.first == Cell::State::ALIVE) {
			cnt[info.second]    += 1;
			losses[info.second] += 1;
		}
	}
	alive[Team::RED]  = cnt[Team::RED];
	alive[Team::BLUE] = cnt[Team::BLUE];
}

bool Game::allOccupied() {
	if (alive[Team::BLUE]
		+ alive[Team::RED]
		+ losses[Team::BLUE]
		+ losses[Team::RED] == Game::HEIGHT * Game::WIDTH)
		return true;
	return false;
}

bool Game::someoneDied() {
	return alive[Team::BLUE] == 0 || alive[Team::RED] == 0;
}
