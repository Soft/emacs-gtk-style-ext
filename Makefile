CC := gcc
LD := gcc
PKG_CONFIG := pkg-config
EMACS := /usr/share/emacs/25.2
GTK_CFLAGS = $(shell $(PKG_CONFIG) --cflags gtk+-3.0)
GTK_LIBS = $(shell $(PKG_CONFIG) --libs gtk+-3.0)
CFLAGS := $(CFLAGS) -Wall -std=gnu11
LDFLAGS := 

all: gtk-css-sys.so

%.so: %.o
	$(LD) -shared $(LDFLAGS) $(GTK_LIBS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -I$(EMACS)/src -fPIC -c $<

.PHONY: clean

clean:
	rm -f gtk-css-sys.so

