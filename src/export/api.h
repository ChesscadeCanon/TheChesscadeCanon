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
#include "def.h"

/**
* pauses the game if it's unpaused and vice versa
*/
EXPORT void input_toggle_pause(struct Game*);

/**
* drops the current piece
*/
EXPORT void input_drop(struct Game*);

/**
* On any given frame, call input_digital_move() with boolean values denoting whether
* to try and move the piece left, right and/or down, as indicated by key inputs. For
* instance, if the right and down keys were being held, you would call:
* input_digital_move(game, false, true, true);
*/
EXPORT void input_digital_move(struct Game*, bool, bool, bool);

/**
* On any given frame, call input_analog_move() with floating point values
* constituting a vector denoting how far the mouse or touch has moved on the x and y
* axes in terms of what fraction of a square width it has moved. For instance, if
* the squares were 64 pixels across in your interface, and on some frame the player
* dragged the mouse 96 pixels to the left and 16 pixels down, you would call:
* input_analog_move(game, -1.5, 0.25);
*/
EXPORT void input_analog_move(struct Game*, long double, long double);

/**
* returns a bitset denoting what events took place in the last frame
*/
EXPORT Events get_events(struct Game*);

/**
* returns the full text of the rules of Chesscade
*/
EXPORT const char* get_rules();

/**
* returns the full text of the rules of Chesscade
*/
EXPORT size_t get_rules_length();

/**
* returns the number of milliseconds it currently takes for the piece to fall
*/
EXPORT size_t get_ease(struct Game* game);

/**
* returns the height of the board
*/
EXPORT Index get_ranks();
    
/**
* returns the width of the board
*/
EXPORT Index get_files();

/**
* seturns the current state of the board as a string
*/
EXPORT Piece get_square(struct Game* game, Index, Index);
    
/**
* returns the branching factor of the trie structure used to identify repeated board states
*/
EXPORT size_t get_symbol_count();
    
/**
* returns the next piece that will spawn if the current piece lands right now
*/
EXPORT Piece get_next_piece(struct Game*);

/**
* returns a character denoting the type of the current falling piece
*/
EXPORT Piece get_player(struct Game*);

/**
* returns an unsigned short denoting the rank of the board where the falling piece currently is
*/
EXPORT Index get_player_rank(struct Game*);

/**
* returns an unsigned short denoting the file of the board where the falling piece currently is
*/
EXPORT Index get_player_file(struct Game*);

/**
* returns an unsigned short denoting the rank of the board where the falling piece currently is
*/
EXPORT Index get_cursor_grade(struct Game*);

/**
* returns an unsigned short denoting the file of the board where the falling piece currently is
*/
EXPORT Index get_cursor_increment(struct Game*);

/**
* returns the current score
*/
EXPORT size_t get_score(struct Game*);

/**
* returns the number of points scored on the last landing
*/
EXPORT size_t get_scored(struct Game*);

/**
* returns the number of consecutive captures
*/
EXPORT size_t get_combo(struct Game*);

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
EXPORT size_t get_square_bit(Index, Index);

/**
* returns one of the four "decks" that new spawns are drawn from
*/
EXPORT const char* get_deck(Index);

/**
* returns a pointer to a newly allocated and initialized Game struct
*/
EXPORT struct Game* malloc_init_default_game();

/**
* returns a pointer to a newly allocated and initialized Game struct
*/
EXPORT struct Game* malloc_init_custom_game(Settings);
    
/**
* begins the game by spawning the first piece
*/
EXPORT void begin_game(struct Game*);

/**
* progresses the game by the given number of milliseconds
*/
EXPORT void pump_game(struct Game*, time_t);

/**
* returns whether the game has ended or not
*/
EXPORT bool is_game_over(struct Game*);

/**
* returns whether dropping the current piece now would result in a game over
*/
EXPORT bool is_on_brink(struct Game* game);

/**
* returns whether not the game is paused
*/
EXPORT bool is_paused(struct Game* game);

/**
* returns whether not the game is paused
*/
EXPORT bool is_repeat(struct Game* game);

/**
* frees all memory allocated on behalf of the given game object
*/
EXPORT void delete_game(struct Game*);
}