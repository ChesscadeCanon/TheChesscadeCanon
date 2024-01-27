#pragma once
#include "def.h"
#include "board.h"

void print_rules();
Time ease(struct Game*);
const char* deck(Index);
Board board_state(struct Game*);
Index board_length();
Piece square_contents(struct Game*, Index, Index);
Set square_bit(Index, Index);
struct Game* malloc_init_game(Settings);
void free_game(struct Game*);
void begin(struct Game*);
void print_board_state(struct Game*);
void pump(struct Game* game, const Time passed);
Piece next_piece(struct Game*);
Index forecast_rank(struct Game*);
char forecast_piece(struct Game*);
Set attack(struct Game*, const Bool, const Bool, const Bool);
Bool cursor_wrapped(struct Game*);
Bool game_over(struct Game*);
Bool on_brink(struct Game*);
void toggle_pause(struct Game*);
Bool paused(struct Game*);
Bool repeated(struct Game*);
void do_drop(struct Game*);
void do_digital_move(struct Game*, Bool, Bool, Bool);
void do_analog_move(struct Game*, Fraction, Fraction);
Events current_events(struct Game*);
Piece player_piece(struct Game*);
Index player_piece_rank(struct Game*);
Index player_piece_file(struct Game*);
Trenary cursor_direction(struct Game*);
Index current_cursor_grade(struct Game*);
Index current_cursor_increment(struct Game*);
Count current_score(struct Game*);
Count last_scored(struct Game*);
Count current_combo(struct Game*);
Time milliseconds(struct Game*);