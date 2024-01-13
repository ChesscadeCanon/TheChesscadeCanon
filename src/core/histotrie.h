#pragma once
#include "board.h"

#define TRIE_CHILDREN 13

struct Histotrie {

	struct Histotrie* children[TRIE_CHILDREN];
};

struct Histotrie* malloc_init_histotrie();
void free_histotrie(struct Histotrie*);
size_t record_state(struct Histotrie*, const char*, size_t);