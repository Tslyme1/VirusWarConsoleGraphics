#pragma once
#include "Player.h"
#include <stdexcept>

class Human 
    : public Player {
public:
    Human(Team t) { team = t; };
    void makeTurn(Field field, const std::string cmd, char c, short d) {
        short y = c - 'a';
        short x = d;

        auto info = field[y * Game::WIDTH + x].getInfo();

        if (!cmd.compare("pass")) {
            return;
        }
        else if (!cmd.compare("split")) {
            if (info.first == Cell::State::EMPTY
                && haveTeammate(field, x, y)) {
                if (team == Team::BLUE) {
                    field[y * Game::WIDTH + x] = Cell(Cell::State::ALIVE, Team::BLUE);
                }
                else {
                    field[y * Game::WIDTH + x] = Cell(Cell::State::ALIVE, Team::RED);
                }
            }
        }
        else if (!cmd.compare("kill")) {
            if (team == Team::BLUE) {
                if (info.first == Cell::State::ALIVE 
                    && team != info.second
                    && haveTeammate(field, x, y)) {

                    field[y * Game::WIDTH + x].changeState(Cell::State::DEAD);
                }
            }
        }
        else {
            throw std::invalid_argument("No such command");
        }
    }
};

