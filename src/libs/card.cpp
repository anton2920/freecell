#include "card.hpp"
#include "game.hpp"

cell::card::card(const std::pair<VALUE, SUIT> type, const sf::Vector2f coords, cell::card::POSITION pos, cell::card::STATE state)
        : type(type), coords(coords), pos(pos), st(state) {}

cell::card::card(const cell::card &other) {

    /* Initializing variables */
    this->type = other.type;
    this->coords = other.coords;
    this->pos = other.pos;
    this->st = other.st;
}

const std::pair<cell::card::VALUE, cell::card::SUIT> &cell::card::getType() const {

    /* Returning value */
    return type;
}

sf::Vector2f cell::card::getCoords() const {

    /* Returning value */
    return coords;
}

cell::card::POSITION cell::card::getPos() const {

    /* Returning value */
    return pos;
}

cell::card::STATE cell::card::getSt() const {

    /* Returning value */
    return st;
}

void cell::card::setType(const std::pair<VALUE, SUIT> &_type) {

    /* Initializing variables */
    card::type = _type;
}

void cell::card::setCoords(const sf::Vector2f &_coords) {

    /* Initializing variables */
    card::coords = _coords;
}

void cell::card::setPos(cell::card::POSITION _pos) {

    /* Initializing variables */
    card::pos = _pos;
}

void cell::card::setSt(cell::card::STATE _st) {

    /* Initializing variables */
    card::st = _st;
}

bool cell::card::isRed() {

    /* Returning value */
    return this->type.second == cell::card::SUIT::hearts || this->type.second == cell::card::SUIT::diamonds;
}

bool cell::card::canMove(cell::card &other) {

    /* Main part */
    return (static_cast<int>(other.type.first) - static_cast<int>(this->type.first) == 1) && (this->isRed() ^ other.isRed());
}

bool cell::card::move(cell::card &other) {

    /* Main part */
    if (this->canMove(other)) {
        this->setCoords(other.getCoords());
        this->coords.y += cell::game::drawSystem::card_row_overlap_space;
    } else {
        return false;
    }

    /* Returning value */
    return true;
}

sf::IntRect cell::card::getSpriteRect() const {

    /* Initializing variables */
    sf::IntRect rect;
    int offset = 0;
    if (this->st == cell::card::STATE::pressed) {
        offset = card_negative_offset;
    }
    rect.left = card_x + card_w * (static_cast<int>(this->getType().first) - 1);
    rect.top = card_y + cell::card::card_h * (static_cast<int>(this->getType().second) - 1) + offset;
    rect.width = card_w;
    rect.height = card_h;

    /* Returning value */
    return rect;
}
