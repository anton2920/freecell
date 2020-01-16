#ifndef SRC_ENDGAME_HPP
#define SRC_ENDGAME_HPP

#include <stack>

#include "placeholder.hpp"

namespace cell {
    class endgame : public placeholder {
    private:
        int counter{};
        std::array<std::stack<card *>, 4> house{};
    public:
        enum endgame_sizes {
            endgame_x_offset = 64
        };

        explicit endgame(sf::RenderWindow *window) : placeholder(window) {};
        ~endgame() = default;

        void put(card &c) override;
        card &get() override;

        [[nodiscard]] const std::array<std::stack<card *>, 4> &getHouse() const;

        bool isEndgame();
    };
}

#endif
