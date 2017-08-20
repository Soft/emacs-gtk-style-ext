 [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
# gtk-style-ext 🍬

`gtk-style-ext` provides extra control over Emacs' gtk front-end's look and
feel. To be more specific it allows you to:

- __Switch to gtk's dark theme variant__ on the fly.
- __Load additional application-level CSS into Emacs__. This allows nearly
  limitless styling of Emacs' user interface components. Think of nyan casts on
  the menu bar or tool bar backgrounds that depend on the major mode.

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

## Dependencies

`gtk-style-ext` requires Emacs to be built with dynamic loading support and gtk
3 interface.

## Usage

TODO

## License

`gtk-style-ext` os licensed under the [GNU General Public License Version
3](http://www.gnu.org/licenses/gpl-3.0.en.html) or any later version. See
`COPYING` for further details.
