/*
   FreeCell — a free Windows freecell clone written on C++ with SFML
   Copyright © Pavlovsky Anton, 2019-2022

   This file is part of FreeCell.

   FreeCell is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   FreeCell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with FreeCell. If not, see <https://www.gnu.org/licenses/>.
   */

#include "safehouse.hpp"

void cell::safehouse::put(cell::card *c, int whichOne) {

    /* Initializing variables */
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

void cell::safehouse::put(card *c, const sf::Vector2i &pos) {

    /* Main part */
    this->put(c, pos.x / cell::card::card_w);
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

void cell::safehouse::copyFrom(const cell::safehouse *other) {

    /* Main part */
    std::copy(other->house.begin(), other->house.end(), this->house.begin());
    std::copy(other->emptyFlags.begin(), other->emptyFlags.end(), this->emptyFlags.begin());
    this->window = other->window;
}

void cell::safehouse::clear() {

    /* Main part */
    this->house = {nullptr, nullptr, nullptr, nullptr};
    this->emptyFlags = {true, true, true, true};
}
