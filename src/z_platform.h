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
#endif /* __linux__ && !__MSYS@__ */

