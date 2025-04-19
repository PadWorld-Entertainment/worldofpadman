/**
 * @file
 * @brief C wrapper for backward.hpp
 * Provides a small C ABI so C code can print
 * stacktraces using backward-cpp.
 */
#ifndef BACKWARD_C_H
#define BACKWARD_C_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// Initialize signal handlers (returns 1 on success, 0 on failure)
int backward_init_signal_handlers(void);

// Shutdown installed signal handlers (no-op if not installed)
void backward_shutdown_signal_handlers(void);

// Print current stacktrace to given FILE* (use stderr for default)
void backward_print_stacktrace(FILE *fp);

// Print stacktrace starting from an address to FILE* (use stderr for default)
void backward_print_stacktrace_from_addr(void *addr, FILE *fp);

// Add a source prefix (used to locate source files). Can be called multiple
// times.
void backward_add_source_prefix(const char *prefix);

// Enable/disable printing source snippets (1=on, 0=off)
void backward_set_snippet(int enabled);

// Set color mode: 0=automatic, 1=never, 2=always
void backward_set_color_mode(int mode);

#ifdef __cplusplus
}
#endif

#endif // BACKWARD_C_H
