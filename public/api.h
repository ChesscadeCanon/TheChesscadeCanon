#pragma once

extern "C" {
#include "../private/model.h"
    __declspec(dllexport) struct Game* test(struct Game*);
    __declspec(dllexport) size_t get_ranks();
    __declspec(dllexport) size_t get_files();
    __declspec(dllexport) size_t get_state_length(); 
    __declspec(dllexport) size_t get_trie_children();
    __declspec(dllexport) Piece get_player(struct Game*);
    __declspec(dllexport) size_t get_player_rank(struct Game*);
    __declspec(dllexport) size_t get_player_file(struct Game*);
    __declspec(dllexport) Piece get_cursor(struct Game*);
    __declspec(dllexport) size_t get_cursor_rank(struct Game*);
    __declspec(dllexport) size_t get_cursor_file(struct Game*);
    __declspec(dllexport) Piece get_next_piece(struct Game*);
    __declspec(dllexport) size_t forecast_captures(struct Game*);
    __declspec(dllexport) size_t attack_pattern(struct Game*);
    __declspec(dllexport) size_t get_forecast_rank(struct Game*);
    __declspec(dllexport) char get_forecast_piece(struct Game*);
    __declspec(dllexport) size_t get_square_bit(size_t, size_t);
    __declspec(dllexport) const char* get_deck(struct Game*, size_t);
    __declspec(dllexport) struct Game* malloc_init_default_game();
    __declspec(dllexport) void begin_game(struct Game*);
    __declspec(dllexport) void increment_game(struct Game*, time_t);
    __declspec(dllexport) bool game_over(struct Game*);
    __declspec(dllexport) void delete_game(struct Game*);
}