// C++ side of the C wrapper for backward-cpp
#include "backward_c.h"
#include "backward.hpp"

#include <memory>

namespace {
std::unique_ptr<backward::SignalHandling> g_signal_handler;
int g_snippet_enabled = 1;
int g_color_mode = 0; // 0 = automatic, 1 = never, 2 = always
} // namespace

extern "C" {

int backward_init_signal_handlers(void) {
  if (g_signal_handler)
    return 1; // already installed
#if defined(BACKWARD_SYSTEM_LINUX) || defined(BACKWARD_SYSTEM_DARWIN) ||       \
    defined(BACKWARD_SYSTEM_WINDOWS)
  g_signal_handler.reset(new backward::SignalHandling());
  return g_signal_handler->loaded() ? 1 : 0;
#else
  (void)g_signal_handler;
  return 0;
#endif
}

void backward_shutdown_signal_handlers(void) { g_signal_handler.reset(); }

void backward_print_stacktrace(FILE *fp) {
  if (!fp)
    fp = stderr;
  backward::StackTrace st;
  st.load_here(32);
  backward::Printer printer;
  printer.snippet = (g_snippet_enabled != 0);
  // color_mode expects backward::ColorMode::type
  if (g_color_mode <= 0)
    printer.color_mode = backward::ColorMode::automatic;
  else if (g_color_mode == 1)
    printer.color_mode = backward::ColorMode::never;
  else
    printer.color_mode = backward::ColorMode::always;
  printer.address = true;
  printer.print(st, fp);
}

void backward_print_stacktrace_from_addr(void *addr, FILE *fp) {
  if (!fp)
    fp = stderr;
  backward::StackTrace st;
  st.load_from(addr, 32);
  backward::Printer printer;
  printer.snippet = (g_snippet_enabled != 0);
  if (g_color_mode <= 0)
    printer.color_mode = backward::ColorMode::automatic;
  else if (g_color_mode == 1)
    printer.color_mode = backward::ColorMode::never;
  else
    printer.color_mode = backward::ColorMode::always;
  printer.address = true;
  printer.print(st, fp);
}

void backward_add_source_prefix(const char *prefix) {
  if (!prefix)
    return;
  backward::SourceFile::add_paths_to_env_variable_impl(std::string(prefix));
}

void backward_set_snippet(int enabled) { g_snippet_enabled = enabled ? 1 : 0; }

void backward_set_color_mode(int mode) {
  if (mode < 0)
    mode = 0;
  if (mode > 2)
    mode = 2;
  g_color_mode = mode;
}

} // extern "C"
