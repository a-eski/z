#pragma once

#if defined(__linux__) && !defined(__MSYS2__)
    #include <linux/limits.h>
#else
#ifndef PATH_MAX
#define     PATH_MAX 4028
#endif /* !PATH_MAX */
#ifndef MAX_INPUT
#define MAX_INPUT PATH_MAX
#endif /* !MAX_INPUT */
#endif /* __linux__ && !__MSYS2__ */


#include <sys/cdefs.h>

#if defined(__GNUC__) || defined(__clang__) || defined(__attribute__malloc__)
#   define ATTR_MALLOC __attribute_malloc__
#elif defined(__malloc_like)
#   define ATTR_MALLOC __malloc_like
#else
#   define ATTR_MALLOC
#endif


#if defined(__GNUC__) || defined(__clang__) || defined(__attribute__alloc_align__)
#   define ATTR_ALLOC_ALIGN(pos) __attribute_alloc_align__((pos))
#elif defined(__alloc_align)
#   define ATTR_ALLOC_ALIGN(pos) __alloc_align((pos))
#else
#   define ATTR_ALLOC_ALIGN(pos)
#endif


