#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libpeephole], [1.0], [example.test.com])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# Checks for libraries.
dnl Check module with PKG_CONFIG
PKG_CHECK_MODULES([CPPUNIT],[cppunit])
PKG_CHECK_MODULES([LIBUV],[libuv])
# Checks for header files.
AC_CHECK_HEADERS([syslog.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/libpeephole/libpeephole.pc
                 src/libpeephole/Makefile
                 test/Makefile
                 test/ut/Makefile
])
AC_OUTPUT
