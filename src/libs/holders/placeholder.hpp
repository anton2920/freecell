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