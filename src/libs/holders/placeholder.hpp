#ifndef SRC_PLACEHOLDER_HPP
#define SRC_PLACEHOLDER_HPP

#include <array>

#include "../card.hpp"

namespace cell {
    class placeholder {
    protected:
        sf::RenderWindow *window;
        std::array<bool, 4> emptyFlags = {true, true, true, true};
    public:
        explicit placeholder(sf::RenderWindow *window);
        ~placeholder() = default;

        bool isEmpty(int n = 0);

        virtual void put(card &c) = 0;
        virtual card &get() = 0;
    };
}

#endif