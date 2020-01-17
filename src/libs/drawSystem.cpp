#include <iostream>

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
        if (pos.x <= cell::safehouse::safehouse_w && pos.y <= cell::safehouse::safehouse_h) {
            man_offset = manl_x;
        } else if (pos.x >= cell::safehouse::safehouse_x + cell::endgame::endgame_x_offset && pos.y <= cell::safehouse::safehouse_h) {
            man_offset = manl_x + manr_offset;
        }
        sprite.setTextureRect(sf::IntRect(man_offset, mans_y, mans_w, mans_h));
    } else {
        sprite.setPosition(sf::Vector2f(manb_x_offset, field_h - manb_h + manb_y_offset));
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
            position.x += cell::card::card_w + card_row_space;
        }
    }
}

void cell::game::drawSystem::drawCard(cell::card &i) {

    /* Main part */
    sf::Sprite sprite(this->texture);
    sprite.setPosition(i.getCoords());
    sprite.setTextureRect(i.getSpriteRect());
    this->window->draw(sprite);
}

void cell::game::drawSystem::drawCards(std::deque<card> &t) {

    /* Main part */
    for (auto &i : t) {
        if (i.getPos() != cell::card::POSITION::endgame) {
            this->drawCard(i);
        }
    }
}

void cell::game::drawSystem::drawEndgame(cell::endgame *endg) {

    /* Main part */
    if (!endg->isEmpty()) {
        for (int i = 0; i < 4; ++i) {
            if (!endg->isEmpty(i)) {
                this->drawCard(const_cast<card &>(endg->getHouse()[i].top()));
            }
        }
    }
}

void cell::game::drawSystem::drawCursor(cursor_type t) {

    /* Initializing variables */
    sf::Sprite sprite(this->texture);
    sf::Vector2i position = sf::Mouse::getPosition(*this->window);

    /* Main part */
    sprite.setPosition(static_cast<float>(position.x - static_cast<float>(((t == up) ? cursor_up_w : cursor_down_w)) / 2.f),
            static_cast<float>(position.y - ((t == up) ? 0 : cursor_down_h)));
    if (t == up) {
        sprite.setTextureRect(sf::IntRect(cursor_up_x, cursor_up_y, cursor_up_w, cursor_up_h));
    } else {
        sprite.setTextureRect(sf::IntRect(cursor_down_x, cursor_down_y, cursor_down_w, cursor_down_h));
    }

    this->window->draw(sprite);
}
