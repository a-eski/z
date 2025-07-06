/* Copyright z by Alex Eski 2025 */

/* Used to build ncsh with a single translation unit, aka a unity build.
 * Try it out with 'make unity_debug' or 'make unity'.
 */

#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include "help.c"
#include "fzf.c"
#include "z.c"
#include "arena.c"
#include "main.c"
