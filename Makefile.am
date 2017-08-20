AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = gtk-css-sys.la
gtk_css_sys_la_SOURCES = gtk-css-sys.c
gtk_css_sys_la_CFLAGS = -std=gnu11 $(GTK_CFLAGS) -I$(EMACS_SOURCES)
gtk_css_sys_la_LDFLAGS = -module -shared $(GTK_LIBS)

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = README.md \
	README \
	COPYING \
	gtk-css.el

