#include "histotrie.h"
#include "board.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct Histotrie {

	struct Histotrie* children[SYMBOL_COUNT];
};

#define HISTOTRIE_TEST 0
#if HISTOTRIE_TEST
#include <stdio.h>
void test_histotrie() {

	const Count n = 1000;
	struct Histotrie* root = malloc_init_histotrie();
	srand((unsigned int)time(NULL));
	char keeper[BOARD_LENGTH];
	init_board(keeper);
	size_t total = 0;
	size_t result = record_state(root, keeper, 0);
	total += result;
	printf("%zu nodes added\n", result);
	assert(result == RANKS * FILES);
	size_t since = 0, until = 0;
	for (Count b = 0; b < n; ++b) {
		char board[BOARD_LENGTH];
		init_board(board);
		for (Index r = 0; r < RANKS; ++r) {
			for (Index f = 0; f < FILES; ++f) {
				board[SQUARE_INDEX(r, f)] = SYMBOLS[rand() % RANKS > r ? SYMBOL_COUNT - 1 : rand() % SYMBOL_COUNT];
			}
		}
		printf("%s\n", board);
		result = record_state(root, board, 0);
		total += result;
		printf("%zu nodes added\n", result);
		assert(result > 0);
		if (since >= until) {
			until = rand() % 8;
			since = 0;
			result = record_state(root, keeper, 0);
			total += result;
			ERRLOGF("%zu nodes added\n", result);
			assert(result == 0);
			memcpy(keeper, board, BOARD_LENGTH);
		}
		else {
			++since;
		}
	}
	printf("%zu total nodes\n", total);
	free_histotrie(root);
}
#else
void test_histotrie() {}
#endif

Count _free_children(struct Histotrie* root) {

	if (!root) return 0;
	Count ret = 1;

	for (Index c = 0; c < SYMBOL_COUNT; ++c) {

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

	Count count = _free_children(histotrie);
	MEMLOGF("freed %llu histotrie nodes\n", count);
}

Count record_state(struct Histotrie* root, const char* board, const Index index) {

	if (index >= BOARD_LENGTH || !board[index]) return 0;
	if (board[index] == '\n') return record_state(root, board, index + 1);

	const Piece piece = board[index];
	const enum Square square = PIECE_MAP[piece];
	const Index symbol_index = SYMBOL_INDEX(piece);
	assert(symbol_index < SYMBOL_COUNT);

	if (root->children[symbol_index]) {

		return record_state(root->children[symbol_index], board, index + 1);
	}

	root->children[symbol_index] = malloc(sizeof(struct Histotrie));
	MEMLOG("malloc histotrie node\n");
	_init_histotrie(root->children[symbol_index]);
	return 1 + record_state(root->children[symbol_index], board, index + 1);
}

struct Histotrie* malloc_init_histotrie() {
	struct Histotrie* ret = malloc(sizeof(struct Histotrie));
	MEMLOG("malloc histotrie\n");
	assert(ret);
	_init_histotrie(ret);
	return ret;
}