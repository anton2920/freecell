#ifndef SRC_GAME_HPP
#define SRC_GAME_HPP

#include <deque>
#include <cstdint>
#include "card.hpp"

namespace cell {
    class random {
    public:
        void srand(int seed) {

            /* Initializing variables */
            _seed = seed;
        }
        int rand() {

            /* Returning value */
            return (_seed = (_seed * MULT + INCR) & MASK) >> 16;
        }
    private:
        friend class game;
        int _seed;
        enum { MULT = 214013, INCR = 2531011, MASK = (1U << 31) - 1 };
    };

    class game {
    public:
        enum class GAMESTATE {
            start,
            game,
            win
        };
    private:
        GAMESTATE state;
        std::deque<card> table;
        random r;
        // window
        // renderer

        void deal();
    public:
        game();
        ~game();

        int exec();

        void start(int n = -1);
        void stop();
    };
}

#endif
