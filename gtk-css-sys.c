#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <gtk/gtk.h>
#include <emacs-module.h>

#include <stdio.h>

int plugin_is_GPL_compatible;

#define GTK_CSS_SYS_LOAD_FROM_STRING_DOC \
  "(gtk-css-sys-load-from_string STRING)\n\n" \
  "Load Gtk CSS from STRING."

static emacs_value emacs_nil;
static emacs_value emacs_t;
static GtkCssProvider *css_provider; // Maybe having this as a global isn't the best idea.

static emacs_value gtk_css_sys_load_from_string(emacs_env *env,
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

// FIXME: How should we get display
static void setup_css_provider(void) {
  GdkDisplay *display = gdk_display_get_default();
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
  args[0] = env->intern(env, "gtk-css-sys-load-from-string");
  args[1] = env->make_function(env, 1, 1, gtk_css_sys_load_from_string,
                               GTK_CSS_SYS_LOAD_FROM_STRING_DOC, NULL);
  env->funcall(env, fset, 2, args);

  emacs_value provide = env->intern(env, "provide");
  emacs_value gtk_css_sys = env->intern(env, "gtk-css-sys");
  env->funcall(env, provide, 1, &gtk_css_sys);
  
  return 0;
}

