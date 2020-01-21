#include "game.hpp"

cell::game::menu::menu(game *currentGame) {

    /* Initializing variables */
    sf::RenderWindow *w = currentGame->window;
    this->gui = new tgui::Gui(*w);
    this->bar = tgui::MenuBar::create();
    this->cl = tgui::Label::create();

    /* Main part */
    this->bar->setSize(static_cast<float>(w->getSize().x), 20.f);
    this->bar->setTextSize(11);
    this->bar->addMenu(gameMenu);

    this->bar->addMenuItem(newGameMenu);
    this->bar->connectMenuItem(gameMenu, newGameMenu, &cell::game::start, currentGame, -1);

    this->bar->addMenuItem(selectGameMenu);
    this->bar->connectMenuItem(gameMenu, selectGameMenu, &cell::game::selectGame, currentGame);

    this->bar->addMenuItem(restartGameMenu);
    this->bar->setMenuItemEnabled(gameMenu, restartGameMenu, false);
    this->bar->connectMenuItem(gameMenu, restartGameMenu, &cell::game::start, currentGame, -2);

    this->bar->addMenuItem(statsMenu);
     this->bar->connectMenuItem(gameMenu, statsMenu, &cell::game::menu::showStats, this, currentGame);

    this->bar->addMenuItem(undoMenu);
    this->bar->setMenuItemEnabled(gameMenu, undoMenu, false);
    this->bar->connectMenuItem(gameMenu, undoMenu, &cell::game::loadState, currentGame);

    this->bar->addMenuItem(exitMenu);
    this->bar->connectMenuItem(gameMenu, exitMenu, &cell::game::setIsClosedEvent, currentGame, true);

    this->gui->add(this->bar);

//    this->cl->setSize(68, static_cast<float>(panel_h));
    this->cl->setSize(80.f, 11.f);
    this->cl->setPosition({panel_w + panel_cl_offset, 4});
    this->cl->setTextSize(10);
    this->gui->add(this->cl);

}

cell::game::menu::~menu() {

    /* Main part */
    delete(gui);
}

tgui::Gui *cell::game::menu::getGui() const {
    return gui;
}

void cell::game::menu::setRestartEnabled(bool flag) {

    /* Main part */
    this->bar->setMenuItemEnabled(gameMenu, restartGameMenu, flag);
}

void cell::game::menu::cardsLeftLabel(int left) {

    /* Main part */
    this->cl->setText(sf::String("Cards Left: ") + std::to_string(left));
}

bool cell::game::menu::wantToResign(cell::game *currentGame) {

    /* Initializing variables */
    sf::RenderWindow w(sf::VideoMode(228, 82), cell::game::TITLE, sf::Style::Titlebar | sf::Style::Close);
    w.setVerticalSyncEnabled(true);
    auto parPos = currentGame->window->getPosition();
    auto parSize = currentGame->window->getSize();
    w.setPosition(sf::Vector2i(parPos.x + parSize.x / 2 - w.getSize().x / 2, parPos.y + parSize.y / 2 - w.getSize().y / 2));
    tgui::Gui g(w);
    auto l = tgui::Label::create("Do you want to resign this game?");
    l->setTextSize(12);
    l->setMaximumTextWidth(159);
    l->setPosition({10, 9});
    g.add(l);

    auto b1 = tgui::Button::create("Yes"), b2 = tgui::Button::create("No");
    b1->setPosition({56, 48});
    b1->setSize(51.f, 25.f);
    b1->connect("pressed", &cell::game::menu::setWantResign, this, true);
    g.add(b1);

    b2->setPosition({125, 48});
    b2->setSize(51.f, 25.f);
    b2->connect("pressed", &cell::game::menu::setWantResign, this, false);
    g.add(b2);

    /* Main part */
    while (w.isOpen()) {
        sf::Event event{};
        while (w.pollEvent(event)) {
            g.handleEvent(event);
            switch (event.type) {
                case sf::Event::Closed:
                    this->setWantResign(false);
                    w.close();
                    break;
                default:
                    break;
            }
        }

        if (this->isChanged) {
            w.close();
        }

        w.clear(sf::Color::White);
        g.draw();

        w.display();
    }

    this->isChanged = false;

    if (this->wantResign) {
        currentGame->stats->gameLost();
    }

    /* Returning value */
//    return !std::system(R"(zenity --question --no-wrap --title="FreeCell" --text="Do you want to resign this game?" --ok-label Yes --cancel-label No)");
    return this->wantResign;
}

void cell::game::menu::setWantResign(bool _wantResign) {

    /* Initializing variables */
    menu::wantResign = _wantResign;
    menu::setIsChanged(true);
}

void cell::game::menu::setUndoEnabled(bool flag) {

    /* Main part */
    this->bar->setMenuItemEnabled(gameMenu, undoMenu, flag);
}

long cell::game::menu::chooseGameNumber(cell::game *currentGame) {

    /* Initializing variables */
    long answer = -1;
    sf::RenderWindow w(sf::VideoMode(180, 122), "Game Number", sf::Style::Titlebar | sf::Style::Close);
	w.setVerticalSyncEnabled(true);
    auto parPos = currentGame->window->getPosition();
    auto parSize = currentGame->window->getSize();
    w.setPosition(sf::Vector2i(parPos.x + parSize.x / 2 - w.getSize().x / 2, parPos.y + parSize.y / 2 - w.getSize().y / 2));
    tgui::Gui g(w);

    auto l = tgui::Label::create("Select a game number from 1 to 1000000");
    l->setTextSize(12);
    l->setMaximumTextWidth(159);
    l->setPosition({20, 15});
    g.add(l);

    auto b1 = tgui::Button::create("OK");
    b1->setSize(60.f, 23.f);
    b1->connect("pressed", &cell::game::menu::setIsChanged, this,true);

    auto edit1 = tgui::EditBox::create();
    edit1->setSize(60.f, 20.f);
    edit1->setPosition({w.getSize().x / 2 - edit1->getSize().x / 2, 51});
    std::srand(std::time(nullptr));
    edit1->setText(std::to_string(std::rand() % 1000001));
    g.add(edit1);

    b1->setPosition({w.getSize().x / 2 - edit1->getSize().x / 2, w.getSize().y - b1->getSize().y - 11});
    g.add(b1);

    /* Main part */
    while (w.isOpen()) {
        sf::Event event{};
        while (w.pollEvent(event)) {
            g.handleEvent(event);
            switch (event.type) {
                case sf::Event::Closed:
                    w.close();
                    break;
                default:
                    break;
            }
        }
        if (this->isChanged) {
            try {
                answer = std::stoi(edit1->getText().toAnsiString());
            } catch (...) {
                this->isChanged = false;
                continue;
            }
            w.close();
        }

        w.clear(sf::Color::White);
        g.draw();

        w.display();
    }

    this->isChanged = false;

    /* Returning value */
    return answer;
}

void cell::game::menu::setIsChanged(bool _isChanged) {

    /* Initializing variables */
    menu::isChanged = _isChanged;
}

cell::game::powerMoveAnswer cell::game::menu::askForPowerMove(cell::game *currentGame) {

    /* Initializing variables */
    sf::RenderWindow w(sf::VideoMode(226, 130), "Move to Empty Column", sf::Style::Titlebar | sf::Style::Close);
	w.setVerticalSyncEnabled(true);
    auto parPos = currentGame->window->getPosition();
    auto parSize = currentGame->window->getSize();
    w.setPosition(sf::Vector2i(parPos.x + parSize.x / 2 - w.getSize().x / 2, parPos.y + parSize.y / 2 - w.getSize().y / 2));
    tgui::Gui g(w);

    auto b1 = tgui::Button::create("Move column");
    b1->setSize(135.f, 23.f);
    b1->setPosition({45, 24});
    b1->connect("pressed", &cell::game::menu::setPma, this,cell::game::stack);
    g.add(b1);

    auto b2 = tgui::Button::create("Move single card");
    b2->setSize(135.f, 23.f);
    b2->setPosition({45, 57});
    b2->connect("pressed", &cell::game::menu::setPma, this,cell::game::one);
    g.add(b2);

    auto b3 = tgui::Button::create("Cancel");
    b3->setSize(60.f, 23.f);
    b3->setPosition({83, 93});
    b3->connect("pressed", &cell::game::menu::setPma, this,cell::game::cancel);
    g.add(b3);

    /* Main part */
    while (w.isOpen()) {
        sf::Event event{};
        while (w.pollEvent(event)) {
            g.handleEvent(event);
            switch (event.type) {
                case sf::Event::Closed:
                    w.close();
                    break;
                default:
                    break;
            }
        }
        if (this->isChanged) {
            w.close();
        }

        w.clear(sf::Color::White);
        g.draw();

        w.display();
    }

    this->isChanged = false;

    /* Returning value */
    return this->pma;
}

void cell::game::menu::setPma(cell::game::powerMoveAnswer _pma) {

    /* Initializing variables */
    menu::pma = _pma;
    menu::setIsChanged(true);
}

cell::game::menu::gameOverAnswer cell::game::menu::startNewGame(cell::game *currentGame) {

    /* Initializing variables */
    this->goa = newCancel;

    sf::RenderWindow w(sf::VideoMode(203, 130), "Game Over", sf::Style::Titlebar | sf::Style::Close);
	w.setVerticalSyncEnabled(true);
    auto parPos = currentGame->window->getPosition();
    auto parSize = currentGame->window->getSize();
    w.setPosition(sf::Vector2i(parPos.x + parSize.x / 2 - w.getSize().x / 2, parPos.y + parSize.y / 2 - w.getSize().y / 2));
    tgui::Gui g(w);
    auto l = tgui::Label::create("Congratulations, you win!");
    l->setTextSize(13);
    l->setMaximumTextWidth(200);
    l->setPosition({19, 5});
    g.add(l);

    auto l2 = tgui::Label::create("Do you want to play again?");
    l2->setTextSize(12);
    l2->setMaximumTextWidth(200);
    l2->setPosition({19, 25});
    g.add(l2);

    auto b1 = tgui::Button::create("Yes");
    b1->setSize(60.f, 23.f);
    b1->setPosition({23, 94});
    b1->connect("pressed", &cell::game::menu::setGoa, this, gameOverAnswer::newRandom);
    g.add(b1);

    auto b2 = tgui::Button::create("No");
    b2->setSize(60.f, 23.f);
    b2->setPosition({120, 94});
    b2->connect("pressed", &cell::game::menu::setGoa, this, gameOverAnswer::newCancel);
    g.add(b2);

    auto ch1 = tgui::CheckBox::create("Select game");
    ch1->setChecked(this->isCheckedSelectGame);
    ch1->setPosition({23, 65});
    g.add(ch1);

    /* Main part */
    while (w.isOpen()) {
        sf::Event event{};
        while (w.pollEvent(event)) {
            g.handleEvent(event);
            switch (event.type) {
                case sf::Event::Closed:
                    this->setWantResign(false);
                    w.close();
                    break;
                default:
                    break;
            }
        }

        if (this->isChanged) {
            if (ch1->isChecked()) {
                this->goa = newSelect;
            }
            w.close();
        }

        w.clear(sf::Color::White);
        g.draw();

        w.display();
    }

    this->isChanged = false;
    this->isCheckedSelectGame = false;

    /* Returning value */
    return this->goa;
}

void cell::game::menu::setGoa(cell::game::menu::gameOverAnswer _goa) {

    /* Initializing variables */
    menu::goa = _goa;
    menu::setIsChanged(true);
}

void cell::game::menu::setIsCheckedSelectGame(bool _isCheckedSelectGame) {

    /* Initializing variables */
    menu::isCheckedSelectGame = _isCheckedSelectGame;
}

void cell::game::menu::showStats(cell::game *currentGame) {

    /* Initializing variables */
    this->goa = newCancel;

    sf::RenderWindow w(sf::VideoMode(225, 219), "FreeCell Statistics", sf::Style::Titlebar | sf::Style::Close);
	w.setVerticalSyncEnabled(true);
    auto parPos = currentGame->window->getPosition();
    auto parSize = currentGame->window->getSize();
    w.setPosition(sf::Vector2i(parPos.x + parSize.x / 2 - w.getSize().x / 2, parPos.y + parSize.y / 2 - w.getSize().y / 2));
    tgui::Gui g(w);
    auto l = tgui::Label::create("This session");
    l->setTextSize(13);
    l->setMaximumTextWidth(200);
    l->setPosition({stats_main_x - 0, stats_main_y - 0});
    g.add(l);

    auto lWin = tgui::Label::create("won:");
    lWin->setTextSize(12);
    g.add(lWin);

    auto lLose = tgui::Label::create("lost: ");
    lLose->setTextSize(12);
    g.add(lLose);

    auto ln1 = tgui::Label::create(std::to_string(currentGame->stats->thisSession.first));
    ln1->setTextSize(12);
    g.add(ln1);

    auto ln2 = tgui::Label::create(std::to_string(currentGame->stats->thisSession.second));
    ln2->setTextSize(12);
    g.add(ln2);

    auto current1 = tgui::Label::create("current: ");
    current1->setTextSize(12);
    current1->setPosition({current_x_1 - 0, current_y - 0});
    g.add(current1);

    sf::String c2str("");
    if (currentGame->stats->current.second == cell::game::statistics::loss && currentGame->stats->current.first) {
        c2str = " loss";
    } else if (currentGame->stats->current.second == cell::game::statistics::win && currentGame->stats->current.first) {
        c2str = " win";
    }

    auto current2 = tgui::Label::create(std::to_string(currentGame->stats->current.first) + c2str);
    current2->setTextSize(12);
    current2->setPosition({current_x_2 - 0, current_y - 0});
    g.add(current2);


    auto b1 = tgui::Button::create("OK");
    b1->setSize(60.f, 23.f);
    b1->setPosition({30, 187});
    b1->connect("pressed", &cell::game::menu::setIsChanged, this, true);
    g.add(b1);

    auto b2 = tgui::Button::create("Clear");
    b2->setSize(60.f, 23.f);
    b2->setPosition({135, 187});
    b2->connect("pressed", &cell::game::menu::showClearStats, this, currentGame);
    g.add(b2);

    /* Main part */
    while (w.isOpen()) {
        sf::Event event{};
        while (w.pollEvent(event)) {
            g.handleEvent(event);
            switch (event.type) {
                case sf::Event::Closed:
                    this->setWantResign(false);
                    w.close();
                    break;
                default:
                    break;
            }
        }

        if (this->isChanged || this->wantToDeleteStats) {
            w.close();
        }

        w.clear(sf::Color::White);

        l->setText("This session");
        l->setPosition({stats_main_x - 0, stats_main_y - 0});

        lWin->setPosition({stats_x_1 - 0, stats_y_1 - 0});
        lWin->setText("won: ");

        lLose->setPosition({stats_x_1 - 0, stats_y_2 - 0});
        lLose->setText("lost: ");

        ln1->setText(std::to_string(currentGame->stats->thisSession.first));
        ln1->setPosition({stats_x_2 - 0, stats_y_1 - 0});

        ln2->setText(std::to_string(currentGame->stats->thisSession.second));
        ln2->setPosition({stats_x_2 - 0, stats_y_2 - 0});

        g.draw();

        l->setText("Total");
        l->setPosition({stats_main_x - 0, stats_main_y + stats_y_offset});

        lWin->setPosition({stats_x_1 - 0, stats_y_1 + stats_y_offset});
        lLose->setPosition({stats_x_1 - 0, stats_y_2 + stats_y_offset});

        ln1->setText(std::to_string(currentGame->stats->total.first));
        ln1->setPosition({stats_x_2 - 0, stats_y_1 + stats_y_offset});

        ln2->setText(std::to_string(currentGame->stats->total.second));
        ln2->setPosition({stats_x_2 - 0, stats_y_2 + stats_y_offset});

        g.draw();

        l->setText("Streaks");
        l->setPosition({stats_main_x - 0, stats_main_y + stats_y_offset + stats_y_offset});

        lWin->setText("wins:");
        lWin->setPosition({stats_x_1 - 0, stats_y_1 + stats_y_offset + stats_y_offset});

        lLose->setText("losses: ");
        lLose->setPosition({stats_x_1 - 0, stats_y_2 + stats_y_offset + stats_y_offset});

        ln1->setText(std::to_string(currentGame->stats->streaks.first));
        ln1->setPosition({stats_x_2 - 0, stats_y_1 + stats_y_offset + stats_y_offset});

        ln2->setText(std::to_string(currentGame->stats->streaks.second));
        ln2->setPosition({stats_x_2 - 0, stats_y_2 + stats_y_offset + stats_y_offset});

        current1->setVisible(true);
        current2->setVisible(true);

        g.draw();

        current1->setVisible(false);
        current2->setVisible(false);

        w.display();
    }

    this->isChanged = false;

    if (this->wantToDeleteStats) {
        this->wantToDeleteStats = false;
        currentGame->stats->clear();
    }
}

void cell::game::menu::showClearStats(cell::game *currentGame) {

    /* Initializing variables */
    sf::RenderWindow w(sf::VideoMode(288, 82), cell::game::TITLE, sf::Style::Titlebar | sf::Style::Close);
	w.setVerticalSyncEnabled(true);
    auto parPos = currentGame->window->getPosition();
    auto parSize = currentGame->window->getSize();
    w.setPosition(sf::Vector2i(parPos.x + parSize.x / 2 - w.getSize().x / 2, parPos.y + parSize.y / 2 - w.getSize().y / 2));
    tgui::Gui g(w);
    auto l = tgui::Label::create("Are you sure you want to delete all statistics?");
    l->setTextSize(12);
    l->setMaximumTextWidth(159);
    l->setPosition({10, 9});
    g.add(l);

    auto b1 = tgui::Button::create("Yes"), b2 = tgui::Button::create("No");
    b1->setPosition({56, 48});
    b1->setSize(51.f, 25.f);
    b1->connect("pressed", &cell::game::menu::setWantToDeleteStats, this, true);
    g.add(b1);

    b2->setPosition({125, 48});
    b2->setSize(51.f, 25.f);
    b2->connect("pressed", &cell::game::menu::setWantToDeleteStats, this, false);
    g.add(b2);

    /* Main part */
    while (w.isOpen()) {
        sf::Event event{};
        while (w.pollEvent(event)) {
            g.handleEvent(event);
            switch (event.type) {
                case sf::Event::Closed:
                    this->setWantResign(false);
                    w.close();
                    break;
                default:
                    break;
            }
        }

        if (this->isChanged) {
            w.close();
        }

        w.clear(sf::Color::White);
        g.draw();

        w.display();
    }

    this->isChanged = false;
}

void cell::game::menu::setWantToDeleteStats(bool _wantToDeleteStats) {
    menu::wantToDeleteStats = _wantToDeleteStats;
    menu::setIsChanged(true);
}

