#pragma once
#include "board.h"

struct Histotrie {

	struct Histotrie* children[SYMBOL_COUNT];
};

struct Histotrie* malloc_init_histotrie();
void free_histotrie(struct Histotrie*);
size_t record_state(struct Histotrie*, const char*, size_t);
void test_histotrie();