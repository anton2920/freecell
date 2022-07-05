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

#ifndef SRC_SAFEHOUSE_HPP
#define SRC_SAFEHOUSE_HPP

#include "placeholder.hpp"

namespace cell {
    class safehouse : public placeholder {
    private:
        std::array<card *, 4> house{};
    public:
        enum safehouse_sizes {
            safehouse_x = 0,
            safehouse_y = 20 + 0,
            safehouse_w = 284,
            safehouse_h = 96
        };

        explicit safehouse(sf::RenderWindow *window) : placeholder(window) {};
        ~safehouse() = default;

        void put(card *c, const sf::Vector2i &pos) override;
        void put(card *c, int whichOne) override;
        void get(const sf::Vector2f &pos) override;

        bool isInArea(const sf::Vector2i &pos) override;
        bool canPut(const sf::Vector2i &pos) override;

        void copyFrom(const safehouse *other);

        void clear() override;
    };
}

#endif
