/* Copyright z (C) by Alex Eski 2025 */
/* This project is licensed under GNU GPLv3 */

#include <assert.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ecolors.h"
#include "z.h"
#include "help.h"
#include "arena.h"

#define Z "z" // the base command, changes directory
#define Z_ADD "add"
#define Z_RM "rm"
#define Z_REMOVE "remove" // alias for rm
#define Z_PRINT "print"
#define Z_COUNT "count"
#define Z_HELP "help"

int z_(z_Database* restrict z_db, char** restrict buffer, size_t* restrict buf_lens, Arena* arena, Arena* restrict scratch);

#define Z_COMMAND_NOT_FOUND_MESSAGE "ncsh z: command not found, options not supported.\n"

[[nodiscard]]
int z_(z_Database* restrict z_db, char** restrict buffer, size_t* restrict buf_lens, Arena* restrict arena, Arena* restrict scratch)
{
    assert(z_db);
    assert(buffer && *buffer);
    assert(buf_lens && * buf_lens);

    if (buf_lens[1] == 0) {
        z(NULL, 0, NULL, z_db, arena, *scratch);
        return EXIT_SUCCESS;
    }

    assert(buffer && buffer + 1);

    // skip first position since we know it is 'z'
    char** arg = buffer + 1;
    size_t* arg_lens = buf_lens + 1;
    if (arg_lens[1] == 0) {
        assert(arg && *arg);

        // z print
        if (estrcmp(*arg, *arg_lens, Z_PRINT, sizeof(Z_PRINT))) {
            z_print(z_db);
            return EXIT_SUCCESS;
        }
        // z count
        if (estrcmp(*arg, *arg_lens, Z_COUNT, sizeof(Z_COUNT))) {
            z_count(z_db);
            return EXIT_SUCCESS;
        }

        // z
        char cwd[PATH_MAX] = {0};
        if (!getcwd(cwd, PATH_MAX)) {
            perror(RED "ncsh z: Could not load cwd information" RESET);
            return EXIT_FAILURE;
        }

        z(*arg, *arg_lens, cwd, z_db, arena, *scratch);
        return EXIT_SUCCESS;
    }

    if (arg && arg[1] && !arg[2]) {
        assert(arg && *arg);

        // z add
        if (estrcmp(*arg, *arg_lens, Z_ADD, sizeof(Z_ADD))) {
            assert(arg[1] && arg_lens[1]);
            if (z_add(arg[1], arg_lens[1], z_db, arena) != Z_SUCCESS) {
                return EXIT_FAILURE;
            }

            return EXIT_SUCCESS;
        }
        // z rm/remove
        else if (estrcmp(*arg, *arg_lens, Z_RM, sizeof(Z_RM)) || estrcmp(*arg, *arg_lens, Z_REMOVE, sizeof(Z_REMOVE))) {
            assert(arg[1] && arg_lens[1]);
            if (z_remove(arg[1], arg_lens[1], z_db) != Z_SUCCESS) {
                return EXIT_FAILURE;
            }

            return EXIT_SUCCESS;
        }
        else if (estrcmp(*arg, *arg_lens, Z_HELP, sizeof(Z_HELP))) {
            assert(arg[1] && arg_lens[1]);
            if (z_help()) {
                return EXIT_FAILURE;
            }

            return EXIT_SUCCESS;
        }
    }

    if (write(STDOUT_FILENO, Z_COMMAND_NOT_FOUND_MESSAGE, sizeof(Z_COMMAND_NOT_FOUND_MESSAGE) - 1) == -1) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char** argv)
{
    (void)argc; (void)argv;
    printf("in z!\n");
    return EXIT_SUCCESS;
}
