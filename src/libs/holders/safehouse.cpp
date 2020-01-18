#include "safehouse.hpp"

void cell::safehouse::put(card *c, const sf::Vector2i &pos) {

    /* Initializing variables */
    int whichOne = pos.x / cell::card::card_w;
    sf::Vector2f coords;

    /* Main part */
    if (this->emptyFlags[whichOne] && whichOne < 4) {
        c->setPos(cell::card::POSITION::safehouse);
        coords.x = safehouse_x + cell::card::card_w * whichOne;
        coords.y = safehouse_y;
        c->setCoords(coords);

        this->emptyFlags[whichOne] = false;
        this->house[whichOne] = c;
    }
}

void cell::safehouse::get(const sf::Vector2f &pos) {

    /* Initializing variables */
    int whichOne = static_cast<int>(pos.x) / cell::card::card_w;

    /* Main part */
    if (!this->emptyFlags[whichOne] && whichOne < 4) {
        this->emptyFlags[whichOne] = true;
    }
}

bool cell::safehouse::isInArea(const sf::Vector2i &pos) {

    /* Returning value */
    return (pos.x >= safehouse_x && pos.x <= safehouse_x + safehouse_w && pos.y >= safehouse_y && pos.y <= safehouse_y + safehouse_h);
}

bool cell::safehouse::canPut(const sf::Vector2i &pos) {

    /* Initializing variables */
    int whichOne = pos.x / cell::card::card_w;

    /* Returning value */
    return isInArea(pos) && this->emptyFlags[whichOne];
}
