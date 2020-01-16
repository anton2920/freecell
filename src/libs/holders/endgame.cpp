#include "endgame.hpp"
#include "safehouse.hpp"

void cell::endgame::put(cell::card &c) {

    /* Initializing variables */
    sf::Vector2i pos = sf::Mouse::getPosition(*this->window);
    int whichOne = pos.x / cell::card::card_w;
    sf::Vector2f coords;

    /* Main part */
    if (this->emptyFlags[whichOne]) {
        c.setPos(cell::card::POSITION::endgame);
        coords.x = cell::safehouse::safehouse_x + endgame_x_offset + cell::card::card_w * whichOne;
        coords.y = cell::safehouse::safehouse_y;
        c.setCoords(coords);

        ++this->counter;
        this->house[whichOne].push(&c);
        this->emptyFlags[whichOne] = true;
    }
}

cell::card &cell::endgame::get() {

    /* Initializing variables */
    sf::Vector2i pos = sf::Mouse::getPosition(*this->window);
    int whichOne = pos.x / cell::card::card_w;
    cell::card *c;

    /* Main part */
    if (this->emptyFlags[whichOne]) {
        this->emptyFlags[whichOne] = true;
        c = this->house[whichOne].top();
        this->house[whichOne].pop();
        --this->counter;
        return *c;
    }
}

bool cell::endgame::isEndgame() {

    /* Returning value */
    return this->counter == 52;
}

const std::array<std::stack<cell::card *>, 4> &cell::endgame::getHouse() const {

    /* Returning value */
    return house;
}
