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

#include "placeholder.hpp"

cell::placeholder::placeholder(sf::RenderWindow *window) : window(window) {}

bool cell::placeholder::isEmpty(int n) {

    /* Initializing variables */
    bool flag;

    /* Main part */
    switch (n) {
        case 0: case 1: case 2: case 3:
            flag = emptyFlags[n];
            break;
        default:
            flag = emptyFlags[0] && emptyFlags[1] && emptyFlags[2] && emptyFlags[3];
            break;
    }

    /* Returning value */
    return flag;
}
