#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <gtk/gtk.h>
#include <emacs-module.h>

#include <stdio.h>

int plugin_is_GPL_compatible;

#define GTK_CSS_FROM_PATH_DOC \
  "(gtk-css-from_path PATH)\n\n" \
  "Load CSS from PATH."

static emacs_value emacs_nil;
static emacs_value emacs_t;

static emacs_value gtk_css_from_path(emacs_env *env,
                                     ptrdiff_t n,
                                     emacs_value *args,
                                     void *ptr) {
  GtkCssProvider *provider = gtk_css_provider_new();
  GdkDisplay *display = gdk_display_get_default();
  assert(display != NULL);
  GdkScreen *screen = gdk_display_get_default_screen(display);

  GError *error = NULL;

  gtk_style_context_add_provider_for_screen(screen,
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  ptrdiff_t buf_size = 0;
  char *path_buf = NULL;
  env->copy_string_contents(env, args[0], path_buf, &buf_size); // Get required buffer size
  path_buf = malloc(buf_size);
  env->copy_string_contents(env, args[0], path_buf, &buf_size); // Copy the string

  printf("%s\n", path_buf);

  gtk_css_provider_load_from_path(provider, path_buf, &error);

  if (error) {
    /* emacs_value signal = env->intern(env, "file-error"); */
    printf("Error: %s", error->message);
    /* emacs_value message = env->make_string(env, error->message, strlen(error->message)); */
    /* env->non_local_exit_signal(env, signal, message); */
    /* return emacs_nil; */
  }

  return emacs_t;
}

int emacs_module_init(struct emacs_runtime *ert) {
  emacs_env *env = ert->get_environment(ert);

  emacs_nil = env->intern(env, "nil");
  emacs_t = env->intern(env, "t");

  emacs_value fset = env->intern(env, "fset");

  emacs_value args[2];
  args[0] = env->intern(env, "gtk-css-from-path");
  args[1] = env->make_function(env, 1, 1, gtk_css_from_path, GTK_CSS_FROM_PATH_DOC, NULL);
  env->funcall(env, fset, 2, args);

  emacs_value provide = env->intern(env, "provide");
  emacs_value gtk_css = env->intern(env, "gtk-css");
  env->funcall(env, provide, 1, &gtk_css);
  
  return 0;
}

