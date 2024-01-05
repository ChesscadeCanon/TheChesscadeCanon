#pragma once

#ifdef __unix__                    /* __unix__ is usually defined by compilers targeting Unix systems */

#define OS_WINDOWS 0
#include <unistd.h>

#elif defined(_WIN32) || defined(WIN32)     /* _Win32 is usually defined by compilers targeting 32 or   64 bit Windows systems */

#define OS_WINDOWS 1
#include <windows.h>
#include <WinUser.h>

#endif

#include <stdbool.h>

#define DROP_KEY ' '
#define QUIT_KEY 'q'
#define PAUSE_KEY 'p'
#define LEFT_KEY 'A'
#define RIGHT_KEY 'D'
#define DOWN_KEY 'S'

struct Game;

/*struct InputBuffer {

	bool drop;
	long int down;
	long int left;
	long int right;
};*/

enum Input {
	NO_INPUT = -1,
	DROP_INPUT,
	DOWN_INPUT,
	LEFT_INPUT,
	RIGHT_INPUT,
};

void control_move(struct Game*, long int*, const char, const time_t);
void control_drop(struct Game* game);
void key_control(struct Game*, const time_t);

