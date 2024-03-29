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

#ifndef SRC_CARD_HPP
#define SRC_CARD_HPP

/* Including config header file */
#include "FreeCell_config.h"

/* Including other headers */
#if (HAVE_UTILITY == 1)
    #include <utility>
#endif

#if (HAVE_SFML_GRAPHICS_HPP == 1)
    #include <SFML/Graphics.hpp>
#endif

namespace cell {
    class card {
    public:
        enum class VALUE {
            ace = 1,
            two = 2,
            three = 3,
            four = 4,
            five = 5,
            six = 6,
            seven = 7,
            eight = 8,
            nine = 9,
            ten = 10,
            jack = 11,
            queen = 12,
            king = 13,
            blank = 0
        };

        enum class SUIT {
            clubs = 1,
            diamonds = 2,
            hearts = 3,
            spades = 4,
            blank = 0
        };

        enum class POSITION {
            tabled,
            safehouse,
            endgame
        };

        enum class STATE {
            unpressed = 0,
            pressed
        };

        enum card_sizes {
            card_x = 632,
            card_y = 0,
            card_w = 71,
            card_h = 96,
            card_negative_offset = 384
        };
    private:
        std::pair <VALUE, SUIT> type;
        sf::Vector2f coords;
        POSITION p;
        STATE st;
    public:
        explicit card(std::pair<VALUE, SUIT> _type = {VALUE::blank, SUIT::blank},
                sf::Vector2f _coords = sf::Vector2f(0, 0), POSITION _pos = POSITION::tabled, STATE _st = STATE::unpressed);
        card(const card &other);
        ~card() = default;

        bool operator==(const card &other) {
            return this->type == other.type;
        }

        [[nodiscard]] const std::pair<VALUE, SUIT> &getType() const;
        [[nodiscard]] sf::Vector2f getCoords() const;
        [[nodiscard]] POSITION getPos() const;
        [[nodiscard]] STATE getSt() const;

        [[nodiscard]] sf::IntRect getSpriteRect() const;

        void setType(const std::pair<VALUE, SUIT> &_type);
        void setCoords(const sf::Vector2f &_coords);
        void setPos(POSITION _pos);
        void setSt(STATE _st);

        bool isRed();

        bool canMove(card &other);
        bool canMoveEndgame(card &other);

        bool move(card &other);

        void moveToFreePos(const sf::Vector2f &pip);

        [[nodiscard]] static bool compCoords(const card &self, const card &other);

        friend std::ostream &operator<<(std::ostream &stream, const cell::card &c);
    };
}

#endif
