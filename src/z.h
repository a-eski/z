/* Copyright z (C) by Alex Eski 2024 */
/* z: a better cd command */
/* idea credited to autojump/z/z-oxide and many others */
/* This project is licensed under GNU GPLv3 */

#pragma once
#ifndef Z_H_
#define Z_H_

#include <time.h>

#include "arena.h"
#include "str.h"

#define Z_DATABASE_FILE "_z_database.bin"
#define Z_DATABASE_IN_MEMORY_LIMIT 200

#define Z_SECOND 1
#define Z_MINUTE 60 * Z_SECOND
#define Z_HOUR 60 * Z_MINUTE
#define Z_DAY 24 * Z_HOUR
#define Z_WEEK 7 * Z_DAY
#define Z_MONTH 30 * Z_DAY

typedef struct {
    double rank;
    time_t last_accessed;
    char* path;
    size_t path_length;
} z_Directory;

typedef struct {
    double z_score;
    z_Directory* dir;
} z_Match;

typedef struct {
    // bool dirty;
    size_t count;
    char* database_file;
    z_Directory dirs[Z_DATABASE_IN_MEMORY_LIMIT];
} z_Database;

enum z_Result {
    Z_CANNOT_PROCESS = -10,
    Z_HIT_MEMORY_LIMIT = -9,
    Z_BAD_STRING = -8,
    Z_FILE_LENGTH_TOO_LARGE = -7,
    Z_MATCH_NOT_FOUND = -6,
    Z_NULL_REFERENCE = -5,
    Z_STDIO_ERROR = -4,
    Z_MALLOC_ERROR = -3,
    Z_ZERO_BYTES_READ = -2,
    Z_FILE_ERROR = -1,
    Z_FAILURE = 0,
    Z_SUCCESS = 1
};

enum z_Result z_init(Str* restrict path, z_Database* restrict db, Arena* restrict arena);

void z(char* restrict target, size_t target_length, char* restrict cwd, z_Database* restrict db, Arena* restrict arena,
       Arena scratch_arena);

enum z_Result z_add(char* restrict path, size_t path_length, z_Database* restrict db, Arena* restrict arena);

enum z_Result z_remove(char* restrict path, size_t path_length, z_Database* restrict db);

enum z_Result z_exit(z_Database* restrict db);

void z_print(z_Database* restrict db);

void z_count(z_Database* restrict db);

#endif // !Z_H_
