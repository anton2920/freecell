#include "card.hpp"

cell::card::card(const std::pair<VALUE, SUIT> type, const std::pair<int, int> coords, cell::card::POSITION pos)
        : type(type), coords(coords), pos(pos) {}

const std::pair<cell::card::VALUE, cell::card::SUIT> &cell::card::getType() const {

    /* Returning value */
    return type;
}

const std::pair<int, int> &cell::card::getCoords() const {

    /* Returning value */
    return coords;
}

cell::card::POSITION cell::card::getPos() const {

    /* Returning value */
    return pos;
}

void cell::card::setType(const std::pair<VALUE, SUIT> &_type) {

    /* Initializing variables */
    card::type = _type;
}

void cell::card::setCoords(const std::pair<int, int> &_coords) {

    /* Initializing variables */
    card::coords = _coords;
}

void cell::card::setPos(cell::card::POSITION _pos) {

    /* Initializing variables */
    card::pos = _pos;
}

bool cell::card::isRed() {

    /* Returning value */
    return this->type.second == cell::card::SUIT::hearts || this->type.second == cell::card::SUIT::diamonds;
}

bool cell::card::canMove(cell::card &other) {

    /* Main part */
    return this->type.first < other.type.first && (this->isRed() ^ other.isRed());
}

bool cell::card::move(cell::card &other) {

    /* Main part */
    if (this->canMove(other)) {
        // something
    } else {
        return false;
    }

    /* Returning value */
    return true;
}