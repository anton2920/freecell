#include <utility>
#include <algorithm>
#include <ctime>
#include <array>

#include <iostream> // Starting to debug (tnx to Ilya)

#include "game.hpp"

int cell::game::exec() {

    /* Initializing variables */
    sf::Vector2i mouseCoords;
    cell::card *selectedCard = nullptr, *topCard = nullptr;
    bool sorted = true;

    /* Main part */
    // window.show();
    while (this->window->isOpen()) {
        mouseCoords = sf::Mouse::getPosition(*this->window);

        this->draw->drawField();
        if (this->eg->isEndgame()) {
            this->stop();
        }
        this->draw->drawMan(this->state);
        if (this->state == cell::game::GAMESTATE::game) {
            if (!sorted) {
                std::stable_sort(this->table.begin(), this->table.end(), cell::card::compCoords);
                sorted = true;
            }
            this->draw->drawCards(this->table);
        }
        if (this->state != cell::game::GAMESTATE::start) {
            this->draw->drawEndgame(this->eg);
        }

        if (selectedCard != nullptr) {
            cell::card *c;
            if ((c = this->findBottomCard(sf::Vector2f(mouseCoords.x, mouseCoords.y))) != nullptr &&
                    c->getPos() != cell::card::POSITION::endgame && c->getPos() != cell::card::POSITION::safehouse) {
                if (selectedCard->canMove(*c) || (topCard = this->canPowerMove(selectedCard,
                                                                              sf::Vector2i(mouseCoords.x, mouseCoords.y))) != nullptr) {
                    this->window->setMouseCursorVisible(false);
                    this->draw->drawCursor(cell::game::drawSystem::down);
                }
            } else if (this->sh->canPut(sf::Vector2i(mouseCoords.x, mouseCoords.y)) ||
                    this->eg->canPut(*selectedCard, sf::Vector2i(mouseCoords.x, mouseCoords.y))) {
                this->window->setMouseCursorVisible(false);
                this->draw->drawCursor(cell::game::drawSystem::up);
            } else if (this->isFreeColomn(sf::Vector2f(mouseCoords.x, mouseCoords.y))) {
                topCard = this->canPowerMove(selectedCard,sf::Vector2i(mouseCoords.x, mouseCoords.y));
                this->window->setMouseCursorVisible(false);
                this->draw->drawCursor(cell::game::drawSystem::up);
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
                            if ((c = this->findBottomCard(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) !=
                                nullptr &&
                                c->getPos() != cell::card::POSITION::endgame) {
                                std::clog << *c << std::endl << std::flush;
                                if (selectedCard == nullptr) {
                                    selectedCard = c;
                                    cell::game::selectCard(*c);
                                } else {
                                    if (c != selectedCard && c->getPos() != cell::card::POSITION::safehouse) {
                                        this->saveState();
                                        if (c->getPos() == cell::card::POSITION::tabled &&
                                            selectedCard->getPos() == cell::card::POSITION::safehouse) {
                                            this->sh->get(selectedCard->getCoords());
                                            selectedCard->setPos(cell::card::POSITION::tabled);
                                        }
                                        if (selectedCard->move(*c)) {
                                            sorted = false;
                                            cell::game::selectCard(*selectedCard);
                                            selectedCard = nullptr;
                                        } else if (topCard != nullptr) {
                                            this->powerMove(selectedCard, topCard, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                                            sorted = false;
                                            cell::game::selectCard(*selectedCard);
                                            selectedCard = nullptr;
                                        }
                                    } else if (c == selectedCard) {
                                        cell::game::selectCard(*selectedCard);
                                        selectedCard = nullptr;
                                    }
                                }
                            } else if (selectedCard != nullptr) { // not tabled card on another tabled card situations...
                                if (this->sh->canPut(sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) {
                                    this->saveState();
                                    if (selectedCard->getPos() == cell::card::POSITION::safehouse) {
                                        this->sh->get(selectedCard->getCoords());
                                    }
                                    cell::game::selectCard(*selectedCard);
                                    selectedCard->setPos(cell::card::POSITION::safehouse);
                                    this->sh->put(selectedCard, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                                    sorted = false;
                                    selectedCard = nullptr;
                                } else if (this->eg->canPut(*selectedCard,sf::Vector2i(event.mouseButton.x, event.mouseButton.y))) {
                                    this->saveState();
                                    if (selectedCard->getPos() == cell::card::POSITION::safehouse) {
                                        this->sh->get(selectedCard->getCoords());
                                    }
                                    cell::game::selectCard(*selectedCard);
                                    selectedCard->setPos(cell::card::POSITION::endgame);
                                    this->eg->put(selectedCard, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                                    sorted = false;
                                    selectedCard = nullptr;
                                } else if (this->isFreeColomn(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                                    this->saveState();
                                    if (selectedCard->getPos() == cell::card::POSITION::safehouse) {
                                        this->sh->get(selectedCard->getCoords());
                                        selectedCard->setPos(cell::card::POSITION::tabled);
                                    }
                                    if (topCard != nullptr) {
                                        switch (cell::game::askForPowerMove()) {
                                            case one:
                                                selectedCard->moveToFreePos(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                                                break;
                                            case stack:
                                                this->powerMove(selectedCard, topCard, sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                                                break;
                                            case cancel:
                                                break;
                                        }
                                        cell::game::selectCard(*selectedCard);
                                        sorted = false;
                                        selectedCard = nullptr;
                                    } else {
                                        selectedCard->moveToFreePos(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                                        cell::game::selectCard(*selectedCard);
                                        sorted = false;
                                        selectedCard = nullptr;
                                    }
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
                    if (event.key.code == sf::Keyboard::Space && this->state == cell::game::GAMESTATE::start) {
                        this->start(26614);
                        cell::game::drawSystem::initCards(this->table);
                    } else if (event.key.code == sf::Keyboard::F10) {
                        this->loadState();
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
    this->fs = new cell::game::fieldState(this->window);
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
    this->window->setTitle(std::string(TITLE) + std::string(" #") + std::to_string(n));

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
        newCoords.x += 5;
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

cell::card *cell::game::findLowerCard(const cell::card *c) const {

    /* Initializing variables */
    cell::card *ca;

    /* Initializing variables */
    sf::Vector2f findCoords = c->getCoords();
    findCoords.y += cell::game::drawSystem::card_row_overlap_space;
    ca = this->findCard(findCoords);

    /* Returning value */
    return (c->getPos() != cell::card::POSITION::tabled) ? nullptr : ca;
}

cell::card *cell::game::findUpperCard(const cell::card *c) const {

    /* Initializing variables */
    cell::card *ca;

    /* Initializing variables */
    sf::Vector2f findCoords = c->getCoords();
    findCoords.y -= cell::game::drawSystem::card_row_overlap_space;
    ca = this->findCard(findCoords);

    /* Returning value */
    return (c->getPos() != cell::card::POSITION::tabled) ? nullptr : ca;
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

bool cell::game::isFreeColomn(const sf::Vector2f &pos) {

    /* Initializing variables */
    sf::Vector2f findCoords;
    findCoords.x = cell::game::drawSystem::card_row_x;
    findCoords.y = cell::game::drawSystem::card_row_y;

    /* Main part */
    for (int i = 0; i < 8; ++i) {
        if (this->findCard(findCoords) == nullptr) {
            if (pos.x >= findCoords.x && pos.x <= findCoords.x + cell::card::card_w) {
                return true;
            }
        }

        findCoords.x += cell::card::card_w + cell::game::drawSystem::card_row_space;
    }

    /* Returning value */
    return false;
}

cell::card *cell::game::canPowerMove(const card *c, const sf::Vector2i &pos) {

    /* Initializing variables */
    sf::Vector2f toCoords;
    toCoords.x = pos.x;
    toCoords.y = pos.y;
    cell::card *findCard = const_cast<card *>(c), *prevCard = nullptr, *toCard = this->findBottomCard(toCoords);

    /* Main part */
    if (toCard != nullptr) {
        for (int i = 0; i < this->availableSpace(); ++i) {
            prevCard = findCard;
            findCard = this->findUpperCard(prevCard);
            if (findCard == nullptr) {
                break;
            }
            if (!prevCard->canMove(*findCard) || prevCard->canMove(*toCard)) {
                break;
            }
        }
        findCard = prevCard;
        if (findCard != nullptr) {
            if (!findCard->canMove(*toCard)) {
                return nullptr;
            }
        }
    } else {
        for (int i = 0; i < this->availableSpace() / 2; ++i) {
            prevCard = findCard;
            findCard = this->findUpperCard(prevCard);
            if (findCard == nullptr) {
                break;
            }
            if (!prevCard->canMove(*findCard)) {
                findCard = prevCard;
                break;
            }
        }

        if (findCard == c) {
            return nullptr;
        }
    }

    /* Returning value */
    return findCard;
}

int cell::game::availableSpace() {

    /* Initializing variables */
    sf::Vector2f c(cell::game::drawSystem::card_row_x, cell::game::drawSystem::card_row_y);
    int n = 0, m = 0, pow = 1;

    /* Main part */
    for (int i = 0; i < 4; ++i) {
        if (this->sh->isEmpty(i)) {
            ++n;
        }
    }

    for (int i = 0; i < 8; ++i) {
        if (this->isFreeColomn(c)) {
            ++m;
        }
        c.x += cell::card::card_w + cell::game::drawSystem::card_row_space;
    }

    for (int i = 0; i < m; ++i) {
        pow *= 2;
    }

    /* Returning value */
    return (n + 1) * pow;
}

bool cell::game::powerMove(cell::card *c, cell::card *topCard, const sf::Vector2i &pos) {

    /* Initializing variables */
    sf::Vector2f toCoords, prevCoords = topCard->getCoords();
    toCoords.x = pos.x;
    toCoords.y = pos.y;
    cell::card *toCard = this->findBottomCard(toCoords);

    //if (this->isFreeColomn(toCoords)) {
    if (toCard == nullptr) {
        topCard->moveToFreePos(toCoords);
        toCard = topCard;
        prevCoords.y += cell::game::drawSystem::card_row_overlap_space;
        topCard = findCard(prevCoords);
        while (topCard != c) {
            topCard->move(*toCard);
            toCard = topCard;
            prevCoords.y += cell::game::drawSystem::card_row_overlap_space;
            topCard = findCard(prevCoords);
        }

        topCard->move(*toCard);
    } else {
        while (topCard != c) {
            topCard->move(*toCard);
            toCard = topCard;
            prevCoords.y += cell::game::drawSystem::card_row_overlap_space;
            topCard = findCard(prevCoords);
        }

        topCard->move(*toCard);
    }

    /* Returning value */
    return true;
}

cell::game::powerMoveAnswer cell::game::askForPowerMove() {

    /* Initializing variables */
    int ans;

    /* I/O flow */
    std::cout << "You are performing move to the free table space. Available options: " <<
        "\n\t1) Move a single card" <<
        "\n\t2) Perform a POWERMOVE!" <<
        "\n\t3) Cancel" <<
        "\n\nAnswer: ";
    std::cin >> ans;

    /* Returning value */
    return static_cast<cell::game::powerMoveAnswer>(ans);
}

void cell::game::saveState() {

    /* Main part */
    this->fs->table.clear();
    for (auto &i : this->table) {
        this->fs->table.push_back(i);
    }
    this->fs->sh->copyFrom(this->sh);
    this->fs->eg->copyFrom(this->eg);

    this->fs->isUndoAvailable = true;
}

void cell::game::loadState() {

    /* Main part */
    if (this->fs->isUndoAvailable) {
        this->table.clear();
        for (auto &i : this->fs->table) {
            if (i.getSt() == cell::card::STATE::pressed) {
                i.setSt(cell::card::STATE::unpressed);
            }
            this->table.push_back(i);
        }
        this->sh->copyFrom(this->fs->sh);
        this->eg->copyFrom(this->fs->eg);

        this->fs->isUndoAvailable = false;
    }
}
