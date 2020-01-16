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

        void put(card &c) override;
        card &get() override;
    };
}

#endif
