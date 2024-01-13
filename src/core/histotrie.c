#include "histotrie.h"
#include "game.h"
#include "config.h"
#include <string.h>

#define CHILD_INDEX(P) (PIECE_MAP[P] == NO_PIECE ? TRIE_CHILDREN - 1 : PIECE_MAP[P] + NO_PIECE * IS_WHITE(P))

size_t _free_children(struct Histotrie* root) {

	if (!root) return 0;
	size_t ret = 1;

	for (size_t c = 0; c < TRIE_CHILDREN; ++c) {

		if (root->children[c]) {

			ret += _free_children(root->children[c]);
		}
	}

	free(root);
	MEMLOG("freed histotrie node\n");
	return ret;
}

void _init_histotrie(struct Histotrie* histotrie) {

	memset(histotrie->children, 0, TRIE_CHILDREN * sizeof(struct Histotrie*));
}

void free_histotrie(struct Histotrie* histotrie) {

	size_t count = _free_children(histotrie);
	MEMLOGF("freed %llu histotrie nodes\n", count);
}

size_t record_state(struct Histotrie* root, const char* board, const size_t index) {

	if (index >= BOARD_LENGTH) return 0;
	if (board[index] == '\n') return record_state(root, board, index + 1);

	Piece piece = board[index];
	enum Square square = PIECE_MAP[piece];
	size_t child = square == NO_PIECE ? TRIE_CHILDREN - 1 : square + NO_PIECE * IS_WHITE(piece);

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