#pragma once
#include <unordered_map>

class Player;

enum class Team { NONE, BLUE, RED };

class Cell {
public:
	enum class State { EMPTY, DEAD, ALIVE };
	Cell() : state(State::EMPTY), team(Team::NONE) {};
	Cell(State s, Team t): state(s), team(t) {};

	void changeState(State s) { state = s; };
	std::pair<State, Team> getInfo() { return std::pair<State, Team>{state, team}; };

private:
	State state = State::EMPTY;
	Team team = Team::NONE;
};

typedef Cell* Field;

class Game {
public:
	static const size_t WIDTH  = 10;
	static const size_t HEIGHT = 10;
	
	enum class State { START, TURN, VICTORY };

	Game();
	~Game() { delete[] field; };

	void initializeField();
	
	void update(const std::string cmd, char c, short d);

	bool isBot(Team team) { return is_bot[team]; };
	Team getCurrentTeam() { return current_turn; };
	State getState() { return game_state; };
	Field getField() { return field; };

	bool isFinished() { return game_state == State::VICTORY || turn_count >= MAX_TURNS; };
private:
	bool allOccupied();
	bool someoneDied();
	void updateAlives();
	void changeTurn();

	const static size_t MAX_TURNS = 100;
	size_t turn_count = 0;
	
	size_t bots_count;
	
	Team current_turn = Team::RED;
	State game_state = State::START;

	std::unordered_map<Team, Player*> players;
	std::unordered_map<Team, bool> is_bot;
	
	std::unordered_map<Team, size_t> alive;
	std::unordered_map<Team, size_t> losses;

	Field field;
};

