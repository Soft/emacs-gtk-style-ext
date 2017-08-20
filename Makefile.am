AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = gtk-css-sys.la
gtk_css_sys_la_CFLAGS = $(GTK_CFLAGS) -I$(EMACS_SOURCES)
gtk_css_sys_la_LDFLAGS = -module $(GTK_LIBS)

gtk_css_sys_la_SOURCES = gtk-css-sys.c

EXTRA_DIST = README.md \
	README \
	COPYING \
	gtk-css.el

