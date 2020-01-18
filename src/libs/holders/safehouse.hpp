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
        void get(const sf::Vector2f &pos) override;

        bool isInArea(const sf::Vector2i &pos) override;
        bool canPut(const sf::Vector2i &pos) override;

        void copyFrom(const safehouse *other);
    };
}

#endif
