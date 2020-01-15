#ifndef SRC_GAME_HPP
#define SRC_GAME_HPP

#include <deque>
#include <cstdint>
#include <SFML/Graphics.hpp>

#include "card.hpp"

namespace cell {
    const int window_panel_size = 20;

    class random {
    public:
        void srand(unsigned int seed) {

            /* Initializing variables */
            _seed = seed;
        }
        int rand() {

            /* Returning value */
            return (_seed = (_seed * MULT + INCR) & MASK) >> 16;
        }
    private:
        friend class game;
        unsigned int _seed;
        enum { MULT = 214013, INCR = 2531011, MASK = (1U << 31) - 1 };
    };

    class game {
    public:
        enum class GAMESTATE {
            start,
            game,
            win
        };

        class drawSystem {
        private:
            enum field {
                field_x = 0,
                field_y = 0,
                field_w = 632,
                field_h = 446
            };

            enum man {
                manl_x = 632,
                mans_y = 384,
                mans_w = 36,
                mans_h = 36,
                manr_offset = mans_w,
                manc_x = 298,
                manc_y = window_panel_size + 19,
                manb_x = 0,
                manb_y = 446,
                manb_w = 320,
                manb_h = 320,
                manb_x_offset = 10,
                manb_y_offset = window_panel_size
            };

            enum safehouse {
                safehouse_x = 0,
                safehouse_y = 20,
                safehouse_w = 284,
                safehouse_h = 96
            };

            enum endgame {
                endgame_x_offset = 64
            };

            enum card_sizes {
                card_x = 632,
                card_y = 0,
                card_w = 71,
                card_h = 96,
                card_row_x = 7,
                card_row_y = window_panel_size + 106,
                card_row_space = 7,
                card_row_overlap_space = 18
            };

            sf::RenderWindow *window;
            sf::Texture texture;

        public:
            explicit drawSystem(sf::RenderWindow *window);
            drawSystem(sf::RenderWindow *window, const sf::Texture &texture);
            ~drawSystem() = default;

            static void initCards(std::deque<card> &t);

            void drawField();
            void drawCards(std::deque<card> &t);
            void drawMan(GAMESTATE st);
        };
    private:
        GAMESTATE state = GAMESTATE::start;
        std::deque<card> table{};
        random r{};
        sf::RenderWindow *window{};

        void deal();

        drawSystem *draw;

        enum gameWindow {
            window_w = 632,
            window_h = 446
        };

        const char *TITLE = "FreeCell";
    public:
        game();
        ~game();

        int exec();

        void start(int n = -1);
        void stop();
    };
}

#endif
