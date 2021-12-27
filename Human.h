#pragma once
#include "Player.h"
#include <stdexcept>

class Human 
    : public Player {
public:
    Human(Turn turn) { setTurn(turn); };
    void makeTurn(Field field, const std::string cmd, char c, short d) {
        short y = c - 'a';
        short x = d;
        if (!cmd.compare("pass")) {
            return;
        }
        else if (!cmd.compare("split")) {
            if (field[y * Game::WIDTH + x].getState() == Cell::State::EMPTY
                && canSplit(field, x, y)) {
                if (my_turn == Turn::BLUE) {
                    field[y * Game::WIDTH + x] = Cell(Cell::State::BLUE_ALIVE);
                }
                else {
                    field[y * Game::WIDTH + x] = Cell(Cell::State::RED_ALIVE);
                }
            }
        }
        else if (!cmd.compare("kill")) {
            if (my_turn == Turn::BLUE) {
                if (field[y * Game::WIDTH + x].getState() == Cell::State::RED_ALIVE
                    && canSplit(field, x, y)) {
                    field[y * Game::WIDTH + x] = Cell(Cell::State::RED_DEAD);
                }
            }
            if (my_turn == Turn::RED) {
                if (field[y * Game::WIDTH + x].getState() == Cell::State::BLUE_ALIVE
                    && canSplit(field, x, y)) {
                    field[y * Game::WIDTH + x] = Cell(Cell::State::BLUE_DEAD);
                }
            }
        }
        else {
            throw std::invalid_argument("No such command");
        }
    }
};

