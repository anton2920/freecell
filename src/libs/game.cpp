#include <iostream>
#include <utility>
#include <ctime>
#include <array>

#include "game.hpp"

cell::game::drawSystem::drawSystem(sf::RenderWindow *window) {

    /* Initializing variables */
    this->window = window;

    /* Main part */
    if (!this->texture.loadFromFile("../files/assets.png")) {
        std::cerr << "Couldn't load texture file!\n";
        return;
    }
}

cell::game::drawSystem::drawSystem(sf::RenderWindow *window, const sf::Texture &texture)
        : window(window), texture(texture) {}

void cell::game::drawSystem::drawField() {

    /* Initializing variables */
    sf::Sprite sprite(this->texture);
    sprite.setTextureRect(sf::IntRect(field_x, field_y, field_w, field_h));

    /* Main part */
    this->window->draw(sprite);
}

void cell::game::drawSystem::drawMan(cell::game::GAMESTATE st) {

    /* Initializing variables */
    sf::Sprite sprite(this->texture);
    sf::Vector2i pos = sf::Mouse::getPosition(*this->window);
    static int man_offset = manl_x;

    /* Main part */
    if (st != cell::game::GAMESTATE::win) {
        sprite.setPosition(sf::Vector2f(manc_x, manc_y));
        if (pos.x <= safehouse_w && pos.y <= safehouse_h) {
            man_offset = manl_x;
        } else if (pos.x >= safehouse_w + endgame_x_offset && pos.y <= safehouse_h) {
            man_offset = manl_x + manr_offset;
        }
        sprite.setTextureRect(sf::IntRect(man_offset, mans_y, mans_w, mans_h));
    } else {
        sprite.setPosition(sf::Vector2f(manb_x_offset, field_h - manb_h));
        sprite.setTextureRect(sf::IntRect(manb_x, manb_y, manb_w, manb_h));
    }

    this->window->draw(sprite);
}

void cell::game::drawSystem::initCards(std::deque<card> &t) {

    /* Initializing variables */
    sf::Vector2f position(card_row_x, card_row_y);
    int count = 0;

    /* Main part */
    for (auto &i : t) {
        i.setCoords(position);
        if (count++ % 8 == 7) {
            position.x = card_row_x;
            position.y += card_row_overlap_space;
        } else {
            position.x += card_w + card_row_space;
        }
    }
}

void cell::game::drawSystem::drawCards(std::deque<card> &t) {

    /* Initializing variables */
    sf::IntRect rect;

    /* Main part */
    for (auto &i : t) {
        sf::Sprite sprite(this->texture);
        sprite.setPosition(i.getCoords());
        rect.left = card_x + card_w * (static_cast<int>(i.getType().first) - 1);
        rect.top = card_y + card_h * (static_cast<int>(i.getType().second) - 1);
        rect.width = card_w;
        rect.height = card_h;
        sprite.setTextureRect(rect);
        this->window->draw(sprite);
    }
}

int cell::game::exec() {

    /* Main part */
    // window.show();
    while (this->window->isOpen()) {
        sf::Event event{};
        while (this->window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                this->window->close();

            if (event.type == sf::Event::KeyPressed) {
                if (this->state == cell::game::GAMESTATE::start) {
                    this->start(6585);
                    this->draw->initCards(this->table);
                } else {
                    this->stop();
                }
            }
        }

//        this->window->clear(sf::Color::Black);
        this->draw->drawField();
        this->draw->drawMan(this->state);
        if (this->state != cell::game::GAMESTATE::start) {
            this->draw->drawCards(this->table);
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
}

cell::game::~game() {

    /* Main part */
    // Destroying renderer, window, and other sh**t...
    delete(draw);
}

void cell::game::start(int n) {

    /* Initializing variables */
    this->state = cell::game::GAMESTATE::game;
    std::srand(std::time(nullptr));
    this->r._seed = (n != -1) ? n : std::rand();

    /* Main part */
    // Generating cards and starting to play...
    this->deal();
}

void cell::game::stop() {

    /* Initializing variables */
    this->state = cell::game::GAMESTATE::win;

    /* Main part */
    // Drawing the endscreen...
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
