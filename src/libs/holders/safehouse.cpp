#include "safehouse.hpp"

void cell::safehouse::put(cell::card &c) {

    /* Initializing variables */
    sf::Vector2i pos = sf::Mouse::getPosition(*this->window);
    int whichOne = pos.x / cell::card::card_w;
    sf::Vector2f coords;

    /* Main part */
    if (this->emptyFlags[whichOne]) {
        c.setPos(cell::card::POSITION::safehouse);
        coords.x = safehouse_x + cell::card::card_w * whichOne;
        coords.y = safehouse_y;
        c.setCoords(coords);

        this->house[whichOne] = &c;
    }
}

cell::card &cell::safehouse::get() {

    /* Initializing variables */
    sf::Vector2i pos = sf::Mouse::getPosition(*this->window);
    int whichOne = pos.x / cell::card::card_w;

    /* Main part */
    if (this->emptyFlags[whichOne]) {
        this->emptyFlags[whichOne] = false;
        return *this->house[whichOne];
    }
}
