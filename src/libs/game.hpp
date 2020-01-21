#ifndef SRC_GAME_HPP
#define SRC_GAME_HPP

/* Including config header file */
#include "FreeCell_config.h"

/* Including other headers */
#if (HAVE_DEQUE == 1)
#include <deque>
#endif

#if (HAVE_CSTDINT == 1)
#include <cstdint>
#endif

#if (HAVE_SFML_GRAPHICS_HPP == 1)
#include <SFML/Graphics.hpp>
#endif

#if (HAVE_TGUI_TGUI_HPP == 1)
#include <TGUI/TGUI.hpp>
#endif

#include "card.hpp"
#include "holders/safehouse.hpp"
#include "holders/endgame.hpp"

namespace cell {
    const int window_panel_size = 20;
    const int additionalHeight = 7;

    char *getPath();

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
                field_y = window_panel_size,
                field_w = 632,
                field_h = 446 - window_panel_size + additionalHeight
            };

            enum man {
                manl_x = 320,
                mans_y = 446 + additionalHeight,
                mans_w = 36,
                mans_h = 36,
                manr_offset = mans_w,
                manc_x = 298,
                manc_y = window_panel_size + 19,
                manb_x = 0,
                manb_y = 446 + additionalHeight,
                manb_w = 320,
                manb_h = 320,
                manb_x_offset = 10,
                manb_y_offset = window_panel_size - additionalHeight
            };

            enum cursors {
                cursor_down_x = 392,
                cursor_down_y = 446 + additionalHeight,
                cursor_down_w = 14,
                cursor_down_h = 27,
                cursor_up_x = 407,
                cursor_up_y = 446 + additionalHeight,
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

        enum powerMoveAnswer {
            one = 1,
            stack = 2,
            cancel = 3
        };

        class menu {
        public:
            enum gameOverAnswer {
                newRandom,
                newSelect,
                newCancel
            };
        private:
            tgui::Gui *gui;
            std::shared_ptr<tgui::MenuBar> bar;
            std::shared_ptr<tgui::Label> cl;

            bool wantResign = false;
            bool isChanged = false;

            game::powerMoveAnswer pma;

            gameOverAnswer goa;
            bool isCheckedSelectGame = false;

            bool wantToDeleteStats = false;

            enum panel_sizes {
                panel_x = 0,
                panel_y = 0,
                panel_w = 632,
                panel_h = window_panel_size,
                panel_cl_offset = -82
            };

            enum stats_sizes {
                stats_main_x = 17,
                stats_main_y = 19,
                stats_x_1 = 73,
                stats_x_2 = 175,
                stats_y_1 = 34,
                stats_y_2 = stats_y_1 + 10,
                stats_y_offset = 55,
                current_x_1 = stats_x_1,
                current_x_2 = stats_x_2,
                current_y = 164
            };

            auto static constexpr gameMenu = "Game";
            auto static constexpr newGameMenu = "New Game\t\t   F2";
            auto static constexpr selectGameMenu = "Select Game\t\tF3";
            auto static constexpr restartGameMenu = "Restart Game";
            auto static constexpr statsMenu = "Statistics...\t\t\tF5";
            auto static constexpr undoMenu =  "Undo        \t\t\tF10\t";
            auto static constexpr exitMenu = "Exit";
        public:
            explicit menu(game *currentGame);
            ~menu();

            [[nodiscard]] tgui::Gui *getGui() const;

            void setIsChanged(bool _isChanged);

            void setRestartEnabled(bool flag);
            void setUndoEnabled(bool flag);

            void cardsLeftLabel(int left = 0);

            bool wantToResign(cell::game *currentGame);
            void setWantResign(bool _wantResign);

            long chooseGameNumber(cell::game *currentGame);

            cell::game::powerMoveAnswer askForPowerMove(cell::game *currentGame);
            void setPma(powerMoveAnswer _pma);

            gameOverAnswer startNewGame(cell::game *currentGame);
            void setGoa(gameOverAnswer _goa);
            void setIsCheckedSelectGame(bool _isCheckedSelectGame);

            void showStats(cell::game *currentGame);
            void showClearStats(cell::game *currentGame);
            void setWantToDeleteStats(bool _wantToDeleteStats);

        };

        class statistics {
            typedef int statType;
            enum streakType {
                loss,
                win
            };
        private:
            friend class menu;
            std::pair<statType, statType> thisSession{};
            std::pair<statType, statType> total{};
            std::pair<statType, statType> streaks{};
            std::pair<statType, streakType> current{};

            auto static constexpr recFileName = "records.bin";
        public:
            statistics();
            ~statistics();

            void gameWin();
            void gameLost();

            void clear();

            friend std::ostream &operator<<(std::ostream &stream, const statistics &st);
            friend std::istream &operator>>(std::istream &stream, statistics &st);
        };
    private:
        GAMESTATE state = GAMESTATE::start;
        std::deque<card> table{};
        random r{};
        sf::RenderWindow *window{};

        bool isClosedEvent = false;

        void deal();
        cell::card *selectedCard = nullptr;

        drawSystem *draw;

        safehouse *sh;
        endgame *eg;

        fieldState *fs;

        menu *m;

        statistics *stats;

        enum gameWindow {
            window_w = 632,
            window_h = 446 + additionalHeight
        };

        auto static constexpr TITLE = "FreeCell";
    public:
        game();
        ~game();

        int exec();

        void start(long n = -1);
        void selectGame();
        void stop();

        void clearGame();

        void setIsClosedEvent(bool _isClosedEvent);

        [[nodiscard]] cell::card *findCard(const sf::Vector2f &coords) const;
        [[nodiscard]] cell::card *findBottomCard(const sf::Vector2f &coords) const;

        cell::card *findUpperCard(const cell::card *c) const;

        static cell::card *selectCard(cell::card &card);

        bool isFreeColomn(const sf::Vector2f &pos);

        int availableSpace();

        card * canPowerMove(const card *c, const sf::Vector2i &pos);
        powerMoveAnswer askForPowerMove();
        bool powerMove(card *c, card *topCard, const sf::Vector2i &pos);

        void saveState();
        void loadState();

        bool generateBottomCardsList(std::vector<card *> &dest);
        bool tryAutoPlay();
    };
}

#endif
