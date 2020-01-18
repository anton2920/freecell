#include "card.hpp"
#include "game.hpp"

cell::card::card(const std::pair<VALUE, SUIT> type, const sf::Vector2f coords, cell::card::POSITION pos, cell::card::STATE state)
        : type(type), coords(coords), p(pos), st(state) {}

cell::card::card(const cell::card &other) {

    /* Initializing variables */
    this->type = other.type;
    this->coords = other.coords;
    this->p = other.p;
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
    return p;
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
    card::p = _pos;
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

bool cell::card::compCoords(const cell::card &self, const cell::card &other) {

    /* Returning value */
    return self.coords.y < other.coords.y;
}

bool cell::card::canMoveEndgame(cell::card &other) {

    /* Returning value */
    return (static_cast<int>(this->type.first) - static_cast<int>(other.type.first) == 1) &&
            (this->type.second == other.type.second);
}

void cell::card::moveToFreePos(const sf::Vector2i &pip) {

    /* Initializing variables */
    sf::Vector2f findCoords;
    findCoords.x = cell::game::drawSystem::card_row_x;
    findCoords.y = cell::game::drawSystem::card_row_y;

    /* Main part */
    for (int i = 0; i < 8; ++i) {
        if (pip.x >= findCoords.x && pip.x <= findCoords.x + cell::card::card_w) {
            this->setCoords(findCoords);
            break;
        }

        findCoords.x += cell::card::card_w + cell::game::drawSystem::card_row_space;
    }
}

std::ostream &cell::operator<<(std::ostream &stream, const cell::card &c) {

    /* I/O flow */
    stream << "{ { " << static_cast<int>(c.type.first) << ", " << static_cast<int>(c.type.second) << " }, " <<
           "{ " << c.coords.x << ", " << c.coords.y << " }, " <<
           static_cast<int>(c.p) << ", " << static_cast<int>(c.st) << " }";

    /* Returning value */
    return stream;
}
