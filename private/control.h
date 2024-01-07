#pragma once

#include <sys/timeb.h>

#define DROP_KEY ' '
#define QUIT_KEY 'q'
#define PAUSE_KEY 'p'
#define LEFT_KEY 'A'
#define RIGHT_KEY 'D'
#define DOWN_KEY 'S'

struct Game;

enum Input {
	NO_INPUT = -1,
	DROP_INPUT,
	DOWN_INPUT,
	LEFT_INPUT,
	RIGHT_INPUT,
};

void key_control(struct Game*, const time_t);

