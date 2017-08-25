AC_PREREQ([2.69])
AC_INIT([emacs-style-ext], [0.1], [samuel.lauren@iki.fi])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
AC_LANG([C])
LT_INIT([disable-static])
AC_PROG_CC

LT_SYS_MODULE_EXT

PKG_CHECK_MODULES([GTK], [gtk+-3.0], [], [AC_MSG_ERROR([gtk+3.0 required])])

AC_HEADER_STDBOOL

dnl /usr/local would be cleaner but /usr/local/share/emacs doesn't seem to be common in practice
AC_PREFIX_DEFAULT([/usr])

AM_PATH_LISPDIR

AS_IF([test "x$lispdir" = "x"],
  [AC_MSG_ERROR([could not find Emacs lisp dir])], [])

dnl This might be a bad idea
AS_IF([test "x$libdir" = "x"],
  [],
  [libdir="$lispdir"
   AC_SUBST(lispdir)])

AC_ARG_VAR([EMACS_SOURCES], [Emacs source code path])

AS_IF([test "x$EMACS_SOURCES" = "x"],
  [AC_MSG_NOTICE([trying to automatically detect Emacs source code location])
  dnl This should be removed
   AC_CHECK_PROG([HAVE_EMACS], emacs, yes, no)
   AS_IF([test "x$HAVE_EMACS" = "xyes"],
     [AC_MSG_CHECKING([checking for Emacs source code])
      dnl Try to automatically find Emacs sourcesets
      dnl This seems to work on Gentoo if Emacs has been installed with "source" USE flag.
      EMACS_SOURCES="$(emacs -q --batch --eval "(require 'find-func)" --eval "(message \"%s\" find-function-C-source-directory)" 2>&1)"
      AS_IF([test "x$EMACS_SOURCES" = "xnil"],
        [AC_MSG_RESULT([no])
         AC_MSG_FAILURE([could not find Emacs source code. To specify source code location manually run ./configure EMACS_SOURCES=path-to-emacs-sources.])],
        [AC_MSG_RESULT([yes])])],
     [AC_MSG_FAILURE([could not find Emacs for automatic source code location. To specify source code location manually run ./configure EMACS_SOURCES=path-to-emacs-sources.])])],
  [])

AC_ARG_ENABLE([debug],
  AS_HELP_STRING([--enable-debug], [enable extra debugging functionality]),
  [AC_DEFINE([DEBUG])],
  [])

dnl Check that the module header is available.
SAVED_CFLAGS="$CFLAGS"
SAVED_CPPFLAGS="$CPPFLAGS"
CFLAGS="-I$EMACS_SOURCES"
CPPFLAGS="-I$EMACS_SOURCES"
AC_CHECK_HEADER([emacs-module.h], [], [AC_MSG_ERROR([could not find emacs-module.h. Is EMACS_SOURCES specified correctly?])])
CFLAGS="$SAVED_CFLAGS"
CPPFLAGS="$SAVED_CPPFLAGS"

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
AC_MSG_RESULT([
  *** $PACKAGE_NAME $VERSION

  Emacs source code path:    $EMACS_SOURCES
  prefix:                    $prefix
  datadir:                   $datarootdir
  library installation path: $libdir
  lisp installation path:    $lispdir

  *** Run 'make' and 'sudo make install'
])