#include <utility>
#include <ctime>
#include <array>

#include "game.hpp"

int cell::game::exec() {

    /* Main part */
    // window.show();

    /* Returning value */
    return 0;
}

cell::game::game() {

    /* Main part */
    // Creating window and other sh**t...
}

cell::game::~game() {

    /* Main part */
    // Destroying renderer, window, and other sh**t...
}

void cell::game::start(int n) {

    /* Initializing variables */
    this->state = cell::game::GAMESTATE::game;
    std::srand(std::time(nullptr));
    this->r._seed = (n != -1) ? n : std::rand();

    /* Main part */
    // Generating cards and starting to play...

}

void cell::game::stop() {

    /* Initializing variables */
    this->state = cell::game::GAMESTATE::win;

    /* Main part */
    // Drawing the endscreen...
}

void cell::game::deal() {

    /* Initializing variables */
    const std::array<cell::card::VALUE, 13> values = {cell::card::VALUE::ace, cell::card::VALUE::two, cell::card::VALUE::three,
                                         cell::card::VALUE::four, cell::card::VALUE::five, cell::card::VALUE::six,
                                         cell::card::VALUE::seven, cell::card::VALUE::eight, cell::card::VALUE::nine,
                                         cell::card::VALUE::ten, cell::card::VALUE::jack, cell::card::VALUE::queen,
                                         cell::card::VALUE::king};
    const std::array<cell::card::SUIT, 4> suits = {cell::card::SUIT::clubs, cell::card::SUIT::diamonds, cell::card::SUIT::hearts, cell::card::SUIT::spades};

    /* Main part */
    for (auto &i : suits) {
        for (auto &j : values) {
            this->table.emplace_back(std::pair(j, i), std::pair(0, 0), cell::card::POSITION::tabled);
        }
    }

    for (int i = 0; i < 51; ++i) {
        int j = 51 - this->r.rand() % (52 - i);
        std::swap(this->table[i], this->table[j]);
    }
}

