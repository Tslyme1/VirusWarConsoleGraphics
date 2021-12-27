#pragma once
#include "Player.h"

#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>

class Bot
    : public Player {
public:
    Bot(Turn turn) { 
        std::srand(std::time(nullptr));
        setTurn(turn); 
        if (turn == Turn::BLUE) {
            x0 = 9;
            y0 = 9;
        }
    };
    void makeTurn(Field field, const std::string _, char __, short ___) {
        std::string cmd = cmds[std::rand() % cmds.capacity()];

        size_t k = std::rand() % 8;

        short dx = MOVES[k][0], dy = MOVES[k][1];

        short y = y0 + dy;
        short x = x0 + dx;

        if (!cmd.compare("pass")) {
            
        }
        else if (!cmd.compare("split")) {
            if (field[y * Game::WIDTH + x].getState() == Cell::State::EMPTY
                && canSplit(field, x, y)) {
                if (my_turn == Turn::BLUE) {
                    field[y * Game::WIDTH + x] = Cell(Cell::State::BLUE_ALIVE);
                    y0 = y;
                    x0 = x;
                }
                else {
                    field[y * Game::WIDTH + x] = Cell(Cell::State::RED_ALIVE);
                    y0 = y;
                    x0 = x;
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
private:
    short x0 = 0;
    short y0 = 0;

    std::vector<std::string> cmds = {"pass", "kill", "split"};
};

