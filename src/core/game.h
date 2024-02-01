#pragma once
#include "def.h"
#include "board.h"

#define EASE_FUNCTOR(__var__) Time (__var__)(const struct Game* game)

void print_rules();
Time ease(const struct Game*);
Time ended(const struct Game*);
const Piece deck(const struct Game*, const Index, const Index);
const Board board_state(const struct Game*);
Index board_length();
Piece square_contents(const struct Game*, Index, Index);
Set square_bit(const Index, const Index);
struct Game* malloc_init_game(const Settings);
struct Game* malloc_init_standard_game_with_ease_functor(EASE_FUNCTOR(ease_func));
void free_game(struct Game*);
void begin(struct Game*);
void print_board_state(const struct Game*);
void pump(struct Game* game, const Time passed);
Piece next_piece(const struct Game*);
Index forecast_rank(const struct Game*);
char forecast_piece(const struct Game*);
Set attack(struct Game*, const Bool, const Bool, const Bool);
Bool cursor_wrapped(const struct Game*);
Bool game_over(const struct Game*);
Bool on_brink(struct Game*);
void toggle_pause(struct Game*);
Bool paused(const struct Game*);
Bool repeated(const struct Game*);
void do_drop(struct Game*);
void do_digital_move(struct Game*, const Bool, const Bool, const Bool);
void do_analog_move(struct Game*, const Fraction, const Fraction);
Events current_events(const struct Game*);
Piece player_piece(const struct Game*);
Index player_piece_rank(const struct Game*);
Index player_piece_file(const struct Game*);
Trenary cursor_direction(const struct Game*);
Index current_cursor_grade(const struct Game*);
Index current_cursor_increment(const struct Game*);
Count current_score(const struct Game*);
Count last_scored(const struct Game*);
Count current_combo(const struct Game*);
Time milliseconds(const struct Game*);
Time move_rate(const struct Game*);
Index spawn_rank(const struct Game*);
Count falls(const struct Game*);
Time time_taken(const struct Game*);