#pragma once
#if defined(_MSC_VER)
//  Microsoft 
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
//  do nothing and hope for the best?
#define EXPORT
#define IMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif

extern "C" {
#include "game.h"

/**
* returns the full text of the rules of Chesscade
*/
EXPORT const char* get_rules();

/**
* returns the full text of the rules of Chesscade
*/
EXPORT size_t get_rules_length();

/**
* returns the height of the board
*/
EXPORT size_t get_ranks();
    
/**
* returns the width of the board
*/
EXPORT size_t get_files();
    
/**
* returns the number of squares on the board, plus newlines
*/
EXPORT size_t get_state_length();
    
/**
* returns the branching factor of the trie structure used to identify repeated board states
*/
EXPORT size_t get_trie_children();
    
/**
* returns the next piece that will spawn if the current piece lands right now
*/
EXPORT Piece get_next_piece(struct Game*);
    
/**
* returns a bitflag denoting which squares on the board are occupied by pieces that would be captured if the current piece landed right now
*/
EXPORT size_t forecast_captures(struct Game*);

/**
* returns a bigflag denoting the attack pattern of the current falling piece
*/
EXPORT size_t attack_pattern(struct Game*);

/**
* returns the rank that the current piece would fall to if it fell right now
*/
EXPORT size_t get_forecast_rank(struct Game*);

/**
* returns the piece that the current falling piece would promote to if it fell right now
*/
EXPORT char get_forecast_piece(struct Game*);

/**
* gets the bit that represents the square at the given coordinates for purposes of finding it in a bitflag
*/
EXPORT size_t get_square_bit(size_t, size_t);

/**
* returns one of the four "decks" that new spawns are drawn from
*/
EXPORT const char* get_deck(size_t);

/**
* returns a pointer to a newly allocated and initialized Game struct
*/
EXPORT struct Game* malloc_init_default_game();
    
/**
* begins the game by spawning the first piece
*/
EXPORT void begin_game(struct Game*);

/**
* progresses the game by the given number of milliseconds
*/
EXPORT void increment_game(struct Game*, time_t);

/**
* returns whether the game has ended or not
*/
EXPORT bool is_game_over(struct Game*);

/**
* returns whether dropping the current piece now would result in a game over
*/
EXPORT bool is_on_brink(struct Game* game);

/**
* frees all memory allocated on behalf of the given game object
*/
EXPORT void delete_game(struct Game*);
}