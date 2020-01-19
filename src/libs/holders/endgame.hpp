#ifndef SRC_ENDGAME_HPP
#define SRC_ENDGAME_HPP

#include <stack>

#include "placeholder.hpp"

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
