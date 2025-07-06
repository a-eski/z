#pragma once

#if defined(__linux__) && !defined(__MSYS2__)
    #include <linux/limits.h>
#else
#ifndef PATH_MAX
#define     PATH_MAX 4028
#endif /* !PATH_MAX */
#endif /* __linux__ && !__MSYS@__ */

