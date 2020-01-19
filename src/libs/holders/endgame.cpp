#include "endgame.hpp"
#include "safehouse.hpp"

void cell::endgame::put(cell::card *c, int whichOne) {

    /* Initializing variables */
    sf::Vector2f coords;

    /* Main part */
    if (whichOne < 4) {
        c->setPos(cell::card::POSITION::endgame);
        coords.x = cell::safehouse::safehouse_x + endgame_x_offset + cell::card::card_w * whichOne;
        coords.y = cell::safehouse::safehouse_y;
        c->setCoords(coords);
        c->setSt(cell::card::STATE::unpressed);

        ++this->counter;
        this->house[whichOne].push(*c);
        this->emptyFlags[whichOne] = false;
    }
}

void cell::endgame::put(card *c, const sf::Vector2i &pos) {

    /* Main part */
    this->put(c, (pos.x - endgame_x_offset) / cell::card::card_w);
}

void cell::endgame::get(const sf::Vector2f &pos) {

    /* Initializing variables */
    int whichOne = (static_cast<int>(pos.x) - endgame_x_offset) / cell::card::card_w;

    /* Main part */
    if (this->emptyFlags[whichOne]) {
        this->emptyFlags[whichOne] = true;
        this->house[whichOne].pop();
        --this->counter;
    }
}

bool cell::endgame::isEndgame() {

    /* Returning value */
    return this->counter == 52;
}

const std::array<std::stack<cell::card>, 4> &cell::endgame::getHouse() const {

    /* Returning value */
    return house;
}

bool cell::endgame::canPut(const sf::Vector2i &pos) {

    /* Returning value */
    return false;
}

bool cell::endgame::isInArea(const sf::Vector2i &pos) {

    /* Returning value */
    return (pos.x >= cell::safehouse::safehouse_x + endgame_x_offset &&
            pos.x <= cell::safehouse::safehouse_x + endgame_x_offset + cell::safehouse::safehouse_w &&
            pos.y >= cell::safehouse::safehouse_y &&
            pos.y <= cell::safehouse::safehouse_y + cell::safehouse::safehouse_h);
}

bool cell::endgame::canPut(card &c, const sf::Vector2i &pos) {

    /* Initializing variables */
    int whichOne = (pos.x - endgame_x_offset) / cell::card::card_w;

    /* Main part */
    if (this->isInArea(pos)) {
        if (this->emptyFlags[whichOne]) {
            if (c.getType().first == cell::card::VALUE::ace) {
                return true;
            }
        } else if (c.canMoveEndgame(reinterpret_cast<card &>(this->house[whichOne].top()))) {
            return true;
        }
    }

    /* Returning value */
    return false;
}

void cell::endgame::copyFrom(const cell::endgame *other) {

    /* Main part */
    std::copy(other->house.begin(), other->house.end(), this->house.begin());
    std::copy(other->emptyFlags.begin(), other->emptyFlags.end(), this->emptyFlags.begin());
    this->window = other->window;
    this->counter = other->counter;
}

void cell::endgame::clear() {

    /* Main part */
    for (auto &i : this->house) {
        while (!i.empty()) {
            i.pop();
        }
    }
    this->emptyFlags = {true, true, true, true};
    this->counter = 0;
}

int cell::endgame::getLeft() {

    /* Returning value */
    return 52 - this->counter;
}
