#pragma once
#include <array>

class Cell {
public:
	enum class State { EMPTY, RED_DEAD, BLUE_DEAD, RED_ALIVE, BLUE_ALIVE };
	Cell() : state(Cell::State::EMPTY) {};
	Cell(State s): state(s) {};

	State getState() { return state; }
	void setState(State s) { state = s; }
private:
	State state = State::EMPTY;
};

typedef Cell* Field;

class Game {
public:
	static const size_t WIDTH  = 10;
	static const size_t HEIGHT = 10;

	enum class State { START, RED_TURN, BLUE_TURN, RED_VICTORY, BLUE_VICTORY };

	Game();
	~Game() { delete[] field; };

	void initializeField();
	
	void update(Field field) { this->field = field; };

	Field getField() { return field; }
	void setState(State s) { state = s; }
	State getState() { return state; }

	bool isFinished() { return state == State::RED_VICTORY || state == State::BLUE_VICTORY; }
private:
	State state = State::START;

	Field field;
};

