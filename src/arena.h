/* Copyright eskilib (C) by Alex Eski 2025 */
/* arena.h: a simple bump allocator for managing memory */
/* Credit to skeeto and his blogs */

#pragma once

#include <stdint.h>
#include <sys/cdefs.h>

typedef struct {
    char* start;
    char* end;
} Arena;

#define arena_malloc(arena, count, type) (type*)arena_malloc_internal(arena, count, sizeof(type), _Alignof(type))

void* arena_malloc_internal(Arena* restrict arena, uintptr_t count, uintptr_t size,
                            uintptr_t alignment) __attribute__((malloc));

#define arena_realloc(arena, count, type, ptr, old_count)                                                              \
    (type*)arena_realloc_internal(arena, count, sizeof(type), _Alignof(type), ptr, old_count)

void* arena_realloc_internal(Arena* restrict arena, uintptr_t count, uintptr_t size, uintptr_t alignment, void* old_ptr,
                             uintptr_t old_count) __attribute__((malloc));
