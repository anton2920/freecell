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

#ifndef SRC_PLACEHOLDER_HPP
#define SRC_PLACEHOLDER_HPP

/* Including other headers */
#include "../card.hpp"

#if (HAVE_ARRAY == 1)
    #include <array>
#endif

namespace cell {
    class placeholder {
    protected:
        sf::RenderWindow *window;
        std::array<bool, 4> emptyFlags = {true, true, true, true};
    public:
        explicit placeholder(sf::RenderWindow *window);
        ~placeholder() = default;

        bool isEmpty(int n = -1);

        virtual void put(card *c, const sf::Vector2i &pos) = 0;
        virtual void put(card *c, int whichOne) = 0;
        virtual void get(const sf::Vector2f &pos) = 0;

        virtual bool isInArea(const sf::Vector2i &pos) = 0;
        virtual bool canPut(const sf::Vector2i &pos) = 0;

        virtual void clear() = 0;
    };
}

#endif