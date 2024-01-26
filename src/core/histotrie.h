#pragma once
#include "board.h"

struct Histotrie* malloc_init_histotrie();
void free_histotrie(struct Histotrie*);
Count record_state(struct Histotrie*, const char*, Index);
void test_histotrie();