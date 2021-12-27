#include "Game.h"

Game::Game()
: field(new Cell[WIDTH * HEIGHT]) {
	initializeField();
}

void Game::initializeField() {
	field[0] = Cell(Cell::State::RED_ALIVE);
	field[HEIGHT * WIDTH - 1] = Cell(Cell::State::BLUE_ALIVE);
};

