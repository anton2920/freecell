# Autoconf requirements

# Initialize, but use more options. Note parameters brackts and whitespace.
AC_INIT([FreeCell], [1.0], [anton2920@gmail.com])

# Safety check - list a source file that wouldn't be in other directories:
AC_CONFIG_SRCDIR([src/main.cpp])

# Put configuration results here, so we can easily #include them:
AC_CONFIG_HEADERS([src/libs/FreeCell_config.h])

# Put autotools auxiliary files in a subdir, so they don't clutter top dir:
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# Enable «automake» to simplify creating makefiles:
AM_INIT_AUTOMAKE
AC_CONFIG_FILES([Makefile src/Makefile scripts/Makefile])

# Look for a C (CXX) compiler:
AC_LANG([C++])
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX

# Check for: (more) programs, libraries, header files, types,
# structures, compiler characteristics, library functions, system services.
LT_INIT

AC_ENABLE_SHARED
AC_PROG_LIBTOOL

# Check for SDL2
AC_CHECK_LIB([sfml-system], [main], [], [
	AC_MSG_ERROR([sfml-system is required to compile and run this game])
	])

AC_CHECK_LIB([sfml-window], [main], [], [
	AC_MSG_ERROR([sfml-window is required to compile and run this game])
	])

AC_CHECK_LIB([sfml-graphics], [main], [], [
	AC_MSG_ERROR([sfml-graphics is required to compile and run this game])
	])

AC_CHECK_LIB([tgui], [main], [], [
	AC_MSG_ERROR([tgui is required to compile and run this game])
	])

AC_CHECK_HEADERS([SFML/Graphics.hpp], [], [
	AC_MSG_ERROR([SFML headers are required to build this project])
	])

AC_CHECK_HEADERS([TGUI/TGUI.hpp], [], [
	AC_MSG_ERROR([TGUI headers are required to build this project])
	])


AC_CHECK_HEADERS([utility algorithm ctime array deque iostream cstdint fstream stack], [], [
	AC_MSG_ERROR([unable to find necessary headers])
	])

 # Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Do final output.
AC_OUTPUT

echo 
echo " ---------------------------------"
echo "| Options used to compile and link:"
echo "| CXX           = $CXX"
echo "| CXXFLAGS      = $CXXFLAGS"
echo "| LDFLAGS       = $LDFLAGS"
echo "| ARFLAGS       = $ARFLAGS"
echo " ---------------------------------"