#pragma once
#include <sys/timeb.h>
#include <string.h>
#include <stdlib.h>

#ifdef __unix__                    /* __unix__ is usually defined by compilers targeting Unix systems */

#define OS_WINDOWS 0

#include <unistd.h>
#include <ncurses.h>
#define max(A, B) (A > B ? A : B)
#define min(A, B) (A < B ? A : B)

#elif defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */

#define OS_WINDOWS 1
#include <windows.h>
#include <WinUser.h>
#include <conio.h>
#endif

#define VIEW_FUNCTOR(VAR) void (*VAR)(struct Game*)
#define MODEL_FUNCTOR(VAR) void (*VAR)(struct Game*, const time_t)
#define CONTROL_FUNCTOR(VAR) void (*VAR)(struct Game*, const time_t)

struct InputBuffer;
