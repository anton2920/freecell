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

#ifndef SRC_ENDGAME_HPP
#define SRC_ENDGAME_HPP

#include "placeholder.hpp"

#if (HAVE_STACK == 1)
    #include <stack>
#endif

namespace cell {
    class endgame : public placeholder {
    private:
        int counter{};
        std::array<std::stack<card>, 4> house{};
    public:
        enum endgame_sizes {
            endgame_x_offset = 348
        };

        explicit endgame(sf::RenderWindow *window) : placeholder(window) {};
        ~endgame() = default;

        void put(card *c, const sf::Vector2i &pos) override;
        void put(card *c, int whichOne) override;
        void get(const sf::Vector2f &pos) override;

        [[nodiscard]] const std::array<std::stack<card>, 4> &getHouse() const;

        bool isEndgame();
        int getLeft();

        bool isInArea(const sf::Vector2i &pos) override;
        bool canPut(const sf::Vector2i &pos) override;
        bool canPut(card &c, const sf::Vector2i &pos);

        void copyFrom(const endgame *other);

        void clear() override;
    };
}

#endif
