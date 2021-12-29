#pragma once
#include "Player.h"

#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <string>

class Bot
    : public Player {
public:
    Bot(Team t) { 
        
        team = t; 
        if (t == Team::BLUE) {
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
private:
    short x0 = 0;
    short y0 = 0;

    std::vector<std::string> cmds = {"kill", "split"};
};

