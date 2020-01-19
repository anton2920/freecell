#include <fstream>

#include "game.hpp"

cell::game::statistics::statistics() {

    /* I/O flow */
    std::ifstream inputs(std::string(globalPath) + std::string(recFileName), std::ios_base::in | std::ios_base::binary);
    struct {
        statType first;
        statType second;
    } toRead{};

    /* I/O flow */
    if (inputs.is_open()) {
        inputs >> *this;
        inputs.close();
    }
}

std::ostream &cell::operator<<(std::ostream &stream, const cell::game::statistics &st) {

    /* Initializing variables */
    struct {
        cell::game::statistics::statType first;
        cell::game::statistics::statType second;
    } toWrite{};

    /* I/O flow */
    toWrite.first = st.total.first;
    toWrite.second = st.total.second;
    stream.write((char *) &toWrite, sizeof(toWrite));

    toWrite.first = st.streaks.first;
    toWrite.second = st.streaks.second;
    stream.write((char *) &toWrite, sizeof(toWrite));

    toWrite.first = st.current.first;
    toWrite.second = static_cast<cell::game::statistics::statType>(st.current.second);
    stream.write((char *) &toWrite, sizeof(toWrite));

    /* Returning value */
    return stream;
}

std::istream &cell::operator>>(std::istream &stream, cell::game::statistics &st) {

    /* Initializing variables */
    struct {
        cell::game::statistics::statType first;
        cell::game::statistics::statType second;
    } toRead{};

    /* I/O flow */
    stream.read((char *)&toRead, sizeof(toRead));
    st.total.first = toRead.first;
    st.total.second = toRead.second;

    stream.read((char *)&toRead, sizeof(toRead));
    st.streaks.first = toRead.first;
    st.streaks.second = toRead.second;

    stream.read((char *)&toRead, sizeof(toRead));
    st.current.first = toRead.first;
    st.current.second = static_cast<cell::game::statistics::streakType>(toRead.second);

    /* Returning value */
    return stream;
}

cell::game::statistics::~statistics() {

    /* Initializing variables */
    std::ofstream outputs(std::string(globalPath) + std::string(recFileName), std::ios_base::out | std::ios_base::binary);

    /* I/O flow */
    if (outputs.is_open()) {
        outputs << *this;
        outputs.close();
    }

}

void cell::game::statistics::gameWin() {

    /* Main part */
    ++this->thisSession.first;
    ++this->total.first;

    if (this->thisSession.first > this->streaks.first) {
        this->streaks.first = this->thisSession.first;
    }

    if (this->current.second == cell::game::statistics::loss) {
        this->current.first = 1;
        this->current.second = cell::game::statistics::win;
    } else {
        ++this->current.first;
        if (this->current.first > this->streaks.first) {
            this->streaks.first = this->current.first;
        }
    }
}

void cell::game::statistics::gameLost() {

    /* Main part */
    ++this->thisSession.second;
    ++this->total.second;

    if (this->current.second == cell::game::statistics::win) {
        this->current.first = 1;
        this->current.second = cell::game::statistics::loss;
    } else {
        ++this->current.first;
        if (this->current.first > this->streaks.second) {
            this->streaks.second = this->current.first;
        }
    }
}

void cell::game::statistics::clear() {

    /* Initializing variables */
    this->thisSession.first = this->thisSession.second = 0;
    this->total.first = this->total.second = 0;
    this->streaks.first = this->streaks.second = 0;
    this->current.first = 0;

//    std::ofstream outputs(std::string(globalPath) + std::string("records.bin"), std::ios_base::out | std::ios_base::binary);

    /* I/O flow */
//    outputs << *this;
}
