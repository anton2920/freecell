#ifndef SRC_CARD_HPP
#define SRC_CARD_HPP

#include <utility>
#include <SFML/Graphics.hpp>

namespace cell {
    class card {
    public:
        enum class VALUE {
            ace = 1,
            two = 2,
            three = 3,
            four = 4,
            five = 5,
            six = 6,
            seven = 7,
            eight = 8,
            nine = 9,
            ten = 10,
            jack = 11,
            queen = 12,
            king = 13,
            blank = 0
        };

        enum class SUIT {
            clubs = 1,
            diamonds = 2,
            hearts = 3,
            spades = 4,
            blank = 0
        };

        enum class POSITION {
            tabled,
            safehouse,
            endgame
        };

        enum class STATE {
            unpressed = 0,
            pressed
        };

        enum card_sizes {
            card_x = 632,
            card_y = 0,
            card_w = 71,
            card_h = 96,
            card_negative_offset = 384
        };
    private:
        std::pair <VALUE, SUIT> type;
        sf::Vector2f coords;
        POSITION pos;
        STATE st;
    public:
        explicit card(std::pair<VALUE, SUIT> _type = {VALUE::blank, SUIT::blank},
                sf::Vector2f _coords = sf::Vector2f(0, 0), POSITION _pos = POSITION::tabled, STATE _st = STATE::unpressed);
        card(const card &other);
        ~card() = default;

        [[nodiscard]] const std::pair<VALUE, SUIT> &getType() const;
        [[nodiscard]] sf::Vector2f getCoords() const;
        [[nodiscard]] POSITION getPos() const;
        [[nodiscard]] STATE getSt() const;

        [[nodiscard]] sf::IntRect getSpriteRect() const;

        void setType(const std::pair<VALUE, SUIT> &_type);
        void setCoords(const sf::Vector2f &_coords);
        void setPos(POSITION _pos);
        void setSt(STATE _st);

        bool isRed();

        bool canMove(card &other);
        bool move(card &other);
    };
}

#endif
