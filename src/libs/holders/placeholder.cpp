#include "placeholder.hpp"

cell::placeholder::placeholder(sf::RenderWindow *window) : window(window) {}

bool cell::placeholder::isEmpty(int n) {

    /* Initializing variables */
    bool flag;

    /* Main part */
    switch (n) {
        case 1: case 2: case 3: case 4:
            flag = emptyFlags[n];
            break;
        default:
            flag = emptyFlags[0] || emptyFlags[1] || emptyFlags[2] || emptyFlags[3];
            break;
    }

    /* Returning value */
    return flag;
}
