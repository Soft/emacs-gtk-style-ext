AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = gtk-style-ext-sys.la
gtk_style_ext_sys_la_SOURCES = gtk-style-ext-sys.c
gtk_style_ext_sys_la_CFLAGS = -std=gnu11 $(GTK_CFLAGS) -I$(EMACS_SOURCES)
gtk_style_ext_sys_la_LDFLAGS = -module -avoid-version -shared $(GTK_LIBS)

lisp_DATA = gtk-style-ext.el
# Switch to this after importing does not break
# lisp_LISP = gtk-style-ext.el

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = README.md \
	README \
	COPYING

