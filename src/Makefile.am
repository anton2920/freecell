AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = $(ACLOCAL_FLAGS)

$(shell ./scripts/pics.sh)

bin_PROGRAMS = FreeCell

FreeCell_SOURCES = main.cpp

FreeCell_CXXFLAGS = $(CXXFLAGS) -Wall -Wextra -std=c++17
FreeCell_LDADD = libFreeCell.la
FreeCell_LDFLAGS = $(LDFLAGS)

include_HEADERS = libs/card.hpp libs/game.hpp libs/holders/endgame.hpp libs/holders/placeholder.hpp libs/holders/safehouse.hpp

lib_LTLIBRARIES = libFreeCell.la

# libFreeCell.la
libFreeCell_la_LDFLAGS = -version-info 0:0:0
libFreeCell_la_SOURCES = libs/card.cpp libs/game.cpp libs/menu.cpp libs/drawSystem.cpp libs/stats.cpp \
	libs/holders/placeholder.cpp libs/holders/safehouse.cpp libs/holders/endgame.cpp
libFreeCell_la_CXXFLAGS = $(FreeCell_CXXFLAGS)
