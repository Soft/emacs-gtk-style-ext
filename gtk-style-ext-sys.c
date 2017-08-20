/*
Copyright (C) 2017 Samuel Laurén <samuel.lauren@iki.fi>

This file is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

This file is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs; see the file COPYING. If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.
*/

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include <gtk/gtk.h>
#include <emacs-module.h>

#include <stdio.h>

int plugin_is_GPL_compatible;

#define GTK_STYLE_EXT_SYS_LOAD_FROM_STRING_DOC \
  "(gtk-style-ext-sys-load-from_string STRING)\n\n" \
  "Load Gtk CSS from STRING."

#define GTK_STYLE_EXT_SYS_PREFER_DARK_THEME_DOC \
  "(gtk-style-ext-sys-prefer-dark-theme ARG)\n\n" \
  "Request for dark Gtk theme variant. If ARG is non-nil, Emacs will request for a dark theme."

#define GTK_STYLE_EXT_SYS_PREFER_DARK_THEME_P_DOC \
  "(gtk-style-ext-sys-prefer-dark-theme-p)"

static emacs_value emacs_nil;
static emacs_value emacs_t;
static GtkCssProvider *css_provider; // Maybe having this as a global isn't the best idea.

static emacs_value gtk_style_ext_sys_prefer_dark_theme(emacs_env *env,
                                                 ptrdiff_t n,
                                                 emacs_value *args,
                                                 void *ptr) {
  bool preference = env->is_not_nil(env, args[0]);
  g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", preference, NULL);
  return emacs_t;
}

static emacs_value gtk_style_ext_sys_prefer_dark_theme_p(emacs_env *env,
                                                 ptrdiff_t n,
                                                 emacs_value *args,
                                                 void *ptr) {
  gboolean preference = false;
  g_object_get(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", &preference, NULL);
  return preference ? emacs_t : emacs_nil;
}

static emacs_value gtk_style_ext_sys_load_from_string(emacs_env *env,
                                     ptrdiff_t n,
                                     emacs_value *args,
                                     void *ptr) {

  GError *load_error = NULL;
  char *source_buf = NULL;
  ptrdiff_t source_buf_size = 0;

  env->copy_string_contents(env, args[0], source_buf, &source_buf_size); // Get required buffer size
  source_buf = malloc(source_buf_size);

  env->copy_string_contents(env, args[0], source_buf, &source_buf_size); // Copy the string

  gtk_css_provider_load_from_data(css_provider, source_buf, -1, &load_error);
  free(source_buf);

  if (load_error != NULL) {
    printf("Error: %s", load_error->message);
    g_error_free(load_error);
    return emacs_nil;
  }

  return emacs_t;
}

static void setup_css_provider(void) {
  GdkDisplay *display = gdk_display_get_default();
  // FIXME: This is bad
  assert(display != NULL);
  GdkScreen *screen = gdk_display_get_default_screen(display);

  css_provider = gtk_css_provider_new();

  gtk_style_context_add_provider_for_screen(screen,
                                            GTK_STYLE_PROVIDER(css_provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

int emacs_module_init(struct emacs_runtime *ert) {
  emacs_env *env = ert->get_environment(ert);

  emacs_nil = env->intern(env, "nil");
  emacs_t = env->intern(env, "t");

  // Hopefully this won't be called multiple times.
  setup_css_provider();

  emacs_value fset = env->intern(env, "fset");
  emacs_value args[2];

  args[0] = env->intern(env, "gtk-style-ext-sys-load-from-string");
  args[1] = env->make_function(env, 1, 1, gtk_style_ext_sys_load_from_string,
                               GTK_STYLE_EXT_SYS_LOAD_FROM_STRING_DOC, NULL);
  env->funcall(env, fset, 2, args);

  args[0] = env->intern(env, "gtk-style-ext-sys-prefer-dark-theme");
  args[1] = env->make_function(env, 1, 1, gtk_style_ext_sys_prefer_dark_theme,
                               GTK_STYLE_EXT_SYS_PREFER_DARK_THEME_DOC, NULL);
  env->funcall(env, fset, 2, args);

  args[0] = env->intern(env, "gtk-style-ext-sys-prefer-dark-theme-p");
  args[1] = env->make_function(env, 0, 0, gtk_style_ext_sys_prefer_dark_theme_p,
                               GTK_STYLE_EXT_SYS_PREFER_DARK_THEME_P_DOC, NULL);
  env->funcall(env, fset, 2, args);

  emacs_value provide = env->intern(env, "provide");
  emacs_value gtk_style_ext_sys = env->intern(env, "gtk-style-ext-sys");
  env->funcall(env, provide, 1, &gtk_style_ext_sys);
  
  return 0;
}

