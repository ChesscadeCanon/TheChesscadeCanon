#pragma once

typedef size_t NOTAGAME;

extern "C" {
#include "../private/model.h"
    __declspec(dllexport) size_t get_ranks();
    __declspec(dllexport) size_t get_files();
    __declspec(dllexport) size_t get_state_length(); 
    __declspec(dllexport) size_t get_trie_children();
    __declspec(dllexport) Piece get_player(NOTAGAME);
    __declspec(dllexport) size_t get_player_rank(NOTAGAME);
    __declspec(dllexport) size_t get_player_file(NOTAGAME);
    __declspec(dllexport) Piece get_cursor(NOTAGAME);
    __declspec(dllexport) size_t get_cursor_rank(NOTAGAME);
    __declspec(dllexport) size_t get_cursor_file(NOTAGAME);
    __declspec(dllexport) Piece get_next_piece(NOTAGAME);
    __declspec(dllexport) size_t forecast_captures(NOTAGAME);
    __declspec(dllexport) size_t get_square_bit(size_t, size_t);
    __declspec(dllexport) const char* get_deck();
    __declspec(dllexport) struct Game* malloc_init_default_game();
    __declspec(dllexport) void begin_game(NOTAGAME);
    __declspec(dllexport) void increment_game(NOTAGAME, time_t);
    __declspec(dllexport) bool game_over(NOTAGAME);
    __declspec(dllexport) void delete_game(NOTAGAME);
    __declspec(dllexport) void test();
}