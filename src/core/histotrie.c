#include "histotrie.h"
#include "game.h"
#include "config.h"
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>

#define HISTOTRIE_TEST 0
#if HISTOTRIE_TEST
void test_histotrie() {

	const size_t n = 1000;
	struct Histotrie* root = malloc_init_histotrie();
	srand((unsigned int)time(NULL));
	char keeper[BOARD_LENGTH];
	init_board(keeper);
	size_t result = record_state(root, keeper, 0);
	printf("%zu nodes added\n", result);
	assert(result == RANKS * FILES);
	size_t since = 0, until = 0;
	for (size_t b = 0; b < n; ++b) {
		char board[BOARD_LENGTH];
		init_board(board);
		for (size_t r = 0; r < RANKS; ++r) {
			for (size_t f = 0; f < FILES; ++f) {
				board[SQUARE_INDEX(r, f)] = SYMBOLS[rand() % SYMBOL_COUNT];
			}
		}
		printf("%s\n", board);
		result = record_state(root, board, 0);
		printf("%zu nodes added\n", result);
		assert(result > 0);
		if (since >= until) {
			until = rand() % 8;
			since = 0;
			result = record_state(root, keeper, 0);
			printf("%zu nodes added\n", result);
			assert(result == 0);
			memcpy(keeper, board, BOARD_LENGTH);
		}
		else {
			++since;
		}
	}
	free_histotrie(root);
}
#else
void test_histotrie() {}
#endif

size_t _free_children(struct Histotrie* root) {

	if (!root) return 0;
	size_t ret = 1;

	for (size_t c = 0; c < SYMBOL_COUNT; ++c) {

		if (root->children[c]) {

			ret += _free_children(root->children[c]);
		}
	}

	free(root);
	MEMLOG("freed histotrie node\n");
	return ret;
}

void _init_histotrie(struct Histotrie* histotrie) {

	memset(histotrie->children, 0, SYMBOL_COUNT * sizeof(struct Histotrie*));
}

void free_histotrie(struct Histotrie* histotrie) {

	size_t count = _free_children(histotrie);
	MEMLOGF("freed %llu histotrie nodes\n", count);
}

size_t record_state(struct Histotrie* root, const char* board, const size_t index) {

	if (index >= BOARD_LENGTH || !board[index]) return 0;
	if (board[index] == '\n') return record_state(root, board, index + 1);

	Piece piece = board[index];
	enum Square square = PIECE_MAP[piece];
	size_t child = square == SYMBOL_INDEX(piece);
	assert(child < SYMBOL_COUNT);

	if (root->children[child]) {

		return record_state(root->children[child], board, index + 1);
	}

	root->children[child] = malloc(sizeof(struct Histotrie));
	MEMLOG("malloc histotrie node\n");
	_init_histotrie(root->children[child]);
	return 1 + record_state(root->children[child], board, index + 1);
}

struct Histotrie* malloc_init_histotrie() {
	struct Histotrie* ret = malloc(sizeof(struct Histotrie));
	MEMLOG("malloc histotrie\n");
	_init_histotrie(ret);
	return ret;
}