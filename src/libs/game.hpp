#ifndef SRC_GAME_HPP
#define SRC_GAME_HPP

#include <deque>
#include <cstdint>
#include <SFML/Graphics.hpp>
//#include <TGUI/TGUI.hpp>

#include "card.hpp"
#include "holders/safehouse.hpp"
#include "holders/endgame.hpp"

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
                manl_x = 320,
                mans_y = 446,
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

            enum cursors {
                cursor_down_x = 392,
                cursor_down_y = 446,
                cursor_down_w = 14,
                cursor_down_h = 27,
                cursor_up_x = 407,
                cursor_up_y = 446,
                cursor_up_w = 8,
                cursor_up_h = 19
            };

            sf::RenderWindow *window;
            sf::Texture texture;
        public:
            enum card_row_sizes {
                card_row_x = 7,
                card_row_y = window_panel_size + 106,
                card_row_space = 7,
                card_row_overlap_space = 18
            };

            enum cursor_type {
                up,
                down
            };

            explicit drawSystem(sf::RenderWindow *window);
            drawSystem(sf::RenderWindow *window, const sf::Texture &texture);
            ~drawSystem() = default;

            static void initCards(std::deque<card> &t);

            void drawField();
            void drawCard(card &c);
            void drawCards(std::deque<card> &t);
            void drawEndgame(endgame *endg);
            void drawMan(GAMESTATE st);
            void drawCursor(cursor_type t);
        };

        struct fieldState {
            std::deque<card> table{};
            safehouse *sh;
            endgame *eg;

            bool isUndoAvailable;

            explicit fieldState(sf::RenderWindow *window) {
                this->sh = new cell::safehouse(window);
                this->eg = new cell::endgame(window);
                isUndoAvailable = false;
            }
            ~fieldState() {
                delete(sh);
                delete(eg);
            }
        };
    private:
        GAMESTATE state = GAMESTATE::start;
        std::deque<card> table{};
        random r{};
        sf::RenderWindow *window{};

        void deal();

        drawSystem *draw;

        safehouse *sh;
        endgame *eg;

        fieldState *fs;

        enum gameWindow {
            window_w = 632,
            window_h = 446
        };

        enum powerMoveAnswer {
            one = 1,
            stack = 2,
            cancel = 3
        };

        const char *TITLE = "FreeCell";
    public:
        game();
        ~game();

        int exec();

        void start(int n = -1);
        void stop();

        [[nodiscard]] cell::card *findCard(const sf::Vector2f &coords) const;
        [[nodiscard]] cell::card *findBottomCard(const sf::Vector2f &coords) const;
        cell::card *findLowerCard(const cell::card *c) const;
        cell::card *findUpperCard(const cell::card *c) const;

        static cell::card *selectCard(cell::card &card);
        cell::card *selectCard(const sf::Vector2f &coords);

        bool isFreeColomn(const sf::Vector2f &pos);

        int availableSpace();

        card * canPowerMove(const card *c, const sf::Vector2i &pos);
        static powerMoveAnswer askForPowerMove();
        bool powerMove(card *c, card *topCard, const sf::Vector2i &pos);

        void saveState();
        void loadState();
    };
}

#endif
