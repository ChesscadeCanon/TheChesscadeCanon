#pragma once

extern "C" {
#include "../private/model.h"

    /**
    * returns the height of the board
    */
    __declspec(dllexport) size_t get_ranks();
    
    /**
    * returns the width of the board
    */
    __declspec(dllexport) size_t get_files();
    
    /**
    * returns the number of squares on the board, plus newlines
    */
    __declspec(dllexport) size_t get_state_length();
    
    /**
    * returns the branching factor of the trie structure used to identify repeated board states
    */
    __declspec(dllexport) size_t get_trie_children();
    
    /**
    * returns the next piece that will spawn if the current piece lands right now
    */
    __declspec(dllexport) Piece get_next_piece(struct Game*);
    
    /**
    * returns a bitflag denoting which squares on the board are occupied by pieces that would be captured if the current piece landed right now
    */
    __declspec(dllexport) size_t forecast_captures(struct Game*);

    /**
    * returns a bigflag denoting the attack pattern of the current falling piece
    */
    __declspec(dllexport) size_t attack_pattern(struct Game*);

    /**
    * returns the rank that the current piece would fall to if it fell right now
    */
    __declspec(dllexport) size_t get_forecast_rank(struct Game*);

    /**
    * returns the piece that the current falling piece would promote to if it fell right now
    */
    __declspec(dllexport) char get_forecast_piece(struct Game*);

    /**
    * gets the bit that represents the square at the given coordinates for purposes of finding it in a bitflag
    */
    __declspec(dllexport) size_t get_square_bit(size_t, size_t);

    /**
    * returns one of the four "decks" that new spawns are drawn from
    */
    __declspec(dllexport) const char* get_deck(size_t);

    /**
    * returns a pointer to a newly allocated and initialized Game struct
    */
    __declspec(dllexport) struct Game* malloc_init_default_game();
    
    /**
    * begins the game by spawning the first piece
    */
    __declspec(dllexport) void begin_game(struct Game*);

    /**
    * progresses the game by the given number of milliseconds
    */
    __declspec(dllexport) void increment_game(struct Game*, time_t);

    /**
    * returns whether the game has ended or not
    */
    __declspec(dllexport) bool is_game_over(struct Game*);

    /**
    * frees all memory allocated on behalf of the given game object
    */
    __declspec(dllexport) void delete_game(struct Game*);
}