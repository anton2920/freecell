#ifndef SRC_CARD_HPP
#define SRC_CARD_HPP

#include <utility>
#include <SFML/Graphics/RectangleShape.hpp>

namespace cell {
    class card {
    public:
        enum class VALUE {
            ace = 1,
            two,
            three,
            four,
            five,
            six,
            seven,
            eight,
            nine,
            ten,
            jack,
            queen,
            king,
            blank = 0
        };
        enum class SUIT {
            clubs = 1,
            spades,
            hearts,
            diamonds,
            blank = 0
        };

        enum class POSITION {
            tabled,
            safehouse,
            endgame
        };
    private:
        std::pair <VALUE, SUIT> type;
        std::pair<int, int> coords;
        POSITION pos;

        bool canMove(card &other);
        bool isRed();
    public:
        static const int w = 70;
        static const int h = 96;

        explicit card(std::pair<VALUE, SUIT> _type = {VALUE::blank, SUIT::blank},
                std::pair<int, int> _coords = {0, 0}, POSITION _pos = POSITION::tabled);
        ~card() = default;

        [[nodiscard]] const std::pair<VALUE, SUIT> &getType() const;
        [[nodiscard]] const std::pair<int, int> &getCoords() const;
        [[nodiscard]] POSITION getPos() const;

        void setType(const std::pair<VALUE, SUIT> &_type);
        void setCoords(const std::pair<int, int> &_coords);
        void setPos(POSITION _pos);

        bool move(card &other);
    };
}

#endif
