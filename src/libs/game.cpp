#include <utility>
#include <ctime>
#include <array>

#include "game.hpp"

int cell::game::exec() {

    /* Initializing variables */
    sf::Vector2i mouseCoords;
    cell::card *selectedCard = nullptr;

    /* Main part */
    // window.show();
    while (this->window->isOpen()) {
        mouseCoords = sf::Mouse::getPosition(*this->window);

        this->draw->drawField();
        this->draw->drawMan(this->state);
        if (this->state != cell::game::GAMESTATE::start) {
            this->draw->drawCards(this->table);
            this->draw->drawEndgame(this->eg);
        }

        if (selectedCard != nullptr) {
            cell::card *c;

            if ((c = this->findBottomCard(sf::Vector2f(mouseCoords.x, mouseCoords.y)))) {
                if (selectedCard->canMove(*c)) {
                    this->window->setMouseCursorVisible(false);
                    this->draw->drawCursor(cell::game::drawSystem::down);
                }
            }
            this->window->setMouseCursorVisible(true);
        }

        sf::Event event{};
        while (this->window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    this->window->close();
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (this->state == cell::game::GAMESTATE::game) {
                            cell::card *c;
                            if ((c = this->findBottomCard(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))) {
                                if (selectedCard == nullptr) {
                                    selectedCard = c;
                                    cell::game::selectCard(*c);
                                } else {
                                    cell::game::selectCard(*selectedCard);
                                    if (c != selectedCard) {
                                        selectedCard->move(*c);
                                    }
                                    selectedCard = nullptr;
                                }
                            }
                        }
                    } else if (event.mouseButton.button == sf::Mouse::Right) {
                        if (this->state == cell::game::GAMESTATE::game) {
                            cell::card *c;
                            if ((c = this->findCard(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))) {
                                while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                                    this->draw->drawCard(*c);
                                    this->draw->drawMan(this->state);
                                    this->window->display();
                                }
                            }
                        }
                    }
                    break;
                case sf::Event::MouseMoved:
                    break;
                case sf::Event::KeyPressed:
                    if (this->state == cell::game::GAMESTATE::start) {
                        this->start(6585);
                        cell::game::drawSystem::initCards(this->table);
                    } else {
                        this->stop();
                    }
                    break;
                default:
                    break;
            }
        }

        this->window->display();
    }

    delete(this->window);

    /* Returning value */
    return 0;
}

cell::game::game() {

    /* Initializing variables */
    // Creating window and other sh**t...
    this->window = new sf::RenderWindow(sf::VideoMode(632, 446), TITLE, sf::Style::Titlebar | sf::Style::Close);
    this->window->setVerticalSyncEnabled(true);
    this->draw = new cell::game::drawSystem(this->window);
    this->sh = new cell::safehouse(this->window);
    this->eg = new cell::endgame(this->window);
}

cell::game::~game() {

    /* Main part */
    // Destroying renderer, window, and other sh**t...
    delete(draw);
    delete(sh);
    delete(eg);
}

void cell::game::start(int n) {

    /* Initializing variables */
    this->state = cell::game::GAMESTATE::game;
    std::srand(std::time(nullptr));
    this->r.srand(((n != -1) ? n : std::rand()));

    /* Main part */
    this->deal();
}

void cell::game::stop() {

    /* Initializing variables */
    this->state = cell::game::GAMESTATE::win;
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
    for (auto j = values.rbegin(); j != values.rend(); ++j) {
        for (auto i = suits.rbegin(); i != suits.rend(); ++i) {
            this->table.emplace_back(std::pair(*j, *i), sf::Vector2f(0, 0), cell::card::POSITION::tabled);
        }
    }

    for (int i = 0; i < 51; ++i) {
        int j = 51 - this->r.rand() % (52 - i);
        std::swap(this->table[i], this->table[j]);
    }
}

cell::card *cell::game::findCard(const sf::Vector2f &coords) const {

    /* Initializing variables */
    sf::Vector2f ccoords;

    /* Main part */
    for (auto i = this->table.rbegin(); i != this->table.rend(); ++i) {
        ccoords = i->getCoords();
        if (coords.x >= ccoords.x && coords.x <= ccoords.x + cell::card::card_w &&
            coords.y >= ccoords.y && coords.y <= ccoords.y + cell::card::card_h) {
            return (cell::card *) &(*i);
        }
    }

    /* Returning value */
    return nullptr;
}

cell::card *cell::game::findBottomCard(const sf::Vector2f &coords) const {

    /* Initializing variables */
    sf::Vector2f newCoords;
    cell::card *c = this->findCard(coords), *next_c;

    /* Main part */
    if (c != nullptr) {
        newCoords = c->getCoords();
        while (true) {
            newCoords.y += cell::game::drawSystem::card_row_space;
            if ((next_c = this->findCard(newCoords)) != nullptr) {
                c = next_c;
            } else {
                break;
            }
        }
    }

    /* Returning value */
    return c;
}

cell::card *cell::game::selectCard(cell::card &card) {

    /* Main part */
    if (card.getSt() == cell::card::STATE::unpressed) {
        card.setSt(cell::card::STATE::pressed);
    } else {
        card.setSt(cell::card::STATE::unpressed);
    }

    /* Returning value */
    return &card;
}

cell::card *cell::game::selectCard(const sf::Vector2f &coords) {

    /* Initializing variables */
    cell::card *c = this->findBottomCard(coords);

    /* Main part */
    if (c != nullptr) {
        cell::game::selectCard(*c);
    }

    /* Returning value */
    return c;
}
