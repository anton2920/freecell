#include "endgame.hpp"
#include "safehouse.hpp"

void cell::endgame::put(card &c, const sf::Vector2i &pos) {

    /* Initializing variables */
    int whichOne = (pos.x - endgame_x_offset) / cell::card::card_w;
    sf::Vector2f coords;

    /* Main part */
    c.setPos(cell::card::POSITION::endgame);
    coords.x = cell::safehouse::safehouse_x + endgame_x_offset + cell::card::card_w * whichOne;
    coords.y = cell::safehouse::safehouse_y;
    c.setCoords(coords);
    c.setSt(cell::card::STATE::unpressed);

    ++this->counter;
    this->house[whichOne].push(c);
    this->emptyFlags[whichOne] = false;
}

cell::card cell::endgame::get(const sf::Vector2f &pos) {

    /* Initializing variables */
    int whichOne = (pos.x - endgame_x_offset) / cell::card::card_w;
    cell::card c;

    /* Main part */
    if (this->emptyFlags[whichOne]) {
        this->emptyFlags[whichOne] = true;
        c = this->house[whichOne].top();
        this->house[whichOne].pop();
        --this->counter;
        return c;
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
