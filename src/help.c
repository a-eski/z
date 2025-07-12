/* Copyright z (C) by Alex Eski 2025 */
/* This project is licensed under GNU GPLv3 */

#include "ecolors.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NCSH_TITLE "z: version 1.0\n"
#define NCSH_COPYRIGHT                                                                                                 \
    "Copyright (C) 2025 Alex Eski\n"                                                                                   \
    "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>. "                                 \
    "This program comes with ABSOLUTELY NO WARRANTY.\n"                                                                \
    "This is free software, and you are welcome to redistribute it "                                                   \
    "under certain conditions.\n\n"

#define HELP_MESSAGE "z help\n\n"
#define HELP_FORMAT "Commands: z {command} {args}\n\n"
#define HELP_Z                                                                                                         \
    "z {directory}:            A builtin autojump/z command. An enhanced cd command that keeps track of history and "  \
    "fuzzy matches against previously visited directories.\n\n"
#define HELP_Z_ADD "z add {directory}:        Manually add a directory to your z database.\n\n"
#define HELP_Z_RM                                                                                                      \
    "z rm {directory}:         Manually remove a directory from your z database. Can also call using 'z remove "       \
    "{directory}'.\n\n"
#define HELP_Z_PRINT "z print:                  Print out information about the entries in your z database.\n\n"

#define HELP_WRITE(str)                                                                                                \
    constexpr size_t str##_len = sizeof(str) - 1;                                                                      \
    if (write(STDOUT_FILENO, str, str##_len) == -1) {                                                          \
        perror(RED "z: error with writing help output." RESET);                                                                           \
        return EXIT_FAILURE;                                                                                           \
    }

[[nodiscard]]
int z_help()
{
    HELP_WRITE(HELP_Z);
    HELP_WRITE(HELP_Z_ADD);
    HELP_WRITE(HELP_Z_RM);
    HELP_WRITE(HELP_Z_PRINT);
    fflush(stdout);
    return EXIT_SUCCESS;
}
