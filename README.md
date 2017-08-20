 [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
# gtk-style-ext 🍬

`gtk-style-ext` provides extra control over Emacs' gtk front-end's style. To be
more specific it allows you to:

- __Switch to gtk's dark theme variant on the fly.__
- __Load additional application-level CSS into Emacs.__ This allows nearly
  limitless styling of Emacs' user interface components. Think of Nyan cats on
  the menu bar or tool bar backgrounds that depend on the active major mode.

It is worth noting that `gtk-style-ext` is still experimental and might break
things.

## Installation

`gtk-style-ext` requires a native module that has to be build separately. This
compilation process requires Emacs' header files to be available. By default,
the configure script tries to find the required headers automatically, but if
this fails, path to them can be passed to it manually using the `EMACS_SOURCES`
argument.

    $ git clone git clone https://bitbucket.org/Soft/emacs-gtk-style-ext.git
    $ cd emacs-gtk-style-ext
    $ ./autogen.sh
    $ ./configure # Or ./configure EMACS_SOURCES=path-to-emacs-sources
    $ make
    $ sudo make install

The last command will install the shared library and the associated Elisp helper
module into your system-wide site-lisp directory (likely
`/usr/share/emacs/site-lisp`).

## Dependencies

`gtk-style-ext` requires Emacs to be built with dynamic loading support and gtk
3 interface.

## Usage

    (require 'gtk-style-ext)
    ;; Enable dark theme variant
    (gtk-style-ext-dark-theme-mode 1)
    ;; Load CSS from a file
    (gtk-style-ext-load-from-file "/path/to/styles.css")
    ;; Load CSS from a string
    (gtk-style-ext-load-from-string "menubar { background-color: #00ff00; }")

## License

`gtk-style-ext` is licensed under the [GNU General Public License Version
3](http://www.gnu.org/licenses/gpl-3.0.en.html) or any later version. See
`COPYING` for further details.
