/* gtk-style-ext-sys.c - Bindings to extra Gtk functionality.

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
Boston, MA 02111-1307, USA. */

#include <stdlib.h>
#include <stdbool.h>

#include <gtk/gtk.h>
#include <emacs-module.h>

int plugin_is_GPL_compatible;

// We keep track of user pointers that belong to us.
static GHashTable *providers;

#define GTK_STYLE_EXT_SYS_CREATE_PROVIDER_DOC \
  "(gtk-style-ext-sys-create-provider)\n\n" \
  "Create new Gtk Css style provider."

#define GTK_STYLE_EXT_SYS_PROVIDER_LOAD_FROM_STRING_DOC \
  "(gtk-style-ext-sys-provider-load-from-string PROVIDER STRING)\n\n" \
  "Load Css STRING into PROVIDER."

#define GTK_STYLE_EXT_SYS_PROVIDER_ACTIVATE_DOC \
  "(gtk-style-ext-sys-provider-load-from-string PROVIDER PRIORITY)\n\n" \
  "Activate PROVIDER with PRIORITY."

#define GTK_STYLE_EXT_SYS_PREFER_DARK_THEME_DOC \
  "(gtk-style-ext-sys-prefer-dark-theme ARG)\n\n" \
  "Request for dark Gtk theme variant. If ARG is non-nil, Emacs will request for a dark theme."

#define GTK_STYLE_EXT_SYS_PREFER_DARK_THEME_P_DOC \
  "(gtk-style-ext-sys-prefer-dark-theme-p)"

static emacs_value emacs_nil;
static emacs_value emacs_t;

// Dark Theme

static emacs_value gtk_style_ext_sys_prefer_dark_theme(emacs_env *env,
                                                 ptrdiff_t n,
                                                 emacs_value *args,
                                                 void *ptr) {
  GtkSettings *settings = gtk_settings_get_default();
  if (settings == NULL) {
    return emacs_nil;
  }

  bool preference = env->is_not_nil(env, args[0]);
  g_object_set(settings, "gtk-application-prefer-dark-theme", preference, NULL);
  return emacs_t;
}

static emacs_value gtk_style_ext_sys_prefer_dark_theme_p(emacs_env *env,
                                                 ptrdiff_t n,
                                                 emacs_value *args,
                                                 void *ptr) {
  GtkSettings *settings = gtk_settings_get_default();
  if (settings == NULL) {
    return emacs_nil;
  }

  gboolean preference = false;
  g_object_get(settings, "gtk-application-prefer-dark-theme", &preference, NULL);
  return preference ? emacs_t : emacs_nil;
}

// Style Provider

static void provider_unref(void *ptr) {
  g_hash_table_remove(providers, ptr);
  g_object_unref(ptr);
}


static emacs_value gtk_style_ext_sys_create_provider(emacs_env *env,
                                                     ptrdiff_t n,
                                                     emacs_value *args,
                                                     void *ptr) {
  GtkCssProvider *provider = gtk_css_provider_new();
  g_hash_table_add(providers, provider);
  env->make_user_ptr(env, provider_unref, (void *)provider);
}

static emacs_value gtk_style_ext_sys_provider_activate(emacs_env *env,
                                                       ptrdiff_t n,
                                                       emacs_value *args,
                                                       void *ptr) {
  GtkCssProvider *provider = (GtkCssProvider *)env->get_user_ptr(env, args[0]);
  if (!g_hash_table_contains(providers, provider)) {
    // TODO: Return error
    return emacs_nil;
  }

  int priority = env->extract_integer(env, args[1]);

  GdkDisplay *display = gdk_display_get_default();
  if (display == NULL) {
    return emacs_nil;
  }

  GdkScreen *screen = gdk_display_get_default_screen(display);

  gtk_style_context_add_provider_for_screen(screen,
                                            GTK_STYLE_PROVIDER(provider),
                                            priority);

  return emacs_t;
}

static emacs_value gtk_style_ext_sys_provider_load_from_string(emacs_env *env,
                                                               ptrdiff_t n,
                                                               emacs_value *args,
                                                               void *ptr) {
  GtkCssProvider *provider = (GtkCssProvider *)env->get_user_ptr(env, args[0]);
  if (!g_hash_table_contains(providers, provider)) {
    // TODO: Return error
    return emacs_nil;
  }

  GError *load_error = NULL;
  char *style_buf = NULL;
  ptrdiff_t style_buf_size = 0;

  // Get required buffer size
  env->copy_string_contents(env, args[1], style_buf, &style_buf_size);
  
  style_buf = malloc(style_buf_size);
  // Copy the string
  env->copy_string_contents(env, args[1], style_buf, &style_buf_size);
  
  gtk_css_provider_load_from_data(provider, style_buf, -1, &load_error);
  free(style_buf);

  if (load_error != NULL) {
    g_error_free(load_error);
    return emacs_nil;
  }

  return emacs_t;
}

struct constant_int {
  const char *name;
  int value;
};

static void make_int_constants(emacs_env *env, size_t len, struct constant_int defs[]) {
  emacs_value eval = env->intern(env, "eval");
  emacs_value list = env->intern(env, "list");
  emacs_value defconst = env->intern(env, "defconst");
  emacs_value list_args[3], eval_args[1], list_val;
  list_args[0] = defconst;
  for (size_t i = 0; i < len; i++) {
    list_args[1] = env->intern(env, defs[i].name);
    list_args[2] = env->make_integer(env, defs[i].value);
    list_val = env->funcall(env, list, 3, list_args);
    eval_args[0] = list_val;
    env->funcall(env, eval, 1, eval_args);
  }
}

int emacs_module_init(struct emacs_runtime *ert) {
  emacs_env *env = ert->get_environment(ert);

  emacs_nil = env->intern(env, "nil");
  emacs_t = env->intern(env, "t");

  emacs_value fset = env->intern(env, "fset");
  emacs_value defconst = env->intern(env, "defconst");
  emacs_value args[2];

  providers = g_hash_table_new(NULL, NULL);

  // Functions

  args[0] = env->intern(env, "gtk-style-ext-sys-create-provider");
  args[1] = env->make_function(env, 0, 0, gtk_style_ext_sys_create_provider,
                               GTK_STYLE_EXT_SYS_CREATE_PROVIDER_DOC, NULL);
  env->funcall(env, fset, 2, args);

  args[0] = env->intern(env, "gtk-style-ext-sys-provider-load-from-string");
  args[1] = env->make_function(env, 2, 2, gtk_style_ext_sys_provider_load_from_string,
                               GTK_STYLE_EXT_SYS_PROVIDER_LOAD_FROM_STRING_DOC, NULL);
  env->funcall(env, fset, 2, args);

  args[0] = env->intern(env, "gtk-style-ext-sys-provider-activate");
  args[1] = env->make_function(env, 2, 2, gtk_style_ext_sys_provider_activate,
                               GTK_STYLE_EXT_SYS_PROVIDER_ACTIVATE_DOC, NULL);
  env->funcall(env, fset, 2, args);

  args[0] = env->intern(env, "gtk-style-ext-sys-prefer-dark-theme");
  args[1] = env->make_function(env, 1, 1, gtk_style_ext_sys_prefer_dark_theme,
                               GTK_STYLE_EXT_SYS_PREFER_DARK_THEME_DOC, NULL);
  env->funcall(env, fset, 2, args);

  args[0] = env->intern(env, "gtk-style-ext-sys-prefer-dark-theme-p");
  args[1] = env->make_function(env, 0, 0, gtk_style_ext_sys_prefer_dark_theme_p,
                               GTK_STYLE_EXT_SYS_PREFER_DARK_THEME_P_DOC, NULL);
  env->funcall(env, fset, 2, args);

  // Constants

  make_int_constants(env, 5, (struct constant_int[]){
      {"gtk-style-ext-sys-provider-priority-fallback", GTK_STYLE_PROVIDER_PRIORITY_FALLBACK},
      {"gtk-style-ext-sys-provider-priority-theme", GTK_STYLE_PROVIDER_PRIORITY_THEME},
      {"gtk-style-ext-sys-provider-priority-settings", GTK_STYLE_PROVIDER_PRIORITY_SETTINGS},
      {"gtk-style-ext-sys-provider-priority-application", GTK_STYLE_PROVIDER_PRIORITY_APPLICATION},
      {"gtk-style-ext-sys-provider-priority-user", GTK_STYLE_PROVIDER_PRIORITY_USER}
    });

  // Module

  emacs_value provide = env->intern(env, "provide");
  emacs_value gtk_style_ext_sys = env->intern(env, "gtk-style-ext-sys");
  env->funcall(env, provide, 1, &gtk_style_ext_sys);
  
  return 0;
}

